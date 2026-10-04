"""Prints a retail function's disassembly, with its inventory row and the
names of what it calls. Jump tables inside the function print as data.

    python tools/disasm.py <va>
"""
import sys

from capstone import CS_ARCH_X86, CS_MODE_32, Cs

from inventory import check_retail, read_rows
from xbe import FUNCTIONS_CSV, load, retail_xbe_path


def main():
    if len(sys.argv) != 2:
        sys.exit('usage: python tools/disasm.py <va>')
    try:
        va = int(sys.argv[1], 16)
    except ValueError:
        sys.exit(f'{sys.argv[1]!r} is not a hexadecimal retail address')
    rows = read_rows(FUNCTIONS_CSV)
    row = rows.get(va)
    if row is None:
        raise SystemExit(f'{va:#x} is not a function start in config/functions.csv')
    path = retail_xbe_path()
    check_retail(path)
    image = load(path)
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
