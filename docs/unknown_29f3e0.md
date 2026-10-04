# Legacy recorded animation playback recovery

Retail range claimed: `0x29f180`–`0x29f47f`
(`unknown_29f3e0.obj`).

The range contains 13 inventory entries totaling 662 retail bytes. Eleven
already match in `src/unknown_29f180.cpp` at base `9cd127f`; their bodies,
flags, declarations, and callback table will be retained unchanged. This
work recovers the two missing stream routines.

## Range evidence

- Retail names `function_29f400` at `0x29f400`.
- The 2003 profile/debug maps group the legacy field readers and stream
  routines in `unknown_29f3e0.obj`.
- The codec pair at `0x46fd54` contains the initializer `0x29f3e0` and
  stream reader `0x29f400`. Its neighboring pair at `0x46fd4c` selects the
  newer format and belongs to PR #14.
- The legacy reader uses a four-byte event header (signed short type,
  unsigned short tick delay) and dispatches through `0x471220`.
- The previous module ends at `0x29f17f`. The next function, `0x29f480`,
  iterates AI data arrays and builds actor/object lists, so it is excluded.
- Issue #9 and all open PRs were checked before claiming; no overlap was found.

## Inventory

| Address | Function | Current result |
| --- | --- | --- |
| `0x29f180`–`0x29f370` | Eleven field-reader callbacks | Upstream matches retained |
| `0x29f3e0` | `function_29f3e0` | 24 / 24 bytes; dependency calling convention |
| `0x29f400` | `function_29f400` | 118 / 115 bytes; boolean return register width |

The initializer's name is inferred from older maps and the codec pair.
The shared reader `0x2c4e10` remains outside the implementation claim.
Its stub file and declaration are identical to PR #14's
`src/stubs/unknown_29ed40.cpp`, so the two PRs can merge without
duplicate definitions or conflicting declarations.

## Recovery results and validation

All 13 functions are implemented: **11 unchanged upstream matches and two
new functions with byte differences**.

- Full XDK 5849 `tools/check.py` against `079ec68`: **3,736 game matches /
  3,737 total**, with no upstream matches lost.
- Eight original-compiler assertions verify the four-byte event header,
  signed type and unsigned delay offsets, scalar widths, and codec layout.
- All 24 linked dispatch-table entries resolve to the corresponding retail
  functions or null slots. The source codec pair agrees with retail
  `0x46fd54`; it is unused until the owning recording code lands.
- Initialization delegates to the shared unit-control reader. It leaves
  the unused controller argument alone. The provisional dependency stub
  accounts for the different argument registers.
- Event application compares signed remaining ticks with the unsigned
  16-bit delay, indexes the table with a signed event type, invokes the
  selected callback or skips the four-byte header for a null slot, then
  subtracts the event delay. Future events and end events remain unconsumed.
  It returns false only when an end event's delay equals the remaining ticks;
  retail's true result for a larger tick count is preserved.
- The event loop matches through its control flow; only `xor eax,eax` /
  `mov eax,1` differ from retail's byte-register boolean returns. Matching
  stopped at this register difference, after one implementation/build.
- Existing `src/unknown_29f180.cpp` is unchanged. No shared-header, upstream
  flag, or inventory changes. No forced compiler attributes. No game runtime
  testing.

Changes are limited to this document, `src/unknown_29f3e0.cpp`,
and the shared dependency stub described above. The implementation pass is
complete; the PR remains a draft pending review.

## Sources

Name and object evidence comes from
[halo-symbol-atlas](https://github.com/tinkerer-red/halo-symbol-atlas)
(CC BY 4.0): retail SHA-256
`03215919bb7163259257d361f4c7bf802a7ab12aa85e2689436369b5c427935d`,
2003 profile map
`4f4f09b181eec4a434418b38efe581e75aaf3047c24add8a712751d6ae0d34d3`,
and 2003 debug map
`96ea21d862dfe6a0bebb23e1a4311202a6e18a79970189a4df320d4ededa439d`.
Boundaries remain provisional where retail symbols are absent.
