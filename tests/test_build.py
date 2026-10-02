import pytest

import build
from build import file_flags, stub_sources, scan, source_flags

SOURCE = '''#include "cseries.h"
#include "crc.h"

// @retail 0x163ba0
void crc_checksum_buffer(
	unsigned long *crc_reference,
	void const *buffer,
	long buffer_size)
{
}

// @retail 0x163c00
PRIVATE void build_crc_table(unsigned long *crc_table)
{
}

// @retail 0x259d0
real _real_random_range(unsigned long *seed, char const *file, long line, real lower_bound, real upper_bound)
{
	return lower_bound;
}
'''


def test_scan_reads_markers_and_signatures():
    marked = scan(SOURCE, 'src/crc.cpp')
    assert [(m.retail, m.name, m.returns, m.params) for m in marked] == [
        (0x163ba0, 'crc_checksum_buffer', 'void', ['unsigned long *', 'void const *', 'long']),
        (0x163c00, 'build_crc_table', 'void', ['unsigned long *']),
        (0x259d0, '_real_random_range', 'real', ['unsigned long *', 'char const *', 'long', 'real', 'real']),
    ]


def tu(text=SOURCE, classes=()):
    return build.tu_source('/abs/src/crc.cpp', scan(text, 'src/crc.cpp'), 'crc', classes)


def test_tu_includes_the_source_then_turns_inlining_off():
    text = tu()
    assert text.index('#include "/abs/src/crc.cpp"') < text.index('#pragma auto_inline(off)')
    assert text.index('#pragma auto_inline(off)') < text.index('#pragma inline_depth(0)')
    assert text.index('#pragma inline_depth(0)') < text.index('standin_crc_0')


def test_tu_calls_directly_with_volatile_arguments_and_no_prototypes():
    text = tu()
    assert 'void crc_checksum_buffer(unsigned long *, void const *, long);' not in text
    assert 'crc_checksum_buffer(*(unsigned long * volatile *)(standin_crc_arguments + 0), ' in text
    assert 'static real volatile standin_crc_result_2;' in text
    assert 'standin_crc_result_2 = _real_random_range(' in text
    assert '&crc_checksum_buffer' not in text  # never through a pointer


MEMBERS = '''
// @retail 0x1000
void __stdcall callback(int x, widget w, widget &r, enum mode m)
{
}

// @retail 0x1010
static inline long *__fastcall find(char c) { return 0; }

// @retail 0x1020
long widget::size(int n) const
{
}

// @retail 0x1030
widget::widget(int a)
{
}

// @retail 0x1040
widget::~widget()
{
}

// @retail 0x1050
void ns::widget::go()
{
}
'''


def test_scan_strips_conventions_and_reads_members():
    callback, find, size, ctor, dtor, go = scan(MEMBERS, 'src/w.cpp')
    assert (callback.returns, callback.kind) == ('void', 'function')
    assert callback.params == ['int', 'widget', 'widget &', 'enum mode']
    assert (find.returns, find.kind) == ('long *', 'function')
    assert (size.name, size.cls, size.kind, size.returns) == ('widget::size', 'widget', 'method', 'long')
    assert (ctor.cls, ctor.kind, ctor.returns) == ('widget', 'constructor', '')
    assert (dtor.cls, dtor.kind, dtor.returns) == ('widget', 'destructor', '')
    assert (go.cls, go.kind) == ('ns::widget', 'method')


def test_tu_member_constructor_and_destructor_standins():
    text = build.tu_source('/abs/src/w.cpp', scan(MEMBERS, 'src/w.cpp'), 'w', {'widget'})
    assert 'standin_w_result_2 = ((widget *)standin_w_arguments)->size(*(int volatile *)(standin_w_arguments + 0));' in text
    assert '::new ((void *)standin_w_arguments) widget(*(int volatile *)(standin_w_arguments + 0));' in text
    assert '((widget *)standin_w_arguments)->~widget();' in text
    assert 'static long volatile standin_w_result_2;' in text
    assert '#include <new>' in text
    assert 'standin_w_result_3' not in text and 'standin_w_result_4' not in text


def test_tu_reads_class_values_through_a_non_volatile_pointer():
    text = build.tu_source('/abs/src/w.cpp', scan(MEMBERS, 'src/w.cpp'), 'w', {'widget'})
    assert '*(widget *)(void *)(standin_w_arguments + 16)' in text      # by value
    assert '**(widget * volatile *)(standin_w_arguments + 32)' in text  # reference
    assert '*(enum mode volatile *)(standin_w_arguments + 48)' in text  # enums are scalars
    assert '*(int volatile *)(standin_w_arguments + 0)' in text


def test_class_names_come_from_declarations():
    found = build.class_names(['struct a { int x; };', 'typedef struct { int y; } b;', 'class c;', 'int d;',
                               'enum e { f };'])
    assert found == {'a', 'b', 'c'}


def test_build_reports_missing_sdk(tmp_path):
    with pytest.raises(SystemExit) as e:
        build.build(root=str(tmp_path), xdk=str(tmp_path / 'nowhere'))
    message = str(e.value)
    assert str(tmp_path / 'nowhere') in message and 'XDK_DIR' in message


def test_every_source_gets_ltcg_and_register_calls():
    assert source_flags(['/O1']) == ['/GL', '/Gr', '/O1']
    assert source_flags([])[:2] == ['/GL', '/Gr']


CONFIG = {'default': ['/O2', '/Gr'], 'files': {'old.cpp': ['/O1', '/Gr']}}


def test_flags_line_in_the_source_wins():
    text = '/* a file */\n// @flags /O1 /Gr\n#include "x.h"\n'
    assert file_flags(text, 'old.cpp', CONFIG) == ['/O1', '/Gr']
    assert file_flags('// @flags /O2 /Ob1 /Gr\n', 'new.cpp', CONFIG) == ['/O2', '/Ob1', '/Gr']


def test_flags_fall_back_to_files_json_then_default():
    assert file_flags('int x;\n', 'old.cpp', CONFIG) == ['/O1', '/Gr']
    assert file_flags('int x;\n', 'new.cpp', CONFIG) == ['/O2', '/Gr']
    assert file_flags('int x;\n', 'new.cpp', {'default': ['/O2']}) == ['/O2']


def test_flags_line_after_the_first_thirty_lines_is_ignored():
    text = '\n' * 30 + '// @flags /O1\n'
    assert file_flags(text, 'new.cpp', CONFIG) == ['/O2', '/Gr']


STUBS = '''#include "cseries.h"

// @stub 0x2f0a40
real hkVector4_length(real const *v)
{
	return 0;
}
'''


def test_scan_reads_stub_markers():
    (stub,) = scan(STUBS, 'src/stubs/havok.cpp')
    assert (stub.retail, stub.name, stub.returns, stub.params, stub.stub) == (
        0x2f0a40, 'hkVector4_length', 'real', ['real const *'], True)
    assert not scan(SOURCE, 'src/crc.cpp')[0].stub


def test_stub_sources_reads_only_the_stubs_folder(tmp_path):
    (tmp_path / 'src' / 'stubs').mkdir(parents=True)
    (tmp_path / 'src' / 'stubs' / 'havok.cpp').write_text(STUBS)
    (tmp_path / 'src' / 'crc.cpp').write_text(SOURCE)
    assert [m.name for m in stub_sources(str(tmp_path))] == ['hkVector4_length']
    assert [m.name for m in build.marked_sources(str(tmp_path))][0] == 'crc_checksum_buffer'
    assert all(not m.stub for m in build.marked_sources(str(tmp_path)))


def test_entry_source_leaves_fltused_to_libcmt():
    text = build.entry_source(['void standin_a_0(void);'], ['	standin_a_0();'])
    assert 'fltused' not in text
    assert 'standin_a_0();' in text and 'int entry(void)' in text
