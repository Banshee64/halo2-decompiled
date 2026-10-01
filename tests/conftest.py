import os
import sys

import pytest

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))


@pytest.fixture
def retail_xbe():
    path = os.environ.get('RETAIL_XBE', os.path.join(ROOT, 'orig', 'default.xbe'))
    if not os.path.exists(path):
        pytest.skip('retail XBE not present')
    return path


@pytest.fixture
def xdk_dir():
    path = os.environ.get('XDK_DIR', os.path.join(ROOT, 'sdk', 'xbox'))
    if not os.path.exists(os.path.join(path, 'bin', 'vc71', 'CL.Exe')):
        pytest.skip('Xbox SDK 5849 not present')
    return path
