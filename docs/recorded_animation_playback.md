# Recorded animation playback recovery

Retail range claimed: `0x29ec30`–`0x29f17f`
(`recorded_animation_playback.obj`).

The inferred module boundary contains 13 inventory entries totaling 1,265
retail bytes. Seven entries already match in `src/unknown_29ec30.cpp` at
base `80435cf`. Those implementations and their declarations are preserved;
this work recovers the six missing entries.

## Range evidence

- Retail names `update_controller_char` at `0x29ed00` and
  `update_controller_short` at `0x29ed40` in this object.
- The 2003 profile map groups the field readers, the two controller updates,
  vector decompression, vector-difference callbacks, stream initialization,
  and event-stream application in `recorded_animation_playback.obj`.
- Retail's byte and short delta callbacks call the corresponding controller
  update and vector decompressor. They have the same vector-copy cases and
  advance the stream cursor by two and four bytes respectively.
- The stream reader at `0x29f0c0` dispatches through table `0x4710f0`.
  Its final jump table ends at `0x29f17f`. The following callbacks at
  `0x29f180` use the legacy event layout and belong to the adjacent
  `recorded_animation_playback_v1.obj` module, excluded from this claim.
- The preceding `0x29e730` routine concerns spatial/path data and is excluded.
- Issue #9 and open PRs were checked before claiming. This range has no
  active claim. Existing upstream functions are retained without rework.

## Inventory

| Address | Interpretation | Initial state |
| --- | --- | --- |
| `0x29ec30`–`0x29ecd0` | Six field-reader callbacks | Six upstream matches retained |
| `0x29ed00` | `update_controller_char` | Upstream match retained |
| `0x29ed40` | `update_controller_short` | To recover |
| `0x29ed80` | `uncompress_vector_from_controller` | To recover |
| `0x29edc0` | `apply_vector_char_difference` | To recover |
| `0x29ef20` | `apply_vector_short_difference` | To recover |
| `0x29f080` | `recorded_animation_initialize_event_stream` | To recover |
| `0x29f0c0` | `recorded_animation_apply_event_stream` | To recover |

Except for the two retail-named controller updates, names are inferred from
older maps and retail behavior. The legacy version and the shared unit-control
reader at `0x2c4e10` remain outside the implementation claim.

## Sources

Name and object evidence comes from
[halo-symbol-atlas](https://github.com/tinkerer-red/halo-symbol-atlas)
(CC BY 4.0): retail SHA-256
`03215919bb7163259257d361f4c7bf802a7ab12aa85e2689436369b5c427935d`,
2003 profile map
`4f4f09b181eec4a434418b38efe581e75aaf3047c24add8a712751d6ae0d34d3`,
and 2003 debug map
`96ea21d862dfe6a0bebb23e1a4311202a6e18a79970189a4df320d4ededa439d`.
Boundaries and names remain provisional where retail symbols are absent.
