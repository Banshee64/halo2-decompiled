# Project set-up: design

Approved 2026-10-01. This turns the feasibility spike (docs/PROGRESS.md) into
a project that can match the retail XBE function by function, with many
workers at once.

## Goal and done-criteria

The project is set up when all five hold:

1. **Inventory.** Every function in the retail `.text` is listed in
   `config/functions.csv`. Each row has its address, size, owner (game, XDK
   library, or third-party), compile style, atlas name (where known) and
   status.
2. **Build.** `tools/build.py` compiles every source in `src/`, each file
   with its own flags, into one LTCG image.
3. **Check.** `tools/check.py` compares every function that has source with
   retail. It writes a progress report, `build/report.json` plus a summary
   printed and copied into the README.
4. **Spike.** The spike's eight matching functions are in `src/` and match
   in the whole-game build.
5. **Workflow.** There is a ready queue (`tools/ready.py`) and a documented
   procedure for decompiling a function, written so that a subagent can
   follow it alone.

## Constraints

- **No proprietary files in the repository.** That means no game files,
  XBEs, SDK files, Havok or Bink code, or leaked material. Contributors
  supply the retail XBE (`orig/default.xbe`) and XDK 5849 (`sdk/xbox`, or
  `XDK_DIR`).
- **Windows only, for the build.** The SDK's compiler is a Windows program.
  The analysis tools (inventory, ready queue) are plain Python with capstone,
  and run anywhere.
- **Names from halo-symbol-atlas are used with credit** under CC BY 4.0.
  Code adapted from the Halo CE decompilation is CC0.

## Components

### 1. Inventory: `tools/inventory.py` → `config/functions.csv`

It finds functions in three steps.

**Starting points:**
- the entry point;
- every direct call target;
- the atlas names;
- pointers into `.text` found in data (vtables, callback tables) and in code
  (`push imm`, `mov reg, imm`).

**Bodies.** Recursive-descent disassembly from the starting points, following
jumps. A body includes its MSVC jump tables (`jmp [reg*4 + table]`) and stops
at `ret` or a tail `jmp`. Bytes that are reached but belong to no start
become starts of their own. Padding (`int3`, `lea` no-ops) is trimmed.

**Owner.** In order of precedence:
1. **XDK library code** (CRT, XAPI, DSOUND, XONLINE, XVOICE, XNET) is
   matched by signature. Each `.obj` member's function bytes are compared
   with retail, with the member's COFF relocations masked.
2. **XDK LTCG libraries** (D3D8, XGRAPHICS): the functions in the `D3D` and
   `XPP` sections and the atlas's library tags.
3. **Third party:** Havok (`hk*` names, and its address range), Bink, the
   WMA decoder and zlib inside D3DX, found by sections and atlas tags.
4. Everything else is **game**.

**Style.** It is `speed` when the function is aligned to 16 bytes, has
padding before it and has no `ebp` frame. It is `size` when the function is
packed, has an `ebp` frame or uses the `push n; pop reg` idiom. The
inventory records the evidence, because style decides a file's flags.

**Columns:** va, size, owner, style, evidence, name, calls, source, status. `evidence` is the style's clues (a16, pad, ebp, packed); `calls` is the space-separated addresses it calls or tail-jumps to.
- `source`: the `src/` file, once assigned.
- `status`: `todo`, `matched`, `near` (within a few instructions) or `skip`.
- The inventory recomputes the analysis columns. The `source` and `status`
  columns are kept by `tools/check.py`, so contributors never edit the CSV
  by hand.

### 2. Layout

| Path | What |
| --- | --- |
| `src/<file>.cpp` | One file per original source file, named after Bungie's object files (`crc.obj` → `src/crc.cpp`). Functions with no known file go to `src/unknown_<address-range>.cpp` until placed. |
| `include/` | Shared declarations: `unknown_11c920.h` (basic types), engine structures and SDK stand-ins as needed. |
| `config/files.json` | Each source file's flags: `{"crc.cpp": ["/O2", "/Ob1", "/Gr"], ...}`, with a default of `/O2 /Gr`. |
| `config/functions.csv` | The inventory. |
| `spike/` | Kept as the feasibility record. |

### 3. Build: `tools/build.py`

1. Compile each `src/*.cpp` with `/c /GL /Gr` and its flags into
   `build/obj/`. A file is recompiled only when it, or a header it includes,
   has changed.
2. Generate `build/gen/standins_<file>.cpp`: one size-optimized
   (`/O1 /Ob0`) caller for each function marked `// @retail`. It keeps the
   function linked and out of line, as retail's own callers do. Every marked
   function gets one, even when another `src/` function calls it; the spike
   showed that extra callers do not change the callee's code.
   - **The call:** each stand-in calls the function directly, never through
     a pointer (a function whose address is taken keeps the standard
     convention). Its prototype comes from the definition that follows the
     function's `// @retail 0x...` comment.
   - **The arguments** are read from a `volatile` buffer, so the compiler
     cannot propagate constants into the function.
   - **Static functions** are written `PRIVATE`. The macro expands to nothing
     in our builds, so a stand-in in another file can reach the function.
     Because LTCG sees the whole program, the function's linkage should not
     change its code. The build's first test checks that claim on
     `function_163c00`.
3. Generate `build/gen/entry.cpp`, which calls every stand-in.
4. Link with `/LTCG /NODEFAULTLIB /MAP /MAPINFO:FIXUPS /FIXED:NO` and the
   SDK libraries that the matched functions need. `/MAPINFO:FIXUPS` lists
   every relative field that the linker resolved.
5. Calls to code we do not decompile go to stubs, compiled **without**
   `/GL` so that they keep the standard conventions retail uses for them.
   This covers Havok, and SDK functions while their libraries are not
   linked.

### 4. Check: `tools/check.py` (built from today's `tools/match.py`)

- **The functions to check:** every function marked `// @retail 0x...` in
  `src/`.
- **Our extent:** from the linker map, from the symbol to the next symbol,
  with the `int3` fill stripped.
- **Retail's extent:** from the inventory.
- **The verdict:** both extents are the same length, and their bytes are
  equal outside the address fields.
- **The address fields:** our base relocations and our `/MAPINFO:FIXUPS`
  fields. Each must line up with the same kind of field in retail: an
  absolute address, or a relative branch that leaves the function.
- **Output:** a listing of the differences per function; `build/report.json`;
  the CSV's `status` updated; and a summary: functions and bytes matched, of
  game code and of everything in scope.
- `tools/match.py` stays for the spike's quick tests.

### 5. Ready queue: `tools/ready.py`

- **The call graph** comes from the inventory: direct calls and tail jumps.
- **When a game function is ready:** every function it calls is matched,
  library, or third party, or is in the same strongly connected component
  (mutual recursion is decompiled as a unit).
- **The output** is the ready functions, sorted by size (smallest first), and
  grouped by likely source file (atlas name, or neighbours in retail).

### 6. Workflow: `docs/DECOMPILING.md`

The procedure a worker (a person or a Sonnet subagent) follows for one
function:

1. Read the disassembly and any related source (the atlas name, the Halo CE
   decompilation, neighbouring matched functions).
2. Write the function in its `src/` file, with a `// @retail 0x...` comment.
3. Run `tools/check.py <address>` until it matches.
4. Stop after a set number of attempts and record it as `near` with the
   remaining difference.

Workers run in parallel, each in its own git worktree. A coordinator (me)
gives out the ready functions, reviews each match and merges it.

`tools/permute.py` comes later. It will rewrite a near function's source
mechanically (orders, types, temporaries) and test each variant.

## What changes from the spike

- **Exact extents and masks replace the heuristics** in today's `match.py`
  (the linear sweep, the mnemonic prefix, "the last 4 bytes"). These come
  from the simplify review's altitude findings.
- **Per-file flags move to `config/files.json`.**

## Not in scope now

- Matching data sections, and placing functions at their retail addresses.
- Third-party code (Havok, Bink): stubbed, not decompiled.
- The native port.

## Risks

- **Inlining can depend on things outside a function**, such as how many
  callers its callee has. The stand-ins approximate retail's callers.
  Functions that do not match despite correct source will show this; the
  answer then is to add those callers too.
- **The SDK libraries Bungie used were later releases** (QFEs) for DSOUND,
  XONLINES, XVOICE and XGRAPHICS. Their functions may not match by signature
  with the December 2003 release. The 5849.17 release is the next candidate.
- **The `/MAPINFO:FIXUPS` format is undocumented.** If it proves unreliable,
  use capstone's relative-branch group and `imm_offset` instead.
