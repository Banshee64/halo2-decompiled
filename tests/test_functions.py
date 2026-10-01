from dataclasses import dataclass

import pytest

from functions import discover
from xbe import Section, Xbe


@dataclass
class FakeImage:
    """A one-section image: code at 0x1000, optional data section."""
    code: bytes
    data: bytes = b''
    entry: int = 0x1000

    def __post_init__(self):
        self.sections = [Section('.text', 0x1000, len(self.code), 0, len(self.code), 0)]
        if self.data:
            self.sections.append(Section('.data', 0x8000, len(self.data), len(self.code), len(self.data), 0))
        self._bytes = self.code + self.data

    def section(self, name):
        return next(s for s in self.sections if s.name == name)

    def section_bytes(self, s):
        return self._bytes[s.raw:s.raw + s.rsize]


def pad(code, to):
    return code + b'\xcc' * (to - len(code))


def test_call_and_padding():
    # 0x1000: call 0x1010 ; ret ; int3 padding ; 0x1010: xor eax, eax ; ret
    code = pad(bytes.fromhex('e80b000000c3'), 0x10) + bytes.fromhex('33c0c3')
    found = discover(FakeImage(code))
    assert sorted(found) == [0x1000, 0x1010]
    assert found[0x1000].end == 0x1006 and found[0x1000].calls == {0x1010}
    assert found[0x1010].end == 0x1013


def test_jump_table_bounded_by_cmp():
    code = bytes.fromhex(
        '83f802'          # 1000 cmp eax, 2
        '7713'            # 1003 ja 1018
        'ff24851c100000'  # 1005 jmp [eax*4 + 0x101c]
        'b801000000c3'    # 100c case 0
        'b802000000c3'    # 1012 case 1
        '33c0c3'          # 1018 default
        '90'              # 101b alignment
        '0c100000' '12100000' '18100000')  # 101c table, 3 entries
    found = discover(FakeImage(code))
    assert list(found) == [0x1000]
    assert found[0x1000].end == 0x1028
    assert found[0x1000].tables == [(0x101c, 4, 3)]


def test_tail_jump_to_known_start():
    # 0x1000: jmp 0x1010 (another function, seeded) ; 0x1010: ret
    code = pad(bytes.fromhex('e90b000000'), 0x10) + bytes.fromhex('c3')
    found = discover(FakeImage(code), seeds=[0x1010])
    assert found[0x1000].end == 0x1005 and found[0x1000].tail_jumps == {0x1010}
    assert 0x1010 in found


def test_data_pointer_finds_callback():
    # 0x1000: ret ; padding ; 0x1010: callback only reached through .data
    code = pad(bytes.fromhex('c3'), 0x10) + bytes.fromhex('b801000000c3')
    found = discover(FakeImage(code, data=(0x1010).to_bytes(4, 'little')))
    assert 0x1010 in found and found[0x1010].end == 0x1016


def test_gap_after_packed_functions_becomes_a_function():
    # two packed functions, the second reached by nothing: ret ; mov eax,1 ; ret
    code = bytes.fromhex('c3b801000000c3')
    found = discover(FakeImage(code))
    assert sorted(found) == [0x1000, 0x1001]


def test_end_clamped_at_next_start():
    # 0x1000: call 0x1010 (no return, no ret) falls into 0x1005: xor eax, eax ; ret
    code = pad(bytes.fromhex('e80b000000') + bytes.fromhex('33c0c3'), 0x10) + bytes.fromhex('c3')
    found = discover(FakeImage(code), seeds=[0x1005, 0x1010])
    assert found[0x1000].end == 0x1005 and found[0x1000].calls == {0x1010}
    assert found[0x1005].end == 0x1008


def test_clamp_drops_calls_tail_jumps_and_tables_past_the_cut():
    code = pad(
        bytes.fromhex('90' * 8)            # 1000 nops, falling into the next function
        + bytes.fromhex(
            'e833000000'             # 1008 call 0x1040
            '83f802'                 # 100d cmp eax, 2
            '7708'                   # 1010 ja 101a
            'ff248520100000'         # 1012 jmp [eax*4 + 0x1020]
            'c3'                     # 1019 case: ret
            'e921000000'             # 101a jmp 0x1040 (tail)
            '90')                    # 101f padding
        + bytes.fromhex('19100000') * 3,   # 1020 table, 3 entries
        0x40) + bytes.fromhex('c3')
    found = discover(FakeImage(code), seeds=[0x1008, 0x1040])
    first, second = found[0x1000], found[0x1008]
    assert first.end == 0x1008
    assert first.calls == set() and first.tail_jumps == set() and first.tables == []
    assert second.calls == {0x1040} and second.tail_jumps == {0x1040}
    assert second.tables == [(0x1020, 4, 3)] and second.end == 0x102c


def test_weak_start_inside_strong_function_is_dropped():
    # 0x1000 (the entry) is 0x20 bytes of nops and a ret; .data points at 0x1010,
    # a 16-aligned address strictly inside it
    code = bytes.fromhex('90' * 0x1f + 'c3')
    found = discover(FakeImage(code, data=(0x1010).to_bytes(4, 'little')))
    assert list(found) == [0x1000]
    assert found[0x1000].end == 0x1020
    assert all(f.end > f.start for f in found.values())


def test_weak_start_at_padding_is_not_a_function():
    # 0x1000: ret ; int3 padding ; .data points at 0x1004, an int3
    code = pad(bytes.fromhex('c3'), 0x10)
    found = discover(FakeImage(code, data=(0x1004).to_bytes(4, 'little')))
    assert list(found) == [0x1000]


def test_weak_start_inside_an_earlier_weak_function_is_dropped():
    # the entry is a lone ret; two weak pointers: 0x1010 (a 0x20-byte inc run, then ret)
    # and 0x1020 strictly inside it
    code = pad(bytes.fromhex('c3'), 0x10) + bytes.fromhex('40' * 0x1f + 'c3')
    data = (0x1010).to_bytes(4, 'little') + (0x1020).to_bytes(4, 'little')
    found = discover(FakeImage(code, data=data))
    assert sorted(found) == [0x1000, 0x1010]
    assert found[0x1010].end == 0x1030


@pytest.mark.retail
def test_retail_known_functions(retail_xbe):
    found = discover(Xbe(retail_xbe))
    assert found[0x163ba0].end == 0x163bf4      # crc_checksum_buffer, ends with ret 4
    assert found[0x163c00].end == 0x163c35      # build_crc_table
    assert found[0x163ba0].calls == {0x163c00}
    assert found[0x1782a0].tail_jumps == {0x17add0}
    ordered = list(found.values())
    assert all(f.end > f.start for f in ordered)                         # no empty functions
    assert all(a.end <= b.start for a, b in zip(ordered, ordered[1:]))   # extents never overlap
    assert len(found) > 9954                    # more than the direct call targets alone


def test_discover_takes_a_section_object_for_same_named_sections():
    class TwoBink(FakeImage):
        def __post_init__(self):
            self.sections = [Section('BINK', 0x1000, 3, 0, 3, 0), Section('BINK', 0x2000, 3, 3, 3, 0)]
            self._bytes = self.code

    image = TwoBink(bytes.fromhex('33c0c3') + bytes.fromhex('33dbc3'), entry=0x1000)
    first, second = image.sections
    assert list(discover(image, text=first)) == [0x1000]
    assert list(discover(image, seeds=[0x2000], text=second)) == [0x2000]
