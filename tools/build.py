"""Builds every source in src/ into one LTCG image, build/halo2.exe, with its
map build/halo2.map, so tools/check.py can compare its functions with retail.

Each source is compiled with its own flags (a "// @flags" line in the source,
else config/files.json). A source with functions marked "// @retail 0x..." is
compiled as build/gen/<stem>_tu.cpp: the source itself, then a stand-in caller
for each marked function, with inlining off. The stand-ins share the source's
translation unit, so its own types and declarations are in scope. A stand-in
keeps its function in the image and out of line, as retail's own callers do. Sources in src/stubs/ are compiled without /GL, so
they keep the standard calling conventions retail uses for code outside the
project.

    python tools/build.py
"""
import json
import os
import re
import subprocess
from dataclasses import dataclass

from xbe import ROOT, xdk_dir

MARKER = re.compile(r'^\s*//\s*@(retail|stub)\s+(0x[0-9a-fA-F]+)\s*$')
FLAGS = re.compile(r'^\s*//\s*@flags\s+(.+?)\s*$')
FLAGS_LINES = 30
SIZE_FLAGS = ['/O1', '/Ob0', '/Gr']
ARGUMENT_STRIDE = 16
LIBRARIES = ['libcmt.lib', 'libcpmt.lib', 'xapilib.lib', 'xboxkrnl.lib', 'dsound.lib', 'xonlines.lib',
             'xvoice.lib', 'xnet.lib', 'd3d8ltcg.lib', 'xgraphicsltcg.lib', 'd3dx8.lib']
EXE_NAME = 'halo2.exe'
MAP_NAME = 'halo2.map'


@dataclass
class Marked:
    path: str
    retail: int
    name: str
    returns: str
    params: list
    stub: bool = False
    cls: str = ''
    kind: str = 'function'  # 'function', 'method', 'constructor' or 'destructor'


def _split_params(text):
    parts, depth, current = [], 0, ''
    for ch in text:
        if ch in '(<[':
            depth += 1
        elif ch in ')>]':
            depth -= 1
        if ch == ',' and depth == 0:
            parts.append(current)
            current = ''
        else:
            current += ch
    if current.strip():
        parts.append(current)
    return [p.strip() for p in parts]


def _param_type(param):
    """'unsigned long *crc_reference' -> 'unsigned long *'."""
    param = param.split('=')[0].strip()
    m = re.match(r'^(.*?)([A-Za-z_]\w*)\s*(\[\s*\d*\s*\])?$', param)
    if m and m.group(1).strip() and not m.group(1).strip().endswith(('struct', 'union', 'enum')):
        kind = m.group(1).strip()
        return kind + ' *' if m.group(3) else kind
    return param


QUALIFIERS = re.compile(r'\b(?:__stdcall|__cdecl|__fastcall|__thiscall|__clrcall|__inline|__forceinline|'
                        r'inline|PRIVATE|static|extern|virtual)\b')
HEADER = re.compile(r'^(.*?)\s*((?:[A-Za-z_]\w*::)*~?[A-Za-z_]\w*)\s*\(')
CLASS_DECLARATION = re.compile(r'\b(?:struct|class|union)\s+([A-Za-z_]\w*)')
CLASS_TYPEDEF = re.compile(r'\btypedef\s+(?:struct|class|union)\b[^;{]*(?:\{.*?\})?\s*([A-Za-z_]\w*)\s*;', re.S)


def _balanced(text, start):
    """The text inside the parentheses that open at text[start], or None."""
    depth = 0
    for i in range(start, len(text)):
        depth += text[i] == '('
        depth -= text[i] == ')'
        if not depth:
            return text[start + 1:i]
    return None


def _kind(name):
    """(class, kind) of a possibly qualified function name."""
    cls, _, last = name.rpartition('::')
    if not cls:
        return '', 'function'
    if last.startswith('~'):
        return cls, 'destructor'
    return (cls, 'constructor') if last == cls.rpartition('::')[2] else (cls, 'method')


def scan(text, path):
    lines = text.splitlines()
    found = []
    for i, line in enumerate(lines):
        m = MARKER.match(line)
        if not m:
            continue
        header = ''
        for later in lines[i + 1:]:
            header += ' ' + later.split('//')[0]
            if '{' in later:
                break
        header = ' '.join(header.split('{')[0].split())
        h = HEADER.match(header)
        inner = h and _balanced(header, h.end() - 1)
        if inner is None:
            raise SystemExit(f'{path}:{i + 2}: cannot read the function after @{m.group(1)} {m.group(2)}')
        params = [_param_type(p) for p in _split_params(inner)]
        if params == ['void']:
            params = []
        name = h.group(2)
        cls, kind = _kind(name)
        returns = ' '.join(QUALIFIERS.sub(' ', h.group(1)).split())
        if kind in ('constructor', 'destructor'):
            returns = ''
        found.append(Marked(path, int(m.group(2), 16), name, returns or ('int' if kind == 'function' else ''),
                            params, m.group(1) == 'stub', cls, kind))
    return found


def class_names(texts):
    """Names declared as struct, class or union in the given sources."""
    names = set()
    for text in texts:
        names.update(CLASS_DECLARATION.findall(text))
        names.update(CLASS_TYPEDEF.findall(text))
    return names


def _is_class(kind, classes):
    words = re.findall(r'[A-Za-z_]\w*', kind)
    return bool(words) and (words[0] in ('struct', 'class', 'union') or any(w in classes for w in words))


def _argument(kind, offset, prefix, classes):
    """An expression reading one parameter of type `kind` from the argument buffer.
    Scalars and pointers are read through a volatile pointer, so the optimizer
    cannot fold them. A class or struct passed by value cannot be copied from
    volatile storage (no volatile copy constructor), so it is read through a
    plain pointer: the buffer itself stays volatile-declared, and the value
    still comes from memory the optimizer knows nothing about."""
    place = f'standin_{prefix}_arguments + {offset}'
    if kind.endswith('&'):
        return f'**({kind[:-1].strip()} * volatile *)({place})'
    if '*' not in kind and _is_class(kind, classes):
        return f'*({kind} *)(void *)({place})'
    return f'*({kind} volatile *)({place})'


def tu_source(source_abs, marked, prefix, classes=()):
    """The translation unit that holds a source and its stand-ins: the source
    itself, then one function per marked function that calls it with values
    read from a volatile buffer, so the call stays out of line."""
    buffer = f'standin_{prefix}_arguments'
    out = [f'/* generated by tools/build.py: {os.path.basename(source_abs)} with its stand-in callers */',
           f'#include "{source_abs}"']
    if any(m.kind == 'constructor' for m in marked):
        out.append('#include <new>')
    out += ['#pragma auto_inline(off)', '#pragma inline_depth(0)',
            f'static __declspec(align(16)) unsigned char volatile {buffer}[{ARGUMENT_STRIDE * 16}];']
    for k, m in enumerate(marked):
        args = ', '.join(_argument(p, ARGUMENT_STRIDE * n, prefix, classes) for n, p in enumerate(m.params))
        member = m.name.rpartition('::')[2]
        if m.kind == 'constructor':
            call = f'::new ((void *){buffer}) {m.cls}({args})'
        elif m.kind == 'destructor':
            call = f'(({m.cls} *){buffer})->{member}()'
        elif m.kind == 'method':
            call = f'(({m.cls} *){buffer})->{member}({args})'
        else:
            call = f'{m.name}({args})'
        stored = (m.kind in ('function', 'method') and m.returns != 'void'
                  and ('*' in m.returns or not _is_class(m.returns, classes)))
        if stored:
            out.append(f'static {m.returns} volatile standin_{prefix}_result_{k};')
            call = f'standin_{prefix}_result_{k} = {call}'
        out.append(f'void standin_{prefix}_{k}(void) {{ {call}; }}')
    return '\n'.join(out) + '\n'


def run_tool(tool, args, cwd, xdk):
    env = dict(os.environ, INCLUDE=os.path.join(xdk, 'include'), LIB=os.path.join(xdk, 'lib'))
    p = subprocess.run([os.path.join(xdk, 'bin', 'vc71', tool), '/nologo', *args], cwd=cwd, env=env,
                       capture_output=True, text=True)
    if p.returncode:
        raise SystemExit(f'{tool} failed:\n{p.stdout}{p.stderr}')


def _stale(target, sources, newest_header):
    if not os.path.exists(target):
        return True
    t = os.path.getmtime(target)
    return newest_header > t or any(os.path.getmtime(s) > t for s in sources)


def source_flags(flags):
    """What every source in src/ is compiled with, whatever files.json says:
    LTCG, and the register calling convention retail's code uses."""
    return ['/GL', '/Gr', *flags]


def file_flags(text, name, config):
    """A source's own flags: its "// @flags ..." line among the first 30 lines,
    else its entry in config/files.json, else the default."""
    for line in text.splitlines()[:FLAGS_LINES]:
        m = FLAGS.match(line)
        if m:
            return m.group(1).split()
    return config.get('files', {}).get(name, config['default'])


def _source_names(root):
    src = os.path.join(root, 'src')
    return sorted(n for n in os.listdir(src) if n.endswith('.cpp')) if os.path.isdir(src) else []


def marked_sources(root=ROOT):
    """Every function marked "// @retail 0x..." in src/*.cpp, in file order.
    Stubs ("// @stub 0x...") are not among them: they are not checked and get no stand-in."""
    marked = []
    for name in _source_names(root):
        with open(os.path.join(root, 'src', name), encoding='utf-8') as f:
            marked += [m for m in scan(f.read(), f'src/{name}') if not m.stub]
    return marked


def stub_sources(root=ROOT):
    """Every function marked "// @stub 0x..." in src/stubs/*.cpp, in file order."""
    folder = os.path.join(root, 'src', 'stubs')
    marked = []
    if os.path.isdir(folder):
        for name in sorted(n for n in os.listdir(folder) if n.endswith('.cpp')):
            with open(os.path.join(folder, name), encoding='utf-8') as f:
                marked += [m for m in scan(f.read(), f'src/stubs/{name}') if m.stub]
    return marked


def entry_source(decls, calls):
    # no _fltused here: float code pulls __fltused from libcmt (fpinit.obj), and a second one clashes
    return ('/* generated by tools/build.py: the image entry point */\n' + '\n'.join(decls) +
            '\nextern "C" int entry(void)\n{\n' + '\n'.join(calls) + '\n\treturn 0;\n}\n')


def build(root=ROOT, xdk=None):
    xdk = xdk or xdk_dir()
    if not os.path.exists(os.path.join(xdk, 'bin', 'vc71', 'CL.Exe')):
        raise SystemExit(f'Xbox SDK 5849 not found at {xdk}. Put its xbox folder at sdk/xbox, '
                         'or set XDK_DIR to it.')
    src, out = os.path.join(root, 'src'), os.path.join(root, 'build')
    obj_dir, gen = os.path.join(out, 'obj'), os.path.join(out, 'gen')
    os.makedirs(obj_dir, exist_ok=True)
    os.makedirs(gen, exist_ok=True)
    with open(os.path.join(root, 'config', 'files.json'), encoding='utf-8') as f:
        config = json.load(f)
    headers = [os.path.join(d, n) for base in ('include', 'src') for d, _, ns in os.walk(os.path.join(root, base))
               for n in ns if n.endswith('.h')]
    newest_header = max((os.path.getmtime(h) for h in headers), default=0)
    includes = ['/I', os.path.join(root, 'include'), '/I', src]
    marked_by_path = {}
    for m in marked_sources(root):
        marked_by_path.setdefault(m.path, []).append(m)

    texts = {}
    for name in _source_names(root):
        with open(os.path.join(src, name), encoding='utf-8') as f:
            texts[name] = f.read()
    classes = class_names([*texts.values(), *(open(h, encoding='utf-8').read() for h in headers)])

    objects, entry_calls, entry_decls = [], [], []
    for name in _source_names(root):
        path = os.path.join(src, name)
        stem = os.path.splitext(name)[0]
        text = texts[name]
        flags = file_flags(text, name, config)
        obj = os.path.join(obj_dir, stem + '.obj')
        marked = marked_by_path.get(f'src/{name}')
        compiled, depends = path, [path, os.path.join(root, 'config', 'files.json')]
        if marked:
            compiled = os.path.join(gen, f'{stem}_tu.cpp')
            source = tu_source(os.path.abspath(path), marked, stem, classes)
            if not os.path.exists(compiled) or open(compiled, encoding='utf-8').read() != source:
                with open(compiled, 'w', encoding='utf-8') as f:
                    f.write(source)
            depends.append(compiled)
            for k in range(len(marked)):
                entry_decls.append(f'void standin_{stem}_{k}(void);')
                entry_calls.append(f'\tstandin_{stem}_{k}();')
        if _stale(obj, depends, newest_header):
            run_tool('CL.Exe', ['/c', *source_flags(flags), *includes, compiled, f'/Fo{obj}'], root, xdk)
        objects.append(obj)

    folder = os.path.join(src, 'stubs')
    if os.path.isdir(folder):
        for name in sorted(n for n in os.listdir(folder) if n.endswith('.cpp')):
            path = os.path.join(folder, name)
            obj = os.path.join(obj_dir, f'stubs_{os.path.splitext(name)[0]}.obj')
            if _stale(obj, [path], newest_header):
                run_tool('CL.Exe', ['/c', '/O2', '/Gr', *includes, path, f'/Fo{obj}'], root, xdk)
            objects.append(obj)

    entry = os.path.join(gen, 'entry.cpp')
    entry_text = entry_source(entry_decls, entry_calls)
    entry_obj = os.path.join(obj_dir, 'entry.obj')
    if not os.path.exists(entry) or open(entry, encoding='utf-8').read() != entry_text:
        with open(entry, 'w', encoding='utf-8') as f:
            f.write(entry_text)
    if _stale(entry_obj, [entry], newest_header):
        run_tool('CL.Exe', ['/c', '/GL', *SIZE_FLAGS, entry, f'/Fo{entry_obj}'], root, xdk)
    objects.append(entry_obj)

    exe, map_path = os.path.join(out, EXE_NAME), os.path.join(out, MAP_NAME)
    run_tool('Link.Exe', ['/LTCG', '/NODEFAULTLIB', '/ENTRY:entry', '/SUBSYSTEM:CONSOLE', '/MAP:' + map_path,
                          '/MAPINFO:FIXUPS', '/FIXED:NO', f'/OUT:{exe}', *objects, *LIBRARIES], root, xdk)
    return map_path


if __name__ == '__main__':
    print(build())
