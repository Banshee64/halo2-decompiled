# Device light fixture recovery

Retail range claimed: `0x11bdc0`–`0x11be1f` (`device_light_fixtures.obj`).

## Mapping evidence

The range contains one untouched entry at base `c1bcd3c`: the 88-byte
placement callback at `0x11bdc0`. Retail's named `light_fixture` type
definition at `0x4684a0`, reached from object type table `0x468630` index 9,
holds this address in its placement slot at `+0x30`. It is the type's sole
non-null code callback. The datum size is `0x1e4`.

The 2003 profile map names `light_fixture_new` at `0x1577e0` in
`device_light_fixtures.obj`. The debug map groups its empty lifecycle,
creation, deletion, and update routines at `0x24c5b0`–`0x24c670`. Neither
map names the retail placement callback. Its `light_fixture_place` name and
field meanings are inferred from the callback slot, the copied layout, and
the corresponding routine in the CC0 [Halo CE reference](https://github.com/punpckhdq/halo),
`source/devices/device_light_fixtures.c`.

Retail copies an RGB color from scenario `+0x3c` to object `+0x1cc`, then
intensity, falloff angle, and cutoff angle from scenario `+0x48/+0x4c/+0x50`
to object `+0x1d8/+0x1dc/+0x1e0`. It has two stack arguments (`ret 8`) and
no external calls. Unlike the CE reference, this retail callback does not
call the generic device scenario helper.

Preceding crate callbacks are excluded. The following function at `0x11be20`
clears unrelated global storage at `0x4f93b8` and jumps to `0x211cc0`, so it
is excluded too. Issue #9 and all open PR descriptions were checked for
conflicts; none overlap this range.

## Implementation and validation

`light_fixture_place` matches all **88 retail bytes** in the first source
implementation and full check. The local views give the color and three
scalar light properties their reference names. The recovered type-definition
prefix ends at its placement slot; later fields and parent types remain
outside the view.

Validation against upstream `c1bcd3c`:

- Full original-compiler `tools/check.py`: **4,229 game / 4,229 total
  matches**, one above baseline, with no upstream match lost.
- Twenty-five original-SDK compile-time assertions validate the color,
  object, object header, scenario, data-array, and type-prefix layouts.
- Linked type metadata/name, placement pointer, object-global load, all
  six copied field offset pairs, and the two-argument return were verified.
- No game runtime tests were run.

The draft claim was published before source. Only `src/device_light_fixtures.cpp`
and this document change. There are no external calls, new stubs, shared-header
edits, other files' flag changes, or committed inventory changes. No compiler
attributes, artificial callers, or flag tuning were needed.

## Attribution

Symbol names and object ownership were consulted in the
[Halo Symbol Atlas](https://github.com/tinkerer-red/halo-symbol-atlas),
licensed CC BY 4.0. Builds consulted:

- Retail: `03215919bb7163259257d361f4c7bf802a7ab12aa85e2689436369b5c427935d`.
- 2003 profile: `4f4f09b181eec4a434418b38efe581e75aaf3047c24add8a712751d6ae0d34d3`.
- 2003 debug: `96ea21d862dfe6a0bebb23e1a4311202a6e18a79970189a4df320d4ededa439d`.

Source was reconstructed from retail disassembly with the CC0 reference;
game and SDK files remain outside the contribution.
