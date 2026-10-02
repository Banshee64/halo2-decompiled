# Decompiling a function

This is the procedure for one function, written for a person or a subagent.

## Before you start

- **The SDK:** XDK 5849 at `sdk/xbox`, or set `XDK_DIR`.
- **The retail XBE:** `orig/default.xbe`. Extract it with
  `python tools/xiso_extract.py "<your Halo 2 image>" orig default.xbe`.
- **Python:** `pip install -r requirements-dev.txt`.
- **Pick a function:** take one from `python tools/ready.py`. Every function
  it calls is already matched, is library code, or is in the same recursion
  group, so the function's own code is the only unknown. The list shows each
  function's likely source file (`~` marks a guess from its neighbours);
  `python tools/ready.py --by-file` groups the functions by file.
- **In a git worktree,** `orig/` and `sdk/` are not there: set `RETAIL_XBE`
  to the retail XBE and `XDK_DIR` to the SDK's `xbox` folder.

## Steps

1. **Read the retail code** with `python tools/disasm.py <va>`.
   - `config/functions.csv` gives the function's size, its name (if the atlas
     knows it) and what it calls.
   - Note which arguments arrive in registers. LTCG gives internal functions
     custom conventions; write normal C++, and the compiler will choose the
     same registers.
2. **Find related source.** It may be:
   - the function of the same name in the Halo CE decompilation
     (punpckhdq/halo, CC0);
   - a neighbouring matched function;
   - the structures in `include/`.
3. **Write the function** in the `src/` file it belongs to: the `object`
   column of `config/functions.csv` (or the object field of `ready.py`'s output)
   names its likely source file (`crc.obj` → `src/crc.cpp`); with no object, use
   `src/unknown_<va>.cpp`.
   - Put `// @retail 0x<va>` on the line above it.
   - Write `static` functions as `PRIVATE`.
4. **Set the file's flags** with a `// @flags <flags>` line among the first 30
   lines of the source (`config/files.json` is only a fallback for files
   without one; the default is `/O2 /Gr`). `/GL` and `/Gr` are always added.
   Choose by the `style` column:
   - `speed`: `// @flags /O2 /Gr`
   - `size`: `// @flags /O1 /Gr`
   - `unknown`: try `/O2` first; if it does not match and the function is
     packed or ebp-framed, try `/O1`.
   If a callee gets inlined that retail calls, add `/Ob1` (`// @flags /O2 /Ob1 /Gr`).
5. **Callees.** Every call out of the function must reach the right function:
   - *Matched or in-progress game functions:* declare them (a header in
     `include/`) and call them; their `@retail` markers tie them to retail.
   - *Third-party code* (Havok, Bink, ...) and game functions not decompiled
     yet: write a stub in `src/stubs/<owner>.cpp` with a plausible signature,
     with `// @stub 0x<va>` (retail's address) above its definition. The body
     does not matter; the signature and calling convention do. Stubs are
     compiled without `/GL`, so they keep the standard convention retail uses
     for code outside the project. A stub is not compared with retail and gets
     no stand-in.
   - *Library functions* (CRT, XAPI, D3D, ...): nothing to write. Declare
     them and call them; the SDK libraries are linked. `config/functions.csv`
     names them where the atlas or a library signature knows them.
6. **Run** `python tools/check.py <va>`. Repeat until it prints `MATCH`.
   When it does not match, it prints the first difference, and
   `build/report.json` stores it too.
   Things that usually decide a match:
   - **types:** `short` against `long`, signed against unsigned;
   - **the order of terms** in floating-point expressions;
   - **whether a value goes through a local variable,** and whether a
     parameter's address is taken (that keeps it on the stack);
   - **the file's flags** (`/O1` against `/O2`, `/Ob1`).
7. **Stop after 20 tries,** or when only register choice or operand order
   differs. Leave the function with its marker. The checker records it as
   `near` or `todo` with the first difference, so someone can come back to
   it.

## What a match means

A match compares the bytes, and for each address field only its kind: whether
it is a call, a global or a jump table entry (jump table entries must map to
the same cases). Call targets are verified: a call to a function with an
`@retail` or `@stub` marker must reach that marker's retail address; a call to
a function retail reaches at an address some marker claims must reach that
marker's function; otherwise both sides' names (the map symbol, the `name`
column) must agree when both are known. Globals are still checked by eye.

## What not to do

- Do not add `__declspec(noinline)` or other attributes Bungie's code would
  not have had. Stand-ins keep functions out of line.
- Do not commit anything from `orig/`, `sdk/` or `build/`.
