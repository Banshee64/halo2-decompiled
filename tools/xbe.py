"""Reads an Xbox executable (XBE): header, certificate, sections, libraries.

    python tools/xbe.py <default.xbe>      print a summary
"""
import datetime
import hashlib
import struct
import sys
from dataclasses import dataclass

ENTRY_KEYS = {'retail': 0xA8FC57AB, 'debug': 0x94859D4B}


@dataclass
class Section:
    name: str
    va: int
    vsize: int
    raw: int
    rsize: int
    flags: int


class Xbe:
    def __init__(self, path):
        self.data = open(path, 'rb').read()
        d = self.data
        if d[:4] != b'XBEH':
            raise ValueError(f'{path}: not an XBE')
        self.base, = struct.unpack_from('<I', d, 0x104)
        self.timestamp, self.cert_va, nsec, sec_va = struct.unpack_from('<IIII', d, 0x114)
        entry, = struct.unpack_from('<I', d, 0x128)
        self.entry = entry ^ ENTRY_KEYS['retail']
        self.debug_path = self.cstr(struct.unpack_from('<I', d, 0x14C)[0])
        self.sections = []
        for i in range(nsec):
            fl, va, vs, ra, rs, na = struct.unpack_from('<IIIIII', d, sec_va - self.base + i * 56)
            self.sections.append(Section(self.cstr(na), va, vs, ra, rs, fl))
        nlib, lib_va = struct.unpack_from('<II', d, 0x160)
        self.libraries = []
        for i in range(nlib):
            o = lib_va - self.base + i * 16
            name = d[o:o + 8].rstrip(b'\0').decode()
            major, minor, build, flags = struct.unpack_from('<HHHH', d, o + 8)
            self.libraries.append((name, f'{major}.{minor}.{build}', flags))
        c = self.cert_va - self.base
        self.title_id, = struct.unpack_from('<I', d, c + 8)
        self.title = d[c + 0xC:c + 0xC + 80].decode('utf-16le').rstrip('\0')
        self.region, = struct.unpack_from('<I', d, c + 0xA0)

    def cstr(self, va):
        o = va - self.base
        return self.data[o:self.data.index(b'\0', o)].decode('latin-1')

    def section(self, name):
        return next(s for s in self.sections if s.name == name)

    def section_bytes(self, s):
        return self.data[s.raw:s.raw + s.rsize]

    def read(self, va, size):
        """Bytes at a virtual address (zeros past a section's raw data)."""
        for s in self.sections:
            if s.va <= va < s.va + s.vsize:
                o = va - s.va
                chunk = self.data[s.raw + o:s.raw + min(o + size, s.rsize)]
                return chunk + bytes(size - len(chunk))
        raise ValueError(f'{va:#x} is in no section')


def main():
    x = Xbe(sys.argv[1])
    when = datetime.datetime.fromtimestamp(x.timestamp, datetime.timezone.utc)
    print(f'sha256      {hashlib.sha256(x.data).hexdigest()}')
    print(f'title       {x.title} ({x.title_id:08X}), region {x.region:#x}')
    print(f'debug path  {x.debug_path}')
    print(f'linked      {when:%Y-%m-%d %H:%M:%S} UTC')
    print(f'entry       {x.entry:#x}')
    for s in x.sections:
        print(f'  {s.name:<10} va {s.va:08x} size {s.vsize:08x}')
    for name, version, flags in x.libraries:
        print(f'  lib {name:<8} {version} flags {flags:#06x}')


if __name__ == '__main__':
    main()
