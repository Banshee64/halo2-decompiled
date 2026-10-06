# Object lighting (unknown_0d37f0)

Retail range covered: `0xd37f0`–`0xd4823`. These are three inventory entries,
4,143 retail bytes, all `todo` with no source: the functions that turn a
surface colour query into an object's lighting record (three colours, three
directions and two reals), and the lightmap colour under a collision result.
**Analysis only:** this document adds no source, and nothing in it has been
built or checked against retail with the original compiler. Names are
provisional. The `function_<va>` form is primary; the descriptions are offered
for whoever decompiles the range.

It uses the names of `src/unknown_175bd0.cpp` (the query),
`src/flexible_surface.cpp` (`function_117c80`, which limits a direction),
`src/unknown_13b390.cpp` (the tag functions that `function_13b390` evaluates)
and `src/unknown_033a0b.cpp` (`s_33a0b_view`, through which the renderer reads
the record). [Surface colour queries](unknown_0d15e0.md) describes the queries
and `0xd16d0`, and
[the object globals](unknown_0b8bd0.md#the-record-at-object-globals-0x1c) the
same record kept for objects lit by script.

## Boundary

- `0xd3630`, just before the range, is matched, with source in
  `src/unknown_0d0690.cpp`. `0xd4830`, just after it, is matched, with source
  in `src/unknown_0d4830.cpp`. Both are excluded.
- No row of the Active claims table (issue #9) covers the range, so it is open.
  This document makes no claim. The callers are in lane W's range (`0x3db00`
  and `0x3dfe0` call `0xd4080`) and lane F's, on machine 2 (`0x180d80` calls
  `0xd47d0`).
- No entry has an `@retail` marker; see
  [Existing declarations](#existing-declarations).

## Conventions

"Callers" counts the functions that call an entry directly, from outside the
range and then from inside it. No entry is reached through a pointer or a
table.

| Retail | Bytes | Arguments | Returns | Callers | What it does |
| --- | --- | --- | --- | --- | --- |
| `0xd37f0` | 2189 | `eax` tag data, `ecx` object type, `edx` query; stack: record out, colour A, colour B (`ret 0xc`) | bool | 0 + 1 | Fills the record from the tag block entry that matches the object type |
| `0xd4080` | 1870 | `eax` query, `ecx` object type, `esi` record out; stack: one argument it never reads (`ret 4`) | void | 2 + 0 | An object's lighting record from a surface colour query |
| `0xd47d0` | 84 | `eax` colour out; stack: collision result (`ret 4`) | bool, always false | 1 + 0 | The lightmap colour under a collision result |

`0xd4080` keeps `esi` unchanged, and `0x3db00` returns it. Neither `0xd37f0`
nor `0xd4080` writes the query.

## Data

**The query** is `s_effect_color_query` (`src/unknown_175bd0.cpp:149`): a
vector at `+0` and the packed colours color_a at `+0xc` and color_b at `+0x10`.
[Surface colour queries](unknown_0d15e0.md#data) gives what each source puts
there; for the structure they are the normal, the base map colour and the
lightmap colour. `0xd4080` unpacks color_a into A and color_b into B with
`unpack_color3f` (`0x131bb0`), so both lie in [0, 1]. Below, L is the length of
the vector and n the vector normalized, or left as it is when its length is
below 0.0001 (`0x45dbdc`).

**The record** is 0x54 bytes. These functions write its first 0x50; `0x3db00`
copies all 0x15 dwords (`0x3dbcc`). The renderer reads it through
`s_33a0b_view` (`src/unknown_033a0b.cpp`), which starts 0x10 bytes before it:
`function_116b00` points the view at `+0x44` of a lighting datum whose record
is at `+0x54` (`src/flexible_surface.cpp:466`).

| Offset | Contents | Read as |
| --- | --- | --- |
| `+0x00` | colour 0 | `vector10`; `function_336f0` case 23 adds it to colour 1 (`src/unknown_0350e0.cpp:939`) |
| `+0x0c` | direction 0 | |
| `+0x18` | L | |
| `+0x1c` | a real: 1.0, or a tag function's value | |
| `+0x20` | colour 1 | `vector30` |
| `+0x2c` | direction 1 | `vector3c` |
| `+0x38` | colour 2 | `vector48` |
| `+0x44` | direction 2 | `vector54` |
| `+0x50` | not written here | script 96 sets NONE in its low 16 bits |

Scripts 96 and 97 fill the same layout in the object globals: script 96 colour
1 with direction 1 (also written as direction 0) and 0.8 in both reals, script
97 colour 2 with direction 2.

**The tag** that `0xd37f0` reads is the scenario's (`g_4e0350`) at `+0x33c`, or
the globals' (`g_4e034c`) at `+0x184` when that is NONE (`0xd417b`–`0xd4192`).
The fallback is not checked for NONE. Its data comes from the tag instances
(`g_4e3b44`, `+8`), and without data `0xd4080` uses its defaults. Neither
offset is declared in the source, and the tag group is not identified. The data
starts with a block (count `+0`, address `+4`) of 0x90-byte entries:

| Offset | Contents |
| --- | --- |
| `+0x00` | flags: an entry applies when bit 0 or bit (type + 1) is set |
| `+0x04` | a real added to each channel of B, giving c |
| `+0x08`, `+0x14` | lower and upper bounds of c for colour 1 |
| `+0x20` | an angle in degrees: `function_117c80`'s limit for directions 1 and 2 |
| `+0x24` | a tag function of L that scales colour 1 |
| `+0x2c`, `+0x38` | bounds of c for colour 2 |
| `+0x44`, `+0x50` | bounds of A for colour 2 |
| `+0x5c` | an angle in degrees: direction 2's turn about +z |
| `+0x60` | a tag function of L that scales colour 2 |
| `+0x68`, `+0x74` | bounds of c for colour 0 |
| `+0x80` | a tag function of c's luminance that scales colour 0 |
| `+0x88` | a tag function of B's luminance, giving the real at `+0x1c` |

Bounds are three reals (r, g, b). A tag function is an `s_tag_data`, evaluated
as `function_13b390(&function, input, 1.0)`; the result is then mapped as
`function_13bb40` does (`src/unknown_13b390.cpp`): when the header's flags have
no bits in `0xf0`, it is pinned to [0, 1] and scaled to the header's bounds.
The mapping is inline after each call. It gives `function_13bb40`'s result in
the same order of operations, with different registers and branches, and unlike
`function_13bb90` it does not check that the function has data.

The object type is the short at `+0` of the object's definition, or NONE.
`0xd4080` treats 0, 1 and 2 (biped, vehicle and weapon, by the group tags of
`g_468630`'s type definitions) as one class.

## The functions

### `0xd4080`: the record from a query

1. It takes L on the x87 stack, summing the squares as x, y, z
   (`0xd4088`–`0xd40b7`), and sets a flag for types 0 to 2
   (`0xd409c`–`0xd40c7`). It normalizes a copy of the vector inline, as
   `function_30bf0` (`src/unknown_030290.cpp:281`) does but summing the squares
   as x, z, y (`0xd40da`–`0xd4107`), and writes that length, or 0 below 0.0001,
   to `+0x18` (`0xd415e`). This happens on every path.
2. It unpacks A and B, and finds the tag. With data it calls
   `0xd37f0(eax = data, ecx = type, edx = query; record, &A, &B)` (`0xd41b7`).
   When that returns true it goes straight to the pins of step 7.
3. The defaults. Colour 1 is B (`0xd41e0`). Direction 1 is
   −`function_117c80`(n, 15.0) (`0xd41f6`, negated at `0xd4209`–`0xd423f`); see
   below for the 15.0.
4. A′ is A pinned to [0.25, 0.75], compared on the x87 stack against the
   doubles at `0x45e2c0` and `0x45e1d0`. X is (B × A′) × 2.0 pinned to [0, 1],
   and each channel of X is then set to 0 unless it is greater than 0, which
   changes only a NaN (`0xd4368`–`0xd4392`). Colour 2 is (1 − L²) × B + L² × X
   (`0xd4397`–`0xd43d9`).
5. Direction 2 is −direction 1 turned 5.0 degrees about (0, 0, 1)
   (`0xd43de`–`0xd4515`). The 5.0 is the real at `0x467494`, converted with
   π/180 (`0x45dc54`), and the axis is `g_4687b0`'s.
6. Colour 0 is B, and direction 0 is (−n.x, −n.y, −2 n.z) normalized by
   `function_30bf0` (`0xd455f`), whose result is discarded. With k = 1.18
   (`0x45e2bc`) with the flag or 0.75 (`0x45dc2c`) without, and lum = 0.114 b +
   0.587 g + 0.299 r of B, summed in that order, colour 0 is multiplied by (1 −
   (0.1 L + 0.6)) × ((k − 0.7) × lum + 0.7). Colour 1 is multiplied by 0.05 L +
   0.4 and colour 2 by 0.4 − 0.05 L, and `+0x1c` is 1.0 (`0xd4682`).
7. On both paths each channel of colour 1, colour 2 and then colour 0 is pinned
   to [0, 1] (`0xd4689`–`0xd47cb`). The directions and the two reals are not.

The stack argument is never read. Both callers pass the same bit as `0xd1850`'s
choice of five rays: the object's flags bit 13 or its definition's byte `+2`
bit 1 (`0x3dc49`–`0x3dc73`, `0x3e2ea`–`0x3e306`).

**The 15.0** is the real at `0x467484`, which `0xd4080` pushes as it is
(`0xd41e8`). `function_117c80` takes radians (`cos(angle)` at
`src/flexible_surface.cpp:325`, `fcos` at `0x117c87`), so the limit is cos 15 =
−0.7597. Every |z| exceeds that, so direction 1 always has |z| = 0.7597, with
the sign of n.z (positive for 0), and a horizontal part of length 0.650
opposite n's ((−0.650, 0) when n's is nearly zero). The 5.0 beside it is
converted, and so is `0xd37f0`'s own angle, so 15.0 was probably meant in
degrees (inferred). Both reals are script globals (numbers 355 and 359 of
`g_473468`'s table, `src/unknown_209520.cpp:96`), so they can change at run
time. A matching decompilation passes the 15.0 unconverted.

### `0xd37f0`: the record from the tag

1. It returns false, writing nothing, when the block is empty or no entry
   applies (`0xd3807`, `0xd3830`). Otherwise it uses the last entry that
   applies: the loop at `0xd3818`–`0xd382c` has no break. With the type NONE
   the mask is 1, so only bit 0 counts.
2. It writes L, taken on the x87 stack (`0xd3836`–`0xd3881`), to `+0x18`
   (`0xd388d`), replacing `0xd4080`'s value, which is 0 below 0.0001. n is
   normalized inline as `function_30bf0` does, but summing the squares as z, y,
   x (`0xd3890`–`0xd38f7`), and that length is unused. c is B plus entry
   `+0x04` on each channel.
3. Colour 1 is c pinned to entry `+0x08`/`+0x14` (`0xd390f`–`0xd39b9`).
4. Direction 1 is −`function_117c80`(n, entry `+0x20` × π/180) (`0xd39d6`,
   negated at `0xd39db`–`0xd3a04`).
5. Colour 2 is ((a′ × b′) × L²) × 2.0 + colour 1 × (1 − L²) on each channel,
   where b′ is c pinned to entry `+0x2c`/`+0x38` and a′ is A pinned to entry
   `+0x44`/`+0x50` (`0xd3a09`–`0xd3b5f`).
6. Direction 2 is −direction 1 turned by entry `+0x5c` degrees about (0, 0, 1)
   (`0xd3b64`–`0xd3c9a`), with `fsin` and `fcos`. The products are in the order
   of `function_117dd0`'s rotation (`src/flexible_surface.cpp:371`–`374`).
7. Colour 0 is c pinned to entry `+0x68`/`+0x74` (`0xd3c9f`–`0xd3d03`).
8. Direction 0 is −n (`0xd3d08`–`0xd3d30`). When its z is above −0.75
   (`0x45e23c`), z is pinned to [−1.0, −0.75], which can only give −0.75,
   though retail still compares it with −1.0 (`0xd3d46`). Only in that case is
   the vector then normalized again inline (`0xd3d6b`–`0xd3dcf`), so for n =
   (1, 0, 0) direction 0 is (−0.8, 0, −0.6).
9. The tag functions follow, in this order. `+0x1c` is F88 of 2.0 × (0.11 b +
   0.59 g + 0.3 r) of B, pinned to [0, 1] (`0xd3dde`–`0xd3e92`). Then F24(L)
   and F60(L) are taken, and colour 1 is multiplied by the first and colour 2
   by the second (`0xd3e85`–`0xd3f94`). Last, colour 0 is multiplied by
   (F80(0.114 b + 0.587 g + 0.299 r of c) × (1 − (0.1 L + 0.6))) × 1.4
   (`0x45e230`), using c before any pin (`0xd3f7e`–`0xd4062`). Each sum is in
   the order written.
10. It returns true (`0xd4068`).

F24, F60, F80 and F88 are the tag functions at entry `+0x24`, `+0x60`, `+0x80`
and `+0x88`, each evaluated and mapped as described under [Data](#data). The
two luminances differ: `0xd1e10` also uses the weights 0.3, 0.59 and 0.11, and
`0xd4080` and `function_336f0` the weights 0.299, 0.587 and 0.114.

### `0xd47d0`: the lightmap colour under a collision result

It sets the colour to the vector of `g_4686d4`'s default, (0, 0, 0)
(`0xd47d6`–`0xd47ed`; `s_33a0b_default`, `src/unknown_033a0b.cpp:22`). When the
structure bsp globals (`g_4e0344`) are set and
`structure_get_lightmap_triangle` (`0x14ac60`) finds a triangle for the
collision result, it calls `0xd16d0` (`0xd4816`). That writes the lightmap
colour, pinned to [0, 1], when it succeeds. `0xd16d0`'s real output is the
address of `0xd47d0`'s own argument slot, and its result is ignored: `0xd47d0`
returns false on every path (`0xd481b`).

Its caller `0x180d80` calls it when word `+4` of its definition is 0 or 1, and
would replace a factor of 1.0 with the colour's luminance if it returned true
(`0x180fdc`–`0x181010`). That branch is dead in retail, and the colour is never
read. A matching `0xd47d0` returns false whatever `0xd16d0` does.

## Callers without source

All three callers are `todo` with no source, and were read only as far as these
functions need.

- `0x3db00` (397 bytes, lane W) gives the record for an object (`eax`; one real
  on the stack, `ret 4`). Its only call site is `0x4ba21`. It returns the
  object globals' record (`g_4de2f4 +0x1c`) when the object's flags have bit
  24, and an entry of `g_4e0348 +0x230` (0x5c bytes each, from `+8`) when the
  object's word `+0xd0` is not NONE. Otherwise it looks up the lighting datum
  of the object's root (`function_baf80`, then `0x3ddd0`), a 0x100-byte datum
  of `g_509434`, and returns that datum's record at `+0x54`. For the root
  itself it first updates the datum with `0x3dfe0`; for an object below it, it
  copies the root's record and query into the object's own datum, if it has one
  (`0x3dbc1`–`0x3dc10`).
- When the root has no datum, `0x3db00` fills a local query with `0xd1850` and
  the record at `0x55ee60` with `0xd4080`, and returns that address
  (`0x3dc20`–`0x3dc83`). The record is a single static one in `.data`, so each
  such call replaces the last (inferred). `0xd1850`'s result is not tested, and
  without a lightmap `0xd1850` returns true without writing the query, so
  `0xd4080` then reads a local that was never written (inferred).
- `0x3dfe0` (828 bytes, lane W), called only from `0x3db00`, updates a lighting
  datum. `0xd1850` fills the query at `+0x30`; on success the datum's byte `+2`
  is set, and without it `0x3dfe0` returns before `0xd4080` (`0x3e174`). The
  query at `+0x1c` becomes a copy of `+0x30` (`0x3e28a`). When byte `+3` and
  the byte at `0x467464` are set and a local flag is clear
  (`0x3e17a`–`0x3e198`), it is instead blended towards it (`0x3e5a0` for the
  vector, `0x3e4e0` for each colour), or kept as it was when a distance test
  fails and `0xbadc0` returns 0 (`0x3e237`). Then `0xd4080` fills the record at
  `+0x54` from the query at `+0x1c` (`0x3e309`), and byte `+3` is set. Lane R's
  `0x17b5d0` reads the colours of `+0x1c` (`src/unknown_175bd0.cpp:2998`).
- `0x180d80` (2683 bytes, lane F, machine 2) makes a decal (`function_17cfa0`,
  `src/unknown_17d2a0.cpp:344`). It passes `0xd47d0` a local colour and its
  third argument, the collision result, whose word `+0x20` must not be NONE
  (`0x180dc2`). See
  [`0xd47d0`](#0xd47d0-the-lightmap-colour-under-a-collision-result).

## Existing declarations

At `b04f998` nothing declares, stubs or calls `0xd37f0`, `0xd4080` or `0xd47d0`
in the source. The reals at `0x467484` and `0x467494`, the record at
`0x55ee60`, and the scenario and globals offsets `+0x33c` and `+0x184` are not
declared either.

## Evidence

- Every entry was disassembled from the retail XBE with capstone 5.0.9, through
  `tools/xbe.py`. Conventions, offsets, flags and constants were read from
  retail code and data, and the callees with source were read from it. Two
  readers analysed the range, and a verifier checked their claims against its
  own disassembly: 30 confirmed and 13 corrected, with the corrections applied
  here, and 2 left as the readers' inferences and test runs, which it did not
  repeat.
- One reader also ran `0xd37f0` and the real `function_117c80` from their
  retail bytes in a small x86 interpreter written for this analysis, with
  `function_13b390` replaced by a stub that records its inputs, and compared
  the results with a model of the steps above. Over 1,500 random cases, and
  1,500 more with new seeds when it was rerun for this document, there were no
  differences in the result, the fields written, their values or the tag
  function calls. Every instruction ran except three in the pin of step 8,
  which cannot run.
- Callers come from an image-wide scan for calls, jumps and absolute pointers.
  The one dword equal to `0x3db00` (at `0x43606f`) spans two entries of a table
  of integers, so it is not a pointer. The object type tags are read from
  `g_468630`'s type definitions (`+4`), and the script global numbers from
  `g_473468`'s table.
- Constants are read from retail `.rdata`: `0x45dbc0` (1.0), `0x45dbcc` (−1.0),
  `0x45dbdc` (0.0001), `0x45dc0c` (2.0), `0x45dc54` (0.0174533), `0x45dc68`
  (0.1), `0x44ae8c` (0.6), `0x45dc1c` (0.05), `0x45dd30` (0.4), `0x45dd1c`
  (0.7), `0x45e2bc` (1.18), `0x45dc2c` (0.75), `0x45dcb4` (0.299), `0x45dcb8`
  (0.587), `0x45dcbc` (0.114), `0x44ae90` (0.3), `0x45e234` (0.59), `0x45e238`
  (0.11), `0x45e23c` (−0.75) and `0x45e230` (1.4); the doubles 0.25 at
  `0x45e2c0` and 0.75 at `0x45e1d0`; and in `.data`, 15.0 at `0x467484` and 5.0
  at `0x467494`, and the pointers `0x4687b0` to (0, 0, 1) and `0x4686d4` to (1,
  0, 0, 0).
- No document covered the range before this one. Source line numbers are at
  `b04f998`.
- No SDK or outside dataset was used. Names are the repository's own, or
  describe behaviour.
