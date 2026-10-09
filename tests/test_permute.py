import random
import re

import pytest

import build
import permute
from linkmap import LinkMap

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


def test_missing_symbol_is_a_failed_variant_not_a_crash(monkeypatch, tmp_path):
    """A variant the linker folds out of the image scores as a failure.

    check.resolve returns None for that. Scoring used to call extent() on it
    and raise AttributeError, which aborts the whole search.
    """
    scorer = permute.Scorer.__new__(permute.Scorer)
    scorer.va = 0x1000
    scorer.root = str(tmp_path)
    scorer.xdk = None
    scorer.image = None
    scorer.path = str(tmp_path / 'f.cpp')
    scorer.row = {'size': '4'}
    scorer.rows = {}
    (tmp_path / 'f.cpp').write_text('original')

    monkeypatch.setattr(permute.build, 'build', lambda *a, **k: 'map')
    monkeypatch.setattr(permute.LinkMap, 'read', staticmethod(lambda path: object()))
    monkeypatch.setattr(permute, 'Pe', lambda path: object())
    monkeypatch.setattr(permute.build, 'marked_sources', lambda root: [type('M', (), {'retail': 0x1000})()])
    monkeypatch.setattr(permute.build, 'stub_sources', lambda root: [])
    monkeypatch.setattr(permute.check, 'StandinCalls', lambda *a, **k: object())
    monkeypatch.setattr(permute.check, 'Identity', lambda *a, **k: object())
    monkeypatch.setattr(permute.check, 'resolve', lambda *a, **k: None)

    assert scorer.score_text('variant') == (permute.WORST, 'not in the image')
    assert (tmp_path / 'f.cpp').read_text() == 'variant'


# ------------------------------------------------------------ --fast

def marker(va, path, stub=False):
    return build.Marked(path, va, f'function_{va:x}', 'void', [], stub)


# 0x100 calls 0x200 (decompiled) and 0x300 (a stub); 0x200 calls 0x400, which
# calls 0x500; 0x300's own callee 0x600 must not be followed (stubs are built
# without LTCG); 0x700 (library code) has no marker; 0x800 calls 0x100.
CALLS = {0x100: '00000200 00000300 00000700', 0x200: '00000400', 0x300: '00000600', 0x400: '00000500',
         0x500: '', 0x600: '', 0x700: '', 0x800: '00000100'}
ROWS = {va: {'va': f'{va:08x}', 'calls': calls} for va, calls in CALLS.items()}
MARKERS = [marker(0x100, 'src/a.cpp'), marker(0x200, 'src/b.cpp'), marker(0x300, 'src/stubs/s.cpp', stub=True),
           marker(0x400, 'src/c.cpp'), marker(0x500, 'src/d.cpp'), marker(0x600, 'src/e.cpp'),
           marker(0x800, 'src/f.cpp')]


def test_reduced_units_follow_retail_callees_to_the_given_depth():
    assert permute.reduced_units(0x100, ROWS, MARKERS, depth=0) == {'src/a.cpp'}
    assert permute.reduced_units(0x100, ROWS, MARKERS, depth=1) == {'src/a.cpp', 'src/b.cpp', 'src/stubs/s.cpp'}
    assert permute.reduced_units(0x100, ROWS, MARKERS, depth=2) == {'src/a.cpp', 'src/b.cpp', 'src/stubs/s.cpp',
                                                                    'src/c.cpp'}
    assert permute.reduced_units(0x100, ROWS, MARKERS) == {'src/a.cpp', 'src/b.cpp', 'src/stubs/s.cpp',
                                                           'src/c.cpp', 'src/d.cpp'}


def test_reduced_units_stop_at_stubs_and_leave_out_callers():
    units = permute.reduced_units(0x100, ROWS, MARKERS)
    assert 'src/e.cpp' not in units and 'src/f.cpp' not in units


def test_reduced_units_survive_call_cycles():
    rows = {0x100: {'calls': '00000200'}, 0x200: {'calls': '00000100'}}
    markers = [marker(0x100, 'src/a.cpp'), marker(0x200, 'src/b.cpp')]
    assert permute.reduced_units(0x100, rows, markers) == {'src/a.cpp', 'src/b.cpp'}


MAP = '''  Address         Publics by Value              Rva+Base     Lib:Object

 0000:00000000       ___safe_se_handler_table   00000000     <absolute>
 0001:00000000       ?sqrt@@YAMM@Z              00401000 f i ai.obj
 0001:00000010       ??1CLocalTalker@XHVNamespace@@QAE@XZ 00401010 f   xvoice:localtalker.obj
 0001:00000020       ?Talk@CLocalTalker@XHVNamespace@@QAEXXZ 00401020 f   xvoice:localtalker.obj
 0001:00000030       __fpclear                  00401030 f   libcmt:fpinit.obj
 0001:00000040       _D3DDevice_Clear@24        00401040 f   d3d8ltcg:device.obj

 Static symbols

 0001:00000050       _helper                    00401050 f   libcmt:other.obj
'''


def test_library_symbols_take_one_public_symbol_per_non_ltcg_library_object():
    assert permute.library_symbols(LinkMap(MAP)) == ['??1CLocalTalker@XHVNamespace@@QAE@XZ', '__fpclear']


def search(full, log=None):
    """A FastSearch over body 'B' (full score 4) whose confirming build scores with full (a dict)."""
    confirmed = []

    def confirm(text):
        confirmed.append(text)
        return full[text], f'full {text}'

    s = permute.FastSearch('<', 'B', '>', confirm, 4, log or (lambda *_: None))
    s.start(5, 'fast baseline')
    return s, confirmed


def test_fast_improvement_is_confirmed_by_a_full_build():
    lines = []
    s, confirmed = search({'<C>': 2}, lines.append)
    s.offer('C', 3, 'fast C', 1)
    assert confirmed == ['<C>']
    assert (s.baseline, s.best, s.best_score, s.walk, s.walk_score) == (4, 'C', 2, 'C', 3)
    assert s.confirmations == 1
    assert any('3 on the reduced image' in l and '2 in the full image' in l and 'new best' in l for l in lines)


def test_fast_improvement_the_full_build_rejects_moves_nothing():
    lines = []
    s, confirmed = search({'<C>': 4}, lines.append)
    s.offer('C', 3, 'fast C', 1)
    assert (s.best, s.best_score, s.walk, s.walk_score) == ('B', 4, 'B', 5)
    assert any('3 on the reduced image' in l and '4 in the full image' in l and 'rejected' in l for l in lines)


def test_fast_plateau_moves_the_walk_without_a_full_build():
    s, confirmed = search({})
    s.offer('C', 5, 'same', 1)
    s.offer('D', 6, 'worse', 2)
    assert (s.walk, s.walk_score, s.best, s.best_score, confirmed) == ('C', 5, 'B', 4, [])


def test_fast_search_stops_at_a_full_match():
    s, confirmed = search({'<C>': 3, '<D>': 0})
    s.offer('C', 3, '', 1)
    s.offer('D', 1, '', 2)
    assert confirmed == ['<C>', '<D>']
    assert (s.best, s.best_score, s.done) == ('D', 0, True)
    assert s.propose(random.Random(0), 100) == (None, None)


def test_fast_search_best_always_has_a_full_build_score():
    """A fast scorer that claims every variant improves: each is confirmed,
    and the best is always a variant whose full score is the lowest seen."""
    class Fast:
        n = 100

        def score_text(self, text):
            Fast.n -= 1
            return Fast.n, 'fast'

    rng = random.Random(5)
    full = {}

    def confirm(text):
        full.setdefault(text, rng.randrange(1, 20))  # never 0, which ends the search
        return full[text], 'full'

    start, end = permute.find_body(SOURCE, 0x1234)
    s = permute.FastSearch(SOURCE[:start], SOURCE[start:end], SOURCE[end:], confirm, confirm(SOURCE)[0],
                           lambda *_: None)
    permute._work(s, Fast(), random.Random(3), 8, None)
    assert s.tries == 8 and s.confirmations == 8
    assert s.best_score == min(full.values()) and full[s.text(s.best)] == s.best_score
    assert s.text(s.best) in full


def test_jobs_needs_fast(monkeypatch, capsys):
    monkeypatch.setattr(permute.sys, 'argv', ['permute.py', '0x1234', '--jobs', '2'])
    with pytest.raises(SystemExit):
        permute.main()
    assert '--jobs' in capsys.readouterr().err


