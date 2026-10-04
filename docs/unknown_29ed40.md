# Recorded animation playback recovery

Retail range claimed: `0x29ec30`–`0x29f17f`.

The inferred boundary contains 13 inventory entries totaling 1,265
retail bytes. Seven entries already match in `src/unknown_29ec30.cpp` at
base `80435cf`. Those implementations and their declarations are preserved;
this work recovers the six missing entries.

## Range evidence

- Retail's byte and short delta callbacks call the corresponding controller
  update and vector decompressor. They have the same vector-copy cases and
  advance the stream cursor by two and four bytes respectively.
- The stream reader at `0x29f0c0` dispatches through table `0x4710f0`.
  Its final jump table ends at `0x29f17f`. The following callbacks at
  `0x29f180` use the legacy event layout and belong to the adjacent
  legacy reader, excluded from this claim.
- The preceding `0x29e730` routine concerns spatial/path data and is excluded.
- Issue #9 and open PRs were checked before claiming. No other active
  claim overlapped this range. Existing upstream functions are retained without rework.

## Inventory

| Address | Interpretation | Current result |
| --- | --- | --- |
| `0x29ec30`–`0x29ecd0` | Six field-reader callbacks | Six upstream matches retained |
| `0x29ed00` | `function_29ed00` | Upstream match retained |
| `0x29ed40` | `function_29ed40` | New exact match, 61 bytes |
| `0x29ed80` | `function_29ed80` | New exact match, 62 bytes |
| `0x29edc0` | `function_29edc0` | 350 / 348 bytes; mask/register/stack differences |
| `0x29ef20` | `function_29ef20` | 350 / 348 bytes; mask/register/stack differences |
| `0x29f080` | `function_29f080` | 51 / 51 bytes; dependency calling convention |
| `0x29f0c0` | `function_29f0c0` | 200 / 188 bytes; switch/register/stack/return differences |

Names are inferred from retail behavior. The legacy version and the shared unit-control
reader at `0x2c4e10` remain outside the implementation claim.

## Validation

All six missing functions are now implemented: two exact matches and four
with byte differences. Including the seven unchanged upstream functions,
**all 13 entries are implemented and 9 match**.

- Full XDK 5849 `tools/check.py` against `f5106e5`: **3,509 game matches /
  3,510 total**, two above baseline, with no upstream or previous local matches lost.
- Twenty-one original-compiler assertions verify the short-delta, controller,
  event header, codec callback pair, and full 0x7c-byte unit-control layout. Both signed controller
  components occupy two bytes.
- The yaw update narrows the sum to a short, then subtracts 1,000 above
  1,000 or adds 1,000 below -1,000. Pitch is updated without wrapping.
- Decompression uses the retail float scale at `0x45dfa4` (π/1,000),
  converting yaw/pitch into `(cos(yaw) cos(pitch), sin(yaw) cos(pitch),
  sin(pitch))`. The built scale bytes match retail.
- The two difference callbacks select facing, aiming, and looking from the
  event mask. They apply the delta to the first selected direction and copy
  its controller/vector to subsequent selections, preserving retail's
  facing-before-aiming precedence. The cursor advances by two or four bytes
  even for an empty mask.
- All 24 entries in the linked dispatch table agree with retail `0x4710f0`,
  including null slots and repeated callbacks. Existing field-reader
  declarations are unchanged. Retail's current-format codec pair at
  `0x46fd4c` holds the initializer and stream-reader pointers; the source
  includes this pair, with the neighboring legacy pair excluded.
- Both callbacks use retail's four stack arguments. Remaining differences
  are mask zero-extension/test instructions, register allocation, stack slots,
  and instruction scheduling. Two versions were checked; no forced compiler
  attributes or upstream flag changes were used.
- Initialization calls the versioned unit-control reader, copies the three
  direction controllers from the stream, then advances the cursor 12 bytes.
  Its remaining differences follow the external dependency's provisional
  calling convention.
- Event application decodes delays of zero, one, an unsigned byte, or an
  unsigned short from the header's low two bits. It consumes due events and
  subtracts their delay after the optional callback. Future events leave
  the cursor untouched. End events return false only when their delay equals
  the remaining ticks, including retail's true result when ticks exceed it.
- The stream reader retains a redundant switch bounds check, different
  registers/stack slots, and full-register boolean returns. One implementation
  was checked for each stream routine; no forced compiler attributes were used.
- Changes are limited to this document, `src/unknown_29ed40.cpp`,
  and `src/stubs/unknown_29ed40.cpp`. The only new dependency
  stub is `0x2c4e10`, `function_2c4e10`; its
  implementation remains outside the claim. No shared-header edits,
  upstream-body/flag changes, or committed inventory changes. No game runtime
  testing.

The implementation pass is complete. Four functions retain the byte differences
listed above; the PR remains a draft pending review.

## Sources

Boundaries and names are inferred from retail code and remain provisional.
