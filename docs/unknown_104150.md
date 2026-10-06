# A weapon barrel's shot (unknown_104150)

Retail range covered: `0x104150`–`0x1055ab`. This is one inventory entry, 5,212
retail bytes, `todo` with no source: the function that fires one barrel's shot,
creating its projectiles. **Analysis only:** this document adds no source, and
nothing in it has been built or checked against retail with the original
compiler. Names are provisional. It is written for lane S, whose claim covers
the function, and makes no claim of its own.

It continues the [weapon object type](unknown_0fd910.md) analysis and
[`0x102fb0`](unknown_102fb0.md), which marks, during the weapon's update, the
shots that this function fires. It uses the names of `src/weapons.cpp` and
`src/projectiles.cpp` and of the object documents
([lifecycle](unknown_0b67c0.md), [part 1](unknown_0b7740.md),
[part 2](unknown_0bc190.md)). Three callees with no source when this was
written are described only as far as this function needs them.

## Boundary

- `0x104080`, just before, and `0x1055b0`, just after, are `todo` with source
  in `src/weapons.cpp`. Both are excluded.
- Lane S's row of the Active claims table (issue #9, `0x100000`–`0x10ffff`)
  covers the function. No open pull request touches it.
- `0x104150` has no `@retail` or `@stub` marker, declaration or stub at
  `5c3f7a9`.

## Conventions

"Callers" counts direct callers.

| Retail | Bytes | Arguments | Returns | Callers | What it does |
| --- | --- | --- | --- | --- | --- |
| `0x104150` | 5212 | stack: weapon, barrel index (16 bits) (`ret 8`) | | 1 | Fires one barrel's shot: aims it, creates its projectiles, applies the barrel's damage and sends the shot |

Its caller, `0xfd980`, runs it for each barrel whose flag1 is set, after the
objects have updated (`0xfd9d7`); `0x102fb0` sets that mark when a barrel
shoots. Its result is unused.

## Data

The frame is `0x1e98` bytes, reserved through `__chkstk` (`0x320560`, the C
library's stack probe; `0x104155`). In it, after the four register pushes: 64
object markers of 0x70 bytes (`s_object_marker`) at `esp+0x2a8`, a placement
block ([part 1](unknown_0b7740.md)) at `esp+0x104`, and a damage data block
(`s_type_1e6529`, 0x88 bytes) at `esp+0x1d4`.

Fields this function uses that `src/weapons.cpp` leaves unnamed:

| Field | Use |
| --- | --- |
| barrel definition `+0x34` | a short read in game options modes 4 and 5: 1 sends nothing, 2 sends the shot, any other value also registers the projectiles and, in mode 4, creates none |
| barrel definition `+0x40`, `+0x44` | the deviation angles, in degrees |
| barrel definition `+0x58`–`+0x64`, `+0x70`–`+0x78` | two sets of error-cone and scale values, blended by f |
| barrel definition `+0x68`, `+0x6a`, `+0x6c` | spread mode, projectiles per shot, spread spacing |
| barrel definition `+0x7c` | three offsets for a player's shot |
| barrel definition `+0x98`, `+0x88` | the damage definition applied at the muzzle, and the 6-bit code stored with it |
| barrel definition flags 12 and 13 | fire one marker per shot in turn; keep a tracked target |
| weapon barrel `+0xe`, `+0x2c`, `+0x2e` | the round counter for tracers, extra shots this tick, the marker turn |
| weapon `+0x1a2` | extra shots for barrel 0 from barrel 1's definition |
| projectile `+0x1a4`, `+0x1a8` | the weapon's definition index, the player mark |

The barrel definition's flag0, flag3, flag5 to flag8 also matter here: aim at
the aim-assist object (0), fire every marker (3), let the weapon's value_16e
bit 6 skip the error cone (5), skip the unit (6), give every projectile the
first one's direction (7), and aim an actor from the markers' mean position
(8). The weapon barrel's flag5 marks a damaged barrel, one of whose markers is
deflected.

## The function

It has one exit and no jump table: a loop over the barrel's markers, and inside
it a loop over the projectiles of each marker's shot.

### Setup (`0x10415a`–`0x104450`)

1. It finds the weapon, the barrel (`+0x1a4` + barrel × `0x34`), the weapon
   definition and the barrel definition, and the holder: unit_index when
   in_inventory is set, else NONE.
2. owner = `0x101fb0(eax = weapon)`: the biped or vehicle parent's `+0x24c`
   unit or the parent itself, as in the weapon update. dual =
   `function_101690(weapon)`.
3. The blend factor f = `0xd1490(holder, weapon)`, or 1.0 or 0.0 from dual when
   the definition has flag27, as in the update. f weights the barrel
   definition's second value set against the first in what follows.
4. In game options mode 4 or 5, barrel definition `+0x34` sets whether the shot
   is sent and the projectiles registered (see [Data](#data)).
5. The markers: `0x101b10` gives the marker name (the barrel definition's name,
   or `0xf0000db` and `0x110000dc` for barrels 0 and 1), and `function_b8d30`
   fills up to 64 markers of the object `0x101ec0` gives. With none found it
   still fills one from the first node. Unless the barrel definition has flag3,
   only the first marker fires.
6. With flag8, the mean of the markers' positions is kept for an actor's aim.
7. With the weapon barrel's flag5, a marker is chosen from a seed that starts
   at the weapon index, so the same weapon always deflects the same marker by
   the same angles (`0x1043bd`–`0x1043e4`). Only `function_1003e0`, from the
   damage code, sets that flag.
8. When the projectile definition (`+0x90`) and the owner's player index are
   set, `0x15cca0` stores a player mark for the shot and returns it.

### Each marker (`0x104457`–`0x10494d`)

With barrel definition flag 12 and several markers, only the marker whose index
is the weapon barrel's `+0x2e` turn fires. The marker's position and forward
are the default origin and aim. Unless the barrel definition has flag6, or
there is no holder, or the holder's `+0x10a` bit 2 is set:

1. The aiming unit is the holder, followed along `+0x24c` (a vehicle's gunner).
   `0xc8290` gives the aim and the origin: normally the unit's aim vector
   (`+0x168`); the origin moved onto that line from the unit's eye point when
   its definition has `+0xbc` bit 3, offset by barrel definition `+0x7c` for a
   player, and clipped against the world. It also writes the root object's
   velocity. An actor in combat state 4, or a holder whose own `+0x24c` is set,
   keeps the marker's forward instead.
2. `0x109e00` reports the velocity of a moving object the unit stands on, and
   the inherited speed is the aim's component of the root velocity, at least 0.
3. With flag3 the origin is the marker's position again, clipped from the
   unit's eye point (`0xcafc0`, `0x16a7c0`).
4. A player's shot (the aiming unit has a player index): with flag0 and an
   aim-assist object in the unit's `+0x1c8` block whose weight (`+0x1c`) is
   positive, that object and the node at block `+0x4` become the target. For
   barrel 1 of a weapon whose first trigger has behaviour 2 and `+0x18` of 1,
   `0x106320` must also agree, or there is neither a target nor an aim change.
   Otherwise flag 13 with object_index_194 set targets it and entry_177.
   Otherwise `0x1a4c00` adjusts the aim (see below).
5. An actor's shot: `0x1fddd0` gives the aim, an AI speed, an error and a
   target; with flag 13 the target is kept only when it is a vehicle or a biped
   in a seat. A false result skips the marker entirely, its damage included.
6. On the deflected marker, the aim is tilted by an angle between barrel
   definition `+0x40` and `+0x44` degrees in a random direction around it
   (`0x10494d`–`0x104c0c`).

### The projectiles (`0x104c12`–`0x1053dc`)

The projectile definition is the barrel definition's
projectile_definition_index (`+0x90`) and the count n its `+0x6a`. For barrel 0
with the weapon's `+0x1a2` positive, the projectiles are barrel 1's (read
without checking that there is a barrel 1), n is multiplied by `+0x1a2`, and
`+0x1a2` is cleared. A non-zero weapon barrel `+0x2c` multiplies n by `+0x2c`
plus one and is cleared. `0x102fb0` stores that count there; it also subtracts
it from barrel `+0x24` (the float `0x102100` accumulates;
s_object_link.accumulator in `src/unknown_101e80.cpp`) and adds it, plus 1, to
value06 (`+0x6`). The first marker to reach this point clears both counters, so
later markers use the plain n. With no projectile definition, or n not
positive, the marker goes straight to its damage. For each projectile:

1. Placement: the owner, from `0x101fb0` once per marker, gives an owner block
   of its player index (`+0x13c`), itself and its team (`+0x138`), or NONE.
   `0xb7930` fills the defaults of [part 1](unknown_0b7740.md) from the
   definition, the owner and that block; then the position is the origin, the
   forward the aim, and flags bit 2 is set when the holder has a player index.
2. Tracers: weapon barrel `+0xe` counts projectiles. A projectile is marked,
   and the count reset, when barrel `+0x10` (a float that rises while the
   barrel fires) is 0.0 or the count has reached barrel definition `+0x2e`;
   otherwise the count rises by 1. So one projectile in every `+0x2e` plus one
   is marked, or every one when `+0x2e` is not positive. An unmarked projectile
   loses flag bit 1, which `function_f8200` sets on every new projectile
   (`src/projectiles.cpp:1262`) and `function_fbe70` exports as function value
   `0x6000564` (line 412).
3. Error: lo = `+0x74` × (1 - f) + `+0x5c` × f and hi = `+0x78` × (1 - f) +
   `+0x60` × f, where a zero `+0x5c` or `+0x60` falls back to the first value,
   and the error is lo + (hi - lo) × weapon barrel `+0x1c`. It is computed once
   per marker, unless the actor supplied one. Then `0x146320`
   (random_vector_in_cone) turns the forward by a random angle between `+0x70`
   × (1 - f) + `+0x58` × f and the error, unless flag5 and the weapon's
   value_16e bit 6 are both set.
4. With flag7, every projectile after the first takes the first one's
   direction.
5. The up vector is the forward crossed with the world axis of its smallest
   component, normalized. Then `0x1055b0` turns the forward about it by barrel
   definition `+0x68` (spread mode; only mode 1 turns it), `+0x6c` (spacing),
   the projectile's index and n.
6. Velocity: the forward times the inherited speed; plus, on a platform, the
   platform's velocity without its forward part; plus, when the AI speed s is
   positive and below the projectile definition's `+0x17c` (unknown17c), the
   forward times (s - unknown17c). That definition is read again from barrel
   definition `+0x90`, even when the projectiles are barrel 1's.
   `function_f8200` later adds the forward times unknown17c × the projectile's
   unknown16c (`+0x16c`, 1.0 unless the definition is difficulty_scaled;
   `src/projectiles.cpp:1272`, 1311). So with unknown16c at 1.0, an AI's
   projectile leaves at the inherited speed plus s.
7. Unless the shot creates nothing (mode 4 with `+0x34` not 1 or 2), `0xb7b40`
   creates the projectile and runs its new handler, `function_f8200`. A NONE
   result skips the rest. Then: `+0x188` (the damage_scale of
   `src/projectiles.cpp:2240`) = (1 - f) + f × barrel definition `+0x64`;
   `+0x1a8` = the player mark; flag bit 8 when s is positive, which stops the
   projectile's speed from moving from unknown17c toward unknown180 (line
   1950); the target at `+0x144`; flag bit 1 cleared on an unmarked round;
   `0xa7870` registers it when asked; `+0x1a4` = the weapon's definition index;
   and `function_fa100` moves it along its velocity at once when its definition
   has the drifts bit.
8. When the shot is sent and barrel definition `+0x34` is 2, a projectile that
   hit an object in that first move is kept for the message (the last one, when
   several did): the object, if it has a simulation entity (`+0xd4`), its node
   and the hit point in the node's space (`0x1052ca`–`0x1053c6`).
   `src/projectiles.cpp:2255`–2261 records that hit (flag bit 13) in modes 4
   and 5.

### Damage and the message (`0x1053e2`–`0x1055a9`)

After each marker's projectiles, outside mode 4, barrel definition `+0x98`
damage is applied at the origin along the aim. The damage data
(`s_type_1e6529`) takes the holder's damage owner (`0xd66d0`,
object_get_damage_owner), the holder, the weapon's root location (`0xba300`,
object_get_root_location), the origin, the aim as cone direction and the 6-bit
code from barrel definition `+0x88`; `0xd6c80` then damages every object in its
radius except the holder. After the last marker, a sent shot goes to `0xa7d50`
with the barrel index and the hit object, node and point.

The function writes neither heat (`+0x184`, `src/weapons.cpp`) nor the `+0x180`
float, and nothing in the unit, the triggers or the magazines. Its only writes
outside its frame are weapon barrel `+0xe` and `+0x2c`, weapon `+0x1a2` and the
new projectiles' fields. Its callees do more: `0xb7b40` creates the projectile
and, when its definition names one, an effect ([part 1](unknown_0b7740.md));
`0xd6c80` damages other objects; and `0x15cca0` and `0x1a4c00` write player
data.

## Callees without source

`0x320560` is the C library's `__chkstk`, which only reserves the frame. The
other two are each called only from here, and are described as far as this
function needs them. `0x1a4c00` has no source. `0xa7d50` gained source after
this was written (`function_a7d50` in `src/unknown_0a7c50.cpp`, `todo`, from
lane Z's rounds 6 and 7), and it agrees with the description below: the weapon,
barrel index, hit object, node and point, and event type `0xc` sent through
`function_b5a70`.

### `0xa7d50`: sending the shot (lane Z)

`0xa7d50` (747 bytes) takes the weapon in `eax` and, on the stack, the barrel
index, the hit object, its 16-bit node and the point (`ret 0x10`). It returns
at once unless the game options mode is 4 or 5, or when barrel definition
`+0x34` is 1. It fills a 0x44-byte payload laid out like s_z_fire_payload
(`src/unknown_09b910.cpp`): the weapon's slot in its unit (`0x101f50`), its
definition index, the barrel index, the hit object's node and point when there
is one, the aiming unit's aim (`+0x168`) and aim-assist values, and, with
barrel definition flag 13, the tracked target (object_index_194, entry_177). It
sends that through `0xb5a70` as event type `0xc`, pushed at `0xa802a`: the
weapon-fire event, whose vtable at `0x452504` returns `0xc` from slot 0 and
whose slot 11, `0xa2ba0`, marks and fires the barrel again with `0x102fb0` and
`0xfd980` on the machine that receives it. The event's two objects are the
shooter and the hit object.

The event's player is NONE in mode 4, where the event goes out only when the
shooter's player has a user index (`0xa76b0`). In mode 5 it is the shooter's
player index (`+0x13c`) for a biped or vehicle, or that object's `+0x24c`
unit's when its own is NONE; any other object gives NONE. `0xb5a70` acts only
when the simulation world mode (`g_4cf77c +0x8`) is 4 or 5. It turns the
objects into entity ids (`0xa5930` returns an entity's id when its flag 4 is
set, `0xa5980` when it is clear), and in mode 4 only, for a player with a
machine index, builds a mask of every machine but one (inferred: the sender's).

### `0x1a4c00`: a player's aim (lane M, on the second machine)

`0x1a4c00` (1,172 bytes) takes the aim in `eax` and the origin in `ebx`, and on
the stack the player index, the aiming unit's `+0x1c8` block and whether the
weapon is the holder's current one (`ret 0xc`). It returns `al`, which this
function ignores. In modes 4 and 5 with block `+0x18` bit 1 set, it aims
straight at the block's point (`+0xc`) and returns true. Otherwise it takes
aim-assist values from `0x1a50a0`, the larger of the primary and secondary
weapons' values (`src/unknown_1a58b0.cpp`), and leaves the aim alone if that
fails. It then casts the player's view ray through `0x1697c0`: the camera
vector from `0x1557c0`, times 128. The new aim points from the origin at what
the ray reaches, or at its end on a miss, blended toward the aim-assist point
that `0x1a6a80` finds from the block, by block `+0x1c` for the current weapon
or `+0x20` for the other. When `0x1a6d30` is true (the player's unit sits in a
seat of its parent, or there is no unit), the change is limited to an angle
from the aim-assist values (`0x11d3b0`). With a full weight and an unclamped
change, it records the target object and the game time at player `+0x174` and
`+0x178`.

## Existing declarations

`0x104150` itself is declared nowhere. Two callees have source whose parameters
this function's calls explain, at `5c3f7a9`:

| Retail | Declared as | Where | What retail shows |
| --- | --- | --- | --- |
| `0x1fddd0` | `bool function_1fddd0(long actor_index, point3f const *origin, vector3f *direction, long *out_index, real *arg_c9241c, long *out_object)` | `src/unknown_1fdbf0.cpp` | Six parameters, as in retail (`eax`, `edx`, `edi` and three on the stack). `out_index` is this function's AI speed, a real: it is compared with 0.0 and with the projectile definition's unknown17c, and scales the forward. The source copies actor `+0x7ac` into it as a long, which moves the same bytes. `arg_c9241c` receives the error angle (actor `+0x7bc`) and `out_object` the target object |
| `0x1055b0` | `void function_1055b0(vector3f *direction, vector3f const *axis, short index, short mode, real spacing, long flags)` | `src/weapons.cpp` | `flags` is the projectile count n (`0x105066`); the function tests its bit 0, so an odd and an even count are spaced differently |

## Evidence

- The function and its callees were disassembled from the retail XBE with
  capstone 5.0.9, through `tools/xbe.py`. Conventions, stack slots, offsets,
  flags and constants were read from retail code and data. A second reader then
  checked each claim with an independent stack-tracking disassembler, matching
  all 32 call sites (31 callees); its corrections are applied here.
- Callers come from an image-wide scan for calls, jumps and absolute pointers:
  `0xfd980` (`0xfd9d7`) is the only reference to `0x104150`, which is in turn
  the only one to `0xa7d50` and to `0x1a4c00`.
- Constants are read from retail `.rdata`: `0x45dc54` (π/180), `0x43f17c` (2π),
  `0x45dc50` (1/65535), `0x45dbb8` (2^32, for unsigned conversions), `0x45dbdc`
  (0.0001), `0x45dbc0` (1.0) and `0x45dbd8` (0.0).
- Field names are those of `src/weapons.cpp`, `src/projectiles.cpp` and the
  object documents, and source line numbers are at `5c3f7a9`.
- No emulator, runtime testing, SDK or outside dataset was used. Names are the
  repository's own, or describe behaviour.
