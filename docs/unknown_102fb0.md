# Taking a barrel's shot (unknown_102fb0)

Retail range covered: `0x102fb0`–`0x103994`. This is one inventory entry, 2,533
retail bytes, `todo` with no source: the function that takes one barrel's shot,
using up its rounds and marking it for [`0x104150`](unknown_104150.md), which
creates the projectiles. **Analysis only:** this document adds no source, and
nothing in it has been built or checked against retail with the original
compiler. Names are provisional. It is written for lane S, whose claim covers
the function, and makes no claim of its own.

It sits between the [weapon object type](unknown_0fd910.md)'s update, which
calls it when a barrel's shot is due, and `0x104150`. It uses the names of
`src/weapons.cpp` and of the object documents ([part 1](unknown_0b7740.md),
[part 2](unknown_0bc190.md)). Two of its callees have no source and are
described only as far as this function needs them.

## Boundary

- `0x102f70`, just before, is matched, and `0x1039a0`, just after, is `todo`
  with source; both are in `src/weapons.cpp` and excluded.
- Lane S's row of the Active claims table (issue #9, `0x100000`–`0x10ffff`)
  covers the function. No open pull request touches it.
- `0x102fb0` has no `@retail` or `@stub` marker, declaration or stub at
  `5c3f7a9`.

## Conventions

"Callers" counts direct callers.

| Retail | Bytes | Arguments | Returns | Callers | What it does |
| --- | --- | --- | --- | --- | --- |
| `0x102fb0` | 2533 | stack: weapon, barrel index (16 bits) (`ret 8`) | | 2 | Takes one barrel's shot: uses up its rounds and extra shots, raises its heat, starts its animations and the holder's reactions, marks it for `0x104150` unless it misfires, and plays its firing effect |

The weapon update `0xfdfb0` calls it at `0xff1bd`, in barrel state 1 when
`0x102100` says a shot is due, unless that call's first output is set while
barrel flag0 is clear; it clears flag0 first. `0xa2ba0`, slot 11 of the
weapon-fire event's vtable (`0x452504`), calls it at `0xa2de1` to replay a shot
that `0xa7d50` sent, then stops the barrel (`0x103e60`) and fires it
(`0xfd980`). Neither reads a result.

## Data

The frame is `0x1c60` bytes, reserved through `__chkstk` (`0x320560`), and
holds 64 object markers (`s_object_marker`) at `esp+0x70` and an effect marker
(`s_effect_marker`) at `esp+0x54`, for the firing effect.

Fields this function uses that `src/weapons.cpp` leaves unnamed:

| Field | Use |
| --- | --- |
| weapon `+0x180` | raised by barrel definition `+0xa4` per shot, to at most 1.0, or to definition `+0x154` when item_flag3 is clear; the update's overheat starts at `+0x154`; above definition `+0x158`, a draw below `+0x15c` can destroy the weapon |
| weapon `+0x248` | the game time of the last shot taken |
| weapon `+0x16c` bits 0 and 3 | a shot waiting for `0x104150`; set for barrel 1 when the definition's reload_style is 3 |
| weapon barrel `+0x4` bits 2, 6 and 7 | bit 2 refuses a magazine shot; bits 6 and 7 mark a refused shot of a barrel with firing effects |
| weapon barrel `+0x8`, `+0xa`, `+0xc` | the firing effects used (a mask), the current one, and the shots left on it |
| weapon barrel `+0x18` | set to 1.0 by a shot when barrel definition `+0xa0` is positive (`0x1035e9`) |
| weapon barrel `+0x20` | the input of the kick's curve (`0x1037d3`); the update ramps it ([weapon type](unknown_0fd910.md)) |
| weapon barrel `+0x24` | the float `0x102100` accumulates (s_object_link.accumulator); its whole part is the extra shots |
| weapon barrel `+0x2c`, `+0x2e`, `+0x30` | the extra shots for `0x104150`, the marker turn, and the effect `0x1039a0` keeps |
| weapon definition `+0x25c`, `+0x260` | the misfire start on heat, and its scale |
| weapon definition `+0x27c`, `+0x280` | the amount and limit that `0xd0e60` applies to the holder's `+0x2b0` and `+0x2b8` |
| barrel definition `+0x36` | a value for the AI event (`0xbfc30`) |
| barrel definition `+0xb0`, `+0xb4`, `+0xc0` | the kick's range and its curve |
| firing effect `+0x0`, `+0x2` | how many shots an effect lasts, as a range |
| firing effect `+0x8`, `+0x10`, `+0x18` and `+0x20`, `+0x28`, `+0x30` | for a shot, a misfire and a refused shot: the tag played on the weapon, and the damage given to the holder |

The flags that matter are barrel definition flag1 (start at a random firing
effect), flag2 (fire with fewer rounds than a shot needs), flag4 (leave value14
alone) and flag12 (play the effect at a marker), and weapon definition flag11
(heat at 1.0 stops the weapon), flag19 (a vehicle's `0xd1210` can stop it) and
flag26 (no heat from shots).

Two names read differently from how the fields behave here. Barrel definition
`+0x2a` is rounds_minimum and `+0x2c` rounds_per_shot, but a shot uses `+0x2a`
rounds (times the extra shots plus one) and needs `+0x2c` loaded;
`function_102540` uses them the same way (`src/weapons.cpp:2221`, 2224), so the
names may be swapped. Weapon `+0x184`, heat, only rises here, stops a flag11
weapon at 1.0, gives the misfire chance, and at 1.0 makes `0x101db0` switch the
weapon to its overheated_definition_index; inferred: it is a charge that firing
uses up, while `+0x180` is the heat that cools.

## The function

It has no jump table and two exits, and its only loops choose the firing
effect. In order: whether the shot can be taken, the firing effect and the
misfire, the shot itself, then three steps that run on every call.

### Whether the shot is taken (`0x102fb0`–`0x103268`)

1. It finds the weapon, its definition and the barrel definition, without
   checking the barrel index against barrel_count (the replay checks it,
   `0xa2c85`), and the holder: unit_index when in_inventory is set, else NONE.
   `0xb7360` wakes the weapon and its parents.
2. Extra shots: when barrel `+0x24` is at least 1.0, its whole part (`_floor`,
   `__ftol2`) is the extra shots, and the shot needs barrel definition `+0x2a`
   × (extra + 1) rounds.
3. With a magazine (barrel definition magazine_index), rounds_loaded is
   checked. If it falls short of the extra shots but holds one shot's `+0x2a`,
   the extra shots are dropped. The shot is refused when: the rounds still fall
   short, unless the barrel definition has flag2; the definition has flag11 and
   heat is at least 1.0, unless `0x106030` exempts the weapon (a juggernaut's,
   through `function_159dd0`); fewer than barrel definition `+0x2c` rounds are
   loaded; the definition has flag19, the holder is a vehicle and `0xd1210`
   gives it a positive value; or barrel `+0x4` bit 2 is set.
4. A taken magazine shot uses its rounds only when item_flag3 is set, which
   `function_10ccc0` sets for a weapon held by a unit with a player
   (`src/items.cpp:336`); rounds_loaded never goes below 0. When rounds remain
   and the magazine definition has `+0x0` bit 1, `0x105a80` puts the magazine
   in state 5. The reserve, rounds_unloaded, is not touched.
5. Without a magazine, only the flag11 heat test applies, with no exemption.
6. A taken shot calls `0xa7cd0`, which in mode 5 marks the holding unit's
   simulation entity for this weapon's slot.

### The firing effect and the misfire (`0x10326d`–`0x1034ce`)

This block runs only when the barrel definition has firing effects
(firing_effect_count, `+0xe4`), and whether or not the shot was taken. Without
firing effects there is no misfire, and the effect tag stays NONE.

1. Barrel `+0xc` counts the shots left on the current firing effect, `+0xa`.
   When none are left, the next effect not yet in the mask `+0x8` is chosen,
   starting from a random one with barrel definition flag1; the mask is cleared
   once every effect has been used. `+0xc` becomes a count drawn between the
   effect's `+0x0` and `+0x2` (below `+0x2` when it is the larger), and the
   choice repeats while that count is not positive, until it comes back to the
   effect it started from. Then `+0xc` is lowered by one.
2. The misfire: with m = weapon definition `+0x25c` and k = `+0x260`, and only
   when 0 < m < 1 and heat is above m, the shot misfires when (heat - m) × k /
   (1 - m) is greater than a draw between 0 and 1. The draw is made even for a
   refused shot.
3. The slot: a refused shot sets barrel `+0x4` bits 6 and 7 and uses slot 2,
   with scales 1.0 and 0.0; a misfire uses slot 1, with barrel `+0x10` and 0.0;
   a shot uses slot 0, with barrel `+0x10` and weapon `+0x180` / definition
   `+0x154` (0.0 when `+0x154` is 0). The slot selects the effect's tag at
   `+0x8` and its damage at `+0x20`, each plus 8 × slot. While bit 6 is set,
   `0x102100` reports no shot due.

Every draw in this function (the effect's start and count, the misfire and the
destruction below) steps the first seed of `g_4e7408`.

### The shot (`0x1034de`–`0x103815`)

Only a taken shot gets here; a misfire still does everything except step 6.

1. Counters: barrel `+0x24` loses the extra shots and `+0x2c` holds them for
   `0x104150`, which multiplies its projectile count by them plus one. value06
   (`+0x6`; s_object_link.time, which `0x102100` compares with barrel
   definition `+0x1c` and `+0x1e`) rises by the extra shots plus one, the
   marker turn `+0x2e` by one, and weapon `+0x248` becomes the game time.
2. Animations: `0x105fa0` plays first-person state 1 or 2 (barrel 0 or 1), or 3
   or 4 on a misfire. For a weapon with a local user it also sets the unit's
   `+0x1fa` or `+0x1fb` to 0.3 s of ticks; otherwise `0x105c20` gives the
   weapon its own animation. Later `0x1058b0` puts the weapon in state 1 or 2,
   when its state allows and its animation is found.
3. value14 becomes 1.0 when barrel definition value_9c is positive and flag4 is
   clear, and barrel `+0x18` becomes 1.0 when `+0xa0` is positive; the update
   lowers both.
4. The holder: when the game options state (`g_4e6948 +0x8`) is 2, `0x1628f0`
   puts the player of the controlling unit (`0x101fb0`) in engine state 1 for
   half a second. A holder with a player gets `0xd0e60`, which takes definition
   `+0x27c` from its `+0x2b0` and limits its `+0x2b8` to `+0x280` (0.25 when
   0).
5. Heat: weapon `+0x180` rises by barrel definition `+0xa4`, to at most 1.0,
   and to at most definition `+0x154` when item_flag3 is clear. Unless the
   definition has flag26, `0x101db0` adds barrel definition value_a8 to heat,
   for a weapon with item_flag3 whose heat is below 1.0 and that
   `function_159dd0` does not exempt; at 1.0 it plays overheated_effect and
   switches the weapon to overheated_definition_index when that is set.
6. The marks, unless the shot misfired: barrel flag1 and weapon `+0x16c` bit 0,
   which `0xfd910` and `0xfd980` turn into the call to `0x104150` later in the
   tick. Then `0xbfc30` passes barrel definition `+0x36` and the holder to the
   AI (inferred: the shot's noise).
7. Damage: with a holder and a damage tag for the slot, `0xd03e0` damages the
   holder's gunner (`0xc7100`, the last unit of its `+0x24c` chain), or the
   holder, from its center along the reverse of its `+0x168` vector; in mode 4
   it goes only through `function_153d10`, for a player with a controller.
8. With reload_style 3, a shot of barrel 1 sets weapon `+0x16c` bit 3.
9. The kick: when the holder, as a biped or vehicle, has a player with a local
   user (`+0x28`), `0x187510` adds `+0xb0` + (`+0xb4` - `+0xb0`) × c to that
   user's pitch, where c is barrel `+0x20` through the curve barrel definition
   `+0xc0` names (`0x17ca10`). Then `0xc86e0` ends any holder's zoom, unless
   `function_100f70` is true for its current weapon, and plays the zoom-out
   sound for a user who was zoomed.

### On every call (`0x10381a`–`0x103992`)

1. Destruction: when weapon `+0x180` is above definition `+0x158` and a draw is
   below `+0x15c`, `0x104030` plays the weapon definition's tag at `+0x180` on
   the weapon and requests its deletion, outside mode 4. That sets the weapon's
   header flag `0x10`, after which `0x101ec0` gives its parent, so the firing
   effect below is placed on the holder.
2. The barrel's timer (`+0x0`), which `0x102100` tests, returns to 0.
3. The firing effect: with barrel definition flag12, `0x176970` creates it with
   scales A and B at the barrel's marker that the marker turn picks
   (`0x101b10`'s name, `function_b8d30`); with no marker of that name, the
   function returns without it. Otherwise `0x1039a0` plays the tag on the
   weapon and keeps its effect in barrel `+0x30`.

### Replays

`0x102fb0` never tests the game mode, and both callers pass only the weapon and
the barrel, so a replayed shot uses rounds, heat and counters on the receiving
machine as a local one does. The mode tests are in its callees: `0xa7cd0` acts
only in mode 5, `0x1628f0` and `0x104030` do nothing in mode 4, and `0xd03e0`
in mode 4 goes only through `function_153d10`. The draws use the machine's own
seed and the event carries none of their results, so a receiver can misfire
where the sender did not, and then create no projectiles (inferred). Before the
call, `0xa2ba0` writes the payload's aim back: the aiming unit's `+0x1c8` block
and, through `0xd0930`, its `+0x15c` and `+0x180` vectors, and the weapon's
tracked target (object_index_194, entry_177).

## Callees without source

Besides the library's `__chkstk`, `_floor` and `__ftol2` (`0x320560`,
`0x321099`, `0x322b0c`), two callees have neither source nor a stub.

- `0x104030` (80 bytes, lane S): the weapon in `esi`, plain `ret`. Outside mode
  4 it plays the tag at weapon definition `+0x180` on the weapon (`0x1039a0`,
  keeping no effect) and requests the weapon's deletion (`0xb8540`). Its other
  caller, `0x102f10` (84 bytes, no source), runs it from the update when a
  charged trigger's timer runs out and trigger definition `+0x24` is 1.
  Inferred: in both cases the weapon blows itself up.
- `0xbfc30` (91 bytes, in lane AB's range): a value in `eax`, an object in
  `ecx` and one stack argument (`ret 4`). For a biped or vehicle it calls
  `0x1ca130` (no source), which acts only while the AI globals are active and
  the value is positive. That function dispatches an AI event for the unit
  through `0x1fbac0` with the value, at most once per `g_510c54 +0x2` ticks
  unless the stack argument is greater than the last one it accepted (unit
  `+0x13a`, `+0x144`). A value of 2 also stores the game time in the object
  globals' `+0x14` (`g_4de2f4`), whatever the object. `0x102fb0` passes barrel
  definition `+0x36`, the holder and 1. Inferred: the AI notices the shot.

## Existing declarations

`0x102fb0` itself is declared nowhere. One callee's source disagrees with
retail in a way that matters here, at `5c3f7a9`:

| Retail | Declared as | Where | What retail shows |
| --- | --- | --- | --- |
| `0x105a80` | `void function_105a80(short state, long weapon_index, short magazine_index)` | `src/weapons.cpp` | Retail's jump table (`0x105b80`) has seven entries: states 1 to 4 take the first-person path, 0 and 5 only store the state, and 6 sets the ticks from magazine definition `+0x18` (`0x105aea`). The source handles 1 and 2 as first-person, 3 as a plain store and 4 as the timed state, and has no case 5 or 6, which reach `__assume(0)`. Its own callers pass 5 and 6 (`src/weapons.cpp:1454`, 1945, 1963, 2231), as does `0x102fb0` |

`s_weapon_firing_effect` (`src/weapons.cpp`) declares 0x1c bytes, with
effect_tag_index at `+0x18`. Retail steps through firing effects by 0x34 bytes
(`0x103318`, `0x1033b1`), and `+0x18` is the refused shot's tag, which
`function_105840` also plays with the scales 1.0 and 0.0
(`src/weapons.cpp:1345`).

## Evidence

- The function and its callees were disassembled from the retail XBE with
  capstone 5.0.9, through `tools/xbe.py`. Conventions, stack slots, offsets,
  flags and constants were read from retail code and data, and the callees with
  source were read from it. A second reader then checked each claim with an
  independent stack-tracking disassembler, matching all 29 call sites (27
  callees); its corrections are applied here.
- Callers come from an image-wide scan for calls, jumps and absolute pointers:
  `0xff1bd` and `0xa2de1` are the only references to `0x102fb0`, which is the
  only one to `0xbfc30`; `0x104030` has one more, at `0x102f5d`.
- Constants are read from retail `.rdata`: `0x45dbc0` (1.0), `0x45dc50`
  (1/65535) and `0x45dbb8` (2^32, for unsigned conversions).
- Field names are those of `src/weapons.cpp`, `src/unknown_101e80.cpp` and the
  object documents, and source line numbers are at `5c3f7a9`.
- No emulator, runtime testing, SDK or outside dataset was used. Names are the
  repository's own, or describe behaviour.
