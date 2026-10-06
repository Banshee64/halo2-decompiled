# Object lifecycle analysis (unknown_0b67c0)

Retail range covered: `0xb67c0`–`0xb773f`. These are 18 inventory entries,
3,812 retail bytes: the object system's entry in the lifecycle table (create,
dispose, map and structure bsp changes, cluster activation), the three passes
the game tick runs over the objects, and the helpers that activate,
deactivate, move and reset an object. **Analysis only:** this document adds no
source, and nothing in it has been built or checked against retail with the
original compiler. Names are provisional. The `function_<va>` form is primary;
the descriptions are offered for whoever decompiles the range.

It precedes [part 1](unknown_0b7740.md) of the object core, and uses the names
of that document, [part 2](unknown_0bc190.md) and the
[lights and liquids](unknown_0bffa0.md) document for the object header, the
object fields and the per-tick functions. It is written for lane AB, whose
claim covers the range.

## Boundary

- `0xb67a0`, just before the range, is matched in `src/unknown_0b66c0.cpp`.
  `0xb7740`, just after it, opens part 1's range and has source in
  `src/unknown_0b7740.cpp` (`todo`). Both are excluded.
- Lane AB's row of the Active claims table (issue #9) covers the range. This
  document makes no claim of its own.
- At `3ea6394`, five entries are matched: `0xb68c0` and `0xb6ab0` in
  `src/unknown_0b68c0.cpp`, `0xb7290` and `0xb7680` in
  `src/unknown_0b8ca0.cpp`, and `0xb7360` in `src/unknown_0b58c0.cpp`. Two more
  have source in `src/unknown_0b68c0.cpp` and are `todo`: `0xb69d0` and
  `0xb6d60`. The other eleven are `todo` and have no source; three of them have
  `@stub` definitions, see [Existing declarations](#existing-declarations).
- Seven entries have no direct callers. They are slots of the lifecycle
  table, see [The lifecycle table](#the-lifecycle-table).

## Conventions

"Callers" counts direct calls from outside the range, then calls from inside
it. Calls through pointers are not counted. Results are in `eax` (`al` for a
bool). "Object" means an object index.

| Retail | Bytes | Arguments | Returns | Callers | What it does |
| --- | --- | --- | --- | --- | --- |
| `0xb67c0` | 250 | none | | 0 | Creates the object data arrays, memory, globals and partitions (lifecycle slot 0) |
| `0xb68c0` | 269 | none | | 0 | Matched. Disposes of them (slot 1) |
| `0xb69d0` | 215 | none | | 0 | Empties the objects and partitions for a new map (slot 2) |
| `0xb6ab0` | 251 | none | | 0 | Matched. Frees every object when the map is unloaded (slot 3) |
| `0xb6bb0` | 147 | none | | 0 | Connects the objects to a new structure bsp (slot 4) |
| `0xb6c50` | 260 | none | | 0 | Disconnects them from the old structure bsp (slot 5) |
| `0xb6d60` | 135 | `eax` object; stack: cluster bit vector (`ret 4`) | bool | 1 + 1 | Whether an object belongs to the set clusters |
| `0xb6df0` | 281 | stack: previous vector, current vector, count (`ret 0xc`) | | 0 | Activates and deactivates objects by cluster (slot 8) |
| `0xb6f10` | 330 | none | | 1 | The tick's update pass |
| `0xb7060` | 228 | none | | 1 | The tick's post-physics pass |
| `0xb7150` | 319 | none | | 1 | The tick's last pass: deferred updates, deletions, garbage collection |
| `0xb7290` | 101 | `edi` object (`ret`) | | 7 + 1 | Matched. Activates an object |
| `0xb7300` | 88 | `edx` object (`ret`) | | 7 + 1 | Deactivates an object |
| `0xb7360` | 76 | `eax` object (`ret`) | | 75 + 1 | Matched. Sets header flag `0x02` on an object and its ancestors |
| `0xb73b0` | 116 | `eax` object (`ret`) | | 3 | Resets an object after it is teleported |
| `0xb7430` | 360 | stack: nine arguments (`ret 0x24`) | bool | 5 + 1 | Moves and orients an object |
| `0xb75a0` | 209 | `eax` position, `ecx` forward, `edx` up; stack: object, location, flag (`ret 0xc`) | | 40 | Places an object, deleting it when the position is invalid |
| `0xb7680` | 177 | `eax` object, `xmm3`; stack: one argument (`ret 4`) | | 4 | Matched. Changes an object's scale |

## Data

### The lifecycle table

The table at `0x440dd8` has 68 entries of 0x24 bytes, nine function slots
each, and ends at `0x441767`; no source declares it yet. Each slot is run for
every entry by one function:

| Slot | Run by | When |
| --- | --- | --- |
| 0 | `0x137c20` | at startup, entry 0 to 67, without a NULL test |
| 1 | `0x12b690` | at shutdown, entry 67 down to 0, without a NULL test |
| 2 | `0x137ca0` | for a new map |
| 3 | `0x137d00` | when the map is unloaded, in reverse |
| 4 | `0x137d40` | for a new structure bsp |
| 5 | `0x137da0` | when the structure bsp is unloaded, in reverse |
| 6 | `0x11c1b0` | at a structure bsp switch, with the new bsp index |
| 7 | `0x138c10` | when the vector at `g_4e6948` `+0x1138` changes |
| 8 | `0x138c10` | when the vector at `g_4e6948` `+0x11b8` changes |

`0x138c10` is called by the game tick, `0x137fe0`. For slots 7 and 8 it
passes a copy of the previous vector, the current vector, and a count from
`g_4e0348`.

The object system is entry 27, at `0x4411a4`: `0xb67c0`, `0xb68c0`, `0xb69d0`,
`0xb6ab0`, `0xb6bb0`, `0xb6c50`, two NULL slots, and `0xb6df0`, the only
function in slot 8 of any entry. Entry 26 holds `0x175f40` in its first two
slots, and entry 28 holds `0x109300`–`0x109380`, matched in
`src/unknown_108fd0.cpp`.

### Object globals

`0xb67c0` takes 0x84 bytes of game state for the object globals at
`g_4de2f4`, zeroed. The fields this range uses:

| Offset | Meaning |
| --- | --- |
| `+0x0` | byte: active. `0xb6bb0` sets it and `0xb6c50` clears it; `function_b8820` tests it (part 1) |
| `+0x3` | byte: set to 1 while one of the three tick passes runs, cleared on every exit; no reader was found |
| `+0x4` | 16 bits: the active objects with object flag bit 14 (`0xb7290` counts up, `0xb7300` down) |
| `+0x8` | NONE after `0xb69d0` |
| `+0xc`, `+0x10` | reset by `0xb69d0` |
| `+0x14` | a time, reset by `0xb69d0` (part 2's `0xbca70`) |
| `+0x18` | the unique id counter (part 1) |
| `+0x80`, `+0x81` | bytes reset by `0xb69d0` |

It also takes 0xa00 bytes at `g_4de2d0`: the object for each of 640 names,
which part 2's `0xbf050` records and `0xb69d0` fills with NONE.

### Header flags

The flags at `+0x2` of an object's header, as part 1 and part 2 describe them,
and how this range uses them:

| Flag | Here |
| --- | --- |
| `0x01` | active. `0xb7290` sets it; `0xb7300` clears it with `0x04` |
| `0x02` | `function_b7360` sets it on an object and its ancestors. The update passes require it with `0x01` |
| `0x04` | woken during this tick (`function_bba20`). `0xb7060` runs the post-physics pass only on such objects |
| `0x08` | update deferred: set at creation and by `0x109580`. `0xb7150` clears it and runs the update |
| `0x10` | deletion requested (`0xb8540`). `0xb7150` frees such objects |
| `0x20` | `0xb93b0` sets it when it attaches an object to a parent (`0xb97df`); `0xb7150` clears it on every object |
| `0x40` | connected to the map (`0xb8600`, `0xb87b0`) |
| `0x80` | has a parent: `0xb93b0` sets it (`0xb9532`) and `function_b9890` clears it |

## The functions

### `0xb67c0` and `0xb68c0`: create and dispose

`0xb67c0` runs at startup. It calls `function_d4830`, `function_1088e0` and
the lights' `0xbffa0` first, then creates:

- the object headers, `g_4e0300 = data_new("object", 0x800, 0xc, 0,
  g_510c2c)`: 2048 headers of 12 bytes (the retail string "object" at
  `0x45322c`);
- the object memory, `g_4de2ec = game_state_loop_allocator_new(0x100000,
  "objects")`, and clears `g_4de2f0`;
- the object globals, 0x84 bytes of game state, zeroed, at `g_4de2f4`;
- zeroes the 0x2008 bytes from `g_4de2f8` up to `0x4e0300`, which hold
  `g_4de2fc` and part 2's stamps at `g_4de300`;
- 0xa00 bytes of game state for the names at `g_4de2d0`;
- the two cluster partitions of part 1:
  `0x1cabc0(&g_4de2e0, "collideable object", 0x14)` and
  `0x1cabc0(&g_4de2d4, "noncollideable object", 0x14)`.

The game state blocks come from an inlined `function_123d40`, as part 3's
light globals do. `function_b68c0`, matched, releases all of
it in reverse. Its branch that frees the object memory and calls
`data_dispose` depends on `g_4de2f0`, which is only ever cleared, so it never
runs in retail.

### `0xb69d0` and `0xb6ab0`: a new map and the old one

`0xb69d0` calls `function_d4890`, then the `+0x18` slot of each object type
definition, then the lights' `0xc0040`. It makes `g_4e0300` valid and empty,
fills the 640 names at `g_4de2d0` with NONE, empties both partitions, and
resets the globals listed above. The source in `src/unknown_0b68c0.cpp`
agrees with retail step for step.

`function_b6ab0`, matched, runs the callbacks registered at `g_4e0330`, makes
the lights and the light partition's arrays invalid, and frees every object.

### `0xb6bb0` and `0xb6c50`: structure bsps

`0xb6bb0` runs for a new structure bsp. It sets the globals' active byte, then
for every object without a parent and without object flag bit 7 calls part
2's `0xbf510(object)` and then `0xb8600(object, &location)`. It calls
`function_108960` (type handler slot `+0x20`) on every object, then
`function_bfcc0` and the lights' `0xc00a0`.

Unlike step 19 of part 1's `0xb7b40`, it passes the location even when
`0xbf510` found no cluster. And because `0x137d40` has just zeroed the vector
at `g_4e6948` `+0x11b8`, the `0xb6d60` test inside `0xb8600` sees no cluster
yet: objects are activated by cluster later, through slot 8.

`0xb6c50` runs before the structure bsp is unloaded. It calls the lights'
`0xc0110` first. Then, for every object, it deactivates it (an inlined
`0xb7300`), disconnects it with `0xb87b0` when it is connected, calls
`function_1089d0` (type handler slot `+0x24`), and sets the header's cluster
and the location's leaf and cluster to NONE and its bsp to `g_4686c4`, which
still names the old bsp at that point. Last, it clears the active byte.

### `0xb6d60` and `0xb6df0`: activation by cluster

`0xb6d60(object, clusters)` returns true when object flag bit 1 is set. For a
machine it asks type handler slot `+0x48` through `function_108d30` (the
machine's `function_115a40`) and returns its answer. Otherwise it returns the
cluster bit vector's bit for the header's cluster, or false when the cluster
is NONE. Its callers are `0xb6df0` and part 1's `0xb8600`. The source in
`src/unknown_0b68c0.cpp` agrees with retail.

`0xb6df0` is slot 8. Of its three arguments it uses only the second, the
current vector at `g_4e6948` `+0x11b8`. It walks every object (an inlined
`function_16bc00`) and looks only at connected ones (header flag `0x40`):

- an active object outside the set clusters is left alone when the game
  options' `mode` (`g_4e6948`, `+0xc`) is 4 and the object has a simulation
  entity (`+0xd4`). Otherwise it is deleted with `0xb8540` when it has object
  flag bit 17, or deactivated with `0xb7300` and
  `havok_component_contacts_mark2`;
- an inactive object inside them is activated with `function_b7290` and
  `havok_component_contacts_mark1`.

Its own test of object flag bit 1 can never succeed, because `0xb6d60`
already returns true for such objects.

### `0xb6f10`, `0xb7060` and `0xb7150`: the tick's passes

The game tick, `0x137fe0`, calls `0xb6f10` (`0x138050`), `0x1c3d30`
(`0x138055`), `0xb7060` (`0x13805a`) and `0xb7150` (`0x13805f`), in that order
and unconditionally. `0x1c3d30` runs on the havok world (`g_51e9a4`). Each pass
sets the globals' byte at `+0x3` on entry and clears it on every exit.

`0xb6f10` updates the objects:

1. First the objects in the list at `g_5107f0` (`object_indices` at `+0x1d00`,
   count at `+0x1d80`). Each with header flags `0x01` and `0x02` and without
   `0x08` gets part 2's `0xbc470` (`0xb6f69`). If it is then woken (`0x04`)
   and still lacks `0x08`, `0x109580` (`0xb6f7a`) finds the objects it
   carries with `function_bb050` and, for each, sets object `+0xc0` bit 2,
   stores the carrier at `+0xb8` (`platform_index`) and calls
   `function_b7360`; items and projectiles (type mask `0x3c`) also get
   `function_bba20` and header flag `0x08`.
2. Then every other object (an inlined `record_pool_iterator_step`):
   `0xbc470` on each with `0x01` and `0x02`, without `0x08`, and not in the
   list (object `+0xc0` bit 3, `0xb701e`). Active objects have no parent, so
   this reaches root objects; `0xbc470` recurses into children.

`0xb7060` is the post-physics pass. It tests only `0x04` and the absence of
`0x08`. For the listed objects it calls part 2's `0xbc820` (`0xb70a9`), then
`0x109660` with the object in `eax` and its list index in `ecx`, which records
the object's motion from its havok rigid body in the list entry. Then it
calls `0xbc820` on every other woken object (`0xb713d`), and last `0xfd910`
(`0xb70f1`), which fires the weapon barrels marked as pending.

`0xb7150` ends the tick:

1. On every object it clears header flag `0x20` (`0xb7214`). An object with
   `0x08`, `0x01` and `0x02` and without `0x10` loses `0x08` (`0xb7235`) and
   gets `0xbc470`, then `0xbc820` when it is woken: the updates deferred from
   creation and from `0x109580` run here, after the havok step.
2. `0xfd910` (`0xb719b`).
3. Every object with `0x10` is freed, with part 1's `0xb83b0(object, 1)` and
   `0xb8460(object, 1)` (`0xb727d`, `0xb7285`). This is where the deletion
   that `0xb8540` requests happens.
4. It clears the globals' `+0x3` and jumps to part 2's garbage collection,
   `0xbf380` (`0xb71ea`).

### `0xb7290` and `0xb7300`: activate and deactivate

`function_b7290`, matched, sets `0x01` on an object that has a cluster and no
parent, and counts object flag bit 14 in the globals' `+0x4`. `0xb7300` is its
counterpart, with neither test. With the object in `edx`, it returns unless
`0x01` is set, and clears `0x01` and `0x04` (`0xb731e`). For the havok types
(type mask `0x1883`) it calls `0x1c38a0` (`0xb733c`), the mirror of
`function_1c3850`. With object flag bit 14 it decrements the count at `+0x4`
(`0xb7353`). `0xb6c50` has an inlined copy.

### `0xb7360` and `0xb73b0`

`function_b7360`, matched, sets header flag `0x02` on an object and its
ancestors (part 1).

`0xb73b0`, with the object in `eax`, resets an object after it is teleported:
it calls part 1's `0xb77d0` with the zero vector `*g_4687a4` as both
velocities, `function_b9b90(object, false)`, the detach sequence of part 1's
`0xb8460` (`function_146bf0`, `havok_object_detach`, `0x278f00`), type
handler slot `0x78` through `function_1091b0` (`0xb7408`), and then
`function_1c35f0` between `function_146bf0` and `0x278f00` to rebuild the
havok component. It ends with a jump to `function_146bf0` (`0xb741f`). Two of
its callers, `0x15e360` and `function_2773d0`, call it right after `0xb75a0`.

### `0xb7430` and `0xb75a0`: placing an object

`0xb7430` takes nine arguments on the stack, in order: the object, position,
forward, up, location, whether to rebuild the matrices, whether to update the
havok component, a flag passed on to `0x1c4a80` (0 at every call site), and
whether to stay connected. It returns a bool:

1. A connected object is disconnected (`0xb87b0`) unless the last argument is
   set.
2. For an object without a parent, each coordinate of the position must lie
   within -32768 and 32768 (`0x45e4ac`, `0x45dffc`); a NaN fails too. If one
   does not, the result is false, and only the store of the position at
   `+0x64` is skipped, with its `function_b58c0` call (mask 2) for the
   object's simulation entity (`+0xd4`).
3. Forward and up go to `+0x70` and `+0x7c`, followed by `function_b58c0`
   with mask 4.
4. With the havok argument, `0x1c4a80` with the object in `eax`.
5. With the matrices argument, part 2's `0xbd020`.
6. An object that was disconnected in step 1 is connected again with
   `0xb8600(object, location)`.

Part 2's `0xbc5e0` calls it as `0xb7430(object, &position, forward, up, 0, 0,
0, 0, 1)`. Its other callers are `0xb75a0`, `0xb8ee0`, `0xb92d0`, `0xb93b0`
and `0x275380`.

`0xb75a0` takes the position, forward and up pointers in `eax`, `ecx` and
`edx`, and the object, location and a flag on the stack. It copies each
vector that is not NULL, lets type handler slot `0x68` adjust the copies
through `function_109290` (only the biped fills that slot, with
`function_dffa0`, which keeps up vertical and flattens forward), and calls
`0xb7430(object, position, forward, up, location, 1, 1, flag, 0)`. When that
fails it requests the object's deletion with `0xb8540`, unless the game
options' `mode` is 4 and the object has a simulation entity. It always ends
with `function_b7360`. Because the handler works on copies, a caller may pass
the object's own fields.

It has 44 call sites in 40 functions, all with the flag false: 18 pass only a
position, 7 only an orientation, 17 all three, and 2 vary. Seven pass a
location.

### `0xb7680`: scale

`function_b7680(object, scale, seconds)`, matched, changes an object's scale
(part 1).

## Existing declarations

At `3ea6394`:

| Retail | Declared as | Where | What retail shows |
| --- | --- | --- | --- |
| `0xb69d0` | `void function_b69d0(void)` | `src/unknown_0b68c0.cpp` | Agrees; the source is `todo` |
| `0xb6d60` | `bool function_b6d60(long object_index, dword const *clusters)` | `src/unknown_0b68c0.cpp` | Roles agree. The object is in `eax` and the vector on the stack (`ret 4`); the source is `todo` |
| `0xb7300` | `void function_b7300(long object_index)` | `src/stubs/unknown_0b7300.cpp`, `src/items.cpp`, `src/unknown_0b7300.cpp` | Roles agree. The object is in `edx` |
| `0xb73b0` | `void function_b73b0(long object_index)` | `src/stubs/lane_a.cpp`, `src/unknown_272b70.cpp` | Roles agree. The object is in `eax` |
| `0xb75a0` | `void function_b75a0(long object_index, point3f const *point, vector3f const *forward, vector3f const *up, s_location const *location, bool unknown)` | `src/bipeds.cpp`, `src/devices.cpp`, `src/projectiles.cpp`, `src/unit_object_type.cpp`, `src/unknown_109a00.cpp`, `src/unknown_272b70.cpp`, `include/object_default_placement.h` | Roles agree. `point`, `forward` and `up` are in `eax`, `ecx` and `edx`; `unknown` is false at every call site. The stub in `src/stubs/projectiles.cpp` spells the vectors `union vector3f` |

Seven of the functions take register arguments (the table under
[Conventions](#conventions)). Nine take none, and `0xb6df0` and `0xb7430`
take all their arguments on the stack.

## Evidence

- Every entry was disassembled from the retail XBE with `tools/disasm.py` at
  `3ea6394` (capstone 5.0.9). Conventions, offsets, flags, callers and the
  lifecycle table were read from retail code and data.
- Constants are read from retail `.rdata`: `0x45e4ac` is -32768.0 and
  `0x45dffc` is 32768.0. "object", "objects", "collideable object" and
  "noncollideable object" (`0x45322c`, `0x453224`, `0x453210`, `0x4531f8`)
  are retail strings.
- The matched entries and the work-in-progress source for `0xb69d0` and
  `0xb6d60` were read from the repository and compared with retail.
- Callees outside the range are described from their source in the
  repository where it exists, named above, and otherwise from their
  disassembly, only as far as this document needs them.
- No emulator, runtime testing, SDK or outside dataset was used. Names are
  the repository's own, or describe behaviour.
