"""Writes config/functions.csv: every function in the retail XBE's code, with
who owns it (game, an Xbox SDK library, or third-party code), how it was
compiled (for speed or for size), its name where known, and what it calls.
Rerunning keeps each row's source and status.

    python tools/inventory.py [--xbe orig/default.xbe] [--xdk sdk/xbox]
                              --atlas <halo-symbol-atlas jsonl> [--out config/functions.csv]

Names come from halo-symbol-atlas (CC BY 4.0).
"""
import argparse
import csv
import hashlib
import json
import os
import re
from collections import Counter

from capstone import CS_ARCH_X86, CS_MODE_32, Cs

import libsig
from functions import discover
from xbe import FUNCTIONS_CSV, Xbe, retail_xbe_path, xdk_dir

RETAIL_SHA256 = '03215919bb7163259257d361f4c7bf802a7ab12aa85e2689436369b5c427935d'
COLUMNS = ['va', 'size', 'owner', 'style', 'evidence', 'name', 'object', 'calls', 'source', 'status']
# code sections (XBE section flags mark data sections executable too, so go by name)
CODE_SECTIONS = {'.text', 'D3D', 'XPP', 'DSOUND', 'WMADEC', 'XONLINE', 'XNET'}
SECTION_OWNERS = {'D3D': 'xdk:d3d8', 'XPP': 'xdk:xapi', 'DSOUND': 'xdk:dsound', 'WMADEC': 'xdk:wmadec',
                  'XONLINE': 'xdk:xonline', 'XNET': 'xdk:xnet'}
XAPI_OBJECTS = {'bootutil.obj', 'heap.obj', 'contsig.obj', 'support.obj'}
ZLIB_OBJECTS = {'deflate.obj', 'trees.obj', 'inflate.obj', 'inftrees.obj', 'inffast.obj', 'adler32.obj'}


def check_retail(path):
    with open(path, 'rb') as f:
        digest = hashlib.sha256(f.read()).hexdigest()
    if digest != RETAIL_SHA256:
        raise SystemExit(f'{path} is not the retail XBE this project matches '
                         f'(sha256 {digest}, expected {RETAIL_SHA256})')


def load_atlas(path):
    names = {}
    with open(path, encoding='utf-8') as f:
        for line in f:
            row = json.loads(line)
            if 'off' in row:
                names[int(row['off'], 16)] = (row['name'], row.get('lib', ''))
    return names


def is_havok(atlas_entry):
    name, lib = atlas_entry
    return lib.split(':')[-1].strip().startswith('hk') or '@hk' in name


# atlas library tags (the text before the ':', lowercased, without a leading "i ")
# by prefix; Bungie's own libraries (blamlib..., interfacelib..., none) stay game
LIBRARY_OWNERS = (('libcmt', 'xdk:libcmt'), ('libcpmt', 'xdk:libcpmt'), ('xvoice', 'xdk:xvoice'),
                  ('bink', 'third:bink'), ('xapilib', 'xdk:xapilib'), ('dsound', 'xdk:dsound'),
                  ('xonline', 'xdk:xonline'), ('xnet', 'xdk:xnet'), ('d3d8', 'xdk:d3d8'),
                  ('d3dx', 'xdk:d3dx'), ('xgraph', 'xdk:xgraphics'), ('xavd', 'xdk:xavd'),
                  ('rockall', 'xdk:rockall'))


def library_owner(lib):
    """The owner an atlas library tag names, else None."""
    if ':' not in lib:
        return None
    prefix = lib.split(':')[0].strip().lower()
    prefix = prefix[2:] if prefix.startswith('i ') else prefix
    return next((who for name, who in LIBRARY_OWNERS if prefix.startswith(name)), None)


def atlas_object(atlas_entry):
    return atlas_entry[1].split(':')[-1].strip() if atlas_entry else ''


def owner(start, section, lib_hit, atlas_entry):
    """The owner the section, a library signature or the atlas decides, else None."""
    if section.startswith('BINK'):
        return 'third:bink'
    if section != '.text':
        return SECTION_OWNERS.get(section, 'xdk:' + section.lower())
    if lib_hit:
        return 'xdk:' + lib_hit
    if atlas_entry:
        obj = atlas_object(atlas_entry)
        if is_havok(atlas_entry):
            return 'third:havok'
        if library_owner(atlas_entry[1]):
            return library_owner(atlas_entry[1])
        if obj in XAPI_OBJECTS:
            return 'xdk:xapi'
        if obj in ZLIB_OBJECTS:
            return 'xdk:d3dx'
        return 'game'
    return None


EH_MAX_SIZE = 32
MIN_HANDLER_THUNKS = 8


def _jmp_target(instructions):
    """The immediate target of a trailing jmp in a stub of at most EH_MAX_SIZE bytes, else None."""
    if not 2 <= len(instructions) <= 6 or sum(i.size for i in instructions) > EH_MAX_SIZE:
        return None
    last = instructions[-1]
    if last.mnemonic != 'jmp' or not last.op_str.startswith('0x'):
        return None
    if any(i.mnemonic.startswith('ret') or (i.mnemonic == 'push' and i.op_str == 'ebp') for i in instructions):
        return None
    return int(last.op_str, 16)


def eh_thunk_target(instructions):
    """T for exactly `mov eax, imm32; jmp T` (a candidate __ehhandler thunk), else None."""
    target = _jmp_target(instructions)
    if (target is not None and len(instructions) == 2 and instructions[0].mnemonic == 'mov'
            and re.fullmatch(r'eax, 0x[0-9a-f]+', instructions[0].op_str)):
        return target
    return None


def is_eh_stub(instructions, handlers):
    """True for the compiler's exception-handling stubs, which belong to a parent
    function: an __ehhandler thunk (`mov eax, imm32; jmp T`, T in handlers) or an
    unwind funclet (starts with an ebp-relative lea/mov, then lea/mov/add on
    ecx/eax only, then jmp imm). `add ecx, N; jmp` (a this-adjustor) is not one."""
    if eh_thunk_target(instructions) in handlers:
        return True
    if _jmp_target(instructions) is None:
        return False
    body = instructions[:-1]
    first = body[0]
    if first.mnemonic not in ('lea', 'mov') or '[ebp' not in first.op_str:
        return False
    return all(i.mnemonic in ('lea', 'mov', 'add') and i.op_str.split(',')[0] in ('ecx', 'eax') for i in body)


def fill_from_neighbours(rows):
    """Settles rows whose owner is None (rows in address order): a gap between two
    decided rows with the same non-game owner takes it, anything else is game.
    'eh' rows are left alone and are transparent when looking for neighbours."""
    decided = [None if r['owner'] == 'eh' else r['owner'] for r in rows]
    following = [None] * len(rows)
    nxt = None
    for i in range(len(rows) - 1, -1, -1):
        following[i] = nxt
        nxt = decided[i] if decided[i] is not None else nxt
    prev = None
    for i, row in enumerate(rows):
        if decided[i] is not None:
            prev = decided[i]
        elif row['owner'] is None:
            row['owner'] = prev if prev and prev == following[i] and prev != 'game' else 'game'


def style(before, fn, first):
    """('speed' | 'size' | 'unknown', evidence). before: the bytes just before
    the function; first: its first two instructions."""
    evidence = []
    if fn.start % 16 == 0:
        evidence.append('a16')
    if before[-1:] == b'\xcc':
        evidence.append('pad')
    if (len(first) >= 2 and first[0].mnemonic == 'push' and first[0].op_str == 'ebp'
            and first[1].mnemonic == 'mov' and first[1].op_str == 'ebp, esp'):
        evidence.append('ebp')
    if 'a16' not in evidence and 'pad' not in evidence:
        evidence.append('packed')
    if 'a16' in evidence and 'pad' in evidence:
        kind = 'speed'
    elif 'packed' in evidence:
        kind = 'size'
    else:
        kind = 'unknown'
    return kind, ' '.join(evidence)


def read_rows(path):
    if not os.path.exists(path):
        return {}
    with open(path, newline='', encoding='utf-8') as f:
        return {int(r['va'], 16): r for r in csv.DictReader(f)}


def write_rows(path, rows):
    os.makedirs(os.path.dirname(path) or '.', exist_ok=True)
    with open(path, 'w', newline='', encoding='utf-8') as f:
        w = csv.DictWriter(f, COLUMNS, lineterminator='\n')
        w.writeheader()
        w.writerows(rows)


def check_unique(rows):
    seen = set()
    for row in rows:
        if row['va'] in seen:
            raise SystemExit(f"duplicate inventory row for {row['va']}")
        seen.add(row['va'])


def merge(new_rows, old_rows):
    for row in new_rows:
        old = old_rows.get(int(row['va'], 16))
        if old:
            row['source'], row['status'] = old['source'], old['status']
    return new_rows


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument('--xbe', default=retail_xbe_path())
    ap.add_argument('--xdk', default=xdk_dir())
    ap.add_argument('--atlas', required=True)
    ap.add_argument('--out', default=FUNCTIONS_CSV)
    args = ap.parse_args()

    check_retail(args.xbe)
    image = Xbe(args.xbe)
    atlas = load_atlas(args.atlas)
    libraries = [os.path.join(args.xdk, 'lib', name + '.lib') for name in libsig.COFF_LIBRARIES]
    if not any(os.path.exists(p) for p in libraries):
        raise SystemExit(f'no Xbox SDK libraries found (looked for {libraries[0]} and others); '
                         'set XDK_DIR or pass --xdk')
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    md.detail = True

    rows = []
    stubs = {}
    for section in image.sections:
        if section.name not in CODE_SECTIONS and not (section.name.startswith('BINK') and section.name != 'BINKDATA'):
            continue
        found = discover(image, seeds=atlas, text=section)
        lib_hits = {}
        if section.name == '.text':
            lib_hits = libsig.find_in_dir(os.path.join(args.xdk, 'lib'), image.section_bytes(section), section.va)
        for fn in found.values():
            before = image.read(fn.start - 1, 1) if fn.start > section.va else b''
            first = list(md.disasm(image.read(fn.start, 16), fn.start, 2))
            kind, evidence = style(before, fn, first)
            entry = atlas.get(fn.start)
            who = owner(fn.start, section.name, lib_hits.get(fn.start), entry)
            if who is None and fn.end - fn.start <= EH_MAX_SIZE:
                stubs[fn.start] = list(md.disasm(image.read(fn.start, fn.end - fn.start), fn.start))
            rows.append(dict(
                va=f'{fn.start:08x}', size=str(fn.end - fn.start), owner=who,
                style=kind, evidence=evidence, name=entry[0] if entry else '', object=atlas_object(entry),
                calls=' '.join(f'{c:08x}' for c in sorted(fn.calls | fn.tail_jumps)),
                source='', status='todo'))
    thunk_targets = Counter(t for t in map(eh_thunk_target, stubs.values()) if t is not None)
    handlers = {t for t, n in thunk_targets.items() if n >= MIN_HANDLER_THUNKS}
    for row in rows:
        if row['owner'] is None and is_eh_stub(stubs.get(int(row['va'], 16), []), handlers):
            row['owner'] = 'eh'
    rows.sort(key=lambda r: r['va'])
    check_unique(rows)
    fill_from_neighbours(rows)
    write_rows(args.out, merge(rows, read_rows(args.out)))
    counts = {}
    for r in rows:
        counts[r['owner']] = counts.get(r['owner'], 0) + 1
    print('frame handlers:', ' '.join(f'{h:08x}' for h in sorted(handlers)))
    print(f'{len(rows)} functions:', ', '.join(f'{k} {v}' for k, v in sorted(counts.items())))


if __name__ == '__main__':
    main()
