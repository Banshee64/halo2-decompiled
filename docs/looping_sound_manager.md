# Looping sound manager recovery

Retail range claimed: `0x2198f0`–`0x21d10f`.
All 44 inventory entries in this range are implemented. The range is inferred
from the code's shared data and neighboring module initialization.

## Evidence for the range

- `0x21a0c0` allocates 128 entries of size `0xd4`, named "looping sounds",
  and stores the array in `g_502110`.
- `0x219b40` allocates 64 controller entries of size `0x1c` in `g_51ebd8`
  and a fallback controller in `g_51ebdc`.
- The next group (`0x21d110` onward) uses `g_51ebe0`; its initializer at
  `0x21d490` allocates 16 entries of size `0x14`, named "sounds effects".

## Function map

Names inferred from retail behavior remain provisional.

| Retail address | Interpretation |
| --- | --- |
| `0x2198f0` | Controller lookup, with fallback for `NONE` |
| `0x219910` | Controller lookup with datum validation |
| `0x219960` | Find existing controller by definition index |
| `0x219a30` | Find controller and add a reference |
| `0x219a90` | Find or create controller and add a reference |
| `0x219b40` | Initialize controllers and fallback controller |
| `0x219c30` | Refresh controller source |
| `0x219c80` | Release controller reference |
| `0x219cc0` | Advance controller random seeds |
| `0x219d90` | Process synchronized controller playback |
| `0x219e90` | `c_looping_sound_controller::initialize` |
| `0x219ed0` | `c_looping_sound_controller::update_controller_source` |
| `0x219f60` | `c_looping_sound_controller::add_synch_count` |
| `0x219f80` | Pitch attenuation outside the natural range |
| `0x21a050` | Test whether the next track state has a sound |
| `0x21a080` | Test whether a track transition requires stopping |
| `0x21a0c0` | Initialize looping sounds |
| `0x21a100` | Update spatial locations for looping sounds |
| `0x21a1e0` | Dispose looping sounds from the old map |
| `0x21a250` | `sound_refresh_looping` |
| `0x21ac30` | Notify track completion |
| `0x21acc0` | Spatialize track listener gains |
| `0x21adb0` | Unlink a stopped track sound |
| `0x21aeb0` | Allocate looping sound, detail timers, and track seeds |
| `0x21b070` | Delete looping sound and release its controller |
| `0x21b0e0` | Process looping sounds |
| `0x21b870` | Find playing sound in a looping track |
| `0x21b910` | `looping_sound_find_or_create_sound` |
| `0x21b940` | Create a sound for a looping track |
| `0x21bc80` | Insert a playing sound into a track list |
| `0x21bd00` | Fade out sounds in a track list |
| `0x21be20` | Fade in playing loop sounds |
| `0x21bf00` | Track sound source callback |
| `0x21bfb0` | Impulse sound source callback |
| `0x21c140` | Compare source positions within tolerance |
| `0x21c190` | `sound_commit_looping_definition` |
| `0x21c400` | Update channel for looping sound |
| `0x21cb90` | Get track or shared permutation mask |
| `0x21cbe0` | Store permutation mask and previous permutation |
| `0x21cc30` | Get previous shared permutation |
| `0x21cc60` | Prepare a detail-sound playback request |
| `0x21ce60` | Detail sound random offset |
| `0x21cf80` | Find looping sound by definition and identifier |
| `0x21d060` | Retail debug-rendering entry |

## Recovered storage

The controller has a datum salt at `+0`, seven reference-count bits and a
source-update flag at `+2`, seven synchronization-count bits and a playback
flag at `+3`, definition index at `+4`, random seed at `+8`, and four
per-listener values at `+0xc`.

The three manager globals already have definitions in `src/sound_manager.cpp`.
Reuse those definitions when adding types; do not allocate duplicate globals.

## Current validation

Fresh full `tools/check.py` against base `330e1e2`: 3,085 game functions
matched (3,086 across all functions in scope), up by twenty-four from upstream.
No upstream matches or matches from earlier local batches were lost.
All 44 functions in the claimed range are implemented: 24 match exactly and
20 retain byte differences. The inventory confirms 14,064 retail bytes and
no missing entries in the range.

Final review also checked 58 structure size/offset assertions with the original
compiler. The three state/reference tables, both eight-slot callback tables,
and the zero-gain constant agree with retail data. All sixteen dependency
stubs are absent from the tested upstream base.

Exact matches:

- `0x2198f0`, `0x219910`, `0x219960`, `0x219a30`, `0x219a90`, `0x219c80`
- `0x219cc0`, `0x219d90`, `0x219e90`, `0x219f60`, `0x219f80`, `0x21a050`
- `0x21a080`, `0x21a0c0`, `0x21a1e0`, `0x21adb0`, `0x21b070`, `0x21b910`
- `0x21bc80`, `0x21bfb0`, `0x21c140`, `0x21cbe0`, `0x21cc30`, `0x21cf80`

Remaining differences:

- `0x21a250`: refresh, 2,568 bytes versus 2,518. Stack/register allocation,
  Boolean comparisons, branch placement, and existing stub conventions differ.
  Its spatialization helpers retain calls as in retail.

- `0x21c400`: channel update, 1,983 bytes versus 1,929. Stack/register
  allocation and external stub conventions differ; upstream helpers
  `function_221810`, `function_219290`, and `sound_voice_mark_channel`
  also inline at sites where retail calls them.
- `0x21b0e0`: playback processing, 1,877 bytes versus 1,923. Stack layout,
  register allocation, floating-point operand order, and the external stub
  conventions differ. The detail timer uses x87 multiplication and integer
  addition followed by the CRT conversion, as retail does.
- `0x219b40`: controller pool initialization, 237 bytes versus retail's 230;
  phase-bit handling and store order differ. The retail allocation label is
  "sound playback controllers".
- `0x219ed0`: source refresh, 176 bytes versus 134. The stub at `0x12a9d0`
  cannot reproduce the retail register convention; upstream's
  `players_next_active_local_player` also gets inlined where retail calls it.
- `0x219c30`: refresh wrapper, 79 bytes in both versions, with register
  differences around its call to `0x219ed0`.
- `0x21a100`: spatial-location update, 281 bytes versus 217. Upstream's
  `function_11bed0` gets inlined where retail calls it.

- `0x21aeb0`: allocation, 443 bytes versus 435. The initialization body
  matches; the compiler emits a separate return path for failed allocation.
- `0x21b870`: track sound lookup, 148 bytes in both versions. Register
  allocation and addressing of the list-link substructure differ.
- `0x21bf00`: source callback, 149 bytes versus 161. Its table now preserves
  the standard convention, but the compiler removes the fallback lookup.
- `0x21ac30`: completion notification, 143 bytes versus 142. The address
  used for the completion counter differs by its offset in the track.
- `0x21acc0`: spatialization, 358 bytes versus 225. Upstream's
  `function_2197f0` inlines, and the `0x1251e0` stub uses a standard convention.
- `0x21cb90`: permutation-mask lookup, 79 bytes in both versions;
  register choices differ.
- `0x21b940`: sound creation, 836 bytes versus 832. Remaining differences
  involve stack slots, register choices, and the stub at `0x218f50`.

- `0x21bd00`: fade-out, 253 bytes versus 286. The `0x12a810` stub uses a
  standard convention; iteration and store scheduling also differ.
- `0x21be20`: fade-in, 202 bytes versus 210. The same stub and caller-driven
  register convention affect this function.
- `0x21ce60`: detail offset, 260 bytes versus 278. The floating-point stack
  uses `faddp` where retail retains extra exchange and pop instructions.
- `0x21cc60`: detail request, 534 bytes versus 508. Upstream's first-active-
  player helper inlines, with additional register and scheduling differences.
- `0x21d060`: debug entry, 199 bytes versus 162. Upstream's minimum-distance
  helper inlines, and the unused maximum-distance call is removed.

- `0x21c190`: track transition commit, 623 bytes versus 616. The remaining
  differences are register choices, mask addressing, and calling conventions
  of stubs `0x218f50`, `0x128500`, `0x128a60`, and `0x127320`.

The callers use the existing upstream definitions and flags. Resolving those
inlining differences requires coordination with the owners of the helpers.

## Recovery notes

The initial source batch uses `/O2 /Ob1 /arch:SSE /Gr`. Controller lookup,
datum validation, reference release, seed updates, initialization, and looping
sound allocation reuse the existing data-array helpers. Reference release is
explicitly inline because retail expands it inside looping-sound deletion.
Controller initialization clears the listener values with `memset`.

The cleanup entry at `0x21a1e0` keeps upstream's `function_21a1e0(void)`
declaration. Its implementation replaces the stub in `src/stubs/simulation.cpp`;
the caller and shared header do not need changes.

The second batch adds controller creation, pool initialization, source
refresh, synchronized playback, and spatial-location updates. The sound datum
now includes its `s_sound_location` at `+0xc`; its controller index remains at
`+0x54`, and its total size remains `0xd4`.

New stubs in `src/stubs/looping_sound_manager.cpp` cover `0x12a9d0`
(per-listener Doppler/pitch calculation, owned by lane L) and `0x21f430`
(synchronized channel playback). No shared headers were changed. Controller
creation reuses upstream's `random_seed_generate` and the refresh loop reuses
its active-local-player iterators.

The third batch recovers detail timers at datum `+0x58` and track storage at
`+0x88`, with `0x14` bytes per track. Playing sounds have a `0xbc` stride;
their loop-state substructure starts at `+0x5c`, with list links at `+0x6c`
and `+0x70`. Giving those links their own structure reproduces list insertion.
The transition test expands an inline predicate that returns at each branch.

The fourth batch defines the complete retail callback tables at `0x44a110`
and `0x44a130`. The position-comparison callback now matches. Their always-true
comparison callback folds to the same address as the existing object-type
method corresponding to `0xa5750`, verified in the link map; no duplicate
retail marker is added.

The sound datum has four listener gains at `+0xc4`; the track area ends there.
A playing sound's source is at `+0x18`, controller index at `+0x5c`, track
index at `+0x60`, and synchronization state at `+0x62`. Source flag copies
use a local 16-bit view to reproduce the byte loads and stores in retail,
without changing the shared source header.

Sound creation draws a seed, selects a listener, initializes playing-sound
state, chooses pitch range and permutation, then links the sound into its
track. The pitch-range parameter is a `long`, as shown by retail's full-width
loads; correcting it makes the find-or-create wrapper match.

Additional stubs cover `0x1251e0` (gain calculation), `0x127d00` (listener
selection), `0x125e60` (playback data preparation), and `0x218f50` (pitch-range
selection). Existing upstream permutation helpers are called with their
original declarations. No shared headers or upstream compilation flags changed.

The fifth batch identifies fade state at playing-sound offsets `+0xae`
(curve), `+0xb0` (gain bits), and `+0xb4`/`+0xb8` (start/end times).
Durations are converted from seconds to milliseconds. Detail requests copy
the looping source, then place a random offset either relative to that source
or around an active local listener. Random draws occur in distance, pitch,
yaw order. The retail debug entry repeatedly tests the first track; no
track-pointer advance survives in its disassembly.

Two additional stubs cover `0x12a810` (current fade gain) and `0x2197b0`
(gain-curve evaluation). No shared headers changed. The gain constant
`g_44a0f0` was verified as zero in retail data; the debug flag is `g_4e636d`.

Track transition commit selects the next definition and permutation, releases
its previous playback reference, and checks source and definition voice limits.
The previous-permutation lookup uses the old pitch range. The track always
advances to its next state; the return value says whether this sound was stopped.
Retail expands the permutation helpers here and calls them from sound creation.
The local voice-count view has two `0x26`-byte groups, each with a count,
sixteen voice indices, and a limit. Voice entries have a `0x24`-byte stride.

New stubs cover `0x128500` (voice counts) and `0x128a60` (voice selection).
The transition uses upstream's `sound_playback_release_reference` and its
existing `function_127320` stub with their original declarations. The voice
array `g_4e6378` remains owned by `sound_manager.cpp`.

Playback processing removes stale sources, smooths track gain using the sound
class rate and elapsed time, updates pitch ranges, and schedules detail sounds.
Blended playback tracks up to nine pitch ranges; ordinary changes crossfade
over 0.75 seconds. Detail timers advance on successful playback or reason 1
from the playback request. Retail has a Boolean update argument. New stubs cover `0x125f70` (detail playback request) and
`0x126df0` (crossfade); no shared headers changed.

Channel update prepares pitch, gain, effect, and impulse properties, queues
chunks, and commits track transitions at the end of a permutation or at
a marked chunk boundary. It exits immediately if the transition stops the
playing sound. Only the track’s primary sound copies the shared seed and
permutation mask on completion. Retail has a third spatialization argument. The property block contains a `0x30`-byte base
and `0x448` bytes of effect and impulse state, initialized separately.

New stubs cover `0x12a1b0` (property setup), `0x21f8a0` (channel start),
`0x21f720` (impulse settings), and `0x21fa80` (channel properties). Existing
queue, permutation, promotion, and channel-marking helpers retain their
upstream declarations and compilation flags. No shared headers changed.

Refresh finds or creates the source, accumulates spatial gains for combined
sources, selects normal or alternate track transitions, and handles end
sounds and fades. A definition index of `NONE` returns true. A failed
allocation returns false; removing an already stopped source returns true,
while removing an inaudible source after track processing returns false.
The track reference slots and fade durations now have named fields. Refresh
reuses existing spatialization helpers and adds no stubs.

The implementation and review passes are complete. Twenty functions still
need matching improvements as dependencies become available. The inferred
range contains 44 inventory entries and 14,064 retail bytes.

## Sources

Addresses, storage layouts, and behavioral interpretations were checked
against retail disassembly.
