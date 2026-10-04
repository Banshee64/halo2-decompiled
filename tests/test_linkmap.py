from linkmap import LinkMap, plain_name

MAP = """ test

 Preferred load address is 00400000

 Start         Length     Name                   Class
 0001:00000000 00000139H .text                   CODE
 0002:00000000 00000511H .bss                    DATA

  Address         Publics by Value              Rva+Base     Lib:Object

 0000:00000000       ___safe_se_handler_table   00000000     <absolute>
 0001:00000000       ?checksum@@YIXPAKPBXJ@Z            00401000 f   a.obj
 0001:000000a0       @entry@0                   004010a0 f   b.obj
 0002:00000008       ?g_buffer@@3PAEA           00402008     b.obj

 entry point at        0000:00000000

 Static symbols

 0001:00000060       ?build_table@@YIXPAK@Z     00401060 f   a.obj

FIXUPS: 1017 a4 2a
FIXUPS: 2000 fffffff0
"""


def test_symbols_and_static():
    m = LinkMap(MAP)
    assert m.base == 0x400000
    names = {s.name: (s.va, s.section, s.static) for s in m.symbols}
    assert names['?checksum@@YIXPAKPBXJ@Z'] == (0x401000, 1, False)
    assert names['?build_table@@YIXPAK@Z'] == (0x401060, 1, True)
    assert '___safe_se_handler_table' not in names  # section 0 is absolute, not code


def test_fixups_each_line_starts_absolute_then_deltas():
    m = LinkMap(MAP)
    assert m.rel_fixups == {0x401017, 0x4010bb, 0x4010e5, 0x402000, 0x401ff0}


def test_extent_runs_to_next_symbol_or_section_end():
    m = LinkMap(MAP)
    first = m.find('checksum')[0]
    table = m.find('build_table')[0]
    entry = m.find('entry')[0]
    assert m.extent(first) == (0x401000, 0x401060)
    assert m.extent(table) == (0x401060, 0x4010a0)
    assert m.extent(entry) == (0x4010a0, 0x401139)


def test_plain_name():
    assert plain_name('?checksum@@YIXPAKPBXJ@Z') == 'checksum'
    assert plain_name('?remove_all@c_world@@QAAXXZ') == 'c_world::remove_all'
    assert plain_name('@entry@0') == 'entry'
    assert plain_name('_strncmp') == 'strncmp'
    assert plain_name('??_Gc_heap@@UAEPAXI@Z') == "c_heap::`deleting destructor'"
    assert plain_name('??_Ec_heap@@UAEPAXI@Z') == "c_heap::`deleting destructor'"
    assert plain_name('_RtlSizeHeap@12') == 'RtlSizeHeap'


def test_plain_name_of_constructors_and_destructors():
    assert plain_name('??0widget@@QAE@J@Z') == 'widget::widget'
    assert plain_name('??1widget@@QAE@XZ') == 'widget::~widget'
    assert plain_name('??0inner@outer@@QAE@XZ') == 'outer::inner::inner'
    assert plain_name('??1inner@outer@@QAE@XZ') == 'outer::inner::~inner'
