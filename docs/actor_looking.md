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
  `330e1e2`. The active claims and open PRs were checked before starting.

## Initial targets

| Retail address | Interpretation | Evidence |
| --- | --- | --- |
| `0x298b60` | `aiming_at_target` | Actor flag, aiming mode, and target comparison; older map order |
| `0x298bc0` | `looking_at_target` | Named retail anchor and call to aiming predicate |
| `0x297600` | `advance_idle_timers` | Decrement and clamp timers at actor `+0x698` / `+0x69c` |
| `0x297560` | Reset idle timers and directions | Timer and direction stores adjacent to timer advancement |

## Current recovery

Four of the 17 claimed functions are implemented. The full XDK 5849 check
against `330e1e2` reports **3,062 game matches / 3,063 total**, up one from
upstream, with no existing matches lost.

| Retail address | Function | Result |
| --- | --- | --- |
| `0x297560` | `reset_idle_timers` (inferred name) | 150 bytes versus 150; register allocation and store scheduling differ |
| `0x297600` | `advance_idle_timers` | Exact match, 92 bytes |
| `0x298b60` | `aiming_at_target` | Checker reports 84 bytes versus 84; argument registers and datum lookup scheduling differ |
| `0x298bc0` | `looking_at_target` | 102 bytes versus 102; register allocation and comparison operands differ |

The three remaining differences are retained for later work as callers are
recovered. No shared headers, dependency stubs, or upstream flags changed.
`config/functions.csv` is regenerated locally and excluded from commits.

## Recovered behavior and layout

The local actor view has the retail stride of `0x888`. The shared actor
array and game-time globals retain their existing definitions. A compile-only
check with the original compiler verifies the size and all sixteen named
field offsets against retail accesses.

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

Thirteen entries remain unwritten. The next small candidate is the direction
validity test at `0x296d60`, followed by looking bounds and interest scoring.

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
