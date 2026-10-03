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

## Scope

The draft initially records the claim. Recovery will use the original SDK
and retail disassembly, preserve existing shared declarations, and record
exact matches and remaining differences after a full check. No shared
headers, dependency stubs, or source implementations are changed by the claim.

## Baseline validation

Full `tools/check.py` with XDK 5849 against `330e1e2`: 3,061 game matches
(3,062 total), with no upstream matches lost.

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
