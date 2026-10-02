"""Prints a retail function's disassembly, with its inventory row and the
names of what it calls. Jump tables inside the function print as data.

    python tools/disasm.py <va>
"""
import sys

from capstone import CS_ARCH_X86, CS_MODE_32, Cs

from inventory import check_retail, read_rows
from xbe import FUNCTIONS_CSV, Xbe, retail_xbe_path


def main():
    va = int(sys.argv[1], 16)
    rows = read_rows(FUNCTIONS_CSV)
    row = rows.get(va)
    if row is None:
        raise SystemExit(f'{va:#x} is not a function start in config/functions.csv')
    path = retail_xbe_path()
    check_retail(path)
    image = Xbe(path)
    print(f"{row['va']} size {row['size']} {row['owner']} {row['style']} ({row['evidence']}) {row['name'] or '-'}")
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    md.skipdata = True
    for address, size, mnemonic, op_str in md.disasm_lite(image.read(va, int(row['size'])), va):
        note = ''
        if mnemonic in ('call', 'jmp') and op_str.startswith('0x'):
            callee = rows.get(int(op_str, 16))
            if callee:
                note = f"   ; {callee['name'] or callee['va']} [{callee['status']}]"
        print(f'  {address:08x}  {mnemonic} {op_str}{note}')


if __name__ == '__main__':
    main()
