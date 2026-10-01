"""Builds source files with the XDK 5849 compiler under link-time code
generation, then compares functions with the retail XBE.

    python tools/match.py "<cl flags>" <source.cpp> [...] [-- <symbol>=<retail va> ...]

Each source is compiled with the flags given before it, so files can differ,
as Bungie's did (some were optimized for size). The sources are linked into
one image with /LTCG.

<symbol> is the decorated name the linker map gives the function. A function
matches when its bytes equal the retail ones, apart from addresses: the
fields the test image relocates, and calls out of the function. Data and
callee addresses differ because the test image lays itself out on its own.

One source supplies an `extern "C" int entry(void)`, and the sources supply
the callers, as the image is linked without the C runtime.

Environment:
  XDK_DIR     the SDK's xbox folder (default: sdk/xbox in the repository)
  RETAIL_XBE  the retail default.xbe (default: orig/default.xbe)
Needs capstone (pip install capstone). Windows only: the SDK's compiler is a
Windows program.
"""
import os
import re
import struct
import subprocess
import sys

from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone import x86

from xbe import Xbe

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
XDK = os.environ.get('XDK_DIR', os.path.join(ROOT, 'sdk', 'xbox'))
VC = os.path.join(XDK, 'bin', 'vc71')
RETAIL = os.environ.get('RETAIL_XBE', os.path.join(ROOT, 'orig', 'default.xbe'))
OUT = os.path.join(ROOT, 'build', 'match')
MAX_FUNCTION = 0x4000

md = Cs(CS_ARCH_X86, CS_MODE_32)
md.detail = True


def run(tool, *args):
    env = dict(os.environ, INCLUDE=os.path.join(XDK, 'include'), LIB=os.path.join(XDK, 'lib'))
    p = subprocess.run([os.path.join(VC, tool), '/nologo', *args], cwd=OUT, env=env,
                       capture_output=True, text=True)
    if p.returncode:
        sys.exit(f'{tool} failed:\n{p.stdout}{p.stderr}')


class Pe:
    """Reads the linked test image."""

    def __init__(self, path):
        d = self.data = open(path, 'rb').read()
        pe, = struct.unpack_from('<I', d, 0x3C)
        nsec, = struct.unpack_from('<H', d, pe + 6)
        opt = pe + 24
        base, = struct.unpack_from('<I', d, opt + 28)
        table = opt + struct.unpack_from('<H', d, pe + 20)[0]
        self.sections = []
        for i in range(nsec):
            vs, va, rs, ra = struct.unpack_from('<IIII', d, table + i * 40 + 8)
            self.sections.append((base + va, vs, ra, rs))
        # the base relocations: every 4-byte field that holds an address
        self.fixups = set()
        rva, size = struct.unpack_from('<II', d, opt + 96 + 5 * 8)
        if rva:
            blocks = self.read(base + rva, size)
            o = 0
            while o + 8 <= size:
                page, length = struct.unpack_from('<II', blocks, o)
                if length < 8:
                    break
                for k in range(8, length, 2):
                    entry, = struct.unpack_from('<H', blocks, o + k)
                    if entry >> 12 == 3:  # IMAGE_REL_BASED_HIGHLOW
                        self.fixups.add(base + page + (entry & 0xFFF))
                o += length

    def read(self, va, size):
        for sva, vs, ra, rs in self.sections:
            if sva <= va < sva + vs:
                o = va - sva
                chunk = self.data[ra + o:ra + min(o + size, rs)]
                return chunk + bytes(size - len(chunk))
        raise ValueError(f'{va:#x} is in no section')


def function(read, va):
    """The function's instructions: up to the last ret or jmp that no branch
    inside the function jumps past."""
    out, reach = [], va
    for ins in md.disasm(read(va, MAX_FUNCTION), va):
        if ins.mnemonic == 'int3':
            break
        out.append(ins)
        if ins.mnemonic.startswith('j') and ins.operands[0].type == x86.X86_OP_IMM:
            if va <= ins.operands[0].imm < va + MAX_FUNCTION:
                reach = max(reach, ins.operands[0].imm)
        if ins.mnemonic in ('ret', 'jmp') and ins.address >= reach:
            break
    return out


def is_branch(ins):
    return (ins.mnemonic.startswith('j') or ins.mnemonic == 'call') and ins.operands[0].type == x86.X86_OP_IMM


def leaves(ins, function):
    """Whether a branch goes outside its function."""
    return not function[0].address <= ins.operands[0].imm <= function[-1].address


def masked(x, y, ours, retail, fixups):
    """Our instruction's bytes and retail's, with the address fields zeroed:
    where our image has a base relocation (the same field holds an address in
    retail), and the displacement of a call or jump out of the function."""
    bx, by = bytearray(x.bytes), bytearray(y.bytes)
    if len(bx) != len(by):
        return bytes(bx), bytes(by)
    for k in range(len(bx)):
        if x.address + k in fixups:
            bx[k:k + 4] = by[k:k + 4] = bytes(4)
    if is_branch(x) and is_branch(y) and leaves(x, ours) and leaves(y, retail) and len(bx) >= 5:
        bx[-4:] = by[-4:] = bytes(4)
    return bytes(bx), bytes(by)


def text(ins, function):
    """The instruction, with branch targets inside the function shown as
    offsets from its start."""
    if is_branch(ins):
        if leaves(ins, function):
            return f'{ins.mnemonic} {ins.op_str}'
        return f'{ins.mnemonic} +{ins.operands[0].imm - function[0].address:#x}'
    return f'{ins.mnemonic} {ins.op_str}'


def compare(name, ours, retail, fixups):
    pairs = [masked(x, y, ours, retail, fixups) for x, y in zip(ours, retail)]
    same = len(ours) == len(retail) and all(a == b for a, b in pairs)
    print(f'{"MATCH" if same else "DIFF "}  {name} ({len(ours)} instructions, retail {len(retail)})')
    if not same:
        for k in range(max(len(ours), len(retail))):
            x = text(ours[k], ours) if k < len(ours) else ''
            y = text(retail[k], retail) if k < len(retail) else ''
            ok = k < len(pairs) and pairs[k][0] == pairs[k][1]
            print(f'  {"  " if ok else "!!"} {x:<44} | {y}')
    return same


def main():
    args = sys.argv[1:]
    split = args.index('--') if '--' in args else len(args)
    builds, flags = [], None
    for arg in args[:split]:
        if arg.lower().endswith(('.c', '.cpp')):
            if flags is None:
                sys.exit(__doc__)
            builds.append((os.path.abspath(arg), flags))
        else:
            flags = arg.split()
    pairs = [arg.rsplit('=', 1) for arg in args[split + 1:]]
    if not builds:
        sys.exit(__doc__)
    stem = os.path.splitext(os.path.basename(builds[0][0]))[0]
    os.makedirs(OUT, exist_ok=True)
    objects = []
    for source, cflags in builds:
        obj = os.path.splitext(os.path.basename(source))[0] + '.obj'
        run('CL.Exe', '/c', '/GL', *cflags, source, f'/Fo{obj}')
        objects.append(obj)
    run('Link.Exe', '/LTCG', '/NODEFAULTLIB', '/ENTRY:entry', '/SUBSYSTEM:CONSOLE', '/MAP', '/FIXED:NO',
        f'/OUT:{stem}.exe', *objects)

    symbols = {}
    for line in open(os.path.join(OUT, stem + '.map')):
        f = line.split()
        if len(f) >= 3 and re.fullmatch(r'[0-9a-f]{8}', f[2]):
            symbols[f[1]] = int(f[2], 16)
    test = Pe(os.path.join(OUT, stem + '.exe'))
    retail = Xbe(RETAIL)

    matched = 0
    for name, va in pairs:
        if name not in symbols:
            print(f'GONE   {name} (inlined or removed)')
            continue
        matched += compare(name, function(test.read, symbols[name]), function(retail.read, int(va, 16)),
                           test.fixups)
    print(f'{matched}/{len(pairs)} match')
    sys.exit(matched != len(pairs))


if __name__ == '__main__':
    main()
