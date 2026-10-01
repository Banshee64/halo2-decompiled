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

from xbe import Xbe

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
        read, write = (({PARTS.get(md.reg_name(r), md.reg_name(r)) for r in regs})
                       for regs in ins.regs_access())
        ops = ins.op_str.split(', ')
        if ins.mnemonic == 'push' or (ins.mnemonic == 'xor' and len(ops) == 2 and ops[0] == ops[1]):
            read = set()
        used = (read & WATCH) - written
        if used:
            return used
        written |= write
    return set()


def main():
    xbe = Xbe(sys.argv[1])
    text = xbe.section('.text')
    code = xbe.section_bytes(text)
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    md.detail = True
    md.skipdata = True

    targets = set()
    for ins in md.disasm(code, text.va):
        if ins.mnemonic == 'call' and ins.operands[0].type == x86.X86_OP_IMM:
            t = ins.operands[0].imm
            if text.va <= t < text.va + text.vsize:
                targets.add(t)

    hits = collections.Counter()
    examples = collections.defaultdict(list)
    users = 0
    for t in sorted(targets):
        off = t - text.va
        used = entry_register_args(md, code[off:off + 200], t)
        users += bool(used)
        for r in used:
            hits[r] += 1
            examples[r].append(t)
    print(f'direct call targets in .text: {len(targets)}')
    print(f'reading eax/ebx/esi/edi at entry: {users} ({100 * users / len(targets):.1f}%)')
    for r, n in hits.most_common():
        print(f'  {r}: {n}, e.g. {", ".join(hex(v) for v in examples[r][:3])}')


if __name__ == '__main__':
    main()
