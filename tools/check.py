"""Builds the project (tools/build.py) and compares every function marked
"// @retail 0x..." with the retail XBE, byte for byte apart from address
fields. Our extent comes from the linker map, retail's from
config/functions.csv. The masked fields are exactly those our linker filled
in: base relocations (absolute) and the relative fields /MAPINFO:FIXUPS lists.
Each masked field must also be the same kind of field in retail: an absolute
one holds an address inside the retail image, a relative one leaves the
function.

Writes build/report.json, updates the status and source columns of
config/functions.csv, and prints a summary.

    python tools/check.py [<retail va> ...] [--no-build]
"""
import argparse
import bisect
import json
import os
import re
import sys

from capstone import CS_ARCH_X86, CS_MODE_32, Cs

import build
from inventory import check_retail, read_rows, write_rows
from linkmap import LinkMap, plain_name
from pe import Pe
from xbe import FUNCTIONS_CSV, ROOT, Xbe, retail_xbe_path

NEAR = 2  # differing instructions, at most, for "near"


def resolve(linkmap, marked):
    """The image symbol of a marked function, or None when the linker left it
    out (folded into an identical function, or unreferenced)."""
    hits = linkmap.find(marked.name)
    if not hits:
        return None
    if len(hits) > 1:
        raise SystemExit(f'{marked.path}: {marked.name} is ambiguous: ' + ', '.join(h.name for h in hits))
    return hits[0]


def extract(full, theirs):
    """The code to compare: exactly retail's length when the rest of our
    extent is fill, otherwise the whole extent minus trailing fill."""
    if len(full) >= len(theirs) and set(full[len(theirs):]) <= {0xCC}:
        return full[:len(theirs)]
    return full.rstrip(b'\xcc')


def field_starts(va, size, fixups):
    """Offsets of the 4-byte fields lying wholly inside [va, va+size)."""
    return {f - va for f in fixups if va <= f and f + 4 <= va + size}


def masked_offsets(va, size, fixups):
    return {k + i for k in field_starts(va, size, fixups) for i in range(4)}


def straddling_offsets(va, size, fixups):
    """Offsets covered by fields that cross an edge of the function."""
    offsets = set()
    for f in fixups:
        if f < va + size and f + 4 > va and not (va <= f and f + 4 <= va + size):
            offsets.update(k for k in range(f - va, f - va + 4) if 0 <= k < size)
    return offsets


def _dword(data, k, signed=False):
    return int.from_bytes(data[k:k + 4], 'little', signed=signed)


def relative_ok(ours, start, theirs, theirs_va, k):
    """Whether the relative field at offset k agrees with retail. A target
    inside our function must be inside retail's at the same offset; any other
    must leave retail's function."""
    mine = start + k + 4 + _dword(ours, k, signed=True)
    target = theirs_va + k + 4 + _dword(theirs, k, signed=True)
    if start <= mine < start + len(ours):
        return target - theirs_va == mine - start
    return not theirs_va <= (target & 0xFFFFFFFF) < theirs_va + len(theirs)


def same_callee(ours, start, theirs, theirs_va, k, identity):
    """Whether the relative field at offset k, if it leaves the function, reaches the same function as retail."""
    mine = (start + k + 4 + _dword(ours, k, signed=True)) & 0xFFFFFFFF
    target = (theirs_va + k + 4 + _dword(theirs, k, signed=True)) & 0xFFFFFFFF
    if identity is None or start <= mine < start + len(ours):
        return True
    return identity.ok(mine, target)


def absolute_ok(ours, start, theirs, theirs_va, k, lo, hi):
    """Whether the absolute field at offset k agrees with retail. When either
    side points inside its own function (a jump table entry) both must point
    at the same offset; otherwise retail's must be an address inside [lo, hi)."""
    mine, value = _dword(ours, k), _dword(theirs, k)
    if start <= mine < start + len(ours) or theirs_va <= value < theirs_va + len(theirs):
        return value - theirs_va == mine - start
    return lo <= value < hi


def compare(ours, theirs, masked):
    for k in range(min(len(ours), len(theirs))):
        if k not in masked and ours[k] != theirs[k]:
            return k
    return None if len(ours) == len(theirs) else min(len(ours), len(theirs))


def _text(ins):
    return f'{ins.mnemonic} {ins.op_str}' if ins else ''


def differing_instructions(ours, ours_va, theirs, theirs_va, masked, forced=frozenset()):
    """Returns (count, lines, complete). complete is False when the
    disassembly does not cover every byte of both sides."""
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    a = list(md.disasm(ours, ours_va))
    b = list(md.disasm(theirs, theirs_va))
    complete = sum(i.size for i in a) == len(ours) and sum(i.size for i in b) == len(theirs)
    lines, count = [], 0
    for k in range(max(len(a), len(b))):
        x, y = (a[k] if k < len(a) else None), (b[k] if k < len(b) else None)
        same = x is not None and y is not None and x.size == y.size and all(
            (x.address - ours_va + i) not in forced and
            ((x.address - ours_va + i) in masked or x.bytes[i] == y.bytes[i]) for i in range(x.size))
        count += not same
        lines.append(f'  {"  " if same else "!!"} '
                     f'{_text(x):<44} | {_text(y)}')
    return count, lines, complete


# Library routines exported under two names; the linker resolves both to one function
ALIASES = {'chkstk': 'alloca_probe', 'memmove': 'memcpy'}


class Identity:
    """Whether a call or jump to another function reaches the same function in
    our image and in retail. Our target is a map symbol, retail's a row of
    config/functions.csv; markers (@retail and @stub) tie the two together."""

    def __init__(self, linkmap, rows, markers):
        self.linkmap, self.rows = linkmap, rows
        self.address_of = {}  # our symbol's name -> the retail address its marker gives
        self.claimed = {m.retail for m in markers}
        for m in markers:
            hits = linkmap.find(m.name)
            if len(hits) == 1:
                self.address_of[hits[0].name] = m.retail

    def ok(self, ours, theirs):
        # several names when the linker folded identical functions into one
        symbols = self.linkmap.symbols_at(ours)
        unmarked = [s for s in symbols if s.name not in self.address_of]
        if any(self.address_of[s.name] == theirs for s in symbols if s.name in self.address_of):
            return True
        if symbols and not unmarked:
            return False
        if theirs in self.claimed:
            return False  # src/ says another function is retail's target
        name = self.rows.get(theirs, {}).get('name')
        if unmarked and name:
            theirs_name = plain_name(name)
            theirs_name = ALIASES.get(theirs_name, theirs_name)
            return any(ALIASES.get(n, n) == theirs_name for n in (plain_name(s.name) for s in unmarked))
        return True


def check_function(full, start, theirs, theirs_va, absolute, relative, lo, hi, identity=None):
    """Compares one function. Returns (status, first difference or None,
    disassembly lines, length of our code). identity, if given, also checks
    that each call out of the function reaches the same function as retail."""
    ours = extract(full, theirs)
    # a field counts only when it lies wholly inside both extents
    size = min(len(ours), len(theirs))
    fixups = [*absolute, *relative]
    failed = {k for k in field_starts(start, size, absolute)
              if not absolute_ok(ours, start, theirs, theirs_va, k, lo, hi)}
    failed |= {k for k in field_starts(start, size, relative)
               if not relative_ok(ours, start, theirs, theirs_va, k)
               or not same_callee(ours, start, theirs, theirs_va, k, identity)}
    forced = straddling_offsets(start, size, fixups)
    for k in failed:
        forced.update(range(k, k + 4))
    masked = masked_offsets(start, size, fixups) - forced
    first = compare(ours, theirs, masked)
    offsets = [k for k in forced if k < size]
    if first is not None:
        offsets.append(first)
    if not offsets:
        return 'matched', None, [], len(ours)
    count, lines, complete = differing_instructions(ours, start, theirs, theirs_va, masked, forced)
    near = complete and len(ours) == len(theirs) and count <= NEAR
    return ('near' if near else 'todo'), min(offsets), lines, len(ours)


def _within(fixups, low, high):
    """The part of the sorted fixups in [low, high)."""
    return fixups[bisect.bisect_left(fixups, low):bisect.bisect_left(fixups, high)]


def parse_addresses(texts):
    try:
        return {int(a, 16) for a in texts}
    except ValueError:
        bad = next(a for a in texts if not re.fullmatch(r'(0[xX])?[0-9a-fA-F]+', a))
        raise SystemExit(f'{bad!r} is not a hexadecimal retail address')


def check_unique_markers(marked):
    seen = {}
    for m in marked:
        if m.retail in seen:
            raise SystemExit(f'duplicate @retail {m.retail:#x}: {seen[m.retail].path} {seen[m.retail].name} '
                             f'and {m.path} {m.name}')
        seen[m.retail] = m


def write_report(path, report, merge):
    """Writes the report; a filtered run (merge) updates the entries it checked
    and keeps the rest of an existing report."""
    if merge and os.path.exists(path):
        with open(path, encoding='utf-8') as f:
            report = {**json.load(f), **report}
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, 'w', encoding='utf-8') as f:
        json.dump(report, f, indent=1, sort_keys=True)


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument('addresses', nargs='*')
    ap.add_argument('--no-build', action='store_true')
    args = ap.parse_args()
    retail_path = retail_xbe_path()
    if not os.path.exists(retail_path):
        raise SystemExit(f'retail XBE not found at {retail_path}: put it at orig/default.xbe or set RETAIL_XBE')
    check_retail(retail_path)
    wanted = parse_addresses(args.addresses)
    map_path = os.path.join(ROOT, 'build', build.MAP_NAME)
    exe_path = os.path.join(ROOT, 'build', build.EXE_NAME)
    if args.no_build:
        if not os.path.exists(map_path) or not os.path.exists(exe_path):
            raise SystemExit('--no-build needs a previous build (build/halo2.exe and build/halo2.map); run without it first')
    else:
        map_path = build.build()

    linkmap = LinkMap.read(map_path)
    image = Pe(exe_path)
    retail = Xbe(retail_path)
    rows = read_rows(FUNCTIONS_CSV)
    absolute, relative = sorted(image.fixups), sorted(linkmap.rel_fixups)
    lo = retail.base
    hi = max(s.va + s.vsize for s in retail.sections)

    marked = build.marked_sources()
    stubs = build.stub_sources()
    check_unique_markers(marked + stubs)
    identity = Identity(linkmap, rows, marked + stubs)
    unmarked = wanted - {m.retail for m in marked}
    if unmarked:
        print('no @retail marker in src/ for: ' + ', '.join(f'{a:#x}' for a in sorted(unmarked)))
        sys.exit(1)
    report, failed, claimed = {}, 0, set()
    for m in marked:
        if wanted and m.retail not in wanted:
            continue
        row = rows.get(m.retail)
        if row is None:
            raise SystemExit(f'{m.path}: @retail {m.retail:#x} is not a function start in config/functions.csv')
        symbol = resolve(linkmap, m)
        claimed.add(m.retail)
        if symbol is None:
            failed += 1
            print(f'MISSING {m.retail:08x} {m.name}: not in the image '
                  '(the linker folded it into an identical function, or nothing references it)')
            row['status'], row['source'] = 'todo', m.path
            report[f'{m.retail:08x}'] = dict(name=m.name, source=m.path, status='todo', size=int(row['size']),
                                             ours=0, first_difference=0)
            continue
        start, end = linkmap.extent(symbol)
        theirs = retail.read(m.retail, int(row['size']))
        window = (start - 3, end)  # every fixup that touches the function
        status, first, lines, length = check_function(
            image.read(start, end - start), start, theirs, m.retail,
            _within(absolute, *window), _within(relative, *window), lo, hi, identity)
        if first is not None:
            failed += 1
            print(f'DIFF   {m.retail:08x} {m.name} at +{first:#x} ({length} bytes, retail {len(theirs)})')
            print('\n'.join(lines))
        else:
            print(f'MATCH  {m.retail:08x} {m.name}')
        row['status'], row['source'] = status, m.path
        report[f'{m.retail:08x}'] = dict(name=m.name, source=m.path, status=status, size=len(theirs),
                                         ours=length, first_difference=first)
    if not wanted:
        for va, r in rows.items():
            if r['source'] and va not in claimed:
                r['source'], r['status'] = '', 'todo'
    write_rows(FUNCTIONS_CSV, list(rows.values()))
    write_report(os.path.join(ROOT, 'build', 'report.json'), report, merge=bool(wanted))

    def summary(label, subset):
        done = [r for r in subset if r['status'] == 'matched']
        total, matched = sum(int(r['size']) for r in subset), sum(int(r['size']) for r in done)
        print(f'matched {len(done)} of {len(subset)} {label} '
              f'({matched} of {total} bytes, {100 * matched / max(total, 1):.2f}%)')

    summary('game functions', [r for r in rows.values() if r['owner'] == 'game'])
    summary('functions in scope',
            [r for r in rows.values() if r['owner'] != 'eh' and not r['owner'].startswith('third:')])
    sys.exit(1 if failed else 0)


if __name__ == '__main__':
    main()
