# Equipment recovery

Retail range claimed: `0xf8090`–`0xf818f` (`equipment.obj`).

## Mapping evidence

The range contains three untouched inventory entries, totaling 236 retail bytes,
at base `c823e94`:

| Retail | Function | Bytes |
| --- | --- | --- |
| `0xf8090` | `function_f8090` (inferred callback name) | 124 |
| `0xf8110` | `function_f8110` | 68 |
| `0xf8160` | `function_f8160` | 44 |

The retail object-type table `0x468630` points to the definition at `0x467d98`
for type 3, whose name is `equipment`. Its callback at offset `0x30` points to
`0xf8090`. This named retail data anchor replaces a missing function symbol.
The callback consumes placement flags at offset `0x34` and updates object
physics/placement state. The next two routines play the pickup sound stored
at equipment-definition offset `0x138`, through the same sound helper.

The 2003 profile map identifies `function_f8110` at `0xb71c0` and
`function_f8160` at `0xb7210` in `equipment.obj`. The debug
map names the same pair at `0x15a9a0` and `0x15a9e0`. Their order and behavior
support the retail mapping. Function names other than the callback come from
these maps; mapping to retail addresses is inferred from behavior and order.

The preceding `0xf8070` is a vehicle helper called from `0xf6d10`. The next
routine, `0xf8190`, iterates the projectile type mask `0x20` and deletes those
objects, so it is excluded. Claude's projectile PR #15 begins at `0xf8200`.
The equipment claim overlaps neither issue #9 nor any open PR.

The CC0 [Halo CE reference](https://github.com/punpckhdq/halo),
`source/items/equipment.c`, independently has this same three-function order
and placement/pickup behavior. Retail Halo 2 uses its existing physics helper
for placement rather than the older direct at-rest flag write.

## Implementation and validation

All three functions are implemented in `src/equipment.cpp`:

| Function | Result |
| --- | --- |
| `function_f8110` | Exact match, 68 bytes |
| `function_f8160` | Exact match, 44 bytes |
| `function_f8090` | Byte differences; 118 bytes through `ret 8` versus retail's 124 |

The checker reports 124 bytes for placement because its comparison includes
six trailing alignment bytes. The remaining differences are the unchanged
physics helper's argument convention, instruction scheduling, combined object
flag stores, and a branchless item-flag assignment. The placement callback uses
`__stdcall` because retail's object-type definition stores its address and its
body returns with `ret 8`. No artificial calling-convention controls are used.

The source includes the actual type-definition prefix at `0x467d98`, through
its placement callback. Parent types and later fields remain outside this
partial view. The built name, tag, metadata, zero slots, and callback target
were checked against retail. The three floating-point references also match:
`0.05f` for the vertical placement adjustment and `1.0f` for both pickup calls.

Full XDK 5849 check3 at base `50e179f`: **4,049 game matches / 4,051 total**,
two above baseline, with no upstream matches lost. Twenty assertions compiled
with the original SDK verify the local structure offsets, object headers,
tag instances, and callback prefix. Validation is build/disassembly based;
no game runtime tests were run.

No new stubs, shared-header changes, other files' flags, or inventory changes.
Both existing callees retain their upstream declarations and implementations.
The draft claim was published before the source was written.

## Attribution

Symbol names and object ownership were consulted in the
[Halo Symbol Atlas](https://github.com/tinkerer-red/halo-symbol-atlas),
licensed CC BY 4.0. The maps used are the retail build
`03215919bb7163259257d361f4c7bf802a7ab12aa85e2689436369b5c427935d`,
2003 profile build `4f4f09b181eec4a434418b38efe581e75aaf3047c24add8a712751d6ae0d34d3`,
and 2003 debug build `96ea21d862dfe6a0bebb23e1a4311202a6e18a79970189a4df320d4ededa439d`.
The C++ will be reconstructed from retail disassembly, with no original game
or SDK files committed.
