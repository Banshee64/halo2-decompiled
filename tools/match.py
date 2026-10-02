"""Builds source files with the XDK 5849 compiler under link-time code
generation, then compares functions with the retail XBE.

For project work, use tools/check.py, which builds src/ and checks every marked function. This script is for quick experiments like spike/.

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
import itertools
import os
import sys

from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone import x86

from build import run_tool
from linkmap import LinkMap
from pe import Pe
from xbe import ROOT, Xbe, retail_xbe_path, xdk_dir

OUT = os.path.join(ROOT, 'build', 'match')
MAX_FUNCTION = 0x4000

md = Cs(CS_ARCH_X86, CS_MODE_32)
md.detail = True


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


def leaves(ins, body):
    """Whether a branch goes outside its function."""
    return not body[0].address <= ins.operands[0].imm <= body[-1].address


def same_bytes(x, y, ours, retail, fixups):
    """Whether our instruction's bytes equal retail's, apart from the address
    fields: where our image has a base relocation (the same field holds an
    address in retail), and the displacement of a call or jump out of the
    function."""
    bx, by = bytearray(x.bytes), bytearray(y.bytes)
    if len(bx) != len(by):
        return False
    for k in range(len(bx)):
        if x.address + k in fixups:
            bx[k:k + 4] = by[k:k + 4] = bytes(4)
    if is_branch(x) and is_branch(y) and leaves(x, ours) and leaves(y, retail) and len(bx) >= 5:
        bx[-4:] = by[-4:] = bytes(4)
    return bx == by


def text(ins, body):
    """The instruction, with branch targets inside the function shown as
    offsets from its start."""
    if is_branch(ins) and not leaves(ins, body):
        return f'{ins.mnemonic} +{ins.operands[0].imm - body[0].address:#x}'
    return f'{ins.mnemonic} {ins.op_str}'


def compare(name, ours, retail, fixups):
    oks = [same_bytes(x, y, ours, retail, fixups) for x, y in zip(ours, retail)]
    same = len(ours) == len(retail) and all(oks)
    print(f'{"MATCH" if same else "DIFF "}  {name} ({len(ours)} instructions, retail {len(retail)})')
    if not same:
        for x, y, ok in itertools.zip_longest(ours, retail, oks):
            a = text(x, ours) if x is not None else ''
            b = text(y, retail) if y is not None else ''
            print(f'  {"  " if ok else "!!"} {a:<44} | {b}')
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
    wanted = [arg.rsplit('=', 1) for arg in args[split + 1:]]
    if not builds:
        sys.exit(__doc__)
    stems = [os.path.splitext(os.path.basename(source))[0] for source, _ in builds]
    objects = [f'{s}.obj' for s in stems]
    stem = stems[0]
    xdk = xdk_dir()
    os.makedirs(OUT, exist_ok=True)
    for (source, cflags), obj in zip(builds, objects):
        run_tool('CL.Exe', ['/c', '/GL', *cflags, source, f'/Fo{obj}'], OUT, xdk)
    run_tool('Link.Exe', ['/LTCG', '/NODEFAULTLIB', '/ENTRY:entry', '/SUBSYSTEM:CONSOLE', '/MAP', '/FIXED:NO',
                          f'/OUT:{stem}.exe', *objects], OUT, xdk)

    symbols = {s.name: s.va for s in LinkMap.read(os.path.join(OUT, stem + '.map')).symbols}
    test = Pe(os.path.join(OUT, stem + '.exe'))
    retail = Xbe(retail_xbe_path())

    matched = 0
    for name, va in wanted:
        if name not in symbols:
            print(f'GONE   {name} (inlined or removed)')
            continue
        matched += compare(name, function(test.read, symbols[name]), function(retail.read, int(va, 16)),
                           test.fixups)
    print(f'{matched}/{len(wanted)} match')
    sys.exit(matched != len(wanted))


if __name__ == '__main__':
    main()
