# Progress log

The newest entry comes first.

## 2026-10-03 (night): 1703 functions match; five region lanes merged

```
matched 1703 of 11317 game functions (131004 of 2783395 bytes, 4.71%)
matched 1703 of 17069 functions in scope (131004 of 3731252 bytes, 3.51%)
```

Five region lanes landed their stints:
- **lane A** (0x2A0000): 127 more of the script engine's built-in functions;
- **lane B** (0x1B0000): 20 more actor slot handlers;
- **lane D** (0x060000): 91 functions of the simulation world, online tasks and
  network sessions;
- **lane E** (0x2B0000–0x2CB8C0): 103 UI screen, list and widget methods, three
  game engines and the uncompressed animation codecs;
- **lane F** (0x180000): 51 functions for local player slots, player control,
  looping sounds, sound sources and the loop allocator.

Two more regular batches added game-speed, camera and quaternion code. The
checker now treats libcmt's `memmove` and `memcpy` as one function, since they
are the same code.

The README and the new [CONTRIBUTING.md](../CONTRIBUTING.md) explain how to
join in.

## 2026-10-03 (late morning): 1292 functions match; AI code and nine more batches

```
matched 1292 of 11317 game functions (101876 of 2783395 bytes, 3.66%)
matched 1292 of 17069 functions in scope (101876 of 3731252 bytes, 2.73%)
```

- **Lane C** (0x1C0000–0x1CFFFF) finished its first stint: 64 AI and actor
  slot-handler functions, including `src/ai.cpp`. The link now reads its
  object list from a response file, since the command line had outgrown
  Windows' 32K limit.
- **Nine regular batches** landed: geometry and axis tables, input records,
  visibility tests and render state, handle tables, aim assist, transport
  addresses and more. Their duplicated constants, inline vector helpers and
  types now live in shared headers.

## 2026-10-03 (morning): 1183 functions match; joint behaviour and Bink playback

```
matched 1183 of 11317 game functions (89984 of 2783395 bytes, 3.23%)
matched 1183 of 17069 functions in scope (89984 of 3731252 bytes, 2.41%)
```

Two more files from @Banshee64 are merged: `joint_behavior.cpp` (actor joint
behaviour, whose seven callbacks are now wired into the slot-handler tables
with their retail `__stdcall` convention) and `bink_playback.cpp` (Bink movie
playback and its memory callbacks).

The checker now handles identical functions that the linker folded into one
body: a call into one of them matches through any of their names.

## 2026-10-03 (early morning): 1166 functions match; shared engine headers

```
matched 1166 of 11317 game functions (88136 of 2783395 bytes, 3.17%)
matched 1166 of 17069 functions in scope (88136 of 3731252 bytes, 2.36%)
```

Six more batches landed: text formatting, game-engine marker objects,
geometry helpers, random-number users and player state. Duplicated
declarations from those batches are now shared: one `c_game_engine` class
in `include/game_engine.h`, common float helpers in `include/real_math.h`,
and the player-state and match-globals layouts in `include/globals.h`.

## 2026-10-03 (late night): 1125 functions match; independent region lanes

```
matched 1125 of 11317 game functions (83039 of 2783395 bytes, 2.98%)
matched 1125 of 17069 functions in scope (83039 of 3731252 bytes, 2.23%)
```

**Lanes.** Work now also runs in "lanes". A lane is an independent agent that
owns one address region and works through it on its own branch, like an
outside contributor. Two lanes finished their first stint:
- one decompiled 143 of the script engine's built-in function evaluators, each
  followed by its retail definition data;
- one decompiled 65 actor slot-handler (AI behaviour) callbacks, with their
  handler structs at retail addresses.

**Contributors.** @Banshee64's `transport_address.cpp` is merged, and more
object files are in progress. Claimed address ranges are kept free of our
automated work.

## 2026-10-03 (night): 913 functions match; network message codecs

```
matched 913 of 11317 game functions (67094 of 2783395 bytes, 2.41%)
matched 913 of 17069 functions in scope (67094 of 3731252 bytes, 1.80%)
```

**Network messages.** Each network message type has an encoder, a decoder and
a clear/compare callback. A registration function stores them in the
message-type table. The codecs, and the bit-stream module they write
through, are now decompiled, and most of them match byte for byte.

**The data arrays are complete.** All 18 core handle-pool routines match.

**More contributors.** @Banshee64 contributed `game_allegiance.cpp` and
`input_xbox.cpp`: two whole original object files, identified from Bungie's
2003 debug map. Work is coordinated by address range, so contributors don't
collide.

## 2026-10-03 (evening): 723 functions match; subsystem lifecycle callbacks

```
matched 723 of 11317 game functions (40978 of 2783395 bytes, 1.47%)
matched 723 of 17069 functions in scope (40978 of 3731252 bytes, 1.10%)
```

**Subsystem lifecycle.** The engine starts and stops its subsystems through a
table of 68 entries (`0x440DD8`). Each entry has up to nine callbacks:
initialize, dispose, per-map and per-BSP set-up and teardown, and change
notifications. 55 of 58 callbacks attempted so far match. They are named after
their subsystems: `players_initialize`, `decals_dispose`,
`arena_initialize_for_new_map` and so on.

**Also matching:**
- the session-state manager;
- script value casts;
- `csnprintf`;
- color conversions;
- binary search and short sort;
- HUD helpers;
- model variant lookups;
- path-finding heap operations.

## 2026-10-03 (later): 612 functions match; the core data arrays

```
matched 612 of 11317 game functions (34854 of 2783395 bytes, 1.25%)
matched 612 of 17069 functions in scope (34854 of 3731252 bytes, 0.93%)
```

**Data arrays.** The engine keeps most of its runtime state in handle-addressed
pools: players, objects, effects, AI and more. 16 of the 18 core routines now
match:
- create and dispose;
- allocating an element at the next free slot or a given one;
- deleting by handle;
- handle-to-pointer lookup;
- iteration.

`include/data_array.h` is the single type every subsystem now uses.

**Also matching:**
- the unit, item, weapon, projectile and device object types;
- the game-engine player entity and breakable-surface entity definitions;
- about 25 event-definition classes;
- surface descriptions;
- tag lookups.

**Contributions.** The tools now also run on Linux under Wine, thanks to a
pull request from @Banshee64.

## 2026-10-03: 442 functions match; more than 1% of the game's code

```
matched 442 of 11317 game functions (29446 of 2783395 bytes, 1.06%)
matched 442 of 17069 functions in scope (29446 of 3731252 bytes, 0.79%)
```

**Switch statements are whole functions now.** Function discovery used to
record each `switch` case label, and the jump tables themselves, as separate
functions. Now they belong to the function that owns them: 525 fragment rows
are gone, and 126 functions have their full extent. The game-function total
dropped from 11,802 to 11,317 for that reason, not because work was lost.

**Also matching:**
- the game-engine class;
- network session state classes;
- UI widgets;
- the vehicle and turret type classes;
- table-driven data readers;
- input mapping;
- and more.

**Next.** The engine's core data-array routines (handle-addressed pools that
nearly every subsystem uses) are decompiled and waiting to merge. They
unblock many callers whose calls into those routines could not match while
the routines were stubs.

## 2026-10-03 (early): 354 functions match; object types as C++ classes

```
matched 354 of 11802 game functions (26036 of 2782989 bytes, 0.94%)
matched 354 of 17586 functions in scope (26036 of 3730854 bytes, 0.70%)
```

**Object types are a class hierarchy.** The vtables for vehicles, turrets and
related object types share most of their slots. Every slot that two or more
of these vtables share is now a method of one base class,
`c_object_type_definition`, in `include/object_type_definitions.h`. Each type's
own overrides live in its derived class. The methods decompiled so far match
with the class written as plain C++.

**Also matching:**
- two interface vtables of 21 slots each, almost entirely;
- table-driven data readers;
- 256-bit bit-vector helpers;
- string and Unicode helpers, including variadic formatting functions.

**Build fixes.** Stand-ins now handle variadic functions. Stubs for code that
is not decompiled yet are built without function-level linking, so the
linker no longer folds identical ones into one address.

## 2026-10-02 (night): 271 functions match

```
matched 271 of 11802 game functions (22986 of 2782989 bytes, 0.83%)
matched 271 of 17586 functions in scope (22986 of 3730854 bytes, 0.62%)
```

**Tables lead to whole families of functions.** Retail's data holds tables of
function pointers: callback tables and C++ vtables. The functions one table
points at usually come from one source file and share one shape. A table at
`0x470828` led to 62 field-descriptor callbacks. All 62 now match, with the
table itself rebuilt entry for entry. A scanner now finds such tables. The
next batches are drawn from them: more vtables and callback tables.

**Also matching since 163:**
- input-device state;
- timed screen effects;
- Direct3D texture and palette set-up;
- a page heap that implements a library allocator;
- object iteration helpers;
- input mapping;
- more class hierarchies, with their deleting destructors.

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
