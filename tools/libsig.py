"""Signatures of Xbox SDK library functions, for finding them in the retail
XBE. Each function's bytes come from its library object, with the fields its
relocations fill in masked, so a match ignores addresses.

Only libraries of ordinary objects work this way. D3D8 and XGRAPHICS ship as
LTCG intermediate code and are found by section instead.

    python tools/libsig.py <xdk lib dir> <default.xbe>   count the hits
"""
import os
import struct
import sys
from dataclasses import dataclass

COFF_LIBRARIES = ('libcmt', 'libcpmt', 'xapilib', 'dsound', 'xonlines', 'xvoice', 'xnet', 'd3dx8')
MIN_COMPARED = 6  # shorter signatures match too much by chance
CODE = 0x20  # IMAGE_SCN_CNT_CODE
MASKED = {0x06: 4, 0x07: 4, 0x0A: 2, 0x0B: 4, 0x14: 4}  # DIR32, DIR32NB, SECTION, SECREL, REL32


@dataclass(frozen=True)
class Signature:
    library: str
    member: str
    name: str
    code: bytes
    mask: bytes


def archive_members(data):
    """(name, bytes) for each object in a COFF archive (.lib)."""
    if data[:8] != b'!<arch>\n':
        raise ValueError('not a COFF archive')
    members, longnames, o = [], b'', 8
    while o + 60 <= len(data):
        name = data[o:o + 16].decode('latin-1').rstrip()
        size = int(data[o + 48:o + 58].decode().strip())
        body = data[o + 60:o + 60 + size]
        if name == '//':
            longnames = body
        elif name != '/':
            if name.startswith('/') and name[1:].isdigit():
                k = int(name[1:])
                name = longnames[k:longnames.index(b'\0', k)].decode('latin-1')
            members.append((name.rstrip('/'), body))
        o += 60 + size + (size & 1)
    return members


def object_signatures(obj, library='', member=''):
    machine, nsec, _, symtab, nsyms, optsize, _ = struct.unpack_from('<HHIIIHH', obj, 0)
    if machine != 0x14C:
        return []  # import stubs and LTCG intermediate code
    strings = symtab + 18 * nsyms
    sections = []
    for i in range(nsec):
        o = 20 + optsize + 40 * i
        _, _, _, rsize, rptr, relptr, _, nrel, _, chars = struct.unpack_from('<8sIIIIIIHHI', obj, o)
        sections.append((rsize, rptr, relptr, nrel, chars))

    names = {}  # section index -> [(offset, name)]
    k = 0
    while k < nsyms:
        o = symtab + 18 * k
        raw, value, section, kind, storage, aux = struct.unpack_from('<8sIhHBB', obj, o)
        if raw[:4] == b'\0\0\0\0':
            start = strings + struct.unpack_from('<I', raw, 4)[0]
            name = obj[start:obj.index(b'\0', start)].decode('latin-1')
        else:
            name = raw.rstrip(b'\0').decode('latin-1')
        if section > 0 and (storage == 2 or (storage == 3 and kind == 0x20 and aux == 0)):
            names.setdefault(section - 1, []).append((value, name))
        k += 1 + aux

    signatures = []
    for index, offsets in names.items():
        rsize, rptr, relptr, nrel, chars = sections[index]
        if not chars & CODE or rptr == 0:
            continue
        code = obj[rptr:rptr + rsize]
        mask = bytearray(b'\1' * rsize)
        for r in range(nrel):
            at, _, kind = struct.unpack_from('<IIH', obj, relptr + 10 * r)
            width = MASKED.get(kind, 0)
            mask[at:at + width] = bytes(width)
        offsets = sorted(set(offsets))
        for n, (start, name) in enumerate(offsets):
            end = offsets[n + 1][0] if n + 1 < len(offsets) else rsize
            body, bits = code[start:end], bytes(mask[start:end])
            while body and body[-1] in (0xCC, 0x90):  # trailing alignment fill
                body, bits = body[:-1], bits[:-1]
            if sum(bits) >= MIN_COMPARED:
                signatures.append(Signature(library, member, name, body, bits))
    return signatures


def library_signatures(path):
    library = os.path.splitext(os.path.basename(path))[0].lower()
    with open(path, 'rb') as f:
        members = archive_members(f.read())
    return [s for member, body in members for s in object_signatures(body, library, member)]


def _anchor(sig):
    """The longest run of compared bytes: (offset, bytes)."""
    best, start = (0, b''), None
    for i, bit in enumerate(sig.mask + b'\0'):
        if bit and start is None:
            start = i
        elif not bit and start is not None:
            if i - start > len(best[1]):
                best = (start, sig.code[start:i])
            start = None
    return best


def _equal(sig, text, at):
    window = text[at:at + len(sig.code)]
    return len(window) == len(sig.code) and all(
        not m or a == b for a, b, m in zip(sig.code, window, sig.mask))


def find(signatures, text, text_va):
    """{va: signature} for the signatures found exactly once in text."""
    hits = {}
    for sig in signatures:
        offset, anchor = _anchor(sig)
        if len(anchor) < 4:
            continue
        places, pos = [], text.find(anchor)
        while pos >= 0 and len(places) < 2:
            at = pos - offset
            if at >= 0 and _equal(sig, text, at):
                places.append(at)
            pos = text.find(anchor, pos + 1)
        if len(places) == 1:
            hits[text_va + places[0]] = sig
    return hits


def main():
    from xbe import Xbe
    lib_dir, image = sys.argv[1], Xbe(sys.argv[2])
    text = image.section('.text')
    code = image.section_bytes(text)
    for library in COFF_LIBRARIES:
        path = os.path.join(lib_dir, library + '.lib')
        if os.path.exists(path):
            print(f'{library:<10} {len(find(library_signatures(path), code, text.va))} functions')


if __name__ == '__main__':
    main()
