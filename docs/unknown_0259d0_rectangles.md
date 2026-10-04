# Rectangle helpers from unknown_0259d0.obj

## Claim

Retail ranges: **`0x11f770`–`0x11f99f` and `0x11fa40`–`0x11fc7f`**.
These are the two missing helpers in the rectangle cluster around the named
retail `function_11f9a0` anchor. They total 1,126 retail bytes.
The vertex builder at `0x11f9a0` is already matched and stays in
`src/unknown_11cb00.cpp`. Surrounding math, including `0x11f5f0` and the
polygon clipper at `0x11fc80`, remains unchanged.

This claim covers the missing rectangle helpers, not the whole historical
`unknown_0259d0.obj`, whose other routines are already distributed across source
files and active lane ranges. Issue #9 and open PRs were checked before
claiming; neither range overlaps an active claim.

## Mapping evidence

| Routine | 2003 profile | 2003 debug | Retail |
| --- | --- | --- | --- |
| function_11f770 | `0x280b0` | `0x42310` | `0x11f770` (inferred) |
| function_11f9a0 | `0x28280` | `0x3cca0` | `0x11f9a0` (named anchor) |
| function_11fa40 | `0x28320` | `0x3ce00` | `0x11fa40` (inferred) |

Both older maps associate these names and signatures with `unknown_0259d0.obj`.
Retail `0x11f770` walks 12-byte points, updating six alternating minimum and
maximum bounds in a 24-byte rectangle. It preserves the incoming bounds and
returns the rectangle pointer. Retail `0x11fa40` calls the named vertex
builder, then copies twelve endpoint pairs from eight vertices using the
box's edge-index table. It returns twelve. Those operations identify the
helpers independently of their relative addresses.

Names and map associations come from
[halo-symbol-atlas](https://github.com/tinkerer-red/halo-symbol-atlas),
licensed CC BY 4.0. Map hashes:

- Retail: `03215919bb7163259257d361f4c7bf802a7ab12aa85e2689436369b5c427935d`
- Profile: `4f4f09b181eec4a434418b38efe581e75aaf3047c24add8a712751d6ae0d34d3`
- Debug: `96ea21d862dfe6a0bebb23e1a4311202a6e18a79970189a4df320d4ededa439d`

## Results

| Retail | Routine | Built/retail bytes | Result |
| --- | --- | ---: | --- |
| `0x11f770` | function_11f770 | 557/557 | Differs (`todo`), first difference +0x16 |
| `0x11fa40` | function_11fa40 | 569/569 | Exact match |

Both routines are in `src/unknown_0259d0_rectangles.cpp`, built with
`/O2 /arch:SSE /Gr`. The enclosure helper preserves the incoming bounds and
updates each bound with an ordered comparison. Empty and negative point
counts leave the rectangle untouched. The compiler unrolls the loop by four
as retail does, but chooses a point-array base offset of +8 where retail uses
+0x14; the associated load displacements compensate. Register conventions,
instruction count, and size agree. Recovery stopped at these operand
differences after the first implementation, as the decompiling guide directs.

The edge builder's local twelve-pair index table, temporary eight vertices,
loop unrolling, stores, and call to the existing vertex builder reproduce
retail's bytes. Its historical capacity parameter is retained in source;
LTCG removes it, as in retail, leaving two stack arguments and eight-byte
cleanup. The function always emits twelve edges.

Existing types come from `include/unknown_11cb00.h` and `unknown_0259d0.h`.
The vertex-builder declaration matches upstream exactly. No new stubs,
shared-header changes, other files' flag changes, or inventory changes are
included. The matched vertex builder and surrounding source are unchanged.

## Validation

Base: upstream `6020dbd`. The full baseline check had 4,917 game/total matches.
The full implementation check has **4,918 game/total matches**, with every
upstream match preserved, including the reused vertex builder at `0x11f9a0`.
The draft range claim was published before the source implementation.

- All 12 original-compiler layout assertions pass: rectangle, point, and
  edge sizes and their coordinate offsets.
- 1,485 isolated retail-versus-linked enclosure comparisons agree on the
  return pointer and every fixture byte. Cases cover negative/empty counts,
  unrolled-loop boundaries, reversed starting bounds, seeded random points,
  signed zero, infinities, quiet/signaling NaN payloads, subnormal values,
  and overlapping input/output memory.
- 240 edge comparisons agree with retail and an independently constructed
  list of expected vertex pairs. Tests compare the whole memory fixture,
  including untouched guard bytes, bit-preserving coordinate copies, and
  rectangle/output aliasing. The actual matched vertex-builder body runs.
- These x86/SSE emulation tests use no dependency hooks. No in-game tests.
- One implementation/full build; no compiler-flag search or forced attributes.
