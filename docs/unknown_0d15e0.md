# Surface colour queries (unknown_0d15e0)

Retail range covered: `0xd15e0`–`0xd2bef`. These are ten inventory entries,
5,574 retail bytes, all `todo` with no source: the queries that give effects
and objects the colour of the surface at a point, as a base colour, a lightmap
colour and a normal. **Analysis only:** this document adds no source, and
nothing in it has been built or checked against retail with the original
compiler. Names are provisional. The `function_<va>` form is primary; the
descriptions are offered for whoever decompiles the range.

It uses the names of `src/structures.cpp` (the structure BSP and its lightmap
triangle), `src/unknown_0d0690.cpp` (the render model triangle interpolation
these functions call) and `src/unknown_175bd0.cpp` (lane R's effect code, which
calls two of them). Three callees outside the range have no source and are
described only as far as these functions need them.

## Boundary

- `0xd1540`, just before the range, is `todo` with source in
  `src/unit_object_type.cpp`. `0xd2bf0`, just after it, is `todo` with source
  in `src/unknown_0d0690.cpp`. Both are excluded.
- No row of the Active claims table (issue #9) covers the range, so it is open.
  This document makes no claim, and it is offered to whoever takes it; lane R's
  code calls `0xd2a50` and `0xd2bb0`.
- Every entry is `todo` with no `@retail` marker. Two have `@stub` definitions;
  see [Existing declarations](#existing-declarations).

## Conventions

"Callers" counts the functions that call an entry directly, from outside the
range and then from inside it. No entry is reached through a pointer or a
table.

| Retail | Bytes | Arguments | Returns | Callers | What it does |
| --- | --- | --- | --- | --- | --- |
| `0xd15e0` | 73 | `eax` bitmap, `xmm2` bias | texture | 2 + 0 | A bitmap's texture: its own when current, else the texture cache's with flags 0 |
| `0xd1630` | 80 | `eax` bitmap | texture | 1 | The same with flags 6 and bias 0 |
| `0xd1680` | 80 | `eax` bitmap | texture | 2 + 0 | The same with flags 4 and bias 0 |
| `0xd16d0` | 370 | `esi` triangle; stack: colour out, real out (`ret 8`) | bool | 1 | The lightmap colour at a structure triangle |
| `0xd1850` | 469 | `edi` object; stack: bool, query out (`ret 8`) | bool | 2 | The surface colour under an object's centre, falling back on the sky |
| `0xd1a30` | 821 | stack: object, point, query out (`ret 0xc`) | long | 0 + 1 | The sky's light direction and colour, as seen from a point |
| `0xd1d70` | 155 | `edx` triangle; stack: section out (`ret 4`) | bool | 0 + 2 | The structure section a triangle belongs to, when it is resident |
| `0xd1e10` | 3130 | stack: triangle, query out, flags, bool (`ret 0x10`) | long | 0 + 2 | Samples a triangle's base colour, lightmap colour and normal |
| `0xd2a50` | 337 | `ebx` point; stack: flags, object to ignore, bool, query out, direction (`ret 0x14`) | long | 1 + 1 | The surface colour found by up to five short rays from a point |
| `0xd2bb0` | 59 | `eax` collision result; stack: query out (`ret 4`) | long | 1 | The surface colour at a collision result |

The callers outside the range are lane R's `0x17b5d0` (`0xd2a50` and
`0xd2bb0`), `0x3db00` and `0x3dfe0` (`0xd1850`), `0xd47d0` (`0xd16d0`), and
`0xd2f90` and `0xd30d0` (the three texture getters).

## Data

**The triangle** is the 0x38-byte `s_structure_lightmap_triangle`
(`src/structures.cpp:133`). `structure_get_lightmap_triangle` (`0x14ac60`)
fills it for the structure BSP, and `0x14a8e0` for a render model; `0x14af40`
reaches them from a ray and `0x14b120` from a collision result. These functions
read its `+0` cluster_index, `+4` instance_index, `+8` object (NONE for the
structure), `+0xc` render model tag, `+0x14` section, `+0x1c` a table index,
`+0x20` part_index, `+0x28` lightmap_part_index (the triangle index that the
`src/unknown_0d0690.cpp` helpers take), `+0x2c` a block index, and the
barycentric `+0x30` u and `+0x34` v.

**The query** is `s_effect_color_query` (`src/unknown_175bd0.cpp:149`): a
vector at `+0` and two packed colours, color_a at `+0xc` and color_b at
`+0x10`. For the structure, the vector is the interpolated normal, color_a the
base map colour and color_b the lightmap colour. For a render model they are a
blended vector, the texture colour and a vertex or table colour. From the sky,
they are the light direction, `0xff555555` and the light colour. Lane R copies
the query's color_b to the effect's color_a (`+0x104`) and its color_a to the
effect's color_b (`+0x108`), as `src/unknown_175bd0.cpp:3091` does
(`0x17b6f1`).

**The status** these functions return: 0 sampled; 1 not available (no lightmap
bitmap or texture, or no model table data); 2 nothing found (no lightmap
loaded, no triangle, or no section); 3 sampled through a locked texture
(`0xd2f90`). Callers treat 0 as found.

Defaults read through pointers in `.data`: the texture colour (1.0, 0.41, 0.7)
(`0x468734`) and the lightmap colour (1.0, 1.0, 0.0) (`0x46872c`) before
sampling, and the vector (0, 0, 1) (`0x4687b0`).

## The functions

### `0xd2bb0` and `0xd2a50`: from a collision or from a point

`0xd2bb0` turns a collision result (`s_structure_collision_result`,
`src/structures.cpp:119`) into a triangle with `0x14b120` and returns
`0xd1e10(&triangle, query, 0, 0)`, or 2 when there is no triangle. `0x14b120`
uses `structure_get_lightmap_triangle` for a structure hit; for an object it
finds the render model triangle through `function_bf9a0` and `0x14a8e0`, or
tests downward from the hit point.

`0xd2a50` finds a triangle near a point (`ebx`). It returns 2 unless the
structure's lightmap is loaded: `g_4e0344` set with `+0x80` positive,
`g_4e0348` set, and the lightmap group (`g_4e0344 +0x84`) with a bitmap tag
(`+0x1c`) and `+4` equal to `g_4e0348 +8`. The rays are the five at `0x4406f0`
with flags bit 0 (10 units along −x, +x, −y and +y, and 40 down), the caller's
direction with bit 2, and otherwise 10 units down (`0x4406e4`). Each starts
0.001 behind the point, and `0x14af40` casts it, ignoring the given object,
with collision flags chosen by the bool. It tries the rays in turn while the
status is 2; a ray that yields a triangle sets the status to `0xd1e10`'s for
it, with the flags and the bool passed on.

### `0xd1e10`: sampling the triangle

It returns 2 at once for a structure triangle whose section `0xd1d70` cannot
give. Otherwise the object field (`+8`) chooses the path.

**The structure** (`+8` NONE):

1. The part is `section +4` + part_index × 0x48. When `function_13d9f0` says
   its type is 4 or 5, it returns 0, first writing the vector (0, 0, −1) and
   both colours `0xff404040` when the bool is set. Lane R starts its query with
   the same values (`src/unknown_175bd0.cpp:3071`).
2. The lightmap entry comes from the lightmap group's instance table (`+0x4c`)
   or cluster table (`+0x2c`): a bitmap index (short `+0`) and a palette index
   (byte `+2`). An instance whose word `+0x56` is set has per-vertex colours
   instead.
3. The lightmap colour: `function_d2bf0` gives the triangle's lightmap uv, and
   `0xd2f90` samples the lightmap bitmap there, its status becoming the result.
   For per-vertex colours, `0xd35a0` gives the instance's colour block and
   `function_d3630` interpolates it (status 0), or the status is 1 without a
   block.
4. The base colour: `function_d2dc0` gives the texture coordinates, and
   `0xd30d0` samples the bitmap that `function_13da30` finds for the part's
   material (`g_4e0348 +0xa8`); 0.5 grey when that fails.
5. The vector is `function_d31c0`'s interpolated normal.
6. Both colours are pinned to [0, 1] and packed with `pack_color3f` into
   color_a (base) and color_b (lightmap).

**A render model** (`+8` an object):

1. The section (`+0x14`) must not be NONE, or it returns 2. The render model
   tag (`+0xc`) gives the mesh (section × 0x5c + `0x34` in its sections) and
   the triangle's three vertex indices.
2. When the model's count at `+0x74` is not positive, this step is skipped and
   the result stays 2, although the query is still written. `0xd2a50` then goes
   on to its next ray, whose triangle can write the query again. Otherwise
   `function_24490` finds the object's table block (keyed by the object, the
   definition at model `+0x78` and the section). The block holds 20-byte
   samples, or 10-byte quantized ones when its type is `0x39`.
   `function_0241c0` interpolates the three samples at u, v into a colour and
   returns a scaled vector (status 0), or the status is 1.
3. `function_d33a0` gives the vertex colour and `function_d31c0` the normal.
4. When both a table colour and a vertex colour are present and both luminances
   (0.3, 0.59, 0.11) are above 0.0001 (`0x45dbdc`, tested at `0xd250f` and
   `0xd2521`), color_b is their pinned sum, and the vector is the
   luminance-weighted sum of the two vectors, normalized and scaled by the
   larger of their lengths. When only one is present, or one of the two
   luminances is not above 0.0001, color_b and the vector come from the other
   one alone. With neither, they are 0 and (0, 0, 1).
5. color_a is the part material's bitmap sampled by `0xd30d0` with flags 0, or
   `0xffc0c0d0`.

`0x14a8e0` looks for the part by comparing with the triangle's `+0x24` before
it writes that field, so the comparison is with −1 and never matches
(`0x14abf3`). Render model triangles therefore always carry part 0 (`+0x20`),
and step 5 always uses part 0's material. This is retail's behaviour.

### `0xd1850` and `0xd1a30`: an object's lighting

`0xd1850` fills a query for the object in `edi`, at its centre (`+0x30`). Its
two callers, `0x3db00` and `0x3dfe0`, keep the result in object lighting
records that lane R's `0x17b5d0` also reads (`src/unknown_175bd0.cpp:3060`).

1. Without the lightmap (the same checks as `0xd2a50`) it returns true and
   writes nothing.
2. It asks `0xd2a50`, ignoring the object itself, with the five rays when the
   object's flags have bit 13 or its definition's byte `+2` has bit 1, and
   otherwise the downward ray. Status 0 returns true.
3. Any other status counts as failed. If it used the five rays, it tries the
   downward ray alone; a 0 there does not clear the failure. A 3 from the last
   call returns false.
4. Only a vehicle whose definition byte `+0x1f0` is 3 or 5, a biped for which
   `0xe5670` gives 2, or a type-12 object whose definition has `+0xd4` bit 4
   goes on to `0xd1a30`; any other object returns false. It returns true when
   `0xd1a30` returns 0.

`0xd1a30` gives the sky's answer at a point. It returns 2 unless a sky is
current (`g_4b9ee9`, the index `g_4b9eec` into `g_4e0350`'s list, and that
tag's data).

1. Without lights (sky `+0x78` not positive), the colour is sky `+0x34` and the
   direction (−1, −1, −1) normalized.
2. Otherwise it sums the directions (cos a cos b, sin a cos b, sin b) of the
   lights whose `+0x2c` is positive, with a and b at light `+0xc` and `+0x10`.
   It blends in each light's colour with `function_131c20`, by a factor from
   that light's intensity and the running total of the intensities before it
   (`0xd1b9e`).
3. A 100-unit ray from the point along that direction (`0x1697c0`, flags
   `0x10000001`, ignoring the object) keeps the light colour only when it hits
   a surface whose word `+0x5a` is −1; otherwise the colour is sky `+0x34`.
4. It writes the direction, `0xff555555` and the packed colour, and returns 0.

### `0xd16d0` and `0xd1d70`

`0xd16d0` gives only the lightmap colour of a structure triangle. Its one
caller, `0xd47d0`, makes the triangle with `structure_get_lightmap_triangle`
first. It returns false for an object's triangle, without a section, or when
`0xd2f90` (flags 0, from (0.5, 0.5, 0.5)) returns any non-zero status. Unlike
`0xd1e10`, it does not check for per-vertex colours. Otherwise it writes the
colour pinned to [0, 1] and returns true. Its second output gets a float that
is never computed: the stack slot of its first argument, which holds only the
result byte (inferred: an uninitialised local in the original).

`0xd1d70` gives the section of a structure triangle. For an instance it is the
instance definition's `+0x50` (`g_4e0348 +0x13c`, by the instance's `+0x34`).
For a cluster it is `function_16e290`'s answer for cluster `+0xa0` +
cluster_index × 0xb0. Either way it is given only when that geometry block
(`+0x28`) is resident (`function_12de70(..., 3)`). An object's triangle returns
false. `function_16e290` can return NULL with `g_4e6494` set while `0xd1d70`
returns true, and `0xd1e10` would then read through it (inferred).

### `0xd15e0`, `0xd1630` and `0xd1680`: textures

All three return the bitmap's texture (`+0x50`) when its frame stamp (`+0x70`)
is newer than `g_4e6488` (`0xd15e9`, `0xd1639`, `0xd1689`). Otherwise they
return `texture_cache_bitmap_get_texture`'s (`0x12ccf0`) with their flags (0, 6
and 4) and bias (`xmm2` for `0xd15e0`, else 0.0), or failing that `0x12ce00`'s.
This is the shape of `function_3bcb0` and `0x12360` (#86, item 22). Two of
`0xd15e0`'s call sites test its result, and the other two pass it to
`function_1d2f0`.

## Callees without source

Three callees of `0xd1e10` sit outside the range, among
`src/unknown_0d0690.cpp`'s functions, and have no source.

- `0xd2f90` (308 bytes) samples the lightmap bitmap. It takes the colour output
  in `ebx` and, on the stack, the flags, the bitmap index, the palette index, u
  and v (`ret 0x14`). It returns status 1 without the lightmap, else finds the
  bitmap in the lightmap group's bitmap tag (`function_137550`). With flags bit
  1 it prefers `0xd1680`'s texture, loading it with `0x1cfb0` when missing. It
  reads the bitmap's own pixels (status 0) when it has no texture, or when
  `0xd1680` has one; otherwise it locks the texture (`0xd1630`, `0xd15e0`,
  `function_1d2f0`) and reads that (status 3). The pixel comes from
  `function_1362b0`, through `function_015cd0`'s palette for format `0x12`
  (P8), and `unpack_color3f` writes it. A failure zeroes the colour.
- `0xd30d0` (236 bytes) samples the first bitmap of a bitmap tag, the same way
  but without a palette. It takes the tag index in `eax`, the colour output in
  `edi`, and on the stack the flags, u and v (`ret 0xc`). It returns a bool and
  zeroes the colour on failure.
- `0xd35a0` (134 bytes) gives a per-vertex colour block. It takes an index in
  `eax`, the lightmap group in `ecx`, a selector in `dl` and one stack argument
  (`ret 4`). It returns the block of an instance (group `+0x54`) or of another
  entry (`+0x64`) when that bucket's geometry block is resident, with its
  offsets set to the entry's first vertex (four bytes a vertex for type `0x2e`,
  three otherwise), or NULL.

## Existing declarations

Two entries have `@stub` definitions in `src/stubs/lane_r.cpp`, repeated at
`src/unknown_175bd0.cpp:158`:

| Retail | Declared as | Where | What retail shows |
| --- | --- | --- | --- |
| `0xd2bb0` | `long function_d2bb0(void *source, s_effect_color_query *query)` | `src/stubs/lane_r.cpp` | The count agrees: the collision result in `eax` and the query on the stack (`ret 4`). `source` is an `s_structure_collision_result`. Retail returns 0 when it found a colour |
| `0xd2a50` | `long function_d2a50(long a, long b, long c, s_effect_color_query *query, long d, point3f const *point)` | `src/stubs/lane_r.cpp` | The count agrees: the point in `ebx` and five stack arguments (`ret 0x14`). `a` is the flags (bit 0 five rays, bit 2 use `d`, bit 1 passed on to the samplers), `b` the object the rays ignore, `c` a bool, and `d` a `vector3f const *` direction rather than a long |

Lane R's caller read `function_d2bb0`'s result the wrong way round when this
was written. Lane R rounds 5 and 6 (`ec81fbe`) fixed it:
`src/unknown_175bd0.cpp:3076` now goes to the fallback when it returns
non-zero, as retail's `0x17b5d0` does (`test eax, eax` and `je 0x17b6f1` at
`0x17b6b4`). Its test of `function_d2a50` (line 3086) was already the right way
round.

## Evidence

- Every entry and the three callees were disassembled from the retail XBE with
  capstone 5.0.9, through `tools/xbe.py`. Conventions, offsets, flags and
  constants were read from retail code and data, and the callees with source
  were read from it. Two readers analysed the range, and a second reader
  checked each claim with an independent disassembler: 21 confirmed and 12
  corrected, with the corrections applied here.
- Callers come from an image-wide scan for calls, jumps and absolute pointers.
  The one dword in the image equal to `0xd2a50` (at `0x43c92c`) lies in a table
  of integers, not pointers.
- Constants are read from retail `.rdata`: `0x44ae90` (0.3), `0x45e234` (0.59),
  `0x45e238` (0.11), `0x45dbdc` (0.0001), `0x45dbc0` (1.0), `0x45dbbc` (0.5),
  `0x45dbcc` (−1.0), `0x45dc0c` (2.0), `0x45dc50` (1/65535), `0x45dc70` (0.001)
  and `0x445420` (100.0); the rays at `0x4406e4` and `0x4406f0`.
- No document covered the range before this one. Names are those of
  `src/structures.cpp`, `src/unknown_0d0690.cpp` and `src/unknown_175bd0.cpp`,
  and source line numbers are at `0f22da1`.
- No emulator, runtime testing, SDK or outside dataset was used. Names are the
  repository's own, or describe behaviour.
