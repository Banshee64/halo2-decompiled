import random
import re

import pytest

import permute

SOURCE = '''// header { not a body
// @retail 0x1234
long f(long a, long b)
{
	long x = a + b;   // "}" in a comment
	long y = 2;
	char const *s = "}{";
	if (x == y) {
		x += 1;
	} else {
		x = x - 1;
	}
	for (y = 0; y < 4; y++) {
		x ^= y;
	}
	return (x + y) & ~3;
}

long g(void) { return 0; }
'''


def body():
    start, end = permute.find_body(SOURCE, 0x1234)
    return SOURCE[start:end]


def rng():
    return random.Random(1)


def test_find_body_braces():
    b = body()
    assert b.startswith('\n\tlong x = a + b;') and b.endswith('~3;\n')
    assert 'g(void)' not in b


def test_find_body_after_a_standard_marker():
    text = SOURCE.replace('// @retail 0x1234', '// @retail 0x1234 standard')
    start, end = permute.find_body(text, 0x1234)
    assert text[start:end] == body()


def test_find_body_missing():
    with pytest.raises(ValueError):
        permute.find_body(SOURCE, 0x99)


def test_swap_operands_commutative():
    out = permute.mut_swap_operands('\n\tx = a + b;\n', rng())
    assert out == '\n\tx = b + a;\n'
    assert permute.mut_swap_operands('\n\tx = a - b;\n', rng()) is None
    assert permute.mut_swap_operands('\n\tx = c * a + b;\n', rng()) is None
    assert permute.mut_swap_operands('\n\tif (a < b) f();\n', rng()) == '\n\tif (b > a) f();\n'


def test_reorder_independent_only():
    out = permute.mut_reorder('\n\tlong a = 1;\n\tlong b = 2;\n', rng())
    assert out == '\n\tlong b = 2;\n\tlong a = 1;\n'
    assert permute.mut_reorder('\n\tlong a = 1;\n\tlong b = a;\n', rng()) is None


def test_retype_changes_one_declaration():
    out = permute.mut_retype('\n\tlong a = 1;\n\treturn a;\n', rng())
    assert out != '\n\tlong a = 1;\n\treturn a;\n' and out.endswith('return a;\n')
    assert re.match(r'\n\t(int|short|unsigned long|dword) a = 1;', out)
    assert re.match(r'\n\tbyte c;', permute.mut_retype('\n\tchar c;\n', rng()))


def test_introduce_and_inline_temp_roundtrip():
    src = '\n\tlong r = (a + b) & ~3;\n\treturn r;\n'
    out = permute.mut_introduce_temp(src, rng())
    assert re.search(r'tmp0 = a \+ b;', out) and 'tmp0 & ~3' in out
    inlined = permute.mut_inline_temp('\n\tlong t = a + b;\n\treturn t;\n', rng())
    assert inlined == '\n\treturn (a + b);\n'
    assert permute.mut_inline_temp('\n\tlong t = a + b;\n\tt += 1;\n\treturn t;\n', rng()) is None


def test_swap_if_else_negates():
    src = '\n\tif (a == b) {\n\t\tx = 1;\n\t} else {\n\t\tx = 2;\n\t}\n'
    out = permute.mut_swap_if_else(src, rng())
    assert 'if (a != b)' in out
    assert out.index('x = 2;') < out.index('x = 1;')
    assert permute.mut_swap_if_else('\n\tif (a) {\n\t\tx = 1;\n\t}\n', rng()) is None


def test_for_while_roundtrip():
    src = '\n\tfor (i = 0; i < 4; i++) {\n\t\tx += i;\n\t}\n'
    w = permute.mut_for_while(src, rng())
    assert 'while (i < 4) {' in w and w.count('i++;') == 1 and 'for' not in w
    assert permute.mut_for_while(w, rng()) == src


def test_compound_assignment_both_ways():
    assert permute.mut_compound('\n\tx += y;\n', rng()) == '\n\tx = x + y;\n'
    assert permute.mut_compound('\n\tx = x + y;\n', rng()) == '\n\tx += y;\n'
    assert permute.mut_compound('\n\tx += y + 1;\n', rng()) == '\n\tx = x + (y + 1);\n'


def test_increment_forms():
    assert permute.mut_increment('\n\ti++;\n', rng()) == '\n\t++i;\n'
    assert permute.mut_increment('\n\t--i;\n', rng()) == '\n\ti--;\n'
    assert 'for (i = 0; i < 4; ++i)' in permute.mut_increment('\n\tfor (i = 0; i < 4; i++) {\n', rng())


def test_split_decl_both_ways():
    assert permute.mut_split_decl('\n\tlong a = b;\n', rng()) == '\n\tlong a;\n\ta = b;\n'
    assert permute.mut_split_decl('\n\tlong a;\n\ta = b;\n', rng()) == '\n\tlong a = b;\n'


def test_mutate_is_deterministic_and_valid_looking():
    b = body()
    first = [permute.mutate(b, random.Random(7)) for _ in range(3)]
    assert first[0] == first[1] == first[2] and first[0] != b
    for seed in range(30):
        out = permute.mutate(b, random.Random(seed))
        assert out is None or all(out.count(c) == b.count(c) for c in '{}()')


@pytest.mark.sdk
@pytest.mark.retail
def test_permute_runs_on_game_state_malloc_aligned(retail_xbe, xdk_dir):
    result = permute.run(0x123d80, tries=3, seed=1, log=lambda *_: None)
    assert result.tries >= 1
    assert 0 <= result.best <= result.baseline
