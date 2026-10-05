# Marker-data accessors

Retail range claimed: `0x11c7d0`–`0x11c83f`.

This contribution recovers two adjacent accessors, 89 retail bytes total:
`0x11c7d0` optionally copies a position and the following scalar from a
provided record; `0x11c800` obtains the record from the table at
`g_4e0350 + 0x11c`, with 0x20-byte stride, and performs the same copies.
Neither routine calls another function. The direct caller of `0x11c7d0`
uses the scalar in sine/cosine calculations.

The retail behavior establishes this bounded pair, not a complete original
source-file boundary. Banshee64 explicitly approved this bounded scope.
Existing callers, shared headers, neighboring implementations and lane A's
`0x11c5f0` are outside scope and remain unchanged.

Implementation and matching results will follow the published draft claim.
