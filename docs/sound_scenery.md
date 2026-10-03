# Sound scenery recovery

Retail range claimed: `0x207b90`–`0x207c1f` (`sound_scenery.obj`).

## Mapping evidence

At base `0300b14`, the range contains two untouched entries, 139 retail bytes:

| Retail | Function | Bytes |
| --- | --- | --- |
| `0x207b90` | `sound_scenery_new` | 44 |
| `0x207bc0` | `sound_scenery_place` (inferred name) | 95 |

Retail's object-type table `0x468630` points to the named `sound_scenery`
definition at `0x4680b8`. Its creation and placement slots, `+0x2c` and
`+0x30`, hold `0x207b90` and `0x207bc0`. The creation callback sets the
shadowless object flag and returns true. Placement copies seven 32-bit
fields from scenario offsets `0x34`–`0x4c` into object offsets
`0x12c`–`0x144`. Their individual meanings are not established here.

The 2003 profile map names `sound_scenery_new` at `0x1242d0` in
`sound_scenery.obj`. The debug map names an empty `sound_scenery_delete`
at `0x24c940` in the same object. The CC0
[Halo CE reference](https://github.com/punpckhdq/halo),
`source/sound/sound_scenery.c`, corroborates the creation routine's
shadowless flag and true return. There is no distinct delete callback in
this retail range. The placement name is inferred from its type-table slot
and its scenario-to-object copies.

The preceding routine at `0x207ab0` uses unrelated per-entry strides and
state; the following `0x207c20` starts code working on different data pools
(`0x4f9384`/`0x4f9394`). Neither accesses these callback fields. Both are
excluded. Issue #9 and all open PR descriptions were checked: no overlap.

## Plan and scope

Publish this draft claim before source. Recover both callbacks and the
actual type-definition prefix through its placement slot. Preserve the
retail callback conventions supported by `ret 12` and `ret 8`. No external
function calls, new stubs, shared-header edits, other files' flags, or
inventory changes are needed. Run a full original-compiler check before
publication.

## Attribution

Symbol names and object ownership were consulted in the
[Halo Symbol Atlas](https://github.com/tinkerer-red/halo-symbol-atlas),
licensed CC BY 4.0. Builds consulted:

- Retail: `03215919bb7163259257d361f4c7bf802a7ab12aa85e2689436369b5c427935d`.
- 2003 profile: `4f4f09b181eec4a434418b38efe581e75aaf3047c24add8a712751d6ae0d34d3`.
- 2003 debug: `96ea21d862dfe6a0bebb23e1a4311202a6e18a79970189a4df320d4ededa439d`.

Source will be reconstructed from retail disassembly with the CC0 reference.
Game and SDK files remain outside the contribution.
