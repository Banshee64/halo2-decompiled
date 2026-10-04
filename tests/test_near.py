import csv

from near import format_report, inventory, main, near_rows
from inventory import read_rows
from xbe import FUNCTIONS_CSV


def row(va, status='near', size=10, source='', owner='game'):
    return dict(va=f'{va:08x}', size=str(size), owner=owner, status=status,
                source=source, calls='', style='speed', evidence='')


def test_near_rows_skip_other_statuses_and_sort_by_size():
    rows = {
        0x10: row(0x10, size=8),
        0x20: row(0x20, status='matched', size=1),
        0x30: row(0x30, status='todo', size=2),
        0x40: row(0x40, size=3),
    }
    assert [r['va'] for r in near_rows(rows)] == ['00000040', '00000010']


def test_inventory_groups_by_source_file_largest_first():
    rows = {
        0x10: row(0x10, size=4, source='src/a.cpp'),
        0x20: row(0x20, size=8, source='src/b.cpp'),
        0x30: row(0x30, size=1, source='src/a.cpp'),
        0x40: row(0x40, status='matched', size=99, source='src/a.cpp'),
    }
    found, groups = inventory(rows)
    assert [r['va'] for r in found] == ['00000030', '00000010', '00000020']
    assert [(source, size) for source, size, _ in groups] == [('src/b.cpp', 8), ('src/a.cpp', 5)]
    assert [r['va'] for r in groups[1][2]] == ['00000030', '00000010']


def test_inventory_orders_equal_sizes_by_name_and_leaves_no_source_last():
    rows = {
        0x10: row(0x10, size=5),
        0x20: row(0x20, size=5, source='src/b.cpp'),
        0x30: row(0x30, size=5, source='src/a.cpp'),
    }
    _, groups = inventory(rows)
    assert [source for source, _, _ in groups] == ['src/a.cpp', 'src/b.cpp', '']


def test_format_report_summary_and_list():
    rows = {
        0x10: row(0x10, status='todo', size=100, source='src/a.cpp'),
        0x20: row(0x20, size=4, source='src/a.cpp'),
        0x30: row(0x30, size=6),
    }
    text = format_report(*inventory(rows))
    assert text == ('near 2 functions, 10 bytes, 2 source files\n'
                    '- (1 function, 6 bytes)\n'
                    'src/a.cpp (1 function, 4 bytes)\n')
    listed = format_report(*inventory(rows), list_functions=True)
    assert listed == ('near 2 functions, 10 bytes, 2 source files\n'
                      '- (1 function, 6 bytes)\n'
                      '  00000030 6\n'
                      'src/a.cpp (1 function, 4 bytes)\n'
                      '  00000020 4\n')
    assert format_report([], []) == 'near 0 functions, 0 bytes, 0 source files\n'


def test_main_reads_a_csv(tmp_path, capsys):
    path = tmp_path / 'functions.csv'
    with path.open('w', newline='') as fh:
        w = csv.DictWriter(fh, list(row(0).keys()))
        w.writeheader()
        w.writerow(row(0x10, size=12, source='src/z.cpp'))
        w.writerow(row(0x20, status='matched', size=99, source='src/z.cpp'))
    assert main(['--csv', str(path), '--list']) == 0
    out = capsys.readouterr().out
    assert out == 'near 1 function, 12 bytes, 1 source file\nsrc/z.cpp (1 function, 12 bytes)\n  00000010 12\n'


def test_main_rejects_an_empty_csv(tmp_path, capsys):
    path = tmp_path / 'empty.csv'
    path.write_text('va,size,status\n')
    assert main(['--csv', str(path)]) == 1
    err = capsys.readouterr().err
    assert 'no rows' in err


def test_committed_csv_near_rows_are_the_report():
    rows = read_rows(FUNCTIONS_CSV)
    found, groups = inventory(rows)
    assert found
    assert len(found) == sum(1 for r in rows.values() if r['status'] == 'near')
    assert sum(int(r['size']) for r in found) == sum(size for _, size, _ in groups)
    seen = [r['va'] for _, _, group in groups for r in group]
    assert len(seen) == len(set(seen)) == len(found)
    for source, _, group in groups:
        assert all((r['source'] or '').replace('\\', '/') == source for r in group)
        sizes = [(int(r['size']), r['va']) for r in group]
        assert sizes == sorted(sizes)
