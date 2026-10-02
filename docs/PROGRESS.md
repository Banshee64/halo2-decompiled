# Progress log

The newest entry comes first.

## 2026-10-02 (later): 163 functions match; C++ destructors and library callers

`python tools/check.py` reports:

```
matched 163 of 11802 game functions (17564 of 2782989 bytes, 0.63%)
matched 163 of 17586 functions in scope (17564 of 3730854 bytes, 0.47%)
```

**What matches now:**
- Random numbers: the seed, and random vectors in a cone.
- Hash tables, bit vectors and integer log2.
- Network-message counters.
- An actor action-slot system.
- A handle table whose classes use virtual methods.

**What the build now handles:**
- **Functions that library code calls.** Havok, the C runtime and the XDK
  libraries were not built with link-time code generation. Any game function
  they call keeps its standard calling convention. The build reads those
  callers from the inventory and models them. Three Havok-called methods now
  match without any changes to their source.
- **Deleting destructors.** The destructor slot of a vtable holds a function
  the compiler generates: it calls the destructor, then `operator delete`.
  A `deleting` marker now ties that function to its class, so a class is
  written as Bungie wrote it, even when its destructor is implicit.
- **Data tables of function pointers.** These are written only where retail's
  data actually holds them, at their retail addresses. For example, the
  handler struct at `0x47d930` is reproduced field for field.

**What the build taught us:**
- VC7.1 fully unrolls `for (i = 0; i < 3; i++)` over a small body. Retail's
  3-iteration loops were written `do { ... } while (i < 3);`.

## 2026-10-02: 132 functions match; virtual methods supported

`python tools/check.py` reports:

```
matched 132 of 11802 game functions (14647 of 2782989 bytes, 0.53%)
matched 132 of 17586 functions in scope (14647 of 3730854 bytes, 0.39%)
```

**How the work is organised.** Parallel workers each take a batch of
neighbouring functions. A batch is usually one original source file. Workers
send their work back in waves. Each wave is merged into `main`, and the
globals that several files share are unified in `include/globals.h`. Wave 2
and wave 3 together added 91 matches.

**What matches now:**
- Object list management and the object header table.
- Localized wide-string getters.
- Game state globals, and more of the AI and animation code.
- Several C++ classes, including a class that overrides two virtual methods of
  its interface.

**What the build taught us:**
- **Bit flags:** flag words are 1-bit bitfields tested with a `bool` cast
  (`TEST_FIELD_BIT` in `cseries.h`). That is the only form that compiles to
  retail's `shr reg, N; test reg, 1` sequence. It appears about 385 times in the
  game.
- **Virtual methods:** stand-ins now call methods by qualified name and
  copy-construct each class that has a vtable. A virtual method's address then
  escapes into the vtable, as in retail, and LTCG keeps its `thiscall`
  convention. Virtual methods are written as Bungie wrote them, with no
  workarounds.
- **Inlining across files:** LTCG inlines a small function into callers in
  other files unless that function's own file is built with `/Ob1`. Per-file
  flags therefore matter across files.
- **Caller-driven conventions:** some near-misses differ only in which
  registers carry their arguments. LTCG picks those registers from the
  callers, which are not decompiled yet. The checker re-tests every function
  on each build, so these can turn into matches later.
- **More code outside Bungie's:** a Havok collision query inlined into game
  code at `0x183910` was excluded from game code.

## 2026-10-02: decompilation has started: 41 functions match

`python tools/check.py` reports:

```
matched 41 of 11815 game functions (4923 of 2785826 bytes, 0.18%)
matched 41 of 17599 functions in scope (4923 of 3733691 bytes, 0.13%)
```

**What matches.** 41 retail functions rebuild byte for byte:
- File path helpers in `files_windows.obj`: `file_path_add_name`,
  `file_path_add_extension` and `file_path_remove_name`.
- Unicode classification and UTF-8 encoding (`unicode.obj`).
- 3x3 and 4x3 matrix maths, including two hand-written assembly routines.
- AI firing-position evaluation and AI clumps.
- Recorded-animation playback readers.
- S3TC and texture helpers.
- The CRC functions and the game state allocator from the spike.

About 20 more are near-misses: the same length, but the register allocation or
operand order differs.

**Where Bungie's code ends.** Bungie's code ends at `0x2cb8c0`. Everything
above that in `.text` is Xbox SDK libraries and third-party code: Havok, Bink,
the C runtime, voice, WMA, DSOUND and compiler-generated stubs. Applying that
boundary in `config/owners.json` cut the game-code total from 12,959 to 11,815
functions. Two regions that the symbol atlas had named as game code turned out
to be Havok physics code.

**What the build taught us:**
- Floating-point code needs `/arch:SSE` (some files need `/arch:SSE2`), because
  Bungie's build used SSE.
- Callbacks stored in tables are `__stdcall`.
- Some maths routines are hand-written inline assembly.
- Game code that calls Direct3D called the public D3D API. The SDK's
  `d3d8ltcg.lib` is linked, and LTCG inlines parts of it, as in retail.

**Tooling changes:**
- Per-file compiler flags now live in the source, in a `// @flags` comment.
- The build links the SDK libraries.
- The checker also verifies call targets. A call must reach the function with
  the same retail address or the same name.
- Stand-in callers are generated inside each source file's own translation
  unit.
- `tools/permute.py` searches variants of a source function for near-misses.

**Next.** Fix the near-misses, then continue up from the leaf functions with
`tools/ready.py`.

## 2026-10-01: project set-up: inventory, whole-game build, checker

The set-up is in place. `python tools/check.py` reports:

```
matched 8 of 12959 game functions (421 of 2891676 bytes, 0.01%)
matched 8 of 17599 functions in scope (421 of 3733691 bytes, 0.01%)
```

Eight functions are MATCH: `crc_checksum_buffer`, `build_crc_table`,
`game_state_malloc`, `distance3d`, `_real_random_range` and three game state
initializers. `game_state_malloc_aligned` (`0x123d80`) is the one near-miss: a
single `lea` operand order.

**The inventory** (`config/functions.csv`) has 19,509 functions. By owner:

| Owner | Functions |
| --- | ---: |
| `game` | 12,959 |
| `eh` (MSVC exception-handling stubs) | 586 |
| `third:havok` | 1,034 |
| `third:bink` | 290 |
| `xdk:xonline` | 915 |
| `xdk:xvoice` | 816 |
| `xdk:wmadec` | 649 |
| `xdk:dsound` | 554 |
| `xdk:xnet` | 416 |
| `xdk:libcmt` | 394 |
| `xdk:xapi` | 270 |
| `xdk:d3d8` | 254 |
| `xdk:xapilib` | 219 |
| `xdk:libcpmt` | 93 |
| `xdk:d3dx` | 36 |
| `xdk:xonlines` | 23 |
| `xdk:rockall` | 1 |

Only `game` functions are ours to decompile. "In scope" is everything except
`eh`, Havok and Bink (19,509 less 586, 1,034 and 290 is 17,599).

**What changed from the spike's tool.** `tools/check.py` replaces
`tools/match.py`:
- Exact extents. Ours come from the linker map and retail's from the
  inventory. When the rest of our function is `0xCC` fill, retail's size is
  compared.
- Exact masks. The masked bytes are the base relocations plus the linker's
  `/MAPINFO:FIXUPS` relative fields, not guesses from values.
- Each masked field is validated against retail: an absolute field must hold a
  retail-image address, a relative field must leave the function, and every
  field must lie inside both extents. A field that points inside the function
  (a jump table entry, a self-call) must point at the same offset in retail.
- Status (`matched`, `near`, `todo`) is written back to
  `config/functions.csv`.

**What we found while building the inventory:**
- Discovery needed end clamping, and had to drop weak starts that land in the
  middle of an instruction.
- Library code is recognised by byte signature from the SDK's own `.lib`
  files (CRT, XAPI, DSOUND, XONLINE, XVOICE, XNET, D3DX) and by section (D3D8,
  XPP, Bink, WMA).
- Unnamed functions take the owner of their neighbours.
- The 586 MSVC exception-handling stubs are classed `eh`: they are
  compiler-generated, not work items, and the checker does not check them.
- A bug that let the checker accept a wrong address field (an absolute field
  was masked without checking that retail held an address there) was fixed
  before first use.

**What we found about `PRIVATE`.** Bungie's static functions can be
compiled with external linkage (the `PRIVATE` macro is empty). That did not
change `build_crc_table`'s code, which still matches. So generated stand-in
callers in other files can reach static functions.

**Note.** The disassembler is `tools/disasm.py`, not `dis.py`: a `dis.py`
would shadow Python's standard-library `dis` module.

**Next.** Decompile from the leaf functions up, picking work with
`tools/ready.py`.

## 2026-10-01: the spike's answer: the LTCG build can be matched

Eight retail functions now rebuild byte for byte, and a ninth is one
instruction short:

| Function | Retail | What it tests |
| --- | --- | --- |
| `crc_checksum_buffer` | `0x163ba0` | a custom calling convention |
| `build_crc_table` | `0x163c00` | a custom calling convention |
| `game_state_malloc` | `0x123d40` | an argument moved from the stack to `eax` |
| a game state initializer | `0x1edbc0` | `game_state_malloc` inlined into a caller optimized for speed |
| two game state initializers | `0x24c819`, `0x165cc3` | callers optimized for size, which call it out of line |
| `distance3d` | `0x3ea30` | x87 floating point and evaluation order (the body only) |
| `_real_random_range` | `0x259d0` | LTCG deleting unused arguments (the body only) |
| `game_state_malloc_aligned` | `0x123d80` | one instruction short: `lea eax, [ebx + ecx]` against our `[ecx + ebx]` |

The game state functions and both CRC functions match together in one LTCG
image, with each source file built with its own flags. "The body only" means
the function matches when kept out of line. What keeps retail's copies of
these two out of line is not yet known.

**What the spike found about Bungie's build:**

- **Two kinds of code.** Most is optimized for speed (`/O2`): functions
  aligned to 16 bytes, no frame pointer. Some is optimized for size (`/O1`):
  functions packed without padding, `ebp` frames, `push 4; pop ecx`. LTCG
  keeps each source file's flags.
- **Inlining follows the flags.** `game_state_malloc` is inlined at about 51
  call sites, all in code optimized for speed. It is called at 7, all in code
  optimized for size. Our compiler makes the same choices from the same
  flags.
- **Some files need `/Ob1`.** With `/Ob2`, our compiler inlines
  `crc_checksum_buffer` into `game_state_malloc`, which retail does not. With
  `crc.cpp` built `/Ob1`, everything matches. Which files differ like this
  is still to be mapped.
- **The source's shape matters, as in any matching decompilation.** Examples:
  - `short` loop counters.
  - The order of the terms in `distance3d`.
  - Checksumming a local copy rather than a parameter. Taking a parameter's
    address keeps it on the stack.
- **SSE.** Retail uses `movss`, `ucomiss`, `xorps` and `fcomi` in some float
  code. We have not reproduced that in a test yet.

**The tool.** `tools/match.py` now takes several source files, each with its
own flags, and links them into one image. It masks only the bytes that the
test image relocates; before, it guessed from values.

**Next.** We will design the project set-up:
- the build, with per-file flags;
- a function inventory of the retail XBE;
- a way to tell each function's flags from its code;
- progress tracking.

## 2026-10-01: the first two functions match

**LTCG can be matched.** Two retail functions now rebuild byte for byte:

| Function | Retail address | Arguments in retail |
| --- | --- | --- |
| `crc_checksum_buffer` | `0x163ba0` | buffer in `eax`, size in `edi`, CRC pointer on the stack (`ret 4`) |
| `build_crc_table` | `0x163c00` | table in `edx` |

- **The source:** Halo CE's `crc.c` from the
  [punpckhdq/halo](https://github.com/punpckhdq/halo) decompilation (CC0),
  compiled as C++.
- **The toolchain:** XDK 5849's compiler, `/O2 /GL /Gr` (fastcall by default,
  as Bungie's 2003 profile build), linked with `/LTCG`.
- **The calling conventions:** the compiler chose the same custom ones as the
  retail build, from the source alone.
- **What "byte for byte" means here:** every instruction and every byte is
  equal, except the data addresses, which differ because the test image lays
  out its own data.
- **One source detail mattered:** the loop counters must be `short`, as in
  Halo CE. With `long` ones, the compiler unrolls the inner loop.

This answers the spike's main question for simple functions: the XDK 5849
compiler and these flags reproduce Bungie's LTCG output. The next tests use
harder cases: floating point, C++ member functions, and functions whose
callees were inlined.

Reproduce it with `tools/match.py`; the command is at the top of
`spike/crc.cpp`.

## 2026-10-01: the target, the toolchain, and LTCG

**The target build.** The retail disc's `default.xbe`:

- SHA-256 `03215919bb7163259257d361f4c7bf802a7ab12aa85e2689436369b5c427935d`.
- Internal name `c:\halo2\bin\halo2ship.exe`, linked 2004-09-28.
- Title ID `4D530064`, all regions.
- `.text` is 0x367C4C bytes (3.4 MiB) at 0x12000.
- Linked libraries:
  - XAPILIB, XBOXKRNL, LIBCMT and LIBCPMT
  - DSOUND, XVOICE, VOICMAIL and XONLINES
  - D3D8LTCG and XGRAPHCL

  Every one is version 1.0.5849.
- Its own sections hold DSOUND, WMADEC, XONLINE, XNET, Bink, D3D and XPP.

**Earlier work.** We found no existing Halo 2 decompilation.
[halo-symbol-atlas](https://github.com/tinkerer-red/halo-symbol-atlas) has
useful names:

| Build | Names | Source |
| --- | ---: | --- |
| Retail (this target) | 1,854 | Propagated from other builds |
| `halo2tagdebug.xbe`, 2003-05-03 | 19,830 | Bungie's linker map |
| `halo2debug.xbe`, 2003-05-03 | 18,592 | Bungie's linker map |
| `halo2profile.xbe`, 2003-05-03 | 13,443 | Bungie's linker map |

The 2003 maps also give the object file (`.obj`) each function came from,
which maps out the source files.

**LTCG.** The retail game code was compiled with link-time code generation:

- `tools/ltcg_probe.py` finds 9,954 direct call targets in `.text`.
- 4,232 of them (42.5%) read `eax`, `ebx`, `esi` or `edi` at entry before
  writing them. MSVC's standard conventions never pass arguments in those
  registers.

Some examples:

| Function | Arguments |
| --- | --- |
| `0x128c0` | A pointer in `eax` |
| `0x14390` | Values in `edi` and `esi` |
| `0x16b10` | A pointer in `esi` |

That rules out the usual decompilation workflow, where each function is
compiled alone and compared with the original. Under LTCG the code is
generated for the whole program at once, when it is linked.

**The toolchain.** XDK 5849 ships its own compiler in `xbox\bin\vc71`:

- `cl` 13.10.3077 and `link` 7.10.3077 (Visual C++ .NET 2003).
- Both run natively on Windows 11.
- The SDK's `d3d8ltcg.lib` and `xgraphicsltcg.lib` are the LTCG forms of the
  libraries linked into the XBE.

**Next.** The feasibility spike asks whether XDK 5849's compiler, in LTCG
mode, can reproduce retail functions byte for byte.
