# Liquid draw callback (unknown_0508d0)

Retail range covered: `0x508d0`–`0x516c9`. This is one inventory entry, 3,578
retail bytes, `todo` with no source: the draw callback of the liquid effect. It
runs once for each queued liquid record. It fades the record by its distance
from `g_4b9da0` (the camera, inferred). Then, for each element of the liquid
definition, it draws up to eight arcs of noise-displaced points as strips
(camera-facing, inferred), cuts an arc short where it hits something, and
starts an effect and a sound at the hit. **Analysis only:** this document adds
no source, and nothing in it has been built or checked against retail with the
original compiler. Names are provisional. The `function_<va>` form is primary;
the descriptions are offered for whoever decompiles it.

It uses the names of `src/unknown_01cf50.cpp` (the queued record, its payload,
and the code that queues and runs it), `src/unknown_050690.cpp` (the noise
points, bases and draw state), `src/unknown_0c40f0.cpp` (liquids) and
`src/unknown_1689b0.cpp` (the collision result).
[Lights and liquids](unknown_0bffa0.md) describes the liquids themselves. Three
callees have no source (`__chkstk` is one) and are described only as far as
this function needs them.

## Boundary

- `0x507e0`, just before, and `0x516d0`, just after, are `todo` with source in
  `src/unknown_050690.cpp`. Both are excluded, and this function calls both.
- Lane D's row of the Active claims table (issue #9: `0x050000`–`0x06ffff`, now
  run on machine 2) covers the function. This document makes no claim and is
  offered to lane D.
- `0x508d0` has no `@retail` marker. A `@stub` and a declaration exist; see
  [Existing declarations](#existing-declarations).

## Conventions

"Callers" counts the functions that call an entry directly, from outside the
range and then from inside it. This entry has none: it is reached only through
a pointer, as described below.

| Retail | Bytes | Arguments | Returns | Callers | What it does |
| --- | --- | --- | --- | --- | --- |
| `0x508d0` | 3578 | stack: payload (`ret 4`) | void | 0 + 0 (by pointer) | Draws one queued liquid record |

There are no register inputs: `eax`, `ecx`, `edx`, the `xmm` registers and the
x87 stack are written before they are read. It saves `edi`, `esi`, `ebx` and
`ebp`. `__chkstk` reserves `0x19a4` bytes (`0x508d0`–`0x508d5`). Below, F is
`esp` just after that call, so the argument is at `F+0x19a8`. It returns
nothing that is used (`eax` is -1 on the first exit, `0x508e7`).

**How it is reached.** The only reference to `0x508d0` in the whole image is
the 32-bit immediate at `0x42ac6`, in `mov dword ptr [eax+0x20], 0x508d0`
(`0x42ac3`) in `function_429a0` (`src/unknown_01cf50.cpp:2608`). That function
takes the next 0x70-byte record of the pool at `0x4b6280` (the array at
`0x4b6288`, at most 0x400 records; a full pool drops the draw) and stores type
3 (`0x42ab6`), flags 0 (`0x42abc`), the callback at `+0x20`, a depth, the
position at `+0x64` and the payload at `+0x24`. The depth is the negative dot
product of `g_4b9dac` and the position less `g_4b9da0` (`0x42a90`–`0x42afd`).
`function_1e370` (`src/unknown_01cf50.cpp:2143`) sorts the records with
`0x1e700` and calls each callback with a pointer to its payload
(`0x1e3e3`–`0x1e3f1`). It is called from `0x2bcd0` (`0x2c19a`, no source) and
from `function_40870`. `0x2bcd0` first calls `0xc3e90` (`0x2c164`), which calls
`function_c3cf0` for each enabled liquid that has an object, skipping hidden
ones, so the records are queued before they are run (inferred from the
addresses).

**Who queues a record.** There are two callers of `function_429a0`:

- `function_c3cf0` (`src/unknown_0c40f0.cpp:260`; `0xc3e75`) queues a liquid's
  record, in pass 0 only. The payload type is 1 on the first-person marker path
  (`0xc3dda`) and 0 otherwise (`0xc3deb`). `a` is the liquid's object index,
  `b` its tag index, `c` its datum index, `position` the marker position,
  `endpoint` the liquid's point, and `first` and `second` the marker's forward
  and up. The opacity is the liquid's value for definitions whose short at `+2`
  is 0, else 1 − 3 × age, and only when positive (`0xc3dfd`–`0xc3e4e`).
- `0x41980` (`function_41980`, `src/unknown_0350e0.cpp:2536`; `0x41bb3`)
  handles the `tdtl` entries of an object definition's block at `+0x94` (count)
  and `+0x98` (0x18-byte entries). It queues type 0, `a` the object, `b` the
  entry's tag, `c` NONE, `endpoint` NULL, `first` and `second` the marker's
  forward and up, and opacity 1.0. A NULL endpoint becomes the position
  (`0x42a07`–`0x42a2e`).

So `c` is NONE for the second caller's records, and then the shared draw state
`g_4c9828` is used.

## Data

**The payload** is `s_429a0_payload` (`src/unknown_01cf50.cpp:2599`), 0x40
bytes at `+0x24` of the record. The function reads:

| Offset | Field | Use here |
| --- | --- | --- |
| `+0x00` | `type` (byte) | 0 or 1; passed to `function_516d0` as `reduced` (`0x51236`) |
| `+0x01` | `opacity` (byte) | The first factor of every point's alpha, over 255 (`0x512aa`–`0x512e5`) |
| `+0x04` | `a` | Not read |
| `+0x08` | `b` | The liquid definition's tag index; NONE returns at once |
| `+0x0c` | `c` | The liquid datum index, or NONE; picks the draw state and the object the ray ignores |
| `+0x10` | `position` | Where the fade distance is measured to, and the origin of every arc |
| `+0x1c` | `endpoint` | Where every arc ends, for definitions of kind 1 |
| `+0x28` | `first` | The arc direction: end = position + `first` × length. Also a frame axis, and the input of the second alpha function |
| `+0x34` | `second` | A frame axis, when the element's flag bit 0 is set |

**The liquid definition** is `s_liquid_definition_ab`
(`src/unknown_0c40f0.cpp:161`). It is the tag data of `b`, reached through
`g_4e3b44` (16-byte entries, data pointer at `+8`).

| Offset | Field | Use here |
| --- | --- | --- |
| `+0x02` | `kind` (short) | 1: every arc ends at the payload's endpoint |
| `+0x40` | real, unnamed | The near distance: the fade is 1.0 inside it |
| `+0x44` | real, unnamed | The far distance: the fade reaches 0 there and nothing is drawn beyond it |
| `+0x68` | `count` | The number of elements |
| `+0x6c` | `elements` | The array of 0xec-byte elements |

**An element** is `s_liquid_element_ab` (0xec bytes,
`src/unknown_0c40f0.cpp:156`). The same bytes are read as
`s_random_draw_definition` and `s_noise_draw_definition`
(`src/unknown_050690.cpp`). Names marked * are descriptive.

| Offset | Meaning | Use here |
| --- | --- | --- |
| `+0x00` | flags (byte) | Bit 0: the frame uses `second` as its left axis. Bit 2: a collision ray cuts the arc. Bit 3 clear: the strip's seventh argument is true |
| `+0x02` | exponent (short) | Not read here; `function_516d0` makes (4 << exponent) + 1 points |
| `+0x04` | length* (real) | End = position + `first` × length; also the cone radius factor and the divisor of the kind-1 ratio |
| `+0x08` | copies* (short) | How many arcs; 0 or less skips the element, more than 8 draws 8 |
| `+0x0c` | cone angle* (real, radians) | 0.0 turns the spread off |
| `+0x10` | period (real; the repository's name in `s_random_draw_definition`) | Nonzero adds `draw.reals[0]` × 2π to a copy's angle |
| `+0x20` | tag index | Handed to `function_1883e0`; NONE skips the hit effects and sounds |
| `+0x28` | tag index (a bitmap) | The strip's texture, in `ecx` of `0x4f7e0` |
| `+0xd4`, `+0xd8` | count, array | The sub-elements, 0x38 bytes each |
| `+0xdc`, `+0xe0` | tag function (size, address) | Value e, of the fraction: the amplitude for `function_516d0` |
| `+0xe4`, `+0xe8` | tag function | Value f, of the fraction: scales every alpha |

`function_516d0` also reads the element at `+0x34`, `+0x3c`, `+0x44`, `+0x48`
and `+0xd0` (`src/unknown_050690.cpp:92`–103), and `function_507e0` reads
`rates[9]` at `+0x90` (lines 40–48 and 70–72); this function does not.

**A sub-element** is 0x38 bytes, and the repository has no type for it. Each
function is a tag function, a size and an address.

| Offset | Meaning | Use here |
| --- | --- | --- |
| `+0x0c` | short | The strip's third argument |
| `+0x10`, `+0x14` | function E | Of t: a point's amount (record `+0x10`) |
| `+0x18`, `+0x1c` | colour function | Of t, then `function_13bc00` and `unpack_color3f`; absent gives (1, 1, 1) |
| `+0x20`, `+0x24` | function A | Of the sub-element's index plus `draw.reals[1]`: an alpha factor |
| `+0x28`, `+0x2c` | function B | Of \|`g_4b9dac` · `first`\|: an alpha factor |
| `+0x30`, `+0x34` | function D | Of t: a point's width (record `+0x0c`) |

**A tag function** is an `s_tag_data` (`src/unknown_13b390.cpp:13`). It is
present when its address is not NULL and its size is positive. Six of the seven
uses below are the same diamond: test that it is present, call
`function_13b390(function, input, 0.0)`, then, when the high nibble of the byte
at `data+1` is 0, pin the result to [0, 1] and map it to
`lower + (upper - lower) × value`, with `lower` at `data+4` and `upper` at
`data+8`; a nonzero high nibble keeps the raw result; a missing function gives
0.0. This is the shape of `noise_curve_value`
(`src/unknown_050690.cpp:130`–148); `curve_endpoint`
(`src/unknown_0350e0.cpp:129`) maps the same way but has no presence test. The
colour function is the exception: `function_13b390` is called without any test
(`0x51449`), its raw result is kept (`0x51453`) with no high-nibble test, pin
or mapping, and only then are the address and size tested
(`0x5144e`–`0x51461`).

**The draw state** is an `s_random_draw` (0x50 bytes,
`src/unknown_050690.cpp:11`). Its `reals[0]` (`+0x48`) is a phase and its
`reals[1]` (`+0x4c`) a time offset. With `c` set, `function_c4210` gives the
liquid element's own state: it returns the datum's address plus `0x20` plus the
element index × 0x50, or NULL when the pool is unset or invalid, and it is
stored unchecked (`0x50a5a`). With `c` NONE the shared `g_4c9828` is used.

**The points** are `s_noise_point` records (0x18 bytes,
`src/unknown_050690.cpp:120`): position (`+0x00`), width (`+0x0c`), amount
(`+0x10`) and a dword the repository calls `flags` (`+0x14`). `function_516d0`
writes the positions and sets the width and amount to 1.0 and the dword to 0.
Here the width and amount are replaced by functions D and E, and the dword
holds a packed ARGB colour (`0x514fc`–`0x51533`). The buffer at F+0x18c holds
exactly 257 records (4 × 64 + 1), so exponents above 6 would run past the
frame; neither function bounds it.

**The collision result** at F+0x130 is an `s_collision_result_1697c0` (0x5c
bytes, `src/unknown_1689b0.cpp:460`). This function reads `t` (`+0x04`), the
point (`+0x08`), the end location (`+0x1c`, eight bytes), the dword at `+0x24`
(its low word is preset to `0xffff` before the call, `0x50f12`, and the high
word is never set here; the low word is the hit material) and the plane
(`+0x28`).

**The frame.** F is `esp` after `__chkstk`. The locals, with the addresses of
their first uses:

| Offset | Holds |
| --- | --- |
| `+0x04`, `+0x14`, `+0x18`, `+0x1c`, `+0x30` | element; copy count; i; j; definition |
| `+0x08`, `+0x0c`, `+0x10`, `+0x20`, `+0x24` | fraction; the current alpha; the point count; the draw state; the fade |
| `+0x28`, `+0x2c`, `+0x34`, `+0x38`, `+0x3c`, `+0x40` | byte offset m × 0x38; t = k × F+0xe8; the record cursor; the cone angle; k; m |
| `+0x44` | a vector: from the position to `g_4b9da0`, later the ray |
| `+0x50`, `+0x84` | two 13-dword bases (`s_noise_basis`, `src/unknown_050690.cpp:105`) |
| `+0xac`–`+0xb4` | the end position (the second basis's origin) |
| `+0xb8`, `+0xbc` | value f; value e |
| `+0xc0` | an `s_sound_position` (`include/sound_sources.h:109`) |
| `+0xe8`, `+0xec`, `+0xf0` | 1 / (count − 1); function A's input; the colour function's result |
| `+0x100`–`+0x10c` | the colour (alpha, red, green, blue); `+0x110` is the green `fistp` temporary and `+0x114` holds 255.0 |
| `+0x118`–`+0x12c` | the six impact values, as three pairs |
| `+0x130`, `+0x18c` | the collision result; 257 point records |

The `fistp` temporaries for blue, red and alpha are at `+0xe4`, `+0xf4` and
`+0xfc`, and `+0xf8` holds the address of the colour.

## The function

The function has 118 basic blocks, one `ret` (`0x516c7`) and five loops. There
are no jump tables and no indirect jumps or calls. The only unreachable bytes
are the filler at `0x51009`; the other fillers (`0x50cfa`, `0x5129e`,
`0x5142d`) run as fall-through before loop heads. The loops, in code order,
are: the elements (`i`); inside each, the copies (`j`); and inside each copy,
first the three impact values and then the sub-elements (`m`), which contain
the points (`k`).

### Entry and the distance fade (`0x508d0`–`0x509d7`)

1. After `__chkstk`, `b` (`+8` of the payload) is the liquid definition's tag
   index. NONE returns at once, through `0x516c1` (`0x508e7`), with nothing
   pushed.
2. The definition is `g_4e3b44`'s data for `b` (`0x508ed`–`0x508fc`), kept in
   `edi` and at F+0x30. An element count (`+0x68`) of 0 or less returns
   (`0x50903`–`0x50909`).
3. The fade (F+0x24) starts at 1.0. The vector from the position to `g_4b9da0`
   goes in F+0x44 (`0x5091d`–`0x50957`), and `function_30bf0` normalizes it in
   place (`0x5095d`). The length it returns, 0 below 0.0001, is the distance.
4. If the far distance (`+0x44`) is not above the near one (`+0x40`), the fade
   stays 1.0. Otherwise r = (far − distance) / (far − near)
   (`0x5096d`–`0x5097b`). A negative r returns (`0x50985`). The fade is r, or
   1.0 when r is above 1.0 (`0x509af`), and a fade of 0 or less returns
   (`0x509d7`). None of these returns touches the render state.

### Setting up the draw (`0x509dd`–`0x50a0b`)

5. `g_46713c` and `g_4670bc` are set to 1 (`0x509e3`, `0x509ea`), and
   `function_16b10` resets the render state block `g_485b48` with `esi`
   (`0x509f1`), the repository's idiom (`src/unknown_01cf50.cpp:1842`–1843).
   `i` starts at zero. If the count is not positive, it jumps to the end block
   (`0x50a03`).

### Each element (`0x50a0d`–`0x50cd4`)

6. The element is `[definition+0x6c] + i × 0xec`. It is skipped when its
   sub-element count (`+0xd4`) or its copy count (short `+8`) is 0 or less
   (`0x50a2d`, `0x50a3a`).
7. With `c` not NONE, `function_c4210(c, i)` gives the draw state (`0x50a55`).
   With `c` NONE the state is `g_4c9828`, and
   `function_507e0(NULL, element, 1/30)` (`0x50a88`–`0x50a90`) advances it, so
   that shared state advances by 1/30 second for each element drawn from such a
   record. The copy count is the short at `+8`, at most 8.
8. The start frame is an `s_noise_basis` at F+0x50: scale 1.0 and origin = the
   position (`0x50a95`–`0x50ac0`). The up slot is `first`. With flag bit 0 set,
   the left slot is `second` and the forward slot is `first` × `second`, not
   normalized (`0x50acf`–`0x50b65`). With it clear, the left slot is
   `*g_4687b0`, which is (0, 0, 1), and the forward slot is `first` × left,
   normalized by `function_30bf0` (`0x50b70`–`0x50c13`); if the length is below
   0.0001, forward and left both become `*g_4687a4`, the zero vector
   (`0x50c25`–`0x50c50`).
9. The basis is copied to F+0x84 (13 dwords, `0x50c6e`–`0x50c7e`). Only its
   origin is meaningful: it is overwritten with the end position, position +
   `first` × length (`0x50c80`–`0x50cbd`). `function_516d0` reads only the
   second basis's origin (inferred from its source,
   `src/unknown_050690.cpp:263`). The copy index `j` is 0 (`0x50cc6`).

### Each copy (`0x50d00`–`0x5111d`)

10. The fraction (F+0x8) is 1.0 (`0x50d00`).
11. **The cone.** If the cone angle (`+0x0c`) is 0.0 the block is skipped
    (`0x50d21`), and then the end position is not recomputed for this copy: it
    keeps what the previous copy left in F+0xac–F+0xb4, which only the cone
    block, the kind-1 endpoint copy and a collision hit rewrite. Otherwise the
    angle is j × 2π / copies (`0x50d27`–`0x50d3f`), plus `draw.reals[0]` × 2π
    when the period (`+0x10`) is nonzero (`0x50d57`–`0x50d68`). With R =
    tan(cone angle) × length (`fptan`, `0x50d78`), the end is position +
    `first` × length + R × (cos × forward slot − sin × left slot) (`fcos`
    `0x50dc7`, `fsin` `0x50dd9`, `0x50e31`–`0x50e43`).
12. **Kind 1.** If the definition's `kind` (short `+2`) is 1 (`0x50e50`), the
    end is the payload's endpoint (`0x50e5b`–`0x50e83`). If the length is above
    0, the fraction is |end − position| / length (`0x50e90`–`0x50ed5`) and it
    goes to the clamp; otherwise it skips to the curves (`0x50e8a`).
13. **The ray.** Otherwise, if flag bit 2 is set (`0x50ede`), the ray vector is
    end − position (`0x50f1c`–`0x50f2e`) and the word at the result's `+0x24`
    is preset to `0xffff` (`0x50f12`). `function_c4250(c)` gives the liquid's
    object, which the ray ignores. `function_1697c0` runs with flags
    `0x680000d`, the start position, the vector, that object, NONE for the unit
    and the result at F+0x130 (`0x50f33`–`0x50f46`). A miss leaves the fraction
    at 1.0 and the end unchanged (`0x50f4d`).

14. **A hit.** The fraction becomes the result's `t` (`0x50f6e`) and the end
    the hit point (`0x50f59`–`0x50f8b`). If the element's tag index (`+0x20`)
    is NONE, it goes to the clamp (`0x50f92`). Otherwise `function_30bf0`
    normalizes the ray vector in place (`0x50f9c`), and a length of 0.0001 or
    less also goes to the clamp (`0x50fa8`).
15. **The impact values.** `function_1883e0` runs with the element's tag index
    in `ebx` and twelve stack arguments (`0x50fae`–`0x51000`; `ret 0x30`):
    `false`, the hit point, the word `g_47d8e0` (zero-extended), the dword at
    the result's `+0x24`, the index 15, the variant 0, and six out pointers.
    They fill three pairs: the first values at F+0x124, F+0x128 and F+0x12c,
    and the second values at F+0x118, F+0x11c and F+0x120.
16. For each of the three pairs (`edi` = 0, 4, 8; `0x51010`–`0x5111d`): a
    second value that is not NONE starts an effect:
    `function_1765e0(point, direction, normal, tag, 0, 0)` with the hit point,
    the normalized ray and the result's plane (`0x51033`); a first value that
    is not NONE plays a sound. For the sound, an `s_sound_position` at F+0xc0
    is zeroed and filled with the hit point, the compressed ray
    (`vector3d_compress`, `0x510b7`), the zero vector as velocity and the
    result's end location, and `function_1895f0(position, 1.0, tag)` plays it
    (`0x51112`).
17. **The clamp** (`0x51123`–`0x5114a`). The fraction is pinned to [0, 1]. The
    kind-1 path, the two shortcuts after a hit and the end of the impact loop
    all arrive here; every other path reaches the next step with the fraction
    still 1.0.

### The curves, the points and the strip (`0x51150`–`0x5166f`)

18. **Values e and f.** Value e is the element's function at `+0xdc` of the
    fraction (`0x51150`–`0x511b0`), stored at F+0xbc; value f is the one at
    `+0xe4` (`0x511b5`–`0x51216`), stored at F+0xb8 (`0x51245`).
19. **The arc.** `function_516d0` runs with `cl` = the payload's type and six
    stack arguments (`ret 0x18`): the seed j << 8, the element, the draw state,
    the pair of bases, value e as the amplitude, and the point buffer
    (`0x51236`–`0x5124e`). It returns the point count, (4 << exponent) + 1,
    with the exponent lowered by 2 (not below 0) when the type is set. A count
    of 1 or less goes to the next copy (`0x5125a`). F+0xe8 is 1.0 / (count − 1)
    (`0x51260`–`0x5127f`).
20. **The sub-elements.** For each sub-element m (`0x512a0`–`0x5166f`; an empty
    list goes to the next copy, `0x51290`): the alpha is the payload's opacity
    × value f × the fade × 1/255 (`0x512d2`–`0x512ed`). Function A of m +
    `draw.reals[1]` multiplies it (`0x512ca`–`0x5136a`), and so does function B
    of |`g_4b9dac` · `first`|, which is computed on the x87 stack
    (`0x51356`–`0x513ec`).
21. **The points.** For each point k, with t = k × the reciprocal in F+0xe8
    (`0x51430`–`0x51440`): the colour function gives s of t
    (`0x51436`–`0x51453`), raw and unmapped. If it is present,
    `function_13bc00(function, s)` gives a packed colour and `unpack_color3f`
    unpacks it (`0x5146d`, `0x5147b`); otherwise the colour is the white at
    `g_4686cc + 4` (`0x51482`–`0x514a1`). The four reals (alpha first) are
    multiplied by 255.0 and stored with `fistp`, rounded by the x87 control
    word and not clamped (`0x514bf`–`0x514f5`), then packed as alpha << 24 |
    red << 16 | green << 8 | blue into the record's dword at `+0x14`
    (`0x514fc`–`0x51533`). The record's width is function D of t
    (`0x51536`–`0x5159c`) and its amount is function E of t
    (`0x515a0`–`0x515fb`).
22. **The strip.** `0x4f7e0` draws the points (`0x51648`; `ret 0x20`) with
    `ecx` = the element's bitmap (`+0x28`, `0x5163c`) and eight stack
    arguments, last pushed first: the points, the count, the sub-element's
    short (sign-extended), 1.0, 0, 1, whether flag bit 3 is clear, and 0
    (`0x5162a`–`0x51647`).
23. The loops close: m advances by 1 and its byte offset by 0x38
    (`0x5164d`–`0x51669`), then j (`0x5166f`–`0x5167e`, back to `0x50cd6`,
    which reloads the end position from F+0xac–F+0xb4 and F+0x80), then i
    (`0x51684`–`0x51696`, back to `0x50a0d`, which reloads the definition).

### The end (`0x5169c`–`0x516c9`)

24. `ebp` and `ebx` are popped. If `g_46713c` is 0, which `function_4fe20` does
    inside the strip drawer, then `g_4b8474` is set to 0 and `0x3f5b20(0)` is
    called (`0x516a8`–`0x516b4`). That writes D3DRS_SAMPLEALPHA = 0 (inferred:
    the matched `function_15680` makes the same stores and call for that state,
    `src/unknown_020560.cpp:263`, and `g_4b8474` is the cache slot of state
    `0x9e`). Then `g_46713c` is set to 1 (`0x516b9`), `edi` popped, the frame
    released (`0x516c1`) and the function returns (`0x516c7`).

## Callees without source

Three callees have no source: `__chkstk` and the two below. `function_516d0`
and `function_1883e0` have source and are covered under
[Existing declarations](#existing-declarations).

- `0x320560` is the C library's `__chkstk`. With `eax` = the frame size (here
  `0x19a4`) it probes each page for sizes of `0x1000` and more, lowers `esp` by
  `eax` and returns normally (`0x320560`–`0x32059f`).
- `0x4f7e0` (1,592 bytes) draws the point list. It takes `ecx` (the bitmap tag
  index) and eight stack arguments (`ret 0x20`). `ecx` is compared with -1 and
  passed to `function_14560` (`0x4f831`–`0x4f841`;
  `src/unknown_01cf50.cpp:672`), with the short as that call's index. The last
  argument, a byte, selects `function_4fe20` (`0x4f800`, `0x4f825`): when it is
  0, `function_4fe20` sets the pixel shader and blend state and clears
  `g_46713c` if it was set (`src/unknown_01cf50.cpp:4118`–4143); when it is
  nonzero, the callee instead stores 0 in `g_4b8474` and calls `0x3f5b20(0)` if
  `g_46713c` is clear (the same D3DRS_SAMPLEALPHA reset as at the end of this
  function, inferred), and sets `g_46713c` to 1 (`0x4f802`–`0x4f81c`). That it
  draws a chain of camera-facing quads, one per segment between points, is
  inferred from its code (`0x4f7e0`–`0x4fe15`). Its fourth and fifth arguments
  (here 1.0 and 0) are a cosine and sine used when the sixth is 0
  (`0x4f9f4`–`0x4fa2c`), and the sixth (here 1) makes it take each segment's
  orientation from the projected direction (`0x4f86c`–`0x4f9ce`); the seventh
  goes to `0x501e0` in `dl` (`0x4f8c3`); the eighth selects `function_4fe20` or
  the `0x3f5b20` path. These roles are inferred, and the names are not known.
- `0x3f5b20` (25 bytes) is in the D3D library. It reads one stack argument,
  stores it in a D3D global and calls `0x3f63f0` (`ret 4`). It has no name in
  the repository. It is the setter that the render-state function at `0x222d0`
  reaches for state `0x9e`.

## Existing declarations

| Retail | Declared as | Where | What retail shows |
| --- | --- | --- | --- |
| `0x508d0` | `void __stdcall function_508d0(void *payload) {}` | `src/stubs/flexible_surface.cpp`; declared at `src/unknown_01cf50.cpp:2504` and used at `:2634` | The same: one pointer, `ret 4`, no result used |
| `0x516d0` | `long __stdcall function_516d0(bool reduced, dword seed, const s_noise_draw_definition *definition, const s_random_draw *draw, const s_noise_basis *basis, real amplitude, s_noise_point *points)` | `src/unknown_050690.cpp:150` | One register and six stack arguments (`ret 0x18`): `reduced` is in `cl`. The count agrees; the declared form would put it on the stack |
| `0x1883e0` | `void function_1883e0(long tag_index, bool ignore_distance, point3f const *point, short element_index, long unused, long index, long variant, long *first_value04, long *second_value04, long *first_value, long *second_value, long *first_value0c, long *second_value0c)` | `src/unknown_187ec0.cpp:380` | The tag index in `ebx` and twelve stack arguments (`ret 0x30`). No parameter is missing, but the roles of slots 3 and 4 differ from the source |

Three notes on that source:

- Retail `0x1883e0` uses its fourth stack slot, which the source calls
  `unused`, as the index of the globals element:
  `[g_4e034c+0x154] + index × 0xb4`, bounded by the count at `+0x150`
  (`0x1883e5`–`0x18841e`), and as the last argument of both `0x188130` calls
  (`0x188478`, `0x188488`). Its third slot, `element_index`, reaches only
  `0x1882d0` (`0x18849f`–`0x1884b3`). The source's body uses `element_index`
  for the globals element (`src/unknown_187ec0.cpp:384`). The call here passes
  `g_47d8e0`, which is NONE, in the third slot and the dword at the result's
  `+0x24`, whose low word is the hit material, in the fourth;
  `src/impacts.cpp:1325` passes `material_a` and `material_b` there.
- `s_noise_point`'s `flags` (`src/unknown_050690.cpp:125`) holds a packed
  colour in this function, while `function_516d0` writes 0 there.
- The slots of `s_noise_basis` are named forward, left and up, but here the
  forward slot gets `first` × `second`, or the normalized `first` × (0, 0, 1),
  the left slot gets `second` or (0, 0, 1), and the up slot gets `first`.
  `function_516d0` uses only the forward and left slots, as displacement axes.

Nothing else is missing a parameter. `function_30bf0`, `function_16b10`,
`function_c4210`, `function_507e0`, `function_c4250`, `function_1697c0`,
`function_1765e0`, `function_1895f0`, `function_13b390`, `function_13bc00` and
`unpack_color3f` all agree with retail in their parameter counts.

## Evidence

- The function was disassembled from the retail XBE with capstone 5.0.9,
  through `tools/xbe.py`. A control-flow graph with a stack-depth walk over
  every path (each callee's `ret N` included) put every stack operand at an
  offset from F, and the depth is consistent at every join. Registers, offsets,
  flags and constants were read from retail code and data, and the callees with
  source were read from it.
- A reader analysed the function and a verifier checked each claim with its own
  disassembly: 23 confirmed and 5 corrected, with the corrections applied here.
  A third agent then checked the finished document against retail and the
  repository: 350 claims confirmed and 15 corrected or reworded, with the
  changes applied here.
- The callers come from an image-wide scan of every section for calls, jumps
  (near and short) and absolute dwords at any alignment. It finds one
  reference, the immediate at `0x42ac6`; the function is in no table.
- Constants are read from retail `.rdata`: `0x45dbc0` (1.0), `0x45dbdc`
  (0.0001), `0x43f17c` (6.2831855), `0x45dbb4` (0.003921569) and `0x45dc38`
  (255.0); the vectors (0, 0, 0) at `0x440af4`, (0, 0, 1) at `0x440b48` and
  white at `0x4409e4`, reached through the pointers `g_4687a4`, `g_4687b0` and
  `g_4686cc`. The immediates are 1/30 (`0x3d088889`), the collision flags
  `0x680000d`, the index 15 and the word `0xffff`.
- No document covered the function before this one. Source line numbers are at
  `ffa2bd2`.
- No emulator, runtime testing, SDK or outside dataset was used. Names are the
  repository's own, or describe behaviour.
