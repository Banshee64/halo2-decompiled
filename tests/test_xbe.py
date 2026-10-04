"""Parser tests for tools/xbe.py. No retail XBE and no SDK."""
import struct
import sys
import time

import pytest

from xbe import CERT_USED, ENTRY_KEYS, Xbe, load, main

BASE = 0x10000
OFF_DEBUG = 0x180
OFF_NAMES = 0x200
OFF_SECTIONS = 0x280
OFF_LIBS = 0x400
OFF_CERT = 0x500
OFF_RAW = 0x800


def _cstr(buf, off, text):
    raw = text.encode('latin-1') + b'\0'
    buf[off:off + len(raw)] = raw
    return BASE + off


def build_xbe(path, *, title='Halo 2', title_id=0x4D530064, region=0x7,
              timestamp=1_000_000_000, entry_va=0x11010, entry_kind='retail',
              debug_path=r'c:\halo2\bin\halo2ship.exe', debug_va=None,
              sections=None, libraries=None, nsec=None, nlib=None,
              tail=b''):
    """A small XBE whose header pointers sit at file offset va - base."""
    if sections is None:
        sections = [('.text', 0x11000, 0x20, OFF_RAW, 0x10, b'\x90' * 0x10)]
    if libraries is None:
        libraries = [('XBOXKRNL', 1, 0, 5849, 0x4001)]
    buf = bytearray(OFF_RAW + 0x40)
    buf[:4] = b'XBEH'
    struct.pack_into('<I', buf, 0x104, BASE)
    names_at = OFF_NAMES
    parsed = []
    for name, va, vsize, raw, rsize, data in sections:
        name_va = _cstr(buf, names_at, name)
        names_at += len(name) + 1
        # only plant raw bytes that actually sit in this small image
        if data and raw <= len(buf):
            end = raw + len(data)
            if len(buf) < end:
                buf.extend(b'\0' * (end - len(buf)))
            buf[raw:end] = data
        parsed.append((0, va, vsize, raw, rsize, name_va))
    if nsec is None:
        nsec = len(parsed)
    if nsec == len(parsed):
        for i, fields in enumerate(parsed):
            struct.pack_into('<IIIIII', buf, OFF_SECTIONS + i * 56, *fields)
    if nlib is None:
        nlib = len(libraries)
    if nlib == len(libraries):
        for i, (name, major, minor, build, flags) in enumerate(libraries):
            o = OFF_LIBS + i * 16
            buf[o:o + 8] = name.encode('latin-1')[:8].ljust(8, b'\0')
            struct.pack_into('<HHHH', buf, o + 8, major, minor, build, flags)
    cert = bytearray(CERT_USED)
    struct.pack_into('<I', cert, 8, title_id)
    encoded = title.encode('utf-16le')
    cert[0xC:0xC + len(encoded)] = encoded
    struct.pack_into('<I', cert, 0x9C, 0xFFFFFFFF)  # canary: not the region
    struct.pack_into('<I', cert, 0xA0, region)
    buf[OFF_CERT:OFF_CERT + len(cert)] = cert
    if debug_va is None:
        debug_va = _cstr(buf, OFF_DEBUG, debug_path)
    struct.pack_into('<IIII', buf, 0x114, timestamp,
                     BASE + OFF_CERT, nsec, BASE + OFF_SECTIONS)
    key = ENTRY_KEYS.get(entry_kind, 0)
    struct.pack_into('<I', buf, 0x128, (entry_va ^ key) & 0xFFFFFFFF)
    struct.pack_into('<I', buf, 0x14C, debug_va)
    struct.pack_into('<II', buf, 0x160, nlib, BASE + OFF_LIBS)
    buf.extend(tail)
    path.write_bytes(buf)
    return path


def test_reads_certificate_sections_and_retail_entry(tmp_path):
    x = Xbe(str(build_xbe(tmp_path / 'game.xbe')))
    assert x.title == 'Halo 2'
    assert x.title_id == 0x4D530064
    assert x.region == 0x7  # the field at 0xA0, not the canary at 0x9C
    assert x.kind == 'retail' and x.entry == 0x11010
    assert x.debug_path == r'c:\halo2\bin\halo2ship.exe'
    text = x.section('.text')
    assert (text.va, text.vsize, text.rsize) == (0x11000, 0x20, 0x10)
    assert x.section_bytes(text) == b'\x90' * 0x10
    assert x.libraries == [('XBOXKRNL', '1.0.5849', 0x4001)]


def test_debug_entry_and_unknown_when_it_misses_every_section(tmp_path):
    debug = Xbe(str(build_xbe(tmp_path / 'debug.xbe', entry_kind='debug')))
    assert debug.kind == 'debug' and debug.entry == 0x11010
    unknown = Xbe(str(build_xbe(tmp_path / 'unk.xbe', entry_va=0x20000)))
    assert unknown.kind == 'unknown'


def test_read_pads_past_raw_bytes_and_rejects_other_addresses(tmp_path):
    path = build_xbe(tmp_path / 'pad.xbe', sections=[
        ('.text', 0x11000, 0x20, OFF_RAW, 4, b'\x11\x22\x33\x44'),
    ])
    x = Xbe(str(path))
    assert x.read(0x11000, 8) == b'\x11\x22\x33\x44' + bytes(4)
    with pytest.raises(ValueError, match='no section'):
        x.read(0x1, 4)


def test_duplicate_section_name_returns_the_first(tmp_path):
    path = build_xbe(tmp_path / 'dup.xbe', sections=[
        ('BINKYUY2', 0x11000, 0x10, OFF_RAW, 4, b'AAAA'),
        ('BINKYUY2', 0x12000, 0x10, OFF_RAW + 16, 4, b'BBBB'),
    ])
    x = Xbe(str(path))
    assert x.section('BINKYUY2').va == 0x11000
    assert x.section_bytes(x.sections[1]) == b'BBBB'
    with pytest.raises(ValueError, match="no '.missing' section"):
        x.section('.missing')


def test_truncated_header_is_a_clean_error(tmp_path):
    path = tmp_path / 'short.xbe'
    path.write_bytes(b'XBEH' + b'\0' * 8)
    with pytest.raises(ValueError, match='truncated XBE'):
        Xbe(str(path))


def test_not_an_xbe(tmp_path):
    path = tmp_path / 'no.xbe'
    path.write_bytes(b'NOPE' + b'\0' * 0x200)
    with pytest.raises(ValueError, match='not an XBE'):
        Xbe(str(path))


def test_pointer_below_base_does_not_read_the_end_of_the_file(tmp_path):
    path = build_xbe(tmp_path / 'low.xbe', debug_va=0, tail=b'SECRET\0')
    with pytest.raises(ValueError, match='debug path') as exc:
        Xbe(str(path))
    assert 'SECRET' not in str(exc.value)


def test_unterminated_debug_path(tmp_path):
    path = build_xbe(tmp_path / 'nonul.xbe')
    data = bytearray(path.read_bytes())
    tail = b'NO-NUL-HERE!!'
    struct.pack_into('<I', data, 0x14C, BASE + len(data))
    path.write_bytes(bytes(data) + tail)
    with pytest.raises(ValueError, match='not terminated'):
        Xbe(str(path))


def test_section_raw_data_outside_the_file(tmp_path):
    path = build_xbe(tmp_path / 'raw.xbe', sections=[
        ('.text', 0x11000, 0x10, 0xFFFFFFF0, 16, b''),
    ])
    with pytest.raises(ValueError, match='raw data is outside'):
        Xbe(str(path))


def test_huge_section_count_is_rejected_quickly(tmp_path):
    path = build_xbe(tmp_path / 'huge.xbe', nsec=0x1000000, sections=[])
    t0 = time.monotonic()
    with pytest.raises(ValueError, match='section table'):
        Xbe(str(path))
    assert time.monotonic() - t0 < 1.0


def test_summary_prints_the_header(tmp_path, capsys, monkeypatch):
    path = build_xbe(tmp_path / 'game.xbe')
    monkeypatch.setattr(sys, 'argv', ['xbe.py', str(path)])
    main()
    out = capsys.readouterr().out
    assert 'Halo 2 (4D530064), region 0x7' in out
    assert '0x11010 (retail build)' in out
    assert '2001-09-09' in out
    assert 'lib XBOXKRNL 1.0.5849' in out


def test_load_reports_missing_and_bad_files(tmp_path):
    with pytest.raises(SystemExit, match='No such file') as missing:
        load(str(tmp_path / 'missing.xbe'))
    assert missing.value.code != 0
    bad = tmp_path / 'bad.xbe'
    bad.write_bytes(b'NOPE')
    with pytest.raises(SystemExit, match='not an XBE') as bad_exit:
        load(str(bad))
    assert bad_exit.value.code != 0


def test_main_requires_a_path(monkeypatch):
    monkeypatch.setattr(sys, 'argv', ['xbe.py'])
    with pytest.raises(SystemExit) as exc:
        main()
    assert exc.value.code == 2


@pytest.mark.retail
def test_retail_header_matches_the_disc_build(retail_xbe):
    x = Xbe(retail_xbe)
    assert (x.title, x.title_id, x.region, x.kind) == ('Halo 2', 0x4D530064, 0x7, 'retail')
    assert x.entry == 0x2d0aee
    assert x.section('.text').va == 0x12000
    assert x.section('BINKYUY2').va == 0x3f3220  # first section of that name
    assert len(x.sections) == 33
    assert ('XBOXKRNL', '1.0.5849', 16385) in x.libraries
    assert len(x.read(x.section('.text').va, 16)) == 16
