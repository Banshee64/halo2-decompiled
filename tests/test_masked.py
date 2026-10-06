import struct

from masked import (bodies, explain, f32, hs_types, main, near_miss, numbers, script_differences,
                    split_top, strings)


def test_numbers_reads_floats_integers_and_macros():
    found = numbers('x * 0.5f + k_half + 3 + 0x10', {'k_half': '0.25f'})
    assert found == {0.5, 0.25, 3.0}


def test_strings_unescape_and_join_adjacent_literals():
    assert strings(r'a = "x\n" "y"; b = L"wide"; c = "q\"t";') == {'x\ny', 'wide', 'q"t'}


def test_near_miss_flags_a_value_one_float_step_away():
    square = f32(f32(0.1) * f32(0.1))
    assert near_miss(square, {0.01, 0.1}) == 0.01
    assert near_miss(f32(0.01), {0.01}) is None
    assert near_miss(0.5, {0.25}) is None
    assert near_miss(f32(3.4028234663852886e+38), {3.4028235e+38}) is None


def test_explain_finds_the_product_that_gives_the_value():
    assert explain(f32(f32(0.1) * f32(0.1)), {0.1, 0.01}) == '0.1f * 0.1f'
    assert explain(f32(f32(1.2) - f32(0.8)), {1.2, 0.8, 0.4}) == '1.2f - 0.8f'
    assert explain(0.123456, {0.5}) is None


def test_bodies_stop_at_the_first_lone_closing_brace():
    text = ('// @retail 0x10\nvoid a()\n{\n\tx = 1.5f;\n}\n\nreal g_20 = 2.5f;\n'
            '// @retail 0x30\nvoid b()\n{\n}\n')
    found = bodies('src/a.cpp', text)
    assert sorted(found) == [0x10, 0x30]
    path, line, body = found[0x10]
    assert (path, line) == ('src/a.cpp', 1)
    assert '1.5f' in body and '2.5f' not in body


def test_split_top_keeps_braces_strings_and_calls_whole():
    assert split_top('a, { b, c }, "d, e", f(g, h)') == ['a', '{ b, c }', '"d, e"', 'f(g, h)']


def test_hs_types_number_the_enumerators_from_zero():
    header = 'enum e_hs_type\n{\n\t_hs_type_unparsed = 0,\n\t_hs_type_special_form,\n\t_hs_type_void\n};'
    assert hs_types(header) == {'_hs_type_unparsed': 0, '_hs_type_special_form': 1, '_hs_type_void': 2}


class Image:
    def __init__(self, data):
        self.data = data

    def read(self, va, size):
        chunk = self.data.get(va, b'')
        return (chunk + bytes(size))[:size]


def test_script_differences_compare_each_field():
    types = {'_hs_type_void': 4, '_hs_type_real': 6, '_hs_type_point_reference': 28}
    image = Image({0x100: struct.pack('<hHIIhh', 4, 1, 0x2a6b30, 0x200, 1, 28), 0x200: b'<point>\0'})
    fields = split_top('_hs_type_void, 0, function_2a6b30, "<point>", 1, { _hs_type_real }')
    assert script_differences(fields, 0x100, image, types, {}) == [
        ('flags', '0', 1), ('parameter types', '{ _hs_type_real }', '{ _hs_type_point_reference }')]
    fields = split_top('_hs_type_void, 1, named, "<point>", 1, { _hs_type_point_reference }')
    assert script_differences(fields, 0x100, image, types, {'named': 0x2a6b30}) == []


def test_main_reads_the_retail_scripts(retail_xbe, capsys):
    assert main(['--scripts', '--xbe', retail_xbe]) in (0, 1)
    assert capsys.readouterr().out.rstrip().endswith('listed')
