# Breakable surface particles and sound (unknown_184440)

Retail range covered: `0x184440`–`0x18562d`. This is one inventory entry, 4,590
retail bytes, `todo` with no source: the function that spawns particles over a
breakable surface and the connected surfaces of the same slot and material,
then plays one sound. **Analysis only:** this document adds no source, and
nothing in it has been built or checked against retail with the original
compiler. Names are provisional. The `function_<va>` form is primary; the
descriptions are offered for whoever decompiles it.

It uses the names of `src/unknown_183ee0.cpp` (the slot entries, `g_4e0340` and
the bit vectors one caller tests), `src/unknown_1ee340.cpp` (the collision
surfaces, edges and vertices, and `c_vertex_shape`, which walks a surface's
edges and tests a point against its polygon the same way), `src/structures.cpp`
(the structure instances), `src/damage.cpp` (`s_type_1e6529`, the damage data),
`include/effects.h` and `src/unknown_173b90.cpp` (particle systems) and
`include/sound_sources.h`. No document in `docs/` covers the function or its
callers.

## Boundary

- `0x184400`, just before, is `matched`, and `0x185630`, just after, is `todo`;
  both have source in `src/unknown_183ee0.cpp`. Both are excluded.
- Lane F's row of the Active claims table (issue #9, `0x180000`–`0x18ffff`, run
  on machine 2) covers the function. This document makes no claim and is
  offered to lane F. No open pull request touches it.
- `0x184440` has no `@retail` or `@stub` marker, declaration or stub at
  `9feed5f`.

## Conventions

"Callers" counts the functions that call an entry directly, from outside the
range and then from inside it.

| Retail | Bytes | Arguments | Returns | Callers | What it does |
| --- | --- | --- | --- | --- | --- |
| `0x184440` | 4590 | stack: instance index, slot, damage data, start surface (`ret 0x10`) | none | 2 + 0 | Spawns particles over a breakable surface and its connected surfaces of the same slot and material, then plays one sound |

All four arguments are on the stack, and no register carries an input: before
`0x18445e` the function reads only `eax`, set to `0x11cc` for `__chkstk`
(`0x184446`), and `cl`, loaded from `g_46dd50` (`0x184450`).

1. `[ebp+8]`, the instance index: -1 for the structure itself (`0x184467`),
   otherwise an index into the structure's instances (`0x18448b`).
2. `[ebp+0xc]`, the slot. A neighbouring surface joins only when its slot byte,
   zero-extended, equals this dword (`0x18499e`–`0x1849a3`), so it is a 32-bit
   parameter (inferred: a long).
3. `[ebp+0x10]`, the damage data, an `s_type_1e6529` (`src/damage.cpp:143`). It
   is only read.
4. `[ebp+0x14]`, the start surface: an index into the collision BSP's surfaces
   (`0x1844b1`, `0x184579`).

It returns nothing: `eax` at the `ret` is whatever the path left (the first
argument, -1, the list index or `function_1895f0`'s result), and both callers
discard it (`0x9cd5e`, `0x1841e6`).

Its two callers:

- `0x9cca0` (call at `0x9cd57`) is slot 11 (`0x451840`) of
  `c_breakable_surface_damage_event_definition`'s vtable at `0x451814`, whose
  slot 1, `function_9ca40`, returns the name `breakable-surface-damage`
  (`src/unknown_09a5e0.cpp:1108`). Slot 11 is the `v11` of
  `include/event_definitions.h:37`, and its fourth argument is the event's
  data, an `s_surface_damage_event_data` (`src/unknown_09a5e0.cpp:1718`;
  `0x9ccae`). It returns false at once unless `function_122c10` accepts the
  data's `+0x30` as a `jpt!` tag (`0x9ccb1`–`0x9ccc1`); after filling the
  damage data it also returns false unless the byte at `g_4ed280` is set
  (`0x9cd28`) and `+0x30` is not -1 (`0x9cd42`). It fills a zeroed
  `s_type_1e6529` on its stack (`0x9ccca`–`0x9cce8`): definition_index from
  `+0x30` (`0x9cd3c`), the eight bytes at `+0x1c` from the payload (`0x9ccf0`,
  `0x9ccf9`), origin from position (`0x9ccf4`–`0x9cd2b`), direction from
  direction (`0x9cd0b`–`0x9cd24`), and `+0x7c` set to -1 (`0x9cd32`). It passes
  index0, index4, that damage data and index8 (`0x9cd47`–`0x9cd56`), and
  returns false (`0x9cd5e`).
- `function_184060` (call at `0x1841e0`; a stub in `src/stubs/projectiles.cpp`)
  takes the instance in `eax` and the slot, the damage data and the surface on
  the stack (`ret 0xc`). It returns unless the byte at `g_4ed280` is set
  (`0x184073`), the damage's definition_index is not -1 (`0x184080`), the slot
  is -1 or `function_184000` finds its bit set for the instance (`0x18408d`; a
  caller can pass -1, `0x184397`), the global material's tag (see
  [Data](#data)) is not -1 (`0x1840f0`) and that tag's `+0` is above 0.0
  (`0x18410f`). Unless bit 8 of the damage data's flags (`+4`) is set, a random
  test then compares an amount from the damage data with that `+0`
  (`0x18419b`–`0x1841ae`; inferred: the chance that the surface breaks); with
  the bit set it instead requires that `+0` to be at most 10.0 (`0x184186`). It
  clears the slot's bit in `function_183fc0`'s vector (`0x1841b4`–`0x1841d6`)
  and calls this function with the instance, slot, damage data and surface
  (`0x1841d8`–`0x1841e0`).

Retail keeps a stack convention with callee cleanup, although no code or data
holds the function's address and both callers push all four arguments. That is
only the second of the three evidence items `docs/DECOMPILING.md` asks for
before the last-resort `standard` marker (lines 116–121); the other two need a
build. For comparison, `function_14c320` (`src/unknown_14b560.cpp:1353`), a
matched function with the same prologue (`and esp, 0xfffffff8` after an `ebp`
frame) and two stack arguments (`ret 8`), takes its parameters' addresses, one
of the idioms that section names.

## Data

**The frame** is `0x11cc` bytes of locals, reserved through `__chkstk`
(`0x18444b`) in an `ebp` frame aligned to 8 bytes (`0x184443`). Offsets are
from `esp` after the three register pushes:

| Offset | Use |
| --- | --- |
| `+0x1d8` | the surface work list, longs up to the frame's top at `+0x11d8`: room for 1024 (inferred size) |
| `+0x178` | the current polygon's vertices as `point3f`, room for 8 before the list (inferred size) |
| `+0x138` | the same vertices projected to 2D as `point2f`, room for 8 (inferred size); the sound's `s_sound_position` later reuses this space |
| `+0xa0` | a `box2f` (`include/unknown_0259d0.h:30`) of the current polygon in the plane's coordinates s and t |
| `+0x80` | a `box3f` (`include/unknown_11cb00.h:16`) of every polygon visited |
| `+0x3c` | four shorts: min t, min s, max t, max s |
| `+0x6c`, `+0x110` | the list's count and the next index to visit, stored as dwords (`0x1845b9`, `0x1849c6`) and compared as shorts (`0x184971`, `0x185548`) |
| `+0x98` | the instance, or NULL for the structure |
| `+0xe4`, `+0xe0`, `+0xdc` | the collision BSP, the global material element and the tag's data |
| `+0xd0`, `+0xb8` | the current surface and its vertex count |

Retail checks none of these bounds: an edge loop of more than 8 edges, or more
than 1024 surfaces in the list, writes past them (`0x184861`, `0x184960`,
`0x1849bf`). `c_vertex_shape` keeps 8 vertices too, and its ray test projects
them into a `point2f polygon[8]` (`src/unknown_1ee340.cpp:203`) (inferred: the
same limit).

**The collision BSP** is `g_4e0340` (`src/unknown_183ee0.cpp:75`) for the
structure (`0x184472`). For an instance it is `+0x70` of the instance's
instanced geometry definition (`0x184485`–`0x1844a0`): the instance is
instances (`g_4e0348 +0x144`) + index × `0x58` (`s_structure_instance`,
`src/structures.cpp:95`), and its definition_index (`+0x34`) selects a
0xc8-byte definition in instanced_geometry_definitions (`g_4e0348 +0x13c`).
That `+0x70` is `s_slot_owner`'s list (`src/unknown_183ee0.cpp:53`), and
`function_10cf80` (`src/items.cpp:474`) computes the same address. For the
structure, `function_184060` reads the BSP from `g_4e0348 +0x18` instead
(collision_bsps, `src/structures.cpp:105`; `0x1840c7`). The fields used:

| Offset | Field | Use |
| --- | --- | --- |
| `+0xc` | planes (`s_bsp3d`, `src/unknown_14a280.cpp:22`) | 16-byte planes: i, j, k, d |
| `+0x2c` | surfaces (`s_shape_source`, `src/unknown_1ee340.cpp:42`) | 8-byte `s_shape_surface` (line 20): `field_0` the plane index, with bit 15 for the reversed plane; `edge` the first edge; `field_5` the slot (`s_slot_entry.slot`, `src/unknown_183ee0.cpp:38`); `material` |
| `+0x34` | edges | 12-byte `s_shape_edge` (line 29): `vertices[2]`, `next[2]`, `surfaces[2]` |
| `+0x3c` | vertices | 16-byte `s_shape_vertex` (line 36), the point first |

**The global material and its tag.** The start surface's material indexes
`g_4e0348`'s entries (`+0x10`, 0x14-byte `s_tag_block_entry`,
`include/globals.h:552`), and that entry's value08 (`+8`) indexes the 0xb4-byte
`s_globals_element`s (`src/unknown_187ec0.cpp:107`) of `g_4e034c` (count
`+0x150`, elements `+0x154`). The element's `+0x34`, inside unknown0a, is a tag
index. Nothing here checks that tag's group, but it becomes each particle
system's tag_index, and `function_1751d0` finds such a system's definition at
the tag's `+0x18` in its `'bsdt'` case (`src/unknown_173b90.cpp:661`), so it is
a `bsdt` tag (inferred). Its fields:

| Offset | Use |
| --- | --- |
| `+0` | a real that `function_184060` requires above 0.0; not read here |
| `+0x10` | a sound tag index; -1 plays nothing |
| `+0x14`, `+0x18` | the count and address of 0x38-byte `s_effect_particle_system_definition`s (`include/effects.h:76`) |
| `+0x1c` | the spacing of the particle grid, 0.25 when its magnitude is below 0.0001 |

**The damage data** (`s_type_1e6529`, `src/damage.cpp:143`):

| Offset | Field | Use |
| --- | --- | --- |
| `+0x0` | definition_index | the damage definition tag, whose data `+0xb0`–`+0xc4` (below) shape each particle's velocity |
| `+0x1c` | unknown1c to unknown22; an `s_location` named location in the view at `src/projectiles.cpp:1517` | each particle system's location and the sound's |
| `+0x30` | origin | the grid's origin on the start surface; each particle's distance and outward direction are measured from it |
| `+0x3c` | direction | added to each particle's velocity |

**The damage definition's particle fields.** `s_damage_definition`
(`src/damage.cpp:336`) ends at `+0x6c`; this function reads six reals beyond
it:

| Offset | Use |
| --- | --- |
| `+0xb0`, `+0xb4`, `+0xb8` | along the damage direction: a scale, a radius (the term applies only when it is positive) and an exponent (applied only when it is not 0.0) |
| `+0xbc`, `+0xc0`, `+0xc4` | away from the origin: the same three |

**Globals.**

- `g_46dd50`, a byte that is 1 in the image, gates the whole function. Its
  address is in the external script global definition at `0x47272c` (type 5,
  `_hs_type_boolean`, `include/hs.h:30`; `s_hs_external_global`,
  `src/unknown_209520.cpp:59`), entry 630 of `g_473468` (`0x473e40`). Retail
  holds no name for it.
- `g_4e7408` (`s_random_globals`, `include/globals.h:797`): the jitter comes
  from its second seed (`+4`), which the comment above that line says is seeded
  from the time.
- `g_510c74`: the particle systems, 0x54-byte records of an `s_record_pool`
  (data at `+0x44`, `include/data_array.h:26`).
- `g_4e3b44`: the tag instances (`s_tag_instance`, `include/globals.h:370`),
  with the group at `+0` and the data at `+8`.
- `g_4687a4` and `g_4687a8` (`include/globals.h:423`, 445) point at (0, 0, 0)
  and (1, 0, 0).

## The function

It has one exit (`0x18562b`), no jump table and one indirect call (`0x184e18`).
An outer loop takes surfaces from the work list. For each, it walks the edge
loop (searching the list for each neighbour) and bounds the polygon's vertices,
then loops over the tag's particle systems, and for each over a grid of rows
and columns. `0x184c57`, `0x185078` and `0x1850b8` are alignment padding after
jumps.

### Setup (`0x184440`–`0x184588`)

1. The prologue saves `ebp`, aligns `esp` to 8, reserves the frame through
   `__chkstk` and saves `ebx`, `esi` and `edi` (`0x184440`–`0x18445d`). If the
   byte `g_46dd50` is 0, it returns (`0x184450`–`0x184461`).
2. It selects the collision BSP and the instance as described in [Data](#data)
   (`0x184467`–`0x1844a4`; the BSP is kept at `0x1844cd`).
3. The start surface's material (`0x1844ab`–`0x1844b8`) gives value08
   (`0x1844c2`). The element follows from the body of `function_188690`,
   globals_element_get (`src/unknown_187ec0.cpp:147`), inline: NULL unless
   value08 is not -1, not negative and below the count (`0x1844c7`–`0x1844f3`),
   otherwise the element at that index (`0x1844f5`–`0x184505`).
4. The tag is element `+0x34`, read without a NULL check (`0x18450c`), so a
   failed lookup reads address `0x34`; `function_184060` does the same
   (`0x1840ed`). It returns when the tag is -1 (`0x18450f`–`0x184512`), and
   otherwise keeps the tag's data (`0x184518`–`0x18453a`).
5. The `box3f` starts with FLT_MAX in each minimum and -FLT_MAX in each maximum
   (`0x184543`–`0x184570`). The list holds the start surface, and its count is
   1 (`0x184579`, `0x184580`).

### The plane (`0x18458a`–`0x1847b1`)

For each surface in the list, while the next index is below the count
(`0x185541`–`0x18554d`):

1. The surface is the list's next entry, and the next index rises by one
   (`0x1845aa`–`0x1845b9`). The `box2f` restarts at FLT_MAX and -FLT_MAX
   (`0x1845d9`–`0x1845f4`).
2. The plane is the one `bsp3d_get_plane` (`0x14a300`,
   `src/unknown_14a280.cpp:50`) gives, inline: the plane at `field_0 & 0x7fff`,
   with each of i, j, k and d replaced by 0.0 - x when bit 15 is set
   (`0x1845b4`–`0x184665`).
3. With an instance, the plane moves to world space by the instance's matrix
   (`0x18466b`–`0x184741`): i' = (forward.i × i + up.i × k) + left.i × j, j'
   likewise, k' = (left.k × j + forward.k × i) + up.k × k, and d' =
   ((position.z × k' + position.y × j') + scale × d) + position.x × i'.
   `item_transform_plane` (`src/items.cpp:461`) is the same transform with its
   terms grouped differently.
4. The dominant axis of the normal is computed as in `function_120850`
   (`src/unknown_120850.cpp:9`), inline: 2 when |k| >= |j| and |k| >= |i|, else
   1 when |j| >= |i|, else 0 (`0x184747`–`0x18479d`). Then positive =
   plane[axis] > 0.0 (`0x18479f`–`0x1847b1`), and side = axis × 2 + positive
   selects a row of `g_440b94` (`include/unknown_0259d0.h:100`;
   `0x1847c5`–`0x1847db`). The row's first two entries are the two coordinates
   the 2D projection keeps.

### The edge loop (`0x1847b3`–`0x1849dd`)

The walk is the one in `c_vertex_shape`'s constructor
(`src/unknown_1ee340.cpp:113`), from the surface's `edge` (`0x1847b3`) until it
returns to that edge:

1. side = (edge.surfaces[1] == surface) (`0x18482a`–`0x184841`). The vertex at
   `vertices[side]` is copied into the 3D polygon (`0x184848`–`0x18486c`), and
   the neighbour is `surfaces[!side]` (`0x18484c`).
2. With an instance, the vertex moves to world space in place, in the order of
   `shape_transform_point` (`src/unknown_1ee340.cpp:98`), with the scale
   skipped when it is 1.0 (`0x18486f`–`0x184944`).
3. Its two coordinates from `g_440b94` go into the 2D polygon
   (`0x184947`–`0x18496e`), and the vertex count rises (`0x184976`).
4. The neighbour is appended to the list (`0x1849ba`–`0x1849c6`) when a linear
   search does not find it there (`0x184971`–`0x184993`), it is not -1
   (`0x184995`), its `field_5` equals the slot argument (`0x18499a`–`0x1849a6`)
   and its material is the start surface's (`0x1849a8`–`0x1849b8`). Unlike
   `function_183f50`, it does not test the surface's flags.
5. The next edge is `next[side]` (`0x1849ca`–`0x1849dd`).

### The sampling frame (`0x1849e3`–`0x184dad`)

1. The origin o: for the start surface (`0x1849e3`–`0x1849ed`) it is the damage
   origin P moved onto the plane: f = 0.0 - (((P.z × k + P.y × j) + i × P.x) -
   d), and o = P + f × normal (`0x1849ef`–`0x184a52`). `function_10cf80` places
   an item the same way, with 0.05f where this has 0.0 (`src/items.cpp:513`).
   For the other surfaces, o is the first vertex (`0x184a5d`–`0x184a89`).
2. u = second vertex - first vertex (x87, `0x184a90`–`0x184ac2`), normalized
   unless its length is below 0.0001 (`0x184ac6`–`0x184b29`); the length is
   used only for the 1/length scale (`0x184b00`) and is not kept. The sum is (z
   × z + y × y) + x × x. `function_30bf0` (`src/unknown_030290.cpp:284`) has
   the same epsilon and scaling but sums x first, so this is not that function
   inline; the source sums k, j, i (inferred).
3. w = normal × u, the cross product (`0x184b45`–`0x184b87`).
4. dot(u, o) and dot(w, o), each summed z, y, x (`0x184bc7`–`0x184c44`).
5. For each vertex v: s = dot(u, v) - dot(u, o) and t = dot(w, v) - dot(w, o),
   summed the same way (`0x184c60`–`0x184cc1`). Ten guarded stores keep the
   bounds: four for the `box2f`, s in x0 and x1 and t in y0 and y1
   (`0x184cc5`–`0x184d04`), and six for the `box3f`, with v
   (`0x184d0d`–`0x184da0`). A polygon with no vertices skips this loop
   (`0x184b37`, `0x184c48`).

### Each particle system (`0x184db3`–`0x18504a`)

For each definition i of the tag (`0x184db3`–`0x184dc5`,
`0x185526`–`0x18553b`), at the address in `+0x18` plus i × `0x38`
(`0x184dd0`–`0x184ddb`):

1. The definition is skipped when its tag_index is -1 (`0x184de0`), when that
   tag's group is neither `PRTM` nor `prt3` (`0x184de6`–`0x184e03`), or when
   its unknown30 is not positive (`0x184e1b`–`0x184e20`). Between the last two,
   `function_137bd0` (`src/unknown_0e4050.cpp:87`) runs inline: `g_479874` for
   `PRTM` or `g_479868` for `prt3` gets initialize(tag_index) through vtable
   `+0x50` (`0x184e09`–`0x184e18`), which is `c_interface_a::v20` (`0x2243a0`,
   `src/unknown_224240.cpp:267`) in both vtables. These are `function_173fd0`'s
   own tests, without its flag3 test (`src/unknown_173b90.cpp:610`).
2. `function_173fd0` creates the particle system from the definition in `edi`,
   with effect_index -1, the element's tag as tag_index, i as definition_index
   and event_index -1 (`0x184e26`–`0x184e36`). A NONE result skips the
   definition (`0x184e3b`–`0x184e3e`). The system is that record of `g_510c74`
   (`0x184e44`–`0x184e5e`).
3. The spacing is tag `+0x1c`, or 0.25 when its magnitude is below 0.0001
   (`0x184e57`–`0x184e87`). `set_location` (`0x175180`) gives the system the
   damage data's location, `+0x1c` (`0x184e8d`–`0x184e96`).
4. The grid bounds (`0x184eab`–`0x185045`): with inv = 1 / spacing, min s and
   min t are inv × x0 and inv × y0 of the `box2f`, each pinned to [-1000, 1000]
   and rounded up by `function_48e70`; max s and max t are inv × x1 and inv ×
   y1, pinned and rounded down by the inline sequence of `float_to_int_down`
   (`src/unknown_180b60.cpp:12`). The pins have the shape of `PIN`
   (`src/unknown_180b60.cpp:10`). The four shorts are laid out as
   `short_rectangle2d` (`include/unknown_030290.h:6`), with top, left, bottom
   and right holding min t, min s, max t and max s (inferred). The definition
   is skipped when min t > max t (`0x185042`–`0x18504a`).
5. When the spacing is exactly 0.0 (`0x184e9e`–`0x184ea5`), the bounds are
   instead all 0 for the start surface, which gets one sample at o, and the
   other surfaces skip the definition (`0x185330`–`0x185356`). After step 3 no
   value of tag `+0x1c` reaches this branch, NaN included, because a NaN fails
   the 0.0 test as unordered; retail still has it, so the source does too
   (inferred).

### The samples (`0x185050`–`0x185260`)

1. t runs over rows from min t to max t (`0x185059`–`0x185076`,
   `0x185503`–`0x185515`) and s over columns from min s to max s
   (`0x185090`–`0x1850b6`, `0x1854f3`–`0x1854fd`), each counted down from a
   16-bit count of max - min + 1. A row is skipped when min s > max s
   (`0x185080`–`0x18508a`).
2. Each sample draws twice from `g_4e7408`'s second seed with the step of
   `_random` (`include/unknown_0259d0.h:140`) and turns each draw r, the seed's
   high 16 bits, into a jitter r × (1/65535) × 1.5 - 0.75
   (`0x1850d9`–`0x18513c`, `0x18510d`–`0x1851d2`). The draw and the scaling by
   1/65535 together are `function_x82e52f` (`include/unknown_0259d0.h:146`),
   whose conversion of an unsigned value explains the 2^32 fix-up (`0x185107`,
   `0x18514c`) that can never apply after the shift. `function_259d0`
   (`src/unknown_0259d0.cpp:42`) gives the same range but adds its lower bound,
   where retail here subtracts 0.75 (`0x18512f`, `0x1851a0`); whether the
   source called it or wrote the expression is undetermined.
3. a = (s + first jitter) × spacing and b = (t + second jitter) × spacing, and
   the point is o + u × a + w × b (`0x185186`–`0x185224`).
4. The point's two coordinates from `g_440b94` (`0x18516a`–`0x185178`,
   `0x18522a`–`0x185250`) go to `function_23a220` with the 2D polygon and
   epsilon 0.0, and a point outside it is skipped (`0x18523f`–`0x185260`).
   `c_vertex_shape::ray_test` (`src/unknown_1ee340.cpp:160`) makes the same
   test.

### Each particle (`0x185266`–`0x18553b`)

1. The direction is the point minus the damage origin (x87,
   `0x185269`–`0x1852be`), and the distance its length, again summed z, y, x
   (`0x1852be`–`0x1852ec`). Below 0.0001 the distance becomes 0 and the
   direction stays as it is (`0x18535b`–`0x185367`); otherwise the direction is
   normalized (`0x1852fe`–`0x185328`).
2. The velocity starts as `*g_4687a4` (`0x185270`–`0x1852d6`), and D is the
   damage definition's data (`0x185295`–`0x1852cc`).
3. Away from the origin: when D `+0xc0` > 0, f = PIN(1 - distance / D `+0xc0`,
   0, 1), raised to the power D `+0xc4` when that is not 0.0, and the velocity
   gains the direction × (D `+0xbc` × f) (`0x18536a`–`0x18541d`).
4. Along the damage: when D `+0xb4` > 0, g is formed the same way from D
   `+0xb4` and D `+0xb8`, and the velocity gains the damage direction × (D
   `+0xb0` × g) (`0x18542b`–`0x1854c7`).
5. `function_175430` adds the particle, given the system, the point and the
   velocity (`0x1854cd`–`0x1854df`).
6. Then come the column and row counters (`0x1854e4`–`0x185515`) and the next
   definition (`0x18551b`–`0x18553b`).

### The sound (`0x185541`–`0x18562d`)

1. When no surface is left (`0x185541`–`0x18554d`), it returns if tag `+0x10`
   is -1 (`0x185553`–`0x185557`).
2. It fills an `s_sound_position` (`include/sound_sources.h:109`) at
   `esp+0x138`: the position is the centre of the `box3f`, (x1 + x0) × 0.5 on
   each axis (`0x18555d`–`0x1855be`); compressed_forward is
   `vector3d_compress(g_4687a8)` (`0x185577`, `0x1855c7`, `0x1855dc`); the
   velocity is `*g_4687a4` (`0x1855cc`–`0x185600`); and the location is the
   damage data's `+0x1c` (`0x1855f0`–`0x185619`).
3. `function_1895f0(&position, 1.0f, tag +0x10)` makes the sound request, an
   `s_sound_request` with no object that it passes to `function_189fe0`
   (`0x1855d4`, `0x185607`–`0x185620`); the result is unused. The epilogue
   follows (`0x185625`–`0x18562d`).

## Callees without source

Every callee has source except two C library helpers (`xdk:libcmt`, `todo`):

- `0x320560`, `__chkstk`, takes the frame size in `eax` (`0x11cc`, `0x184446`)
  and only reserves the frame. The source needs about 4.5 KB of locals, mostly
  the surface list.
- `0x372d48`, which has no name in the inventory, takes the base in `st(1)` and
  the exponent in `st(0)` and leaves the power in `st(0)`
  (`0x1853b2`–`0x1853e1`, `0x185470`–`0x185482`). `(real)pow(x, y)` compiles to
  it, as in the matched `function_13cbb0` (`src/unknown_13cbb0.cpp:19`).

## Existing declarations

Nothing declares or stubs `0x184440`. Its `todo` callees with source are
declared with the parameters retail passes:

| Retail | Declared as | Where | What retail shows |
| --- | --- | --- | --- |
| `0x173fd0` | `long function_173fd0(s_effect_particle_system_definition *definition, long effect_index, long tag_index, short definition_index, long event_index)` | `src/unknown_173b90.cpp:606` | The definition in `edi` and the other four on the stack (`ret 0x10`) |
| `0x48e70` | `long function_48e70(real value)` | `src/unknown_02b400.cpp:219` | One stack argument (`ret 4`); only the result's low word is kept |
| `0x23a220` | `bool function_23a220(point2f const *point, short volatile count, point2f const *points, real epsilon)` | `src/unknown_239b80.cpp:336` | The point in `ebx`, the points in `edx`, and the count and epsilon on the stack (`ret 8`) |
| `0x175430` | `void __stdcall function_175430(s_particle_system_datum *system, point3f const *position, vector3f const *velocity)` | `src/unknown_173b90.cpp:568` | Three stack arguments (`ret 0xc`) |
| `0x1895f0` | `long function_1895f0(s_sound_position const *position, real scale, long tag_index)` | `src/unknown_189010.cpp:227` | The position in `edx`, the scale in `xmm0` and the tag index on the stack (`ret 4`) |

Declarations with the same parameters are also at `src/unknown_170d70.cpp:989`
and `src/unknown_1ee340.cpp:157` (`function_23a220`),
`src/unknown_0494b0.cpp:230` (`function_48e70`), `include/effects.h:420`
(`function_173fd0`), and `src/items.cpp:594`, `src/projectiles.cpp:593`,
`src/unknown_175bd0.cpp:400`, `src/unknown_189010.cpp:525`,
`src/unknown_189a50.cpp:94` and `src/unknown_246f10.cpp:26`
(`function_1895f0`). The matched callees
`s_particle_system_datum::set_location` (`0x175180`, `include/effects.h:302`)
and `vector3d_compress` (`0x188dd0`, `src/unknown_188dd0.cpp:43`) take their
arguments as declared: `ecx` and one stack argument, and `eax`.

Of the callers, `function_184060` is stubbed in `src/stubs/projectiles.cpp`
with four parameters, as in retail (`eax` and three on the stack): unknown3c,
`unsigned char unknown59` (the slot), data and unknown50. Retail uses that slot
as a full dword (`0x184062`, `0x1841bb`–`0x1841c8`, `0x1841de`) (inferred: a
long). `c_breakable_surface_damage_event_definition`
(`include/event_definitions.h:146`) does not declare `v11`, so nothing declares
`0x9cca0`. `s_surface_damage_event_data` names `+0x30` object_name
(`src/unknown_09a5e0.cpp:1727`), but `0x9cca0` checks it as a `jpt!` tag and
copies it into definition_index (inferred: a damage definition's tag index).

## Evidence

- The function, its callers and its callees were disassembled from the retail
  XBE with capstone 5.0.9, through `tools/xbe.py`. Conventions, stack slots,
  offsets, flags and constants were read from retail code and data. A reader
  analysed the function, and a verifier checked each claim against its own
  disassembly: 23 confirmed and 5 corrected, with the corrections applied here.
  A third agent wrote this text from that work, and a fourth checked the
  finished document against retail and the repository: 200 claims confirmed and
  6 corrected or reworded, with the changes applied here.
- Callers come from an image-wide scan of every section for `E8` and `E9`
  rel32, `0F 80`–`8F` rel32 and absolute dwords at any alignment: the calls at
  `0x9cd57` and `0x1841e0` are the only references to `0x184440`.
- Constants are read from retail `.rdata`: `0x44aed4` (3.4028235e38),
  `0x4409d4` (-3.4028235e38), `0x45dbc0` (1.0), `0x45dbdc` (0.0001), `0x45dc20`
  (0.25), `0x45df28` (-1000.0), `0x45dccc` (1000.0), `0x45dbd8` (0.0),
  `0x45dbb8` (4294967296.0), `0x45dc50` (1.5259022e-05), `0x43ff08` (1.5),
  `0x45dc2c` (0.75) and `0x45dbbc` (0.5); the table `g_440b94` at `0x440b94`;
  and the vectors at `0x440af4` (0, 0, 0) and `0x440b30` (1, 0, 0), through
  `g_4687a4` and `g_4687a8`.
- Source line numbers are at `9feed5f`.
- No SDK or outside dataset was used. Names are the repository's own, or
  describe behaviour.
