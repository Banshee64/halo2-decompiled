"""Builds a source file with the XDK 5849 compiler under link-time code
generation, then compares functions with the retail XBE.

    python tools/match.py <source.cpp> "<cl flags>" <symbol>=<retail va> [...]

<symbol> is the decorated name the linker map gives the function. A function
matches when its instructions and bytes equal the retail ones, apart from
addresses: data addresses differ because the test image lays out its data
on its own.

The source supplies its own callers and an `extern "C" int entry(void)`, as
the image is linked without the C runtime.

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
# the retail image's span, and the test image's (its base is 0x400000)
RETAIL_SPAN = (0x10000, 0x600000)
TEST_SPAN = (0x400000, 0x1400000)
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


def text(ins, start, span):
    """The instruction with addresses masked: branch targets inside the
    function become offsets from its start, other addresses in the image ADDR."""
    if is_branch(ins):
        rel = ins.operands[0].imm - start
        return f'{ins.mnemonic} ' + (f'+{rel:#x}' if 0 <= rel < MAX_FUNCTION else 'FUNC')

    def mask(m):
        return 'ADDR' if span[0] <= int(m.group(0), 16) < span[1] else m.group(0)
    return re.sub(r'0x[0-9a-f]+', mask, f'{ins.mnemonic} {ins.op_str}')


def masked_bytes(ins, span):
    """The instruction's bytes with addresses and branch displacements zeroed."""
    b = bytearray(ins.bytes)
    if is_branch(ins):
        if len(b) >= 5:
            b[-4:] = bytes(4)
        return bytes(b)
    for op in ins.operands:
        v = op.imm if op.type == x86.X86_OP_IMM else op.mem.disp if op.type == x86.X86_OP_MEM else None
        if v is not None and span[0] <= (v & 0xFFFFFFFF) < span[1]:
            k = bytes(b).find(struct.pack('<I', v & 0xFFFFFFFF))
            if k >= 0:
                b[k:k + 4] = bytes(4)
    return bytes(b)


def compare(name, ours, retail):
    a = [text(i, ours[0].address, TEST_SPAN) for i in ours]
    b = [text(i, retail[0].address, RETAIL_SPAN) for i in retail]
    same = a == b and all(masked_bytes(x, TEST_SPAN) == masked_bytes(y, RETAIL_SPAN)
                          for x, y in zip(ours, retail))
    print(f'{"MATCH" if same else "DIFF "}  {name} ({len(a)} instructions, retail {len(b)})')
    if not same:
        for k in range(max(len(a), len(b))):
            x = a[k] if k < len(a) else ''
            y = b[k] if k < len(b) else ''
            print(f'  {"  " if x == y else "!!"} {x:<44} | {y}')
    return same


def main():
    if len(sys.argv) < 4:
        sys.exit(__doc__)
    source, cflags = os.path.abspath(sys.argv[1]), sys.argv[2].split()
    pairs = [arg.rsplit('=', 1) for arg in sys.argv[3:]]
    stem = os.path.splitext(os.path.basename(source))[0]
    os.makedirs(OUT, exist_ok=True)
    run('CL.Exe', '/c', '/GL', *cflags, source, f'/Fo{stem}.obj')
    run('Link.Exe', '/LTCG', '/NODEFAULTLIB', '/ENTRY:entry', '/SUBSYSTEM:CONSOLE', '/MAP',
        f'/OUT:{stem}.exe', f'{stem}.obj')

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
        matched += compare(name, function(test.read, symbols[name]), function(retail.read, int(va, 16)))
    print(f'{matched}/{len(pairs)} match')
    sys.exit(matched != len(pairs))


if __name__ == '__main__':
    main()
