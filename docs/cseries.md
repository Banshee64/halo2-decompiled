# Cseries string helper recovery

Retail range claimed: `0x11c920`–`0x11ca1f` (`cseries.obj` string helpers).

## Mapping evidence

Retail names `csstricmp` at `0x11c920` in `cseries.obj` (128 bytes). Both
2003 maps corroborate that name and ownership: profile `0x1b580`, debug
`0x1e5d0`. The nearby bounded length and formatting routines also occur in
those maps. The four retail entries in this cluster total 252 bytes:

| Address | Routine | Base status |
| --- | --- | --- |
| `0x11c920` | `csstricmp` | Missing |
| `0x11c9a0` | `csstrnlen` | Matched in `src/unknown_11c9a0.cpp` |
| `0x11c9c0` | `csnprintf` (upstream name) | Matched in `src/unknown_11c9c0.cpp` |
| `0x11c9e0` | `function_11c9e0` (append formatting) | Matched in the same file |

The three existing routines, their declarations, and their flags will remain
unchanged. This claim covers the surviving string-helper cluster, not every
routine historically compiled into `cseries.obj`. The preceding function
at `0x11c840` iterates and preprocesses tags; the following `0x11ca20` begins
memory wrappers. Both are excluded. Issue #9 and open PR descriptions were
checked: no overlap.

## Comparison behavior

The comparator reads signed narrow characters, converts each to the SDK's
16-bit `wchar_t`, and calls `towlower` at retail `0x321846`. It compares the
zero-extended results as integers and returns only -1, 0, or 1. It stops at
an unequal pair or either terminator. It uses the wide-character CRT helper,
not the narrow `tolower` or `_stricmp` functions; preserving that distinction
also preserves how bytes with the high bit set are passed to the CRT.

## Plan and scope

Publish the draft claim before source, then recover `csstricmp` in
`src/cseries.cpp`. Use the SDK declaration and linked `towlower` implementation;
no dependency stubs, shared headers, other files' flags, or inventory changes
are needed. Run a full original-compiler check and confirm that the three
existing helpers and every other upstream match survive.

## Attribution

Names and original object ownership were consulted in the
[Halo Symbol Atlas](https://github.com/tinkerer-red/halo-symbol-atlas),
licensed CC BY 4.0:

- Retail: `03215919bb7163259257d361f4c7bf802a7ab12aa85e2689436369b5c427935d`.
- 2003 profile: `4f4f09b181eec4a434418b38efe581e75aaf3047c24add8a712751d6ae0d34d3`.
- 2003 debug: `96ea21d862dfe6a0bebb23e1a4311202a6e18a79970189a4df320d4ededa439d`.

Implementation will follow retail disassembly. The CC0 Halo CE cseries
reference was inspected, but it does not supply this comparator's body.
Game and SDK files remain outside the contribution.
