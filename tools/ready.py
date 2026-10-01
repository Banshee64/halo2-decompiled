"""Lists the game functions ready to decompile: not yet matched, and every
function they call is matched, belongs to a library, or is part of the same
mutual recursion. Smallest first, one line per function:
va, size, likely object file, name, calls. An object with a "~" is a guess: the
nearest object file named before the function (functions of one source file sit
together). --by-file groups the functions by likely object instead.

    python tools/ready.py [N] [--by-file]
"""
import argparse
import bisect
import os

from inventory import read_rows
from xbe import Xbe

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def components(graph):
    """Strongly connected components (Tarjan), iteratively."""
    index, low, on, stack, out, counter = {}, {}, set(), [], [], [0]
    for root in graph:
        if root in index:
            continue
        work = [(root, iter(graph.get(root, ())))]
        index[root] = low[root] = counter[0]; counter[0] += 1
        stack.append(root); on.add(root)
        while work:
            node, edges = work[-1]
            for nxt in edges:
                if nxt not in graph:
                    continue
                if nxt not in index:
                    index[nxt] = low[nxt] = counter[0]; counter[0] += 1
                    stack.append(nxt); on.add(nxt)
                    work.append((nxt, iter(graph.get(nxt, ()))))
                    break
                if nxt in on:
                    low[node] = min(low[node], index[nxt])
            else:
                work.pop()
                if work:
                    low[work[-1][0]] = min(low[work[-1][0]], low[node])
                if low[node] == index[node]:
                    group = set()
                    while True:
                        n = stack.pop(); on.discard(n); group.add(n)
                        if n == node:
                            break
                    out.append(group)
    return out


def ready(rows):
    def calls(va):
        text = rows[va]['calls']
        return {int(c, 16) for c in text.split()} if text else set()

    game = {va for va, r in rows.items() if r['owner'] == 'game'}
    graph = {va: calls(va) & game for va in game}
    result = []
    for group in components(graph):
        if all(rows[va]['status'] == 'matched' for va in group):
            continue
        outside = set().union(*(calls(va) for va in group)) - group
        if all(c not in rows or rows[c]['owner'] != 'game' or rows[c]['status'] == 'matched' for c in outside):
            result += [rows[va] for va in group if rows[va]['status'] != 'matched']
    return sorted(result, key=lambda r: (int(r['size']), r['va']))


def likely_objects(rows, ready_rows, boundaries=()):
    """{va: object} for the ready rows: the row's own object, else "~" and the
    object of the nearest preceding row (not across a section start in
    boundaries) that has one, else ''."""
    named = sorted(va for va, r in rows.items() if r.get('object'))
    cuts = sorted(boundaries)
    out = {}
    for r in ready_rows:
        va = int(r['va'], 16)
        if r.get('object'):
            out[va] = r['object']
            continue
        i = bisect.bisect_left(named, va)
        floor = cuts[bisect.bisect_right(cuts, va) - 1] if cuts and va >= cuts[0] else 0
        out[va] = '~' + rows[named[i - 1]]['object'] if i and named[i - 1] >= floor else ''
    return out


def by_file(ready_rows, objects):
    """[(object, rows)] (guessed and known objects of one name together), groups ordered by their smallest member, rows by size."""
    groups = {}
    for r in ready_rows:
        groups.setdefault(objects[int(r['va'], 16)].lstrip('~'), []).append(r)
    ordered = [(o, sorted(g, key=lambda r: (int(r['size']), r['va']))) for o, g in groups.items()]
    return sorted(ordered, key=lambda g: (int(g[1][0]['size']), g[1][0]['va']))


def line(r, objects):
    return ' '.join((r['va'], r['size'], objects[int(r['va'], 16)] or '-', r['name'] or '-', r['calls'] or '-'))


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument('count', nargs='?', type=int, default=20)
    ap.add_argument('--by-file', action='store_true')
    args = ap.parse_args()
    rows = read_rows(os.path.join(ROOT, 'config', 'functions.csv'))
    retail = os.environ.get('RETAIL_XBE', os.path.join(ROOT, 'orig', 'default.xbe'))
    boundaries = [s.va for s in Xbe(retail).sections] if os.path.exists(retail) else []
    shown = ready(rows)[:args.count]
    objects = likely_objects(rows, shown, boundaries)
    if args.by_file:
        for obj, group in by_file(shown, objects):
            print(f'{obj or "-"} ({len(group)})')
            for r in group:
                print('  ' + line(r, objects))
    else:
        for r in shown:
            print(line(r, objects))


if __name__ == '__main__':
    main()
