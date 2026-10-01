import os

import pytest

import build
from build import scan, source_flags, standin_source

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
    marked = scan(chr(10).join(['// @retail 0x1000', 'void foo::bar(int x)', '{', '}']), 'src/foo.cpp')
    with pytest.raises(SystemExit) as e:
        standin_source('src/foo.cpp', [], marked, 'foo')
    assert 'member functions need a caller in src/' in str(e.value)
