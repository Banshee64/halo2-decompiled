"""ltcg_probe on a synthetic XBE. No retail disc and no SDK."""
import struct
import sys

from ltcg_probe import main

BASE = 0x10000
OFF_NAME = 0x180
OFF_DEBUG = 0x190
OFF_SECTIONS = 0x1C0
OFF_CERT = 0x280
OFF_RAW = 0x400


def write_xbe(path, code):
    """A header both the current parser and a stricter one will accept."""
    buf = bytearray(OFF_RAW + len(code))
    buf[:4] = b'XBEH'
    struct.pack_into('<I', buf, 0x104, BASE)
    buf[OFF_NAME:OFF_NAME + 6] = b'.text\0'
    buf[OFF_DEBUG:OFF_DEBUG + 2] = b'x\0'
    struct.pack_into('<IIIIII', buf, OFF_SECTIONS,
                     0, 0x11000, len(code), OFF_RAW, len(code), BASE + OFF_NAME)
    struct.pack_into('<I', buf, OFF_CERT + 8, 0x4D530064)
    title = 'Halo 2'.encode('utf-16le')
    buf[OFF_CERT + 0xC:OFF_CERT + 0xC + len(title)] = title
    struct.pack_into('<I', buf, OFF_CERT + 0xA0, 0x7)
    struct.pack_into('<IIII', buf, 0x114, 1_000_000_000,
                     BASE + OFF_CERT, 1, BASE + OFF_SECTIONS)
    struct.pack_into('<I', buf, 0x14C, BASE + OFF_DEBUG)
    struct.pack_into('<II', buf, 0x160, 0, 0)
    buf[OFF_RAW:OFF_RAW + len(code)] = code
    path.write_bytes(buf)
    return path


def test_no_direct_calls_prints_zero_percent(tmp_path, monkeypatch, capsys):
    # call eax; ret — a call, but not a direct one, so the target set is empty
    path = write_xbe(tmp_path / 'nocalls.xbe', bytes.fromhex('ffd0c3'))
    monkeypatch.setattr(sys, 'argv', ['ltcg_probe.py', str(path)])
    main()
    out = capsys.readouterr().out
    assert 'direct call targets in .text: 0\n' in out
    assert 'reading eax/ebx/esi/edi at entry: 0 (0.0%)\n' in out


def test_direct_call_still_reports_a_register_argument(tmp_path, monkeypatch, capsys):
    # 11000: call 11006; nop; 11006: mov eax, ebx; ret
    path = write_xbe(tmp_path / 'onecall.xbe', bytes.fromhex('e801000000908bc3c3'))
    monkeypatch.setattr(sys, 'argv', ['ltcg_probe.py', str(path)])
    main()
    out = capsys.readouterr().out
    assert 'direct call targets in .text: 1\n' in out
    assert 'reading eax/ebx/esi/edi at entry: 1 (100.0%)\n' in out
    assert 'ebx: 1, e.g. 0x11006' in out
