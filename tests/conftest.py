import os
import sys

import pytest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'tools'))

from xbe import retail_xbe_path, xdk_dir  # noqa: E402


@pytest.fixture
def retail_xbe():
    path = retail_xbe_path()
    if not os.path.exists(path):
        pytest.skip('retail XBE not present')
    return path


@pytest.fixture
def xdk_dir():
    path = xdk_dir()
    if not os.path.exists(os.path.join(path, 'bin', 'vc71', 'CL.Exe')):
        pytest.skip('Xbox SDK 5849 not present')
    return path
