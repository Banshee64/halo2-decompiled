# Object core analysis (unknown_0b7740)

Retail range covered: `0xb7740`–`0xb8bc5`. These are 21 inventory entries,
5,101 retail bytes: creating an object from placement data and the placement
data's defaults, the velocity setters, deleting objects, connecting objects to
the map and disconnecting them, the iterators over the two cluster partitions,
and switching an object's havok component on and off. **Analysis only:** this
document adds no source, and nothing in it has been built or checked against
retail with the original compiler. Names are provisional. The `function_<va>`
form is primary; the descriptions are offered for whoever decompiles the
range.

## Boundary

- `0xb7680`, just before the range, changes an object's scale. It is matched
  in `src/unknown_0b8ca0.cpp` and is excluded.
- `0xb8bd0`, just after it, returns one of an object's node matrices. It is
  matched in `src/unknown_0b8bd0.cpp` and is excluded.
- At `e2d126a`, three entries have source in `src/unknown_0b7740.cpp`:
  `0xb8820` and `0xb8b20` are matched, and `0xb7740` is `todo`. The other 18
  are `todo` and have no source. Twelve of them have `@stub` definitions in
  other files, and two more are declared in `src/unknown_1689b0.cpp`; see
  [Existing declarations](#existing-declarations).
- Apart from this document's own row, no row of the Active claims table
  (issue #9) and no other open pull request covers the range. Pull request
  #28 (bipeds, merged) changed the `0xb7880` declarations and added source
  for one of its callers, `0xde620`.

## Conventions

"Callers" counts direct calls from outside the range, then calls from inside
it. Calls through pointers are not counted.

| Retail | Bytes | Arguments | Returns | Callers | What it does |
| --- | --- | --- | --- | --- | --- |
| `0xb7740` | 132 | `eax` object, `ecx` linear velocity, `edi` angular velocity; stack: skip flag (`ret 4`) | | 13 + 1 | Stores the velocities and marks the simulation entity |
| `0xb77d0` | 168 | `esi` object, `eax` angular velocity; stack: linear velocity (`ret 4`) | | 16 | Sets the velocities and wakes the object unless both are near zero |
| `0xb7880` | 161 | stack: 5 arguments (`ret 0x14`) | | 3 | Applies an impulse through `0x1c4c50` and stores the resulting velocities |
| `0xb7930` | 271 | `esi` placement data; stack: tag index, source object, owner (`ret 0xc`) | | 29 | Fills placement data with its defaults |
| `0xb7a40` | 252 | stack: scenario object identifier, index out (`ret 8`) | scenario object, or NULL | 3 | Finds the scenario object an identifier names |
| `0xb7b40` | 2150 | stack: placement data (`ret 4`) | object index, or NONE | 32 | Creates an object from placement data |
| `0xb83b0` | 175 | stack: object, unused (`ret 8`) | | 7 + itself | Runs the deletion request and the eight deletion callbacks on an object and its children |
| `0xb8460` | 214 | stack: object, top-level flag (`ret 8`) | | 7 + itself | Frees an object and its children |
| `0xb8540` | 189 | stack: object (`ret 4`) | | 62 + 3 | Requests an object's deletion |
| `0xb8600` | 428 | stack: object, location or NULL (`ret 8`) | | 13 + 1 | Connects an object to the map |
| `0xb87b0` | 100 | stack: object (`ret 4`) | | 9 + 1 | Disconnects an object from the map |
| `0xb8820` | 23 | none | bool | 0 + 1 | Whether the object globals exist and are active |
| `0xb8840` | 68 | `eax` object (`ret`) | | 6 | Switches the object's havok component off |
| `0xb8890` | 74 | stack: object (`ret 4`) | | 6 + 1 | Switches the object's havok component on |
| `0xb88e0` | 90 | `edx` object, `edi` record out (`ret`) | | 1 + 2 | Builds the object's partition record |
| `0xb8940` | 104 | `ebx` iterator; stack: cluster, record out (`ret 8`) | object index, or NONE | 3 | First object in a cluster, partition at `0x4de2e0` |
| `0xb89b0` | 90 | `ebx` iterator; stack: record out (`ret 4`) | object index, or NONE | 2 | Next object, partition at `0x4de2e0` |
| `0xb8a10` | 104 | `ebx` iterator; stack: cluster, record out (`ret 8`) | object index, or NONE | 1 | First object in a cluster, partition at `0x4de2d4` |
| `0xb8a80` | 151 | `eax` object, `ebx` iterator (`ret`) | cluster index (16 bits), or NONE | 1 | First cluster of the object's root |
| `0xb8b20` | 71 | `ebx` iterator (`ret`) | cluster index (16 bits), or NONE | 1 | Next cluster |
| `0xb8b70` | 86 | `esi` object (`ret`) | | 16 + 2 | Refreshes the object's record in the partition at `0x4de2e0` |

The functions that touch objects reach them through `g_4e0300->data`
(`+0x44`) with 12-byte headers, as the rest of the repository does.

## Data

### Object types

`g_468630` holds the 13 object type definitions. Each definition starts with
a pointer to its name and its tag group; the names below are retail strings.
`+0x8` is the size of the type's objects, which `0xb7b40` allocates, and
`+0xa`, `+0xc` and `+0xe` are the scenario block offsets and datum size that
`src/unknown_0b7300.cpp` already describes.

| Type | Name | Group | Object size |
| --- | --- | --- | --- |
| 0 | biped | `bipd` | 0x464 |
| 1 | vehicle | `vehi` | 0x428 |
| 2 | weapon | `weap` | 0x25c |
| 3 | equipment | `eqip` | 0x184 |
| 4 | garbage | `garb` | 0x170 |
| 5 | projectile | `proj` | 0x1ac |
| 6 | scenery | `scen` | 0x13c |
| 7 | machine | `mach` | 0x1e0 |
| 8 | control | `ctrl` | 0x1d4 |
| 9 | light_fixture | `lifi` | 0x1e4 |
| 10 | sound_scenery | `ssce` | 0x148 |
| 11 | crate | `bloc` | 0x130 |
| 12 | creature | `crea` | 0x208 |

The type masks the range tests:

- `0x3`: bipeds and vehicles.
- `0x40`: scenery. `0x80`: machines. `0x380`: machines, controls and light
  fixtures.
- `0x7e0`: projectiles through sound scenery.
- `0x1883`: bipeds, vehicles, machines, crates and creatures, the types that
  can have a havok component (`havok_object_type_can_have_component`,
  `0x1c3930`).

The eight pointers right after the table, at `0x468664`, are the callbacks
`0xb83b0` calls with an object that is about to be freed: `0xbb880`,
`0xc1670`, `0x1c9f30`, `0x152cf0`, `0x16651c`, `0x15e7a0`, `0x2095e0` and
`0x17b3c0`. `include/globals.h` declares `g_468630[16]`, so its last three
entries are the first three callbacks, not type definitions.

### The object header

The elements of `g_4e0300` (12 bytes):

| Offset | Meaning in this range |
| --- | --- |
| `+0x0` | salt |
| `+0x2` | flags, see below |
| `+0x3` | type |
| `+0x4` | cluster index (16 bits): NONE at creation, set by `0xb8600` |
| `+0x6` | size of the object's memory (16 bits), kept by `0xbc280` and `0xbc380` |
| `+0x8` | the object |

| Flag | Meaning |
| --- | --- |
| `0x01` | active: `0xb7290` sets it and `0xb7300` clears it (with `0x04`) |
| `0x02` | set by `function_b7360` on the object and all its ancestors, unless the object already has it |
| `0x08` | set at creation |
| `0x10` | deletion requested (`0xb8540`). `object_or_parent_hidden` (`0xb9ce0`) and `collision_object_flags` (`0x16b430`) treat it as hidden |
| `0x40` | connected to the map: set by `0xb8600`, cleared by `0xb87b0`. `function_b9d20` reads it on the root object |

### Object fields

Offsets in the object. "At creation" means in `0xb7b40`.

| Offset | Meaning |
| --- | --- |
| `+0x00` | tag index (placement data `+0x00`) |
| `+0x04` | flags, see below |
| `+0x0c`, `+0x10`, `+0x14` | next sibling, first child, parent; NONE at creation |
| `+0x1a` | 16 bits: the index of the scenario object it was placed from (placement data `+0x10`), else NONE |
| `+0x24` | placement data `+0xa8` |
| `+0x28` | location (`s_location`: leaf, cluster, bsp). At creation the leaf and cluster are NONE and the bsp is `g_4686c4`; `0xb8600` sets it |
| `+0x40`, `+0x4c` | bounding sphere: centre and radius |
| `+0x60` | first cluster reference in the object's partition; NONE at creation |
| `+0x64`, `+0x70`, `+0x7c` | position, forward, up |
| `+0x88`, `+0x94` | linear velocity, angular velocity |
| `+0xa0` | scale |
| `+0xa4`–`+0xab` | the scenario object identifier: unique id, origin bsp index, type, source |
| `+0xac` | 16 bits, NONE at creation |
| `+0xae` | structure bsp index (8 bits) |
| `+0xaf` | 8 bits, NONE at creation. `0xb8540` passes it to `0x15b220` when it is set |
| `+0xb0` | bsp policy (placement data `+0x14`) |
| `+0xb1` | 8 bits, NONE at creation |
| `+0xb3` | 8 bits: 6 at creation when `g_510c54->game_time` is at least 6 and the object is not a machine, else 0. It is cleared again when the object gets no animation state |
| `+0xb4` | havok component index; NONE at creation |
| `+0xc0` | bit 1 from placement flag `0x20`; bit 6 `physics_active` (`0xb8840`, `0xb8890`) |
| `+0xc1` | bit 0 `physics_disabled`, from placement flag `0x08` |
| `+0xc2`, `+0xc4`, `+0xc8` | the owner: `s_effect_owner`'s `unknown8`, `unknown0` and `unknown4` |
| `+0xcc`, `+0xd0` | NONE at creation (32 and 16 bits) |
| `+0xd4` | simulation entity index; NONE at creation |
| `+0xd8` | 0 at creation |
| `+0xe0`, `+0xe2` | placement data `+0xb0` and `+0xb2` (16 bits each) |
| `+0xe4`–`+0xf0` | `maximum_body_vitality`, `maximum_shield_vitality`, `body_vitality`, `shield_vitality` (the names in `src/damage.cpp`) |
| `+0x108`, `+0x109` | 8 bits each, NONE at creation |
| `+0x10c`–`+0x12b` | the variable-length blocks, see below |

| Flag bit | Meaning |
| --- | --- |
| 0 | hidden (`object_or_parent_hidden`); see step 9 of `0xb7b40` |
| 8 | connected to the map: set by `0xb8600`, cleared by `0xb87b0` |
| 9 | set at creation when the model definition's tag reference at `+0x8` is set (its index at `+0xc`). It selects the partition: `0x4de2e0` when set, `0x4de2d4` when clear |
| 10 | placement flag `0x01`; `src/unknown_0b8ca0.cpp` calls it `mirrored` |
| 11 | the object's animation state accepted the model's animation graph |
| 14 | `0xb7290` and `0xb7300` count the active objects that have it, in the object globals at `+0x4` |
| 16 | the object definition's flag byte at `+0x2`, bit 0 |
| 17 | deleted when not active, see `0xb7b40` and `0xb8600`. It is clear before the type creation handlers run |
| 18 | set by `0xb8600` when it finds no location |
| 21 | `0xb8600` gives the partition insert a cluster bit vector with every bit set |
| 31 | the result of `0xbf9a0` at creation |

### Placement data

`0xb7930` clears the first 0xc4 bytes and sets the defaults below.
`src/unknown_0b7300.cpp` declares part of this layout as
`s_object_placement_data`, and the two agree.

| Offset | Default | Meaning |
| --- | --- | --- |
| `+0x00` | argument | tag index |
| `+0x04` | NONE | scenario object identifier: unique id |
| `+0x08` | NONE (16 bits) | origin bsp index |
| `+0x0a` | NONE (8 bits) | type |
| `+0x0b` | NONE (8 bits) | source; NONE for an object made at run time |
| `+0x0c` | 0 | passed to `0xba540` with the object |
| `+0x10` | NONE | the scenario object's index, to object `+0x1a` |
| `+0x14` | 0 | bsp policy, to object `+0xb0` |
| `+0x18` | 0, or `0x20` | flags, see below |
| `+0x1c` | zero | position |
| `+0x28` | `*g_4687a8` = (1, 0, 0) | forward |
| `+0x34` | `*g_4687b0` = (0, 0, 1) | up |
| `+0x40` | zero | linear velocity |
| `+0x4c` | zero | angular velocity |
| `+0x58` | 1.0 | scale |
| `+0x5c` | NONE | the source object's `player_index` (`+0x13c`), when it is a biped or vehicle |
| `+0x60` | NONE | the source object |
| `+0x64` | NONE | the source object's `team` (`+0x138`, sign-extended), when it is a biped or vehicle |
| `+0x68` | NONE, NONE, NONE | the owner, an `s_effect_owner` (12 bytes) |
| `+0x74` | 0 | passed to `0xbe240` |
| `+0x78` | zero | four colours (`field_3c` in the existing struct), passed to `0xbe240` |
| `+0xa8` | 0 | to object `+0x24` |
| `+0xac` | 0 | passed to `0xba590` |
| `+0xb0`, `+0xb2` | 0 | to object `+0xe0` and `+0xe2` |
| `+0xb4` | NONE (16 bits) | not read by `0xb7b40` |
| `+0xb8` | 0 | whether `+0xbc` holds the object's location |
| `+0xbc` | zero | the location (`s_location`), filled by `0xbf510` |

The 8 bytes at `+0x04` have the same layout as a scenario object's `+0x28`
(`s_type_4f0dcc`'s `unique_id`, `origin_bsp_index`, `type` and `source`).
`0xb7b40` does not read `+0x5c`, `+0x60`, `+0x64` or `+0xb4`. The type
handlers it calls get the whole block: `0x108a40` passes it to handler slot
`0x28`, and `function_108a90` to slot `0x2c`.

| Placement flag | Effect in `0xb7b40` |
| --- | --- |
| `0x01` | object flag bit 10 |
| `0x02` | the object is not deleted at the end of creation; bit 17 is clear while it is made |
| `0x04` | bit 17 is clear while it is made; at the end the object is deleted only when it has a cluster |
| `0x08` | `physics_disabled` |
| `0x10` | skips `0x108a40` |
| `0x20` | object `+0xc0` bit 1. `0xb7930` copies it from the source object's `+0xc0` bit 2 |

### Variable-length blocks

`0xbc280` allocates an object's memory at its type's size, zeroed, and
records the size in the header. `0xbc380(object, field, size, alignment)`
then appends a zeroed block: it stores the size and the offset as two 16-bit
values at `field` and `field + 2`, and stores 0 and NONE when the size is 0.
`0xb7b40` adds these blocks, in this order:

| Size at | Offset at | Contents | Size |
| --- | --- | --- | --- |
| `+0x11c` | `+0x11e` | filled with `0xff` | 8 bytes per entry; object definition `+0x94` entries |
| `+0x120` | `+0x122` | | 8 bytes per entry; the count is `+0xbc` of the first element of the model's block at `+0x60`/`+0x64` |
| `+0x124` | `+0x126` | | 0x18 bytes per entry; object definition `+0xac` entries |
| `+0x114` | `+0x116` | node matrices | 0x34 bytes per node; model `+0x78` nodes (at least 1) |
| `+0x118` | `+0x11a` | | 10 bytes per region; model `+0x70` regions (at least 1) |
| `+0x110` | `+0x112` | 16-byte aligned | 32 bytes per node, or none |
| `+0x10c` | `+0x10e` | 16-byte aligned | 32 bytes per node, or none |
| `+0x128` | `+0x12a` | the animation state | 0x90 bytes, or none |

The model is the tag at object definition `+0x38`. Without a model, one node
and one region are assumed. The animation state is allocated when the model
names an animation graph (`+0x14`) and `0x1cb0d0` accepts it. The two
32-byte blocks exist in the same case, but only for types outside mask
`0x7e0`, or for machines, controls and light fixtures whose object
definition has bit 2 of the byte at `+0xbc`. `src/unknown_0b8ca0.cpp`
already names `+0x10e`, `+0x114`, `+0x116` and `+0x11a`.

### The cluster partitions

`0x4de2d4` and `0x4de2e0` are two `s_cluster_partition`s
(`src/unknown_1cabc0.cpp`: `cluster_first_data_references`,
`data_references`, `cluster_references`). An object goes into the one at
`0x4de2e0` when its flag bit 9 is set, else into the one at `0x4de2d4`.

A data reference holds the object index at `+0x4`, the next reference at
`+0x8`, and at `+0xc` the 0x14-byte record that `0xb88e0` builds. That record
is `s_object_cluster_reference` in `src/unknown_1689b0.cpp`:

| Offset | Meaning |
| --- | --- |
| `+0x0` | type, stored as 16 bits (sign-extended from object `+0xaa`) |
| `+0x2` | `collision_object_flags` (`0x16b430`) for an object with flag bit 9, else 0 |
| `+0x4` | bounding sphere centre (object `+0x40`) |
| `+0x10` | bounding sphere radius (object `+0x4c`) |

A cluster reference holds the cluster index (16 bits) at `+0x4` and the next
reference at `+0x8`. The iterators keep their state where `ebx` points:
`0xb8940`, `0xb89b0` and `0xb8a10` keep a reference index
(`s_object_cluster_iterator`); `0xb8a80` and `0xb8b20` keep the partition,
then a reference index. Each prefetches the next element with `prefetcht0`.

## The functions

### `0xb7740`: store the velocities

Stores `*linear` at object `+0x88` and `*angular` at `+0x94`, each only when
its pointer is not NULL. It collects `0x10` for the linear and `0x20` for the
angular velocity. Unless the stack flag is set, and when it collected
something and the object's simulation entity (`+0xd4`) is not NONE, it calls
`function_b58c0(entity, mask)`. That function ORs the mask into the entity's
update flags while the simulation world (`g_4cf77c`) is in state 4.

Callers: `0xbc5e0`, `0xdd990`, `0xe4770`, `0xecdc0`, `0xf8eb0`, `0x10b010`,
`0x10b850`, `0x119020`, `0x119c10`, `0x14e200`, `0x1e55d0`, `0x1ed340`,
`0x2909b0`, and `0xb77d0`.

### `0xb77d0`: set the velocities and wake the object

1. `0xb7740(object, linear, angular)` with the flag clear, so the entity is
   marked.
2. `0x1c4b00`, with the object in `eax` and (angular velocity, 1) on the
   stack. It works on the object's havok component.
3. When the squared length of the linear velocity, or else of the angular
   one, is more than 0.0001 (`0x45dbdc`), it wakes the object:
   `function_b9b90(object, false)` (enables physics),
   `function_b7360(object)` and `0xbba20` (object in `eax`).

Callers: `0xaa260`, `0xaab40`, `0xb73b0`, `0xb9a90`, `0xcc590`, `0xe0ef0`,
`0xee090`, `0xef870`, `0xefde0`, `0xfd560`, `0x10cf80`, `0x118b10`,
`0x11b520`, `0x11b710`, `0x2056e0` and `0x256bd0`.

### `0xb7880`: apply an impulse

All five arguments stay on the stack and go unchanged to `0x1c4c50`, followed
by three outputs: an object index, which the compiler keeps in the fifth
argument's own stack slot, and two local vectors. `0x1c4c50` starts from the
object's root (`function_baf80`), writes back the object that owns the havok
component it used (the component's `+0x8`), and returns a bool. When it is
true, `0xb7880` works on that object:

1. clears `physics_disabled`;
2. `function_b7360` and `0xbba20`;
3. stores the two vectors as the linear (`+0x88`) and angular (`+0x94`)
   velocity. It does this directly, without `0xb7740`, so the simulation
   entity is not marked.

`0x1c4c50` copies the vectors that its fourth and fifth arguments point to,
when they are not NULL. In `0xd9640` and `0x179fb0` the arguments are the
object, a node index or NONE, a point, a vector, and 0.

Callers: `0xd9640`, `0xde620` and `0x179fb0`.

### `0xb7930`: placement data defaults

Clears 0xc4 bytes and sets the defaults in the table above. It looks the
source object up with `function_badc0(object, 0xffffffff)` (any type). When
that object exists:

- `+0x60` becomes its index.
- Placement flag `0x20` copies its `+0xc0` bit 2.
- For a biped or vehicle, `+0x5c` and `+0x64` take its player index and team.

With an owner, the 12 bytes are copied to `+0x68`; without one, its two longs
and its short are NONE.

Callers: `0x9d890`, `0xa5d90`, `0xbe760`, `0xccab0`, `0xccd60`, `0xceb30`,
`0xd09c0`, `function_d5060`, `0xdbc80`, `0xe7730`, `0xed910`, `0xeda70`,
`0x104150`, `0x13c250`, `0x14f2e0`, `0x152a50`, `0x15aad0`, `0x17a8a0`,
`0x19ef40`, `0x1e04a0`, `0x1e0850`, `0x1f6090`, `0x201520`, `0x242210`,
`0x28dbd0`, `0x2902b0`, `0x293130`, `0x2bc1f0` and `0x2c0980`. All of them
but `0xa5d90` and `function_d5060` also call `0xb7b40`.

### `0xb7a40`: find a scenario object

The identifier is 8 bytes: unique id, origin bsp index, type and source. Only
sources 0 and 1 are looked up; any other gives NULL. The type's scenario
block comes from `g_468630[type]` (`+0xa`, NULL when NONE) in `g_4e0350`,
with the datum size at `+0xe`. A linear search compares each datum's unique id
(`+0x28`), type (`+0x2e`) and source (`+0x2f`), and for source 0 also its
origin bsp index (`+0x2c`). On a match it writes the datum's index through the
second argument, when that is not NULL, and returns the datum.

Callers: `0x201330`, `0x295e60` and `0x2960e0`.

### `0xb7b40`: create an object

1. Unless placement flag `0x10` is set, `0x108a40` lets the type handlers
   (slot `0x28`) adjust the placement data. A NONE tag index fails.
2. The object definition is the tag's data (`g_4e3b44`); its first 16 bits are
   the type. The model definition is the tag at definition `+0x38`, if any.
3. For the havok types (`0x1883`) it calls `0x146bf0`.
4. `0xbc280` allocates the object at the type's size (definition `+0x8`).
   Failure returns NONE. The header gets flag `0x08` and the type, and the
   object gets its tag index.
5. Identity. When placement `+0x0b` is not NONE, it copies the scenario
   identifier to object `+0xa4`–`+0xab`, the scenario index to `+0x1a`, and
   the low byte of the origin bsp index to `+0xae`. Otherwise it takes a new
   unique id from the object globals (`g_4de2f4`, `+0x18`, incremented first),
   with source 2, origin bsp NONE, scenario index NONE and `g_4686c4` as the
   bsp.
6. Copies the position, forward and up vectors, both velocities and the
   scale.
7. Flags: bit 10 from placement flag `0x01`, and bit 9 when the model's tag
   reference at `+0x8` is set. Bit 31 comes from `0xbf9a0(object, &a, &b)`,
   whose two outputs are not used, and bit 16 from the definition.
8. Resets the location (leaf and cluster NONE, bsp `g_4686c4`), the header's
   cluster, the links (`+0x0c`, `+0x10`, `+0x14`), `+0x60`, `+0xac`, `+0xaf`,
   `+0x108` and `+0x109`, and copies the bsp policy.
9. An inlined "show" sequence: when flag bit 0 is set, it clears it, calls
   `0xbef30(object, 0, 1, 0, 0)` if the root is connected
   (`function_b9d20`), then `0xb8b70`. `0xbc280` zeroes the object, so this
   never runs here. It reads like a helper LTCG inlined, the counterpart of the
   hide sequence that starts `0xb8540`.
10. Sets the owner fields and clears the physics flags at `+0xc0`, then sets
    `physics_disabled` from placement flag `0x08`. The havok component and
    simulation entity become NONE. `+0xd8` is 0, and `+0xe0`, `+0xe2` and
    `+0xb3` are set as listed above.
11. When the model names an animation graph, it builds a temporary animation
    state on the stack (`0x1caed0`) and asks `0x1cb0d0` whether the graph
    drives it. Then it clears the state with `0x1cb0a0`
    (`s_animation_state::channels_clear_partial`).
12. Adds the blocks (see above), then checks
    `havok_object_type_can_have_component(tag)`. Creation fails when the havok
    components are used up. A failure here frees the object (`0xbc300`) and
    returns NONE.
13. With an animation state, it calls `s_animation_state::reset` on the
    object's own state, then `0x1cb0d0` again; the result is flag bit 11.
    Without one, `+0xb3` is 0.
14. Fills the `+0x11c` block with `0xff`.
15. `function_108a90(object, placement, &flag)` runs the type handlers'
    slot `0x2c`; the flag is a local that starts false and is not read
    afterwards. If it fails, `function_108bf0` (slot `0x3c`) runs, `0xbc300`
    frees the object, and the result is NONE.
16. Remembers bit 17, and clears it when placement flags `0x06` are set. Then
    it calls `0xba540(object, placement +0x0c)`,
    `0xba590(object, placement +0xac)` and
    `0xbe240(object, placement +0x74, &placement +0x78)`.
17. Vitality: an inlined `function_d5c90(object, NULL, NULL)` (matched,
    `src/damage.cpp`). The maxima come from the damage information that
    `function_d5b60` returns (`+0x28` body, `+0x8c` shield). The current values
    are 1.0 where the maximum is positive.
18. `0xbe8e0(object)`; object `+0x24` = placement `+0xa8`; `0xba3d0` when the
    object has an animation state; `0xbd020(object)`.
19. When the object globals are active (`g_4de2f4` is set and so is its first
    byte), it fills the placement's location once (`0xbf510` into `+0xbc`,
    the result in `+0xb8`). Then it calls `0xb8600(object, &location)`, or
    `0xb8600(object, NULL)` when `0xbf510` failed.
20. `0xbe1d0`, `function_b7360`, `+0xc0` bit 1 from placement flag `0x20`,
    `0xb8890(object)` (havok component on), `0xbe690`, and
    `function_108b80` (slot `0x38`).
21. When the object definition names an effect at `+0x50`, it calls
    `function_176780(object, &placement +0x68, 0.0, effect, 0.0, NULL, NULL)`
    (`src/unknown_175bd0.cpp`), which creates the effect on the object with
    the placement's owner.
22. `0x2e1e0`, restores bit 17, and `function_109390`.
23. Deletes the new object with `0xb8540` when all of these hold:
    - it has bit 17;
    - it is not active (header flag `0x01`);
    - the object globals are active (`0xb8820`);
    - placement flag `0x02` is clear;
    - placement flag `0x04` is clear, or the object has a cluster.
24. Returns the object index.

Callers: `0x9d890`, `0xa0350`, `0xa07f0`, `0xa0c50`, `0xa3eb0`, `0xbe760`,
`0xbf0f0`, `0xccab0`, `0xccd60`, `0xceb30`, `0xd09c0`, `0xdbc80`, `0xe7730`,
`0xed910`, `0xeda70`, `0x104150`, `0x13c250`, `0x14f2e0`, `0x152a50`,
`0x15aad0`, `0x17a8a0`, `0x19ef40`, `0x1e04a0`, `0x1e0850`, `0x1f6090`,
`0x201520`, `0x242210`, `0x28dbd0`, `0x2902b0`, `0x293130`, `0x2bc1f0` and
`0x2c0980`.

### `0xb83b0`: deletion request for an object and its children

It never reads its second argument. It calls `0xb8540(object)`, then the
eight callbacks at `0x468664` with the object, then visits each child
(`+0x10`, then `+0x0c`):

- Normally it recurses with `(child, false)`.
- When the game options' mode (`g_4e6948`, byte `+0xc`) is 4 and the child
  has a simulation entity, it does not recurse. It skips a child whose parent
  is NONE, calls `0xcc590` (child in `eax`) for a biped or vehicle, and calls
  `0xb9a90` (child in `ebx`) for anything else.

Its callers are exactly `0xb8460`'s: `0xb7150`, `0xbf2c0`, `function_d52b0`,
`0x159e20`, `0x1c8560`, `0x1e2990` and `0x268c90`. `function_d52b0` calls
`0xb83b0(object, true)`, then `0xb8460(object, true)`.

### `0xb8460`: free an object and its children

1. When the second argument is set: `0xb9890` if the object has a parent,
   then `0xb7300` (deactivates), then `0xb87b0` if flag bit 8 is set.
2. `object_widgets_delete` (`0xd4a60`) and `0xbee60`.
3. Each child, recursively, with `(child, false)`.
4. Havok: `havok_object_detach` (`0x1c37f0`). When `physics_active` is set,
   `0x146bf0` runs before it, and `0x278f00` and `0x146bf0` run after it.
5. `function_108bf0` (slot `0x3c`), then `0xbc300` frees the object.

### `0xb8540`: request an object's deletion

1. A hide sequence. When neither the object nor a parent is hidden
   (`object_or_parent_hidden`) and the root is connected, it calls
   `0xbef30(object, 1, 0, 0, 0)`. It sets header flag `0x10` and refreshes the
   partition record (`0xb8b70`).
2. `0x10a250(object)` for scenery, and `0xbf090` (object in `edi`).
3. Clears the scenario identity: unique id, origin bsp index and source become
   NONE, and the type is kept.
4. `0x15b220(object, +0xaf)` when `+0xaf` is not NONE.
5. `0x109400(object)`, `0xbb950` with the object in `eax` and (0, NONE) on the
   stack, `0xa7a60(object)` and `function_10ace0(object)`.

It does not free anything; `0xb8460` does. Every decompiled caller uses it to
get rid of an object: damage that destroys it (`src/damage.cpp`), a projectile
that is done (`src/projectiles.cpp`), an entity whose object goes away
(`src/unknown_09a9f0.cpp`), and a projectile that could not be placed
(`src/unknown_0a76b0.cpp`). Inside the range, `0xb7b40` (step 23), `0xb83b0`
and `0xb8600` call it.

### `0xb8600`: connect an object to the map

1. Without a location argument, it finds one from the bounding sphere's
   centre (`function_11bed0`), and if that has no cluster, from the position.
2. With a cluster, it stores the location at object `+0x28`, puts the cluster
   in the header and clears flag bit 18; without one it sets bit 18.
3. With flag bit 21, it builds a bit vector on the stack with every bit set,
   one bit per entry of `g_4e0348->list_count` (`+0x9c`).
4. Builds the partition record (`0xb88e0`) and calls `0x1cac60` with the bit
   vector or NULL in `eax`. Its stack arguments are, in order:
   - the partition, chosen by flag bit 9;
   - the object;
   - `&object +0x60`;
   - `&object +0x40`;
   - object `+0x4c`;
   - `&object +0x28`;
   - 0x14;
   - the record;
   - the address of a stack slot, which `0xb8600` does not read back.
5. Sets flag bit 8 and header flag `0x40`, and calls
   `0xbef30(object, 0, 1, 0, 0)`.
6. Activation. When `0xb6d60(object, &g_4e6948 +0x11b8)` is true it calls
   `0xb7290`, which activates the object. Otherwise, an object with bit 17
   that `0xa7670` does not keep is deleted (`0xb8540`), and anything else
   gets `0xb7300`, which deactivates it.
7. When the object had no cluster before and has one now, it calls
   `0x146bf0`, `0x1c36f0(object, object)`, `0x278f00` and `0x146bf0`.

Callers: `0xb6bb0`, `0xb7430`, `0xb93b0`, `0xb9a90`, `0xbdef0`,
`function_d4e60`, `function_d4ff0`, `0xe24f0`, `0x10a660`, `0x10cdf0`,
`0x11b520`, `0x1e55d0`, `0x29ffb0`, and `0xb7b40`.

### `0xb87b0`: disconnect an object from the map

`0xbef30(object, 1, 0, 0, 0)`, then
`function_1cae40(partition, object, &object +0x60)`, with the partition
chosen by flag bit 9 and passed in `edi`. That removes the object's data
references and its cluster references. Finally it clears flag bit 8 and header
flag `0x40`.

### `0xb8820`: the object globals are active

Returns true when `g_4de2f4` is not NULL and its first byte is set.
`0xb7b40` also makes the same test inline before step 19.

### `0xb8840` and `0xb8890`: the havok component off and on

When the object has a havok component, they pass its element of `g_51e9b8`
(0xa0 bytes) in `ecx`. `0xb8840` passes it to `0x1d1260` and then clears
`physics_active`. `0xb8890` passes it to `0x1d1540` and then sets it. Both
change the flag whether or not there is a component.

### `0xb88e0`: the partition record

Fills the 0x14-byte record shown above. Callers: `0xb8600`, `0xb8b70` and
`0x295e60`.

### `0xb8940`, `0xb89b0` and `0xb8a10`: objects in a cluster

`0xb8940(cluster, &record)` reads the cluster's first data reference from the
partition at `0x4de2e0` and returns its object index. It writes a pointer to
the reference's record and stores the next reference in the iterator.
`0xb89b0(&record)` continues from the iterator. `0xb8a10` is `0xb8940` for the
partition at `0x4de2d4`. All three return NONE at the end.

Callers: `0xbb050` uses `0xb8940` and `0xb8a10`. `0x168f40` and
`0x1697c0` use `0xb8940` and `0xb89b0`; `src/unknown_1689b0.cpp` has source
for `0x1697c0` without a marker.

### `0xb8a80` and `0xb8b20`: the clusters an object is in

`0xb8a80` walks up to the root object, since attached objects use their
root's cluster references: `0xb93b0`, which attaches an object to a parent's
node (`src/stubs/projectiles.cpp`), calls `0xb87b0` and `0xb8840` on it.
It stores the root's partition (chosen by flag bit 9) and its first cluster
reference (`+0x60`) in the iterator, and returns that reference's cluster
index. `0xb8b20` returns the next one. Both return NONE at the end, and only
the low 16 bits of `eax` are meaningful otherwise. Their one caller is
`0xbba80`.

### `0xb8b70`: refresh the partition record

Only when header flag `0x40` and flag bit 9 are set, it rebuilds the record
(`0xb88e0`) and calls `0x1cadf0`, with the partition at `0x4de2e0` in `edi`,
the first cluster reference in `eax`, the record in `ebx`, and
`(object, 0x14)` on the stack. Objects in the other partition are left alone.

## Existing declarations

| Retail | Declared as | Where | What retail shows |
| --- | --- | --- | --- |
| `0xb7740` | `function_b7740(long object_index, vector3f const *linear_velocity, vector3f const *angular_velocity, bool skip_update)` | `src/unknown_0b7740.cpp` | Agrees: `skip_update` suppresses the entity update bits. The source is `todo` |
| `0xb77d0` | `__stdcall function_b77d0(long object_index, union vector3f const *linear_velocity, union vector3f const *angular_velocity)` | `src/stubs/projectiles.cpp` | Roles agree |
| `0xb7880` | `__stdcall function_b7880(long object_index, long node_index, point3f const *point, union vector3f const *impulse, union vector3f const *angular_impulse)` | `src/stubs/damage.cpp`; also, without `union`, `src/damage.cpp`, `src/bipeds.cpp` and `src/unknown_175bd0.cpp` | Agrees: `0x1c4c50` copies the vector `angular_impulse` points to when it is not NULL. #28 changed it from a `bool`; the biped caller `0xde620` can pass a vector, and the others pass NULL |
| `0xb7930` | `function_b7930(void *data, long tag_index, long object_index, s_effect_owner const *owner)` | `src/stubs/lane_o.cpp`, `src/unknown_0b7300.cpp` | Roles agree |
| `0xb7b40` | `long function_b7b40(void *creation)` | `src/stubs/unknown_09a9f0.cpp`, `include/unknown_0a58d0.h` | `creation` is the placement data; the result is NONE on failure |
| `0xb83b0` | `__stdcall function_b83b0(long object_index, bool a)` | `src/stubs/unknown_0b7300.cpp` | `a` is never read |
| `0xb8460` | `__stdcall function_b8460(long object_index, bool a)` | `src/stubs/unknown_0b7300.cpp` | `a` is true for the object itself, false for its children |
| `0xb8540` | `__stdcall function_b8540(long a)` | `src/stubs/unknown_09a9f0.cpp`, `include/unknown_0a58d0.h` | `a` is the object index |
| `0xb8600` | `__stdcall function_b8600(long object_index, long unknown)` | `src/stubs/unknown_0b7300.cpp` | `unknown` is an `s_location const *`, or NULL |
| `0xb87b0` | `__stdcall function_b87b0(long object_index)` | `src/stubs/unknown_0b7300.cpp` | Agrees |
| `0xb8840`, `0xb8890` | `function_b8840(long unit_index)`, `__stdcall function_b8890(long unit_index)` | `src/stubs/unknown_0a76b0.cpp` | Any object: `0xb7b40` calls `0xb8890` for every new one |
| `0xb8940`, `0xb89b0` | `long function_b8940(short cluster_index, s_object_cluster_reference **reference, s_object_cluster_iterator *iterator)` and `0xb89b0` without the cluster | `src/unknown_1689b0.cpp` | Roles agree; the iterator is in `ebx` |
| `0xb8b70` | `function_b8b70(long object_index)` | `src/stubs/damage.cpp` | Agrees |

Eleven of the functions take register arguments (the table under
[Conventions](#conventions)). `0xb7880`, `0xb7a40`, `0xb7b40`, `0xb83b0`,
`0xb8460`, `0xb8540`, `0xb8600`, `0xb87b0` and `0xb8890` take all their
arguments on the stack, and `0xb8820` takes none.

Two more notes for whoever decompiles the range:

- `0xb7b40` inlines `function_d5c90` (step 17), and the same test as
  `0xb8820` (step 19).
- `0xb7b40`'s step 9 and the start of `0xb8540` look like a pair of inlined
  show and hide helpers.

## Evidence

- Every entry was disassembled from the retail XBE with `tools/disasm.py` at
  `8adcc51` (capstone 5.0.9). Conventions, offsets, masks, callers and the
  callback table at `0x468664` were read from retail code and data.
- Constants are read from retail `.rdata`: `0x45dbdc` is 0.0001 and
  `0x45dbc0` is 1.0. `g_4687a8` and `g_4687b0` point at (1, 0, 0) and
  (0, 0, 1).
- The type names and group tags are strings and values in retail's type
  definitions.
- Callees outside the range are described from their source in the
  repository where it exists, named above, and otherwise from their
  disassembly, only as far as this document needs them. `0xbc280`, `0xbc300`
  and `0xbc380` were read for the allocation and the blocks, `0xb7290` and
  `0xb7300` for header flag `0x01`, and `0x108a40`, `0x1c4b00` and
  `0x1c4c50` for their arguments.
- No emulator, runtime testing, SDK or outside dataset was used. Names are
  the repository's own, or describe behaviour.
