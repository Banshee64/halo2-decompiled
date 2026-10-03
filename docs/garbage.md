# Garbage object recovery

Retail range claimed: `0x11bb40`–`0x11bbef` (`garbage.obj`).

## Mapping evidence

The range contains one untouched 169-byte entry at base `c1bcd3c`:
`garbage_new` at `0x11bb40` (name inferred from callback role and reference).
Retail's named `garbage` type definition at `0x467e60` stores this address
in its creation slot at `+0x2c`. It is the only non-null code callback in that
type definition. The type's datum size is `0x170`.

The 2003 profile and debug maps record static initializers in `garbage.obj`
(at `0x21bc90`–`0x21bd00` and `0x3847c0`–`0x384830` respectively), confirming
that original object file's presence. Those maps do not name the retail
creation function. The CC0 [Halo CE reference](https://github.com/punpckhdq/halo),
`source/items/garbage.c`, identifies the corresponding `garbage_new` behavior:
garbage-list membership, shadowless/deactivation flags, and a randomized
lifetime. Halo 2's retail callback has a different lifetime calculation.

The retail callback calls the existing object-list helper `0xbb950`, sets
object flag `0x20000`, advances the deterministic random state at `0x4e7408`
once, and stores an absolute expiration tick at object offset `0x16c`.
The random step is `seed = 1664525 * seed + 1013904223`, with its high 16 bits
scaled by `1 / 65535`. The lifetime is `(sample + 1) * 10 * ticks_per_second`,
rounded by the x87 `fistp` instruction and added to current game time.
Finally, it sets the shadowless flag `0x10000` and returns true.

The preceding device-controls module belongs to PR #18 and ends at
`0x11bb3f`. The following `0x11bbf0` is the crate creation callback and is
excluded. Issue #9 and all open PR descriptions were checked: no overlap.

## Plan and scope

Publish the draft claim before code. Recover the creation callback and the
actual type-definition prefix through its creation slot. Reuse upstream
`function_bb950(long, bool, long)` without changes. No new stubs, shared-header
edits, other files' flags, or inventory changes are planned. Preserve the
retail random draw and floating-point conversion order, and run a full
original-compiler check before publishing source.

## Attribution

Symbol names and object ownership were consulted in the
[Halo Symbol Atlas](https://github.com/tinkerer-red/halo-symbol-atlas),
licensed CC BY 4.0. Builds consulted:

- Retail: `03215919bb7163259257d361f4c7bf802a7ab12aa85e2689436369b5c427935d`.
- 2003 profile: `4f4f09b181eec4a434418b38efe581e75aaf3047c24add8a712751d6ae0d34d3`.
- 2003 debug: `96ea21d862dfe6a0bebb23e1a4311202a6e18a79970189a4df320d4ededa439d`.

Source will be reconstructed from retail disassembly with the CC0 reference;
game and SDK files remain outside the contribution.
