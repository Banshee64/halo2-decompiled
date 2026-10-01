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
4. **Set the file's flags** in `config/files.json` if it was compiled for
   size (the `style` column says `size`: use `/O1 /Gr`). For style `unknown`,
   start with `/O2`; if it does not match and the function is packed or
   ebp-framed, try `/O1`.
5. **Run** `python tools/check.py <va>`. Repeat until it prints `MATCH`.
   When it does not match, it prints the first difference, and
   `build/report.json` stores it too.
   Things that usually decide a match:
   - **types:** `short` against `long`, signed against unsigned;
   - **the order of terms** in floating-point expressions;
   - **whether a value goes through a local variable,** and whether a
     parameter's address is taken (that keeps it on the stack);
   - **the file's flags** (`/O1` against `/O2`, `/Ob1`).
6. **Stop after 20 tries,** or when only register choice or operand order
   differs. Leave the function with its marker. The checker records it as
   `near` or `todo` with the first difference, so someone can come back to
   it.

## What a match means

A match compares the bytes, and for each address field only its kind: whether
it is a call, a global or a jump table entry, not which function or global it
names (jump table entries must map to the same cases). Reviewers check call
targets and globals by eye, or against the `@retail` markers.

## What not to do

- Do not add `__declspec(noinline)` or other attributes Bungie's code would
  not have had. Stand-ins keep functions out of line.
- Do not commit anything from `orig/`, `sdk/` or `build/`.
