# Lights and liquids analysis (unknown_0bffa0)

Retail range covered: `0xbffa0`–`0xc40e7`. These are 45 inventory entries,
16,363 retail bytes, in two parts: the lights, a data array of 350 lights
with their own cluster partition, and the liquids, a data array of 64
entries behind the `tdtl` entry of a table of per-group functions. **Analysis
only:** this document adds no source, and nothing in it has been built or
checked against retail with the original compiler. Names are provisional.
The `function_<va>` form is primary; the descriptions are offered for whoever
decompiles the range.

It follows the two object core documents, [part 1](unknown_0b7740.md) and
[part 2](unknown_0bc190.md), and uses their names for the object header and
fields. Part 2's attachment code creates and removes the lights described
here.

## Boundary

- `0xbff60`, just before the range, is matched in `src/unknown_0bfd20.cpp`.
  `0xc40f0`, just after it, is matched in `src/unknown_0c40f0.cpp`. Both are
  excluded.
- No row of the Active claims table (issue #9) and no open pull request
  covers the range.
- Every entry in the range is `todo` and has no `@retail` marker at
  `7431fe6`. Two of them are declared or have `@stub` definitions in other
  files; see [Existing declarations](#existing-declarations).
- Nine entries have no direct callers. They are reached through the tables in
  [Pointer tables](#pointer-tables).

## Conventions

"Callers" counts direct calls from outside the range, then calls from inside
it. Calls through pointers are not counted. Results are in `eax` (`al` for a
bool) unless the table names another register. "Light" and "liquid" mean
datum indices of the two data arrays.

| Retail | Bytes | Arguments | Returns | Callers | What it does |
| --- | --- | --- | --- | --- | --- |
| `0xbffa0` | 146 | none | | 1 | Creates the lights data array, the light globals and the light partition |
| `0xc0040` | 92 | none | | 1 | Empties the lights and the light partition |
| `0xc00a0` | 108 | none | | 1 | Connects every light that has no object |
| `0xc0110` | 161 | none | | 1 | Disconnects every connected light that has no object |
| `0xc01c0` | 106 | none | | 1 | Deletes every light without an attachment object |
| `0xc0230` | 273 | stack: tag, object, attachment index, value, 16-bit value (`ret 0x14`) | light, or NONE | 1 | Creates an attachment's light |
| `0xc0350` | 452 | `edi` forward, `ebx` position, `xmm0` scale; stack: tag, object, node, up (`ret 0x10`) | light, or NONE | 1 | Creates an effect's light |
| `0xc0520` | 795 | `eax` tag; stack: scenario light index, flag, placement (`ret 0xc`) | light, or NONE | 0 + 1 | Creates a scenario light |
| `0xc0840` | 383 | `eax` end, `esi` start; stack: point out, distance out (`ret 8`) | bool | 0 + 1 | Finds the first point inside the map along a segment |
| `0xc09c0` | 133 | `eax` game options state (`ret`) | | 2 | Creates the scenario's lights |
| `0xc0a50` | 175 | `eax` point, `ecx` index, `xmm2` range; stack: radius, table (`ret 8`) | float in `xmm0` | 0 + 1 | A distance fade |
| `0xc0b00` | 384 | `ecx` animation block, `eax` colour; stack: seed, range, value to scale, values out (`ret 0x10`) | | 0 + 1 | Applies a light definition's animation functions |
| `0xc0c80` | 1607 | stack: light (`ret 4`) | | 0 + 4 | Updates a light |
| `0xc12d0` | 296 | `edx` light (`ret`) | | 0 + 1 | Computes a light's distance fades |
| `0xc1400` | 412 | none | | 1 | Updates every light |
| `0xc15a0` | 197 | none | | 2 | Computes the fades of every lit light |
| `0xc1670` | 163 | stack: object (`ret 4`) | | 0 | Deletes the lights on an object's nodes |
| `0xc1720` | 203 | stack: object, remove, add (`ret 0xc`) | count | 1 | Removes or adds the lights on an object's nodes |
| `0xc17f0` | 237 | `eax` render data out, `edx` light; stack: flag (`ret 4`) | bool | 3 + 1 | Prepares a lit light for rendering |
| `0xc18e0` | 75 | none | float in `xmm0` | 1 | A value from the time in the light globals |
| `0xc1930` | 78 | `eax` definition, `edx` shape out (`ret`) | | 1 + 1 | Copies a light definition's shape |
| `0xc1980` | 104 | `esi` placement, `ebx` definition, `edx` shape out (`ret`) | | 0 + 1 | Copies a scenario placement's shape |
| `0xc19f0` | 130 | `eax` light, `edi` shape out (`ret`) | | 0 + 4 | A light's shape |
| `0xc1a80` | 773 | `edx` shape, `ecx` a frame laid out like a light's `+0x84`; stack: definition (`ret 4`) | | 1 + 2 | Derives a shape's render values |
| `0xc1d90` | 2840 | stack: frame, shape, entries, 16-bit count out (`ret 0x10`) | | 2 | Builds a light's render projections |
| `0xc28b0` | 919 | `eax` light (`ret`) | | 0 + 1 | Computes a light's bounding sphere |
| `0xc2c50` | 162 | `ebx` light (`ret`) | | 0 + 1 | Computes a light's fade sphere |
| `0xc2d00` | 1027 | stack: light (`ret 4`) | | 1 + 4 | Connects a light to the map |
| `0xc3110` | 42 | `eax` light (`ret`) | bool | 2 | Stamps a light |
| `0xc3140` | 212 | `eax` light (`ret`) | bool | 3 | Whether a light is bright enough to draw |
| `0xc3220` | 58 | `eax` light (`ret`) | | 1 + 1 | Disconnects a light |
| `0xc3260` | 212 | stack: light, flag (`ret 8`) | | 1 + 3 | Deletes a light |
| `0xc3340` | 607 | stack: light, unused (`ret 8`) | | 0 | Draws a light definition's two tag references |
| `0xc35a0` | 820 | `edx` light; stack: corners out (`ret 4`) | bool | 0 + 1 | The eight corners of a light's volume |
| `0xc38e0` | 104 | `bx` name index (`ret`) | light, or NONE | 1 | Finds a scenario light by name |
| `0xc3950` | 235 | `eax`, tested only for NONE; stack: lights out, maximum (`ret 8`) | count | 1 | Collects lights with definition flags 5 and 18 |
| `0xc3a40` | 64 | none | | 0 | Creates the liquids data array |
| `0xc3a80` | 18 | none | | 0 | Makes the liquids valid and empty |
| `0xc3aa0` | 10 | none | | 0 | Makes the liquids invalid |
| `0xc3ab0` | 20 | none | | 0 | Forgets the liquids data array |
| `0xc3ad0` | 496 | stack: tag, object (`ret 8`) | liquid, or NONE | 0 | Creates a liquid |
| `0xc3cc0` | 34 | stack: liquid (`ret 4`) | | 0 | Deletes a liquid |
| `0xc3cf0` | 404 | stack: 16-bit pass, liquid (`ret 8`) | | 0 + 1 | Draws a liquid |
| `0xc3e90` | 252 | none | | 1 | Draws the liquids |
| `0xc3f90` | 344 | stack: one argument (`ret 4`) | | 0 | Updates the liquids |

The functions reach lights through the data (`+0x44`) of the array at
`0x4e030c`, 0x110 bytes per light, and objects through `g_4e0300` as in the
earlier parts.

## Data

### The lights

`0xbffa0` creates three things:

- the lights data array at `0x4e030c`: 350 lights of 0x110 bytes, named
  with the retail string "lights" (`0x45323c`). In the repository's terms it
  is `data_new_inlined("lights", 350, 0x110, 0, g_510c2c)`
  (`include/data_array.h`);
- 12 bytes of game state for the light globals at `g_5107e8`, with an
  inlined `function_123d40` (`src/unknown_123b30.cpp`);
- the light partition, an `s_cluster_partition` (`src/unknown_1cabc0.cpp`)
  at `0x4e0310`, named with the retail string "light" (`0x453234`). Its
  three fields are `g_4e0310`, `g_4e0314` and `g_4e0318`.

The light globals:

| Offset | Meaning |
| --- | --- |
| `+0x0` | byte: `0xc18e0` returns its curve value when it is set, and 1 minus that value otherwise. `0xc0040` clears it |
| `+0x4` | a game time that `0xc18e0` measures from, set outside the range. `0xc0040` clears it |
| `+0x8` | byte: `0xc17f0` prepares lights for rendering only while it is set. `0xbffa0` and `0xc0040` set it |

A light:

| Offset | Meaning |
| --- | --- |
| `+0x00` | salt |
| `+0x02` | flags (16 bits), see below |
| `+0x04` | tag index of the light definition |
| `+0x08` | the scenario light's index (`0xc0520`), else NONE |
| `+0x0c` | stamp: the counter at `0x4e0308` minus 1 at creation; see `0xc3110` |
| `+0x10` | first cluster reference in the light partition; NONE until connected |
| `+0x14` | creation time (`game_time`) of an effect's light, else NONE |
| `+0x18`, `+0x24` | bounding sphere: centre and radius (`0xc28b0`) |
| `+0x28`, `+0x34` | fade sphere: centre and radius (`0xc2c50`), measured by `0xc12d0` |
| `+0x38` | the point whose location `0xc2d00` finds |
| `+0x44` | location (`s_location`, with the cluster at `+0x48`) |
| `+0x4c` | an attachment light's object |
| `+0x50` | the attachment's function id (element `+0x10` in part 2) |
| `+0x54` | the attachment index (16 bits), else NONE |
| `+0x56` | 16 bits from the attachment's `+0xc`, passed to `function_bad50` |
| `+0x58`, `+0x5c` | a node light's object and node (16 bits) |
| `+0x60`, `+0x6c`, `+0x78` | a node light's position, forward and up, in the node's space |
| `+0x84`, `+0x90`, `+0xac`, `+0xb8` | position, end point, forward and up. The end point is one forward vector ahead, except for a scenario light, where `0xc0520` sets it |
| `+0x9c`, `+0xa0` | a scenario light's distance to its first point inside the map, and that point (`0xc0840`) |
| `+0xc4` | radius (`0xc0c80`) |
| `+0xc8` | the fade taken from the root unit (`0xc0c80`) |
| `+0xcc` | scale: 1.0, or the effect's scale |
| `+0xd0` | intensity |
| `+0xd4`, `+0xe0` | two colours |
| `+0xec` | a value clamped to 0 to 4 |
| `+0xf0` | two values from the animation block (`0xc0b00`) |
| `+0xf8`–`+0x108` | five distance fades (`0xc12d0`) |
| `+0x10c` | NONE at creation |

The flags at `+0x2`, by where they are set and tested:

| Bit | Set | Tested |
| --- | --- | --- |
| 0 | at creation: for an attachment light when its definition's bit 0 is clear, and always for effect and scenario lights | `0xc0c80` lights only such lights. Without it, or with definition bit 3, `0xc2d00` uses the object's root location when the light's point has no cluster |
| 1 | by `0xc0c80` when the light has a radius and a colour, cleared otherwise | by `0xc0c80`, `0xc17f0`, `0xc2d00`, `0xc3140`, `0xc3220`, `0xc3260`, `0xc3340` and the walks over all lights |
| 2 | at creation of a scenario light | `0xc2d00` clips such a light with `0xc0840` |
| 3 | by `0xc2d00` when it inserts the light in the partition | by `0xc0110`, `0xc0c80`, `0xc1400`, `0xc2d00` and `0xc3260`; every disconnect clears it |
| 4 | at creation of an effect's light whose definition has a positive duration at `+0xb0` | `0xc0c80` fades such lights over the duration, and `0xc1400` deletes them after it |
| 5 | at creation of a scenario light whose placement has bit 2 of the byte at `+0x3e` | not tested in the range |

The light definition fields the range reads:

| Offset | Meaning |
| --- | --- |
| `+0x00` | flags. Bit 0: attachment lights start without flag bit 0, and `0xc0230` and `0xc0520` test it; bit 1: `0xc28b0` uses only the shape's first value; bit 3: see `0xc2d00` and `0xc0520`; bit 5: markers from `first_person_weapon_get_marker`; bit 7: no fade from the root unit; bit 8: see `0xc17f0`; bit 15: see `0xc2d00`; bits 5 and 18: `0xc3950` |
| `+0x04` | shape kind (16 bits), with shape values at `+0x18`, `+0x1c` and `+0x20` (`0xc1930`) |
| `+0x08`, `+0x0c` | a pair `0xc0c80` interpolates by intensity for the radius; `+0x0c` also scales the bounding sphere |
| `+0x34` | flags for the colour interpolations (`function_131c20`, `function_131d60`) |
| `+0x40`, `+0x4c` and `+0x58`, `+0x64` | two pairs of colours |
| `+0x70`, `+0x74` | a pair `0xc0c80` interpolates by intensity |
| `+0x94`, `+0xa0` | tag references that `0xc3340` draws. When `+0x94` is NONE, `0xc0230` and `0xc0520` can refuse to create the light |
| `+0x98` | the smallest bounding radius |
| `+0xa4` | 16 bits; 2 keeps `0xc0230` from creating the light outside game options states 4 and 5 |
| `+0xb0`, `+0xb6` | the duration of a timed light, and its curve (`function_17ca10`). `0xc0c80` divides the ticks since creation by the duration times the time globals' `field_2_3` for the curve, and `0xc1400` deletes the light once the ticks exceed that product |
| `+0xb8`, `+0xba`, `+0xbc` | indices into the fade tables (`0xc12d0`) |
| `+0xc0` | the animation block (`0xc0b00`) |

### Lights and objects

Lights come from three places:

- **Attachments.** Part 2's `0xbeca0` creates kind 0 attachments, group
  `ligh`, with `0xc0230`. Such a light keeps its object at `+0x4c` and the
  attachment index at `+0x54`. Part 2's `0xbef30` passes it to `0xc3220` to
  remove it and `0xc2d00` to add it, and `0xbee60` deletes it with
  `0xc3260(light, 1)`.
- **Effects.** `0xc0350` creates the light of an effect part, either on an
  object's node (`+0x58`, `+0x5c`) or in the world. A node light sets the
  object's flag bit 6, which makes part 2's `0xbef30` call `0xc1720`. When
  the object is deleted, `0xc1670`, the second of the deletion callbacks at
  `0x468664` (part 1), deletes its node lights.
- **The scenario.** `0xc09c0` creates a light for each entry of the
  scenario's lights block with `0xc0520`.

`0xc1400` reconnects a light whose object has header flag `0x04`, which
part 2 reads as woken during this tick. `0xc0c80` fades the lights of a
biped or vehicle by the positive float at `+0x2b0` of the root unit. Part 2's
export id `0x130006bb` is `0xc18e0`.

### Liquids

The liquids are the `tdtl` entry of a table of per-group functions. Each
entry is 0x38 bytes: a group tag, a zero, and twelve function slots. The
two entries before the `tdtl` entry at `0x467508` are `ant!` at `0x467498`
and `clwd` at `0x4674d0`. `src/unknown_1169f0.cpp` decompiles the `clwd`
entry's slots as `g_4674d8`, the cloth, and the liquids fill the first slots
in the same way:

| Slot | Liquid | Cloth (`src/unknown_1169f0.cpp`) |
| --- | --- | --- |
| 0 | `0xc3a40`: creates the data array | `function_116a10` |
| 1 | `0xc3a80`: makes it valid and empty | `function_116a50` |
| 2 | `0xc3aa0`: makes it invalid | `function_116a70` |
| 3 | `0xc3ab0`: sets `g_4e031c` to NULL | `function_116a80`, which also calls `data_dispose` |
| 4 | `0xc3ad0(tag, object)`: creates a liquid | `function_116980` |
| 5 | `0xc3cc0(liquid)`: deletes it | `function_1169f0` |
| 6 | `0xc3f90`: updates the liquids | `function_1168a0` |

Slots 7 to 11 of the `tdtl` entry are NULL. The function slots start at
`0x467510`.

The data array is at `g_4e031c`, which `src/unknown_0c40f0.cpp` defines: 64
liquids of 0x110 bytes, named with the retail string "liquid" (`0x453244`).
`0xc3a40` has the shape of `function_116a10`:
`data_new_inlined("liquid", 64, 0x110, 0, g_510c2c)`. The names in
parentheses below are those of `s_c40f0_datum` in the same file.

| Offset | Meaning |
| --- | --- |
| `+0x02` | flags: bit 0 set and bit 1 clear at creation; `0xc3cf0`, `0xc3e90` and `0xc3f90` work only with bit 0 |
| `+0x04` | a time, as game ticks times `rate`. At creation it is -1.0 when the definition's 16 bits at `+0x2` are 0, and 0.0 otherwise. A refresh by `0xc3ad0` sets the current time, and `0xc3f90` lowers it to the current time when it is later |
| `+0x08` | tag index (`tag_index`) |
| `+0x0c` | object (`object_index`) |
| `+0x10` | `value`: 1.0 at creation when the definition's 16 bits at `+0x2` are not 0, else 0. `0xc3cf0` uses it as the alpha when those bits are 0 |
| `+0x14` | a point from `function_b9dd0` |
| `+0x20` | 0x50 bytes for each of the definition's elements (count `+0x68`, 0xec-byte elements at `+0x6c`) |

`function_c40f0(tag_index, object_index, value)`, matched in
`src/unknown_0c40f0.cpp` right after the range, sets `value`, clamped to 0
to 1, on each liquid of that tag and object whose definition has 0 in the 16
bits at `+0x2`. The effects code calls it with 0.0 for `tdtl` parts
(`src/unknown_175bd0.cpp`). Part 2's attachment code gives `tdtl`
attachments kind 5 and creates nothing for them.

### Pointer tables

| Function | Referenced from |
| --- | --- |
| `0xc1670` | `0x468668`, the second entry of `g_468664`, the object deletion callbacks (`src/unknown_157450.cpp`) |
| `0xc3340` | pushed by `0x2c340` as a callback for `masked_list_iterate` |
| `0xc3a40`, `0xc3a80`, `0xc3aa0`, `0xc3ab0`, `0xc3ad0`, `0xc3cc0`, `0xc3f90` | slots 0 to 6 of the `tdtl` entry, from `0x467510` |

## The functions

### `0xbffa0` and `0xc0040`: create and empty

`0xbffa0` creates the lights data array, sets its allocated flag, takes the
light globals from the game state and sets their byte at `+0x8`, and creates
the light partition with `0x1cabc0(&g_4e0310, "light", 0)` when the data
array exists. `0xb67c0` calls it.

`0xc0040` makes the lights data array valid and calls
`record_pool_release_all` on it. It sets the globals' byte at `+0x8`, fills
the partition's 512 first data references with NONE, makes the partition's
two data arrays valid and empty the same way, and clears the globals' `+0x0`
and `+0x4`. `0xb69d0` calls it.

### `0xc00a0`, `0xc0110` and `0xc01c0`: lights without an object

All three walk the lights data array.

- `0xc00a0` connects (`0xc2d00`) every light with neither `+0x4c` nor `+0x58`.
  `0xb6bb0` calls it.
- `0xc0110` disconnects every such light that is connected (bit 3) and lit
  (bit 1): `function_1cae40` with the partition in `edi`, then it clears bit 3.
  `0xb6c50` calls it.
- `0xc01c0` deletes, with `0xc3260(light, 1)`, every light whose `+0x4c` is
  NONE. `0x159e20` calls it.

### `0xc0230`: an attachment's light

`0xc0230(tag, object, attachment index, value, 16-bit value)`. It creates
nothing, and returns NONE, when the definition has bit 0 and its `+0x94` is
NONE, or when the definition's 16 bits at `+0xa4` are 2 and the game options
state (`g_4e6948`, `+0x8`) is neither 4 nor 5. Otherwise it allocates a light
(`record_pool_allocate`) and stores the tag, the object at `+0x4c`, the index
at `+0x54`, the value at `+0x50` and the 16-bit value at `+0x56`. It sets
`+0xcc` and `+0xd0` to 1.0, the flags to bit 0 when the definition lacks
bit 0 and to 0 otherwise, and NONE in `+0x08`, `+0x10`, `+0x14`, `+0x58`,
`+0x5c` and `+0x10c`. Then it calls `0xc0c80(light)`, sets the stamp and
returns the light.

Callers: `0xbeca0`.

### `0xc0350`: an effect's light

`0xc0350(tag, object, node, up)`, with forward in `edi`, position in `ebx`
and scale in `xmm0`. It allocates a light and stores the tag, the creation
time at `+0x14` and the scale at `+0xcc` and `+0xd0`. The flags are 1, or
0x11 when the definition's duration (`+0xb0`) is positive.

- With an object, it keeps the vectors in the node's space (`+0x60`,
  `+0x6c`, `+0x78`), the node at `+0x5c` and the object at `+0x58`, and sets
  the object's flag bit 6.
- Without one, it stores the position at `+0x84`, the point one forward
  vector ahead at `+0x90`, forward at `+0xac`, up at `+0xb8`, and NONE in
  `+0x58` and `+0x5c`.

It stores NONE in `+0x08`, `+0x10`, `+0x4c`, `+0x54` and `+0x10c`, and 0 in
`+0x50` and `+0x56`. Then it calls `0xc0c80(light)`, sets the stamp and
returns the light.

Callers: `function_17a8a0`.

### `0xc09c0` and `0xc0520`: scenario lights

`0xc09c0` takes the game options state in `eax`, and passes on whether it is
4 or 5. For each entry of the scenario's lights block (`g_4e0350`, count
`+0xe8`, 0x6c-byte placements at `+0xec`), when the placement's palette index
(16 bits at `+0x0`) is a valid index into the palette (count `+0xf0`, 40-byte
entries at `+0xf4`) whose tag (`+0x4`) is set, it calls
`0xc0520(tag, index, flag, &placement)`. Its callers are `0x159e20` and
`0x18f000`.

`0xc0520` creates nothing when the definition's `+0x94` is NONE and either
the definition has bit 0, or the placement's 16 bits at `+0x40` are 3 and the
flag is clear. Otherwise it allocates a light with the scenario index at
`+0x08`, flags 5 (bits 0 and 2) and bit 5 from the placement's byte at
`+0x3e`:

1. the shape (`0xc19f0`);
2. the position from `+0x08`, and forward and up from the matrix that
   `function_141ce0` makes of the placement's rotation at `+0x14`: forward is
   the matrix's `up` and up is its `left` (`transform4x3f`);
3. the end point at `+0x90`: the position plus a direction times a distance.
   The direction is the normalized vector from the position to the
   placement's point at `+0x4c`. It is forward instead when the two points
   are less than 0.0001 apart, or, for shape kinds other than 0, when its dot
   product with forward is below 0.5. For kind 0 the distance is the larger
   of the shape's first two values; for the other kinds it is the shape's
   value at `+0x14` times the direction's dot product with forward;
4. when the definition has neither bit 0 nor bit 3, it moves the position
   0.001 along the direction;
5. `0xc0c80(light)`, the stamp, and the light as the result.

### `0xc0840`: a point inside the map

`0xc0840(point out, distance out)`, from `esi` toward `eax`. It steps from the
start along the segment, the step growing from 0.001 to at most 0.1 and
doubling each time. It stops at the first point whose leaf
(`function_14a280` on `g_4e033c`) has a cluster in `g_4e0348`, writes that
point and its distance, and returns true. When the map has no structure bsp
(`g_4686c4` is NONE), no point is accepted. It returns false when no point
qualifies, and for a segment shorter than 0.0001.

Callers: `0xc2d00`.

### `0xc0a50` and `0xc12d0`: distance fades

`0xc0a50` takes the distance from its point to the point at `0x4b9da0`,
subtracts its stack value and `table[index]`, divides by the range (at least
0.1), clamps the result to 0 to 1, and returns 1 minus it.

`0xc12d0` fills five fades of a light from its fade sphere's centre and
radius, with the definition's indices:

| Fade | Table | Index | Range |
| --- | --- | --- | --- |
| `+0x100` | `0x4405f0` | definition `+0xb8` | 10.0 |
| `+0xf8` | `0x440604` | definition `+0xba` | 5.0 |
| `+0xfc` | `0x440618` | definition `+0xba` | the radius |
| `+0x104` | `0x44062c` | definition `+0xbc` | 1.0 |
| `+0x108` | `0x440640` | 0 | half the radius |

Then `+0xf8` is at most `+0x100`, and `+0x104` at most `+0xf8`. `0xc15a0`
calls `0xc12d0` for every lit light.

### `0xc0b00`: animation functions

`0xc0b00` works on the definition's animation block at `+0xc0`. The time is
the game time, times `rate`, when the block's first byte has bit 0.
Otherwise it first adds an offset to the game time: the light's index times
0x1387, with the sign bit cleared, modulo 60 times the time globals'
`field_2_3`. With the block's functions:

- `+0x4` and `+0x8`: the value to scale is multiplied by `function_13b390`
  of the time and the range. When the function's byte at `+0x1` has none of
  the bits `0xf0`, that result is first clamped to 0 to 1 and mapped between
  the function's two values;
- `+0xc` and `+0x10`: the colour is multiplied by the colour that
  `function_13bc00` gives for `function_13b390` of the time, a byte per
  channel divided by 255;
- `+0x14` and `+0x18`: two values from `function_13bb90`.

Callers: `0xc0c80`.

### `0xc0c80`: update a light

1. The intensity and the two colours, in one of three ways:
   - an attachment light: the intensity is the attachment's function
     (`0xbab40(object, +0x50, &+0xd0)`), and the colours are interpolated by
     it between the definition's pairs with the object's tint
     (`function_bad50` with `+0x56`, else `*g_468710`) by `function_131d60`;
   - a timed light: the intensity is 1 minus the definition's curve
     (`function_17ca10`, 16 bits at `+0xb6`) of the elapsed part of the
     duration, times the scale; the colours are interpolated by
     `function_131c20`;
   - any other light: the scale, and the second colour of each pair.

   In the first two cases `+0xec` is interpolated between the definition's
   `+0x70` and `+0x74`; otherwise it is `+0x74`.
2. `0xc0b00` with the light's index as the seed and the intensity as the
   range. It scales the first colour (`+0xd4`) and `+0xec`, and fills `+0xf0`.
3. When the root (`function_baf80`) of the light's object is a biped or
   vehicle whose float at `+0x2b0` is positive, and the definition lacks bit
   7, it keeps that value, clamped to 0 to 1, at `+0xc8` and scales the
   colours by 1 minus it.
4. Clamps the colours to 0 to 1, and `+0xec`, doubled, to 0 to 4.
5. The radius `+0xc4` is the definition's `+0x08` to `+0x0c`, interpolated
   by the intensity.
6. A light with bit 0, a positive radius and any colour is lit (bit 1).
   When it was not lit before, `0xc0c80` connects it (`0xc2d00`), unless its
   object is hidden (`object_or_parent_hidden`), its root is not connected
   (header flag `0x40`, `function_b9d20`), or the object globals are not
   active (`g_4de2f4`, as `function_b8820` tests). A light that stops being
   lit loses bit 1, and is first disconnected (`0xc3220`) when it is
   connected (bit 3).

Callers: `0xc0230`, `0xc0350`, `0xc0520` and `0xc1400`.

### `0xc1400` and `0xc15a0`: every light

`0xc1400`, which `0x137fe0` calls, updates every light:

1. `0xc0c80(light)`.
2. When the light is connected and its object (`+0x4c`, else `+0x58`) has
   header flag `0x04`, it disconnects a lit light and connects it again
   (`0xc2d00`).
3. A timed light whose duration has passed is deleted with
   `0xc3260(light, 1)`.

`0xc15a0` calls `0xc12d0` for every lit light. Its callers are `0x2ba10` and
`0x2c560`.

### `0xc1670` and `0xc1720`: node lights

`0xc1670(object)` is an object deletion callback. When the object has flag
bit 6, it deletes every light whose `+0x58` is the object, with
`0xc3260(light, 0)`. Then it clears the flag.

`0xc1720(object, remove, add)` returns how many lights have the object at
`+0x58`. With `remove` it disconnects each lit one; with `add` it connects
each (`0xc2d00`). Part 2's `0xbef30` calls it for objects with flag bit 6.

### `0xc17f0`: prepare a light for rendering

When the definition lacks bit 8 and the stack flag is set, `0xc17f0`
refuses the light when the engine object (`g_55e4d0` at `g_4e9ae8`'s
`engine_index`, `+0xc14`) exists and bit 1 of `g_4e9ae8`'s first 32 bits is
set. Otherwise, while the light globals' byte at `+0x8` is set and the light
is lit, it fills the render data in `eax` with the shape (`0xc19f0`) and
`0xc1a80`. It returns whether the result has any size: for kind 0 a value
above 0.0001 at `+0x44`; for the other kinds one above 0.0001 at `+0x64`,
and a pair at `+0x68` and `+0x6c` whose squared length is above 1e-8.

Callers: `0x31590`, `0x31c80`, `0x404b0` and `0xc35a0`.

### `0xc18e0`: the export value

The ticks since the light globals' time at `+0x4`, times `rate` and at most
1.0, through the curve `function_17ca10(t, 5)`. It returns that value when
the globals' byte at `+0x0` is set, and 1 minus it otherwise. Part 2's
`0xbcc20` uses it for the id `0x130006bb`.

Callers: `0xbcc20`.

### `0xc1930`, `0xc1980` and `0xc19f0`: shapes

A shape starts with a kind (32 bits) and a few values; the other bytes up to
0x7c are zeroed.

- `0xc1930` copies the definition's kind (16 bits at `+0x4`) and, for kind 0,
  the values at `+0x18` and `+0x1c`, or for other kinds five values from
  `+0x20`. `0x32350` also calls it.
- `0xc1980` does the same from a scenario placement, with the kind at
  `+0x3c`. For kind 0 both values are the placement's `+0x68`; when the
  definition's `+0x1c` is not 0, the second is scaled by `+0x1c` over
  `+0x18`. For other kinds it copies five values from `+0x58`.
- `0xc19f0` uses the placement's shape for a scenario light whose placement
  has bit 0 of the byte at `+0x3e`, and the definition's otherwise. Then it
  clears `+0xc` for kind 1 and `+0x4` for kind 3. Its callers are `0xc0520`,
  `0xc17f0`, `0xc28b0` and `0xc2c50`.

### `0xc1a80`, `0xc1d90` and `0xc35a0`: render geometry

These were not traced in full.

- `0xc1a80` derives a shape's render values from the light's frame (position,
  end point, forward and up at `+0x84` to `+0xc0`) and radius: for kind 0 the
  two values times the radius and their maximum, for the other kinds a
  direction and extents along it. Its callers are `0x32350`, `0xc17f0` and
  `0xc28b0`; `0x32350` passes `0x47fd18` as the frame.
- `0xc1d90(frame, shape, entries, &count)`, with a frame laid out like a
  light's `+0x84`, sets the 16-bit count to 6 for kind 0 and to 1
  otherwise, and fills that many 0x1bc-byte projections, with
  `function_11f5f0`, `function_141590` and `0x163db0`. `0x32350` passes
  `0x47fd18` as the first argument. Its callers are `0x31c80` and `0x32350`.
- `0xc35a0` writes the eight corners of a light's volume to the stack
  argument, four at `+0x0` and four at `+0x30`, when `0xc17f0` accepts the
  light, and returns whether it did. `0xc2c50` calls it.

### `0xc28b0` and `0xc2c50`: spheres

`0xc28b0` computes the bounding sphere from the shape. For kind 0 the radius
is the shape's first value, or the larger of its first two when the
definition lacks bit 1, times the definition's `+0x0c`. For other kinds it is
the size of the volume around the cone, also times the definition's
`+0x0c`. Either way it is at least the definition's `+0x98`. The centre is
the position (`+0x84`), or the clip point `+0xa0` for a light with a clip
distance at `+0x9c`; for kind 0 that distance is also added to the radius.
The centre goes to `+0x18` and `+0x38`, the radius to `+0x24`. Then
`0xc2c50`.

`0xc2c50` sets the fade sphere: for kind 0 the bounding sphere; for other
kinds the sphere `0x1df080` fits around the eight corners from `0xc35a0`, or
a zero sphere when `0xc35a0` fails.

### `0xc2d00`: connect a light

Only for a lit light:

1. A scenario light clips its segment from `+0x84` to `+0x90` with `0xc0840`
   into `+0xa0` and `+0x9c`.
2. The frame:
   - an attachment light calls `function_b8c40` with the object in `ecx`
     and the attachment index in `eax`, and passes the result as the marker
     name. With definition bit 5 it asks `first_person_weapon_get_marker`
     first; otherwise, or when that fails,
     `function_b8d30(object, marker, &marker, 1, false)` supplies the
     position, forward and up;
   - a node light transforms its node-space vectors by the node's matrix
     (`transform4x3f_apply_point` for the position);
   - for both, the end point `+0x90` becomes the position plus forward. A
     light with neither keeps its frame.
3. `0xc28b0`, the spheres.
4. For an unconnected light without a cluster, whose definition has bit 15
   and whose radius is positive, `0x16a8e0(0x0d800001, &centre, radius,
   light, NONE, &centre, &local)`, after which `+0x38` takes the centre.
5. The location of `+0x38` (`function_11bed0`). Without a cluster, a light
   that lacks bit 0, or whose definition has bit 3, takes its object's root
   location (`object_get_root_location`), or without an object the location
   of the sphere's centre.
6. `0x1cac60` inserts the light in the partition with its sphere and
   location, as part 1's `0xb8600` inserts an object, and `0xc2d00` sets bit
   3.

Callers: `0xbef30`, `0xc00a0`, `0xc0c80`, `0xc1400` and `0xc1720`.

### `0xc3110` and `0xc3140`: per-light tests

`0xc3110` returns true, and stores the counter at `0x4e0308` in the light's
`+0xc`, when the two differ; it is true once per light each time the counter
changes, like part 2's `0xbec70` for objects. Its callers are `0x132220` and
`0x132a30`.

`0xc3140` is true for a lit light whose bounding radius (`+0x24`) and first
fade (`+0x100`) are above 0.0001, and one of whose colours has a squared
length above 0.05. Its callers are `0x31590`, `0x132220` and `0x132a30`.

### `0xc3220` and `0xc3260`: disconnect and delete

`0xc3220` disconnects a lit light (`function_1cae40` with the partition in
`edi`) and clears bit 3. Its callers are `0xbef30` and `0xc0c80`.

`0xc3260(light, flag)` disconnects a connected lit light. For a node light,
when the flag is set, it keeps the object's flag bit 6 only while another
light has the same object. Then `record_pool_release` frees the light.
Its callers are `0xbee60`, `0xc01c0`, `0xc1400` and `0xc1670`.

### `0xc3340`: the definition's tag references

Only for a lit light, and only when `0x31520` is true:

1. When the definition names a tag at `+0x94`, `0xc3340` calls `0x2dba0`
   with that tag in `ecx` and forward in `edi`. The stack arguments, in
   order, are the result of `0x3e9c0` for the light's object, 2, the light,
   the marker index (0 without markers), the position, the colour, 1 minus
   the unit fade (`+0xc8`), the intensity and 1.0. An attachment light does
   it at its marker: `first_person_weapon_get_markers` for a weapon with a
   parent, and otherwise, or when that finds none,
   `function_b8d30(object, marker, markers, 2, false)`. When more than one
   marker is found, it sets the byte at `0x55e708` and uses only the first.
2. When the definition names a tag at `+0xa0`, `0x42850` gets that tag, the
   light's object and frame, and the intensity times 1 minus the unit fade,
   clamped to 0 to 1.

### `0xc38e0` and `0xc3950`: finding lights

`0xc38e0` returns the scenario light whose placement's 16 bits at `+0x2`
equal `bx`, for a value from 0 to 0x27f, or NONE. `0x2b0baa` calls it.

`0xc3950` stores, up to the maximum, the lights whose definition has bits 5
and 18 and for which `0x31520` is true, and returns how many. `0x132220`
calls it.

### `0xc3a40` to `0xc3ab0`: the liquids data array

`0xc3a40` creates the data array (see [Liquids](#liquids)). `0xc3a80` makes
it valid and calls `record_pool_release_all`; `0xc3aa0` makes it invalid.
`0xc3ab0` sets `g_4e031c` to NULL when it is set, without `data_dispose`.

### `0xc3ad0`: create a liquid

`0xc3ad0(tag, object)` does nothing, and returns NONE, without a valid data
array, a tag or an object.

- When the definition's 16 bits at `+0x2` are 2, the object must be a
  projectile. With the root (`function_baf80`) of the projectile's `+0xc8`,
  when that object exists, it refreshes every liquid whose definition has
  value 1 there and for which `0xc4280(root, liquid +0xc)` is not NONE: the
  point becomes the projectile's position (`function_b9dd0`), and the time
  the current time. It returns NONE.
- Otherwise, when the definition's `+0x4` is set, it allocates a liquid,
  fills it as the table in [Liquids](#liquids) shows, and calls
  `function_50690` for each of the definition's elements. It returns the
  liquid.

### `0xc3cc0`: delete a liquid

`record_pool_release` on the liquid, when the data array is set and valid
and the liquid is not NONE.

### `0xc3cf0` and `0xc3e90`: draw the liquids

`0xc3cf0(pass, liquid)` draws only for pass 0, and only an enabled liquid
with an object whose time, when set, is no more than a third of a second old.
It finds the definition's marker (`+0x4`) on the object, with
`first_person_weapon_get_markers` when `0x3e9c0` is true and `function_b8d30`
otherwise. The alpha is 1 minus 3 times the age in seconds, at most 1.0, or
`+0x10` for definitions whose 16 bits at `+0x2` are 0. When the marker is
found and the alpha is positive, it calls `0x429a0`.

`0xc3e90` calls `0xc3cf0(0, liquid)` for each enabled liquid with an object,
unless the object is hidden. A hidden weapon, equipment or garbage still
draws when `function_10cf50` is true for it. `0x2bcd0` calls it.

### `0xc3f90`: update the liquids

For each liquid, the time at `+0x4` is lowered to the current time if it is
later. For an enabled liquid with an object, `0x507e0` updates each of its
0x50-byte entries with the matching 0xec-byte element of the definition and
`0xc3f90`'s argument.

## Existing declarations

At `7431fe6`:

| Retail | Declared as | Where | What retail shows |
| --- | --- | --- | --- |
| `0xc0350` | `void function_c0350(long tag_index, long object_index, long node_index, vector3f const *up, vector3f const *forward, point3f const *position, real scale)` | `src/stubs/lane_r.cpp`, `src/unknown_175bd0.cpp` | It returns the light, or NONE. The first four arguments are on the stack (`ret 0x10`); `forward` is in `edi`, `position` in `ebx` and `scale` in `xmm0`. Its caller with source, `function_17a8a0`, is `todo` |
| `0xc1670` | `void __stdcall function_c1670(long object_index)` | `src/stubs/lane_q.cpp`, `src/unknown_157450.cpp` | Agrees. `src/unknown_157450.cpp` lists it second in `g_468664`, the object deletion callbacks |

Twenty of the functions take register arguments (the table under
[Conventions](#conventions)). Thirteen take none, and the other twelve take
all their arguments on the stack.

## Evidence

- Every entry was disassembled from the retail XBE with `tools/disasm.py` at
  `8adcc51` (capstone 5.0.9). Conventions, offsets, flags, callers and the
  pointer tables were read from retail code and data. `0xc1a80`, `0xc1d90`
  and `0xc35a0` were read for their arguments, results and callers, not
  traced in full.
- Constants are read from retail `.rdata`: `0x45dbc0` is 1.0, `0x45dbcc` is
  -1.0, `0x45dbd8` is 0, `0x45dbbc` is 0.5, `0x45dbdc` is 0.0001, `0x45dc70`
  is 0.001, `0x45dc0c` is 2.0, `0x45dc68` is 0.1, `0x45dc00` is 10.0,
  `0x45dbf0` is 5.0, `0x45dc10` is 4.0, `0x45dc1c` is 0.05, `0x45dc64` is
  1/3, `0x45dbd0` is -3.0, `0x45dbb4` is 1/255, `0x45dbb8` is 2^32 and
  `0x45e0b0` is 1e-8. The fade tables hold five floats each: `0x4405f0`
  (20, 15, 10, 5, 0), `0x440604` (15, 10, 7, 3, 0), `0x440618` and
  `0x44062c` (2.5, 2, 1.5, 1, 0.5), and `0x440640` (zeros).
- The strings "lights", "light" and "liquid" and the group tags `tdtl`,
  `clwd` and `ant!` are strings and values in retail.
- Callees outside the range are described from their source in the
  repository where it exists, named above, and otherwise from their
  disassembly, only as far as this document needs them.
- No emulator, runtime testing, SDK or outside dataset was used. Names are
  the repository's own, or describe behaviour.
