# Euler angles to forward and up vectors

## Claim

Retail range: **`0x11df60`–`0x11dfff`**, two entries totaling 136 bytes.
The helpers construct a temporary rotation matrix and copy its forward and
up vectors. `0x11df60` takes three angles; `0x11dfb0` takes two and supplies
zero for the third angle.

The neighboring `0x11df30` and `0x11e000` routines remain in
`src/unknown_11cb00.cpp`. The existing matrix constructor at `0x141ce0`
remains unchanged. Issue #9 and open PRs were checked before claiming;
the range has no active overlap.

## Retail behavior

Both helpers allocate a 52-byte temporary matrix and call `0x141ce0`.
The input pointer arrives in EAX, with forward and up output pointers in
EDX and ECX. The three-angle helper reads input offsets 0, 4, and 8;
the two-angle helper reads offsets 0 and 4 and passes positive zero as the
third angle. Each copies twelve bytes from matrix offset 4 to the forward
output, then twelve bytes from matrix offset 28 to the up output.

The source file is named for this conversion. Functions retain neutral
address names. `function_11df60` preserves its existing upstream declaration
for compatibility with its object-placement and camera-scripting callers.

## Results

| Retail | Routine | Built/retail bytes | Result |
| --- | --- | ---: | --- |
| `0x11df60` | function_11df60, three angles | 69/69 | Exact match |
| `0x11dfb0` | function_11dfb0, two angles | 67/67 | Exact match |

Both functions are in `src/euler_vector_conversion.cpp`, built with
`/O2 /arch:SSE /Gr`. They use existing vector and matrix types and the exact
upstream declaration of `function_141ce0`. All angles are read before any
output is written. The forward vector is copied before the up vector,
including when the input and output buffers overlap.

The only shared-file change removes the replaced `0x11df60` stub from
`src/stubs/unknown_0b7300.cpp`. Existing caller declarations remain
unchanged. No new stubs, shared-header changes, other files' flag changes,
or inventory changes are included.

## Validation

Base: upstream `dc401df`. The full baseline check reproduced all 5,004
game/total matches. The full implementation check reports **5,006
game/total matches**, with every upstream match preserved. Both new
functions matched on the first implementation/build, without tuning or
forced attributes. A second full check after renaming the source file
reproduced those results, and all 852 behavioral comparisons passed again.
Draft PR #38 published the range claim before coding.

- All 13 original-compiler layout assertions pass: vector sizes and
  coordinates, matrix size, and matrix field offsets.
- 640 retail-versus-linked comparisons supply identical matrix payloads
  at the constructor boundary. These verify all three forwarded angle
  bit patterns, the two-angle helper's zero third angle, output ordering,
  and every fixture byte. Cases cover eight buffer layouts (including
  overlapping inputs/outputs), random bits, signed zero, infinities,
  quiet/signaling NaN payloads, and subnormal values.
- 212 comparisons run both actual matrix constructors with no dependency
  hooks. Forward/up results and all fixture bytes agree exactly for zero,
  signed zero, quarter/half turns, and seeded random finite angles.
- The reused matrix constructor remains `todo` in upstream. The 212
  integrated cases do not establish equivalence for every possible angle;
  the two wrapper bodies themselves match retail. No in-game tests.
