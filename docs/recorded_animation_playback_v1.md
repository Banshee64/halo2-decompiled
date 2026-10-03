# Legacy recorded animation playback recovery

Retail range claimed: `0x29f180`–`0x29f47f`
(`recorded_animation_playback_v1.obj`).

The range contains 13 inventory entries totaling 662 retail bytes. Eleven
already match in `src/unknown_29f180.cpp` at base `9cd127f`; their bodies,
flags, declarations, and callback table will be retained unchanged. This
work recovers the two missing stream routines.

## Range evidence

- Retail names `recorded_animation_apply_event_stream_v1` at `0x29f400`.
- The 2003 profile/debug maps group the legacy field readers and stream
  routines in `recorded_animation_playback_v1.obj`.
- The codec pair at `0x46fd54` contains the initializer `0x29f3e0` and
  stream reader `0x29f400`. Its neighboring pair at `0x46fd4c` selects the
  newer format and belongs to PR #14.
- The legacy reader uses a four-byte event header (signed short type,
  unsigned short tick delay) and dispatches through `0x471220`.
- The previous module ends at `0x29f17f`. The next function, `0x29f480`,
  iterates AI data arrays and builds actor/object lists, so it is excluded.
- Issue #9 and all open PRs were checked before claiming; no overlap was found.

## Inventory

| Address | Function | Current result |
| --- | --- | --- |
| `0x29f180`–`0x29f370` | Eleven field-reader callbacks | Upstream matches retained |
| `0x29f3e0` | `recorded_animation_initialize_event_stream_v1` | To recover |
| `0x29f400` | `recorded_animation_apply_event_stream_v1` | To recover |

The initializer's name is inferred from older maps and the codec pair.
The shared reader `0x2c4e10` remains outside the implementation claim. If
still missing upstream, its stub file and declaration will be identical to
PR #14's `src/stubs/recorded_animation_playback.cpp`, so both PRs can merge
without duplicate definitions or conflicting declarations.

## Validation

Full XDK 5849 baseline check: **3,620 game matches / 3,621 total**,
with no upstream matches lost. Implementation changes
will receive a full check against current upstream and retain all prior
matches. The event and codec layouts and callback table targets will also
be verified. No game runtime testing is planned.

## Sources

Name and object evidence comes from
[halo-symbol-atlas](https://github.com/tinkerer-red/halo-symbol-atlas)
(CC BY 4.0): retail SHA-256
`03215919bb7163259257d361f4c7bf802a7ab12aa85e2689436369b5c427935d`,
2003 profile map
`4f4f09b181eec4a434418b38efe581e75aaf3047c24add8a712751d6ae0d34d3`,
and 2003 debug map
`96ea21d862dfe6a0bebb23e1a4311202a6e18a79970189a4df320d4ededa439d`.
Boundaries remain provisional where retail symbols are absent.
