# An actor's combat firing update (unknown_1fc7e0)

Retail range covered: `0x1fc7e0`–`0x1fdbe7`. This is one inventory entry, 5,128
retail bytes, `todo` with no source: the per-tick update that moves an actor
through its combat states (`+0x6fe`), tests and aims at its target, and writes
the control flags and scales that `function_1e4390` passes on to the unit.
**Analysis only:** this document adds no source, and nothing in it has been
built or checked against retail with the original compiler. Names are
provisional. The `function_<va>` form is primary; the descriptions are offered
for whoever decompiles it.

It uses the names of the actor's views: `s_actor_view`
(`include/slot_handler.h`), `s_actor_moving` (`include/unknown_1e3920.h`),
`s_actor_datum` (`include/squads.h`), `s_actor_looking_view`
(`src/unknown_050650.cpp`) and `s_actor_control_view`
(`src/unknown_1e3920.cpp`). The combat state's other functions are in
`src/unknown_1fdbf0.cpp`, and the character's weapon entry is named in
`src/ai.cpp`, `src/unknown_1ab3d0.cpp` and `src/unknown_1b7920.cpp`. Props,
units and weapons take the names of `include/props.h`,
`src/unit_object_type.cpp` and `src/weapons.cpp`, and the cluster vector those
of the [object lifecycle](unknown_0b67c0.md) document.
[A weapon barrel's shot](unknown_104150.md) shows how the weapon uses the aim
this function leaves. Three callees have no source and are described only as
far as this function needs them.

## Boundary

- `0x1fc710`, just before, is `matched` with source in
  `src/unknown_1fc2f0.cpp`, and `0x1fdbf0`, just after, is `todo` with source
  in `src/unknown_1fdbf0.cpp`. Both are excluded; this function calls both.
- Lane B's row of the Active claims table (issue #9, `0x1f0000`–`0x1fffff`)
  covers the function. This document makes no claim, and it is offered to lane
  B.
- `0x1fc7e0` has no `@retail` or `@stub` marker.

## Conventions

"Callers" counts the functions that call an entry directly, from outside the
range and then from inside it.

| Retail | Bytes | Arguments | Returns | Callers | What it does |
| --- | --- | --- | --- | --- | --- |
| `0x1fc7e0` | 5128 | stack: actor index (`ret 4`) | void | 1 + 0 | One tick of an actor's combat: its state, target, aim and control outputs |

The actor index is the only argument (`0x1fc7ec`), and the function pops it
(`ret 4` at `0x1fdbb9` and `0x1fdbd1`). No register is read before it is
written. It saves `ebx`, `ebp`, `esi` and `edi`, and keeps `0xa4` bytes of
locals. Whatever is left in `eax` is not a result. The shape is that of a
`void __stdcall` function of one `long` (inferred).

The one caller is `0x1e34e0` (269 bytes, `todo`, no source), the update of one
actor, which `0x1e2f50` calls for each actor whose short `+0x84` is positive
(`0x1e318e`). It pushes the actor index (`0x1e35c8`) and calls this function at
`0x1e35c9` when the actor's byte `+6` is set and `unknown007` (`+7`) is clear,
after `function_25a380`, `function_1f3190`, `0x1f38c0` and `function_298370`.
Right after it, `function_1e4390` (`0x1e35d0`) copies `s_actor_control_view`'s
`flags` (`+0x810`), `first_scale` (`+0x820`) and `second_scale` (`+0x824`) into
the unit's control request (`src/unknown_1e3920.cpp:488`–`491`): these are this
function's outputs. Nothing else in the image refers to `0x1fc7e0`.

Below, "frame `+X`" is `[esp + X]` after the four register pushes (the entry
`esp` − `0xb4`), and "the actor" is the 0x888-byte element of `g_4f55f0` that
`ebp` holds throughout (`0x1fc7f4`–`0x1fc809`). When a call below takes "the
actor" in a register, that register holds the actor's index, not the element
pointer. Where the actor index is needed after `ebx` is reused, it is read
again from the stack (frame `+0xb8`).

## Data

**The actor.** The actors are the 0x888-byte elements of `g_4f55f0`. These are
the fields this function uses outside its own block; the names are
`s_actor_view`'s unless another view is given.

| Offset | Name | Use here |
| --- | --- | --- |
| `+0x18` | `unknown018`, the unit | the unit object, through `g_4e0300` |
| `+0x24` | `unknown024` | a short that `function_1e9720` takes as its team |
| `+0x7c` | `clump_object_index` (`s_actor_datum`) | passed to `function_26b770` |
| `+0x86` | `unknown086` (`alert_state` in `s_actor_looking_view`) | `function_1e4220`'s bool: whether it is below 6 |
| `+0x223` | `unknown223` (`flag223` in `s_actor_datum`) | set: reset |
| `+0x229`, `+0x264` | `unknown229`, `unknown264` (`direction_locked` in `s_actor_looking_view`) | a gate |
| `+0x22c` | `position` in `s_actor_looking_view` | the visibility ray's start; the other views name `+0x238` `position` |
| `+0x265`, `+0x267` | `unknown265`, `unknown267` (`seated` in `s_actor_looking_view`) | gates |
| `+0x26c` | `unknown26c` (`object_index` in `s_actor_looking_view`) | a gate; chooses the ray's flags |
| `+0x338` | `prop_index` | the prop target |
| `+0x488`, `+0x48c` | `unknown488`, `unknown48c` | choose the target type; `unknown488` set is also the first case of the `+0x6d0` step, and `unknown48c` clear brings in the `function_298b60` and `function_298bc0` fire gates |
| `+0x489` | unnamed | with entry `+0x1c`, sets `+0x73b` |
| `+0x48b` | `unknown48b` (`s_actor_moving`), which `function_1fee20` reads as engaged | skips three gates |
| `+0x490` | `unknown490_point`, an `s_type_c3b527` | the point target |
| `+0x4a0`, `+0x4a1`, `+0x4a2` | `unknown4a0`, `unknown4a1`, `unknown4a2` | a gate; the `+0x6d0` choice |
| `+0x5d0`, `+0x5d4` | `unknown5d0` (`movement_aiming` in `s_actor_looking_view`), `unknown5d4` | `+0x5d0` set: `+0x714` = 1 and `+0x720` = 0; `+0x5d4`: a gate |
| `+0x620` | `value620` (`s_actor_datum`) | counts down; positive: reset |
| `+0x6d0` | `unknown6d0` (`s_actor_looking_view`) | written as `function_1e4220` writes it; set: reset |
| `+0x6f8` | `aiming` (`s_actor_looking_view`) | clear: reset |
| `+0x810` | `flags810` (`flags` in `s_actor_control_view`, `output_flags` in `s_actor_looking_view`) | bits 16, 17, 21 and 22, and bit 27 as `function_1e4220` sets it |
| `+0x820`, `+0x824` | `first_scale`, `second_scale` (`s_actor_control_view`) | 1.0 or 0.0 |

**The combat block.** The fields from `+0x6fc` to `+0x7c4` belong to the combat
code. `src/unknown_1fdbf0.cpp` calls `+0x6fe` and `+0x722` the combat state
(`src/unknown_1fdbf0.cpp:2`) and `+0x700` the combat delay in ticks (`:87`).
The source of `function_1dfb90` gives them their first values, state 1 among
them (`src/ai.cpp:1302`).

| Offset | Type | Role |
| --- | --- | --- |
| `+0x6fc` | word (`unknown6fc`, declared `dword`) | cleared each tick; bit 0 set when the weapon's byte `+0x16c` has bit 1, which also stops `function_102540` (`src/weapons.cpp:2212`) |
| `+0x6fe` | short (`unknown6fe`) | the state, 0 to 4 |
| `+0x700` | short (`unknown700`) | the state's timer: counts down, and up while the line of fire is blocked; `function_1fee20`, `function_1fed70` and `0x1fe620` set it |
| `+0x702` | short | counts down; positive resets, unless `+0x48b` is set |
| `+0x704` | short | counts down; ticks to the next shot, at least 2 |
| `+0x706` | short | ticks the line of fire has been blocked, up to 60 seconds' worth |
| `+0x708` | short | counts down; the special mode's cooldown |
| `+0x70a` | short (`unknown70a`) | 3 after a special mode of type 3; positive stops the special mode; `function_1abcf0` counts it down (`src/unknown_1ab3d0.cpp:246`) |
| `+0x70c` | byte | special mode 1 is pending |
| `+0x70d` | byte | aim with barrel 1; `0x1fe620` copies it from `+0x70e` |
| `+0x70e` | byte | special mode 2 was chosen |
| `+0x710` | real (`unknown710`, declared `long`) | the blend factor for `function_1fe1e0`, ramped in states 2 and 3 |
| `+0x714`, `+0x720` | byte, word | set to 1 and 0 while `+0x5d0` is set |
| `+0x718` | long | NONE in state 4; `0x1fe620` stores the game time |
| `+0x722` | short (`unknown722`) | the target type: 0 none, 1 a prop, 2 a point |
| `+0x724` | 16 bytes (`unknown724`, declared long) | the target: a `prop_ref` index, or an `s_type_c3b527` |
| `+0x734` | long | ticks since the target changed |
| `+0x738` | byte | the visibility was 0 or 1 |
| `+0x739` | byte | the prop state's `unknown69` |
| `+0x73a` | byte | cleared with each evaluation; the flag of the second aim |
| `+0x73b` | byte | `+0x489` set and entry `+0x1c` above 0.0 |
| `+0x73c` | byte | for a prop, 1 unless its cluster is active (inferred); 0 for a point |
| `+0x73e` | short | visibility: 0 when the ray hit nothing, 2 or 4 when it hit, or the tracking view's `unknown06` |
| `+0x740` | byte | every gate passed this tick |
| `+0x744` | `s_type_c3b527` | the target point |
| `+0x754` | real | the distance to the target |
| `+0x758` | vector3f (`combat_direction` in `s_actor_looking_view`) | the first aim's direction |
| `+0x764` | real | the first aim's distance (see [Existing declarations](#existing-declarations)) |
| `+0x768` | `s_type_c3b527` | the aim's base, which `0x1fe620` copies from `+0x744` |
| `+0x778` | point3f | the aim point before the offset |
| `+0x784`, `+0x790` | vector3f | an offset added to the aim point, and its step per tick |
| `+0x79c` | point3f | the aim point |
| `+0x7a8` | byte | the second aim's `linear` output was false, as `function_fa6a0` gives it for a projectile with flag bit 1 and a positive `gravity_scale` (`0xfa71a`–`0xfa77a`) |
| `+0x7ac` | real (inferred) | the second aim's projectile speed; `src/ai.cpp:1321` clears it as a real, `function_1fddd0` copies it out as a `long` |
| `+0x7b0` | vector3f | the second aim's direction |
| `+0x7c4` | short | a countdown during which a type-0x16 unit request is tried |

`function_1fddd0` (`src/unknown_1fdbf0.cpp:296`) gives the weapon its aim from
this block: in state 4 it returns true and leaves the direction alone; in state
2 it uses `+0x7b0` and `+0x7ac` when `+0x7a8` is set, and otherwise the
direction from the origin to `+0x79c`.

**The character's weapon entry.** `function_1e5280` returns the entry of the
actor's character, or of a parent character, whose key is the weapon's
definition index: an `s_character_entry` of the block at `+0xcc`
(`src/unknown_1e5240.cpp:51`). This function keeps it in frame `+0x1c` ("the
entry"); several files give its fields names:

| Offset | Name | Use here |
| --- | --- | --- |
| `+0` | `flags` (`s_ai_weapon_properties`, `src/ai.cpp:863`) | bit 1: the `unknown2c4` gate |
| `+0xc` | `range` (the same) | the largest target distance |
| `+0x10` | unnamed | the smallest target distance; the aim point is moved out to it |
| `+0x1c` | unnamed | above 0.0, with `+0x489`, sets `+0x73b` |
| `+0x48`, `+0x4c` | `delay_lower`, `delay_upper` (`s_combat_delay`, `src/unknown_1fdbf0.cpp:78`) | read by `function_1fee20` |
| `+0x78` | unnamed short | the special mode, 1 or 2 |
| `+0x7a` | `unknown7a` (`s_weapon_entry`, `src/unknown_1ab3d0.cpp:54`) | the type passed to `function_1fdbf0` and `function_1fdce0`; 3 sets `+0x70a` |
| `+0x7c` | unnamed real | the special mode's chance, against a random number in [0, 1] (`function_259a0`) |
| `+0x80` | unnamed real | seconds added to the special mode's cooldown |
| `+0x98`, `+0xa4`, `+0xb0` | `difficulty[3]` (`s_character_weapon_1e`, `src/unknown_1b7920.cpp:165`) | per difficulty, three reals: the ramp's start, its end, and the seconds it takes |

The repository declares the three values of each `difficulty` element as
`long`s (`unknown0`, `unknown4`, `unknown8`), but this function reads them as
reals, and `function_1b7cc0` copies `unknown4` into `unknown710`
(`src/unknown_1b7920.cpp:211`).

`function_1e5380` gives another entry: the one of the block at `+0xdc`
(`s_character_entry_dc`, `src/unknown_1e5240.cpp:107`) whose key, the short at
`+4`, is the unit's `current_grenade_index` (`+0x23c`). This function reads
only that key, and keeps the entry in frame `+0x20`.

**The frame.** Several slots are used for more than one thing:

| Slot | What it holds, in order |
| --- | --- |
| `+0x13` | byte: the state may advance; read only by case 0 of the switch |
| `+0x14` | byte: the actor does not face the target; then the second aim's `linear` output; then the value of bits 17 and 22 |
| `+0x15` | byte: the target was evaluated; then set in state 4, or for a clear line with barrel 0 |
| `+0x16` | byte: the target changed |
| `+0x17` | byte: the first `function_1e2030` result; then a clear line with barrel 1 |
| `+0x18` | the unit object; a random real; the visibility period in ticks; the ray's length; the facing threshold |
| `+0x1c` | the entry, or NULL |
| `+0x20` | the `+0xdc` entry; the special mode's seconds; the visibility period as a real (`field_2_3` × 0.33, before `fistp` writes the ticks to `+0x18`); the prop state; a squared distance; the second aim's speed; the blocker |
| `+0x24` | the weapon definition (`function_1fe0e0`); the object `function_1c8d20` ignores; the output scale |
| `+0x28` | the next state, −1 for none |
| `+0x2c` | the target point; the aim base's point; a vector |
| `+0x38` | the weapon (`function_1e1f20`); then the rate of fire |
| `+0x3c` | the ray's vector; the facing vector; the prop's point; the aim vector |
| `+0x48` | the firing origin (`function_1fc710`) |
| `+0x54` | the weapon in the unit's `+0x213` slot (`function_1e1fd0`) |
| `+0x58` | an `s_collision_result_1697c0` (`src/projectiles.cpp:986`) while the target is evaluated; then the `s_combat_blend_values` (`src/unknown_1fdbf0.cpp:184`) |

## The function

There is no loop. The 271 basic blocks run forward; the five backward jumps
(`0x1fc94e`, `0x1fcb21`, `0x1fcb42`, `0x1fcb49`, `0x1fcdf9`) only reach shared
tails. One jump table switches on the state (`0x1fd415`). Most failed tests
jump to one reset at `0x1fd3ed`, which sets the state to 0, and there are two
epilogues, both `ret 4`.

In outline, the state goes from 0 to 1 when the target passes every test, from
1 or 3 to 2 when the actor faces the target and `+0x700` is 0, from 2 to 3 and
from 4 to 0 when `+0x700` is 0, and to 0 on any reset; the state machine below
has the details. This function never sets state 4. The weapon fires in state 4,
and in state 2 once the line of fire is clear (inferred from the outputs and
from `function_1fddd0`).

### Setup (`0x1fc7e0`–`0x1fc8dc`)

1. `ebx` = the actor index (`0x1fc7ec`). `ebp` = the actor: `g_4f55f0`'s data
   (`+0x44`) plus the index's low word × 0x888; `esi` keeps that offset
   (`0x1fc7f4`–`0x1fc809`).
2. The weapon (frame `+0x38`) = `function_1e1f20(actor)`, and frame `+0x54` =
   `function_1e1fd0(actor)`, both with the actor in `eax`
   (`0x1fc80b`–`0x1fc81d`).
3. Frame `+0x1c` and `+0x24` = 0, frame `+0x20` = `function_1e5380(actor)` with
   the actor in `ecx`, frame `+0x28` = −1, and the bytes frame `+0x13` to
   `+0x16` = 0 (`0x1fc821`–`0x1fc85b`). Frame `+0x18` = the unit object, or
   NULL when `unknown018` is NONE (`0x1fc836`–`0x1fc877`).
4. Word `+0x6fc` = 0 (`0x1fc885`). When `+0x5d0` is set, `+0x714` = 1 and word
   `+0x720` = 0 (`0x1fc88e`, `0x1fc895`).
5. With a weapon (`0x1fc89c`): frame `+0x24` = `function_1fe0e0(actor)`, the
   weapon definition (`0x1fc8b9`, stored at `0x1fc8c3`); the entry =
   `function_1e5280(actor, key)`, with the actor in `ecx` and the weapon
   object's definition index (`+0`) on the stack (`0x1fc8be`–`0x1fc8cc`); and
   `+0x6fc` bit 0 is set when the weapon's byte `+0x16c` has bit 1
   (`0x1fc8d0`–`0x1fc8dc`).

### `+0x6d0` and bit 27 (`0x1fc8e3`–`0x1fc95d`)

These steps do what `function_1e4220` does (`src/unknown_1e4290.cpp:56`): set
byte `+0x6d0` and `flags810` bit 27 together, or clear both. The first three
cases are two inlined copies of it; the fourth calls it.

1. `unknown488` set: clear both (`0x1fc8ed`–`0x1fc909`).
2. Else `unknown4a1` set: set both (`0x1fc915`–`0x1fc93a`).
3. Else `unknown4a2` set: clear both, by jumping into the first copy
   (`0x1fc946`–`0x1fc94e`).
4. Else `function_1e4220(actor, unknown086 < 6)`, with the actor in `eax` and
   the bool in `cl` (`0x1fc950`–`0x1fc95d`).

### Timers (`0x1fc962`–`0x1fc9e6`)

1. Frame `+0x17` = `function_1e2030(actor)` (`0x1fc964`, `0x1fc969`).
2. Each of the shorts `+0x700`, `+0x702`, `+0x704`, `+0x708` and `+0x620` is
   decremented when it is positive (`0x1fc96f`–`0x1fc9be`,
   `0x1fc9d9`–`0x1fc9e6`).
3. `di` = the old target type (`+0x722`; `0x1fc9c5`); when it is positive,
   `+0x734` is incremented (`0x1fc9d1`).

### A seat event (`0x1fc9ed`–`0x1fca91`)

1. The event happens when the unit object exists, its `parent_index` (`+0x14`)
   and `parent_seat_index` (`+0x1fc`) are not NONE, the seat's definition
   (`UNIT_SEATS` of the parent's definition, 0xb0 bytes each;
   `include/units.h:16`) has `flags` bit 12, and the unit's `flags_134` has bit
   25 (`0x1fc9ed`–`0x1fca74`).
2. Then word `+0x722` = 0 (`0x1fca83`), and `function_1fb7e0` is called with
   the actor in `ecx`, type `0x38` in `eax`, NULL data in `edx`, and NONE and
   NONE on the stack (`ret 8`; `0x1fca76`–`0x1fca8c`). Its result is not used.
3. Target selection is skipped (`0x1fca91`).

### Target selection (`0x1fca96`–`0x1fcb80`)

Selection is skipped in state 2 (`0x1fca96`).

1. The new type is 2 when `unknown488` and `unknown48c` are set, 1 when
   `unknown488` is set and `prop_index` is not NONE, and otherwise 0
   (`0x1fcaa4`–`0x1fcaca`).
2. Frame `+0x16` (changed) is set when the type differs from the old one
   (`0x1fcad4`). For the same type 1, it is whether `prop_index` differs from
   `+0x724` (`0x1fcae1`–`0x1fcaf2`). For the same type 2, it is whether
   `function_210a30` gives more than 0.25 for `+0x724` (in `eax`) and `+0x490`
   (in `edi`): a squared distance, returned in `xmm0` (`0x1fcb23`–`0x1fcb44`).
3. On a change, `+0x734` = 0 (`0x1fcaf8`).
4. Word `+0x722` = the new type (`0x1fcb06`). Type 1 copies `prop_index` to
   `+0x724` (`0x1fcb0f`, `0x1fcb15`), and type 2 copies the 16 bytes of
   `+0x490` (`0x1fcb51`–`0x1fcb70`), changed or not.
5. Byte `+0x740` = 0, and `function_1fc710` writes the firing origin to frame
   `+0x48`, with the actor in `eax` and the output in `ecx`
   (`0x1fcb73`–`0x1fcb80`).

### The `+0x7c4` countdown (`0x1fcb85`–`0x1fcbfe`)

While the short `+0x7c4` is positive, this branch takes the place of the rest
of the tests, and it always ends in the reset (inferred: a grenade throw, since
the entry it uses is keyed by the unit's grenade type).

1. `function_255ce0(actor)`, with the actor in `edi` (`0x1fcb91`). Per its
   source it checks the target at `+0x7d0` with `function_255b10`
   (`src/unknown_2551c0.cpp:428`). When it is false, `+0x7c4` is decremented
   and the reset follows (`0x1fcb98`, `0x1fcbf7`–`0x1fcbfe`).
2. When frame `+0x20` is not NULL and `function_cdff0(unit, key)` returns 0,
   `function_cccd0(unit, key, 1)` gives the unit one of that grenade type, and
   makes the type current (`src/unit_object_type.cpp:886`;
   `0x1fcb9a`–`0x1fcbbf`). The unit goes in `eax`, the key in `cx` and then
   `dx`, and the 1 on the stack. The key is never NONE here, because
   `function_1e5380` looks up nothing for a NONE `current_grenade_index`
   (`0x1e53c1`) and returns only an entry whose key matches (`0x1e5426`).
3. `function_1e4310(actor)` (`0x1fcbc6`), which per its source makes a unit
   request of type 0x16 and stores the game time at `+0x7c8`
   (`src/unknown_1e4290.cpp:118`). When it is false, `+0x7c4` is decremented
   and the reset follows (`0x1fcbcd`). When it is true: `function_1fb7e0` with
   type `0x1c`, as in the seat event (`0x1fcbcf`–`0x1fcbdc`);
   `function_26b770(clump_object_index)` with the clump in `eax` (`0x1fcbe1`,
   `0x1fcbe4`); `+0x7c4` = 0 (`0x1fcbe9`); and the reset (`0x1fcbf2`).

### First gates (`0x1fcc03`–`0x1fccda`)

1. Each of these resets: `value620` is positive (`0x1fcc0b`);
   `function_1e2030(actor)` is false (`0x1fcc1a`); `+0x6d0` is set
   (`0x1fcc28`); `function_113da0(unit)` is false (`0x1fcc3a`); `unknown26c` is
   not NONE and `unknown5d4` is set (`0x1fcc40`–`0x1fcc51`); the unit object
   exists, its `type` (`+0xaa`) is 0, and `function_e4020(unit)` is true
   (`0x1fcc57`–`0x1fcc72`), which `src/bipeds.cpp:2944` describes as whether
   the biped is landing.
2. In state 4 the other tests are skipped: frame `+0x13` = 1, and it jumps to
   the proximity check with `esi` = the actor index and `edi` = the entry
   (`0x1fcc78`–`0x1fcc95`). Frame `+0x15` is still 0 there, so the check does
   nothing.
3. Otherwise each of these resets: `unknown223` is set (`0x1fcca2`); there is
   no entry (`0x1fccae`); the unit object exists, `unknown267` is clear, entry
   `flags` has bit 1, and the unit's `unknown2c4` is below 1.0
   (`0x1fccb4`–`0x1fccda`).

### The special mode (`0x1fcce0`–`0x1fce01`)

It runs when entry `+0x78` is positive, the state is not 2, and `+0x708` and
`+0x70a` are not positive (`0x1fcce0`–`0x1fcd0b`).

1. `function_1e9720(0x12, team)` is called, with the kind in `eax` and the team
   (`unknown024`) in `cx`; its `xmm0` result is never read
   (`0x1fcd11`–`0x1fcd1a`).
2. Mode 1 calls `function_1e9720(0x11, team)` the same way, again unread, and
   needs the weapon definition's `barrel_count` (`+0x2d0`) above 0; mode 2
   needs it above 1; other values need nothing (`0x1fcd1f`–`0x1fcd3d`,
   `0x1fcddf`–`0x1fcdff`).
3. `function_1fdbf0(actor, entry +0x7a)`, with the actor in `eax` and the type
   on the stack (`0x1fcd43`–`0x1fcd4c`). When it is false, the special mode
   does nothing more (`0x1fcd53`).
4. The seconds are `function_259d0` with the seed `g_4e7408` in `ecx` and the
   bounds 0.0 and 1.5 on the stack, plus entry `+0x80` (`0x1fcd59`–`0x1fcd75`).
   r = `function_259a0(seed)` (`0x1fcd79`). `+0x708` =
   `function_1469f0(seconds)`, the seconds in ticks (`0x1fcd86`–`0x1fcd90`).
5. When entry `+0x7c` is above r and `function_1fdce0(actor, entry +0x7a)` is
   true, with the actor in `edi` (`0x1fcd97`–`0x1fcdb4`): `+0x70a` = 3 when
   entry `+0x7a` is 3 (`0x1fcdc5`); then `+0x70c` = 1 in mode 1 (`0x1fce01`),
   or `+0x70e` = 1 in mode 2 (`0x1fcdd6`).

`function_1e9720` and the functions it calls store nothing to memory, so the
two calls have no effect (inferred: the source discards their results).

### Target evaluation (`0x1fce08`–`0x1fd197`)

It runs only while `+0x722` is positive. Otherwise `esi` and `edi` are set to
the actor index and the entry, and the may-fire gates follow (`0x1fce10`,
`0x1fd199`).

1. The visibility test's period in ticks is `g_510c54->field_2_3` × 0.33,
   rounded by `fistp` into frame `+0x18` (`0x1fce16`–`0x1fce35`): the shape of
   `ai_seconds_to_ticks(0.33f)` (`src/ai.cpp:877`).
2. A prop (type 1; `0x1fce47`–`0x1fcfe4`). The `prop_ref` element of `+0x724`
   (`g_502418`, 0x3c bytes each) gives the prop state (`function_25d690`, kept
   in frame `+0x20`) and the tracking view (`function_25d740`, in `ebx`, or
   NULL), each with the element in `ecx`. `+0x754` = the element's `unknown28`
   (`0x1fce77`, `0x1fce85`). `0x200110` writes the target point to frame
   `+0x2c`, and the target, as an `s_type_c3b527`, to `+0x744` (see
   [Callees without source](#callees-without-source)); its result is not used
   (`0x1fce7a`–`0x1fce95`). Frame `+0x15` = 1 (`0x1fce9c`). With a view,
   `+0x73e` = its `unknown06` (`0x1fcea3`); without one, the ray test (step 5)
   runs when `+0x734` is a multiple of the period (`0x1fceac`–`0x1fceb9`).
3. Still for a prop: `+0x739` = the state's `unknown69` (`0x1fcf9d`,
   `0x1fcfa0`). `+0x73c` = 1, unless the state's short `+0x2c` is not NONE;
   then it is 1 when that bit of the cluster vector at `g_4e6948` `+0x11b8` is
   clear, and 0 when it is set (`0x1fcfa6`–`0x1fcfde`). That vector is the set
   of active clusters of the [object lifecycle](unknown_0b67c0.md), so `+0x2c`
   is the prop's cluster (inferred).
4. A point (any other type; `0x1fcfe9`–`0x1fd049`). `+0x724` is copied to
   `+0x744` (`0x1fcfe9`–`0x1fd012`). `+0x754` = `function_210ac0`, the distance
   from `+0x724` (in `eax`) to `+0x22c` (in `edi`), returned on the x87 stack
   (`0x1fd015`, `0x1fd01a`). `+0x739` and `+0x73c` = 0 (`0x1fd024`,
   `0x1fd02b`). `function_210850` writes the target point to frame `+0x2c` from
   `+0x744`, with the input in `esi` and the output in `eax` (`0x1fd032`).
   Frame `+0x15` = 1 (`0x1fd042`). The ray test runs when `+0x734` is a
   multiple of the period.
5. The ray test is inlined twice (`0x1fcebf`–`0x1fcf92` and
   `0x1fd04f`–`0x1fd11c`). The flags are
   `actor->unknown26c != NONE ? 0x15808c0f : 0x15808c2f`. The result's short
   `+0x24` is set to NONE (`0x1fced5`), and the vector is the target point
   minus `+0x22c`. `function_1697c0` takes the flags, `+0x22c`, the vector,
   NONE, NONE and the result at frame `+0x58`, all on the stack (`ret 0x18`;
   `0x1fcf26`, `0x1fd0b0`). With no hit, `+0x73e` = 0. With a hit, d = the
   length of the vector (x87 `fsqrt`, kept in frame `+0x18`), and `+0x73e` = 4
   when d is below 1.0, else 2 when d × the result's `t` (`+4`) is below 1.0,
   else 4 (`0x1fcf33`–`0x1fcf92`).
6. `+0x73a` = 0 (`0x1fd12f`), and `+0x73b` = `+0x489` set and entry `+0x1c`
   above 0.0 (`0x1fd123`–`0x1fd15a`).
7. The first aim calls `function_1ffbd0` with the actor in `eax` and, on the
   stack, the weapon, barrel 0, the origin (frame `+0x48`), the target point
   (frame `+0x2c`), false, `+0x758` for the direction, NULL, `+0x764`, NULL and
   NULL (`ret 0x28`; `0x1fd14f`–`0x1fd185`). When it returns false, `+0x722` =
   0 (`0x1fd18e`).

### May-fire gates (`0x1fd1a4`–`0x1fd37b`)

1. Each of these resets: `+0x722` is 0 (`0x1fd1ac`); `+0x73c` is set
   (`0x1fd1ba`); `+0x48b` is clear and `+0x702` is positive
   (`0x1fd1c0`–`0x1fd1d2`); `function_1f50e0(actor)` is true (`0x1fd1e1`);
   `+0x48b` is clear, `unknown264` is set and `unknown229` is clear
   (`0x1fd1e7`–`0x1fd203`); `+0x739` is set (`0x1fd211`); `unknown265` is set
   (`0x1fd21f`); `aiming` is clear (`0x1fd22d`); `unknown48c` is clear and
   `function_298b60(actor)` or `function_298bc0(actor)`, each with the actor in
   `ecx`, is false (`0x1fd233`–`0x1fd255`).
2. In state 2 the other tests are skipped: frame `+0x13` = 1, and it jumps to
   the proximity check (`0x1fd25b`–`0x1fd26a`).
3. `+0x738` = whether `+0x73e` is 0 or 1 (`0x1fd26f`–`0x1fd292`). It resets
   when `unknown4a0`, `+0x738` and `+0x73b` are all clear
   (`0x1fd28a`–`0x1fd2a6`).
4. When `+0x48b` is clear and frame `+0x17` is set, it resets when `+0x754` is
   at least entry `range` (`0x1fd2be`–`0x1fd2ca`), or at most entry `+0x10`
   (`0x1fd2dc`–`0x1fd2e8`).
5. Every gate has passed: frame `+0x13` = 1 and `+0x740` = 1 (`0x1fd2fd`,
   `0x1fd302`). The facing threshold is 0.70710677 plus `+0x754` × 0.17526217
   when `+0x754` is below 1.5, and 0.97 from there on, where the two meet
   (`0x1fd2ee`–`0x1fd325`); it is kept in frame `+0x18`.
6. `function_1fe120` writes the facing vector to frame `+0x3c`, with the actor
   in `eax` and the output in `ecx` (`0x1fd339`). Frame `+0x14` = 1 when the
   dot product of `+0x758` and that vector is below the threshold
   (`0x1fd33e`–`0x1fd37b`).

### The proximity check and the reset (`0x1fd380`–`0x1fd3fd`)

1. The check needs `+0x722` positive, frame `+0x15` set, `barrel_count` above
   0, barrel 0's `projectile_definition_index` (`barrels` `+0x90`) not NONE,
   and that projectile definition's real at `+0xd0` above 0.0
   (`0x1fd380`–`0x1fd3d1`); `s_projectile_definition` leaves it inside
   `unknownc4` (`src/projectiles.cpp:111`).
2. Then `function_255d60` takes the actor in `eax`, 0.0 in `xmm0`, the real at
   `+0xd0` plus 1.0 in `xmm1`, and the target point and NULL on the stack
   (`ret 8`; `0x1fd3d3`–`0x1fd3e4`). True resets (`0x1fd3eb`). Per its source
   (`src/unknown_2551c0.cpp:268`), it is true when a unit whose team is not the
   actor's enemy, or, when `team_is_enemy(team, 1)` is false, a player's unit,
   lies within the larger of the two radii of the point (inferred: so as not to
   hit friends).
3. The reset: frame `+0x13` = 0 (`0x1fd3ed`); `esi` = the actor index, `edi` =
   the entry, and word `+0x6fe` = 0 (`0x1fd3f2`–`0x1fd3fd`). Twenty-three
   jumps, one of them the `+0x7c4` countdown's success path, and the
   fall-through from `0x1fd3eb` reach `0x1fd3ed`; the countdown's decrement
   path jumps to `0x1fd3f2`.

### The state machine (`0x1fd406`–`0x1fd5bc`)

1. `cx` = the state as it is after any reset (`0x1fd406`); `movsx` widens it
   (`0x1fd40d`), and values above 4, negative ones included, skip the switch
   (`ja` at `0x1fd413`). The table at `0x1fdbd4` holds `0x1fd41c`, `0x1fd42e`,
   `0x1fd44a`, `0x1fd42e` and `0x1fd45e`, and every case ends at `0x1fd470`.
2. The cases set the next state (frame `+0x28`): state 0 gives 1 when frame
   `+0x13` is set; states 1 and 3 give 2 when frame `+0x14` is clear and
   `+0x700` is 0; state 2 gives 3, and state 4 gives 0, when `+0x700` is 0.
3. Without an entry it goes straight to the transition (`0x1fd472`). With one,
   the difficulty is `g_4e6948->difficulty` when `g_4e6948->state` is 1, else
   1, and it picks the entry's `difficulty` element at `+0xa4` for 2, `+0xb0`
   for 3, and `+0x98` otherwise (`0x1fd478`–`0x1fd4ac`). Call its reals a, b
   and c.
4. When frame `+0x16` is set or the state is 0, 1 or 4, `+0x710` = a
   (`0x1fd4fd`). Otherwise `+0x710` grows by (b − a) / (`field_2_3` × c), so
   that it climbs from a to b in c seconds (`0x1fd4cb`–`0x1fd4f3`). It is then
   pinned to [0, b] (`0x1fd505`–`0x1fd534`).
5. `function_1fe1e0` blends the character's two `s_combat_blend_values` by
   `+0x710` into frame `+0x58`, with the actor in `ecx`, the blend in `xmm1`
   and the output in `eax` (`0x1fd52e`–`0x1fd53c`). When it returns false in
   state 0, the transition is skipped and a next state of 1 is dropped
   (`0x1fd54d`); in any other state the next state becomes 0
   (`0x1fd54f`–`0x1fd555`).
6. When the next state is not −1 (`0x1fd55b`): for 3, `function_1fed70` sets
   `+0x700` from the blend values, with the actor in `eax` and the values in
   `ecx` (`0x1fd56d`–`0x1fd573`); for 2, `0x1fe620` runs with the actor in
   `eax`, the blend values in `ebx` and the entry on the stack
   (`0x1fd57a`–`0x1fd5ae`); for 1, `function_1fee20` starts the combat delay,
   with the actor in `eax` and the entry in `ecx` (`0x1fd581`–`0x1fd585`), and
   when it returns false and frame `+0x14` is clear, the next state becomes 2
   and `0x1fe620` runs as well (`0x1fd58e`–`0x1fd5ae`).
7. Word `+0x6fe` = the next state (`0x1fd5b7`, `0x1fd5bc`).

### State 2: the aim point and the line of fire (`0x1fd5c3`–`0x1fd9dc`)

1. Without an entry the function returns here (`0x1fd5c5`): bits 16, 17, 21 and
   22 of `+0x810` and the scales at `+0x820` and `+0x824` keep their values,
   while bit 27 has already been rewritten this tick by the `+0x6d0` step.
2. Frame `+0x15` = 0, frame `+0x17` = 0 and `+0x7a8` = 0
   (`0x1fd5d6`–`0x1fd5e0`). In state 4, frame `+0x15` = 1 and `+0x718` = NONE
   (`0x1fd5e9`, `0x1fd5ee`), and the outputs follow; so they do in every state
   but 2 (`0x1fd601`).
3. Frame `+0x24` = NONE, and `function_210850` writes `+0x778` from `+0x768`
   (`0x1fd607`–`0x1fd61d`).
4. For a prop (type 1): `ebx` = the prop state (`function_25d690`), whose
   `unknown40` goes to frame `+0x24` (`0x1fd630`–`0x1fd655`), and s =
   `function_1e9700(15)` plus blend `field_4` (`0x1fd653`–`0x1fd65e`);
   `function_1e9700` takes the row on the stack and returns `xmm0`. The test of
   s has two steps, s ≥ 1.0 or s > 0.0 (`0x1fd664`–`0x1fd678`), which is how a
   test of s pinned to [0, 1] being above 0 compiles (inferred).
5. When s passes and `+0x73b` is clear, the aim follows the prop:
   `function_210850` writes `+0x768`'s point to frame `+0x2c`, `0x200110`
   writes the prop's point to frame `+0x3c`, with NULL for its second output,
   and `+0x778` grows by (frame `+0x3c` − frame `+0x2c`) × `field_4`, not × s
   (`0x1fd67e`–`0x1fd6fa`).
6. For a prop, when blend `field_8` passes the same two-step test
   (`0x1fd6ff`–`0x1fd719`), the aim leads it: t =
   `weapon_barrel_estimate_time_to_target`, with the weapon in `eax`, `+0x70d`
   ≠ 0 as the barrel in `edx`, and `+0x764` as the distance on the stack
   (`0x1fd71b`–`0x1fd733`), and `+0x778` grows by the prop state's reals at
   `+0x1c`, `+0x20` and `+0x24` × t × `field_8` (`0x1fd738`–`0x1fd778`)
   (inferred: the prop's velocity).
7. `+0x784` grows by `+0x790`, and `+0x79c` = `+0x778` plus `+0x784`
   (`0x1fd77d`–`0x1fd801`). With `+0x70d` set, `function_1fc710` writes the
   origin again (`0x1fd806`–`0x1fd816`).
8. d2, the squared distance from the origin to `+0x79c`, goes to frame `+0x20`
   (`0x1fd81b`–`0x1fd874`). When it is below entry `+0x10` squared, v =
   `+0x79c` − the origin is normalized by `function_30bf0` (vector in `eax`,
   length returned in `xmm0`), and when the length is above 0, `+0x79c` grows
   by v × (entry `+0x10` − √d2): the aim point is moved out to that distance
   (`0x1fd87c`–`0x1fd8e5`).
9. The second aim calls `function_1ffbd0` with the actor in `eax` and, on the
   stack, the weapon, `+0x70d` ≠ 0 as the barrel, the origin, `+0x79c`,
   `+0x73a`, `+0x7b0` for the direction, NULL, NULL, frame `+0x20` and frame
   `+0x14` (`0x1fd8e7`–`0x1fd920`). Its result is not used. `+0x7a8` = frame
   `+0x14` is 0, and when `+0x7a8` is set (frame `+0x14` clear), `+0x7ac` =
   frame `+0x20` (`0x1fd925`–`0x1fd93e`).
10. Frame `+0x3c` = `+0x79c` − the origin. `function_1c8d20` tests the line of
    fire against the capsules that `function_1c89b0` lists, with the actor in
    `ecx`, that vector in `eax`, and on the stack the object to ignore (frame
    `+0x24`), the origin and frame `+0x20` for the blocker (`ret 0xc`;
    `0x1fd946`–`0x1fd98d`). The blocker is never read.
11. Clear (true): `+0x706` = 0, and frame `+0x17` = 1 when `+0x70d` is set,
    else frame `+0x15` = 1 (`0x1fd992`–`0x1fd9b5`). Blocked: `+0x706` is
    incremented while it is below `field_2_3` × 60, and `+0x700` is incremented
    (`0x1fd9b7`–`0x1fd9dc`).

### The outputs (`0x1fd9e3`–`0x1fdbd1`)

1. `bl` = 0, frame `+0x14` = 0 and the scale (frame `+0x24`) = 0.0
   (`0x1fd9e3`–`0x1fd9f2`).
2. When frame `+0x15` is set, the rate is blend `field_0`, kept in frame
   `+0x38` (`0x1fda07`, `0x1fda0d`). It is doubled (`0x1fda59`) when
   `g_4e6948->state` is 1, the byte at `0x4f55e9` is set, and the team
   (`unknown024`) is NONE, lies outside 0 to 15, or lacks bit 16 plus the team
   in `g_4f55ec`'s `peace_bits` (`+0xc4`), which `0xbfe60` tests with the bits
   on the stack and the bit number in `edx` (`0x1fd9fe`–`0x1fda51`). That is
   the test of `team_is_enemy(1, team)` (`include/slot_handler.h:504`), inlined
   (inferred), and `function_1e9720` reads the same bit as "enemy"
   (`src/unknown_1e9700.cpp:323`).
3. Still with frame `+0x15` set: when `+0x70c` is set, it is cleared and bit 16
   is cleared (`0x1fda67`, `0x1fdb29`–`0x1fdb36`). Else, with a rate below
   0.0001, bit 16 is set, the scale is 1.0 and `bl` = 1 (`0x1fda75`–`0x1fda86`,
   `0x1fdb0c`–`0x1fdb27`). Else, with `+0x704` not 0, bit 16 is cleared
   (`0x1fda8c`). Else bit 16 is set, the scale is 1.0, `bl` = 1, and `+0x704` =
   `function_1469f0(1.0 / rate)`, at least 2 (`0x1fda9a`–`0x1fdaed`).
4. When frame `+0x15` is clear and frame `+0x17` is set, `+0x70c` is cleared
   when it is set, else frame `+0x14` = 1; bit 16 is cleared either way
   (`0x1fdaef`–`0x1fdb06`). When both are clear, a set `+0x70c` sets bit 16,
   the scale to 1.0 and `bl` to 1, and `+0x70c` stays set
   (`0x1fdb08`–`0x1fdb27`); otherwise bit 16 is cleared.
5. The stores: `flags810` (`0x1fdb41`), `first_scale` = the scale (`0x1fdb53`),
   and bit 17 = frame `+0x14` (`0x1fdb47`–`0x1fdb6b`).
6. When frame `+0x54` is not NONE, bit 21 = `bl`, bit 22 = frame `+0x14`, and
   `second_scale` = the scale (`0x1fdb78`–`0x1fdbc1`); otherwise they keep
   their values (`0x1fdb76`). The epilogues are at `0x1fdba4` and `0x1fdbc7`.

So bit 16 is set exactly when `bl` is, and the scales are then 1.0. Bits 21, 22
and `second_scale` repeat bits 16, 17 and `first_scale` when the unit holds a
weapon in its `+0x213` slot (inferred: for a second weapon;
`src/unknown_1e1f20.cpp:44` calls that weapon `secondary_weapon_get`, while
`s_unit` names `+0x213` `next_weapon_index`). The rate is shots per second,
since `+0x704` becomes the ticks between shots (inferred). Mode 1 keeps bit 16
set on every tick that does not fire and clears it on the tick that would
(inferred: the trigger is held, as for a charged shot), and bit 17 marks a shot
with barrel 1, which mode 2 asks for (inferred).

## Callees without source

- `0x1697c0` casts a ray. It takes its six arguments on the stack and pops them
  (`ret 0x18`): the collision flags, the start, the vector, two objects to
  ignore (NONE and NONE here) and the result, an `s_collision_result_1697c0`
  whose `t` (`+4`) this function reads. It returns a bool in `al`. It has a
  `@stub` in `src/stubs/lane_c.cpp`, and `src/unknown_1689b0.cpp` holds source
  for it that is left out of the build.
- `0x200110` (303 bytes) gives the point of a prop: the actor in `eax`, the
  `prop_ref` index in `ecx`, and on the stack a point output and an optional
  `s_type_c3b527` output (`ret 8`). The point is the position of the actor's
  `target_marker` (`+0x33c`, `s_actor_looking_view`) on the root
  (`function_baf80`) of the prop's object, when `target_marker_valid`
  (`+0x340`) is set, `+0x338` is this reference, the marker is not 0 and
  `function_b8d30` finds it (`0x200176`–`0x2001c3`); otherwise it is the prop
  state's `+0x10` (`0x2001c5`–`0x2001d4`). For the second output it calls
  `function_26c2d0` (a `@stub` in `src/stubs/lane_i.cpp`) with the reference in
  `eax`, converts the point with `function_210690`, and only when that succeeds
  sets `output_index` (`+0xc`) to the state's `unknown54` and returns true
  (`0x200203`–`0x20022b`). Without the second output it returns true.
- `0x1fe620` (1,861 bytes) sets up state 2: the actor in `eax`, the blend
  values in `ebx` and the entry on the stack (`ret 4`). It clears `+0x70e` when
  `function_1fdce0(actor, entry +0x7a)` refuses it, then moves it to `+0x70d`
  (`0x1fe642`–`0x1fe67c`). It sets `+0x700` to a random time between blend
  `field_20` and `field_24` seconds (`0x1fe683`–`0x1fe6e4`; later it may scale
  that timer, by a ratio at `0x1feb71` or by 1.5 at `0x1feba2`), `+0x718` to
  the game time and `+0x714` to `+0x5d0` (`0x1fe6f4`, `0x1fe6fa`), and writes
  `+0x71c`, `+0x7bc` and `+0x7c0`. Last it copies `+0x744` to `+0x768` and sets
  `+0x784`, `+0x790` and `+0x79c` (`0x1fecc5`–`0x1fed56`).

## Existing declarations

Nothing declares `0x1fc7e0`: the only mentions of it in the repository are its
own row in `config/functions.csv` and `0x1e34e0`'s list of calls there. Every
callee with source is declared with at least as many parameters as retail
passes; none has fewer. Where retail passes a parameter in a register that the
declaration does not mark, that is the LTCG convention. Three declarations
differ from retail in other ways:

| Retail | Declared as | Where | What retail shows |
| --- | --- | --- | --- |
| `0x1ffbd0` | `bool function_1ffbd0(long actor_index, long weapon_index, short barrel_index, point3f const *origin, point3f const *target, bool flag, vector3f *direction, void *unknown0, void *unknown1, void *unknown2, void *unknown3)` | `src/unknown_1ff010.cpp` | Eleven parameters, as in retail: the actor in `eax` and ten on the stack (`ret 0x28`). Retail hands `unknown0`, `unknown1` and `unknown2` to `weapon_barrel_aim` as its `time`, `distance` and `speed_out` (`0x1ffd61`, `0x1ffd82`, `0x1ffd86`), while `src/unknown_1ff010.cpp:67` and `:70` pass them as `speed_out`, `time` and `distance`. The names hold further down: `weapon_barrel_aim` passes the three to `function_fa6a0` in the same places (`0x100e3f`–`0x100e49`), and for a projectile without gravity `function_fa6a0` passes them to `function_fa580` (`0xfa6c0`, `0xfa78f`–`0xfa799`), which stores the length over the speed, the length and the speed in them (`0xfa688`, `0xfa676`, `0xfa67e`). So `+0x764` is a distance, as its use with `weapon_barrel_estimate_time_to_target` expects, and `+0x7ac` a speed, as [A weapon barrel's shot](unknown_104150.md) reads it |
| `0x1c8d20` | `bool function_1c8d20(long actor_index, point3f const *origin, long excluded_object, vector3f const *direction, long *blocking_index)` | `src/ai.cpp` | Five parameters, as in retail: the actor in `ecx`, the direction in `eax`, and three on the stack (`ret 0xc`). Retail's first stack argument is the object to ignore, which it compares with each capsule's object (`0x1c8d60`–`0x1c8d70`), and its second is the origin (`0x1c8d2a`); the declaration lists `origin` first |
| `0x259d0` | `real function_259d0(dword *seed, char const *file, long line, real lower_bound, real upper_bound)` | `include/unknown_0259d0.h` | The seed in `ecx` and the two bounds on the stack (`ret 8`); no file or line is passed. `function_259d0` is `matched`, and `function_1fee20`, also `matched`, calls it with `__FILE__` and `__LINE__` (`src/unknown_1fdbf0.cpp:98`) |

## Evidence

- The function and its callees were disassembled from the retail XBE with
  capstone 5.0.9, through `tools/xbe.py`. Conventions, stack slots, offsets and
  constants were read from retail code and data, with a control-flow graph that
  tracks the stack depth (`0xb4` on every path) and a check of which registers
  are read before they are written. A reader analysed the function, and a
  verifier checked each claim with its own disassembly: 24 confirmed and 6
  corrected, with the corrections applied here. A third agent wrote this text
  from that work, and a fourth checked the finished document against retail and
  the repository: 480 claims confirmed and 9 corrected or reworded, with the
  changes applied here. The writer then traced `function_1ffbd0`'s outputs
  through `weapon_barrel_aim` and `function_fa6a0` to `function_fa580`, and
  read the sources cited.
- Callers come from an image-wide scan of every section for E8 and E9 rel32
  calls and jumps, 0F 80–8F rel32 jumps and absolute dwords. The only reference
  to `0x1fc7e0` is the call at `0x1e35c9`, and the only reference to `0x1e34e0`
  is the call at `0x1e318e`, in `0x1e2f50`.
- Constants are read from retail `.rdata`: `0x45dc20` (0.25), `0x45dbc0` (1.0),
  `0x45e1f4` (0.33), `0x45dbd8` (0.0), `0x43ff08` (1.5), `0x45e550`
  (0.17526217), `0x45e22c` (0.70710677), `0x45e54c` (0.97), `0x45dc0c` (2.0)
  and `0x45dbdc` (0.0001). The jump table at `0x1fdbd4` is the last 20 bytes of
  the function, and the bounds 0.0 and 1.5 for `function_259d0` are pushed as
  immediates (`0x1fcd5f`, `0x1fcd64`).
- Source line numbers are at `9feed5f`.
- No SDK or outside dataset was used. Names are the repository's own, or
  describe behaviour.
