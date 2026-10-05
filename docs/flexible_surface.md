# Cloth simulation and rendering pass

Retail range claimed: `0x1168a0`–`0x11850f`.

The retail callback table at `0x4674d8` identifies this group. Its entries
include creation (`0x116980`), update (`0x1168a0`), rendering (`0x116b00`)
and render submission (`0x117060`). They share the record pool `g_4e0338`;
its existing initializer uses the retail string `cloth` and a record size
of `0x1258`. Calls from those callbacks reach the remaining helpers through
`0x118430`. The next routine, `0x118510`, starts a separate call graph and
is outside this claim.

There are 21 discovered entries and 7,138 retail bytes in the range. Six
lifecycle/accessor routines already live in `src/unknown_1169f0.cpp` and
will be preserved: `0x1169f0`, `0x116a10`, `0x116a50`, `0x116a70`,
`0x116a80`, and `0x116ac0`. This pass recovers the remaining 15 functions.
The shared callback table will gain their four currently missing entries.
The shared callback at `0x24dc50` is outside the claim.

## Evidence and scope

The range is based on retail callback pointers, direct calls, and shared
pool and tag-data references. Names introduced by this pass describe the
observed behavior or use address placeholders. Issue #9 and all open PRs
were checked before claiming; no active range overlaps this pass.

The draft claim precedes implementation. Validation results, recovered
layouts, dependencies, and remaining byte differences will be recorded
here before the PR is marked ready.
