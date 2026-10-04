"""decomp.dev export from the function inventory. No retail XBE and no XDK."""
import json

import pytest

from decomp_dev_export import (
    build_report, in_scope, load_check_report, main, overlay_check_report,
)
from inventory import read_rows
from xbe import FUNCTIONS_CSV


def _row(va, size, owner='game', status='todo', name='', source='', object=''):
    return dict(va=f'{va:08x}', size=str(size), owner=owner, status=status, name=name,
                source=source, object=object, style='speed', evidence='', calls='')


def _rows(*rows):
    return {int(r['va'], 16): r for r in rows}


def test_game_scope_counts_matched_bytes_and_leaves_near_unmatched():
    rows = _rows(
        _row(0x10, 10, status='matched', source='src/a.cpp', name='alpha'),
        _row(0x20, 5, status='near', source='src/a.cpp', name='alpha'),  # same display name
        _row(0x30, 7, status='todo'),
        _row(0x40, 100, owner='xdk:libcmt', status='matched'),
        _row(0x50, 4, owner='third:havok', status='matched'),
        _row(0x60, 3, owner='eh', status='todo'),
    )
    report, info = build_report(rows, 'game')
    measures = report['measures']
    assert report['version'] == 2
    assert info == {'scope': 'game', 'overlaid': 0, 'unknown_check_entries': 0,
                    'matched': 1, 'near': 1, 'todo': 1}
    assert measures['total_code'] == '22'
    assert measures['matched_code'] == '10'
    assert measures['total_functions'] == 3
    assert measures['matched_functions'] == 1
    assert measures['total_units'] == 2
    assert measures['complete_units'] == 0
    assert measures['complete_code'] == '0'
    assert measures['total_data'] == '0'
    assert measures['matched_data'] == '0'
    assert isinstance(measures['total_code'], str)
    assert isinstance(measures['matched_functions'], int)
    assert isinstance(measures['matched_code_percent'], float)
    assert measures['matched_code_percent'] == pytest.approx(100 * 10 / 22)
    # Near has no recorded match length, so it adds nothing to fuzzy or matched.
    assert measures['fuzzy_match_percent'] == pytest.approx(measures['matched_code_percent'])

    by_name = {unit['name']: unit for unit in report['units']}
    assert set(by_name) == {'src/a.cpp', 'asm/unmatched'}
    source = by_name['src/a.cpp']
    assert source['metadata']['source_path'] == 'src/a.cpp'
    assert source['metadata']['complete'] is False
    assert source['metadata']['progress_categories'] == ['with-source']
    assert [fn['name'] for fn in source['functions']] == ['alpha', 'alpha_00000020']
    assert source['functions'][0]['fuzzy_match_percent'] == 100.0
    assert source['functions'][0]['metadata']['virtual_address'] == str(0x10)
    assert source['functions'][0]['metadata']['demangled_name'] == 'alpha'
    assert source['functions'][1]['fuzzy_match_percent'] == 0.0
    assert source['functions'][1]['size'] == '5'
    unmatched = by_name['asm/unmatched']
    assert unmatched['metadata']['progress_categories'] == ['no-source']
    assert unmatched['functions'][0]['fuzzy_match_percent'] == 0.0
    cats = {c['id']: c for c in report['categories']}
    assert set(cats) == {'no-source', 'with-source'}
    assert cats['with-source']['measures']['matched_code'] == '10'
    assert cats['no-source']['measures']['total_code'] == '7'


def test_complete_unit_when_every_function_matches():
    rows = _rows(_row(0x10, 8, status='matched', source='src/b.cpp', object='b.obj'))
    report, _info = build_report(rows, 'game')
    unit = report['units'][0]
    assert unit['metadata']['complete'] is True
    assert unit['measures']['complete_code'] == '8'
    assert unit['measures']['complete_units'] == 1
    assert report['measures']['complete_units'] == 1
    assert report['measures']['complete_code'] == '8'


def test_in_scope_keeps_xdk_and_drops_third_and_eh():
    rows = _rows(
        _row(0x10, 10, status='matched'),
        _row(0x40, 100, owner='xdk:libcmt', status='todo'),
        _row(0x50, 4, owner='third:havok', status='matched'),
        _row(0x60, 3, owner='eh'),
        _row(0x70, 9, owner='other:library', status='todo'),
    )
    assert [in_scope(r, 'in-scope') for r in rows.values()] == [True, True, False, False, True]
    report, info = build_report(rows, 'in-scope')
    assert info['matched'] == 1 and info['todo'] == 2
    assert report['measures']['total_code'] == '119'
    assert report['measures']['matched_code'] == '10'
    names = {unit['name'] for unit in report['units']}
    assert 'asm/unmatched [game]' in names
    assert 'asm/unmatched [xdk]' in names
    assert 'asm/unmatched [other]' in names
    assert all('havok' not in name and 'eh' not in name for name in names)
    cats = {c['id'] for c in report['categories']}
    assert {'game', 'xdk', 'other', 'no-source'} <= cats
    assert 'third' not in cats and 'eh' not in cats


def test_check_report_overlay_replaces_status_without_sizes():
    rows = _rows(_row(0x10, 10, status='todo', source='src/a.cpp'))
    check = {0x10: {'status': 'matched', 'source': 'src/a.cpp', 'size': 99},
             0x99: {'status': 'matched'}}
    overlaid, n, unknown = overlay_check_report(rows, check)
    assert rows[0x10]['status'] == 'todo'  # inventory not mutated
    assert n == 1 and unknown == 1
    assert overlaid[0x10]['status'] == 'matched'
    report, info = build_report(rows, 'game', check)
    assert info['overlaid'] == 1 and info['unknown_check_entries'] == 1
    assert report['measures']['matched_code'] == '10'  # CSV size, not the report's 99
    assert report['units'][0]['metadata']['complete'] is True


def test_load_check_report_rejects_an_objdiff_report(tmp_path):
    path = tmp_path / 'report.json'
    path.write_text(json.dumps({'measures': {}, 'units': []}), encoding='utf-8')
    with pytest.raises(SystemExit):
        load_check_report(str(path))


def test_cli_writes_report_and_does_not_need_an_xbe(tmp_path, capsys):
    csv_path = tmp_path / 'functions.csv'
    csv_path.write_text(
        'va,size,owner,style,evidence,name,object,calls,source,status\n'
        '00000010,4,game,speed,,,,,,matched\n',
        encoding='utf-8')
    out = tmp_path / 'decomp.dev.json'
    assert main(['--csv', str(csv_path), '-o', str(out), '--scope', 'game']) == 0
    report = json.loads(out.read_text(encoding='utf-8'))
    assert report['version'] == 2
    assert report['measures']['matched_functions'] == 1
    assert report['measures']['total_code'] == '4'
    err = capsys.readouterr().err
    assert 'needs a live check.py pass' in err
    assert 'near/todo' in err


def test_repo_inventory_matches_the_readme_headline():
    """The committed CSV is the checker's last summary. No XBE required.

    README's status block (game, then in-scope). Update these if functions.csv
    is regenerated; do not loosen them to hide a dropped row.
    """
    rows = read_rows(FUNCTIONS_CSV)
    game, _info = build_report(rows, 'game')
    scope, _info = build_report(rows, 'in-scope')
    assert game['measures']['matched_functions'] == 4940
    assert game['measures']['total_functions'] == 11321
    assert game['measures']['matched_code'] == '483661'
    assert game['measures']['total_code'] == '2785198'
    assert game['measures']['matched_code_percent'] == pytest.approx(100 * 483661 / 2785198)
    assert scope['measures']['matched_functions'] == 4940
    assert scope['measures']['total_functions'] == 17215
    assert scope['measures']['matched_code'] == '483661'
    assert scope['measures']['total_code'] == '3739274'
    assert scope['measures']['matched_code_percent'] == pytest.approx(100 * 483661 / 3739274)
    # Every matched game byte is inside some unit, and near bytes are not matched.
    unit_matched = sum(int(u['measures']['matched_code']) for u in game['units'])
    assert unit_matched == 483661
    near = sum(1 for u in game['units'] for fn in u['functions'] if fn['fuzzy_match_percent'] == 0.0)
    assert near == 6251 + 130  # todo + near
