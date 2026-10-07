# Biped helpers (unknown_0e5040)

Retail range covered: `0xe5040`–`0xe68b8`, except `0xe5280` and `0xe6800`.
These are 21 inventory entries, 5,915 retail bytes, all `todo` with no source
when this was written: the helpers that sit between the bipeds and the unit
actions. Most set or read a biped's physics mode, measure and choose the
targets of a biped's grab, or move a biped in an animation-driven mode; the
rest play footsteps, push a vehicle, check a weapon swap, spawn on death, snap
a direction and run a unit's action updates. **Analysis only:** this document
adds no source, and nothing in it has been built or checked against retail with
the original compiler. Names are provisional. The `function_<va>` form is
primary; the descriptions are offered for whoever decompiles the range.

It uses the names of `src/bipeds.cpp` (`s_biped`, `BIPED_GET` and the physics
modes), `src/unknown_0a76b0.cpp` (the unit actions that call several of these
functions), `src/unknown_1fc2f0.cpp` (`s_tracked_point`) and
`src/unit_object_type.cpp` (unit fields). Fourteen of the entries are already
declared in the source; see [Existing declarations](#existing-declarations).

## Boundary

- `0xe5040` follows @Banshee64's bipeds (`0xdc370`–`0xe503f`, #28), and
  `0xe68c0`, matched with source in `src/unknown_0e68c0.cpp`, begins the unit
  actions (`0xe68c0`–`0xee89f`, #23). Inside the range, `0xe5280` (`near`,
  `src/unknown_0e5280.cpp`) and `0xe6800` (`todo`, `src/unknown_0e6800.cpp`)
  have source and are excluded.
- No row of the Active claims table (issue #9) covers the range, so it is open.
  This document makes no claim. @BrassMonkey71 has since matched `0xe5670`
  (#115), `0xe5240` (#119) and `0xe58e0` (#125). Besides the bipeds and the
  unit actions, the callers include lane Q's `0x1515e0` and `0x150400`, lane
  AB's `0xbf600` and the vehicle update `0xefde0`.
- Apart from `0xe5670`, `0xe5240` and `0xe58e0`, no entry has an `@retail`
  marker.

## Conventions

"Callers" counts the functions that call an entry directly, from outside the
range and then from inside it. No entry is reached through a pointer or a
table. "Biped" is a biped's object index; `ret N` gives the stack bytes the
callee pops.

| Retail | Bytes | Arguments | Returns | Callers | What it does |
| --- | --- | --- | --- | --- | --- |
| `0xe5040` | 504 | `edi` biped; stack: frame event (`ret 4`) | void | 1 + 0 | Plays the footstep effect of an animation frame event |
| `0xe5240` | 49 | `eax` biped | bool | 1 + 0 | Walking (mode 1) and `function_e4050` |
| `0xe5300` | 353 | `eax` biped; stack: entering (`ret 4`) | void | 3 + 0 | Pushes the vehicle a biped enters, leaves or boards |
| `0xe5470` | 502 | `eax` player; stack: unit (`ret 4`) | bool | 1 + 0 | Whether a player's unit and a friendly AI unit can swap weapons (inferred) |
| `0xe5670` | 29 | `eax` biped | long | 1 + 0 | The physics mode |
| `0xe5690` | 82 | `esi` biped; stack: point (`ret 4`) | void | 1 + 0 | Physics mode 4, at a point |
| `0xe56f0` | 82 | `esi` biped; stack: point (`ret 4`) | void | 2 + 0 | Physics mode 5, at a point |
| `0xe5750` | 62 | `eax` biped | void | 2 + 1 | Physics mode 2 |
| `0xe5790` | 66 | `ebx` biped | void | 1 + 1 | Physics mode 3 |
| `0xe57e0` | 89 | `ecx` biped | void | 3 + 0 | Meets `function_e4050`'s flag and 0.18-second count at once |
| `0xe5840` | 155 | `esi` biped, `ecx` offset; stack: bool (`ret 4`) | void | 1 + 0 | Moves an animation-driven biped by an offset, or stores the offset |
| `0xe58e0` | 73 | `eax` biped | bool | 2 + 0 | A test of `+0x3e4`, `+0x3e8` and two flag bits |
| `0xe5930` | 68 | `eax` biped | void | 6 + 0 | Returns the biped to its default physics mode |
| `0xe5980` | 83 | `eax` biped | long | 0 + 1 | The default physics mode: 1, 2 or 3 |
| `0xe59e0` | 479 | `eax` biped, `ebx` velocity out, `esi` position out | void | 1 + 0 | The animation's movement, in world space |
| `0xe5bc0` | 391 | `edi` unit; stack: candidate, current (`ret 8`) | long | 1 + 0 | Chooses between two grab targets |
| `0xe5d50` | 1624 | `eax` biped; stack: target, reference direction, output (`ret 0xc`) | void | 2 + 1 | Measures a target from the biped's grab eye: yaw, pitch and distance |
| `0xe63b0` | 170 | `ecx` measurement, `dl` bool | bool | 1 + 1 | Whether a measurement is inside the grab window |
| `0xe6460` | 119 | stack: biped (`ret 4`) | void | 1 + 0 | Spawns from a dying biped's definition (inferred) |
| `0xe64e0` | 798 | `eax` reference, `esi` direction, `edi` fallback; stack: bool, out, index (`ret 0xc`) | void | 2 + 0 | Snaps a direction to one of four axes |
| `0xe6830` | 137 | stack: unit (`ret 4`) | bool | 2 + 0 | Runs the update of each active unit action |

## Data

**The physics mode** is the byte `s_biped::physics_mode` at `+0x3dc`.
`src/bipeds.cpp:1083` names the modes: walking (1), flying (2), dead (3),
animation driven (4 and 5) and grabbing (6). The mode starts a small state:
`+0x3e0` holds the biped's own index, then come `+0x3e4` and `+0x3e8`, and
`+0x3ec` is a block whose type depends on the mode. It is `s_state_c570`
(`src/unknown_1eb8a0.cpp`) in mode 3 and `s_tracked_point`
(`src/unknown_1fc2f0.cpp`) in modes 4 and 5. So `s_biped`'s `unknown3f8` and
`unknown3f9` are `s_state_c570`'s `field_0c` and `field_0d` in mode 3, and
`+0x3f8` is `s_tracked_point`'s `offset` in modes 4 and 5.

The mode setters here, like the other callers in the source, change the mode
through `0x1e54d0`, which has no source (a `@stub` in
`src/stubs/unknown_0a76b0.cpp`). It takes `edi` = `&biped->physics_mode` and
the new mode on the stack (`ret 4`). It runs an exit action for the old mode
(modes 4 and 5 call `0x1fc350` on the block), stores the new mode (`0x1e54fb`),
and runs an entry action from the table at `0x1e55b4`:

| Mode | Entry action |
| --- | --- |
| 1 | `0x1f03e0` on the block |
| 2 | clears the block's first dword and copies `*g_4687a4` after it |
| 3 | `function_1ec570` (`src/unknown_1eb8a0.cpp`) |
| 4, 5 | `function_1fc2f0` with the object's position, and true for mode 5 |
| 6 | `0x1eb640` |

When the mode changes into or out of 3, 4 or 5, it also calls
`0x1c3770(object, 0)`. `0xde4c0` instead zeroes the byte directly when it sets
up a biped (`0xde5f6`), then calls `0xe5930`.

**Forgetting the melee.** Three of the mode setters also make the three stores
of `function_e4300` (`src/bipeds.cpp`, matched): `+0x34c` = NONE as a word, and
`+0x34e` = `+0x34f` = 0. `0xe5690` and `0xe56f0` make them before the mode
changes, and `0xe5750` after (inferred: `function_e4300` inlined).

**A grab measurement** is three reals, as `0xe5d50` writes them and `0xe63b0`
reads them: yaw, pitch and distance. Yaw and pitch are in radians, relative to
a reference direction; distance is the gap between the two bodies, not less
than 0.

## The functions

### The physics mode

- `0xe5670` returns the mode, zero-extended. Its one caller, `0xd1850`,
  compares it with 2 ([surface colour queries](unknown_0d15e0.md)).
- `0xe5980` gives the mode a biped returns to: 3 when `flags_10a` (`+0x10a`)
  has bit 2, else 2 when its definition's `+0x264` has bit 4 (`s_dc_tag::flag4`
  in `src/unknown_0dc450.cpp`, which the matched `0xdc4c0` also tests), else 1.
- `0xe5930` sets that mode. For 1 it calls `0x1e54d0` itself; for 2 it jumps to
  `0xe5750` (`0xe5948`), and for 3 it calls `0xe5790`. It is the only caller of
  `0xe5980`, and it reads the result with `dec eax` twice.
- `0xe5750` sets mode 2 and then forgets the melee.
- `0xe5790` sets mode 3. Then, when `function_e4050` is true, it clears
  `+0x3f9` and then `+0x3f8` (`0xe57c3`, `0xe57c9`).
- `0xe5690` and `0xe56f0` forget the melee, set mode 4 or 5, and move the biped
  to the point with `function_b75a0(biped, point, NULL, NULL, NULL, false)`.
  They differ only in the mode.
- `0xe5840` does nothing unless the mode is 4 or 5. With its bool set, it moves
  the biped to its position (`+0x64`) plus the offset with `function_b75a0`.
  Otherwise it copies the offset to `+0x3f8` (the tracked point's offset) and
  sets byte `+0x415` (its `unknown29`) to 1.
- `0xe5240` returns false unless the mode is 1, and otherwise jumps to
  `function_e4050` (`0xe5269`), returning its result.
- `0xe57e0` sets bit 0 of `flags_348` and sets `unknown399` to
  `g_510c54->field_2_3` × 0.18, rounded by `fistp` (0.18 is `0x45df20`). Then
  it calls `function_b9b90(biped, false)`. `function_e4050` requires the same
  bit and `unknown399` at least that product, and `0xdd360` counts `unknown399`
  up each tick while the bit is set. So this meets both conditions at once,
  without the 0.18-second wait (inferred).

Mode 6 is set elsewhere, by `0xdee60` (`src/bipeds.cpp`).

### `0xe5d50`, `0xe63b0` and `0xe5bc0`: grab targets

`0xe5d50` measures a target object from the biped's grab eye. Its callers are
`0xdc5c0` in mode 6 (`src/bipeds.cpp:1227`), `0xe01d0` twice (`:2588` and
`:2593`), and `0xe5bc0`.

1. The eye is what the repository's inline `biped_grab_eye` computes
   (`src/bipeds.cpp:1077`): `function_b9dd0`'s position, raised to the standing
   or crouching eye height (definition `+0x218`, `+0x21c`) times the scale
   (`+0xa0`), and moved to the head unless something is in the way
   (`0xe5da5`–`0xe60a2`). The constants are the same: −0.25 (`0x445634`), 0.9
   (`0x45dc84`), 0.05 (`0x45dc1c`) and the collision flags `0x4808c2d`. It
   first calls `function_df5f0` on the biped itself, and uses only the radius
   it returns.
2. The target point T depends on the target's type. For a biped (header type 0)
   it is the centre from `function_df5f0`, with the eye's height clamped to
   [T.z, T.z + height]. For any other object it is the closest point that
   `0x183670` finds between the two Havok components (`+0xb4`), taken to be the
   one on the target (inferred), or else the target's `function_b9dd0`
   position, with no radius or height.
3. d is T − eye, normalized inline, and r is its length. If r is below 0.0001,
   r counts as 0 and the output is (π, 0, 0) (`0x45dddc`).
4. Otherwise the distance is r less the two radii, and 0 if that is negative. R
   is the reference direction, U its up from `function_11d090`, and L = U × R.
   Yaw is atan2(d · L, d · R) and pitch is atan2(d · U, √((d · R)² + (d ·
   L)²)), both by `fpatan` (`0xe637e`, `0xe6396`).

The output's three stores of the distance (`0xe6245`, `0xe6277`, `0xe6288`)
suggest the source writes the output field directly (inferred). There is no
NONE check on the target.

`0xe63b0` tests a measurement. With d = max(0, distance), it fails unless d <
2.0 (`0x45dc0c`) when `dl` is set, or d < 1.2 (`0x45dce8`) when not. With t = d
× 0.8333333 (`0x45dce4`, 1/1.2, whatever the range), the yaw limit is (1 − t) ×
50° + t × 15° (`0x45dce0`, `0x45dcdc`). It fails when the yaw lies outside
±limit or the pitch outside ±30° (`0x45dcd8`, `0x45dcd4`), each tested by
pinning the value and comparing it with the original. The limit reaches 0 at d
≈ 1.714 and is negative beyond, where every yaw fails, so the 2.0 range ends at
about 1.714 in practice (arithmetic from the constants). The callers pass
`function_1012c0` of the current weapon as `dl`.

`0xe5bc0` chooses between a candidate and the current target for lane Q's
`0x1515e0`, which stores the result. It measures each against the unit's
`unknown168` (`+0x168`) and tests it with the current weapon's flag,
`function_1012c0`'s value, inline (`0xe5be1`–`0xe5c34`).

- With no current target (NONE) it returns the candidate if it passes, else
  NONE.
- If only one passes, it returns that one, and NONE if neither does.
- If both pass and both distances are below 0.6 (`0x44ae8c`), it returns the
  one with the smaller |yaw|. Otherwise it returns the nearer. A tie keeps the
  current target.

`0xe58e0` is true when `+0x3e8` is not NONE and either `flags_c0` bit 1 or bit
2 is set or `+0x3e8` equals `+0x3e4`. The same test is inline in `0xf8200`
(`src/projectiles.cpp:1376`), which calls `+0x3e8` `weapon_index`. Its callers
are `0x1d96d0` and `0x1e0fe0`, both without source.

### `0xe59e0`, `0xe5040` and `0xe5300`: movement and footsteps

`0xe59e0` gives the movement of the biped's animation in world space, for
`0xdc5c0` in modes 4 and 5 (`src/bipeds.cpp:1193`). It takes the biped's
transform M from `function_ba160`, then calls `s_animation_state::velocity_get`
(`0x1cdcd0`) on the biped's animation state (the object plus the short at
`+0x12a`), with the position and velocity outputs. On success it transforms the
position by M as a point and the velocity as a vector, each scaled only when
M's scale is not 1.0. On failure the position is M's position and the velocity
is `*g_4687a4`, the zero vector.

`0xe5040` handles a biped's animation frame events, for `0xbf600`
([object core part 2](unknown_0bc190.md)). It returns at once when the biped
has no animation state (`+0x12a` NONE).

1. A biped is slow when its velocity (`+0x88`) is at most 1.5 long (its square
   against 2.25, `0x45dc8c`).
2. The scale is 1.0, or, when the state's channel 1 is valid, 1 − f of the
   state's `+0x80`. f is the inline body of `blend_fraction`
   (`src/unknown_1cafc0.cpp`), and a scale below 0.5 also makes the biped slow
   (`0x45dbbc`).
3. It writes scale × 127 (`0x45ddcc`), truncated and pinned to [0, 127], to
   `unknown39f` (`+0x39f`) for every event that gets this far. `function_e4c50`
   reads it back.
4. The kind is 0 when slow, else 6 when `flags_348` has bit 4, else 1.
5. A jump table (`0xe5214`) on the event's type (`+0x8`) calls `function_e4d90`
   with marker 0 for type 2 and marker 1 for type 3, each with the kind, and
   with marker 2 and kind 3 for type 9 or kind 7 for type 10. The marker goes
   in `edx`, with the biped, the kind and the scale on the stack. Other types
   do nothing.

`0xe5300` pushes the vehicle a biped is in, during the first 0.2 seconds of the
unit actions that enter (`0xea7c0`), leave (`0xeaeb0`) and board (`0xeb340`)
it. The stack argument is 1 for entering and boarding and 0 for leaving.

1. The biped's parent (`parent_index`, `+0x14`) must be a vehicle, and the
   vehicle's own `parent_index` must not be NONE (`0xe5365`), so a vehicle that
   is not attached to another object is not pushed. This test is in retail as
   written.
2. The factor is the vehicle definition's real at `+0x274` when entering or
   `+0x278` when leaving, and nothing happens unless it is above 0.
3. The body is the rigid body that `function_1cfe70` finds for the biped's
   `parent_node` (`+0x18`) in the vehicle's Havok component (`+0xb4`).
4. The velocity change is `*g_4687bc` (0, 0, −1) × (definition `+0x274` of the
   biped / the body's mass, at least 1) × `g_510c54->rate` ×
   `g_51e9c4->unknown0` (gravity, as `src/bipeds.cpp:3087` uses it) × the
   factor. `havok_component_rigid_body_point_impulse_apply` applies it at the
   biped's position: multiplied by the mass, or, when the component's `flag1`
   is set, as a plain change of linear velocity.

### `0xe5470`, `0xe64e0`, `0xe6460` and `0xe6830`

`0xe5470` is true when a player's unit and another unit could swap their
current weapons (inferred). Lane Q's `0x1515e0` calls it with the player index
(`g_4e8c24`, 0x21c bytes each, the unit at `+0x2c`) only when the two are not
enemies, and on true builds a target of kind 2 for `0x151320`.

1. The other unit must have an actor (`+0x12c`), and both units must hold a
   current weapon (`+0x212`, `+0x218`).
2. D runs from the player's unit's marker `0x4000095` to the other unit's
   (`function_caf60`). Its length must be below 1.0 and above 0, which means at
   least 0.0001, since `function_30bf0` returns 0 for less.
3. The player's unit must aim at the other one, with `function_cb7e0`'s
   `+0x168` · D above 0.8 (`0x44ae88`), and the other unit must face back, with
   `function_118e80`'s forward · D below 0.
4. `function_cd7b0` must accept the player's weapon for the other unit, and the
   other unit's weapon for the player's unit, with the third byte of its
   four-byte output set each time.

`0xe64e0` snaps a direction to one of four axes. v is the reference (`eax`),
flattened to z = 0 unless the bool is set, then normalized; the axes are v, −v,
w and −w, where w is (−v.y, v.x, 0) when flat. When |direction · v| is above a
threshold, or |direction.z| is above 0.9 (`0x45dc84`), the choice is v or −v by
the sign of direction · v, and otherwise w or −w by the sign of direction · w.
The threshold is 0.4 (`0x45dd30`) when |fallback · v| is below 0.5, else 0.3
(`0x44ae90`). It writes the index (0 to 3) and the axis, each only when its
pointer is not NULL. When v has no length it writes index 0 and the direction
itself, flattened and normalized with `function_1201a0` when flat. When the
bool is set, w is `*g_4687a4`, the zero vector, so index 2 would give a zero
axis; neither caller sets it. The callers are lane Q's `0x150400` and the AI's
`0x1f6090`, neither with source.

`0xe6460` acts when a biped is destroyed, for `0xd6bc0` (`src/damage.cpp:722`).
When the long at `+0x30c` of the biped's definition is not NONE and its flags
at `+0x1f0` have bit 8, it calls `0x1e0fe0` with the biped's position, the
biped, that long and the word at `+0x310`. `0x1e0fe0` reads the biped's actor,
uses 3 when the word is not positive, and calls `0x1e0850`, which creates
objects (`0xb7b40` at `0x1e0af4`). Inferred: it spawns that many units of the
character tag at `+0x30c`.

`0xe6830` runs a unit's actions for the bipeds' update `0xdd360` and the
vehicles' `0xefde0`. For each of the 60 request types whose bit is set in the
unit's `s_unit_actions` (at the offset in `+0x346`, `active` at `+4`), it calls
the type's update from `g_4677c8` (`src/unknown_0e6900.cpp`), when it has one,
as `update(unit, type)`. A true result keeps the bit and makes the result true;
a false result clears the bit. Types without an update keep their bit.

## Callees without source

- `0x1e54d0`, the mode setter, is described under [Data](#data).
- `0x1697c0` casts a ray. `0xe5d50` passes it the collision flags, the start,
  the vector, NONE twice for the objects to ignore and the result, all on the
  stack (`ret 0x18`), and tests the bool in `al`. It has a `@stub` in
  `src/stubs/lane_c.cpp`, and `src/bipeds.cpp` declares it with those six
  parameters.
- `0x1e0fe0` takes the point in `eax` and, on the stack, the object, the
  definition's `+0x30c` value and the count (`ret 0xc`); see `0xe6460`.

## Existing declarations

Fourteen entries are declared where they are called, each with a `@stub`
definition in the file named, except `0xe5240`, which #119 has since defined in
`src/unknown_0e5240.cpp`. All fourteen have retail's parameter count; where the
declaration says `__stdcall` but retail takes registers, that is the LTCG
convention, not a missing parameter.

| Retail | Declared as | Where | What retail shows |
| --- | --- | --- | --- |
| `0xe5240` | `bool function_e5240(long arg_0);` | `src/unknown_1d8dd0.cpp`; `src/unknown_0e5240.cpp` | The object in `eax`, a bool in `al` |
| `0xe5300` | `void function_e5300(long unit_index, long a);` | `src/unknown_0a76b0.cpp`; `src/stubs/unknown_0a76b0.cpp` | The biped in `eax` and `a` on the stack: 1 entering or boarding, 0 leaving |
| `0xe5690` | `void __stdcall function_e5690(long unit_index, point3f const *point);` | `src/unknown_0a76b0.cpp`; `src/stubs/unknown_0a76b0.cpp` | The biped in `esi`, the point on the stack |
| `0xe56f0` | `void __stdcall function_e56f0(long unit_index, point3f const *point);` | `src/unknown_0a76b0.cpp`; `src/stubs/unknown_0a76b0.cpp` | The same |
| `0xe5750` | `void function_e5750(long unit_index);` | `src/unknown_0a76b0.cpp`; `src/stubs/unknown_0a76b0.cpp` | The biped in `eax` |
| `0xe5790` | `void function_e5790(long arg_159e6d);` | `src/bipeds.cpp`; `src/stubs/bipeds.cpp` | The biped in `ebx` |
| `0xe57e0` | `void __stdcall function_e57e0(long arg_159e6d);` | `src/bipeds.cpp`; `src/stubs/bipeds.cpp` | The biped in `ecx`, and a plain `ret` |
| `0xe5840` | `void function_e5840(long arg_0, vector3f const *arg_1, bool arg_2);` | `src/unknown_10f1e0.cpp`; `src/stubs/lane_aa.cpp` | The biped in `esi`, the offset in `ecx`, the bool on the stack |
| `0xe5930` | `void function_e5930(long unit_index);` | `src/bipeds.cpp`, `src/unknown_0a76b0.cpp`; `src/stubs/unknown_0a76b0.cpp` | The biped in `eax` |
| `0xe59e0` | `void function_e59e0(long arg_159e6d, vector3f *velocity, point3f *position);` | `src/bipeds.cpp`; `src/stubs/bipeds.cpp` | `eax`, `ebx` and `esi`; both pointers are outputs |
| `0xe5d50` | `void __stdcall function_e5d50(long arg_159e6d, long target_index, vector3f *offset, point3f *point);` | `src/bipeds.cpp`; `src/stubs/bipeds.cpp` | The biped in `eax`, three on the stack. `offset` is a reference direction that is only read, and `point` receives a measurement (yaw, pitch, distance) |
| `0xe63b0` | `bool function_e63b0(vector3f const *aim, bool flag);` | `src/bipeds.cpp`; `src/stubs/bipeds.cpp` | `ecx` and `dl`; `aim` is a measurement, not a vector |
| `0xe6460` | `void __stdcall function_e6460(long object_index);` | `src/damage.cpp`; `src/stubs/damage.cpp` | As declared. The comment above the stub, "sets a region's permutation", describes another function |
| `0xe6830` | `bool __stdcall function_e6830(long unit_index);` | `src/bipeds.cpp`, `src/vehicles.cpp`; `src/stubs/vehicles.cpp` | As declared |

Three notes on the code around these functions:

- The three `function_e5930` calls in `0xe24f0`'s switch on the physics mode
  (`src/bipeds.cpp:4031`, `:4045`, `:4049`) are one retail call (`0xe286b`)
  that all three branches reach.
- `biped_eye_raise` (`src/bipeds.cpp`) sets `collision.unknown24` to NONE
  inside its `if`, after `function_30bf0`. Both retail copies of that code
  store it before the `function_11d090` call (`0xdcc9f` in `0xdc5c0`, `0xe5e39`
  here), where `src/unknown_0cafc0.cpp:165` also puts it.
- `0xe5040`'s foot events are types 2 and 3, for markers 0 and 1
  (`0xe51b4`–`0xe51df`), and it ignores types 0 and 1. The comment on
  `s_animation_event` (`include/unknown_1dacb0.h`) says types 0 and 1 are the
  feet, and `animation_events_dispatch` copies the type unchanged
  (`src/unknown_1c62f0.cpp:498`).

## Evidence

- Every entry was disassembled from the retail XBE with capstone 5.0.9, through
  `tools/xbe.py`. Conventions, offsets, flags and constants were read from
  retail code and data, and the callees with source were read from it. Three
  readers analysed the range in three groups, and a verifier checked each
  group's claims against its own disassembly: 92 confirmed and 13 corrected,
  with the corrections applied here, and 1 left open (which body `0x183670`'s
  second point lies on, taken here to be the target's).
- Callers come from an image-wide scan for calls, jumps and absolute pointers;
  no entry is referenced by a pointer. Apart from calls, the only reference to
  an entry is `0xe5930`'s tail jump to `0xe5750`.
- Constants are read from retail `.rdata`: `0x45dbc0` (1.0), `0x45dbbc` (0.5),
  `0x45dbdc` (0.0001), `0x45dc0c` (2.0), `0x45dc1c` (0.05), `0x45dc84` (0.9),
  `0x445634` (−0.25), `0x45dddc` (3.1415927), `0x45dce8` (1.2), `0x45dce4`
  (0.8333333), `0x45dce0` (0.8726646), `0x45dcdc` (0.2617994), `0x45dcd8`
  (−0.5235988), `0x45dcd4` (0.5235988), `0x44ae8c` (0.6), `0x44ae88` (0.8),
  `0x44ae90` (0.3), `0x45dd30` (0.4), `0x45df20` (0.18), `0x45dc8c` (2.25) and
  `0x45ddcc` (127.0); and in `.data`, the pointers `0x4687a4` to (0, 0, 0),
  `0x4687b0` to (0, 0, 1) and `0x4687bc` to (0, 0, −1).
- No document covered the range before this one. Source line numbers are at
  `0f22da1`.
- No SDK or outside dataset was used. Names are the repository's own, or
  describe behaviour.
