"""Lists or extracts the files of an Xbox disc image (XISO).

    python tools/xiso_extract.py <image.iso>                     list every file
    python tools/xiso_extract.py <image.iso> <out_dir> [path...]  extract the named
                                                                  files (all when none)

Handles images whose game partition starts at offset 0 (XISO) and full
Redump images (game partition at 0x18300000). Never writes to the image.
"""
import argparse
import hashlib
import os
import struct
import sys

SECTOR = 0x800
MAGIC = b'MICROSOFT*XBOX*MEDIA'
PARTITION_OFFSETS = (0, 0x18300000)


def find_partition(f):
    for base in PARTITION_OFFSETS:
        f.seek(base + 0x10000)
        if f.read(len(MAGIC)) == MAGIC:
            return base
    sys.exit('not an Xbox disc image (no XDVDFS header found)')


def walk(f, base, sector, size, prefix, out):
    """Appends (path, size, start_sector) for a directory's binary tree."""
    if size == 0:
        return
    f.seek(base + sector * SECTOR)
    data = f.read(size)
    stack, seen = [0], set()
    while stack:
        off = stack.pop()
        p = off * 4
        if off in seen or p + 14 > len(data):
            continue
        seen.add(off)
        left, right, start, fsize, attr, nlen = struct.unpack_from('<HHIIBB', data, p)
        if left == 0xFFFF:  # padding
            continue
        name = data[p + 14:p + 14 + nlen].decode('latin-1')
        if attr & 0x10:
            out.append((prefix + name + '/', 0, start))
            walk(f, base, start, fsize, prefix + name + '/', out)
        else:
            out.append((prefix + name, fsize, start))
        stack += [x for x in (left, right) if x]


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument('image')
    ap.add_argument('out_dir', nargs='?')
    ap.add_argument('paths', nargs='*')
    args = ap.parse_args()

    with open(args.image, 'rb') as f:
        base = find_partition(f)
        f.seek(base + 0x10000 + len(MAGIC))
        root_sector, root_size = struct.unpack('<II', f.read(8))
        entries = []
        walk(f, base, root_sector, root_size, '', entries)
        entries.sort()

        if not args.out_dir:
            for path, size, _ in entries:
                print(f'{size:>12}  {path}')
            print(f'{len(entries)} entries, {sum(e[1] for e in entries)} bytes')
            return

        wanted = set(args.paths)
        for path, size, start in entries:
            if path.endswith('/') or (wanted and path not in wanted):
                continue
            dst = os.path.join(args.out_dir, *path.split('/'))
            os.makedirs(os.path.dirname(dst), exist_ok=True)
            f.seek(base + start * SECTOR)
            digest = hashlib.sha256()
            with open(dst, 'wb') as o:
                left = size
                while left:
                    chunk = f.read(min(left, 1 << 20))
                    digest.update(chunk)
                    o.write(chunk)
                    left -= len(chunk)
            print(f'{path}  sha256 {digest.hexdigest()}')


if __name__ == '__main__':
    main()
