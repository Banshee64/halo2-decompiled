# String helper recovery (unknown_11c920)

Retail range claimed: `0x11c920`–`0x11ca1f` (string helpers).

## The cluster

The four retail entries in this cluster total 252 bytes:

| Address | Routine | Base status |
| --- | --- | --- |
| `0x11c920` | `function_11c920` | Missing |
| `0x11c9a0` | `function_11c9a0` | Matched in `src/unknown_11c9a0.cpp` |
| `0x11c9c0` | `function_11c9c0` (upstream name) | Matched in `src/unknown_11c9c0.cpp` |
| `0x11c9e0` | `function_11c9e0` (append formatting) | Matched in the same file |

The three existing routines, their declarations, and their flags remain
unchanged. This claim covers the string-helper cluster only. The preceding
function at `0x11c840` iterates and preprocesses tags; the following `0x11ca20` begins
memory wrappers. Both are excluded. Issue #9 and open PR descriptions were
checked: no overlap.

## Comparison behavior

The comparator reads signed narrow characters, converts each to the SDK's
16-bit `wchar_t`, and calls `towlower` at retail `0x321846`. It compares the
zero-extended results as integers and returns only -1, 0, or 1. It stops at
an unequal pair or either terminator. It uses the wide-character CRT helper,
not the narrow `tolower` or `_stricmp` functions; preserving that distinction
also preserves how bytes with the high bit set are passed to the CRT.

## Implementation and validation

`function_11c920` is implemented in `src/unknown_11c920.cpp`. Its 135 code bytes differ
from retail's 128, starting at `+4`: the compiler exchanges pointer registers,
advances both pointers independently instead of keeping a pointer difference,
omits retail's loop-alignment instruction, and emits different loop exits
and return blocks (including a duplicated zero-result path). The comparison
and termination behavior is retained. The checker labels this entry `todo`;
it is not an exact match.

One source implementation and full check were used. No forced attributes,
artificial callers, pointer-difference tricks, or compiler-flag tuning were
introduced to steer the remaining instruction differences.

Validation against upstream `37bce25`:

- Full original-compiler `tools/check.py`: **4,331 game / 4,331 total
  matches**, no upstream match lost.
- The three existing string helpers still match; both existing source files
  are byte-for-byte unchanged from upstream.
- Four SDK compile-time assertions confirm 32-bit `int`, 16-bit `wchar_t`,
  byte-sized `char`, and signed plain-character behavior.
- Linked code has four calls to the SDK `towlower`, four signed byte loads,
  and four zero extensions of the 16-bit results. Return paths were inspected
  for the -1/0/1 outcomes and terminator handling.
- No game runtime tests were run.

The draft claim was published before source. Only `src/unknown_11c920.cpp` and
this document change. The dependency uses the SDK header/library; no stubs,
shared-header edits, other files' flags, or inventory changes are included.

## Sources

Implementation follows retail disassembly. The CC0 Halo CE unknown_11c920
reference was inspected, but it does not supply this comparator's body.
Game and SDK files remain outside the contribution.
