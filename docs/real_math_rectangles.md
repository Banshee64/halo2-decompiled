# Rectangle helpers from real_math.obj

## Claim

Retail ranges: **`0x11f770`–`0x11f99f` and `0x11fa40`–`0x11fc7f`**.
These are the two missing helpers in the rectangle cluster around the named
retail `rectangle3d_build_vertices` anchor. They total 1,126 retail bytes.
The vertex builder at `0x11f9a0` is already matched and stays in
`src/unknown_11cb00.cpp`. Surrounding math, including `0x11f5f0` and the
polygon clipper at `0x11fc80`, remains unchanged.

This claim covers the missing rectangle helpers, not the whole historical
`real_math.obj`, whose other routines are already distributed across source
files and active lane ranges. Issue #9 and open PRs were checked before
claiming; neither range overlaps an active claim.

## Mapping evidence

| Routine | 2003 profile | 2003 debug | Retail |
| --- | --- | --- | --- |
| real_rectangle3d_enclose_points | `0x280b0` | `0x42310` | `0x11f770` (inferred) |
| rectangle3d_build_vertices | `0x28280` | `0x3cca0` | `0x11f9a0` (named anchor) |
| rectangle3d_build_edges | `0x28320` | `0x3ce00` | `0x11fa40` (inferred) |

Both older maps associate these names and signatures with `real_math.obj`.
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

## Implementation plan

Add `src/real_math_rectangles.cpp` with the two missing routines. Reuse
`real_rectangle3d` from `include/unknown_11cb00.h`, existing point types,
and the exact upstream vertex-builder declaration. No new stubs, shared
header changes, or other files' flag changes are needed. Retain the old
capacity parameter in the edge-builder signature even though retail's
optimized function ignores it and always emits twelve edges.

Publish this claim as a draft PR before source. Validate the baseline and
implementation with full original-compiler checks, preserve every upstream
match, and leave the generated inventory out of commits.
