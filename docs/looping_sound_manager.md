# Looping sound manager recovery

Retail range claimed: `0x2198f0`–`0x21d10f` (`looping_sound_manager.obj`).
The boundaries are provisional pending recovery of the complete module.

## Evidence for the range

- The retail symbol atlas names `0x21b910`
  `looping_sound_find_or_create_sound` in `looping_sound_manager.obj`.
- The 2003 profile and debug maps put controller lookup, reference counting,
  initialization, sound creation, playback updates, and detail sounds in this
  object. Retail has additional helpers and a different function order.
- `0x21a0c0` allocates 128 entries of size `0xd4`, named "looping sounds",
  and stores the array in `g_502110`.
- `0x219b40` allocates 64 controller entries of size `0x1c` in `g_51ebd8`
  and a fallback controller in `g_51ebdc`.
- The next group (`0x21d110` onward) uses `g_51ebe0`; its initializer at
  `0x21d490` allocates 16 entries of size `0x14`, named "sounds effects".

## Initial function map

Names inferred from the older maps and retail behavior remain provisional
except for the named retail anchor.

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
| `0x21a0c0` | Initialize looping sounds |
| `0x21b070` | Delete looping sound and release its controller |
| `0x21b0e0` | Process looping sounds |
| `0x21b870` | Find playing sound in a looping track |
| `0x21b910` | `looping_sound_find_or_create_sound` (retail symbol) |
| `0x21b940` | Create a sound for a looping track |
| `0x21c400` | Update channel for looping sound |
| `0x21ce60` | Detail sound random offset |

## Recovered storage

The controller has a datum salt at `+0`, seven reference-count bits and a
source-update flag at `+2`, seven synchronization-count bits and a playback
flag at `+3`, definition index at `+4`, random seed at `+8`, and four
per-listener values at `+0xc`.

The three manager globals already have definitions in `src/sound_manager.cpp`.
Reuse those definitions when adding types; do not allocate duplicate globals.

## Sources

Function-name and object-name evidence comes from
[halo-symbol-atlas](https://github.com/tinkerer-red/halo-symbol-atlas)
(CC BY 4.0): retail SHA-256
`03215919bb7163259257d361f4c7bf802a7ab12aa85e2689436369b5c427935d`
and the 2003 profile map
`4f4f09b181eec4a434418b38efe581e75aaf3047c24add8a712751d6ae0d34d3`.
Addresses, storage layouts, and behavioral interpretations were checked
against retail disassembly.
