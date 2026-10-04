"""Hardening tests for tools/xiso_extract.py."""
from __future__ import annotations

import os
import struct
import time
from pathlib import Path

import pytest

import xiso_extract as xiso

SECTOR = xiso.SECTOR
MAGIC = xiso.MAGIC


def _entry(left: int, right: int, start: int, fsize: int, attr: int, name: str) -> bytes:
    name_b = name.encode('latin-1')
    return struct.pack('<HHIIBB', left, right, start, fsize, attr, len(name_b)) + name_b


def _image_with_root(root_sector: int, root_bytes: bytes, extra_sectors: dict[int, bytes]) -> bytes:
    last = max([root_sector, *extra_sectors.keys()], default=root_sector)
    size = max(0x10000 + 28, (last + 1) * SECTOR)
    img = bytearray(size)
    img[0x10000:0x10000 + len(MAGIC)] = MAGIC
    struct.pack_into('<II', img, 0x10000 + len(MAGIC), root_sector, len(root_bytes))
    padded = root_bytes + b'\x00' * (SECTOR - (len(root_bytes) % SECTOR or SECTOR))
    # Keep exactly one sector for simple roots unless longer.
    if len(root_bytes) <= SECTOR:
        padded = root_bytes + b'\x00' * (SECTOR - len(root_bytes))
        img[root_sector * SECTOR:(root_sector + 1) * SECTOR] = padded
    else:
        img[root_sector * SECTOR:root_sector * SECTOR + len(root_bytes)] = root_bytes
    for sector, data in extra_sectors.items():
        chunk = data if len(data) >= SECTOR else data + b'\x00' * (SECTOR - len(data))
        img[sector * SECTOR:sector * SECTOR + len(chunk)] = chunk[:SECTOR] if len(data) <= SECTOR else data
        if len(data) > SECTOR:
            img[sector * SECTOR:sector * SECTOR + len(data)] = data
    return bytes(img)


def test_rejects_dotdot_filename(tmp_path: Path) -> None:
    """A crafted \"..\" directory entry must not write outside out_dir."""
    root = _entry(0, 0, 2, 4, 0, '..')
    img = _image_with_root(1, root, {2: b'EVIL'})
    iso = tmp_path / 'evil.iso'
    iso.write_bytes(img)
    out = tmp_path / 'out'
    out.mkdir()
    with pytest.raises(ValueError, match=r'\.\.|unsafe|escape'):
        xiso.extract(str(iso), str(out))
    # Ensure nothing landed beside out/.
    siblings = [p for p in tmp_path.iterdir() if p.name not in ('evil.iso', 'out')]
    assert siblings == []
    assert list(out.rglob('*')) == []


def test_truncated_image_does_not_hang(tmp_path: Path) -> None:
    """A file size larger than remaining image bytes must not loop forever."""
    root = _entry(0, 0, 2, 1_000_000, 0, 'big.bin')
    img = _image_with_root(1, root, {2: b'short!!!'})
    iso = tmp_path / 'trunc.iso'
    iso.write_bytes(img)
    out = tmp_path / 'out'
    out.mkdir()
    t0 = time.monotonic()
    with pytest.raises(EOFError):
        xiso.extract(str(iso), str(out))
    assert time.monotonic() - t0 < 2.0


def test_directory_cycle_does_not_recurse_forever(tmp_path: Path) -> None:
    """A directory whose child points back at a parent must stop."""
    ent_a = _entry(0, 0, 2, SECTOR, 0x10, 'a')
    ent_b = _entry(0, 0, 1, SECTOR, 0x10, 'b')
    img = _image_with_root(1, ent_a, {2: ent_b + b'\x00' * (SECTOR - len(ent_b))})
    # Ensure sector 1 is a full sector directory for the cycle back-reference size.
    iso = tmp_path / 'cycle.iso'
    # Rebuild so sector 1 is also a full SECTOR-sized directory matching fsize.
    img = bytearray(max(0x10000 + 28, 3 * SECTOR))
    img[0x10000:0x10000 + len(MAGIC)] = MAGIC
    struct.pack_into('<II', img, 0x10000 + len(MAGIC), 1, SECTOR)
    img[1 * SECTOR:2 * SECTOR] = ent_a + b'\x00' * (SECTOR - len(ent_a))
    img[2 * SECTOR:3 * SECTOR] = ent_b + b'\x00' * (SECTOR - len(ent_b))
    iso.write_bytes(img)

    t0 = time.monotonic()
    entries = xiso.list_entries(str(iso))
    assert time.monotonic() - t0 < 2.0
    assert len(entries) < 100
