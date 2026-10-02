import pytest

import build
from build import file_flags, stub_sources, scan, source_flags, standin_source

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


def test_standins_call_directly_with_volatile_arguments():
    text = standin_source('src/crc.cpp', ['#include "cseries.h"', '#include "crc.h"'],
                          scan(SOURCE, 'src/crc.cpp'), 'crc')
    assert 'void crc_checksum_buffer(unsigned long *, void const *, long);' in text
    assert 'crc_checksum_buffer(*(unsigned long * volatile *)(standin_crc_arguments + 0), ' in text
    assert 'static real volatile standin_crc_result_2;' in text
    assert 'standin_crc_result_2 = _real_random_range(' in text
    assert '&crc_checksum_buffer' not in text  # never through a pointer


def test_build_reports_missing_sdk(tmp_path):
    with pytest.raises(SystemExit) as e:
        build.build(root=str(tmp_path), xdk=str(tmp_path / 'nowhere'))
    message = str(e.value)
    assert str(tmp_path / 'nowhere') in message and 'XDK_DIR' in message


def test_every_source_gets_ltcg_and_register_calls():
    assert source_flags(['/O1']) == ['/GL', '/Gr', '/O1']
    assert source_flags([])[:2] == ['/GL', '/Gr']


def test_member_function_standin_error():
    marked = scan('\n'.join(['// @retail 0x1000', 'void foo::bar(int x)', '{', '}']), 'src/foo.cpp')
    with pytest.raises(SystemExit) as e:
        standin_source('src/foo.cpp', [], marked, 'foo')
    assert 'member functions need a caller in src/' in str(e.value)


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
