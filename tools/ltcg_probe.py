"""Measures link-time code generation (LTCG) in an XBE's .text.

    python tools/ltcg_probe.py <default.xbe>

Counts the direct call targets whose first instructions read eax, ebx, esi or
edi (as data, not by saving them with push) before writing them. MSVC's
standard conventions (cdecl, stdcall, fastcall, thiscall) never pass
arguments in those registers; LTCG's custom conventions for internal
functions do.
"""
import collections
import sys

from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone import x86

from xbe import load

WATCH = {'eax', 'ebx', 'esi', 'edi'}
PARTS = {'al': 'eax', 'ah': 'eax', 'ax': 'eax', 'bl': 'ebx', 'bh': 'ebx', 'bx': 'ebx',
         'si': 'esi', 'di': 'edi'}


def entry_register_args(md, code, va):
    """The watched registers the function at va reads before writing."""
    written = set()
    for n, ins in enumerate(md.disasm(code, va)):
        if (n >= 40 or ins.id == 0  # data, not an instruction
                or ins.mnemonic in ('ret', 'call') or ins.mnemonic.startswith('j')):
            break
        read, write = ({PARTS.get(name, name) for name in map(md.reg_name, regs)} for regs in ins.regs_access())
        ops = ins.operands
        zeroing = (ins.mnemonic == 'xor' and len(ops) == 2 and ops[0].type == ops[1].type == x86.X86_OP_REG
                   and ops[0].reg == ops[1].reg)
        if ins.mnemonic == 'push' or zeroing:
            read = set()
        used = (read & WATCH) - written
        if used:
            return used
        written |= write
    return set()


def main():
    if len(sys.argv) != 2:
        sys.exit('usage: python tools/ltcg_probe.py <default.xbe>')
    xbe = load(sys.argv[1])
    text = xbe.section('.text')
    code = xbe.section_bytes(text)
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    md.detail = True
    md.skipdata = True

    # the sweep needs no operand detail, and is much faster without it
    targets = set()
    for _, _, mnemonic, op_str in md.disasm_lite(code, text.va):
        if mnemonic == 'call' and op_str.startswith('0x'):
            t = int(op_str, 16)
            if text.va <= t < text.va + text.vsize:
                targets.add(t)

    examples = collections.defaultdict(list)
    users = 0
    for t in sorted(targets):
        off = t - text.va
        used = entry_register_args(md, code[off:off + 200], t)
        users += bool(used)
        for r in used:
            examples[r].append(t)
    print(f'direct call targets in .text: {len(targets)}')
    print(f'reading eax/ebx/esi/edi at entry: {users} ({100 * users / len(targets):.1f}%)')
    for r, ts in sorted(examples.items(), key=lambda item: len(item[1]), reverse=True):
        print(f'  {r}: {len(ts)}, e.g. {", ".join(hex(v) for v in ts[:3])}')


if __name__ == '__main__':
    main()
