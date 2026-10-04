"""Summarises functions whose status is near in config/functions.csv.

No XBE and no SDK: the report is the csv alone. One summary line, then one
line per likely object file (how many functions, how many bytes). --list
adds each function, smallest first. A "~" is a guess: the nearest object
named on an earlier row, the same rule as ready.py. Without the XBE that
guess can cross a section start.

    python tools/near.py [--list] [--csv config/functions.csv]
"""
import argparse
import sys

from inventory import read_rows
from ready import likely_objects
from xbe import FUNCTIONS_CSV


def near_rows(rows):
    """Near-status rows, smallest first."""
    found = [r for r in rows.values() if r.get('status') == 'near']
    return sorted(found, key=lambda r: (int(r['size']), r['va']))


def inventory(rows):
    """(near rows, {va: object}, [(object name, bytes, rows)]).

    The object name is '' when nothing names the function. A group is every
    near row that shares that name, guessed or written on the row. Largest
    total size first; a tie goes to a named object, then the name."""
    found = near_rows(rows)
    objects = likely_objects(rows, found)
    groups = {}
    for r in found:
        name = objects[int(r['va'], 16)].lstrip('~')
        groups.setdefault(name, []).append(r)
    ordered = []
    for name, group in groups.items():
        group = sorted(group, key=lambda r: (int(r['size']), r['va']))
        nbytes = sum(int(r['size']) for r in group)
        ordered.append((name, nbytes, group))
    ordered.sort(key=lambda g: (-g[1], g[0] == '', g[0]))
    return found, objects, ordered


def group_label(name, group, objects):
    """The object as printed. '~' when every row in the group is only a guess."""
    if not name:
        return '-'
    if all(objects[int(r['va'], 16)].startswith('~') for r in group):
        return '~' + name
    return name


def _n(n, word):
    return f'{n} {word}' + ('' if n == 1 else 's')


def format_report(found, objects, groups, list_functions=False):
    """The text near.py prints. Ends with a newline when there is any text."""
    nbytes = sum(int(r['size']) for r in found)
    lines = [f'near {_n(len(found), "function")}, {nbytes} bytes, {_n(len(groups), "object")}']
    for name, size, group in groups:
        label = group_label(name, group, objects)
        lines.append(f'{label} ({_n(len(group), "function")}, {size} bytes)')
        if list_functions:
            for r in group:
                obj = objects[int(r['va'], 16)] or '-'
                lines.append('  ' + ' '.join((r['va'], r['size'], obj, r['name'] or '-', r.get('source') or '-')))
    return '\n'.join(lines) + '\n'


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument('--list', action='store_true', help='print each near function under its object')
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
