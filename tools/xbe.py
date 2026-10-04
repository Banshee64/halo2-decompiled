"""Reads an Xbox executable (XBE): header, certificate, sections, libraries.

    python tools/xbe.py <default.xbe>      print a summary

A truncated file or a header pointer that falls outside it raises ValueError.
Virtual addresses below the image base are rejected: they are not file
offsets, and indexing the file with them would wrap to the end of the bytes.
"""
import argparse
import datetime
import hashlib
import os
import struct
import sys
from dataclasses import dataclass

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FUNCTIONS_CSV = os.path.join(ROOT, 'config', 'functions.csv')


def retail_xbe_path():
    return os.environ.get('RETAIL_XBE', os.path.join(ROOT, 'orig', 'default.xbe'))


def xdk_dir():
    return os.environ.get('XDK_DIR', os.path.join(ROOT, 'sdk', 'xbox'))


# the entry point is stored XORed with one of these, by kind of build
ENTRY_KEYS = {'retail': 0xA8FC57AB, 'debug': 0x94859D4B}

# fixed header fields this reader uses run through the library-table address
HEADER_SIZE = 0x168
CERT_USED = 0xA4  # title id, title name, game region


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
        self.path = path
        with open(path, 'rb') as f:
            d = self.data = f.read()
        if d[:4] != b'XBEH':
            raise ValueError(f'{path}: not an XBE')
        self._need(0, HEADER_SIZE, 'header')
        self.base, = struct.unpack_from('<I', d, 0x104)
        self.timestamp, cert_va, nsec, sec_va = struct.unpack_from('<IIII', d, 0x114)
        debug_va, = struct.unpack_from('<I', d, 0x14C)
        self.debug_path = self.cstr(debug_va, 'debug path')
        self.sections = []
        if nsec:
            table = self._va_off(sec_va, nsec * 56, 'section table')
            for i in range(nsec):
                fl, va, vs, ra, rs, na = struct.unpack_from('<IIIIII', d, table + i * 56)
                name = self.cstr(na, 'section name')
                if ra > len(d) or rs > len(d) - ra:
                    raise ValueError(f'{path}: section {name} raw data is outside the file')
                self.sections.append(Section(name, va, vs, ra, rs, fl))
        # the key whose entry point lands in a section tells the kind of build
        entry, = struct.unpack_from('<I', d, 0x128)
        self.kind, self.entry = 'unknown', entry
        for kind, key in ENTRY_KEYS.items():
            decoded = entry ^ key
            if any(s.va <= decoded < s.va + s.vsize for s in self.sections):
                self.kind, self.entry = kind, decoded
                break
        nlib, lib_va = struct.unpack_from('<II', d, 0x160)
        self.libraries = []
        if nlib:
            lib_off = self._va_off(lib_va, nlib * 16, 'library table')
            for i in range(nlib):
                o = lib_off + i * 16
                name = d[o:o + 8].rstrip(b'\0').decode('latin-1')
                major, minor, build, flags = struct.unpack_from('<HHHH', d, o + 8)
                self.libraries.append((name, f'{major}.{minor}.{build}', flags))
        c = self._va_off(cert_va, CERT_USED, 'certificate')
        self.title_id, = struct.unpack_from('<I', d, c + 8)
        try:
            self.title = d[c + 0xC:c + 0xC + 80].decode('utf-16le').rstrip('\0')
        except UnicodeDecodeError as e:
            raise ValueError(f'{path}: certificate title is not UTF-16') from e
        self.region, = struct.unpack_from('<I', d, c + 0xA0)

    def _need(self, off, size, what):
        if off < 0 or size < 0 or off > len(self.data) or size > len(self.data) - off:
            raise ValueError(f'{self.path}: truncated XBE ({what})')
        return off

    def _va_off(self, va, size, what):
        """File offset of a header pointer. Header pointers are image VAs of
        bytes stored at file offset va - base; a VA below the base is outside."""
        if va < self.base:
            raise ValueError(f'{self.path}: {what} points outside the file ({va:#x})')
        return self._need(va - self.base, size, what)

    def cstr(self, va, what='string'):
        o = self._va_off(va, 1, what)
        end = self.data.find(b'\0', o)
        if end < 0:
            raise ValueError(f'{self.path}: {what} is not terminated')
        return self.data[o:end].decode('latin-1')

    def section(self, name):
        """The first section with this name (a name can repeat; BINKYUY2 does)."""
        for s in self.sections:
            if s.name == name:
                return s
        raise ValueError(f'{self.path}: no {name!r} section')

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


def load(path):
    """The XBE at path. OSError and ValueError become a one-line SystemExit."""
    try:
        return Xbe(path)
    except OSError as e:
        sys.exit(f'error: {path}: {e.strerror}')
    except ValueError as e:
        sys.exit(f'error: {e}')


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument('xbe')
    args = ap.parse_args()
    x = load(args.xbe)
    try:
        when = datetime.datetime.fromtimestamp(x.timestamp, datetime.timezone.utc)
        linked = f'{when:%Y-%m-%d %H:%M:%S} UTC'
    except (OverflowError, OSError, ValueError):
        linked = f'invalid timestamp {x.timestamp}'
    print(f'sha256      {hashlib.sha256(x.data).hexdigest()}')
    print(f'title       {x.title} ({x.title_id:08X}), region {x.region:#x}')
    print(f'debug path  {x.debug_path}')
    print(f'linked      {linked}')
    print(f'entry       {x.entry:#x} ({x.kind} build)')
    for s in x.sections:
        print(f'  {s.name:<10} va {s.va:08x} size {s.vsize:08x}')
    for name, version, flags in x.libraries:
        print(f'  lib {name:<8} {version} flags {flags:#06x}')


if __name__ == '__main__':
    main()
