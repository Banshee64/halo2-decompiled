# A biped actor's path movement (unknown_1f6090)

Retail range covered: `0x1f6090`–`0x1f7c6b`. This is one inventory entry, 7,132
retail bytes, `todo` with no source: the movement routine that `0x1f38c0` runs
for an actor whose unit is a biped, which decides whether the current path
point is reached, sends the unit the requests that the path entry needs, and
gives the facing and the throttle that take the actor toward the point.
**Analysis only:** this document adds no source, and nothing in it has been
built or checked against retail with the original compiler. Names are
provisional. The `function_<va>` form is primary; the descriptions are offered
for whoever decompiles it.

It uses the names of the two views of the actor, `s_actor_view`
(`include/slot_handler.h`) and `s_actor_moving` (`include/unknown_1e3920.h`);
of the movement code in `src/unknown_1f5560.cpp` and `src/unknown_1f2fe0.cpp`;
of `src/unknown_050650.cpp` for some actor fields; of
`src/path_transitions.cpp`, `include/unknown_1fa590.h` and
`include/unknown_26c380.h` for the path data; and of
[part 1](unknown_0b7740.md) of the object documents for `function_b7930` and
`function_b7b40`. Three callees have no source and are described only as far as
this function needs them.

## Boundary

- `0x1f5f30`, just before, is `matched` and `0x1f7c70`, just after, is `todo`;
  both have source in `src/unknown_1f5560.cpp`, and this function calls both.
  Both are excluded.
- Lane B's row of the Active claims table (issue #9: `0x1f0000`–`0x1fffff`,
  actor slot-handler callbacks) covers the function and its only caller,
  `0x1f38c0`. This document makes no claim and is offered to lane B.
- `0x1f6090` has no `@retail` or `@stub` marker, declaration or stub at
  `9feed5f`, and neither has its only caller, `0x1f38c0`.

## Conventions

"Callers" counts the functions that call an entry directly, from outside the
range and then from inside it.

| Retail | Bytes | Arguments | Returns | Callers | What it does |
| --- | --- | --- | --- | --- | --- |
| `0x1f6090` | 7132 | stack: actor index, vector to the path point, direction out, type out, movement out, turning out, reached (`ret 0x1c`) | void (inferred) | 1 + 0 | Moves a biped actor toward its current path point: arrival, path requests, facing and throttle |

There are no register arguments: `eax`, `ecx` and `edx` are written before they
are read (`0x1f6096`–`0x1f60a3`), and `ebx`, `ebp`, `esi` and `edi` are saved.
Nothing is returned (inferred `void`): `eax` holds whatever was last loaded at
either `ret`, and the caller does not read it (`jmp 0x1f3bff` at `0x1f3b93`).

Its only caller, `0x1f38c0` (1,408 bytes, `todo`, no source), keeps the actor
in `esi` and calls it at `0x1f3b8e` with pointers into the same actor
(`0x1f3b69`–`0x1f3b8d`):

| Argument | The caller passes | Use here |
| --- | --- | --- |
| 1 | the actor index | |
| 2 | `&actor+0x5ec` | read only: normally the vector from the actor to its current path point, which `function_1f2fe0` stores there (`src/unknown_1f2fe0.cpp:37-47`) |
| 3 | `&actor+0x6d4` | the direction out: the facing the unit should take. The caller copies `+0x290` there first (`0x1f38ec`–`0x1f3907`) |
| 4 | `&actor+0x5d6` | the type out: a word, the axis index below |
| 5 | `&actor+0x814` | the movement out: a throttle, in the facing's frame (inferred) |
| 6 | `&actor+0x5d3` | set while the actor turns in place, cleared while it moves |
| 7 | `&actor+0x5d2` | read and written: set when the point is reached; when a request or a jump for the entry succeeds; for a type-2 entry whose action word is 0 on a surface whose word `+0xa` is 2 (`0x1f6473`); and for types 1 and 6 when the actor's sector equals the entry's `+4` (`0x1f7170`). Cleared at `0x1f6258`, `0x1f625e` and `0x1f71ef` |

When `+0x456` is set, the caller first copies `+0x458` over `+0x5ec`, sets
`+0x5d0` and clears `+0x6d1` (`0x1f3952`–`0x1f397f`). It fills `+0x458` itself,
setting `+0x456`, with the plane normal that `function_1f3540` returns
(`0x1f390a`–`0x1f394f`; `src/unknown_1f2fe0.cpp:209-234`). `arg2` is then that
vector, not the vector to the path point.

The caller picks this function in its case for word `+0x270` = 0 (table
`0x1f3e28`, case at `0x1f3af3`): with `+0x229` clear, for a unit whose type
byte `+0xaa` is 0, a biped (`src/unknown_1f5560.cpp:157-166`), and whose
movement_type byte `+0x3dc` (`src/unknown_1f5560.cpp:136`) is neither 2
(`0x29bab0`) nor 5 (`function_1f5ab0`) (`0x1f3af3`–`0x1f3b67`). Before the
switch it clears `+0x6d2` and sets `+0x6d1` (`0x1f3912`–`0x1f3919`). After the
call, while `+0x456` is clear, it runs the switch again when `+0x5d2` is set,
`function_1f3100` has moved on to the next point
(`src/unknown_1f2fe0.cpp:51-79`) and `+0x5d0` is still set
(`0x1f3bff`–`0x1f3c27`), so one update can pass several points.

Two of those fields are also used directly. The function reads `+0x5d2`, the
field `arg7` points to, directly at `0x1f68d1`. In the seated case it clears
`+0x5d6` (`0x1f6662`), and later stores the old type back through `arg4`
(`0x1f7024`). On that path `+0x5d0` is clear, so the zero lasts only if
`function_1f58c0` stores 0 through `arg4` (`src/unknown_1f5560.cpp:294`), or
the path-entry part does for an entry of type 1 or 6 (`0x1f7137`) (inferred
from the caller's arguments).

## Data

The actor is `ebp` throughout: `actor_get(arg1)`
(`include/slot_handler.h:881`), the data of `g_4f55f0` plus (`arg1` & 0xffff) ×
0x888 (`0x1f6096`–`0x1f60bf`). The offset is kept at `esp+0x58` and used to
read the actor again at `0x1f6691` and `0x1f7065`. The frame is `0x1b0` bytes,
and frame offsets here are from `esp` after the four register pushes
(`0x1f60c2`), where the arguments are at `esp+0x1c4` to `esp+0x1dc`.

| Frame | Name here | Value |
| --- | --- | --- |
| `esp+0x20` | v | `arg2` with z cleared, divided by d; `+0x290` when d is below 0.0001. The weave can turn it |
| `esp+0x48` | d | the horizontal length of `arg2`, or 0 |
| `esp+0x4c` | facing | the direction output; starts as `+0x290` |
| `esp+0x2c` | throttle | the movement output; starts as the zero vector |
| `esp+0x3c` | axis | the type output, a word; starts as `+0x5d6`. The path-entry part reuses the slot for the surface (`0x1f70d2`) |
| `esp+0x44` | threshold | the facing dot product needed to move; starts at 0.8660254 |
| `esp+0x60` | scale | the throttle's size; starts at 1.0 |
| `esp+0x38` | block | the movement block, or NULL |
| `esp+0x5c` | entry | the current path entry, or NULL |
| `esp+0x43` | last | the current entry is the path's last |
| `esp+0x10` | flag | a slot with several uses: the bytes given in each part, the word that `function_e70b0` writes (`0x1f629b`), the dword first from `function_10f630` (`0x1f6570`), a float temporary (`0x1f66f9`) and the weave's angle θ |
| `esp+0x68` | buffer | a unit request (0x20 bytes), a trace result (`s_sector_trace_result`, 0x24 bytes) or path settings (`s_path_settings`) |

**The actor.** These are the main fields, with the names the sources give them
(`s_actor_view` unless another file is named). The parts below name the rest.

| Offset | Name in the source | Use here |
| --- | --- | --- |
| `+0x18` | unknown018, the unit | requests and tests |
| `+0x54` | unknown054, the character | for `function_1e4a50` and `function_1e4990` |
| `+0x86` | alert_state (`src/unknown_050650.cpp:24`) | at least 3 for the timed jump |
| `+0x225`, `+0x226` | unknown225, unknown226 | a one-shot request, and its done mark |
| `+0x229` | unknown229 | blocks some requests |
| `+0x238` | position | |
| `+0x264` | unknown264; direction_locked (`src/unknown_050650.cpp:30`) | blocks requests, the point prepared for type 6 and the pending jump |
| `+0x267` | unknown267; seated (`src/unknown_050650.cpp:33`) | stops the movement |
| `+0x26c` | unknown26c; the vehicle (`src/unknown_1f5560.cpp:55`) | must be NONE for the weapon change and the pending jump |
| `+0x27c` | unknown27c (`s_location_view`), with the output index at `+0x288` and the sector at `+0x28c` | origin and sector of the traces |
| `+0x290` | unknown290; forward (`src/unknown_050650.cpp:39`) | the current facing |
| `+0x454` | unnamed word | forces the axis when it is 0 to 3 |
| `+0x464`–`+0x474` | unknown464, unknown465, unknown466, unknown468 | the pending jump: set; send request `0x30`; has a direction; then x, y and two reals. unknown468 is typed `s_type_c3b527`, whose output_index would be at `+0x474`, where this function writes a real |
| `+0x504`, `+0x50c` | unknown504 and unknown50c; `src/unknown_050650.cpp:65` names `+0x50c` path_active | the path is active when `+0x50c` is set and `+0x504` is 1 |
| `+0x539`, `+0x53a` | path_count, path_index (`include/unknown_1e3920.h:95-96`) | the current entry |
| `+0x53c` | unknown53c[4] | the path entries, below |
| `+0x5ac`, `+0x5b0` | unknown5ac, unknown5b0 | a target object and its kind (1, 3, 4 or 5) |
| `+0x5d0` | unknown5d0; movement_aiming (`src/unknown_050650.cpp:72`) | moving along the path; cleared to stop |
| `+0x5d1` | unknown5d1 (`s_actor_moving`); movement_aiming_valid (`src/unknown_050650.cpp:73`) | lets `0xe64e0` choose the axis |
| `+0x5f8` | movement_aiming_direction (`src/unknown_050650.cpp:76`) | the direction `0xe64e0` snaps to an axis |
| `+0x604` | unnamed byte | set while the actor turns in place |
| `+0x622`–`+0x630` | unknown622 to unknown630 (`s_actor_moving`) | the pending facing |
| `+0x634`–`+0x648` | unknown634, unknown638, unknown648 (`s_actor_moving`) | a deadline, a point and a vector for `function_1f58c0` |
| `+0x6c8`, `+0x6cc` | unnamed; `function_1dfb90` clears both (`src/ai.cpp:1334-1335`) | a pending animation, and whether it waits for the facing |
| `+0x6d1`, `+0x6d2` | unknown6d1 (`s_actor_moving`); unrestricted_looking, attention_selected (`src/unknown_050650.cpp:94-95`) | set or cleared by branch |
| `+0x6fa` | unnamed word, cleared by `function_1dfb90` (`src/ai.cpp:1331`) | the weave counter |
| `+0x7f8` | unnamed | the game time of the last timed jump |
| `+0x7fc` | field_7fc (`src/unknown_1e3920.cpp:42`) | the unit animation mode |

**The path entries.** The current entry is `actor + 0x53c` + path_index × 0x1c
(`lea edi, [edx + ebp + 0x53c]` at `0x1f6234`): s_actor_view's unknown53c[4],
an `s_actor_point_entry` (`include/slot_handler.h:607`) whose first 0xc bytes
the header leaves unnamed. This function reads:

| Entry offset | Use |
| --- | --- |
| `+0` | the type, a word: −1, 1, 2, 5 and 6 are tested |
| `+4` | a long, compared with the actor's sector `+0x28c`, and a node index into the pathfinding data |
| `+8` | a word, a surface index or NONE |
| `+0xc` | the point (`s_type_c3b527`), with its output_index at `+0x18` |

`include/unknown_1e3920.h:98` starts path[4] at `+0x548` with the point first.
The point is at the same address, but for i ≥ 1 entry i's first three fields
would fall in path[i − 1]'s unknown10 (entry 0's in s_actor_moving's
unknown53b), and path[3] would run over `+0x5ac` and `+0x5b0`, which this
function reads as a long and a word. The header's comment records that
disagreement (lines 1-6); this function's base fits `s_actor_view` (inferred).

**The movement block** is `function_1e4a50(actor +0x54)`, "the actor's movement
block of its character tag" (`src/unknown_1e3920.cpp:45-50`):

| Offset | Use |
| --- | --- |
| `+0` | flags byte: bit 5 (`0x20`) for requests, bit 1 (`2`) for `+0x5d1`, bit 3 (`8`) for the timed jump |
| `+4` | a real, unknown4 of `s_character_a50` (`src/unknown_1be410.cpp:30-34`): a distance in the jump tests |
| `+8` | arrival_radius |
| `+0x1c` | marker_flags of `s_transition_character` (`src/path_transitions.cpp:34-38`): tested with `0x1803` and `0xe0` |
| `+0x20` | a real: the largest scale, when positive |

**The pathfinding data** is `function_1fa7f0()`, an `s_pathfinding_data`
(`include/unknown_1fa590.h:42-53`). This function reads its nodes (`+4`, 8
bytes, the flags word first), vertices (`+0x2c`, 12 bytes) and surfaces
(`+0x3c`, 0x14 bytes). The surface fields it reads fit `s_transition_surface`
(`src/path_transitions.cpp:39-46`): type `+0`, the words vertices[1] to
vertices[3] at `+6`, `+8` and `+0xa`, the byte at `+0xc` (bit 0; the address of
`+0xd` goes to `function_1f8160` as its types pointer), and source_output and
destination_output at `+0x10` and `+0x12`. `s_pathfinding_surface`
(`include/unknown_1fa590.h:23-30`) lays out the same 0x14 bytes differently
after `+4`.

**The unit requests** go out through `function_e6900` or, with no arguments,
`function_e68c0`. `s_unit_request` (`include/unit_requests.h:11-71`) has no
members for the types built here:

| Type | Built at | Arguments |
| --- | --- | --- |
| `0x22` | `0x1f642b`, `0x1f64a8` | a point at `+4`, a 2D vector at `+0x10`, a byte at `+0x18` |
| `0x2b` | `0x1f727a` | a point at `+4`, a vector at `+0x10` |
| `0x30` | `0x1f7b44` | zeroed first; byte 1 at `+4`, a 2D direction at `+8` |
| `0x27`, `0x28`, `0x2c`, `0x33` | `function_e68c0` | none |

## The function

It has no loop and returns only at its end: every path reaches the pending-jump
part at `0x1f7ac9`, and the exits are `ret 0x1c` at `0x1f7c25` and `0x1f7c39`.
Three jump tables of four entries follow the code (`0x1f7c3c`, `0x1f7c4c` and
`0x1f7c5c`; `tools/disasm.py` prints them as one table of 12), and the path
entry's type is a compiled switch at `0x1f70f1`. The parts below are address
ranges; where one jumps into another's range, the text says so.

### Inputs (`0x1f6090`–`0x1f6209`)

1. block = `function_1e4a50(actor +0x54)` (`0x1f60c7`); axis = word `+0x5d6`;
   word `+0x454` is copied to `esp+0x64`; facing = `+0x290`; throttle = the
   zero vector `g_4687a4` (`0x1f60cc`–`0x1f611e`). scale = 1.0, threshold =
   0.8660254, last = 0 and entry = NULL (`0x1f6142`–`0x1f616d`).
2. d is the length of `arg2`'s x and y, on the x87 (`0x1f6117`–`0x1f6175`).
   When 0.0001 > |d|, d = 0 and v = `+0x290` (`0x1f61f4`, `0x1f61c5`).
   Otherwise v's x and y are `arg2`'s divided by d, and its z is (1 / d) × 0.0
   (`0x1f6187`–`0x1f61bd`); a d equal to 0 would also take `+0x290` (`jp` at
   `0x1f61c3`). This is the shape of an inline normalisation with a fallback,
   applied with z cleared (inferred).
3. The arrival radius is the block's arrival_radius but at least 0.05, or 0.2
   without a block (`0x1f61d9`–`0x1f6202`).

### The current path point (`0x1f620a`–`0x1f6260`)

1. With `+0x50c` set and word `+0x504` = 1, the test `function_1f8660` makes
   (`src/unknown_1f8640.cpp:19-28`): entry = the entry at path_index, and last
   = (path_index = path_count − 1) (`0x1f621e`–`0x1f6244`).
2. Unless the entry's type is 2, `*arg7` = (radius > d)
   (`0x1f624a`–`0x1f625c`). An entry of type 2 leaves `*arg7` as it is.
3. Without an active path, `*arg7` = 0 (`0x1f625e`).

### A one-shot request (`0x1f6261`–`0x1f62d4`)

When `+0x225` is set and `+0x226` clear, `function_110ab0(unit)` is false,
`+0x264` and `+0x229` are clear, and the unit's action word is 0
(`s_unit_actions` unknown36, `src/unknown_0a76b0.cpp:21-46`, read with
`function_e70b0`): `function_e68c0(0x33, unit)` (`0x1f62b0`) and, when it
succeeds, `function_114b60(NONE, 0x10, unit, 0xd, NULL)` (`0x1f62c9`). Either
way `+0x226` = 1 (`0x1f62ce`).

### Requests for the current entry (`0x1f62d5`–`0x1f653e`)

This part runs when `*arg7` is clear, there is a block, `function_110ab0(unit)`
is false and `function_1f8660(arg1)` is true (`0x1f62d5`–`0x1f6309`). It reads
the action word inline as a: the unit's word `+0x346` gives the offset of its
actions block, and a is that block's `+0x36` (`0x1f630f`–`0x1f6331`). The first
case that applies:

1. a is 6 or 7: request `0x27` (`0x1f6341`, sent at `0x1f653a`).
2. Word `+0x4ac` is 6, 5 or 3, `+0x229` is clear and the block has bit `0x20`:
   request `0x28` when a is 5, otherwise `0x2c` unless `+0x264` is set; then
   `+0x5d0` = 0 (`0x1f634b`–`0x1f63b0`).
3. The entry's type is 2. With a surface index of NONE, or `+0x264` set, it
   only sets `+0x5d0` = 0 (`0x1f63c2`–`0x1f63d7`, `0x1f64fe`). Otherwise, when
   a is 5: request `0x22` with the unit's position (`function_b9dd0`), the x
   and y of its forward (`function_b9fc0`, with up NULL) and byte `+0x18` = 1
   (`0x1f63e3`–`0x1f6443`). When a is 0 and the surface's word `+0xa` is 2:
   `*arg7` = 1 (`0x1f6473`). When a is 0 otherwise: request `0x22` with the
   vertex at the surface's word `+6`, the x and y of the vertex at its word
   `+8`, and byte `+0x18` = 0 (`0x1f6489`–`0x1f64e5`). Any other a: request
   `0x27` (`0x1f64f9`). Each of these then sets `+0x5d0` = 0.
4. The entry's type is −1 and a is 5: request `0x28` when the block has bit
   `0x20`, or when `function_1e4990(actor +0x54)` gives a block with bit 1
   (`2`); otherwise `0x27` (`0x1f6513`–`0x1f653a`).

### The animation mode (`0x1f653f`–`0x1f6649`)

1. mode = `function_1f5f30(arg1)` (`0x1f6546`), whose source chooses "the unit
   animation mode from the actor's requested mode and state".
2. Only for `mode == 0x4000089`, with `+0x26c` NONE:
   `function_10f630(unit, &first, &second)` must succeed with first not already
   the mode, and `function_10fcd0(unit, 0x4000089, 0x7000101, 0x7000101)` must
   fail (`0x1f6559`–`0x1f65a1`). Then the mode stands only if the unit is given
   the character's weapon (`0x1f65a7`–`0x1f6636`): the current weapon
   (`function_cbd50(unit, current_weapon)`, the char at `+0x212`), the element
   of `function_1e4ef0(arg1)` and its tag index `+0x48` must all exist, and the
   weapon's tag must differ from that index. Then `function_b7930` fills
   placement data at `esp+0xfc` (that tag, the unit, no owner),
   `function_b7b40` creates the object, and `function_cd0c0(unit, object, 4)`
   must succeed. By that function's comment, mode 4 puts the weapon in the
   first hand, also changing weapons (`src/unit_object_type.cpp:5348-5353`). If
   one of the first tests fails, the mode is kept (`0x1f6644`); only a failure
   in the weapon steps sets `mode = 0x6000086` (`0x1f6638`).
3. field_7fc = mode, unless the mode is NONE (`0x1f663f`–`0x1f6644`).

### Whether to move, and a pending animation (`0x1f664a`–`0x1f689a`)

The first case that applies, each going on to the flags at `0x1f689b`:

1. Seated (`+0x267`): `+0x5d0` = 0, word `+0x5d6` = 0, and `+0x6d1` = 1 when
   word `+0x4` is `0xf` or long `+0x274` is not NONE, else 0
   (`0x1f6654`–`0x1f6686`).
2. The unit is not NONE and `function_110ab0(unit)` is true: `+0x5d0` = 0 and
   `+0x6d1` = 0 (`0x1f6691`–`0x1f66be`).
3. A pending animation (long `+0x6c8` not 0): `+0x6d1` = 1, `+0x5d0` = 0, and
   word `+0x620` = 2.0 × field_2_3, through `cvtsi2ss` and `fistp`
   (`0x1f66d5`–`0x1f671a`), which is two seconds in ticks (inferred from
   `function_1469f0`, `src/unknown_1469f0.cpp:2-3`). With `+0x6cc` clear,
   `function_1f57f0(arg1, +0x6c8, NULL)` plays it at once (`0x1f67c6`). With
   `+0x6cc` set, it waits for the override direction: word `+0x686` above 0,
   word `+0x684` at least 1,
   `function_2967b0(arg1, &+0x688, false, &dir, NULL, NULL)` true, and dir with
   z cleared normalised to a positive length (`0x1f6727`–`0x1f677a`). It then
   plays when dir · `+0x290` > 0.98, and otherwise sets `+0x44d` = 1 and keeps
   the animation (`0x1f677c`–`0x1f67eb`). Playing it, or failing any of those
   tests, clears `+0x6c8` and `+0x6cc` (`0x1f67cb`).
4. Otherwise (`0x1f67f7`): a non-zero action word, or `field_7fc == 0x6000084`,
   sets `+0x5d0` = 0 and `+0x6d1` = 0; `+0x264` set with `+0x229` clear sets
   `+0x5d0` = 0 and `+0x6d1` = 1; word `+0x3e2` above 0 sets `+0x5d0` = 0. When
   none of these applies, `+0x5d1` is cleared (`0x1f6894`), unless
   `field_7fc == 0x6000086` and the block is NULL or lacks bit 1.

### Flags (`0x1f689b`–`0x1f68de`)

Word `+0x3e0` above 0 sets `+0x449` and `+0x44a` (`0x1f68a4`), and `+0x44d`
sets `+0x6d2` (`0x1f68bc`). When `+0x5d0` is clear or `+0x5d2` is set, it jumps
to `0x1f700a`, inside the throttle part's range, which clears `+0x604` and goes
on to the outputs. They then hold the starting values: facing `+0x290`, the old
`+0x5d6` and a zero throttle. `*arg6` is not written.

### Weaving (`0x1f68df`–`0x1f6b7d`)

1. It needs byte `+0x481` and d > 0.8; otherwise word `+0x6fa` = 0
   (`0x1f6b77`). The element of `function_1e4ef0(arg1)` must exist with
   positive reals at `+0x3c` (a period in seconds, inferred) and `+0x38` (an
   angle), and after `function_26c180(arg1)`, which "computes the actor's
   location once per update" (`src/unknown_26bfa0.cpp:137`), the sector
   `+0x28c` must not be NONE; otherwise `+0x6fa` = 0 (`0x1f6b6e`).
2. ticks = `function_1469f0(element +0x3c)` (`0x1f6972`). The angle is element
   `+0x38`, times (d − 0.8) × 1.25 when d < 1.6 (`0x1f6977`–`0x1f69a3`).
3. q = 2 × word `+0x6fa` / (short)ticks (`cdq` and `idiv`,
   `0x1f69a9`–`0x1f69b6`; the divisor is sign-extended from `cx` at
   `0x1f69b2`). q = 1 negates the angle and q > 1 resets `+0x6fa` to 0
   (`0x1f69b8`–`0x1f69cf`), so the sign flips every half period (inferred).
4. v is turned by the angle about `g_4687b0`, (0, 0, 1), with Rodrigues'
   formula: u cos θ + (a × u) sin θ + a (a · u)(1 − cos θ), with `fsin` at
   `0x1f69df` and `fcos` at `0x1f6a43` (`0x1f69d6`–`0x1f6aef`).
5. `function_210770(+0x288, &turned, &out)` converts it out of world space
   (`include/unknown_20fe20.h:18`; `0x1f6af5`, its result unused), and
   `function_26c590` traces 0.5 units along it (`0x1f6b27`): the pathfinding
   data from `function_1fa7f0`, origin `&+0x27c`, sector `+0x28c`, target
   sector NONE, direction `&out`, location NULL, and the trace into the buffer.
   Its return value is not read: when the trace's blocked byte (`+0`) is set,
   `+0x6fa` = 0; when it is clear, v = the turned vector, `+0x6fa` rises by 1
   and threshold = −0.2 (`0x1f6b2c`–`0x1f6b66`).

### Choosing the facing (`0x1f6b7e`–`0x1f6c78`)

1. When the copy of word `+0x454` is 0 to 3 (signed tests at
   `0x1f6b82`–`0x1f6b8b`), axis = that word and the first table (`0x1f7c3c`,
   used at `0x1f6b94`) sets facing from v: axis 0 gives (x, y, z), 1 gives (−x,
   −y, z), 2 gives (−y, x, z) and 3 gives (y, −x, z) (`0x1f6b9b`–`0x1f6bf8`).
2. Otherwise, threshold = 0 when d > 0.8 and the threshold is positive
   (`0x1f6bfa`–`0x1f6c17`). Then, with `+0x5d1` set, `0xe64e0` chooses the axis
   and the facing from `arg2`, `+0x5f8` and `+0x290` (`0x1f6c27`–`0x1f6c46`;
   see [Callees without source](#callees-without-source)); with it clear,
   facing = v and axis = 0 (`0x1f6c4d`–`0x1f6c65`).

### Turning or moving (`0x1f6c79`–`0x1f6e9d`)

1. dot = facing · `+0x290`, kept at `esp+0x64` (`0x1f6c79`–`0x1f6cab`). With
   `+0x604` set, the threshold is raised to at least 0.99
   (`0x1f6cb3`–`0x1f6cc6`).
2. When dot is not above the threshold, flag = 0: the actor turns in place
   (`0x1f6d16`). Otherwise flag = 1 (`0x1f6cd6`), and the move is checked when
   `function_26bf10(unit)` is true and, after `function_26c180(arg1)`, the
   sector is not NONE (`0x1f6cdb`–`0x1f6cfb`).
3. The second table (`0x1f7c4c`, used at `0x1f6d0f`) gives the direction in
   which the current facing f = `+0x290` moves the actor for the axis: f,
   (−f.x, −f.y, f.z), (f.y, −f.x, f.z) or (−f.y, f.x, f.z), the first table's
   mapping turned around (`0x1f6d20`–`0x1f6d98`). It must keep a positive
   length through `normalize2d` (`0x1f6da2`).
4. `function_210770` and `function_26c590` trace 0.4 units along it, as in the
   weave (`0x1f6dc6`–`0x1f6df8`). A true return raises the threshold to at
   least 0.95 and sets flag = (dot > threshold) (`0x1f6e01`–`0x1f6e2c`). In the
   source of `function_26c590` (`todo`), true goes with `blocked` set (for
   example `src/unknown_26c380.cpp:205-207` and 309-311).
5. Whatever it returned, when the trace's unknown1c (`+0x1c`) is set and its
   unknown20 (`+0x20`) is above 0 and below 1.5 × block `+4`, a jump is made
   pending: `+0x464` = `+0x466` = 1, `+0x468` and `+0x46c` = the direction's x
   and y, and `+0x470` = `+0x474` = 0.75 (`0x1f6e31`–`0x1f6e96`).
   `function_1f8160` fills the same fields (`src/unknown_1f5560.cpp:623-638`).
   The block is read here without a NULL test (`0x1f6e40`).

### The throttle (`0x1f6e9e`–`0x1f7010`)

1. `function_1f40b0(arg1, &stopping, &round_trip)` (`0x1f6eb0`) gives the two
   distances its source describes (`src/unknown_1f4460.cpp:102-105`), stopping
   at `esp+0x64` and round_trip at `esp+0x8c`.
2. Unless `+0x4ae` is set, when stopping > d: scale = (d − round_trip) /
   (stopping − round_trip) when d > round_trip + 0.05, stopping > round_trip
   and that ratio is below 1.0 (a ratio of 1.0 or more keeps the scale);
   otherwise scale = 0 (`0x1f6ebf`–`0x1f6f0c`).
3. With flag clear: `+0x604` = 1, `+0x6d2` = 1 and `*arg6` = 1
   (`0x1f6ff1`–`0x1f7006`). The throttle stays zero.
4. With flag set: the third table (`0x1f7c5c`, used at `0x1f6f28`) picks (1,
   0), (−1, 0), (0, −1) or (0, 1) for axes 0 to 3, keeping the throttle's own x
   or y, still 0 (`0x1f6f2f`–`0x1f6f63`). `+0x483` sets scale = 0.5
   (`0x1f6f79`), and a block whose `+0x20` is positive caps the scale at it
   (`0x1f6f87`–`0x1f6fae`). The throttle is the pair times the scale, with z
   times the scale, then `+0x604` = 0 and `*arg6` = 0 (`0x1f6fb4`–`0x1f6fec`).
   The movement output is thus (scale, 0, 0), (−scale, 0, 0), (0, −scale, 0) or
   (0, scale, 0) in the facing's frame, with x forward and y to the left
   (inferred).

The default arms of the second and third tables (`0x1f6d9e`, `0x1f6f63`) cannot
be reached, since the axis is 0 to 3 by then (inferred).

### Outputs (`0x1f7011`–`0x1f7064`)

`*arg4` = axis, `*arg3` = facing and `*arg5` = throttle
(`0x1f7011`–`0x1f7055`). Then `function_1f58c0(arg1, arg3, arg4)` (`0x1f7060`):
while the game time is before `+0x634`, it can turn `*arg3` toward the point at
`+0x638` and set `*arg4` = 0, and near the point it sends request `0x2d`
(`src/unknown_1f5560.cpp:275-313`). Its result goes to `bl` and to the flag
(`0x1f7069`, `0x1f707e`).

### Path entry types (`0x1f7065`–`0x1f75ae`)

1. It reads the actor again and needs `+0x50c` set and word `+0x504` = 1
   (`0x1f7065`–`0x1f7090`), `function_110ab0(unit)` false, the result of
   `function_1f58c0` false and an entry type other than −1
   (`0x1f7096`–`0x1f70b7`). Otherwise it goes to the target object at
   `0x1f75af`.
2. The pathfinding data replaces the actor offset at `esp+0x58`, flag = 1, and
   surface = surfaces + index × 0x14 when the entry's index is not NONE and the
   data exists, else NULL (`0x1f70bd`–`0x1f70ed`).
3. The switch at `0x1f70f1` (`dec`, `sub 4`, `dec`) sends types 1 and 6 to
   `0x1f7103` and type 5 to `0x1f7178`; any other type goes to `0x1f75af`.
   Every exit below continues into the target object at `0x1f75af`.

**Types 1 and 6** (`0x1f7103`–`0x1f7177`, `0x1f72ee`–`0x1f75ae`):

1. `function_26c180(arg1)`; `*arg3` = v, `*arg4` = 0, `*arg5` = the zero
   vector, `+0x6d2` = 1 and `+0x6d1` = 0 (`0x1f710a`–`0x1f7159`).
2. When the actor's sector `+0x28c` equals the entry's `+4`, `*arg7` = 1 and
   the part ends (`0x1f7160`–`0x1f7173`).
3. Otherwise the actor must be lined up (`0x1f72ee`–`0x1f7348`). With `+0x5d0`
   set, v.x f.x + v.y f.y for f = `+0x290` must exceed the double 0.95 at
   `0x45e5c0`; with it clear, the action word from `function_e70b0` must be 5.
   A surface of type 1 with bit 0 of byte `+0xc` also fails. Failing clears
   `+0x5d0` (`0x1f731c`).
4. point = the entry's point in world space, `function_210850(entry +0xc)` at
   `esp+0x2c` (`0x1f7355`), and target = a copy of it at `esp+0x14`; b = (the
   surface exists and its `+0x10` and `+0x12` differ) (`0x1f735a`–`0x1f7388`).
5. For type 6, `function_2702a0` (`0x1f73c6`;
   `src/path_transitions.cpp:381-384`) gets `arg1`, `mode = 0x11000555`,
   `set = 0x5000049`, `target_mode = 0x6000086`, `target_set = 0x800001e`,
   position `&point`, forward `g_4687a8`, which points to (1, 0, 0),
   out_position `&target`, and NULL for the rest. When it succeeds, target
   keeps the z it returned with point's x and y; otherwise target = point again
   (`0x1f73cb`–`0x1f73fd`).
6. `function_1f8160(arg1, surface ? surface +0xd : NULL, &target, !b)`
   (`0x1f741e`). True: `*arg5` = (1, 0, 0) (`0x1f742b`–`0x1f7445`). False with
   b: `*arg5` = the zero vector and `+0x5d0` = 0, and the part ends
   (`0x1f74fb`–`0x1f751f`). False without b: a jump is made pending as in the
   turning part, but along v and with `+0x470` = `+0x474` = 1.5
   (`0x1f7524`–`0x1f7556`).
7. For type 6 with `+0x264` clear, after a true result or the pending jump
   (`0x1f7450`–`0x1f7466`), it prepares the point that `function_1f58c0` will
   face. node = the pathfinding nodes + entry `+4` × 8; the collision result at
   `esp+0xa0` gets unknown24 = NONE (`0x1f7482`); the entry's point in world
   space again, at `esp+0x94` (`0x1f748c`); the vector at `esp+0x2c`, still
   point, becomes the axis of the node's flags through `function_1fa600`
   (`0x1f7498`), which leaves it as it is when no flag matches; then
   `function_26d100(&vector, &collision, NULL, &esp+0x94)` (`0x1f74b2`). Unless
   that returns NONE, the vector at `+0x28` of the collision result
   (`esp+0xc8`) is converted by `function_210770(entry +0x18, ...)`, and the
   converted vector is kept when that succeeds, the raw one otherwise
   (`0x1f74c0`–`0x1f74e0`, `0x1f7563`). Then `+0x634` = the game time + 4 ×
   field_2_3, `+0x638` = the entry's point and `+0x648` = the vector
   (`0x1f756f`–`0x1f75ac`).

**Type 5** (`0x1f7178`–`0x1f72ed`):

1. It needs `+0x5d0`. `function_26c180(arg1)`; `*arg3` = v, `*arg4` = 0,
   `*arg5` = the zero vector, `+0x6d2` = 1, `+0x6d1` = 0 and `*arg7` = 0
   (`0x1f7186`–`0x1f71ef`).
2. When v.x f.x + v.y f.y exceeds the double 0.95: settings =
   `function_1f9240(arg1)` into the buffer (`0x1f7222`), end =
   vertex[the surface's `+8`], and `function_26f2d0(&settings, &position, end)`
   (`0x1f7247`), true when end.z − start.z lies within a type bound that the
   settings allow (`src/path_transitions.cpp:100-118`). True: request `0x2b`
   with end at `+4` and −vertex[the surface's `+0xa`] at `+0x10`
   (`0x1f7250`–`0x1f72b8`). False: `function_1f8160(arg1, NULL, end, true)`
   (`0x1f72d9`). Either one succeeding sets `*arg7` = 1 (`0x1f72c5`,
   `0x1f72e6`).
3. Not lined up: `+0x5d0` = 0 (`0x1f731c`).

No NULL test guards the pathfinding data or the surface for type 5 (`0x1f722b`,
`0x1f7232`), the pathfinding data for type 6 (`0x1f7475`), or the pathfinding
data in the type-2 requests (`0x1f6462`, `0x1f648e`). The surface is NULL when
the entry's index is NONE.

### The target object (`0x1f75af`–`0x1f79e3`)

This part needs a block and `+0x5ac` not NONE (`0x1f75af`–`0x1f75c2`).

1. With the block's `+0x1c` dword tested against `0x1803`,
   `function_110ab0(unit)` false and `+0x5d0` set: for word `+0x5b0` = 3, flag
   = `function_1f7c70(arg1, 3, true, arg3, arg4, arg5)`; for 5, flag =
   `function_1f7c70(arg1, 5, false, arg3, arg4, arg5)` (`0x1f75c8`–`0x1f765b`).
2. A set flag ends the part (`0x1f765f`). The flag holds the result of
   `function_1f7c70` when that ran; otherwise 1 when the path-entry part got
   past its first step (`0x1f70d6`), or else the result of `function_1f58c0`
   (`0x1f707e`).
3. Kind 1 (word `+0x5b0` = 1, `function_110ab0` false and `+0x5d0` set,
   `0x1f766b`–`0x1f7698`): o = the object, offset = o's `+0x30` − `+0x238`,
   normalised by `function_30bf0` (`0x1f76f0`). It needs a positive length
   below 2 × block `+4` + o's `+0x3c`, and p = (offset · v) × length > 0
   (`0x1f76f5`–`0x1f7762`). If v · `+0x290` > 0.99: flag = 1 << c, where c =
   max(1, min(n, 6) − 1) for the word n at `+0xe` of the first element of the
   block at `+0x5c` and `+0x60` of o's tag (`g_4e3b44`), and c = 1 when that
   block is empty (`0x1f77a0`–`0x1f77eb`); target = `+0x238` + v × 2p, past the
   object (inferred) (`0x1f77e5`–`0x1f7839`); then
   `function_1f8160(arg1, &flag, &target, true)`, with the flag as its types
   (`0x1f783f`). If v · `+0x290` is not above 0.99, it stops and turns: `*arg3`
   = v, `*arg4` = 0, `*arg5` = the zero vector, `+0x6d2` = 1, `+0x6d1` = 0 and
   `+0x5d0` = 0 (`0x1f785f`–`0x1f78b1`).
4. Kind 4, when kind 1 does not apply or fails its tests (`0x1f78bd`): it needs
   the block's byte `+0x1c` & `0xe0`, word `+0x5b0` = 4 and `function_110ab0`
   false, and with `+0x5d0` clear it goes straight to the pending jump
   (`0x1f78f2`). Otherwise
   `function_26fc80(arg1, +0x5ac, block +4, &marker, &target)` (`0x1f791c`, the
   marker at `esp+0xfc`) must succeed, and target − `+0x238` must have a
   `magnitude3d` (`0x1f7969`) above 0 and below 2 × block `+4` + o's `+0x3c`;
   then `function_1f8160(arg1, NULL, &target, true)` (`0x1f79bd`).
5. When `function_1f8160` succeeds in kind 1 or 4:
   `function_1f9450(arg1, +0x5ac)`, then `+0x5ac` = NONE and word `+0x5b0` = −1
   (`0x1f79cf`–`0x1f79dd`).

### A timed jump (`0x1f79e4`–`0x1f7ac8`)

When `+0x5d0` and last are set, `+0x5ac` is NONE, `+0x464` is clear, the block
has bit 3 (`8`), alert_state is at least 3, d > 2.0, and the game time less
`+0x7f8` exceeds 3 × field_2_3 (`0x1f79e4`–`0x1f7a64`):
`function_1f8160(arg1, NULL, &(+0x238 + *arg2), false)` (`0x1f7ab5`), whose
result is not read, and `+0x7f8` = the game time (`0x1f7ac3`). The target is
the actor's position plus `arg2`: normally the current path point, the path's
last one by the last test (inferred: when `+0x456` is set, the caller has put
the plane normal `+0x458` there instead).

### The pending jump and facing (`0x1f7ac9`–`0x1f7c39`)

1. With `+0x264` set or `+0x26c` not NONE: `+0x622` = 0, and return
   (`0x1f7c28`).
2. Return unless `+0x464` is set and `function_1f50e0(arg1)` is false
   (`0x1f7ae4`–`0x1f7b00`). Nothing in this function clears `+0x464`.
3. With `+0x465` set: request `0x30` with byte `+4` = 1 and direction `+0x468`,
   `+0x46c` when `+0x466` is set; otherwise the x and y of `+0x290` through
   `normalize2d`, or `g_468778`, (1, 0), when they have no length
   (`0x1f7b14`–`0x1f7ba1`). When the request succeeds:
   `function_1fb7e0(arg1, 0x5a, NULL, NONE, -1)` (`0x1f7bc7`).
4. With `+0x465` clear, or when the request fails: `function_1e4260(arg1)`,
   which sets bit 1 of flags810 (`0x1f7bd5`; `src/unknown_1e4290.cpp:66-72`).
5. With `+0x466` set: `+0x624` to `+0x630` = `+0x468` to `+0x474` and `+0x622`
   = 1 (`0x1f7be4`–`0x1f7c17`), the pending facing that `function_1f5560`
   applies (`src/unknown_1f5560.cpp:55-72`).

## Callees without source

`function_b7b40`, in lane AB's range, creates an object from placement data
([part 1](unknown_0b7740.md)) and is a stub in `src/stubs/unknown_09a9f0.cpp`.
The other two are described as far as this function needs them.

### `0xe64e0`: snapping a direction to the path's axes

`0xe64e0` (798 bytes, `todo`, no source, in no claimed range) has one other
caller, `0x150400` (`0x1504ec`); [biped helpers](unknown_0e5040.md) describes
it in full. It takes a reference vector in `eax`, a direction in `esi` and a
fallback in `edi`, and on the stack a bool, the axis out and the index out
(`ret 0xc`). It returns nothing, and either output may be NULL. Here: `arg2`,
`&+0x5f8`, `&+0x290`, false, &facing and &axis.

1. v = `*eax` normalised by `function_30bf0`, with z cleared first when the
   bool is clear (`0xe6568`). With the bool set it normalises all three
   components and uses a zero w (`0xe650b`–`0xe6566`).
2. The axes are v, −v, w = (−v.y, v.x, 0) and −w, indices 0 to 3
   (`0xe65a9`–`0xe65bf`, `0xe6629`–`0xe6684`).
3. With d = `*esi` and f = `*edi`: it takes ±v by the sign of d · v when |d ·
   v| exceeds 0.3 (if |f · v| is at least 0.5) or 0.4 (otherwise), or when |d ·
   `g_4687b0`| exceeds 0.9. Otherwise it takes ±w by the sign of d · w
   (`0xe670d`–`0xe67ca`). The constants are `0x44ae90` (0.3), `0x45dd30` (0.4),
   `0x45dbbc` (0.5) and `0x45dc84` (0.9).
4. It writes the index and the axis (`0xe67cf`–`0xe67f3`). When v has no length
   it writes index 0 and, with the bool clear, d with z cleared, normalised by
   `function_1201a0`, which falls back to f (`0xe65c9`–`0xe661c`).

### `function_26d100`: the path-location query (lane V)

This stub (`src/stubs/lane_c.cpp`) is the "path-location query" of
[the path transitions document](path_transitions.md). Here it takes the vector
in `eax`, the collision result in `ecx`, NULL in `edx` and the point on the
stack (`ret 4`), the order of `0x26bfa0`'s call (`0x26c046`–`0x26c056`), and
its result is compared with −1. Its callers with source all pass `g_4687b0`,
(0, 0, 1), as `up`, and those in `src/unknown_26bfa0.cpp` and
`src/path_transitions.cpp` set unknown24 to NONE first, as this one does (for
example `src/unknown_26bfa0.cpp:105-106`). This one passes the axis of the
node's flags or, when no flag matches, the entry's point. It then reads a
vector at `+0x28` of the collision result, which `s_collision_result_1697c0`
(`include/unknown_0259a0.h:37-44`) leaves unnamed.

## Existing declarations

`0x1f6090` itself is declared nowhere, nor is its caller `0x1f38c0`; it is
mentioned only in the caller lists of `function_b7930` and `function_b7b40` in
[part 1](unknown_0b7740.md), and as a caller of `0xe64e0` in
[biped helpers](unknown_0e5040.md). Every callee with source is declared with
as many parameters as retail passes, wherever retail puts them. Four
declarations matter here:

| Retail | Declared as | Where | What retail shows |
| --- | --- | --- | --- |
| `0x1f7c70` | `bool __stdcall function_1f7c70(long actor_index, short type, bool allow_jump, vector3f *direction_out, short *type_out, vector3f *movement_out)` | `src/unknown_1f5560.cpp` | It receives `arg3` to `arg5` unchanged (`0x1f7606`–`0x1f7656`), and its names fit what this function writes through them: the facing, the axis and the throttle |
| `0x1f5ab0` | `void __stdcall function_1f5ab0(long actor_index, vector3f const *input, vector3f *movement_out, short *type_out, vector3f *facing_out, bool *blocked)` | `src/unknown_1f5560.cpp` | Not a callee: the caller's routine for movement type 5, given six of this function's seven slots, all but `+0x5d3` (`0x1f3b95`–`0x1f3bb3`; it pops six, `ret 0x18`). Retail writes the vector it copies from `+0x290` (`0x1f5ad8`) through the third (`0x1f5ea0`–`0x1f5ebc`), and the one that starts as the zero vector (`0x1f5af0`) and can become (0, 0, ±1) (`0x1f5e8d`–`0x1f5e99`) through the fifth (`0x1f5ecc`–`0x1f5ee5`), the order of this function. The source writes them the other way round (lines 753-758) |
| `0x26d100` | `long function_26d100(vector3f const *up, s_collision_result_1697c0 *collision, long *unknown, point3f const *point)` | `src/stubs/lane_c.cpp`, `include/unknown_0259a0.h` | The count agrees. `up` gets a node axis or a point here, and `unknown` is NULL |
| `0x26fc80` | `bool function_26fc80(long actor_index, long object_index, real distance, void *path, point3f *point)` | `src/path_transitions.cpp` | The count agrees (`eax`, `ebx` and three on the stack). `path` receives a marker buffer, which its body fills as an `s_object_marker` (lines 241-243) |

So this function's slots can take `function_1f7c70`'s names, direction_out,
type_out and movement_out, and `arg2` can take `function_1f5ab0`'s input.
`function_1f5ab0` calls the `arg7` slot blocked, although here it marks the
point as reached; `arg6` has no name in the source (inferred).

## Evidence

- The function and its callees were disassembled from the retail XBE with
  capstone 5.0.9, through `tools/xbe.py`. Conventions, stack slots, offsets,
  flags and constants were read from retail code and data. A stack-depth
  tracker gave every one of the 310 blocks one depth and resolved every `esp`
  operand, and a search of the control-flow graph found no back edge. A reader
  analysed the function, and a verifier checked each claim with its own tools:
  29 confirmed and 5 corrected, with the corrections applied here. A third
  agent wrote this text from that work, and a fourth checked the finished
  document against retail and the repository: 318 claims confirmed and 10
  corrected or reworded, with the changes applied here.
- Callers come from an image-wide scan for calls, jumps and absolute pointers:
  `0x1f38c0` (`0x1f3b8e`) is the only reference to `0x1f6090`. The same scan
  finds `0x150400` as the only other caller of `0xe64e0`.
- Constants are read from retail `.rdata`: `0x45dbc0` (1.0), `0x45dfd8`
  (0.8660254), `0x45dbdc` (0.0001), `0x45dc1c` (0.05), `0x45dc5c` (0.2),
  `0x45dc0c` (2.0), `0x45dc9c` (0.98), `0x44ae88` (0.8), `0x45e2fc` (1.6),
  `0x45dc88` (1.25), `0x45debc` (-0.2), `0x45e0dc` (0.99), `0x45ddac` (0.95),
  `0x43ff08` (1.5), `0x45dc2c` (0.75), `0x45dbd8` (0.0), `0x45dbcc` (-1.0),
  `0x45dbbc` (0.5), and the double 0.95 at `0x45e5c0`. Through pointers in
  `.data`: `g_4687a4` (0, 0, 0), `g_4687a8` (1, 0, 0), `g_468778` (1, 0) and
  `g_4687b0` (0, 0, 1). The trace distances 0.5 and 0.4 are pushed as
  immediates (`0x1f6b07`, `0x1f6dd8`).
- Source line numbers are at `9feed5f`.
- No SDK or outside dataset was used. Names are the repository's own, or
  describe behaviour.
