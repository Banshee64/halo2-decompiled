"""Summarises functions whose status is near in config/functions.csv.

No XBE and no SDK: the report is the csv alone. One summary line, then one
line per source file (how many near functions, how many bytes), largest
first. --list adds each function under its file, smallest first. A function
is near only after it was built and compared, so it has a source file; a row
without one is grouped under "-".

Only the va, size, status and source columns are read.

    python tools/near.py [--list] [--csv config/functions.csv]
"""
import argparse
import sys

from inventory import read_rows
from xbe import FUNCTIONS_CSV


def near_rows(rows):
    """Near-status rows, smallest first."""
    found = [r for r in rows.values() if r.get('status') == 'near']
    return sorted(found, key=lambda r: (int(r['size']), r['va']))


def inventory(rows):
    """(near rows, [(source file, bytes, rows)]).

    The source file is '' when the row has none. Largest total size first;
    a tie goes to a named file, then the name. Each group keeps the near
    rows' smallest-first order."""
    found = near_rows(rows)
    groups = {}
    for r in found:
        groups.setdefault((r.get('source') or '').replace('\\', '/'), []).append(r)
    ordered = [(source, sum(int(r['size']) for r in group), group)
               for source, group in groups.items()]
    ordered.sort(key=lambda g: (-g[1], g[0] == '', g[0]))
    return found, ordered


def _n(n, word):
    return f'{n} {word}' + ('' if n == 1 else 's')


def format_report(found, groups, list_functions=False):
    """The text near.py prints. Ends with a newline."""
    nbytes = sum(int(r['size']) for r in found)
    lines = [f'near {_n(len(found), "function")}, {nbytes} bytes, {_n(len(groups), "source file")}']
    for source, size, group in groups:
        lines.append(f'{source or "-"} ({_n(len(group), "function")}, {size} bytes)')
        if list_functions:
            lines += [f'  {r["va"]} {r["size"]}' for r in group]
    return '\n'.join(lines) + '\n'


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument('--list', action='store_true', help='print each near function under its source file')
    ap.add_argument('--csv', default=FUNCTIONS_CSV, help='functions csv (default: config/functions.csv)')
    args = ap.parse_args(argv)
    rows = read_rows(args.csv)
    if not rows:
        print(f'no rows in {args.csv}', file=sys.stderr)
        return 1
    sys.stdout.write(format_report(*inventory(rows), list_functions=args.list))
    return 0


if __name__ == '__main__':
    sys.exit(main())
