import os
import struct

import pytest

from libsig import Signature, find, library_signatures, object_signatures


def coff(code, relocs, symbols):
    """A minimal i386 COFF object: one .text section.
    relocs: [(offset, type)], symbols: [(name, offset)] (external functions)."""
    nsyms = len(symbols)
    header_size, section_size = 20, 40
    raw = header_size + section_size
    rel = raw + len(code)
    symtab = rel + 10 * len(relocs)
    head = struct.pack('<HHIIIHH', 0x14C, 1, 0, symtab, nsyms, 0, 0)
    sect = struct.pack('<8sIIIIIIHHI', b'.text', 0, 0, len(code), raw, rel, 0, len(relocs), 0, 0x60000020)
    body = code + b''.join(struct.pack('<IIH', off, 0, kind) for off, kind in relocs)
    syms = b''.join(struct.pack('<8sIhHBB', n.encode().ljust(8, b'\0'), off, 1, 0x20, 2, 0) for n, off in symbols)
    return head + sect + body + syms + struct.pack('<I', 4)


def test_object_signature_masks_relocations():
    code = bytes.fromhex('55 8bec a1 00000000 5d c3'.replace(' ', ''))
    sigs = object_signatures(coff(code, [(4, 0x06)], [('_f', 0)]))
    assert len(sigs) == 1 and sigs[0].name == '_f'
    assert sigs[0].mask == bytes([1, 1, 1, 1, 0, 0, 0, 0, 1, 1])


def test_find_unique_hit_with_different_address():
    code = bytes.fromhex('558beca100000000a1000000005dc3')
    sig = object_signatures(coff(code, [(4, 0x06), (9, 0x06)], [('_g', 0)]))[0]
    text = b'\xcc' * 32 + bytes.fromhex('558beca178563412a1ddccbbaa5dc3') + b'\xcc' * 8
    assert find([sig], text, 0x10000) == {0x10020: sig}


def test_find_ignores_ambiguous():
    sig = Signature('lib', 'm', '_h', bytes.fromhex('8b442404c3909090'), b'\1' * 8)
    text = sig.code + sig.code
    assert find([sig], text, 0x10000) == {}


@pytest.mark.sdk
@pytest.mark.retail
def test_retail_libcmt(xdk_dir, retail_xbe):
    from xbe import Xbe
    image = Xbe(retail_xbe)
    text = image.section('.text')
    hits = find(library_signatures(os.path.join(xdk_dir, 'lib', 'libcmt.lib')),
                image.section_bytes(text), text.va)
    names = {va: s.name for va, s in hits.items()}
    assert names.get(0x321340) == '_strncmp'
    assert names.get(0x320bd0) == '__allmul'
