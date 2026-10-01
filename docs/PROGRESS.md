# Progress log

The newest entry comes first.

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
