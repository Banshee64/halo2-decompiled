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
MAX_DIR_DEPTH = 256


def find_partition(f):
    """The game partition's offset, and its root directory's sector and size."""
    for base in PARTITION_OFFSETS:
        f.seek(base + 0x10000)
        if f.read(len(MAGIC)) == MAGIC:
            return (base, *struct.unpack('<II', f.read(8)))
    sys.exit('not an Xbox disc image (no XDVDFS header found)')


def _safe_name(name):
    """Reject path components that would escape the output directory."""
    if name in ('', '.', '..') or '/' in name or '\\' in name or '\x00' in name:
        raise ValueError(f'refusing unsafe XISO path component: {name!r}')
    return name


def walk(f, base, sector, size, prefix, out, seen_dirs=None, depth=0):
    """Appends (path, size, start_sector) for a directory's binary tree."""
    if size == 0:
        return
    if depth > MAX_DIR_DEPTH:
        raise ValueError(f'XISO directory nesting exceeds {MAX_DIR_DEPTH}')
    if seen_dirs is None:
        seen_dirs = set()
    key = (sector, size)
    if key in seen_dirs:
        return
    seen_dirs.add(key)

    f.seek(base + sector * SECTOR)
    data = f.read(size)
    if len(data) < size:
        raise EOFError(
            f'truncated XISO directory at sector {sector}: '
            f'wanted {size} bytes, got {len(data)}'
        )
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
        if p + 14 + nlen > len(data):
            raise EOFError(
                f'truncated XISO directory entry at sector {sector} offset {off}'
            )
        name = _safe_name(data[p + 14:p + 14 + nlen].decode('latin-1'))
        if attr & 0x10:
            folder = prefix + name + '/'
            out.append((folder, 0, start))
            walk(f, base, start, fsize, folder, out, seen_dirs, depth + 1)
        else:
            out.append((prefix + name, fsize, start))
        stack += [x for x in (left, right) if x]


def list_entries(image):
    """Return sorted (path, size, start_sector) tuples from an XISO image."""
    with open(image, 'rb') as f:
        base, root_sector, root_size = find_partition(f)
        entries = []
        walk(f, base, root_sector, root_size, '', entries)
        entries.sort()
        return entries


def _destination_path(out_dir, path):
    """Join path under out_dir and refuse anything that escapes it."""
    out_root = os.path.realpath(out_dir)
    parts = [p for p in path.split('/') if p]
    for part in parts:
        _safe_name(part)
    dst = os.path.realpath(os.path.join(out_root, *parts))
    try:
        common = os.path.commonpath([out_root, dst])
    except ValueError as e:
        raise ValueError(f'XISO path escapes output directory: {path!r}') from e
    if common != out_root:
        raise ValueError(f'XISO path escapes output directory: {path!r}')
    return dst


def _copy_file(f, base, start, size, dst):
    """Copy size bytes from the image, failing cleanly on a short read."""
    os.makedirs(os.path.dirname(dst), exist_ok=True)
    f.seek(base + start * SECTOR)
    digest = hashlib.sha256()
    with open(dst, 'wb') as o:
        left = size
        while left:
            chunk = f.read(min(left, 1 << 20))
            if not chunk:
                raise EOFError(
                    f'truncated XISO file data at sector {start}: '
                    f'{left} bytes still needed'
                )
            digest.update(chunk)
            o.write(chunk)
            left -= len(chunk)
    return digest.hexdigest()


def extract(image, out_dir, paths=None):
    """Extract files from image into out_dir. paths=None extracts everything."""
    wanted = set(paths or ())
    digests = []
    with open(image, 'rb') as f:
        base, root_sector, root_size = find_partition(f)
        entries = []
        walk(f, base, root_sector, root_size, '', entries)
        entries.sort()
        for path, size, start in entries:
            if path.endswith('/') or (wanted and path not in wanted):
                continue
            dst = _destination_path(out_dir, path)
            digest = _copy_file(f, base, start, size, dst)
            digests.append((path, digest))
            print(f'{path}  sha256 {digest}')
    return digests


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument('image')
    ap.add_argument('out_dir', nargs='?')
    ap.add_argument('paths', nargs='*')
    args = ap.parse_args()

    if not args.out_dir:
        entries = list_entries(args.image)
        for path, size, _ in entries:
            print(f'{size:>12}  {path}')
        print(f'{len(entries)} entries, {sum(e[1] for e in entries)} bytes')
        return

    extract(args.image, args.out_dir, args.paths)


if __name__ == '__main__':
    main()
