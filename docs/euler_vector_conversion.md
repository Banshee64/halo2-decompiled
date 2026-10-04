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

## Planned integration

Add `src/euler_vector_conversion.cpp` using existing vector and matrix
types and the existing matrix constructor. Replace only the `0x11df60`
stub in `src/stubs/object_placement.cpp`, preserving its declaration and
callers. Full match-preservation and retail comparison results will follow
source implementation.
