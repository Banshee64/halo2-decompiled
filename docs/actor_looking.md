# Actor looking recovery

Retail range claimed: `0x296580`–`0x298c2f` (`actor_looking.obj`).
This is an inferred module boundary: 17 inventory entries totaling 9,768
retail bytes. Names without a retail symbol remain provisional.

## Evidence

- Retail names `0x298bc0` as `looking_at_target` in `actor_looking.obj`.
- Both 2003 maps place `aiming_at_target` immediately before that function.
  Retail `0x298b60` tests an actor's aiming state and target; `0x298bc0`
  tests its looking state and falls back to the aiming predicate.
- The surrounding functions use actor array `g_4f55f0`, with stride `0x888`,
  and the same direction, target, and idle-timer fields. The profile map
  groups interest scoring, direction decoding, looking bounds, idle timers,
  random directions, and the main looking update in this object.
- `0x296580` updates an actor's aiming-dependent movement flag. The preceding
  `0x296520` traverses a two-dimensional tree and is excluded.
- The next function, `0x298c30`, uses object data and movement parameters;
  it and its caller at `0x298d90` are excluded. Recheck these inferred edges
  as the surrounding code is recovered.
- No inventory entry in this range has an upstream implementation at
  `80435cf`. The active claims and open PRs were checked before starting.

## Initial targets

| Retail address | Interpretation | Evidence |
| --- | --- | --- |
| `0x298b60` | `aiming_at_target` | Actor flag, aiming mode, and target comparison; older map order |
| `0x298bc0` | `looking_at_target` | Named retail anchor and call to aiming predicate |
| `0x297600` | `advance_idle_timers` | Decrement and clamp timers at actor `+0x698` / `+0x69c` |
| `0x297560` | Reset idle timers and directions | Timer and direction stores adjacent to timer advancement |

## Current recovery

All 17 claimed functions are implemented. The full XDK 5849 check
against `80435cf` reports **3,407 game matches / 3,408 total**, up three from
upstream, with no existing matches lost.

| Retail address | Function | Result |
| --- | --- | --- |
| `0x296580` | `actor_look_affect_movement` | 114 bytes versus 114; datum lookup registers and scheduling differ |
| `0x296600` | `actor_look_compute_prop_interest` | Exact match, 421 bytes |
| `0x2967b0` | `actor_look_decode_direction` | 1,380 bytes versus 1,444; register allocation, branch sharing, return layout, and dependency conventions differ |
| `0x296d60` | `actor_look_valid_aim_vector` | 276 bytes versus 253; upstream normalization inlines, with register and floating-point scheduling differences |
| `0x296e60` | `actor_look_find_random_vector` | 1,496 bytes versus 1,423; argument registers, stack copies, return branches, and floating-point scheduling differ |
| `0x2973f0` | `actor_get_looking_bounds` | 372 bytes versus 356; register allocation, store scheduling, and the existing tag-element helper convention differ |
| `0x297560` | `reset_idle_timers` (inferred name) | 150 bytes versus 150; register allocation and store scheduling differ |
| `0x297600` | `advance_idle_timers` | Exact match, 92 bytes |
| `0x297660` | `find_new_random_vector` | 1,017 bytes versus 1,004; registers, instruction scheduling, and helper calling conventions differ |
| `0x297a50` | `actor_look_direction_within_bounds` (inferred name) | 449 bytes versus 443; registers, floating-point operand order, and clamped-arccos helper convention differ |
| `0x297c10` | `idle_time_get` | 285 bytes versus 281; stack slots, registers, and instruction scheduling differ |
| `0x297d30` | `actor_look_select_attention_direction` (inferred name) | 897 bytes versus 924; registers, floating-point scheduling, stack copies, and path-trace stub convention differ |
| `0x2980d0` | `generate_idle_vector` | 486 bytes versus 536; registers, sign-test encoding, and decoder branch sharing differ |
| `0x2982f0` | `actor_look_can_select_direction` (inferred name) | Exact match, 121 bytes |
| `0x298370` | `actor_look_update` | 1,988 bytes versus 2,020; registers, stack copies, branch layout, upstream combat-helper inlining, and dependency conventions differ |
| `0x298b60` | `aiming_at_target` | Checker reports 84 bytes versus 84; argument registers and datum lookup scheduling differ |
| `0x298bc0` | `looking_at_target` | 102 bytes versus 102; register allocation and comparison operands differ |

The fourteen remaining differences are retained for later work as dependencies
are recovered. Seven dependency stubs in
`src/stubs/actor_looking.cpp` cover missing callees; their implementations
remain outside this claim:

| Address | Purpose inferred from retail calls |
| --- | --- |
| `0x50650` | Clamped arccos helper |
| `0x1e5160` | Character looking-properties lookup (lane C) |
| `0xcaf60` | Unit head position |
| `0x1e3b00` | Object-relative point adjustment |
| `0x1fc710` | Actor firing origin |
| `0x1ffbd0` | Weapon aiming calculation |
| `0x1ffe00` | Target-marker selection |

The new stub signatures are provisional. Optional parameters of `0x1e3b00`
and `0x1ffbd0` that this caller always passes as null remain opaque pointers.
Existing object, prop, path, math, and marker helpers are reused unchanged.
The branch is rebased onto the upstream batch at `80435cf`, including its
new actor-location helper `0x26c180`. The existing path-trace stub declaration
at `0x26c590` is reused as declared by upstream.
No shared headers or upstream flags changed.
`config/functions.csv` is regenerated locally and excluded from commits.

## Recovered behavior and layout

The local actor view has the retail stride of `0x888`. The shared actor
array and game-time globals retain their existing definitions. A compile-only
check with the original compiler verifies 111 sizes and field offsets for
the actor view, looking properties, object headers, seat data, random state,
collision result, path points, direction specifications, object markers,
location data, path-trace results, animation state, output fields, and turn requests.

- Aiming at the target requires the byte at `+0x6f8`, aiming mode at
  `+0x41c` of at least 2, and either direction type 2 or type 1 referring
  to the actor's target prop at `+0x338`.
- Looking mode 0 uses the aiming predicate only when the state at `+0x86`
  is at least 3. Looking mode 2 tests direction type and target at `+0x434`
  and `+0x438`. Other modes return false.
- Timer reset rounds the ticks-per-second value with the x87 conversion,
  narrows it to a signed short, and stores it in both integer timers at
  `+0x698` / `+0x69c`. Both idle direction types become 4, and both vectors
  copy the actor's forward vector at `+0x290`.
- Timer advancement subtracts one from each integer timer, converts to
  `real`, clamps to zero, and converts back to an integer. This conversion
  order is present in retail and is preserved.

- Direction validity accepts either a full 3D dot product or a horizontal
  comparison. The horizontal path normalizes each XY projection, requires
  both lengths to be positive, then compares the dot product to the supplied
  cosine. Retail's Boolean switch is absent from the older map signature.
- Interest scoring reads the actor's prop reference, perceived state, and
  optional tracking view. Retail's constants (including 1.8, 0.4, 0.6, and
  the seven-second threshold) were read from the XBE. Tracking-view byte
  `+0x3a` adds interest; byte `+0x38` scales it. The last switch has three
  explicit cases; other values retain the score.
- Looking bounds start with the character properties. A used object's tag
  element can override the aiming cosine; a seated actor instead uses the
  parent unit's seat yaw bounds. Movement mode 4 selects another pair of
  idle angles. A missing character-property block returns false without
  writing the output bounds.

- Idle duration selects one of four character-property ranges according to
  alert state and the alternate-range flag. The extended flag scales both
  endpoints by 1.5. A missing block defaults to three seconds in ticks;
  otherwise the random sample is converted to ticks with x87 rounding.
- Direction selection is suppressed by the flag at actor `+0x264`, unit
  type 0 in movement state 5, movement mode at least 4 with actor `+0x5d0`
  set, or the existing `0x110ab0` predicate.
- Random-vector generation draws yaw then pitch, rotates around a horizontal
  perpendicular axis then the vertical axis, and tries up to ten candidates.
  Collision checks use a three-unit ray. The first clear candidate wins;
  otherwise the greatest positive collision fraction supplies a fallback.
  Retail still consumes twenty random draws when collision testing is
  disabled, then returns false without writing the output. Both random
  routines use the deterministic state at `g_4e7408 + 0`, exposed by the
  existing `unknown0` field.

- Direction decoding handles movement (0), explicit prop reference (1),
  current target prop (2), world point (3), vector (4), attention point (5),
  and object (6). Movement can follow the next path segment when the stored
  direction is short. Prop handling distinguishes current and remembered
  positions, camera/head positions, and cached target markers. Aiming uses
  the weapon helper when available and falls back to a normalized direction
  from the firing origin if that helper fails. Optional point outputs are
  written only after success and only when both output pointers are present.
- The movement caller resets aiming mode for movement type 0 without an
  active path, leaving the previous movement-aiming flag untouched on that
  early return. Otherwise it decodes the aiming specification when aiming
  mode is at least 3 and stores success at actor `+0x5d1`, with direction at
  `+0x5f8`.

- Direction validation accepts the bypass flag at actor `+0x6d1`, missing
  character properties, failed decoding, and zero-length horizontal vectors.
  Otherwise it tests absolute yaw and pitch against the aiming or looking
  limits with a double-precision `0.01` tolerance.
- Random selection uses character aiming bounds or offsets around the current
  aiming/idle direction. Actor state `+0x270` above 1 scales applicable bounds
  by 0.6. Movement mode 4 chooses the moving angle pair. The local-offset path
  clips yaw and pitch windows before subtracting the current angles. Failed
  sampling writes direction type 4 and the actor's forward vector, returning
  false. The three mutable angular limits `g_55e5bc`, `g_55e5c8`, and
  `g_55e5cc` are 10°, 10°, and 5°. Their values were recovered from retail
  startup initializers and checked byte-for-byte against the built globals.

- Attention selection requires no associated object, a clear actor flag at
  `+0x229`, an unlocked direction, and a valid path location. It normalizes
  the horizontal forward vector and transforms it into path coordinates.
  A blocked 1.25-unit trace triggers eight candidates, rotated in 45° steps
  from the full actor forward vector. Five-unit traces score each candidate
  by clear distance and its forward dot product, clamped below at 0.6.
  The best positive score wins only when its forward dot product is below
  0.7; that vector is transformed back and stored as direction type 4.
  Trace decisions use the result's first byte, ignoring the helper's return.
- Idle-vector generation can select attention when the character-bounds flag
  and actor flags `+0x6d1` / `+0x40` allow it. Otherwise expired or invalid
  aiming directions are regenerated, with a random chance to reuse a valid
  looking direction whose timer exceeds one second. That random draw is
  consumed before the timer and bounds checks. Refreshing the aiming timer
  also refreshes the looking timer and copies the full direction specification.
  Looking directions can then refresh independently. Failed decoding uses
  the actor's forward vector, and the routine returns true.

- The main update advances idle timers, selects aiming and looking directions,
  and publishes reference, aiming, looking, and optional point outputs. Aiming
  selection prioritizes animation state 6's marker, the combat direction,
  temporary overrides, then the aiming-mode switch. Looking selection uses
  the aiming vector in state 6, then overrides, explicit directions, or idle
  selection. Invalid aiming falls back to the reference vector; invalid
  looking falls back to the actor's forward vector.
- State 7 explicit aiming uses a 45° cosine limit. When it exceeds that limit,
  the routine can request unit action `0x27` while retaining the aim. State 5
  can instead issue turn request `0x2d` when the animation side disagrees with
  the signed yaw and the angle exceeds 20°. The request carries a side,
  object position, and forward vector in the existing 32-byte request layout.
- The update decrements temporary-override and animation timers, flattens and
  normalizes the reference vector unless actor flag `+0x229` is set, and
  copies the final vectors to output fields. It sets or clears attention bit
  3, sets bit 15 only when a decoded target point exists, and writes output
  mode from alert state. Failure to obtain character bounds skips direction
  selection but preserves the later timer and output work, including the
  existing aiming flag.

The implementation pass covers the full claimed range. Fourteen functions
retain byte differences; those results are recorded above for later matching
work. No game runtime behavior has been tested.

## Sources

Function-name and object-name evidence comes from
[halo-symbol-atlas](https://github.com/tinkerer-red/halo-symbol-atlas)
(CC BY 4.0): retail SHA-256
`03215919bb7163259257d361f4c7bf802a7ab12aa85e2689436369b5c427935d`,
2003 profile map
`4f4f09b181eec4a434418b38efe581e75aaf3047c24add8a712751d6ae0d34d3`,
and 2003 debug map
`96ea21d862dfe6a0bebb23e1a4311202a6e18a79970189a4df320d4ededa439d`.
Layouts and behavioral interpretations are checked against retail code.
