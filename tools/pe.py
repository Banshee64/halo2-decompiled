import struct

from xbe import Section, Xbe


class Pe:
    """Reads a linked test image (PE): sections and base relocations."""

    def __init__(self, path):
        d = self.data = open(path, 'rb').read()
        pe, = struct.unpack_from('<I', d, 0x3C)
        nsec, = struct.unpack_from('<H', d, pe + 6)
        opt = pe + 24
        base = self.base = struct.unpack_from('<I', d, opt + 28)[0]
        table = opt + struct.unpack_from('<H', d, pe + 20)[0]
        self.sections = []
        for i in range(nsec):
            vs, va, rs, ra = struct.unpack_from('<IIII', d, table + i * 40 + 8)
            self.sections.append(Section('', base + va, vs, ra, rs, 0))
        # the base relocations: every 4-byte field that holds an address
        self.fixups = set()
        rva, size = struct.unpack_from('<II', d, opt + 96 + 5 * 8)
        if rva:
            blocks = self.read(base + rva, size)
            o = 0
            while o + 8 <= size:
                page, length = struct.unpack_from('<II', blocks, o)
                if length < 8:
                    break
                for k in range(8, length, 2):
                    entry, = struct.unpack_from('<H', blocks, o + k)
                    if entry >> 12 == 3:  # IMAGE_REL_BASED_HIGHLOW
                        self.fixups.add(base + page + (entry & 0xFFF))
                o += length

    read = Xbe.read
