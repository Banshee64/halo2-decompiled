# Weapon object type analysis (unknown_0fd910)

Retail range covered: `0xfd910`–`0x100126`. These are 10 inventory entries,
10,220 retail bytes: the callbacks of the `weapon` object type and the
helpers only they use, just below lane S's range. The largest is the
weapon's per-tick update, 5,684 bytes. **Analysis only:** this document adds
no source, and nothing in it has been built or checked against retail with
the original compiler. Names are provisional. The `function_<va>` form is
primary; the descriptions are offered for whoever decompiles the range.

It uses the names that `src/weapons.cpp` gives the weapon, its barrels,
triggers and magazines and their definitions, and the object core
documents' names ([part 1](unknown_0b7740.md), [part 2](unknown_0bc190.md))
for the object header and object fields. Offsets without a name in the
repository are given as offsets.

## Boundary

- `0xfd8b0`, just before the range, is matched in `src/projectiles.cpp`.
  `0x100130`, just after it, is `todo` with source in `src/weapons.cpp`.
  Both are excluded.
- No row of the Active claims table (issue #9) covers the range, so it is
  open. Lane S's row starts at `0x100000`, which falls inside `0xfff40`
  (`0xfff40`–`0x100126`); by its start address `0xfff40` is outside the
  claim. This document makes no claim. No open pull request touches the
  range.
- Every entry is `todo` with no `@retail` marker at `e02c566`. Two have
  `@stub` definitions; see [Existing declarations](#existing-declarations).
- Four entries have no direct callers. They are slots of the weapon type
  definition; see [The type definition](#the-type-definition).

## Conventions

"Callers" counts direct calls from outside the range, then calls from inside
it. Calls through the type definition are not counted. Results are in `al`
when the table names one. "Weapon" means the weapon's object index.

| Retail | Bytes | Arguments | Returns | Callers | What it does |
| --- | --- | --- | --- | --- | --- |
| `0xfd910` | 104 | none | | 2 | Runs `0xfd980` on every awake weapon with `+0x16c` bit 0 set |
| `0xfd980` | 127 | stack: weapon | | 1 + 1 | Runs `0x104150` for each barrel whose `flag1` is set, then clears the marks |
| `0xfda00` | 202 | stack: weapon, scenario datum | | 0 | Type slot 8: sets up a weapon placed from the scenario |
| `0xfdad0` | 411 | stack: weapon, placement, flag pointer (both unused) | bool, always true | 0 | Type slot 7: resets a new weapon |
| `0xfdc70` | 224 | stack: weapon, magazine index | bool | 1 + 1 | Asks the holding unit to reload a magazine |
| `0xfdd50` | 608 | stack: weapon, magazine index | bool | 0 + 1 | One magazine's tick: recharge, countdowns and its reload states |
| `0xfdfb0` | 5684 | stack: weapon | bool | 0 | Type slot 12: the per-tick update |
| `0xff5f0` | 1642 | stack: weapon, name, `real *value`, `bool *active` | bool | 1 | Type slot 15: the weapon's function values |
| `0xffc60` | 731 | stack: weapon, source object, unit, a value tested against NONE, `short *rounds` | bool | 1 | Moves rounds from a touched object into the weapon's reserves |
| `0xfff40` | 487 | `edi` weapon; stack: silent, immediate | | 1 | Readies the weapon when a unit takes it out |

## Data

### The type definition

`0x467cd0` is the `weapon` object type definition, entry 2 of `g_468630`.
In the layout of `s_object_type_definition_view` (`src/projectiles.cpp`) it
holds the name `0x452780` ("weapon"), the group tag `weap`, the datum size
`0x25c` and, at `+0xe`, a scenario datum size of `0x54`. Its callback
table at `+0x10` has four slots filled, and the repository's comments on
other types' callbacks say what each slot is for:

| Slot | Offset | Weapon | What the slot does | Dispatcher |
| --- | --- | --- | --- | --- |
| 7 | `+0x2c` | `0xfdad0` | resets a new object | `function_108a90`, at creation (`0xb7b40`, `0xb8166`); stops at a false result |
| 8 | `+0x30` | `0xfda00` | places it from the scenario | `function_108b10`, from `0xbf0f0` (`0xbf186`) |
| 12 | `+0x40` | `0xfdfb0` | the per-tick update | `function_108c60`, from `0xbc470`; the results are ORed |
| 15 | `+0x4c` | `0xff5f0` | the exported function values | `function_108d90`, until a handler returns true |

Each dispatcher walks the handler list at `+0x84`: `0x4678e8` (`object`),
`0x467c08` (`item`) and `0x467cd0` (`weapon`). Of these four slots, `object`
fills only slot 15 (`0xbcc20`), and `item` fills slot 7 (`function_10b2c0`,
always true) and slot 12 (`0x10b4d0`). So the weapon's handlers run after
the item's on every weapon. For function values, `function_108d90` stops at
the first handler that returns true, so a name the object handler answers
never reaches `0xff5f0`.

### Fields

`src/weapons.cpp` names the weapon (`s_weapon`), its barrels
(`s_weapon_barrel`, 0x34 bytes from `+0x1a4`), triggers
(`s_weapon_trigger`, 0xc bytes from `+0x20c`) and magazines
(`s_weapon_magazine`, 0x10 bytes from `+0x224`), and their definitions
(`s_weapon_definition`; barrel definitions are 0xec bytes from `+0x2d4`,
trigger definitions 0x40 bytes from `+0x2cc`, magazine definitions 0x5c
bytes from `+0x2c4`). Fields the range uses that have no name there:

| Field | Use |
| --- | --- |
| weapon `+0x16c` | 16-bit flags. Bit 0: a barrel shot is waiting for `0x104150`. Bit 1: overheated. Bit 2: first-person state 15 requested. Bit 3: cleared at overheat for reload style 3. Bit 4: the effect for a `+0x190` change has played. Bit 5: the `+0x19e` countdown has finished |
| weapon `+0x175` | a byte that wraps, advanced from barrel `+0x10`; a function value |
| weapon `+0x17e` | a short, NONE when new; the game engine's slot for this weapon (`function_242140`) |
| weapon `+0x180` | a float that each shot raises and that cools every tick: overheat starts at definition `+0x154` and ends below `+0x150` |
| weapon `+0x188` | a float, zeroed every tick; cooling waits while it is non-zero |
| weapon `+0x18c`, `+0x190` | floats: `+0x190` is a target of 0 or 1 that value_16e bit 0 toggles, and `+0x18c` moves toward it |
| weapon `+0x19e` | a short countdown, set when the weapon is readied |
| weapon `+0x244` | the index of the overheat effect (definition `+0x170`), or NONE |
| weapon `+0x248` | a game time that the `0x0e000572` value compares with |
| barrel `+0x10`, `+0x18`, `+0x1c`, `+0x20` | floats between 0 and 1 that rise while the barrel fires and fall otherwise |
| barrel `+0x30` | NONE when new |
| magazine `+0xa` | the fraction left over from recharging |
| trigger definition `+0x4` | which of three inputs the trigger reads |
| trigger definition `+0x8` | a barrel index, besides `primary_barrel` (`+0xa`) |

In the update, value_16e (`+0x16e`) carries the inputs that the holding unit
writes: bits 1, 2 and 8 are input A for input 0, 1 and 2; bits 3 and 4 are
input B for inputs 0 and 1. Bit 0 toggles `+0x190`, bit 5 blocks the
triggers, bit 6 picks `primary_barrel` in behaviour 4, and bit 7 plays
barrel 0's first firing effect.

## The functions

### `0xfdad0`: a new weapon

Slot 7. It reads only its first argument and returns true (`0xfdc64`), so
creation always goes on to the next handler.

1. It sets state (`+0x178`) to 0, object_index_194 (`+0x194`) and `+0x244`
   to NONE, and the floats `+0x190` and `+0x18c` to 0.0 (`0xfdb15`).
2. It calls `0x1058b0(weapon, 0, true)` (`0xfdb3b`): state 0 with force,
   which wakes the weapon and plays the state-0 animation when it has an
   animation state. Inside, the state is stored only when the animation
   can be set, so the store in step 1 is what holds otherwise.
3. It sets object_index_194 to NONE again, entry_177 (`+0x177`) to `0xff`,
   time_24c (`+0x24c`) to 0, and the bytes `+0x173`, `+0x174`, `+0x172` and
   `+0x171` to 0, in that order (`0xfdb40`).
4. For each magazine below magazine_count, rounds_loaded becomes the
   smaller of the magazine definition's `+0x6` (the starting rounds) and
   rounds_loaded_maximum, and rounds_unloaded the smaller of what is left
   of `+0x6` and rounds_total_maximum (`0xfdb6c`–`0xfdbcd`).
5. Each trigger's effect_index becomes NONE (`0xfdbe4`). Each barrel's timer
   becomes `0x7f` and its `+0x30` NONE (`0xfdc1b`, `0xfdc1e`).
6. `+0x17e` becomes NONE (`0xfdc37`). value250 and both halves of
   animation_254 become NONE and value258 0.0, the same four stores as the
   matched `function_105be0`, which may be inlined here
   (`0xfdc2e`–`0xfdc5c`).

### `0xfda00`: a weapon from the scenario

Slot 8, run after creation for a weapon placed from the scenario. The
scenario datum is 0x54 bytes; its `+0x4c` (rounds total), `+0x4e` (rounds
loaded) and `+0x50` (flags) are not named in the repository.

1. When the definition has a magazine, it reads the datum's two counts and
   halves both (a signed divide that rounds toward zero) when
   `g_4e6948`'s state (`+0x8`) is 1 and `g_4f55dc[4]` is set
   (`0xfda46`–`0xfda6f`). `src/unknown_1df3e0.cpp` describes `g_4f55dc` as
   whether each of the script-driven one-time hud messages has been shown.
2. `0x101980` gets the weapon in `eax`, rounds loaded in `cx` and (0, rounds
   total) on the stack (`0xfda76`). It sets magazine 0's rounds_loaded to
   the count, at least 0 and at most the magazine definition's
   rounds_loaded_maximum, and rounds_unloaded to what is left of the
   total, at least 0.
3. `function_b9b90(weapon)` with `dl` = datum `+0x50` bit 0 (`0xfda82`): set,
   it disables the weapon's physics; clear, it enables them and wakes the
   weapon.
4. It sets object flags (`+0x4`) bit 15 (`0xfda87`), which `src/equipment.cpp`
   calls cannot_be_garbage, and sets item_flags bit 7 to the inverse of
   datum `+0x50` bit 2 (`0xfda8e`–`0xfdaa2`).
5. When datum `+0x50` bit 0 is clear, it raises the weapon by 0.05 (`0x45dc1c`)
   on z (`+0x6c`) (`0xfdaa9`–`0xfdabe`).

The equipment type's placement handler, `function_f8090`, has the same flag
handling and the same 0.05.

### `0xfdfb0`: the per-tick update

Slot 12, reached from `0xbc470` (update an awake object) through
`function_108c60`. It returns the byte at `[esp+0x13]` (`0xff56b`), set
whenever something changed or is still running this tick; while it is
true the weapon stays awake. Below, `rate` is `g_510c54`'s rate (`+0x4`),
the seconds per tick. The function ends with five jump tables
(`0xff57c`–`0xff5e3`), counted in its size.

**Setup** (`0xfdfb0`–`0xfe087`).

1. It finds the weapon and its definition, as `WEAPON_GET` and
   `WEAPON_DEFINITION` do.
2. `0x101690(weapon)` is true when the parent is a biped or vehicle holding
   two weapons and this weapon is one of them (`0xfdffc`).
3. A blend factor f (`[esp+0x38]`), used only by the barrel floats below:
   `0xd1490(eax = unit, esi = weapon)`, where the unit is unit_index when
   in_inventory is set (`0xfe032`). It is 1.0 when either is NONE, and
   otherwise the unit's byte `+0x1fb` (`+0x1fa` when this is not the current
   weapon) times rate times 10/3, kept between 0 and 1. When the
   definition's flag27 is set, f is instead 1.0 or 0.0 from step 2.
4. The controlling player: `0x101fb0(eax = weapon)` gives the parent's
   `+0x24c` unit or the parent (the grandparent's, when the parent's
   definition has `+0xbc` bit 26), and that unit's player_index (`+0x13c`)
   is kept at `[esp+0x30]`, or NONE.

**Animation and timers** (`0xfe08b`–`0xfe53a`).

5. When object_index_194 is set and `0xbadc0` no longer finds that object,
   it becomes NONE (`0xfe09f`).
6. `0x105e80(weapon)` plays one tick of the weapon's own animation
   (value250, animation_254, value258) and returns true while it plays; its
   result seeds the return value (`0xfe0c5`). When the animation ends, or
   cannot be set up, `function_105be0` clears it.
7. When animation_state_offset is not NONE, the weapon's animation state
   advances with `0x1cb710`, passing frame events to `0xbf600`. A type-0
   frame event calls `0x105740(eax = weapon)` (`0xfe115`); otherwise, at the
   last frame, `0x105800` returns the weapon to state 0 unless its state is
   7, 8 or 10 (`0xfe13d`).
8. While `+0x16c` bit 5 is clear, `+0x19e` counts down; at 0 the bit is set
   (`0xfe167`–`0xfe18f`).
9. When the definition has flag10 and the weapon lies in the map with no
   parent, `0x10d4e0(edx = weapon)` runs the item's periodic effect
   (`0xfe193`–`0xfe1be`).
10. value_16e bit 7 plays barrel 0's first firing effect (`0x105840`). Bit 0
    clears `+0x16c` bit 4 and toggles `+0x190` between 0.0 and 1.0.
11. While `+0x18c` differs from `+0x190`, it moves toward it by definition
    `+0x2d8` times rate when rising or `+0x2dc` times rate when falling.
    The first tick of a change plays definition `+0x248` (rising) or `+0x250`
    (falling) through `function_1039a0` and sets `+0x16c` bit 4
    (`0xfe210`–`0xfe2f2`).

**Heat** (`0xfe2f6`–`0xfe50d`), only while `+0x180` is above 0.

12. At or above definition `+0x154`, with `+0x16c` bit 1 clear: it sets bit
    1, zooms the holding unit out (`0xc86e0`), and either clears `+0x16c`
    bit 3 (reload_style 3 with bit 3 set) or plays first-person state 14
    (`0x105fa0`).
13. While `+0x188` is 0.0, `+0x180` cools by definition `+0x168` (when
    overheated and positive) or `+0x160`, times rate, times
    (1 - heat × definition `+0x254`) when `+0x254` is positive, and stops at
    0. When the overheat will end within the length of first-person
    animation `0x8000071`, it sets `+0x16c` bit 2 and plays state 15
    (`0xfe418`–`0xfe470`).
14. Below definition `+0x150` the overheat ends: bits 1 and 2 are cleared
    (`0xfe496`).
15. The overheat effect (definition `+0x170`) is kept at `+0x244` while bit 1
    is set and item_flag1 is clear: `0x103a60` creates it, attached to the
    weapon or its parent; `0x177610` or `0x177260` stops it (`0xfe4a7`).
16. `+0x188` becomes 0.0, and state_ticks counts down (`0xfe517`).

**Inputs** (`0xfe53e`–`0xfe5c4`). It builds input A and B for each of the
three inputs from value_16e (see [Fields](#fields)). value_16e bit 5, a
non-zero state_ticks or the overheat blocks the triggers: both inputs read
as clear and the byte `+0x174` is zeroed.

**Magazines** (`0xfe5c8`–`0xfe5fb`). `0xfdd50(weapon, index)` runs for each
magazine, and its results are ORed into the return value.

**Triggers** (`0xfe5ff`–`0xfefe8`). For each trigger, the timer counts down
while positive. A and B come from the input that trigger definition `+0x4`
selects; both are clear when the triggers are blocked, and A is forced on
while trigger flag7 is set. The trigger's state (`+0x0`) picks a case
through the table at `0xff57c`. Below, "request barrel X" means: X is not
NONE, `0x102540(weapon, X, 0)` is true, and barrels[X].flag0 is set; the
barrel loop then fires it.

- State 0 switches on the trigger definition's behavior through `0xff59c`:
  - 0: with A or B, request barrel `+0x8` and set trigger flag5, else clear
    flag5; then, unless A is set and B is not, `0x102f70(eax = weapon,
    cx = +0x8)`.
  - 1: with A and trigger flag0 set, request barrel `+0x8` and clear flag0.
    Without A, set flag0 and call `0x102f70` for `+0x8`.
  - 4: as 1, but the barrel is primary_barrel when value_16e bit 6 is set.
    Without A it sets flag0 and calls `0x102f70` for `+0x8` and
    primary_barrel.
  - 5: with A, flag0 set and `0x102540(weapon, 0, 0)` true: when a player
    controls the weapon, `0x106280` decides between state 5 (`0x103c40`)
    and requesting barrel `+0x8`; with no player it requests
    primary_barrel. Without A, as 4.
  - 2: when blocked, it clears the bytes `+0x171`, value_170 and `+0x173`.
    When value_170 or `+0x171` is set (an analog pull), `0x1027b0` reports
    three events against trigger definition value_10 and value_14, and
    each event sets trigger flag3 or flag4 and acts through the tables at
    `0xff5b4` (definition `+0x1a`) and `0xff5c4` (`+0x18`): request barrel
    `+0x8` or primary_barrel, start charging (`0x103a90`, then `0x103ce0`),
    state 3 (`0x103b10`) or state 6 (`0x103bd0`). The third event clears
    flags 3 and 4. Event 1 is dropped while flag3 or flag4 is set; event 2
    while flag4 is set, or flag3 with trigger definition bit 0. Otherwise,
    with B, definition `+0x18` equal to 1 and `0x103a90` true, it starts
    charging and sets flag6; without B it clears flags 3 and 4 and calls
    `0x102f70` for `+0x8` and primary_barrel.
  - 3: with A or B and `0x103a90` true, it starts charging (`0x103ce0`) and
    sets flag6 when B is the input; otherwise `0x102f70` for `+0x8`.
- State 1, charging: nothing happens while an input is held and the timer
  runs. A release before the timer ends fires barrel `+0x8` (only its NONE
  test, no `0x102540`) unless flag6 is set, which it clears instead; then
  the trigger returns to state 0, timer 0, and its effect_index is stopped
  (`0x177260`) and set to NONE. When the timer reaches 0, `0x102d60` moves
  it to state 2.
- State 2, charged: without flag6, releasing A (unblocked) calls
  `0x102e10`, and a zero timer calls `0x102f10`. With flag6, releasing B
  resets it as in state 1 and clears flag6.
- States 3 and 7: while A is held or the triggers are blocked, `0x106280`
  can move it to state 5 (`0x103c40`). On release, state 7 returns to 0;
  state 3 first requests barrel `+0x8`, clearing flag0.
- State 5: on release, as state 3. While held, `0x106280` returns an object
  and an entry: none moves it to state 7 (`0x103b70`); the same object as
  object_index_194 updates entry_177 and, with the timer at 0, moves it to
  state 6 (`0x103bd0`); a different one calls `0x103c40` and stores both.
- State 6: on release it requests primary_barrel without clearing flag0,
  then returns to 0. While held, when `0x1061c0` finds object_index_194
  for the holding unit, `0x103bd0` restarts the timer; a zero timer with a
  controlling player moves it to state 7.
- State 4 does nothing. A trigger counts as activity when its state is not
  0 or an input is set.

**Barrels** (`0xfefee`–`0xff54c`). For each barrel, with its definition:

- value14 falls by barrel definition `+0xd0` × rate and `+0x18` by
  `+0xcc` × rate, each only while positive and not below 0; ticks counts
  down. The state (`+0x1`) picks a case through `0xff5d4`.
- State 0, idle: when blocked, flag0 is cleared. With flag0 set the barrel
  goes to state 1, unless its definition has flag9 or flag10, there are two
  barrels, and the other barrel is in state 1 with flag9 or state 3 with
  flag10. With flag0 clear, `0x1023d0(eax = barrel, weapon)` checks the
  magazine: when it is low and reserve rounds can fill it, it asks for a
  reload through `0xfdc70` (unless value_16e bit 5 is set or the weapon has
  no parent) and returns true; with no reserve, or a full magazine, it
  resets the triggers and stops the firing barrels. A barrel
  that has just gone to state 1 runs state 1 at once.
- State 1, firing: when blocked, `0x103e60` stops it (state 2). Otherwise
  `0x102100(eax = barrel; weapon, &a, &b)` decides whether a shot is due;
  it also accumulates into barrel `+0x24`. A due shot clears flag0 and
  calls `0x102fb0(weapon, barrel)`, unless a is set and flag0 is clear,
  which stops the barrel. When no shot is due, b, or a with flag0 clear,
  stops it; otherwise timer rises by 1, at most `0x7f`. A shot or a timer
  step counts as busy for this tick (`[esp+0x15]`).
- States 2 and 3 wait for ticks to reach 0, then call `0x103f60` (state 2)
  or `0x103dd0` (state 3).
- While busy, `+0x10` rises by barrel definition `+0xd4` × rate to at most
  1, `+0x20` by `+0xc4` × rate and `+0x1c` by ((1 - f) × `+0xdc` +
  f × `+0x50`) × rate, both to at most 1. When not busy they fall by
  `+0xd8`, `+0xc8` and ((1 - f) × `+0xe0` + f × `+0x54`) times rate, to at
  least 0 (`+0x20` and `+0x1c` only while `+0x1c` is positive).
- When `+0x10` passes barrel definition `+0x18` (if non-zero), `0xba7f0`
  sets region state bit 0 on the object's regions and barrel flag3 is set;
  when it falls back below, `0xba7f0` clears the bit and flag3. The object
  is the weapon, or its parent by `function_101ec0`'s rule, inlined
  (`0xff2a1`–`0xff2e9`). `0xba7f0` does nothing when the object has no
  model.
- While `+0x10` is positive, the byte `+0x175` becomes the low byte of
  ((`+0x10` / 7 × (barrel definition `+0x14`, or 1)) + `+0x175` / 255) × 255,
  so it wraps (`0xff3ab`–`0xff413`).

`0x102fb0` marks the shot for `0x104150`: it sets barrel flag1 and weapon
`+0x16c` bit 0 (`0x103708`, `0x103710`), and `0xfd980` fires it later in the
tick. It sets neither for a shot it refuses, or when a random draw from
`g_4e7408` falls below (heat - definition `+0x25c`) × `+0x260` / (1 -
`+0x25c`). It tests that only for a barrel with firing effects (barrel
definition `+0xe4` positive), when `+0x25c` is between 0 and 1 and heat is
above it (`0x1033be`–`0x10343b`); this reads as a heat-driven misfire.

### `0xfdd50` and `0xfdc70`: a magazine's tick and the reload request

`0xfdd50(weapon, m)` runs once per magazine from the update (`0xfe5e6`) and
returns whether the magazine was active. Its states go through the table
at `0xfdf98`, part of its size.

1. Recharge: when the magazine definition's `+0x4` is positive and
   rounds_loaded is below rounds_loaded_maximum, rounds_loaded grows by
   `+0x4` divided by `g_510c54`'s field_2_3 (used as ticks per second),
   with the remainder kept in magazine `+0xa` until it makes a whole round;
   the result is capped at the maximum, and `function_a7cd0(weapon)` reports it
   (`0xfdda5`–`0xfde0b`).
2. ticks and ticks_0c count down.
3. States 1, 2 and 3 (a reload): ticks_0c is kept at most ticks, and at 0
   `0x102b90` finishes the reload (moves rounds from the reserve). When
   ticks reaches 0, the magazine goes to state 0 (from 1) or 4 (from 2 and
   3) through `0x105a80`. Then, with reserve rounds (`0x1008f0`), room in
   the magazine and magazine definition flags bit 0 clear, and unless
   value_16e bit 1 or 2 asks for barrel 0 or 1 and `0x102540(weapon, i, 1)`
   allows it, it asks for a reload with `0xfdc70` and stops there when that
   is accepted. Otherwise `0x102c60` ends the reload, as interrupted when
   such a barrel was allowed, and sets state 5.
4. State 5 calls `0x102cc0(edi = weapon, m)`: in magazine state 0 or 5,
   with no trigger active and the weapon in state 0, it plays the magazine
   definition's effect_48, puts the weapon in state m + 3 and the magazine
   in state 6.
5. State 6 returns to state 0 when ticks reaches 0.

`0xfdc70(weapon, m)` sends the holding unit a request, through
`function_e68c0`, when the weapon is in that unit's inventory and is its
current weapon (type m) or its other weapon (type 10 + m). The unit's
request table maps types 0, 1, 10 and 11 to `0xe7280`, whose calls reach
`function_101490`, the reload of a magazine. It returns whether the
request was accepted, or false. Its callers are `0xfdd50` (`0xfdf38`) and
`0x1023d0` (`0x1024b0`), which ignores the result.

### `0xff5f0`: the function values

Slot 15, and also called directly by the unit's export function `0xc6b90`
(`0xc6dc8`) for the unit's current weapon, which maps two of the unit's
names onto the weapon's (`0x0b0005a9` to itself, `0x130005a8` to
`0x0c000569`). `function_108d90` calls the object handler (`0xbcc20`) first
and stops at a true result; the item has no slot 15.

It is a binary search on 27 string ids (`0xff630`–`0xffb47`). A known name
sets `*value`, sets `*active` to whether the value is above 0, and returns
true; any other name returns false and writes nothing (`0xff900`). Values
read from fields or helpers are kept between 0 and 1 (`0xffc16`).

| Name | Value |
| --- | --- |
| `0x03000577` | heat |
| `0x04000567` | `+0x180` |
| `0x05000024` | 1 when `function_10cf50` (in_inventory set, item_flag1 clear), else 0 |
| `0x05000081` | `+0x18c` |
| `0x09000568` | 0 when `function_10cf50` is true and `+0x16c` bit 5 is clear; else 1 when heat is below 1, else 0 |
| `0x0a000066` | when overheated: (`+0x180` - definition `+0x150`) / (1 - `+0x150`), or 0 when `+0x150` is exactly 1; else 0 |
| `0x0b0005a9` | `+0x175` / 255 |
| `0x0c000569` | the largest of: each barrel's `+0x18`; for each trigger with value_1c above 0, `function_102070` × trigger definition `+0x28`, and for a trigger in state 2, `+0x28` + (1 - `+0x28`) × `+0x188`; and `+0x180` × definition `+0x164` |
| `0x1d0007ba` | 0 when the object globals' byte `+0x82` is set, else as `0x0c000569` |
| `0x0d0006e7` | 1 when `function_162b70` gives exactly 1, else 0 |
| `0x0e0006e6` | 1 when `function_162b70` is between 0 and 1, else 0 |
| `0x0f0006e5` | 1 when `function_162b70` gives exactly 0, else 0 |
| `0x120006e4` | `function_162b70` |
| `0x0e000572`, `0x10000573` | barrel 0 or 1: its `+0x10` when the game time is at most one tick past `+0x248`, else 0 |
| `0x0f000574`, `0x11000575` | trigger 0 or 1: `function_102070`, its charge |
| `0x1200056a`, `0x1400056d` | magazine 0 or 1: rounds_loaded / rounds_loaded_maximum |
| `0x14000570`, `0x16000571` | barrel 0 or 1: its `+0x10` |
| `0x1500056e`, `0x1700056f` | barrel 0 or 1: its value14 |
| `0x14000578`, `0x16000579` | 1 when value_16e bit 1 (or bit 2) is set, else 0 |
| `0x1700056b` | magazine 0's rounds_loaded: its ones digit × 0.1 |
| `0x1700056c` | magazine 0's rounds_loaded: its tens digit × 0.1 |

The four names that use `function_162b70` (the game engine's progress for
the weapon's slot `+0x17e`, in engine mode 9) give 0 unless `g_4e6948`'s
state is 2, `+0x17e` is not NONE and `function_162b10` is true. The indexed
names give 0 for a barrel, trigger or magazine the definition does not
have, and a magazine with a zero maximum gives 0.

### `0xfd910` and `0xfd980`: the shots marked during the tick

`0xfd910` is not a type callback. Its callers, `0xb7060` (`0xb70f1`) and
`0xb7150` (`0xb719b`), are object update passes, both `todo` with no source:
`0xb7150` calls it after its update loop and before it deletes the objects
that requested deletion, and `0xb7060` after both of its loops. It walks the
awake weapons with the object iterator (type mask 4, header flag 2, the
stores `function_bae80` would make, then `function_baeb0`) and calls
`0xfd980` for each one whose `+0x16c` bit 0 is set (`0xfd95e`).

`0xfd980(weapon)` calls `0x104150(weapon, i)` for each barrel whose flag1
is set, clears flag1, and finally clears `+0x16c` bit 0. `0x102fb0` sets
both marks when a barrel shoots, from the update and from `0xa2ba0`, the
weapon-fire event's handling (slot 11 of the vtable at `0x452504`), which
also calls `0xfd980` directly (`0xa2df5`). `0x104150` (5,212 bytes, in lane
S's range) reads the barrel definition's projectile_definition_index and
creates objects with `0xb7930` and `0xb7b40`: it creates the shot's
projectiles. So the shots of a tick are fired together, after the objects
have updated.

### `0xffc60`: rounds from a touched object

Its only caller is `0x150e30` (`0x150f73`), which tries each of its unit's
four weapons from the current one and stops at the first true result.
`0xffc60(A, B, unit, value, &rounds)` moves rounds from object B into the
reserves (rounds_unloaded) of weapon A's magazines, up to each magazine
definition's rounds_total_maximum, and adds them to `*rounds`.

- When B has A's definition, the rounds come from B's own magazines,
  rounds_unloaded first.
- Otherwise each of A's magazine definitions has a block at `+0x54`
  (count) and `+0x58` (12-byte entries): the entry whose `+0x8` is B's
  definition gives the rounds (`+0x0`).
- When a magazine matched (the result follows the last magazine with room
  on the same-definition path), it wakes A and B (`function_b7360`). In
  game options mode 4 it then returns false. Otherwise it deletes B
  (`0xb8540`) unless `function_101640` keeps it (B a weapon) or a
  same-definition transfer left rounds in the magazine it came from. It
  plays A's definition `+0x268` (B a weapon) or B's pickup_sound_index
  when the fourth argument is not NONE, reports the pickup to `0xa88a0`,
  and marks A's and B's simulation entities (`function_a7cd0`,
  `function_b58c0` with `0x2000`).

### `0xfff40`: readying the weapon

Its only caller is `0xe8720` (`0xe88bc`), which takes out the weapon in a
unit's slot for a hand; the weapon index is in `edi`.

1. `function_104080(weapon)` resets its triggers and magazines and clears
   its animation; `function_b7360` wakes it.
2. With an animation state, it plays animation set `0x5000024` with weapon
   class `0x400054b` when the holder dual-wields (retrying with
   `0x7000001`), else `0x7000001`; on success the weapon's state becomes 9.
3. Unless silent, it plays definition `+0x144` (`function_1039a0`).
4. When immediate, state_ticks becomes 0. Otherwise `+0x16c` bit 5 is
   cleared, `+0x19e` becomes field_2_3 × definition `+0x138`, rounded, and
   state_ticks the first-person length of `0x5000024`.

It also stores whether the weapon is the unit's current weapon in a local
that nothing reads (`0x100091`).

## Existing declarations

At `e02c566`:

| Retail | Declared as | Where | What retail shows |
| --- | --- | --- | --- |
| `0xff5f0` | `bool __stdcall function_ff5f0(long weapon_index, long name, real *value, bool *active)` | `src/stubs/unit_object_type.cpp`, `src/unit_object_type.cpp` | Agrees: four stack arguments, `ret 0x10`. `src/unit_object_type.cpp` calls it from the unit's export function |
| `0xfff40` | `void __stdcall function_fff40(long a, long b)` | `src/stubs/unknown_0a76b0.cpp`, `src/unknown_0a76b0.cpp` | Retail also takes the weapon index, in `edi` (read at `0xfff46`), besides the two stack arguments. The source of `0xe8720` calls `function_fff40((long)silent, (long)immediate)` without it, so the declaration needs the weapon before that call can match retail's `0xe88bc` |

The other eight entries are not declared anywhere.

## Evidence

- Every entry was disassembled from the retail XBE with capstone 5.0.9,
  through `tools/xbe.py`, at `e02c566`. Conventions, offsets, flags,
  callers and the tables were read from retail code and data. Each
  function's analysis was then checked claim by claim against the code by
  a second reader, and its corrections are applied here.
- References come from an image-wide scan for calls, jumps and absolute
  pointers to each entry. The four slots are the dwords at `0x467cfc`,
  `0x467d00`, `0x467d10` and `0x467d1c`; every other reference is a call
  named above.
- The jump tables are `0xfdf98` (in `0xfdd50`) and `0xff57c`, `0xff59c`,
  `0xff5b4`, `0xff5c4` and `0xff5d4` (in `0xfdfb0`).
- Constants are read from retail `.rdata`: `0x45dc1c` (0.05), `0x45dbc0`
  (1.0), `0x45dbd8` (0.0), `0x45dc68` (0.1), `0x45dc38` (255.0),
  `0x45dbb4` (1/255) and `0x43fde4` (1/7).
- Field names are those of `src/weapons.cpp` at `e02c566`, after lane S
  renamed some of them during this analysis; the object fields follow the
  object core documents.
- `0x104150` was read only for its callees and the definition fields it
  uses. The meanings of the 27 string ids in `0xff5f0` are not known, and
  none is guessed here.
- No emulator, runtime testing, SDK or outside dataset was used. Names are
  the repository's own, or describe behaviour.
