# Halo 2 Decompiled (work in progress)

An open-source effort to decompile **Halo 2 for the original Xbox** into C
source code, and from there to port the game natively to PC, Linux and
handhelds. It follows the route that
[halo-ce-universal](https://github.com/cybersecurity/halo-ce-universal) took
for Halo: Combat Evolved.

**Status: early research.** The first two retail functions rebuild byte for
byte from C++, despite the build's link-time code generation. Refer to
[docs/PROGRESS.md](docs/PROGRESS.md), which is updated as work lands.

## The target

The retail disc build of Halo 2, the one every owner of the game has:

| | |
| --- | --- |
| `default.xbe` SHA-256 | `03215919bb7163259257d361f4c7bf802a7ab12aa85e2689436369b5c427935d` |
| Internal name | `halo2ship.exe` |
| Linked | 2004-09-28 |
| Xbox SDK | XDK 5849 (libraries 1.0.5849) |
| Compiler | Visual C++ 7.1 from XDK 5849 (`cl` 13.10.3077), `/O2 /GL /Gr`, linked with `/LTCG` |

## The approach

The aim is a **matching** decompilation: C source that the original compiler
turns back into the same bytes as the retail XBE. Matching gives every
function an automatic pass or fail, and that check is what made the Halo CE
decompilation dependable.

Halo 2's retail build is harder than Halo CE's. It was compiled with
**link-time code generation (LTCG)**: the compiler generates code for the
whole program at once, when it is linked. So functions are inlined across
source files, and internal functions get custom calling conventions. 42.5%
of the functions called in the retail XBE take arguments in `eax`, `ebx`,
`esi` or `edi`, which MSVC's standard conventions never do. Under LTCG, a
function's bytes depend on its callers and callees, not only on its own
source. We know of no earlier project that has matched such a build.

So the work runs in stages:

1. **Feasibility spike (in progress).** Rebuild retail functions with the
   XDK 5849 compiler in LTCG mode, and find out whether their bytes can be
   matched. The first two match, custom calling conventions and all; harder
   cases are next.
2. **Project set-up.** The build, a function inventory, and progress
   tracking, shaped by what the spike finds. If byte matching proves
   impractical, the fallback is to check each function's behaviour against
   the original machine code in an x86 emulator.
3. **Decompilation**, from the leaf functions up.
4. **Native port.**

## What is not here

No game files, no XBEs, and no Xbox SDK. You bring your own Halo 2 disc. The
Xbox SDK belongs to Microsoft and is not redistributed. Contributors who build
for matching get XDK 5849 themselves, as Halo CE contributors do for their
SDK.

## Tools

| Tool | Use |
| --- | --- |
| `tools/xiso_extract.py` | Lists or extracts the files of an Xbox disc image. |
| `tools/xbe.py` | Summarises an XBE: sections, linked libraries, certificate. |
| `tools/ltcg_probe.py` | Counts the functions that take arguments in registers (the LTCG evidence above). Needs `pip install capstone`. |
| `tools/match.py` | Builds a source file with the XDK 5849 compiler under LTCG and compares its functions with the retail XBE. Needs the SDK and capstone. |

To get `default.xbe` from your disc image:

```
python tools/xiso_extract.py "Halo 2.iso" orig default.xbe
```

## Credits

- [halo-symbol-atlas](https://github.com/tinkerer-red/halo-symbol-atlas)
  (CC BY 4.0): function names for Halo binaries, including this build and
  Bungie's May 2003 Halo 2 builds.
- [punpckhdq/halo](https://github.com/punpckhdq/halo),
  [bnunu/halo-1](https://github.com/bnunu/halo-1) and
  [halo-ce-universal](https://github.com/cybersecurity/halo-ce-universal):
  the Halo CE decompilation and port that this project follows.

This project is not affiliated with Microsoft, Bungie or 343 Industries.
Halo is a trademark of Microsoft.

## License

The contents of this repository are released under CC0 1.0. Refer to
[LICENSE](LICENSE).
