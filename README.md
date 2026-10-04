# Halo 2 Decompilation (work in progress)

The **Halo 2 decompilation** project: an open-source, *matching*
decompilation of **Halo 2 for the original Xbox**. We are rebuilding the
retail game as C and C++ source code that the original compiler turns back
into exactly the same bytes. From there, the goal is to port Halo 2 natively
to PC, Linux and handhelds, as
[halo-ce-universal](https://github.com/cybersecurity/halo-ce-universal) did
for Halo: Combat Evolved.

**Contributors are welcome.** No prior decompilation experience is needed,
only patience and some C. Read [How to help](#how-to-help) and
[CONTRIBUTING.md](CONTRIBUTING.md).

## Status

Decompilation is under way, and 4549 retail functions now match byte for
byte. The checker reports:

```
matched 4549 of 11321 game functions (412575 of 2785198 bytes, 14.81%)
matched 4549 of 17215 functions in scope (412575 of 3739274 bytes, 11.03%)
```

Matched code so far includes:
- the script engine's built-in functions;
- AI, actor behaviours and actor slot handlers;
- object damage, shields and vitality;
- network sessions, message codecs and the bitstream;
- UI screens and widgets, and the game engines;
- sound sources, looping sounds and Bink movie playback;
- the game's data arrays, object lists and hash tables;
- geometry, quaternion and matrix maths;
- input, file paths, localized strings, random numbers and game state.

Every function is checked automatically on every build, so a match stays a
match. Refer to [docs/PROGRESS.md](docs/PROGRESS.md), which is updated as work
lands.

## The target

The retail disc build of Halo 2, the one every owner of the game has:

| | |
| --- | --- |
| `default.xbe` SHA-256 | `03215919bb7163259257d361f4c7bf802a7ab12aa85e2689436369b5c427935d` |
| Internal name | `halo2ship.exe` |
| Linked | 2004-09-28 |
| Xbox SDK | XDK 5849 (libraries 1.0.5849) |
| Compiler | Visual C++ 7.1 from XDK 5849 (`cl` 13.10.3077), `/GL /Gr` with `/O2` or `/O1` per file, linked with `/LTCG` |

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

1. **Feasibility spike (done).** Rebuild retail functions with the XDK 5849
   compiler in LTCG mode, and find out whether their bytes can be matched.
   They can: custom calling conventions, deleted arguments and inlining
   decisions all reproduce. The test sources are in `spike/`.
2. **Project set-up (done).** The function inventory, the build with each
   source file's flags, the checker and the ready queue.
3. **Decompilation (in progress)**, from the leaf functions up.
4. **Native port.**

## What is not here

No game files, no XBEs, and no Xbox SDK. You bring your own Halo 2 disc. The
Xbox SDK belongs to Microsoft and is not redistributed. Contributors who build
for matching get XDK 5849 themselves, as Halo CE contributors do for their
SDK.

## Build and check

1. Install the Python dependencies: `pip install -r requirements-dev.txt`.
2. Supply the SDK and the XBE. Put XDK 5849's `xbox` folder at `sdk/xbox`
   (or set `XDK_DIR` to it) and the retail XBE at `orig/default.xbe` (or set
   `RETAIL_XBE`). Both folders are git-ignored. In Git Bash, also run
   `export MSYS_NO_PATHCONV=1`, which stops Git Bash rewriting the
   `/`-style arguments (such as `/O2`) that the SDK tools take. To get the
   XBE from your disc image:

   ```
   python tools/xiso_extract.py "Halo 2.iso" orig default.xbe
   ```
3. The inventory, `config/functions.csv`, is already committed, so most
   people never rerun this step. To regenerate it, get
   [halo-symbol-atlas](https://github.com/tinkerer-red/halo-symbol-atlas) and
   run `python tools/inventory.py --atlas <atlas>/symbols/halo_2/03215919bb7163259257d361f4c7bf802a7ab12aa85e2689436369b5c427935d.jsonl`.
4. Run `python tools/check.py`. It builds the whole game as one LTCG image,
   compares every decompiled function with the retail bytes, and writes each
   function's status (`matched`, `near` or `todo`) back to
   `config/functions.csv`. It exits 1 while any function differs.
5. Pick work with `python tools/ready.py`, following
   [docs/DECOMPILING.md](docs/DECOMPILING.md).

| Tool | Use |
| --- | --- |
| `tools/xiso_extract.py` | Lists or extracts the files of an Xbox disc image. |
| `tools/xbe.py` | Summarises an XBE: sections, linked libraries, certificate. |
| `tools/ltcg_probe.py` | Counts the functions that take arguments in registers (the LTCG evidence above). Needs capstone. |
| `tools/inventory.py` | Finds every function in the XBE, names it from the atlas, and writes `config/functions.csv`. |
| `tools/functions.py` | Function discovery that the inventory uses. |
| `tools/libsig.py` | Recognises library code by byte signature from the SDK's `.lib` files. |
| `tools/build.py` | Builds the whole game as one LTCG image, with each source file's flags. |
| `tools/check.py` | Compares our functions with retail and records progress. Needs the SDK and capstone. |
| `tools/ready.py` | Lists the functions that are ready to decompile next, with their likely source file (`--by-file` groups them). |
| `tools/permute.py` | Searches variants of a source function for ones that turn a near-miss into a match. |
| `tools/disasm.py` | Disassembles retail code. |
| `tools/match.py` | The spike's one-file matcher, kept for reference. Replaced by `check.py`. |

## How to help

Halo 2 has about 11,300 game functions, so there is room for many people.
1. Get set up as in [Build and check](#build-and-check): your own Halo 2
   disc and XDK 5849.
2. Pick a source file or an address range nobody has claimed (the pinned
   [Active claims](https://github.com/kirklandsig/halo2-decompiled/issues/9)
   issue lists them), and open a draft pull request saying what you are
   taking.
3. Decompile, run `python tools/check.py`, and push as functions match.
   [docs/DECOMPILING.md](docs/DECOMPILING.md) explains the conventions and
   the compiler's quirks.

Details are in [CONTRIBUTING.md](CONTRIBUTING.md). Questions are welcome as
issues.

## Credits

- [halo-symbol-atlas](https://github.com/tinkerer-red/halo-symbol-atlas)
  (CC BY 4.0): function names for Halo binaries, including this build and
  Bungie's May 2003 Halo 2 builds.
- [punpckhdq/halo](https://github.com/punpckhdq/halo),
  [bnunu/halo-1](https://github.com/bnunu/halo-1) and
  [halo-ce-universal](https://github.com/cybersecurity/halo-ce-universal):
  the Halo CE decompilation and port that this project follows.
- [BirchWoodGod/halo2-decomp](https://github.com/BirchWoodGod/halo2-decomp):
  a functional recovery of the same XBE. Its published analysis gave us
  leads: the subsystem lifecycle table, the data-array layout and some
  register conventions. We verified each one against the retail code
  ourselves; none of its code or text is used here.

This project is not affiliated with Microsoft, Bungie or 343 Industries.
Halo is a trademark of Microsoft.

## License

The contents of this repository are released under CC0 1.0. Refer to
[LICENSE](LICENSE). The exception is the `name` and `object` columns of
`config/functions.csv`: they come from
[halo-symbol-atlas](https://github.com/tinkerer-red/halo-symbol-atlas), are
reformatted into the CSV, and remain under
[CC BY 4.0](https://creativecommons.org/licenses/by/4.0/). Everything else is
CC0.
