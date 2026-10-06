# Object parenting, regions and queries analysis (unknown_0b8bd0)

Retail range covered: `0xb8bd0`–`0xbc18f`, between the object core's
[part 1](unknown_0b7740.md) and [part 2](unknown_0bc190.md). It has 66
inventory entries, 13,242 retail bytes. At `119b4df`, 29 are matched and 16
have source and are `todo`; this document analyses the other 21, which have
no source: attaching objects to markers and nodes and detaching them, hiding
and showing, region permutations, function values, change colours, the
nearest point on an object, the sphere and cluster queries, named scenario
objects, whether a player can see an object, and two script setters.
**Analysis only:** this document adds no source, and nothing in it has been
built or checked against retail with the original compiler. Names are
provisional. The `function_<va>` form is primary; the descriptions are
offered for whoever decompiles the range.

It uses the names of parts 1 and 2, the [lights and liquids](unknown_0bffa0.md)
document and the [object lifecycle](unknown_0b67c0.md) document. It is written
for lane AB, whose claim covers the range.

## Boundary

- `0xb8b70`, just before the range, is the last entry of part 1. `0xbc190`,
  just after it, opens part 2's range and is matched in
  `src/unknown_0bbf40.cpp`. Both are excluded.
- Lane AB's row of the Active claims table (issue #9) covers the range. This
  document makes no claim of its own.
- The 45 entries with source are listed under
  [Other entries](#other-entries). Fifteen of the 21 analysed here had `@stub`
  definitions at `119b4df`; see
  [Existing declarations](#existing-declarations). Since then lane AB has
  given `0xba8c0`, `0xbab40`, `0xbacc0` (now matched), `0xbb050` and `0xbb430`
  source, so twelve keep stubs at `e02c566`, and it has matched `0xbb7f0` and
  `0xbb8f0` among the other entries.
- None of the 21 is reached through a pointer: every caller calls it
  directly.

## Conventions

"Callers" counts direct calls from outside the range, then calls from inside
it. Results are in `eax` (`al` for a bool) unless the table names another
register. "Object" means an object index.

| Retail | Bytes | Arguments | Returns | Callers | What it does |
| --- | --- | --- | --- | --- | --- |
| `0xb8ee0` | 738 | `eax` the object's marker name; stack: parent, the parent's marker name, object (`ret 0xc`) | | 7 | Attaches an object so that its marker meets the parent's marker |
| `0xb92d0` | 213 | `ebx` object; stack: the object's marker, target matrix (`ret 8`) | | 2 + 1 | Moves an object so that its marker lies on a matrix |
| `0xb93b0` | 1236 | stack: parent, object, node (`ret 0xc`) | | 4 + 1 | Attaches an object to a parent's node |
| `0xb98e0` | 359 | `esi` object; stack: matrix (`ret 4`) | | 0 + 1 | Places an object at a matrix times its own transform |
| `0xb9a50` | 63 | `eax` object (`ret`) | | 7 | Detaches an object, by type |
| `0xb9a90` | 249 | `ebx` object (`ret`) | | 13 + 1 | Detaches an object and puts it back in the world |
| `0xb9c60` | 127 | `eax` object; stack: hide (`ret 4`) | | 7 | Hides or shows an object |
| `0xba410` | 299 | stack: object, region name, permutation name (`ret 0xc`) | | 1 | Sets region permutations by name |
| `0xba590` | 249 | stack: object, region mask (`ret 8`) | | 1 | Chooses a new object's permutations |
| `0xba6f0` | 256 | stack: object, region, state, force (`ret 0x10`) | | 4 | Changes a region's state |
| `0xba7f0` | 194 | stack: object, region, bit index, region mask (`ret 0x10`) | | 4 | Sets a region bit and re-chooses permutations |
| `0xba8c0` | 632 | `eax` function element; stack: object, value out (`ret 8`) | bool | 0 + 1 | Evaluates one function element of the definition |
| `0xbab40` | 369 | stack: object, id, value out (`ret 0xc`) | bool | 11 + 2 | An object's function value |
| `0xbacc0` | 143 | `esi` index, `edi` colour; stack: object (`ret 4`) | bool | 3 | Sets one change colour |
| `0xbaff0` | 92 | `eax` object, `ebx` point out, `edi` normal out; stack: origin (`ret 4`) | | 1 | The point of an object nearest an origin, and a normal |
| `0xbb050` | 986 | stack: partition mask, type mask, location, centre, radius, objects out, maximum (`ret 0x1c`) | count (16 bits) | 9 | The objects in a sphere |
| `0xbb430` | 561 | stack: partition mask, cluster count, clusters, maximum, objects out (`ret 0x14`) | count (16 bits) | 1 | The objects in a list of clusters |
| `0xbb670` | 236 | stack: name index (16 bits), recreate (`ret 8`) | object, or NONE | 5 | Creates the scenario object a name refers to |
| `0xbba80` | 895 | `eax` object (`ret`) | bool | 2 | Whether the object is near, or in front of, a player's unit |
| `0xbbfc0` | 173 | stack: five floats (`ret 0x14`) | | 1 | Script 96: a direction and three values in the object globals |
| `0xbc070` | 129 | stack: five floats (`ret 0x14`) | | 1 | Script 97: a second direction and three values |

## Data

### Parents and children

An attached object keeps its parent at `+0x14` and the parent's node at
`+0x18` (8 bits), and is linked into the parent's list of children through
the parent's `+0x10` (first child) and its own `+0xc` (next sibling), as in
part 1. Two header flags go with it:

| Flag | Meaning |
| --- | --- |
| `0x80` | has a parent: `0xb93b0` sets it (`0xb9532`) and `function_b9890` clears it |
| `0x20` | `0xb93b0` sets it after attaching (`0xb97df`); the tick's last pass ([lifecycle](unknown_0b67c0.md)) clears it on every object, and part 2's `0xbc470` returns at once for an object that has it |

Object flag bit 0 hides an object (`0xb9c60`). Bit 7 is the one
`include/object_default_placement.h` calls `hidden`; `0xb93b0` clears it and
connects the object, and `function_10cd50` sets it.

### The regions block

The 16 bits at object `+0x118` give its size and those at `+0x11a` its
offset in the object's memory. With 10 bytes per region, for n regions:

| Bytes | Meaning |
| --- | --- |
| 0 to n | each region's permutation (0xff for none) |
| n to 2n | a copy of the first part, made at creation by `0xba590` |
| 2n to 10n | an 8-byte record per region (`s_region_permutation_choice` in `src/unknown_16d280.cpp`): `+0` the chosen entry, `+1` the region's state, `+2` bits, `+4` an effect index |

`function_ba690` returns the region count and the three parts. In retail the
count comes back through `esi`, and the parts through `edi`, `ebx` and its
one stack argument, in that order.

### Function elements

The definition holds a count at `+0x64` and the address at `+0x68` of
0x20-byte function elements, which `0xba8c0` evaluates:

| Offset | Meaning |
| --- | --- |
| `+0x00` | flags. Bit 0 inverts the input; bit 1 keeps the input's own activity; bit 2 makes the result active; bit 3 adds a per-object offset to a periodic function's time |
| `+0x04` | input id |
| `+0x08` | output id: `0xbab40` evaluates the elements whose `+0x8` is the id |
| `+0x0c` | gate id: when set, the result is kept only while the type handlers report this id active |
| `+0x10` | threshold: when positive, the result is active only above it |
| `+0x14` | the function, for `function_13bb90`; a first byte of 3 is `_function_periodic` (`src/unknown_13b390.cpp`) |
| `+0x1c` | scale id: the value is multiplied by `0xbab40`'s value for it |

### The record at object globals `+0x1c`

A 0x54-byte record that `0x3db00` and `0x3dc90` return for objects with
object flag bit 24. Script 96 (`0xbbfc0`) sets a direction at `+0x48` and
`+0x28` of the globals, three values at `+0x3c`, 0.8 at `+0x34` and `+0x38`,
and NONE in the 16 bits at `+0x6c`; script 97 (`0xbc070`) sets a second
direction at `+0x60` and three values at `+0x54`.

## The functions

### `0xb93b0`, `0xb8ee0` and `0xb92d0`: attaching

`0xb93b0(parent, object, node)` attaches an object to a node of its parent:

1. It refuses (`0xb987a`) when the object already has a parent or a next
   sibling, when it is an item whose unit (`+0x154`) is not the parent while
   its bit 0 at `+0x12c` is set, or when it is the parent or one of the
   parent's ancestors. It does not refuse a NONE parent.
2. It keeps the object's header flag `0x04` (`0xb9478`). If object flag bit 7
   is set, it clears it, and connects the object when it is not connected and
   the object globals are active (`0xb947e`–`0xb94a5`). Then it deactivates
   and disconnects the object, and switches havok off for a non-machine that
   has `physics_active` (`0xb94ed`).
3. It links the object to the parent (`+0x14`, `+0x18`, the child list,
   header flag `0x80` at `0xb9532`), and resets its location to NONE with the
   bsp in `g_4686c4`.
4. It brings the position, forward and up into the node's space through the
   inverse of the node matrix, then calls `0xb7430`
   ([lifecycle](unknown_0b67c0.md)) and `function_b9b90(object, true)`. If the
   parent's root is connected it calls part 2's `0xbef30(..., 0, 1, 0, 0)`;
   then part 2's `0xbd020`.
5. It sets header flag `0x20` (`0xb97e4`). It activates the root when the
   root's cluster is set in the vector at `g_4e6948` `+0x11b8`, and calls
   `function_bba20(root)` when the object had been woken.
6. It calls `function_b7360` on the parent and the object, and creates any
   missing havok components (`0xb985b`–`0xb9875`).

The node is read as 16 bits and stored as 8. Callers: `0xb8ee0`,
`function_e7730`, `function_fd560`, `0x119c10` and `0x28f0e0`.

`0xb8ee0` attaches an object so that a marker of the object meets a marker
of the parent. With the object's marker name in `eax` and (parent, parent's
marker name, object) on the stack, it deactivates the object (`0xb7300`),
disconnects it when it is connected, and finds marker A on the parent and
marker B on the object with `function_b8d30` (`0xb8f39`, `0xb8f4b`), without
testing either. When the object's marker name is neither NONE nor 0, it calls
`0xb92d0(&B, &A.matrix)` (`0xb8f66`); otherwise it inlines the same
alignment, in which B's node matrix is the identity, so the object goes
straight to the parent's marker. Last, `0xb93b0(parent, object, A's node)`
(`0xb91b0`). One of its callers, `function_ce6b0`, attaches and at once
detaches with `0xb9a90` to place a dropped object at a marker.

`0xb92d0`, with the object in `ebx`, computes the target matrix times the
inverse of (the inverse of the object's own transform times the marker's
matrix), from `function_1420f0(+0x64, +0x70, +0x7c)` (`0xb92fc`–`0xb933b`),
then calls `function_b91d0` and `0xb7430(..., NULL, 1, 1, 0, 0)`. No caller
reads its result. Besides `0xb8ee0`, `0x10b850` and `0x10d710` call it with
matrices they build.

### `0xb9a50`, `0xb9a90` and `0xb98e0`: detaching

`0xb9a50`, with the object in `eax`, returns when the object has no parent.
Bipeds and vehicles go to `function_cc590` (`0xb9a80`), anything else to
`0xb9a90` with the object in `ebx`. It is also inlined in part 1's `0xb83b0`
(`0xb8424`–`0xb8444`) and in `0x275380`.

`0xb9a90` records whether the parent's root is connected (`0xb9ac6`) and
the address of the parent's node matrix (`0xb9aef`). When connected, it
removes the lights (`0xbef30` with 1, 0, 0, 0). Then:

1. `function_b9890` unlinks the object.
2. `0xb98e0` returns it to world space.
3. It is connected again with `0xb8600(object, NULL)` when the root was.
4. It switches havok back on (`0xb8890`) unless the object is a machine or
   has `+0xc0` bit 7.
5. It takes the parent's velocities through `0xb77d0`.
6. `function_b7290` and `function_b7360` follow, and object flag bit 26 is
   cleared (`0xb9b7b`).

`0xb98e0`, with the object in `esi` and a matrix on the stack, multiplies the
matrix by the object's own transform (as `function_ba160` builds it, at
`0xb9a0a` and `0xb9a1c`) and calls `0xb75a0` with the result's position,
forward and up and (object, NULL, false) (`0xb9a38`). Its only caller is
`0xb9a90`, with the old parent's node matrix.

### `0xb9c60`: hide and show

With the object in `eax` and a hide flag on the stack. To hide, when object
flag bit 0 is not yet set, it removes the lights through `0xbef30` when the
root is connected, sets bit 0, and calls part 1's `0xb8b70`. To show, when
bit 0 is set, it clears it, adds the lights when the root is connected, and
calls `0xb8b70`. Script function 57 (`function_2a1610`) is one caller.

Part 1 suspected an inlined show and hide pair: the show branch is part 1's
`0xb7b40` step 9 (`0xb7de4`–`0xb7e14`), and the hide branch is inlined in
`function_10cd50` (`0x10cd8f`–`0x10cdb2`). The start of part 1's `0xb8540` is
similar but not a copy: it tests `object_or_parent_hidden`, sets header flag
`0x10` instead of bit 0, and always calls `0xb8b70`.

### `0xba410`, `0xba590`, `0xba6f0` and `0xba7f0`: region permutations

`0xba410(object, region name, permutation name)` sets the named region's
permutation, or every region's when the region name is 0, to the named one; a
permutation name of 0 means none, and a region without that permutation is
left alone. Each changed record is reset to (0xff, 0, 0, NONE) without
stopping the old `+4` effect, which `0x16d660` would stop. For scenery placed
from a scenario object whose scenery flags at `+0x12c` lack bit 1, it first
calls `0x1c5710`, which wakes the havok bodies that a box around the object's
sphere (`+0x30`) overlaps. It ends with `function_17b1d0`, which re-places the
object's first effect attachment, and `function_1c54b0`, which rebuilds the
havok component when a changed permutation needs it. Script function 110
(`0x2a26c0`) calls it.

`0xba590(object, region mask)` chooses a new object's permutations with
`render_model_choose_permutations` (`0x16d280`), from the model, the variant
at `+0xb1` and the mask: regions whose bit is set get none. Without a model
every region gets permutation 0. It then copies the first part into the
second, and calls `function_17b1d0` and `function_1c54b0`. Its only caller is
part 1's `0xb7b40` (`0xb81b9`), with placement `+0xac`, so that field is the
mask of regions that get no permutation.

`0xba6f0(object, region, state, force)` calls
`object_change_region_permutations` (`0x16d660`) with a bit index of NONE. It
re-chooses the region's permutation for the new state only when the state is
at least the record's `+1` or force is set. It runs the same scenery wake as
`0xba410`, then `function_17b1d0`, `function_1c54b0` and `function_b7360`.
Its callers are part 2's `0xbf890` (each region, without force), `0xda110`
(a damage permutation is destroyed), `0xdb5c0` (back to state 0, with force)
and `0x2a0380` (a region by name).

`0xba7f0(object, region or NONE, bit index, 16-bit region mask)` calls
`0x16d660` with a state of NONE, which keeps each region's state, and without
force: it sets or clears bit `bit index` of each region's record `+2` from the
mask and re-chooses. It has no scenery test and no `function_b7360`. Shield
damage (`0xd6a70`) and recharge (`0xd5de0`) use bit 2, vehicle speed
(`0xefde0`) and `0xfdfb0` bit 0.

### `0xbab40` and `0xba8c0`: function values

`0xbab40(object, id, value out)` is the function value parts 1 and 2 and the
lights use. It returns whether the value is active:

1. An interpolated value of the id, when there is one.
2. Ids 0 and `0x030005a6` give 1.0 and true; `0x040005a7` gives 0 and false.
3. A NONE object gives false. It reads the header of index `0xffff` before
   that test, and `0x18a7b0` and `0x18cbc0` do pass NONE.
4. The type handlers' slot `0x4c`, through `function_108d90`.
5. Otherwise `0xba8c0` for every function element whose output id is the id.
   It does not stop at a match, so the last match wins.
6. With no element, the parent's value when object flag bit 26 is set.

`0xba8c0`, with the element in `eax`, takes the input value. When object
`+0xc1` bit 1 is set and `0x10aac0` finds an interpolated value for the input
id, it returns that value and true at once (`0xba918`–`0xba938`), with no
mapping, scale, threshold, gate or clamp. Otherwise it takes the type
handlers' value (`function_108d90`) and maps it through the element's
function with `function_13bb90`, or uses the game time for a periodic
function. Then it multiplies by `0xbab40`'s value for the scale id, applies
the threshold, the always-active flag and the gate id, and clamps the result
to 0 to 1.

### `0xbacc0` and `0xbaff0`

`0xbacc0`, with the index in `esi`, a colour in `edi` and the object on the
stack, writes the colour as both the base and the current colour i of the
change-colour block at `+0x124`, then calls `function_3dd10(object, 1)` to
repack the object's render colours. It returns false for an index out of
range. `0x14bfc0` sets colours 0 to 3, `0x243a20` colour 0, and `0x2b0f76`
the colours of named bipeds.

`0xbaff0`, with the object in `eax`, the point out in `ebx`, the normal out in
`edi` and an origin on the stack, asks havok (`0x183910`) for the nearest
point and normal on the object's rigid bodies, when it has a havok component
with component flag bit 5. Otherwise the point is the sphere's centre
(`+0x30`) and the normal `*g_4687b0`, (0, 0, 1). Area damage (`0xd74e0`) is
its caller.

### `0xbb050` and `0xbb430`: queries

`0xbb050` takes seven stack arguments: a partition mask, a type mask, a
location, a centre, a radius, the objects out and a maximum. It finds the
clusters the sphere reaches with `function_14a5b0`, or only the location's
cluster when the radius is 0 or less. Then it walks the collideable partition
(`0x4de2e0`, mask bit 0) and the noncollideable one (`0x4de2d4`, bit 1),
filters by type, stamps each object in `g_4de300` with `g_4de2fc` so that none
is reported twice, and tests the sphere against the partition record. A mask
of 0 means both partitions or all types. It returns the 16-bit count as soon
as the maximum is reached. It has nine callers.

`0xbb430` (partition mask, cluster count, clusters, maximum, objects out) does
the same stamping and walks over a given list of clusters, with no type or
distance test. Its only caller, `0x1a54a0`, passes the clusters a cone reaches
(`function_14a6d0`) and adds each player unit's root.

### `0xbb670`: named scenario objects

`0xbb670(name index, recreate)` creates the scenario object that a name
refers to, with part 2's `0xbf0f0(datum; type, index, palette, 1, 0)`, from
the scenario's name entry (`+0x4c`, 0x24 bytes) and the type definition's
block offsets. Without recreate, an existing object gives NONE. With it, the
existing object first loses its name (`function_bf090`) and becomes a
run-time object: a new unique id, source 2 and no scenario index. Five script
functions call it; none reads the result.

### `0xbba80`: seen by a player

`0xbba80`, with the object in `eax`, returns true when the object is near, or
in front of, a player's unit. The root must be active, connected, have a
location, and be in a cluster set in the vector at `g_4e6948` `+0x1138`. Then,
for some player's unit, it takes marker `0x4000095`'s position, divides its
distance to the object's centre by the unit's zoom (`function_c8880`), and
compares it with the radius at `+0x3c`; or, within the model's `+0x28` times
1.2 (40 when that is not positive), the object must lie within
atan2(r, d) + pi/4 of the unit's aim at `+0x15c`. Part 2's garbage collection
(`0xbf2c0`) keeps an object for which it is true; actor code at `0x268c90` is
the other caller.

## Existing declarations

At `119b4df`, in the stub files:

| Retail | Declared as | Where | What retail shows |
| --- | --- | --- | --- |
| `0xb8ee0` | `void __stdcall function_b8ee0(long parent_index, long marker_name, long object_index, long a)` | `src/stubs/unknown_0a76b0.cpp` | `a`, the object's own marker name, is in `eax`, not on the stack (`ret 0xc`) |
| `0xb93b0` | `void __stdcall function_b93b0(long parent_index, long object_index, long node_index)` | `src/stubs/projectiles.cpp` | Agrees |
| `0xb9a50` | `void __stdcall function_b9a50(long unit_index)` | `src/stubs/unknown_0a76b0.cpp` | Any object, in `eax`, with no stack argument |
| `0xb9a90` | `void function_b9a90(long object_index)` | `src/stubs/projectiles.cpp` | Roles agree. The object is in `ebx` |
| `0xb9c60` | `void function_b9c60(long object_index, bool flag)` | `src/stubs/damage.cpp` | Roles agree: true hides. The object is in `eax` |
| `0xba410` | `void __stdcall function_ba410(long object_index, long a, long b)` | `src/stubs/lane_a.cpp` | Agrees: `a` is the region name and `b` the permutation name |
| `0xba6f0` | `void __stdcall function_ba6f0(long object_index, long region_index, long state, bool flag)` | `src/stubs/lane_a.cpp` | Agrees; `flag` is force |
| `0xba7f0` | `void __stdcall function_ba7f0(long object_index, long a, long b, long c)` | `src/stubs/damage.cpp` | Agrees: region, bit index and region mask |
| `0xbab40` | `bool __stdcall function_bab40(long object_index, long name, real *value)` | `src/stubs/lane_r.cpp` | Agrees |
| `0xbacc0` | `bool function_bacc0(long object_index, long index, point3f const *point)` | `src/stubs/lane_o.cpp` | Roles agree; `point` is a colour. The index and colour are in `esi` and `edi` |
| `0xbaff0` | `void function_baff0(long object_index, point3f const *origin, point3f *arg_149545, union vector3f *normal)` | `src/stubs/damage.cpp` | Roles agree. The object is in `eax`, the point out in `ebx`, the normal out in `edi` |
| `0xbb050` | `short __stdcall function_bb050(long a, unsigned long type_mask, void const *location, point3f const *position, float radius, long *objects, short maximum_count)` | `src/stubs/damage.cpp` | Agrees; `a` is the partition mask |
| `0xbb670` | `void __stdcall function_bb670(short name_index, bool flag)` | `src/stubs/lane_a.cpp` | It returns the object, or NONE; `flag` recreates |
| `0xbbfc0` | `void __stdcall function_bbfc0(real a, real b, real c, real d, real e)` | `src/stubs/lane_a.cpp` | Agrees |
| `0xbc070` | `void __stdcall function_bc070(real a, real b, real c, real d, real e)` | `src/stubs/lane_a.cpp` | Agrees |

A note for code outside the range: `function_d9f70` (`src/damage.cpp`) and
`function_ea6b0` (`src/unknown_0a76b0.cpp`), both `todo`, read a region's
state as `states[i * 8 + 1]`, from `function_ba690`'s first output, the
first part of the regions block. Retail reads it from the third output, the
records (`0xd9fc3`, `0xea779`).

## Other entries

The 45 entries with source at `119b4df`:

| Retail | Status | Source |
| --- | --- | --- |
| `0xb8bd0` | `matched` | `src/unknown_0b8bd0.cpp` |
| `0xb8c00` | `matched` | `src/unknown_0b8c00.cpp` |
| `0xb8c40` | `matched` | `src/unknown_0b7740.cpp` |
| `0xb8ca0` | `todo` | `src/unknown_0b8ca0.cpp` |
| `0xb8d30` | `todo` | `src/unknown_0b8ca0.cpp` |
| `0xb91d0` | `todo` | `src/unknown_0b8ca0.cpp` |
| `0xb9890` | `matched` | `src/unknown_0b8ca0.cpp` |
| `0xb9b90` | `todo` | `src/unknown_0b8ca0.cpp` |
| `0xb9ce0` | `matched` | `src/unknown_0b8bd0.cpp` |
| `0xb9d20` | `matched` | `src/unknown_0b8ca0.cpp` |
| `0xb9d70` | `todo` | `src/unknown_0b9d70.cpp` |
| `0xb9dd0` | `todo` | `src/unknown_0b58c0.cpp` |
| `0xb9ef0` | `todo` | `src/unknown_0cafc0.cpp` |
| `0xb9fc0` | `matched` | `src/unknown_0b8ca0.cpp` |
| `0xba160` | `todo` | `src/unknown_0b7740.cpp` |
| `0xba1d0` | `todo` | `src/unknown_0b8bd0.cpp` |
| `0xba300` | `todo` | `src/unknown_0b8bd0.cpp` |
| `0xba350` | `todo` | `src/unknown_0b8ca0.cpp` |
| `0xba3d0` | `matched` | `src/unknown_0bad50.cpp` |
| `0xba540` | `matched` | `src/unknown_0bad50.cpp` |
| `0xba690` | `matched` | `src/unknown_0bad50.cpp` |
| `0xbad50` | `matched` | `src/unknown_0bad50.cpp` |
| `0xbadc0` | `matched` | `src/unknown_0bad50.cpp` |
| `0xbae20` | `matched` | `src/unknown_0bad50.cpp` |
| `0xbae80` | `matched` | `src/unknown_0bad50.cpp` |
| `0xbaeb0` | `matched` | `src/unknown_0bad50.cpp` |
| `0xbaf40` | `matched` | `src/unknown_0bad50.cpp` |
| `0xbaf80` | `matched` | `src/unknown_0bad50.cpp` |
| `0xbafb0` | `matched` | `src/unknown_0bad50.cpp` |
| `0xbb760` | `matched` | `src/unknown_0bb760.cpp` |
| `0xbb780` | `matched` | `src/unknown_0bb760.cpp` |
| `0xbb7b0` | `matched` | `src/unknown_0bb760.cpp` |
| `0xbb7f0` | `todo` | `src/unknown_0bb760.cpp` |
| `0xbb880` | `matched` | `src/unknown_0bb760.cpp` |
| `0xbb8f0` | `todo` | `src/unknown_0bb760.cpp` |
| `0xbb950` | `todo` | `src/unknown_0bb760.cpp` |
| `0xbba20` | `todo` | `src/unknown_0bbf40.cpp` |
| `0xbbe00` | `matched` | `src/unknown_0bbf40.cpp` |
| `0xbbe60` | `matched` | `src/unknown_0bbf40.cpp` |
| `0xbbe90` | `matched` | `src/unknown_0bbf40.cpp` |
| `0xbbec0` | `matched` | `src/unknown_0bbf40.cpp` |
| `0xbbf40` | `matched` | `src/unknown_0bbf40.cpp` |
| `0xbbf80` | `matched` | `src/unknown_0bbf40.cpp` |
| `0xbc100` | `todo` | `src/unknown_0bbf40.cpp` |
| `0xbc150` | `matched` | `src/unknown_0bbf40.cpp` |

## Evidence

- Every entry was disassembled from the retail XBE with `tools/disasm.py` at
  `119b4df` (capstone 5.0.9). Conventions, offsets and flags were read from
  retail code and data, and callers from `config/functions.csv`, checked
  against the relative calls and jumps in the code.
- The work-in-progress and matched source named above, and the stub
  declarations, were read from the repository and compared with retail.
- Callees outside the range are described from their source in the
  repository where it exists, named above, and otherwise from their
  disassembly, only as far as this document needs them.
- No emulator, runtime testing, SDK or outside dataset was used. Names are
  the repository's own, or describe behaviour.
