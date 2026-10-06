"""Lists what tools/check.py cannot see: values behind the address fields it
masks, and data that src/ defines at retail addresses.

check.py compares code byte for byte apart from address fields, so a float
constant, a string or a table entry with a different value still matches.
This reads the retail XBE and src/, builds nothing, and reports:

- constants: a float that a function's retail code loads from .rdata, which
  no literal in its source gives while one comes within 1e-5 of it (the
  source writes 0.01f where retail has 0.1f * 0.1f, one float step above);
- strings: a C string of four or more characters that retail's code refers
  to, which is not a string literal in src/ or include/ and not a
  g_<address> global at that address;
- scripts: the fields of every s_type_f4462a g_<address> initializer (a
  script function definition) against retail's data at that address.

A function's source is the text from its @retail marker to the first line
that is a lone closing brace. Numbers are read from that text and from the
#define macros it names; a string counts when it is a literal anywhere in
src/ or include/. Exit status 1 when something is listed.

    python tools/masked.py [--constants] [--strings] [--scripts] [va ...]
"""
import argparse
import glob
import os
import re
import struct
import sys

from capstone import CS_ARCH_X86, CS_MODE_32, Cs, x86

from inventory import read_rows
from xbe import FUNCTIONS_CSV, ROOT, load, retail_xbe_path

_MARKER = re.compile(r'//\s*@retail\s+0x([0-9a-fA-F]+)')
_DEFINE = re.compile(r'^\s*#define\s+(\w+)(?:\([^)]*\))?\s+(.+?)\s*$', re.M)
_FLOAT = re.compile(r'(?<![\w.])((?:\d+\.\d*|\.\d+)(?:[eE][-+]?\d+)?|\d+[eE][-+]?\d+)[fF]?(?![\w.])')
_INT = re.compile(r'(?<![\w.])(\d+)(?![\w.xX])')
_IDENT = re.compile(r'\b([A-Za-z_]\w*)\b')
# a string literal: \x5c is a backslash
_STRING = re.compile(r'L?"((?:[^"\x5c\n]|\x5c.)*)"')
_ADJACENT = re.compile(r'"\s*L?"')
_ESCAPES = {'n': '\n', 't': '\t', 'r': '\r', '0': '\0', 'a': '\a', 'b': '\b',
            'f': '\f', 'v': '\v'}
_FLOAT_OPS = {'movss', 'addss', 'subss', 'mulss', 'divss', 'comiss', 'ucomiss',
              'maxss', 'minss', 'sqrtss', 'cvtss2sd', 'cvttss2si', 'cvtss2si',
              'rcpss', 'rsqrtss', 'fld', 'fadd', 'fsub', 'fsubr', 'fmul', 'fdiv',
              'fdivr', 'fcom', 'fcomp'}
_SCRIPT = re.compile(r's_type_f4462a\s+const\s+g_([0-9a-f]+)\s*=\s*\{(.*?)\};', re.S)


def f32(value):
    """value rounded to a float (infinite past the float range)."""
    try:
        return struct.unpack('<f', struct.pack('<f', value))[0]
    except OverflowError:
        return float('inf') if value > 0 else float('-inf')


def unescape(text):
    """A C string literal's characters."""
    out, i = [], 0
    while i < len(text):
        c = text[i]
        if c == '\x5c' and i + 1 < len(text):
            n = text[i + 1]
            if n == 'x':
                digits = re.match(r'[0-9a-fA-F]+', text[i + 2:]).group(0)
                out.append(chr(int(digits, 16)))
                i += 2 + len(digits)
                continue
            out.append(_ESCAPES.get(n, n))
            i += 2
            continue
        out.append(c)
        i += 1
    return ''.join(out)


def defines(texts):
    """Every #define in texts: name -> its replacement text (the first wins)."""
    found = {}
    for text in texts:
        for m in _DEFINE.finditer(text):
            found.setdefault(m.group(1), m.group(2))
    return found


def numbers(text, macros, depth=0):
    """The numeric literals in text and in the macros it names."""
    values = {float(m.group(1)) for m in _FLOAT.finditer(text)}
    values |= {float(m.group(1)) for m in _INT.finditer(text) if int(m.group(1)) < 1 << 24}
    if depth < 3:
        for name in set(_IDENT.findall(text)):
            if name in macros:
                values |= numbers(macros[name], macros, depth + 1)
    return values


def strings(text):
    """The string literals in text, adjacent literals joined."""
    return {unescape(m.group(1)) for m in _STRING.finditer(_ADJACENT.sub('', text))}


def bodies(path, text):
    """{retail va: (path, line, text)} for each @retail marker in a file: the
    text from the marker to the first lone closing brace after it."""
    found = {}
    markers = list(_MARKER.finditer(text))
    for k, m in enumerate(markers):
        end = markers[k + 1].start() if k + 1 < len(markers) else len(text)
        body = text[m.start():end]
        close = re.search(r'\n\}[ \t]*\n', body)
        if close:
            body = body[:close.end()]
        found.setdefault(int(m.group(1), 16), (path, text.count('\n', 0, m.start()) + 1, body))
    return found


def float_loads(image, va, size, lo, hi):
    """(instruction va, constant va, value) for each float that the code at va
    loads from [lo, hi) by its absolute address."""
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    md.detail = True
    md.skipdata = True  # jump tables inside a function
    found = []
    for i in md.disasm(image.read(va, size), va):
        if i.id == 0 or i.mnemonic not in _FLOAT_OPS:  # skipped data has id 0
            continue
        for op in i.operands:
            if op.type == x86.X86_OP_MEM and op.mem.base == 0 and op.mem.index == 0 and op.size == 4:
                a = op.mem.disp & 0xffffffff
                if lo <= a < hi:
                    found.append((i.address, a, struct.unpack('<f', image.read(a, 4))[0]))
    return found


def near_miss(value, literals, tolerance=1e-5):
    """The literal within tolerance of value (relative) when none equals it as
    a float, else None."""
    if value == 0.0:
        return None
    if any(f32(v) == value for v in literals):
        return None
    close = [v for v in literals if v and abs(f32(v) - value) <= tolerance * abs(value)]
    return min(close, key=lambda v: abs(f32(v) - value)) if close else None


def explain(value, literals):
    """A square (a * a) or a difference (a - b) of the source's literals that
    gives value as a float, or None. These are the two ways retail's
    constants were found to differ: a squared threshold written as its value,
    and a range written as its width."""
    bits = struct.pack('<f', value)
    # only short decimals (1.2, 0.05, 3.0), so that the pair is a plausible source
    small = sorted(v for v in literals if 0 < v < 1e6 and len(repr(v).strip('0.').replace('.', '')) <= 3)
    for a in small:
        if struct.pack('<f', f32(f32(a) * f32(a))) == bits:
            return f'{a!r}f * {a!r}f'
    for a in small:
        for b in small:
            if a > b and struct.pack('<f', f32(f32(a) - f32(b))) == bits:
                return f'{a!r}f - {b!r}f'
    return None


def cstring(image, va, limit=256):
    """The C string at va when it is four or more printable characters."""
    data = image.read(va, limit)
    n = data.find(b'\0')
    if n < 4 or not all(c in (9, 10, 13) or 32 <= c < 127 for c in data[:n]):
        return None
    return data[:n].decode('latin-1')


def string_refs(image, va, size, lo, hi):
    """(instruction va, string va, text) for each C string in [lo, hi) that the
    code at va refers to by an immediate."""
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    md.detail = True
    md.skipdata = True  # jump tables inside a function
    found = []
    for i in md.disasm(image.read(va, size), va):
        if i.id == 0:  # skipped data
            continue
        for op in i.operands:
            if op.type == x86.X86_OP_IMM and lo <= op.imm & 0xffffffff < hi:
                text = cstring(image, op.imm & 0xffffffff)
                if text is not None:
                    found.append((i.address, op.imm & 0xffffffff, text))
    return found


def split_top(text):
    """text split at the commas outside braces, parentheses and string literals."""
    parts, depth, cur, quote, i = [], 0, '', None, 0
    while i < len(text):
        c = text[i]
        if quote:
            cur += c
            if c == '\x5c':
                cur += text[i + 1]
                i += 1
            elif c == quote:
                quote = None
        elif c in '"\'':
            quote, cur = c, cur + c
        elif c in '({':
            depth, cur = depth + 1, cur + c
        elif c in ')}':
            depth, cur = depth - 1, cur + c
        elif c == ',' and depth == 0:
            parts.append(cur.strip())
            cur = ''
        else:
            cur += c
        i += 1
    if cur.strip():
        parts.append(cur.strip())
    return parts


def hs_types(header):
    """The e_hs_type enumerators of include/hs.h, numbered from 0."""
    body = header.split('enum e_hs_type', 1)[1].split('};', 1)[0]
    return {name: k for k, name in enumerate(re.findall(r'(_hs_type_\w+)', body))}


def marked_functions(texts):
    """name -> retail va for the function declared after each @retail or @stub marker."""
    found = {}
    for text in texts:
        for m in re.finditer(r'//\s*@(?:retail|stub)\s+0x([0-9a-fA-F]+)[^\n]*\n((?:[^\n]*\n){1,4})', text):
            lines = [l for l in m.group(2).split('\n') if l.strip() and not l.strip().startswith('//')]
            name = re.search(r'([A-Za-z_]\w*)\s*\(', lines[0]) if lines else None
            if name and name.group(1) not in ('if', 'return', 'sizeof'):
                found.setdefault(name.group(1), int(m.group(1), 16))
    return found


def script_differences(fields, va, image, types, functions):
    """(field, source, retail) for each field of a script function definition
    initializer that differs from retail's s_type_f4462a at va."""
    def number(token):
        token = token.strip()
        if token in types:
            return types[token]
        if re.fullmatch(r'-?(0x[0-9a-fA-F]+|\d+)', token):
            return int(token, 0)
        return None
    raw = image.read(va, 0x1e)
    ret, flags, evaluate, parse, count = struct.unpack_from('<hHIIh', raw)
    found = []
    if number(fields[0]) != ret:
        found.append(('return type', fields[0], ret))
    if number(fields[1]) is not None and number(fields[1]) != flags:
        found.append(('flags', fields[1], flags))
    name = fields[2].strip().lstrip('&')
    m = re.fullmatch(r'function_([0-9a-f]+)', name)
    want = 0 if name in ('NULL', '0') else int(m.group(1), 16) if m else functions.get(name)
    if want != evaluate:
        found.append(('evaluate', name, f'{evaluate:#x}'))
    if fields[3].startswith('"'):
        text = ''.join(unescape(s) for s in re.findall(r'"((?:[^"\x5c]|\x5c.)*)"', fields[3]))
        data = image.read(parse, 256) if parse else b''
        if not parse or data[:data.find(b'\0')].decode('latin-1') != text:
            found.append(('usage string', fields[3], data[:data.find(b'\0')].decode('latin-1') if parse else None))
    elif fields[3] in ('NULL', '0') and parse:
        found.append(('usage string', fields[3], f'{parse:#x}'))
    if number(fields[4]) != count:
        found.append(('parameter count', fields[4], count))
    elif 0 < count <= 8 and len(fields) > 5:
        source = [number(t) for t in split_top(fields[5].strip()[1:-1])][:count]
        retail = list(struct.unpack_from(f'<{count}h', raw, 0xe))
        if source != retail:
            names = {k: n for n, k in types.items()}
            found.append(('parameter types', fields[5], '{ ' + ', '.join(names.get(v, str(v)) for v in retail) + ' }'))
    return found


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument('va', nargs='*', help='limit the constants and strings to these functions (hex)')
    ap.add_argument('--constants', action='store_true', help='only the float constants')
    ap.add_argument('--strings', action='store_true', help='only the strings')
    ap.add_argument('--scripts', action='store_true', help='only the script function definitions')
    ap.add_argument('--xbe', default=retail_xbe_path(), help='the retail XBE')
    args = ap.parse_args(argv)
    every = not (args.constants or args.strings or args.scripts)
    wanted = {int(v, 16) for v in args.va}
    image = load(args.xbe)
    rdata = image.section('.rdata')
    lo, hi = rdata.va, rdata.va + rdata.vsize
    rows = read_rows(FUNCTIONS_CSV)
    paths = sorted(glob.glob(os.path.join(ROOT, 'src', '**', '*.cpp'), recursive=True))
    headers = sorted(glob.glob(os.path.join(ROOT, 'include', '*.h')))
    texts = {p: open(p, encoding='utf-8', errors='replace').read() for p in paths + headers}
    macros = defines(texts.values())
    found = {}
    for p in paths:
        rel = os.path.relpath(p, ROOT).replace(os.sep, '/')
        for va, item in bodies(rel, texts[p]).items():
            found.setdefault(va, item)
    named = {int(a, 16) for t in texts.values() for a in re.findall(r'\bg_([0-9a-f]{5,8})\b', t)}
    every_string = set().union(*(strings(t) for t in texts.values()))
    listed = 0
    for va, (path, line, body) in sorted(found.items()):
        row = rows.get(va)
        if row is None or (wanted and va not in wanted):
            continue
        size = int(row['size'])
        if every or args.constants:
            literals = numbers(body, macros)
            literals |= {-v for v in literals}
            done = set()
            for at, const, value in float_loads(image, va, size, lo, hi):
                near = near_miss(value, literals)
                if near is None or const in done:
                    continue
                done.add(const)
                bits = struct.pack('<f', value)[::-1].hex()
                ours = struct.pack('<f', f32(near))[::-1].hex()
                how = explain(value, literals)
                print(f'constant {va:#x} {row["status"]} {path}:{line}: '
                      f'{const:#x} is 0x{bits} ({value!r}); the source has {near!r} (0x{ours})'
                      + (f"; {how} gives retail's" if how else ''))
                listed += 1
        if every or args.strings:
            done = set()
            for at, where, text in string_refs(image, va, size, lo, hi):
                if text in every_string or where in named or where in done:
                    continue
                done.add(where)
                print(f'string {va:#x} {row["status"]} {path}:{line}: {where:#x} is {text!r}, '
                      'which is not a string literal in src/ or include/')
                listed += 1
    if every or args.scripts:
        types = hs_types(texts[os.path.join(ROOT, 'include', 'hs.h')])
        functions = marked_functions(texts.values())
        for p in paths:
            rel = os.path.relpath(p, ROOT).replace(os.sep, '/')
            for m in _SCRIPT.finditer(texts[p]):
                fields = split_top(m.group(2))
                if len(fields) < 5:
                    continue
                for field, source, retail in script_differences(fields, int(m.group(1), 16), image, types, functions):
                    line = texts[p].count('\n', 0, m.start()) + 1
                    print(f'script g_{m.group(1)} {rel}:{line}: {field}: the source has {source}, retail {retail}')
                    listed += 1
    print(f'{listed} listed')
    return 1 if listed else 0


if __name__ == '__main__':
    sys.exit(main())
