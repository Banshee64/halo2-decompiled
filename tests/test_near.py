import csv

from near import format_report, group_label, inventory, main, near_rows
from inventory import read_rows
from xbe import FUNCTIONS_CSV


def row(va, status='near', size=10, name='', object='', source='', owner='game'):
    return dict(va=f'{va:08x}', size=str(size), owner=owner, status=status, name=name,
                object=object, source=source, calls='', style='speed', evidence='')


def test_near_rows_skip_other_statuses_and_sort_by_size():
    rows = {
        0x10: row(0x10, size=8),
        0x20: row(0x20, status='matched', size=1),
        0x30: row(0x30, status='todo', size=2),
        0x40: row(0x40, size=3),
    }
    assert [r['va'] for r in near_rows(rows)] == ['00000040', '00000010']


def test_inventory_uses_own_object_or_nearest_preceding():
    rows = {
        0x10: row(0x10, status='matched', object='early.obj'),
        0x20: row(0x20, size=4),
        0x30: row(0x30, size=8, object='own.obj', name='named'),
        0x40: row(0x40, size=1),
    }
    found, objects, groups = inventory(rows)
    assert objects == {0x20: '~early.obj', 0x30: 'own.obj', 0x40: '~own.obj'}
    assert [(name, size) for name, size, _ in groups] == [('own.obj', 9), ('early.obj', 4)]
    assert group_label('early.obj', groups[1][2], objects) == '~early.obj'
    assert group_label('own.obj', groups[0][2], objects) == 'own.obj'


def test_inventory_orders_equal_sizes_by_name_and_leaves_unnamed_last():
    rows = {
        0x10: row(0x10, size=5),                 # before any named object
        0x20: row(0x20, size=5, object='b.obj'),
        0x30: row(0x30, size=5, object='a.obj'),
    }
    _, _, groups = inventory(rows)
    assert [name for name, _, _ in groups] == ['a.obj', 'b.obj', '']


def test_format_report_summary_and_list():
    rows = {
        0x10: row(0x10, status='todo', object='early.obj', size=100),
        0x20: row(0x20, size=4, source='src/a.cpp'),
    }
    text = format_report(*inventory(rows))
    assert text == 'near 1 function, 4 bytes, 1 object\n~early.obj (1 function, 4 bytes)\n'
    listed = format_report(*inventory(rows), list_functions=True)
    assert '  00000020 4 ~early.obj - src/a.cpp' in listed
    assert format_report([], {}, []) == 'near 0 functions, 0 bytes, 0 objects\n'


def test_main_reads_a_csv(tmp_path, capsys):
    path = tmp_path / 'functions.csv'
    with path.open('w', newline='') as fh:
        w = csv.DictWriter(fh, ['va', 'size', 'owner', 'style', 'evidence', 'name', 'object', 'calls', 'source', 'status'])
        w.writeheader()
        w.writerow(row(0x10, object='z.obj', size=12, name='fn'))
        w.writerow(row(0x20, status='matched', size=99, object='z.obj'))
    assert main(['--csv', str(path), '--list']) == 0
    out = capsys.readouterr().out
    assert out.startswith('near 1 function, 12 bytes, 1 object\n')
    assert 'z.obj (1 function, 12 bytes)' in out
    assert '00000010 12 z.obj fn -' in out


def test_main_rejects_an_empty_csv(tmp_path, capsys):
    path = tmp_path / 'empty.csv'
    path.write_text('va,size,status\n')
    assert main(['--csv', str(path)]) == 1
    err = capsys.readouterr().err
    assert 'no rows' in err


def test_committed_csv_near_rows_are_the_report():
    rows = read_rows(FUNCTIONS_CSV)
    found, objects, groups = inventory(rows)
    assert found
    assert len(found) == sum(1 for r in rows.values() if r['status'] == 'near')
    assert sum(int(r['size']) for r in found) == sum(size for _, size, _ in groups)
    seen = [r['va'] for _, _, group in groups for r in group]
    assert len(seen) == len(set(seen)) == len(found)
    assert set(objects) == {int(r['va'], 16) for r in found}
    for _, _, group in groups:
        sizes = [(int(r['size']), r['va']) for r in group]
        assert sizes == sorted(sizes)
