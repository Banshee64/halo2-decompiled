import os
import subprocess

import pytest

from pe import Pe

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


@pytest.mark.sdk
def test_pe_reads_base_relocations(xdk_dir, tmp_path):
    vc = os.path.join(xdk_dir, 'bin', 'vc71')
    src = os.path.join(ROOT, 'spike')
    for name in ('crc', 'crc_test'):
        subprocess.run([os.path.join(vc, 'CL.Exe'), '/nologo', '/c', '/GL', '/O2', '/Gr',
                        os.path.join(src, name + '.cpp'), f'/Fo{tmp_path / name}.obj'], check=True)
    subprocess.run([os.path.join(vc, 'Link.Exe'), '/nologo', '/LTCG', '/NODEFAULTLIB', '/ENTRY:entry',
                    '/SUBSYSTEM:CONSOLE', '/FIXED:NO', f'/OUT:{tmp_path / "t.exe"}',
                    str(tmp_path / 'crc.obj'), str(tmp_path / 'crc_test.obj')], check=True)
    image = Pe(str(tmp_path / 't.exe'))
    assert image.base == 0x400000
    assert image.fixups, 'a /FIXED:NO image has base relocations'
    assert all(image.base <= f < image.base + 0x100000 for f in image.fixups)
    assert len(image.read(image.base + 0x1000, 16)) == 16
