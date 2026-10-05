# Object core analysis, part 2 (unknown_0bc190)

Retail range covered: `0xbc190`–`0xbf8e8`. These are 51 inventory entries,
13,804 retail bytes: the object memory, the per-tick update and the
post-physics pass, the node matrices and the bounding sphere, the export
function of the base object type, change colours, attachments, the scenario
object names, garbage collection, and a few small queries. **Analysis only:**
this document adds no source, and nothing in it has been built or checked
against retail with the original compiler. Names are provisional. The
`function_<va>` form is primary; the descriptions are offered for whoever
decompiles the range.

It continues [the first part](unknown_0b7740.md) (`0xb7740`–`0xb8bc5`) and
uses its names for the object header, the object fields and the placement
data. Several functions here are steps of that part's `0xb7b40`.

## Boundary

- `0xbc100` and `0xbc150`, just before the range, have source in
  `src/unknown_0bbf40.cpp`: `0xbc150` is matched and `0xbc100` is `todo`.
  `0xbf8f0` and `0xbf950`, just after it, are matched in
  `src/unknown_0bf8f0.cpp`. All four are excluded.
- Lane A's row of the Active claims table (issue #9) lists `0xbc100`,
  `0xbf8f0` and `0xbf950`, but none of the 51 entries here, and no other row
  covers the range. No open pull request other than #47, which refers to some
  of these functions as callees, touches the range.
- Every entry in the range is `todo` and has no `@retail` marker, at `8adcc51`
  and on `main` at `b11793f`. Fourteen of them are declared or have `@stub`
  definitions in other files; see
  [Existing declarations](#existing-declarations).
- Seven entries have no direct callers. They are reached through the tables in
  [Pointer tables](#pointer-tables).

## Conventions

"Callers" counts direct calls from outside the range, then calls from inside
it. Calls through pointers are not counted. Results are in `eax` (`al` for a
bool) unless the table names another register.

| Retail | Bytes | Arguments | Returns | Callers | What it does |
| --- | --- | --- | --- | --- | --- |
| `0xbc190` | 50 | `eax` object, `ecx` owner out (`ret`) | | 4 | Copies the object's owner |
| `0xbc1d0` | 171 | `edi` object, `ecx` point; stack: object to ignore (`ret 4`) | bool | 3 | Moves the object to where a ray from the point to it is blocked |
| `0xbc280` | 127 | stack: size (`ret 4`) | object index, or NONE | 1 | Allocates an object's header and memory |
| `0xbc300` | 118 | stack: object (`ret 4`) | | 3 | Frees an object's memory and header |
| `0xbc380` | 230 | `eax` object; stack: field, size, alignment bits (`ret 0xc`) | bool | 3 | Appends a zeroed block to an object |
| `0xbc470` | 362 | stack: object (`ret 4`) | bool | 2 + itself | Updates an awake object and its children |
| `0xbc5e0` | 454 | stack: object (`ret 4`) | | 0 + 1 | Copies the havok component's motion to the object |
| `0xbc7b0` | 107 | stack: object (`ret 4`) | | 1 + itself | Wakes the bipeds, vehicles and creatures in a subtree |
| `0xbc820` | 330 | stack: object (`ret 4`) | | 2 + itself | The post-physics pass for an object and its children |
| `0xbc970` | 246 | `ecx` object, `eax` id; stack: value out (`ret 4`) | bool | 0 + 1 | Export values from the object's bytes at `+0x24` |
| `0xbca70` | 86 | none | float in `xmm0` | 0 + 1 | A value from the time at `+0x14` of the object globals |
| `0xbcad0` | 64 | none | float in `xmm0` | 0 + 1 | The local hour, as a fraction of 12 |
| `0xbcb10` | 66 | none | float in `xmm0` | 0 + 1 | The local minute, as a fraction of 60 |
| `0xbcb60` | 66 | none | float in `xmm0` | 0 + 1 | The local second, as a fraction of 60 |
| `0xbcbb0` | 108 | none | float in `xmm0` | 0 + 1 | A value for the holiday |
| `0xbcc20` | 1023 | stack: object, id, value out, active out (`ret 0x10`) | bool | 0 | The base type's export function (handler slot `0x4c`) |
| `0xbd020` | 108 | stack: object (`ret 4`) | | 16 + itself | Rebuilds the node matrices and bounding spheres of an object and its children |
| `0xbd090` | 1394 | stack: object (`ret 4`) | | 0 + 3 | Builds the object's node matrices |
| `0xbd610` | 856 | stack: object, node list, matrices (`ret 0xc`) | matrices | 1 | The node matrices to render the object with |
| `0xbd970` | 482 | stack: object, render model, animation state, node mask, node count, orientations (`ret 0x18`) | | 1 + 1 | Applies the animation graph's function-driven animations |
| `0xbdb60` | 215 | `ecx` node mask or NULL, `eax` node count, `edx` object; stack: render model, animation state, orientations (`ret 0xc`) | | 0 + 2 | Samples the object's animation into node orientations |
| `0xbdc40` | 684 | `eax` up, `ecx` forward, `bl` parent mirrored; stack: position, scale, mirrored, parent matrix or NULL, matrix out (`ret 0x14`) | | 1 + 1 | Builds a root node matrix |
| `0xbdef0` | 736 | stack: object (`ret 4`) | bool | 0 + 2 | Recomputes the bounding sphere |
| `0xbe1d0` | 104 | `eax` object (`ret`) | | 1 + 1 | Gives the type handlers the node matrices (slot `0x74`) |
| `0xbe240` | 1034 | stack: object, flags, colours (`ret 0xc`) | | 3 | Sets the change colours |
| `0xbe650` | 60 | `edx` list head, `edi` object (`ret`) | | 1 | Unlinks an object from a sibling list |
| `0xbe690` | 64 | `esi` object (`ret`) | | 1 | Creates a new object's widgets and attachments |
| `0xbe6d0` | 134 | `edx` object, `ecx` attachment index (`ret`) | float in `xmm0` | 1 | An attachment's function value |
| `0xbe760` | 333 | stack: object (`ret 4`) | | 0 | Creates the model variant's objects (handler slot `0x38`) |
| `0xbe8b0` | 40 | stack: object, id (`ret 8`) | float in `st(0)` | 0 | A function value, or 0 |
| `0xbe8e0` | 580 | stack: object (`ret 4`) | bool | 1 + 1 | Computes the current change colours |
| `0xbeb30` | 310 | stack: object (`ret 4`) | bool | 7 + itself | Whether the object is tied to a player |
| `0xbec70` | 39 | `eax` index (`ret`) | bool | 2 | Stamps an entry of `g_4de300` |
| `0xbeca0` | 440 | stack: object (`ret 4`) | | 0 + 1 | Creates the definition's attachments |
| `0xbee60` | 204 | `ebx` object (`ret`) | | 1 | Deletes the attachments |
| `0xbef30` | 281 | stack: object, remove, add, siblings, own flags only (`ret 0x14`) | | 17 + 1 + itself | Removes or adds the light attachments of an object and its children |
| `0xbf050` | 54 | `ecx` name index, `edx` object (`ret`) | | 0 + 1 | Records a named object |
| `0xbf090` | 93 | `edi` object (`ret`) | | 2 | Forgets an object's name |
| `0xbf0f0` | 207 | `eax` scenario datum; stack: type, index, palette, check, flag (`ret 0x14`) | object index, or NONE | 4 | Creates an object from a scenario datum |
| `0xbf1c0` | 125 | `eax` level, `esi` critical out (`ret`) | bool | 0 + 1 | Whether garbage collection goes on |
| `0xbf240` | 122 | stack: level, buffer, size (`ret 0xc`) | | 0 | Gathers the collectable objects |
| `0xbf2c0` | 159 | stack: level, buffer, size, again out, unused, unused (`ret 0x18`) | bool | 0 | Deletes one collectable object |
| `0xbf360` | 18 | stack: six arguments, unused (`ret 0x18`) | 0 | 0 | Compacts the object memory |
| `0xbf380` | 393 | none | | 5 | Collects garbage |
| `0xbf510` | 142 | `edi` location out; stack: object (`ret 4`) | bool | 2 | Finds the object's location |
| `0xbf5a0` | 37 | `eax` object (`ret`) | bool | 1 + 1 | Whether the object has an animation state |
| `0xbf5d0` | 37 | `eax` object (`ret`) | bool | 2 | Whether the object has its own node orientations |
| `0xbf600` | 344 | stack: object, frame, event (`ret 0xc`) | | 0 | The animation event callback: sounds and effects |
| `0xbf760` | 195 | `edi` scenario object identifier (`ret`) | object index, or NONE | 2 | Finds the object made from a scenario object |
| `0xbf830` | 83 | `eax` object, `xmm0` shield; stack: 16-bit value (`ret 4`) | | 1 | Sets `shield_vitality` and the 16 bits at `+0x104` |
| `0xbf890` | 89 | `ebx` object; stack: region bytes (`ret 4`) | | 1 | Passes one byte per region to `0xba6f0` |

The functions that touch objects reach them through `g_4e0300->data`
(`+0x44`) with 12-byte headers, as in the first part.

## Data

### Type definitions and handler slots

Each of the 13 type definitions in `g_468630` has at `+0x84` a NULL-terminated
list of the definitions whose handlers run for its objects.
`src/unknown_108a90.cpp` calls the list `handlers`
(`s_object_type_definition`) and names the slots of each entry
(`s_object_handlers`). Every list starts with the definition of the abstract
type 'object' and ends with the type's own definition. The lists of units,
items and devices have the abstract 'unit', 'item' or 'device' definition in
between. The names and groups are retail strings:

| Definition | Name | Group | Object size | In the lists of |
| --- | --- | --- | --- | --- |
| `0x4678e8` | object | `obje` | 0x12c | every type |
| `0x4679b0` | unit | `unit` | 0x348 | bipeds and vehicles |
| `0x467c08` | item | `item` | 0x16c | weapons, equipment and garbage |
| `0x468248` | device | `devi` | 0x1cc | machines, controls and light fixtures |

The sizes are where the next level's fields start: every object has the
0x12c bytes of the first part's tables, and a weapon's own fields start at
`+0x16c`.

The 'object' definition fills three handler slots:

| Slot | Function |
| --- | --- |
| `+0x38` | `0xbe760`, which creates the model variant's objects. `function_108b80` runs this slot near the end of creation (step 20 of `0xb7b40` in the first part) |
| `+0x4c` | `0xbcc20`, the export function |
| `+0x80` | `0x72c70`, outside the range |

`function_108d90` calls slot `+0x4c` along the list until a handler returns
true. The 'object' definition comes first in every list, so `0xbcc20` answers
before any type's own handler. For devices the next one is
`device_export_function` (`0x106d70`, `src/devices.cpp`), which has the same
form.

The other dispatchers this range calls are `function_108c60` (slot `0x40`),
`function_108cd0` (`0x44`), `function_108b10` (`0x30`), `function_109050`
(`0x6c`), `function_1090d0` (`0x70`) and `function_109140` (`0x74`), in
`src/unknown_108a90.cpp` and `src/unknown_108fd0.cpp`.

### The object header

Additions to the header flags in the first part:

| Flag | Meaning |
| --- | --- |
| `0x02` | awake. `function_b7360` sets it on the object and its ancestors. `0xbc470` updates only awake objects, and clears the flag when nothing kept the object awake |
| `0x04` | woken during this tick. `0xbba20` sets it, after `function_b7360`, on an active object (`0x01`). `0xbc470` clears it before the update and keeps the object awake when it is set again by the end. The machine update in `src/unknown_0b9fc0.cpp` tests it on the machine's parents |
| `0x20` | `0xbc470` returns true at once, without updating the object |
| `0x80` | `0xbba20` wakes the root object (`function_baf80`) instead |

### Object fields

Additions to the object fields in the first part:

| Offset | Meaning |
| --- | --- |
| `+0x18` | the parent's node the object is attached to (8 bits); `0xbd090` takes the parent's matrix for it |
| `+0x1c` | the next object in the collectable list, whose head is at `+0x8` of the object globals (`g_4de2f4`) |
| `+0x20` | the game time from which garbage collection may delete the object (`0xbf2c0`) |
| `+0x24`–`+0x26` | export values (`0xbc970`); the bytes come from placement data `+0xa8` |
| `+0x30`, `+0x3c` | the definition's bounding sphere in the world: centre and radius (`0xbdef0`) |
| `+0x50`, `+0x5c` | a second sphere from the definition's `+0x20` and `+0x24` (`0xbdef0`) |
| `+0xac` | the scenario object name index, or NONE; `g_4de2d0` maps it back (`0xbf050`, `0xbf090`) |
| `+0xb1` | the model variant index, or NONE |
| `+0xb3` | the awake countdown: while it is positive, `0xbc470` decrements it and keeps the object awake |
| `+0xc0` | bit 2: `0xbc820` moves it into bit 1. It makes `0xbc470` call `0xbba20` |
| `+0xc1` | bit 0 `physics_disabled`: `0xbc5e0` sets it when no rigid body is active. Bit 3: `0xbc5e0` calls `0x1d96d0` and clears it |
| `+0xc2`, `+0xc4`, `+0xc8` | the owner. `0xbc190` copies it into an `s_damage_owner` (`src/damage.cpp`), whose names for the three are `team`, `player_index` and `object_index` |
| `+0xd8` | bit 0: with a simulation entity, `0xbd090` asks `0xa98b0` and `0xa9940` for the position and orientation; in game options mode 4 or 5, `0xbc470` calls `0xa9570` |
| `+0xf4`, `+0xf8` | two floats after `shield_vitality`; `0xbcc20` exports them |
| `+0x104` | 16 bits, set by `0xbf830` |
| `+0x10a` | bit 2 makes the export id `0x0500055b` 0 |

| Flag bit | Meaning |
| --- | --- |
| 4 | the object has light attachments (`0xbeca0`); `0xbef30` removes and adds them, `0xbee60` clears the bit |
| 5 | the object has looping sound attachments (`0xbeca0`); `0xbee60` clears the bit |
| 6 | `0xbef30` calls `0xc1720` |
| 26 | set on the objects `0xbe760` creates for the model variant |
| 29 | `0xbd090` and `0xbd610` do not sample the animation |
| 30 | the last update did not sample the nodes in the model's node set at `+0xa4`; see `0xbd090` and `0xbd610` |

### Variable-length blocks

The last argument of `0xbc380` is a power of two: `0xb7b40` passes 4 for the
two 16-byte aligned blocks and 0 for the others. Additions to the block table
in the first part:

| Block | Contents |
| --- | --- |
| `+0x10c` | blended orientations: `0xbdb60` passes the block to `function_1d9470` (`s_blend_orientation`) |
| `+0x110` | the node orientations, 32 bytes per node (`rigid_transform_scaled` in `render_model_get_default_orientations`). `0xbd090` uses a static buffer at `0x4dc2f0` for an object without them |
| `+0x11c` | the attachment slots, `{char kind; long index}`, one per entry of the definition's attachment block; see [Attachments](#attachments) |
| `+0x124` | the change colours: for `size / 0x18` colours, the base colours that `0xbe240` sets, then the current colours that `0xbe8e0` computes from them |

### Attachments

The object definition's attachment block has its count at `+0x94` and
0x18-byte elements at `+0x98`. An element holds a tag reference (group at
`+0x0`, index at `+0x4`), 16 bits at `+0xc`, and values at `+0x10` and
`+0x14` that go to the creators. `0xbe6d0` reads `+0x10` as a function id.

`0xbeca0` gives each slot its kind from the tag group, and the creator's
result as the index:

| Kind | Groups | Created by | Flag bit | Deleted by `0xbee60` with |
| --- | --- | --- | --- | --- |
| 0 | `ligh` | `0xc0230(tag, object, attachment index, +0x10, +0xc)` | 4 | `0xc3260(index, 1)` |
| 1 | `lsnd` | `looping_sound_new_attached` (`0x188f90`): tag, object, attachment index, `+0x10` | 5 | `record_pool_release` on `g_4ed28c` |
| 2 | `effe` | `function_1766b0(object, tag, +0x10, +0x14, +0xc)` | | `function_177610` |
| 3 | `cont` | `function_17bab0(object, attachment index, tag)` | | `contrail_update(index, true, 0.0)`, after `0xbd090` |
| 5 | `lens`, `MGS2`, `tdtl` | not created here; the index is NONE | | |

Any other group, or a tag index of NONE, gives kind NONE. The flag bit is set
only when the creator returns an index. `0xbef30` passes the kind 0 indices to
`0xc3220` and `0xc2d00`.

### Garbage collection

`0xbf380` walks the table at `0x440570`. Each 16-byte entry holds a mask of
levels, a flag that limits the entry to critical states, a gather function and
an act function. An entry of zeros at `0x4405c0` ends it.

| Entry | Levels | Critical only | Gather | Act |
| --- | --- | --- | --- | --- |
| `0x440570` | 3 | no | | `0xbf360`: compacts the object memory |
| `0x440580` | 0 to 3 | no | `0xbf240` | `0xbf2c0`: deletes collectable objects |
| `0x440590` | 3 | no | | `0xbf360` again |
| `0x4405a0` | 3 | yes | | `0x1c8560` |
| `0x4405b0` | 3 | yes | `ai_importance_list_build` (`0x1c8700`, `src/ai.cpp`) | `0x1c88c0` |

A gather function takes `(level, buffer, size)` and an act function
`(level, buffer, size, &again, &unused, 0x200)`; `ai_importance_list_build`
has the same form, with its first and last arguments unused.

### Pointer tables

| Function | Referenced from |
| --- | --- |
| `0xbcc20` | `0x467934`, slot `+0x4c` of the 'object' definition |
| `0xbe760` | `0x467920`, slot `+0x38` of the 'object' definition |
| `0xbe8b0` | `0x3ddd0`, which stores it at `+0x48` of a structure |
| `0xbf240`, `0xbf2c0`, `0xbf360` | the garbage collection table at `0x440570` |
| `0xbf600` | pushed as an argument 12 times: by `0xfdfb0`, `0x106790` (twice), `0x107cc0`, `0x10a2f0`, `0x111650` (four times), `function_1152e0` (twice) and `0x119380`. `function_1152e0`, the machine update in `src/unknown_0b9fc0.cpp`, passes it to `c_animation_channel::set_frame_ratio_and_advance` with the machine's index as the user |

## The functions

### `0xbc190`: the owner

Copies the object's owner into the 12 bytes that `ecx` points to: `+0xc4` to
`+0x0`, `+0xc8` to `+0x4`, and the 16 bits at `+0xc2` to `+0x8`. That is the
layout of `s_damage_owner` in `src/damage.cpp`: `player_index`,
`object_index` and `team`. The first part's placement data carries the same
12 bytes as an `s_effect_owner`.

Callers: `function_faa60`, `function_fc330`, `0x10b4d0` and `0x242660`.

### `0xbc1d0`: move an object back along a ray

1. It sets the collision result's `unknown24` to NONE and casts a ray with
   `0x1697c0(0x20800005, point, vector, object, ignored, &result)`. The
   vector runs from the point to the object's position (`+0x64`). The object
   itself and the stack argument are the two objects the ray ignores; the
   repository's declarations of `0x1697c0` (`src/damage.cpp`, `src/items.cpp`,
   `src/projectiles.cpp`) call them `ignore_object_index` and
   `ignore_unit_index`.
2. When the ray hits nothing and the object has a cluster (`+0x2c`), it
   returns true.
3. Otherwise, when the result's location (`+0x1c`) has a cluster, it moves the
   object to the result's point (`+0x8`) with
   `0xb75a0(object, &point, NULL, NULL, &location, false)`, in the order of
   its stub in `src/stubs/projectiles.cpp`. In retail the point is in `eax`,
   the two NULL vectors are in `ecx` and `edx`, and the object, the location
   and false are on the stack. It returns true. Else it returns false.

In `function_e7900` the object is the unit's `grenade_projectile_index`, read
at `0xe7933`, and the stack argument is the unit; when `0xbc1d0` fails, the
projectile is deleted (`0xb8540` at `0xe7a80`). In `function_fc330` the object
is the function's first argument, the projectile, and the stack argument is
its parent (`+0x14`). See [Existing declarations](#existing-declarations).

Callers: `function_ce6b0`, `function_e7900` and `function_fc330`.

### `0xbc280`: allocate an object

Takes a header from `g_4e0300` (`record_pool_allocate`), then the given number
of bytes from the loop allocator `g_4de2ec` (`loop_allocate`,
`src/loop_allocator.cpp`) into the header's object pointer (`+0x8`). It
stores the size in the header (`+0x6`), zeroes the memory and returns the
object index. When the allocator fails, it releases the header
(`record_pool_release`) and returns NONE.

Callers: `0xb7b40`.

### `0xbc300`: free an object

Clears the header's flags (`+0x2`). When the object has memory, it frees it
with an inlined `loop_free`: it adds the block's size to the allocator's
`free`, unlinks the block from the allocator's list (`first`, `last`) and
clears the header's object pointer. Then `record_pool_release` frees the
header.

Callers: `0xb6ab0`, `0xb7b40` and `0xb8460`.

### `0xbc380`: append a block

`0xbc380(object, field, size, bits)` takes the object in `eax`.

- A size of 0 stores size 0 and offset NONE at `field` and returns true.
- Otherwise it grows the object's memory by `size + (1 << bits) - 1` bytes
  with `loop_reallocate`, which can move the object, so callers read the
  object pointer from the header again. The block starts at the old size
  rounded up to a multiple of `1 << bits`. It stores the size at `field` and
  the offset at `field + 2`, adds the new bytes to the header's size, zeroes
  them and returns true.
- When `loop_reallocate` fails, it returns false and changes nothing.

Callers: `0xb7b40`, `function_c4410` and `0x118510`. `function_c4410` is slot
`+0x2c` of the 'unit' definition.

### `0xbc470`: update an awake object

`0xbc470(object)` returns whether the object should stay awake. Without
header flag `0x02` it returns false, and with `0x20` it returns true.
Otherwise:

1. It clears header flag `0x04`.
2. `0xbba20(object)` when `+0xc0` bit 2 is set, and `0xa9570(object)` when the
   game options' mode (`g_4e6948`, `+0xc`) is 4 or 5 and `+0xd8` bit 0 is
   set.
3. It collects activity from:
   - `s_animation_state::blend_counters_update` on the object's animation
     state, when it has one with a graph (`+0x68`);
   - `function_108c60`, handler slot `0x40`;
   - `function_d5de0` (`src/damage.cpp`);
   - `0xbe8e0`, the change colours;
   - `0xbc470` on each child.
4. With a havok component and `physics_active`, it calls `0x1ced00` with the
   component's element in `edi`.
5. While the countdown at `+0xb3` is positive, it decrements it and returns
   true.
6. Otherwise it clears header flag `0x02` when there was no activity and
   `0x04` is still clear, and returns the activity.

Callers: `0xb6f10`, `0xb7150`, and itself.

### `0xbc5e0`: copy the havok motion

Without `physics_active`, it only sets `physics_disabled` to the parent's, or
to true when there is no parent. Otherwise, with the component's element of
`g_51e9b8`:

1. `havok_component_any_rigid_body_active`. When a body is active, it clears
   bit 16 of the component's flags (`+0x4`).
2. When a body is active or `physics_disabled` is already set, and the
   component has rigid bodies:
   - with the component's `flag8`, or when
     `havok_component_main_rigid_body_movable` is false, it sets both
     velocities to zero through `function_b7740` (`*g_4687a4`);
   - otherwise `0x1d4360` reads the main body's transform into a 0x34-byte
     matrix. Unless the component has `flag1`, `0xb91d0` adjusts the matrix's
     forward and up for the object. Then
     `0xb7430(object, &position, forward, up, 0, 0, 0, 0, 1)` moves the
     object, with NULL forward and up under `flag1`. Last, `function_b7740`
     stores the body's linear velocity
     (`havok_component_rigid_body_linear_velocity_get`) and, without
     `flag1`, its angular velocity
     (`havok_component_rigid_body_angular_velocity_get`) when a body is
     active, and zero velocities otherwise.
3. Unless the game options' mode is 4, when `+0xc1` bit 3 is set it calls
   `0x1d96d0` with the component index and clears the bit.
4. `physics_disabled` becomes true when no body is active, false otherwise.

Callers: `0xbc820`.

### `0xbc7b0`: wake the units in a subtree

For a biped, vehicle or creature (type mask `0x1003`) it calls
`function_b7360(object)`, which sets header flag `0x02` on the object and its
ancestors. Then it does the same for each child. `0xbba20` calls it after it
wakes an object.

Callers: `0xbba20`, and itself.

### `0xbc820`: the post-physics pass

1. `0xbc5e0(object)` when the object has a havok component.
2. `0xbd090(object)`, except for machines. For a machine with
   `physics_active` whose havok component has bit 15 in its flags, it calls
   `0x1d3920(component, &mask, 0.0)` with a zeroed node mask, which it does
   not read afterwards. A machine's own update calls `0xbd020` when the
   machine moved (`src/unknown_0b9fc0.cpp`).
3. `function_108cd0`, handler slot `0x44`.
4. It moves `+0xc0` bit 2 into bit 1 and clears bit 2.
5. `0xbdef0(object)`. When it returns false and the object is connected
   (header flag `0x40`), it calls `0xbef30(object, 1, 1, 0, 0)`.
6. `0xbc820` on each child, then `0xbe1d0(object)`.

Callers: `0xb7060`, `0xb7150`, and itself.

### `0xbc970`: values from the bytes at `+0x24`

`0xbcc20` passes six export ids here. `0xbc970` returns false and 0 for an
object of NONE, and when the object's `+0x26` bit 3 is clear in a campaign
scenario: one whose type (`g_4e0350`, `+0x10`) is 0, as `src/loading.cpp`
reads it. Otherwise it returns true:

| Id | Value |
| --- | --- |
| `0x0d0005ff` | 1.0 |
| `0x170005fa` | the byte at `+0x24`, divided by 64 |
| `0x170005fb` | the byte at `+0x25`, divided by 32 |
| `0x180005fc` | 0 when `+0x26` bit 0 is set, else 1.0 |
| `0x1d0005fd` | 1.0 when `+0x26` bit 1 is set, else -1.0 |
| `0x1d0005fe` | 1.0 when `+0x26` bit 2 is set, else -1.0 |

Any other id gives true and 0.

Callers: `0xbcc20`.

### `0xbca70`, `0xbcad0`, `0xbcb10`, `0xbcb60` and `0xbcbb0`: clock values

Each returns a float in `xmm0`, and `0xbcc20` is the only caller of each.

- `0xbca70`: 1.0 when the time at `+0x14` of the object globals is 0, or
  `g_510c54->game_time` has not passed it. Otherwise, with s the seconds since
  that time (the ticks times `rate`), it returns 0 when s is more than 1, 1.0
  when s is more than 20, and else (s - 15) times 0.2. The test for 20 comes
  after the test for 1, so it never succeeds, and the last case is negative;
  `0xbcc20` clamps it to 0, so the export value is 1.0 before the time and 0
  after it.
- `0xbcad0`: the local hour from `function_139030`, clamped to 1 to 24, modulo
  12, divided by 12.
- `0xbcb10` and `0xbcb60`: the local minute and second, clamped to 0 to 59,
  divided by 60.
- `0xbcbb0`: the holiday from `function_139090`: 0.25 for
  `_holiday_christmas`, 0.5 for `_holiday_new_years_day`, 0.75 for
  `_holiday_halloween`, 1.0 for `_holiday_july_7`, and 0 otherwise.

### `0xbcc20`: the export function

`0xbcc20(object, id, &value, &active)` is slot `+0x4c` of the 'object'
definition. It returns true for the ids below, and false with 0 for any
other. The values from fields and helpers are clamped to 0 to 1. `active` is
the result of `0xbc970` for the ids that go there, and otherwise whether the
value is positive.

| Id | Value |
| --- | --- |
| `0x030005a6` | 1.0 |
| `0x040005a7` | 0 |
| `0x0500055b` | 1.0, or 0 when `+0x10a` bit 2 is set |
| `0x0700055c` | the heading of node 0's forward vector: `atan2` of its first component over its second, minus the scenario's angle at `+0x1c`, wrapped by `function_0bff60`, divided by 2 pi, plus 0.5. It is 1.0 when the vector's third component is at least 0.995 in size |
| `0x0700055d` | the variant index (`+0xb1`) plus 1, divided by the model's variant count (`+0x50`); 0 for a model without variants |
| `0x070006ba` | `0xbcbb0` |
| `0x090006b5` | `0xbca70` |
| `0x0a0006b6` | `0xbcad0` |
| `0x0a0006b9` | 1.0 |
| `0x0c0006b7` | `0xbcb10` |
| `0x0c0006b8` | `0xbcb60` |
| `0x0d000558` | `body_vitality` (`+0xec`) |
| `0x0d0005ff`, `0x170005fa`, `0x170005fb`, `0x180005fc`, `0x1d0005fd`, `0x1d0005fe` | `0xbc970` |
| `0x0f000559` | `shield_vitality` (`+0xf0`) |
| `0x0f0005aa` | a number from the object index: the index times `0x19660d` plus `0x3c6ef35f`, shifted right by 16, divided by 65535 |
| `0x0f0005ac` | 1.0 when `shield_vitality` is exactly 0, else 0. In game options state 2 (`+0x8`), a biped or vehicle with a `player_index` (`+0x13c`) also needs `function_1588b0(player, 2)` to be positive |
| `0x100006b4` | 1.0 |
| `0x13000556` | `+0xf8` |
| `0x130006bb` | `0xc18e0()` |
| `0x15000557` | `+0xf4` |
| `0x1800055a` | `shield_vitality` minus 1 |
| `0x1a00071a`, `0x1a00071b`, `0x1a00071c`, `0x1a00071d` | the object globals' floats at `+0x70`, `+0x74`, `+0x78` and `+0x7c` |

The ids are the same kind of values `device_export_function` compares
(`src/devices.cpp`).

### `0xbd020`: rebuild an object's matrices and spheres

`0xbd090(object)`, then `0xbdef0(object)`, then `0xbd020` on each child that
is not a machine (by the child's type at `+0xaa`).

Callers: `0xb7430`, `function_b7680`, `0xb7b40`, `0xb93b0`, `function_c42e0`,
`function_ea1f0`, `function_ea8e0`, `function_ec0d0`, `function_ee460`,
`vehicle_place`, `0x106790`, `function_107ed0`, `function_10a660`,
`function_1152e0`, `function_11b520`, `0x1d3fc0`, and itself.

### `0xbd090`: the node matrices

1. It clears flag bit 30. A biped whose byte at `+0x34b` is 2 keeps its
   matrices: the function returns.
2. It zeroes a node mask, one bit per node of the `+0x114` block. With
   `physics_active`, `0x1d3920(component, &mask, value)` marks the nodes the
   havok component drives; `value` is `function_e1670(object)` for a biped
   and 0.0 otherwise.
3. The orientations are the object's `+0x110` block, or the static buffer at
   `0x4dc2f0` for an object without one.
4. Without a model (definition `+0x38`) or a render model (model `+0x4`),
   node 0 comes from `0xbdc40` with the object's position, forward, up, scale
   and flag bit 10, and the parent's node matrix, and the function returns.
5. With a parent (`+0x14`), it takes the parent's matrix for node `+0x18`
   (`function_b8bd0`) and the parent's flag bit 10.
6. Unless flag bit 29 is set, or the object is a biped whose byte at `+0x34b`
   is not 0:
   - `render_model_get_default_orientations` fills the orientations;
   - when the object has an animation state (`0xbf5a0`) with a graph, and
     `0xbfc90(object)` is false, `0xbdb60` samples the animation into them.
     When the object has its own orientations and the model's node set at
     `+0xa4` is not empty (`s_bit_vector::is_empty`), only the nodes outside
     that set are sampled. The mask is the set's complement
     (`function_0bfe80`) cut to the node count (`function_0bfed0`), in a
     static at `0x55eeb4`. Flag bit 30 records whether such a mask was used
     (`function_0bfe20`);
   - when `function_0b6780` accepts the state, it keeps bit 1 of the byte at
     `+0x16` of the state's animation (`c_animation_channel::function_1c6440`)
     for step 8.
7. A render model without nodes (`+0x48`) gets the identity orientation
   (`*g_4687d8`).
8. Node 0, unless the havok component drives it: with the bit from step 6, it
   is orientation 0 as a matrix (`function_1421f0`). Otherwise it is the root
   matrix from `0xbdc40` times orientation 0 (`function_142a60`). The root
   matrix uses the position (`+0x64`), forward (`+0x70`) and up (`+0x7c`),
   except that for an object with a simulation entity and `+0xd8` bit 0,
   `0xa98b0` and `0xa9940` can supply them. Then
   `function_1090d0(object, matrices)` runs handler slot `0x70`.
9. Every other node that the havok component does not drive gets its
   parent's matrix times its orientation. The render model's nodes are 0x60
   bytes each at `+0x4c`, with the parent at `+0x4`.

Callers: `0xbc820`, `0xbd020` and `0xbee60`.

### `0xbd610`: the matrices to render with

`0xbd610(object, node list, matrices)` returns the matrices to render the
object with, usually `matrices` itself:

1. It returns `matrices` at once when the object has no `+0x110` block, when
   its countdown `+0xb3` is positive, or when it has no model.
2. When flag bit 30 is set, and neither flag bit 29 nor a biped's nonzero
   byte at `+0x34b` stops it: when a node in the list is in the model's node
   set at `+0xc4` (`function_0bfe60`), it samples the nodes of the set at
   `+0xa4` (`0xbdb60`), clears bit 30, and rebuilds those nodes' matrices.
   The list has one byte per entry of the render model's count at `+0x1c`,
   0xff for none.
3. For a biped or vehicle for which `function_10db60` or `function_10edd0`
   is true, it copies the orientations to `0x4dc2f0` and the matrices to
   `0x4d8f20`, lets `0x10ec00` and `0x10e530` change the copied orientations,
   rebuilds the copied matrices of the nodes in the set at `+0xa4`, and
   returns `0x4d8f20`.
4. For a biped or vehicle it then calls `0x1132f0` with the set at `+0xa4`,
   the node count and the result.

Callers: `0x137800`, which passes its own first two arguments.

### `0xbd970`: function-driven animations

`0xbd970(object, render model, state, node mask, node count, orientations)`
goes through the entries of the animation graph, the tag at the state's
`+0x68` (count `+0x44`, 20-byte entries at `+0x48`). For each entry with a
function id at `+0xc`, it starts a local channel with the entry's animation
(`+0x4`, `s_animation_state::channel_start`, after `function_1dd9d0`) and
reads the function's value (`0xbab40`). Then, by the 16 bits at `+0xa`:

- 0: the frame is the value times the animation's last frame index (frame
  count `+0x14` minus 1), sampled with weight 1.0.
- 1: a frame that loops with the game time, offset by the object index,
  sampled with the value as the weight.

Sampling is `c_animation_channel::set_frame_position`, then
`c_animation_channel::sample(weight, node mask, node count, orientations)`.
Other values are skipped.

Callers: `function_16760c` and `0xbdb60`.

### `0xbdb60`: sample the animation

1. With a node mask, the node count is cut to the mask's highest set bit plus
   one (`bit_vector_highest_set_bit`). A count of 0 returns.
2. When the state has a graph (`+0x68`) and the 32 bits at `+0x0` and the 16
   bits at `+0x6` are not NONE, it calls `function_1daea0` and then
   `s_animation_state::sample(state, render model, 1.0, mask, orientations,
   0, object)`.
3. `0xbd970(object, render model, state, mask, count, orientations)`.
4. `function_109050(object, mask, count, orientations)`, handler slot `0x6c`.
5. When the state's byte at `+0x65` is set and bit 1 of its byte at `+0x67`
   is clear, `function_1d9470` blends with the `+0x10c` block: the state's
   `+0x64` in `eax`, the count in `ebx`, the orientations in `ecx`, and the
   mask and the block on the stack.

Callers: `0xbd090` and `0xbd610`.

### `0xbdc40`: a root matrix

`0xbdc40` builds a rotation from the forward vector, the up vector, and the
cross product of up and forward as the left vector, which it negates when
`mirrored` is set. When `scale` is not 1.0, the rotation is multiplied by a
uniform scale. The result is the translation to `position` times the
rotation. With a parent matrix, the parent comes first. When the parent's
scale is not 1.0, the position is multiplied by it and a copy of the parent
gets scale 1.0. When `bl` (the parent's mirrored flag) is set, the copy's left
vector is negated. The products use `function_142a60`.

Callers: `0xbfa40` and `0xbd090`.

### `0xbdef0`: the bounding sphere

1. For a crate with `physics_active`,
   `0x1d44f0(component, &+0x30, &+0x3c)` can supply the first sphere.
   Otherwise `+0x30` is the definition's sphere centre (`+0x8`) through node
   0's matrix, scaled by the node's scale when it is not 1.0, and `+0x3c` is
   the definition's radius (`+0x4`) times the object's scale (`+0xa0`).
2. When the definition's radius at `+0x20` is positive, `+0x50` is `+0x30`
   moved by the definition's offset at `+0x24`, and `+0x5c` is that radius
   times the scale. Otherwise they copy `+0x30` and `+0x3c`.
3. The new bounding sphere is `+0x50` and `+0x5c` when the definition's flags
   (`+0x2`) have bit 12, else `+0x30` and `+0x3c`.
4. A connected object (flag bit 8) whose centre moved no more than 0.1 and
   whose radius is the same keeps its sphere, and the result is false. Any
   other object stores the sphere at `+0x40` and `+0x4c` and the result is
   true; a connected one is disconnected first (`0xb87b0`) and connected again
   after (`0xb8600(object, NULL)`).

Callers: `0xbc820` and `0xbd020`.

### `0xbe1d0`: handler slot `0x74`

When the definition's model has a render model (`+0x4`) and an animation graph
(`+0x14`), it calls `function_109140(object, count, matrices)` with the node
matrices and their count from `function_b8c00`.

Callers: `0xb7b40` and `0xbc820`.

### `0xbe240`: set the change colours

`0xbe240(object, flags, colours)` fills the `+0x124` block. For each of the
definition's change colours (`+0xac` count, 16-byte entries at `+0xb0`):

- When bit i of `flags` is set, colour i is `colours[i]`.
- Otherwise it is white (`*g_468710`), unless the entry has permutations
  (count `+0x0`, 32-byte elements at `+0x4`: a weight at `+0x0`, two colours
  at `+0x4` and `+0x10`, and a variant name at `+0x1c`). Then it chooses
  among the permutations without a name or with the name of the object's
  variant (the first field of the model's variant `+0xb1`, 0x38-byte
  elements at `+0x54`). A number in 0 to 1 made from the object's position
  (`+0x64`) and i picks one by weight, and a second such number blends its two
  colours in HSV (`function_1318d0`, `function_131a00`), the shorter way
  around the hue.
- The colour is clamped to 0 to 1 and stored in both halves of the block.

Last, it calls `0x3dd10(object, 1)`.

Callers: `0xb7b40`, `0x14bfc0` and `territory_update_item_colors`.

### `0xbe650`: unlink from a sibling list

`edx` points at the head of a list linked through `+0xc`, such as a parent's
`+0x10`. It follows the list to the object in `edi`, unlinks it, and sets its
`+0xc` to NONE.

Callers: `0xb9890`.

### `0xbe690`: widgets and attachments

For a projectile it sets `+0xdc` to NONE; for any other type it calls
`object_widgets_new(object)`. Then `0xbeca0(object)`.

Callers: `0xb7b40`.

### `0xbe6d0`: an attachment's function value

Returns the value of the function whose id is at `+0x10` of the definition's
attachment `ecx` (`0xbab40`), or 1.0 when the id is 0. When the definition's
flags (`+0x2`) have bit 9, the value is multiplied by the object's scale.

Callers: `0x41980`.

### `0xbe760`: the model variant's objects

When the object has a variant (`+0xb1`), it creates each object the variant
lists: model `+0x54`, 0x38-byte variants, each with a count at `+0x1c` and
16-byte elements at `+0x20` whose tag index is at `+0xc`. For each tag:

1. `0xb7930(&placement, tag, object, &owner)` with the object's owner
   (`+0xc4`, `+0xc8`, `+0xc2`), and `function_b9dd0(object, &placement +0x1c)`
   for the position.
2. `0xb7b40(&placement)`.
3. When that succeeds, `0xb8ee0(object, element +0x0, new object)`, with the
   element's `+0x4` in `eax`. The repository's stub of `0xb8ee0` calls its
   arguments `parent_index`, `marker_name`, `object_index` and `a`. It sets
   flag bit 26 on the new object.

Callers: none; see [Pointer tables](#pointer-tables).

### `0xbe8b0`: a function value

`0xbe8b0(object, id)` returns the value `0xbab40` gives for the id, or 0 when
it fails, in `st(0)`.

Callers: none; see [Pointer tables](#pointer-tables).

### `0xbe8e0`: the current change colours

For a definition whose byte at `+0x1c` has bit 0, and an object with change
colours, it returns true; otherwise false. For each colour i:

1. The current colour (second half) starts as the base colour (first half).
2. Each of the entry's functions (count `+0x8`, 0x28-byte elements at `+0xc`)
   with an id at `+0x24` sets it to
   `function_131c20(colour +0x8, colour +0x14, flags +0x4, value)`, with the
   value from `0xbab40`, or 0 when that fails.
3. Each with an id at `+0x20` multiplies it by that function's value.
4. The colour is clamped to 0 to 1.

Because the result is true, `0xbc470` keeps such objects awake.

Callers: `0xb7b40` and `0xbc470`.

### `0xbeb30`: tied to a player

True when any of these holds:

- the object is a biped or vehicle (`function_badc0(object, 3)`) with a
  `player_index` (`+0x13c`);
- `0xbeb30` is true for a child;
- an ancestor is a biped or vehicle with a `player_index`;
- the object is a weapon, equipment or garbage (mask `0x1c`) whose byte at
  `+0x12c` has bit 3.

Callers: `function_d4e60`, `function_1fb510`, `0x2736c0`, `function_29fd80`,
`function_29fdd0`, `0x29fe10`, `function_2a14d0`, and itself.

### `0xbec70`: once per stamp

When `g_4de300[index]` differs from `g_4de2fc`, it stores `g_4de2fc` there and
returns true; otherwise it returns false. So it is true once per index each
time `g_4de2fc` changes.

Callers: `0x133050` and `0x269e60`.

### `0xbeca0`: create the attachments

For each entry of the definition's attachment block, it sets the slot's kind
from the tag group and creates the attachment, as
[Attachments](#attachments) shows. The slot's index is the creator's result,
or NONE.

Callers: `0xbe690`.

### `0xbee60`: delete the attachments

For each slot of kind 0 to 3 with an index, it deletes the attachment as
[Attachments](#attachments) shows, then sets the slot's kind and index to
NONE. It rebuilds the object's matrices (`0xbd090`) before it detaches a
contrail. Last it clears flag bits 4 and 5.

Callers: `0xb8460`.

### `0xbef30`: the light attachments

`0xbef30(object, remove, add, siblings, own flags only)`:

1. It returns at once for a hidden object: with `own flags only`, when the
   object has header flag `0x10` or flag bit 0; otherwise when
   `object_or_parent_hidden` is true. The siblings are then not visited
   either.
2. With flag bit 4, for each light slot with an index: `0xc3220`, with the
   index in `eax`, when removing, and `0xc2d00(index)` when adding.
3. With flag bit 6, `0xc1720(object, remove, add)`.
4. `0xbef30(first child, remove, add, true, true)`.
5. With `siblings`, the same for the next sibling (`+0xc`), as a loop.

The first part's callers pass (1, 0, 0, 0) to remove and (0, 1, 0, 0) to add.
`0xbc820` passes (1, 1, 0, 0) when `0xbdef0` did not reconnect a connected
object.

Callers: `0xb7b40`, `0xb8540`, `0xb8600`, `0xb87b0`, `0xb93b0`, `0xb9a90`,
`0xb9c60`, `function_cc590`, `function_da110`, `function_ea8e0`,
`function_f8eb0`, `0x10ca80`, `function_10cd50`, `0x10cdf0`, `0x10cec0`,
`0x14ee20`, `0x14f2e0`, `0xbc820`, and itself.

### `0xbf050` and `0xbf090`: object names

`g_4de2d0` holds an object index for each of the scenario's object names, as
many as the scenario's count at `+0x48`. `src/unknown_025600.cpp` reads the
names themselves at `+0x4c`.

- `0xbf050` records the object for a name index when the entry is NONE, and
  then stores the index at `+0xac`. `0xbf0f0` calls it.
- When `+0xac` is set, `0xbf090` sets it to NONE and clears every entry that
  holds the object. `0xb8540` and `0xbb670` call it.

### `0xbf0f0`: create a scenario object

1. It returns NONE when the datum's `palette_index` is NONE, or when its
   `name_index` (0 to 0x27f) already has an object in `g_4de2d0`.
2. `function_d5060(type, index, flag, datum, palette, &placement)` fills the
   placement data; NONE on failure. With `check`, `0xa7640(&placement)` must
   also be true.
3. `0xb7b40(&placement)`. When it succeeds: `function_108b10(object, datum)`,
   handler slot `0x30`; `0xbf050` for the name; and `0xa7870(object)`.
4. It returns the object index, or NONE.

Callers: `0xa73b0`, `0xbb670`, `function_d5560` and `function_d5640`.

### `0xbf380`: collect garbage

1. The level:
   - 0 or 1 when collection was requested (the object globals' `+0x1`): 0
     when `+0x2` is also set, else 1;
   - 3 when the bytes after the last block of `g_4de2ec` are at most 0xcccc,
     or at most 0x66 headers are free (0x800 minus `g_4e0300`'s
     `actual_count`);
   - 2 when the 16 bits at `+0x4` of the object globals, the active objects
     with flag bit 14 (first part), are at least 0x32;
   - otherwise it does nothing.
2. For each table entry, while `0xbf1c0` says to go on: when the entry's mask
   has the level, and the entry is not limited to critical states or the
   state is critical, it calls the gather function once and then the act
   function until the act function leaves `again` false. The buffer is
   0x2800 bytes on the stack.
3. It clears `+0x1` and `+0x2`.

Callers: `0xb7150`, `function_d5560`, `function_d5640`, `0x203780` and
`function_2a0580`.

### `0xbf1c0`: go on?

- Levels 0 and 1: true, not critical.
- Level 2: true while the count at `+0x4` is more than 0x1e; not critical.
- Level 3: true while the bytes after the last block are under 0x19999 or
  the free headers (`maximum_count` minus `high_water_index`) under 0xcc.
  Critical while they are under 0x6666 or 0x33.

Callers: `0xbf380`.

### `0xbf240`, `0xbf2c0` and `0xbf360`: the table's functions

- `0xbf240` stores a count and up to 0x800 object indices in the buffer: the
  objects of the collectable list without a parent, leaving out those with a
  simulation entity in game options mode 4.
- `0xbf2c0` takes the last index. It deletes the object when the game time has
  reached its `+0x20`; at level 2 only when it is active (header flag `0x01`);
  and at any level but 0 only when `0xbba80` is false. `0xbba80` is the caller
  of the first part's `0xb8a80` and `function_b8b20`. The deletion is
  `function_bb950(object, false, NONE)`, `0xb83b0(object, true)` and
  `0xb8460(object, true)`. It returns whether it deleted, and sets `again`
  while indices remain.
- `0xbf360` calls `loop_compact` on `g_4de2ec` and returns 0.

None of them has a direct caller; see [Pointer tables](#pointer-tables).

### `0xbf510`: an object's location

Fills the `s_location` in `edi` with `function_11bed0` for the bounding
sphere's centre (`+0x40`). When that has no cluster, `0x1dde10` looks for
what the bounding sphere touches in `g_4e0340`. When it finds something,
`0x11be90` makes the location from the first result; otherwise
`function_11bed0` uses the position (`+0x64`). It returns whether the location
has a cluster. `0xb7b40` uses it for the placement's location (first part,
step 19).

Callers: `0xb6bb0` and `0xb7b40`.

### `0xbf5a0` and `0xbf5d0`: block tests

`0xbf5a0` returns whether the object has an animation state (`+0x12a` is not
NONE), and `0xbf5d0` whether it has its own orientations (`+0x112` is not
NONE). `function_107ed0` and `0xbd090` call `0xbf5a0`; `function_e01d0` and
`function_107ed0` call `0xbf5d0`.

### `0xbf600`: animation events

It has the form of `animation_event_callback` (`include/unknown_1c62f0.h`),
with the object as the user, and reads the event's `category`:

- 1, a sound, with a `tag_index`: skipped for a biped or vehicle with a
  `player_index` unless bit 0 of the event's `flags` is set. It plays at the
  marker `marker_name` (`function_b8d30`). When the marker is NONE or
  `0x0600008a`, or is not found, it uses `*g_468788` (0, 0, 0) and
  `*g_4687a8` (1, 0, 0) instead. The call is
  `function_189060(object, ..., 1.0, &position, &direction, tag)`.
- 2, an effect, with a `tag_index`: `function_176780` creates it on the object.
- Anything else, for a biped: `0xe5040(event)`, with the object in `edi`.

It never reads `frame`.

Callers: none; see [Pointer tables](#pointer-tables).

### `0xbf760`: the object made from a scenario object

`edi` points at an 8-byte scenario object identifier: unique id, origin bsp
index, type and source, as at placement data `+0x04` in the first part. A
source of NONE gives NONE. Otherwise it walks the objects of that type
(`function_baeb0`, type mask `1 << type`) and compares the unique id
(`+0xa4`), type (`+0xaa`) and source (`+0xab`), and for source 0 also the
origin bsp index (`+0xa8`). It returns the last object that matches, or NONE.
It is the counterpart of the first part's `0xb7a40`, which finds the scenario
object for an identifier.

Callers: `0xa73b0` and `function_d5560`.

### `0xbf830` and `0xbf890`: shield and regions

`0xbf830` sets `shield_vitality` (`+0xf0`) from `xmm0`, and the 16 bits at
`+0x104` from its argument, clamped to 0 to 0x7ffe. `0xbf890` calls
`0xba6f0(object, region, byte, 0)` for each region of the object (10 bytes
each in the `+0x118` block), with the byte for that region from its argument.
`0xa6430` is the only caller of each.

## Existing declarations

As of `main` at `b11793f`:

| Retail | Declared as | Where | What retail shows |
| --- | --- | --- | --- |
| `0xbc190` | `void function_bc190(long object_index, s_damage_owner *owner)` | `src/stubs/projectiles.cpp`, `src/projectiles.cpp` | Roles agree. The object is in `eax` and the owner in `ecx` |
| `0xbc1d0` | `bool __stdcall function_bc1d0(long object_index, point3f *point)` | `src/stubs/unknown_0a76b0.cpp`, `src/projectiles.cpp`, `src/unknown_0a76b0.cpp` | There is a third argument, on the stack: the object the ray ignores. The object to move is in `edi` and the point in `ecx` (`ret 4`). In the two callers with source, `function_e7900` and `function_fc330` (both `todo`), the value the source passes as `object_index`, a unit and a parent, is the third argument in retail. `edi` holds the projectile: the unit's `grenade_projectile_index` in `function_e7900`, and the first argument of `function_fc330` |
| `0xbc380` | `bool function_bc380(long object_index, long block_offset, long size, long a)` | `src/stubs/unit_object_type.cpp`, `src/unit_object_type.cpp` | Roles agree. The object is in `eax`; `block_offset` is the field, and `a` is the alignment in bits |
| `0xbd020` | `void __stdcall function_bd020(long object_index)` | `src/stubs/lane_s.cpp`, `include/object_default_placement.h`, `src/unknown_0a76b0.cpp`, `src/unknown_0b8ca0.cpp`, `src/unknown_0b9fc0.cpp`, `src/vehicles.cpp` | Agrees |
| `0xbd970` | `void function_bd970(long weapon_index, s_16760c_render_model *render_model, s_animation_state *state, long unknown, long node_count, byte *orientations)` | `src/stubs/lane_t.cpp`, `src/unknown_165ce5.cpp` | Any object. All six arguments are on the stack (`ret 0x18`). `unknown` is the node mask, which `0xbdb60` passes and the weapon caller sets to 0 |
| `0xbe240` | `void __stdcall function_be240(long object_index, dword color_mask, color3f const *colors)` | `src/stubs/ui_screens.cpp`, `src/unknown_2bd960.cpp` | Agrees: bit i of `color_mask` selects `colors[i]` |
| `0xbeb30` | `bool __stdcall function_beb30(long object_index)` | `src/stubs/lane_a.cpp`, `include/unknown_29f5b0.h`, `src/unknown_0b7300.cpp`, `src/unknown_1fb350.cpp` | Agrees |
| `0xbef30` | `void __stdcall function_bef30(long object_index, long a, long b, long c, long d)` | `src/stubs/damage.cpp`, `src/damage.cpp`, `src/items.cpp`, `src/projectiles.cpp`, `src/unknown_0a76b0.cpp` | `a` to `d` are read as bytes: remove, add, siblings, own flags only |
| `0xbf0f0` | `void function_bf0f0(s_type_4f0dcc const *datum, long type, long index, s_scenario_block *palette, bool a, bool b)` | `src/stubs/unknown_0b7300.cpp`, `src/unknown_0b7300.cpp` | It returns the object index, or NONE. The datum is in `eax` and the other five are on the stack (`ret 0x14`). `a` asks `0xa7640` first, and `b` goes to `function_d5060` as `unknown` |
| `0xbf380` | `void function_bf380()` | `src/stubs/unknown_0b7300.cpp`, `src/unknown_0b7300.cpp`, `src/unknown_29f5b0.cpp` | Agrees |
| `0xbf5a0` | `bool function_bf5a0(long object_index)` | `src/stubs/lane_s.cpp`, `src/devices.cpp` | Roles agree. The object is in `eax` |
| `0xbf5d0` | `bool function_bf5d0(long object_index)` | `src/stubs/bipeds.cpp`, `src/bipeds.cpp` | Roles agree. The object is in `eax` |
| `0xbf600` | `void __stdcall function_bf600(long user, float frame, s_animation_frame_event const *event)` | `src/stubs/unknown_0b9fc0.cpp`, and `src/unknown_0b9fc0.cpp` with `real frame` | Roles agree, and `frame` is not read |
| `0xbf760` | `long function_bf760(long const *unique_id)` | `src/stubs/unknown_0b7300.cpp`, `src/unknown_0b7300.cpp` | The pointer is in `edi`. Besides the unique id, it reads the origin bsp index, type and source that follow it |

Twenty-two of the functions take register arguments (the table under
[Conventions](#conventions)). `0xbca70`, `0xbcad0`, `0xbcb10`, `0xbcb60`,
`0xbcbb0` and `0xbf380` take none, and the other 23 take all their arguments
on the stack.

Two more notes for whoever decompiles the range:

- `0xbc300` inlines `loop_free`.
- `0xbd090` and `0xbd610` both use the static orientation buffer at
  `0x4dc2f0`. `0xbd610` also uses static matrices at `0x4d8f20`, and
  `0xbd090` a static node mask at `0x55eeb4`, guarded by `0x55eed4`.

## Evidence

- Every entry was disassembled from the retail XBE with `tools/disasm.py` at
  `8adcc51` (capstone 5.0.9). Conventions, offsets, masks, ids, callers and the
  pointer tables were read from retail code and data. The callers of
  `0xbc1d0` were read in their disassembly.
- Constants are read from retail `.rdata`: `0x45dbc0` is 1.0, `0x45dbcc` is
  -1.0, `0x45de4c` is 1/64, `0x45de50` is 1/32, `0x45dbe8` is 0.995,
  `0x45e0b4` is 1/(2 pi), `0x45dbbc` is 0.5, `0x45dc50` is 1/65535,
  `0x45dcf0` is 1/12, `0x45dcec` is 1/60, `0x45dc20` is 0.25, `0x45dc2c` is
  0.75, `0x45dc08` is 20, `0x45dbf8` is 15, `0x45dc5c` is 0.2, `0x45e02c` is
  0.01 and `0x45dc6c` is 1/30. `0xbe240`'s numbers use `0x45df34` to
  `0x45df44` and the double 1.0 at `0x45dcc0`. `*g_4687d8` is the identity
  orientation: quaternion (0, 0, 0, 1), no translation, scale 1.
  `*g_4687a4` and `*g_468788` are zero, and `*g_468710` is (1, 1, 1).
- The definition names, group tags and the attachment groups are strings and
  values in retail.
- Callees outside the range are described from their source in the
  repository where it exists, named above, and otherwise from their
  disassembly, only as far as this document needs them.
- The Existing declarations section reflects `main` at `b11793f`.
- No emulator, runtime testing, SDK or outside dataset was used. Names are
  the repository's own, or describe behaviour.
