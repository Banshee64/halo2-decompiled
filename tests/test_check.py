import json

import pytest

from build import Marked
from check import (Identity, check_function, check_unique_markers, compare, extract, masked_offsets, parse_addresses,
                   relative_ok, resolve, write_report)
from linkmap import LinkMap

MAP = """ Preferred load address is 00400000
 0001:00000000 00000100H .text                   CODE
 0001:00000000       ?f@@YIXH@Z                 00401000 f   a.obj
 0001:00000040       ?f@@YIXM@Z                 00401040 f   a.obj
 0001:00000080       ?g@@YIXXZ                  00401080 f   a.obj
"""


def marked(name):
    return Marked('src/a.cpp', 0x1000, name, 'void', [])


def test_resolve_symbol():
    assert resolve(LinkMap(MAP), marked('g')).va == 0x401080


def test_resolve_symbol_ambiguous():
    with pytest.raises(SystemExit) as e:
        resolve(LinkMap(MAP), marked('f'))
    assert '?f@@YIXH@Z' in str(e.value) and '?f@@YIXM@Z' in str(e.value)


def test_resolve_symbol_missing_is_none():
    # folded into an identical function or dropped: reported as todo, not fatal
    assert resolve(LinkMap(MAP), marked('h')) is None


def test_extract_strips_fill_after_retail_length():
    assert extract(bytes.fromhex('c3cccccc'), bytes.fromhex('c3')) == bytes.fromhex('c3')


def test_extract_keeps_real_cc_bytes():
    assert extract(bytes.fromhex('ebcccccc'), bytes.fromhex('ebcc')) == bytes.fromhex('ebcc')


def test_compare_masks_only_listed_fields():
    ours = bytes.fromhex('a1' '00204000' 'c3')
    theirs = bytes.fromhex('a1' '88e75500' 'c3')
    assert compare(ours, theirs, masked_offsets(0x401000, 6, {0x401001})) is None
    assert compare(ours, theirs, set()) == 1


def test_compare_unequal_lengths():
    assert compare(bytes.fromhex('33c0c3'), bytes.fromhex('33c0'), set()) == 2


def test_relative_fields_must_leave_in_retail():
    call_out = bytes.fromhex('e8' '10000000' 'c3')     # call va+0x15, outside a 6-byte function
    jump_self = bytes.fromhex('e9' 'fbffffff' 'c3')    # jmp va, inside
    assert relative_ok(call_out, 0x401000, call_out, 0x1000, 1)
    assert not relative_ok(call_out, 0x401000, jump_self, 0x1000, 1)


LO, HI = 0x10000, 0x600000


def run(full, start, theirs, theirs_va=0x1000, absolute=(), relative=()):
    return check_function(full, start, theirs, theirs_va, set(absolute), set(relative), LO, HI)


def test_absolute_field_must_be_an_address_in_retail():
    ours = bytes.fromhex('b8' '00204000' 'c3')
    status, first, _, _ = run(ours, 0x401000, bytes.fromhex('b8' '2a000000' 'c3'), absolute={0x401001})
    assert first == 1 and status == 'near'
    assert run(ours, 0x401000, bytes.fromhex('b8' '00204000' 'c3'), absolute={0x401001})[:2] == ('matched', None)


def test_field_straddling_the_start_is_not_masked():
    status, first, _, _ = run(bytes.fromhex('c3'), 0x401000, bytes.fromhex('90'), relative={0x400ffd})
    assert first == 0 and status == 'near'


def test_disjoint_field_is_ignored():
    assert run(bytes.fromhex('c3'), 0x401000, bytes.fromhex('c3'), relative={0x400ff0})[1] is None


def test_failed_relative_field_checked_even_when_bytes_equal():
    jump_self = bytes.fromhex('e9' 'fbffffff' 'c3')
    jump_out = bytes.fromhex('e9' '10000000' 'c3')
    status, first, _, _ = run(jump_out, 0x401000, jump_self, 0x1000, relative={0x401001})
    assert first == 1 and status == 'near'


def test_failed_field_counts_as_differing_instruction():
    # two nops differ from retail; with the failed jmp that is three instructions
    ours = bytes.fromhex('e9' '10000000' '90' '90' 'c3')
    theirs = bytes.fromhex('e9' 'fbffffff' '91' '91' 'c3')
    assert run(ours, 0x401000, theirs, relative={0x401001})[0] == 'todo'
    same_jmp = theirs[:5] + ours[5:]
    assert run(same_jmp, 0x401000, theirs)[0] == 'near'


def test_incomplete_disassembly_is_todo():
    # 0f is not decodable on its own
    assert run(bytes.fromhex('0f'), 0x401000, bytes.fromhex('c3'))[0] == 'todo'


def test_ours_longer_with_fill_compares_retail_length():
    assert run(bytes.fromhex('ebcc' 'cccc'), 0x401000, bytes.fromhex('ebcc'))[:2] == ('matched', None)


@pytest.mark.parametrize('opcode, fixups', [('b8', 'absolute'), ('e8', 'relative')])
def test_field_beyond_retail_length_is_not_masked(opcode, fixups):
    ours = bytes.fromhex(opcode + '00204000' + 'c3')
    status, first, _, _ = run(ours, 0x401000, bytes.fromhex(opcode + '90' + 'c3'), **{fixups: {0x401001}})
    assert first == 1 and status == 'todo'


def test_jump_table_entries_must_map_to_the_same_offsets():
    # jmp [eax*4+table]; three case bodies (nop, nop, ret); table of three absolute entries
    def func(base, order):
        body = bytes.fromhex('ff2485' + (base + 12).to_bytes(4, 'little').hex() + '90' '90' 'c3')
        # code is 10 bytes (0..9), table at 12? pad two bytes so the table sits at offset 12
        return body + bytes.fromhex('9090') + b''.join((base + o).to_bytes(4, 'little') for o in order)
    ours = func(0x401000, (7, 8, 9))
    fields = {0x401000 + 12, 0x401000 + 16, 0x401000 + 20, 0x401000 + 3}
    same = func(0x1000, (7, 8, 9))
    assert run(ours, 0x401000, same, absolute=fields)[:2] == ('matched', None)
    swapped = func(0x1000, (7, 9, 8))
    assert run(ours, 0x401000, swapped, absolute=fields)[1] == 16


def test_self_recursive_call_matches_when_retail_calls_itself():
    ours = bytes.fromhex('e8' 'fbffffff' 'c3')      # call own start
    assert run(ours, 0x401000, ours, relative={0x401001})[:2] == ('matched', None)


def test_call_elsewhere_inside_retail_is_a_difference():
    ours = bytes.fromhex('e8' 'fbffffff' 'c3')
    theirs = bytes.fromhex('e8' '00000000' 'c3')    # calls its own ret
    assert run(ours, 0x401000, theirs, relative={0x401001})[1] == 1


def test_parse_addresses_rejects_non_hex():
    assert parse_addresses(['163ba0', '0x259d0']) == {0x163ba0, 0x259d0}
    with pytest.raises(SystemExit) as e:
        parse_addresses(['crc_checksum_buffer'])
    assert 'not a hexadecimal' in str(e.value)


def test_duplicate_retail_markers_are_rejected():
    a, b = Marked('src/a.cpp', 0x1000, 'f', 'void', []), Marked('src/b.cpp', 0x1000, 'g', 'void', [])
    check_unique_markers([a, Marked('src/a.cpp', 0x2000, 'h', 'void', [])])
    with pytest.raises(SystemExit) as e:
        check_unique_markers([a, b])
    assert 'duplicate @retail 0x1000' in str(e.value)


def test_filtered_run_merges_into_the_report(tmp_path):
    path = str(tmp_path / 'report.json')
    write_report(path, {'a': 1, 'b': 1}, merge=False)
    write_report(path, {'b': 2}, merge=True)
    assert json.load(open(path)) == {'a': 1, 'b': 2}
    write_report(path, {'c': 3}, merge=False)
    assert json.load(open(path)) == {'c': 3}


IDENT_MAP = """ Preferred load address is 00400000
 0001:00000000 00000300H .text                   CODE
 0001:00000000       ?caller@@YAXXZ             00401000 f   a.obj
 0001:00000100       ?crc_checksum_buffer@@YIXPAK@Z 00401100 f   a.obj
 0001:00000200       _strncmp                   00401200 f   libcmt:strncmp.obj
"""
CALL_START = 0x401000
CALLEE = Marked('src/a.cpp', 0x2100, 'crc_checksum_buffer', 'void', ['unsigned long *'])


def call_to(base, target):
    """A function at base that calls target, then returns."""
    return bytes.fromhex('e8') + ((target - (base + 5)) & 0xFFFFFFFF).to_bytes(4, 'little') + bytes.fromhex('c3')


def run_call(retail_target, markers=(CALLEE,), rows=None, our_target=0x401100):
    identity = Identity(LinkMap(IDENT_MAP), rows or {}, list(markers))
    return check_function(call_to(CALL_START, our_target), CALL_START, call_to(0x1000, retail_target), 0x1000,
                          set(), {CALL_START + 1}, LO, HI, identity)


def test_call_to_a_marked_callee_matches_when_retail_targets_its_address():
    assert run_call(0x2100)[:2] == ('matched', None)


def test_call_to_a_marked_callee_with_the_wrong_address_differs_at_the_call_field():
    assert run_call(0x2200)[1] == 1


def test_call_to_a_marked_retail_function_must_reach_its_symbol():
    # retail calls 0x2100, which src/ claims; ours calls something else
    assert run_call(0x2100, our_target=0x401200)[1] == 1


def test_call_to_a_library_function_matches_by_plain_name():
    rows = {0x2200: {'name': '_strncmp'}}
    assert run_call(0x2200, markers=(), rows=rows, our_target=0x401200)[:2] == ('matched', None)


def test_call_to_a_library_function_with_another_name_differs():
    rows = {0x2200: {'name': '_strcmp'}}
    assert run_call(0x2200, markers=(), rows=rows, our_target=0x401200)[1] == 1


def test_call_between_unnamed_functions_is_accepted():
    assert run_call(0x2200, markers=(), rows={0x2200: {'name': ''}}, our_target=0x401200)[:2] == ('matched', None)
