# A depth-range pixel shader (unknown_03a5c0)

Retail range covered: `0x3a5c0`–`0x3b89b`. This is one inventory entry, 4,828
retail bytes, `todo` with no source: the function that builds an 8-stage
register-combiner program in `g_484f68` for a range in one of four modes, sets
the render state for it unless told not to, binds it and gives the start of the
range. **Analysis only:** this document adds no source, and nothing in it has
been built or checked against retail with the original compiler. Names are
provisional. The `function_<va>` form is primary; the descriptions are offered
for whoever decompiles it.

It uses the names of `src/unknown_020560.cpp` (`g_484f68`, the render-state
cache `g_4b81fc` and `function_0222d0`, which sets a state and caches it),
`src/unknown_023540.cpp` (`function_276d0` and `function_26520`, simpler passes
of the same shape, and the callbacks `function_26710` and `function_27520`),
`src/unknown_01cf50.cpp` (`function_14bc0`, `function_14f60`, `function_1cc30`,
`function_1c710` and `function_1ccf0`) and `src/unknown_131e50.cpp` (the colour
packers), with the `D3DRS_` names those sources use, and the name
`docs/flexible_surface.md` gives `function_1c710`. No document covered the
function before this one.

The range is a depth range (inferred). It is divided by `g_48565c`, the real
that `function_12d50` divides 16777215.0 by (`src/unknown_01cf50.cpp:2687`);
`0x4f7e0` passes a depth; and `function_26710` and `function_27520`, registered
beside two of the callers, map the same ranges as those callers pass
(`src/unknown_023540.cpp:337`, `803`).

## Boundary

- `0x3a550`, just before, is `todo` with no source. `0x3b8a0`, just after, is
  matched, with source in `src/unknown_03bcb0.cpp`. Both are excluded. The
  range ends with a two-byte `mov edi, edi` (`0x3b88a`) and the function's own
  jump table (`0x3b88c`–`0x3b89b`); four `int3` bytes follow before `0x3b8a0`.
- Lane W's row of the Active claims table (issue #9) covers the function and
  its four callers. This document makes no claim and is offered to lane W. No
  open pull request touches it.
- `0x3a5c0` has no `@retail` or `@stub` marker; see
  [Existing declarations](#existing-declarations).

## Conventions

"Callers" counts the functions that call an entry directly, from outside the
range and then from inside it. No pointer or table refers to the function.
Below, argN is the Nth stack argument, at `esp` + 4N on entry.

| Retail | Bytes | Arguments | Returns | Callers | What it does |
| --- | --- | --- | --- | --- | --- |
| `0x3a5c0` | 4828 | stack: nine arguments (`ret 0x24`); see [Data](#data) | bool in `al` | 4 + 0 | Builds and binds the combiner program for a range in mode 0 to 3, sets its render state unless arg8 is set, and gives the start of the range in *arg9 |

It reads no register input: every register is set before use, and `ebx`, `edi`,
`esi` and `ebp` are saved (`0x3a5d2`, `0x3a5d9`, `0x3a7c8`, `0x3a98d`). The
frame is `0x2c` bytes (`0x3a5c0`). It returns true at `0x3b877` and false at
`0x3b881`.

The four callers are `todo` with no source, and each passes a different mode:

| | `0x25e50` | `0x26e50` | `0x37a60` | `0x4f7e0` |
| --- | --- | --- | --- | --- |
| Call | `0x25e89` | `0x26ef0` | `0x37db1` | `0x4fb6a` |
| arg1 | `&g_48574c` | a 0x18-byte block | a 0x34-byte block | −1 |
| arg2, arg3 | `g_485778`, `g_48577c` | `g_4857dc`, `g_4857e0` | its argument's `+0x80`, `+0x84` | d − r, d + r |
| arg4 (mode) | 1 | 2 | 3 | 0 |
| arg5 to arg8 | 0, 0, 0, 0 | 0, 0, 1, 0 | 0, 0, 1, 1 | 1, 1, 1, 0 |
| arg9 | `&g_509400` | `&g_509400` | a local | a local |

- `0x25e50` calls it only when `g_48578c` is 0.0 (`0x25e50`–`0x25e66`) and
  returns true at once on success (`0x25e90`); on failure it stores `g_485ae0`
  in `g_509400` and goes on (`0x25e96`). `0x25d50` registers `0x25e50` with
  `0x34420` (in `eax`), beside `function_26710`, when `g_485a75` is clear and
  `g_48574c` is 1, 4 or 5 or the byte at `0x485860` is set, and otherwise, when
  `g_48574c` is 2 or 3, registers `function_26520` instead
  (`0x25d50`–`0x25d98`).
- `0x26e50` calls it only when `g_4b9d9c` is false (`0x26e59`, `0x26e6d`); on
  failure it stores `g_485ae0` in `g_509400` (`0x26f81`). Its block holds the
  reals at `0x4857d4` and `0x4857d8`, a copy of `g_4857bc` at `+8`, and at
  `+0x14` a byte that is 1 when the byte at `0x5093fe` is 0
  (`0x26e53`–`0x26eec`). `0x25dc0` registers it beside `function_27520` when
  its `cl` input is set, after storing its `dl` input at `0x5093fe`
  (`0x25de7`), and only when the dword at `0x4857b8` is neither −1 nor 0 and
  the real at `0x4857d8` is above 0.0 (`0x25dc5`–`0x25ddd`). That path first
  registers `0x26880` beside `function_26ca0`; with `cl` clear and `dl` set it
  registers `function_276d0` instead (`0x25e14`–`0x25e28`).
- `0x37a60`, called only from `0x36b50` (`0x36eca`), builds its block at
  `0x37c32`–`0x37d8d`: byte `+0` is 1; the colours at `+4`, `+0x10` and `+0x1c`
  have red and blue 0, and as green a product of three reals, pinned to [0, 1],
  that it reads from its argument at an index (the short at `0x485600`) times
  0xb8 (`0x37a87`, `0x37a90`, `0x37c32`–`0x37c6a`); and the colour at `+0x28`
  is its argument's colour at `+0x98`, pinned. It reads the local after a true
  result (`0x37dce`) and skips that code on false (`0x37db8`).
- `0x4f7e0`, called from `0x4f792` and `0x51648`, calls it in a loop over
  records. d is the dot product of a point minus `g_4b9da0` with `g_4b9dac`,
  the quantity `src/unknown_01cf50.cpp:2218` calls depth, and r is a real of
  the record; both ends are pinned to the reals at `0x4b9de0` and `0x4b9de4`
  (`0x4fa6f`–`0x4fb36`). It calls only when the pinned d + r is above the
  pinned d − r and d is not 0 (`0x4fb33`–`0x4fb49`), and compares the local
  with d after a true result (`0x4fb77`–`0x4fb8a`).

No caller uses mode 0 with arg1 other than −1, or a mode outside 0 to 3.

## Data

**The arguments.** arg5 to arg8 are read only as bytes.

| Argument | Read as | Use |
| --- | --- | --- |
| arg1 | dword | mode 0: compared with −1 only (`0x3aaf3`); modes 1 to 3: a pointer to the mode's parameters, below |
| arg2, arg3 | real | the start and the end of the range |
| arg4 | dword | the mode (`0x3a5ce`, `0x3aae7`); a value outside 0 to 3, negative ones included, gives the common program only |
| arg5 | byte | when the slope is capped, centre the start (`0x3a71c`) |
| arg6 | byte | when capped, set the start from the end (`0x3a744`); with arg7, accept a negative start in modes other than 0 and 1 (`0x3a7a4`) |
| arg7 | byte | when capped, keep the start (`0x3a75b`); with arg6, as above (`0x3a7b0`) |
| arg8 | byte | leave the render state alone (`0x3a7c2`, `0x3ab07`, `0x3afba`, `0x3b29f`, `0x3b4fe`, `0x3b556`, `0x3b855`) |
| arg9 | real pointer | receives the start, unless NULL (`0x3b821`–`0x3b871`) |

The argument slots are reused once read. The arg2 and arg3 slots hold the
scaled start and end (`0x3a60a`, `0x3a610`), then the start and the slope
(`0x3a6ab`, `0x3a6fb`). The arg5, arg6 and arg7 slots hold temporaries in the
modes (from `0x3aafd`). arg4, arg8 and arg9 are never written.

**The modes' parameters**, through arg1:

| Mode | Offset | Read as | Use | What the caller passes |
| --- | --- | --- | --- | --- |
| 1 | `+0x1c`, `+0x20`, `+0x24` | reals | times `+0x28`, pinned: colour A's red, green and blue | `g_485768` (`src/unknown_033a0b.cpp:46`) |
| 1 | `+0x28` | real | pinned: colour A's alpha | `g_485774` (`src/unknown_033a0b.cpp:47`) |
| 1 | `+0x5c`, `+0x60`, `+0x64` | reals | pinned: colour B's red, green and blue | `g_4857a8` (`src/unknown_023540.cpp:885`) |
| 1 | `+0x68` | real | pinned: colour B's alpha | `g_4857b4` (`src/unknown_023540.cpp:886`) |
| 2 | `+0`, `+4` | reals | `function_131fc0`, into `PSConstant0[5]` and `PSConstant1[5]` | the reals at `0x4857d4` and `0x4857d8` |
| 2 | `+8` | `color3f` | `pack_color3f`, into `PSFinalCombinerConstant0` when `+0x14` is clear | a copy of `g_4857bc` (`src/unknown_023540.cpp:1002`) |
| 2 | `+0x14` | byte | chooses the final combiner, two more render states (COLORWRITEENABLE 1 and ALPHABLENDENABLE 0) and a `function_14bc0(3, 0, true)` call that selects the render target and depth surfaces | 1 when the byte at `0x5093fe` is 0 |
| 3 | `+0` | byte | chooses `PSRGBInputs[4]` and `PSRGBInputs[5]` | 1 |
| 3 | `+4`, `+0x10`, `+0x1c`, `+0x28` | `color3f` | `pack_color3f`, into `PSConstant0[4]`, `PSConstant1[4]`, `PSConstant0[5]` and `PSFinalCombinerConstant0` | four pinned colours |

`g_48574c` is declared as a `long` (`src/unknown_03bcb0.cpp:540`), and
`0x25d50` compares it with 1 to 5. Yet mode 1 reads `g_485768`, `g_485774`,
`g_4857a8` and `g_4857b4` through its address, globals the source declares
separately, so `0x48574c` is probably the start of one structure (inferred).

**The program** is `g_484f68`, the `D3DPIXELSHADERDEF` defined at
`src/unknown_020560.cpp:59`. It is cleared (0xf0 bytes, `0x3a9ee`–`0x3a9f8`),
and these fields are written. The offsets come from retail stores of functions
whose source names the field (`function_20f30`, `function_251b0`,
`function_276d0`):

| Offset | Field |
| --- | --- |
| `+0x00` + 4i | `PSAlphaInputs[i]` |
| `+0x20`, `+0x24` | `PSFinalCombinerInputsABCD`, `PSFinalCombinerInputsEFG` |
| `+0x28` + 4i | `PSConstant0[i]` |
| `+0x48` + 4i | `PSConstant1[i]` |
| `+0x68` + 4i | `PSAlphaOutputs[i]` |
| `+0x88` + 4i | `PSRGBInputs[i]` |
| `+0xac` | `PSFinalCombinerConstant0` |
| `+0xb4` + 4i | `PSRGBOutputs[i]` |
| `+0xd4`, `+0xd8` | `PSCombinerCount`, `PSTextureModes` |

The repository writes these fields as raw numbers, and so does this document.

**The render states.** `function_0222d0` stores the value in `g_4b81fc[state]`
(`dword g_4b81fc[0xa6]`, `src/unknown_020560.cpp:58`) and passes both to
`D3DDevice_SetRenderState` (`src/unknown_020560.cpp:611`). The states here are
`g_4b81fc` indices. Their names come from sources that set a named state beside
its `g_4b81fc` slot: the matched `function_142f0`
(`src/unknown_0494b0.cpp:50`), `function_4a780` (`src/unknown_01cf50.cpp:1453`)
and `src/unknown_0494b0.cpp:138`–`169`, and the order of `function_26520`'s
calls (`src/unknown_023540.cpp:896`, `todo`) against its retail stores.

| State | Name | From |
| --- | --- | --- |
| `0x39` | `D3DRS_ZFUNC` | `function_26520` |
| `0x3b` | `D3DRS_ALPHABLENDENABLE` | `function_142f0` |
| `0x3c` | `D3DRS_ALPHATESTENABLE` | `function_26520` |
| `0x3e`, `0x3f` | `D3DRS_SRCBLEND`, `D3DRS_DESTBLEND` | `function_142f0` |
| `0x40` | `D3DRS_ZWRITEENABLE` | `function_26520` |
| `0x43` | `D3DRS_COLORWRITEENABLE` | `src/unknown_0494b0.cpp:169` |
| `0x4a` | `D3DRS_BLENDOP` | `function_142f0` |
| `0x4b` | `D3DRS_BLENDCOLOR` | `function_26520` |
| `0x52` | `D3DRS_DEPTHCLIPCONTROL` | `src/unknown_0494b0.cpp:141` |
| `0x8f` | `D3DRS_ZENABLE` | `function_26520` |
| `0x90` | `D3DRS_STENCILENABLE` | `function_4a780` |
| `0x93` | `D3DRS_CULLMODE` | `src/unknown_0494b0.cpp:158` |
| `0x95` | `D3DRS_ZBIAS` | `src/unknown_0494b0.cpp:147` |

The values `D3DCMP_ALWAYS` (`0x207`, `function_251b0`), `D3DCMP_LESSEQUAL`
(`0x203`), `D3DBLEND_CONSTANTCOLOR` (`0x8001`), `D3DBLEND_INVSRCALPHA`
(`0x303`), `D3DBLENDOP_ADD` (`0x8006`) (all `function_26520`),
`D3DBLEND_SRCALPHA` (`0x302`, `function_1e930`) and `D3DBLEND_ONE` (1,
`function_29080`, stored at `0x29204`) are named the same way, from those
functions' sources against their retail code.

**Globals.** The range is multiplied by 1.0 / `g_48565c` (`real`,
`src/unknown_023540.cpp:308`) on entry, and the start by `g_48565c` on the way
out. The real at `0x485658`, which has no repository name, is the least start
the function reports; the image has three references to it, all reads
(`0x38093` in `0x37a60`, `0x3b848`, `0x3b869`). Both are 0 in the image and set
at run time. `g_4858b8` (`long`, `src/unknown_01cf50.cpp:1342`) is read as a
word, and `g_51f0f0` (`byte g_51f0f0[0x2d8]`, `src/flexible_surface.cpp:441`)
is passed by address.

**Locals.** The slope cap, the cut-off alpha a0 (0.0 until set) and the byte
flag "rescaled", at entry `esp` − 0x24, − 0x28 and − 0x29 (`0x3a604`,
`0x3a5d3`, `0x3a5ff`); and in mode 1 two `color4f` values
(`{alpha, red, green, blue}`, `include/unknown_0259d0.h:25`), colour A and
colour B, at entry `esp` − 0x10 and − 0x20 (`0x3b158`, `0x3b218`).

## The function

It has one entry and two exits: `0x3b875` returns true and `0x3b880` false. It
checks the range, sets up the render state unless arg8 is set, builds the part
of the program that every mode shares, adds the mode's stages, and binds the
program. Nothing outside the stack is written before `0x3a7c2`, so a false
result changes nothing. A jump table on arg4 (`0x3a618`, table at `0x3b88c`)
sets the mode's limits at the start, and a chain of compares (`0x3aaed`,
`0x3ae5e`, `0x3b299`, `0x3b550`) chooses the mode's stages later.

The listings below use this notation; k is set in step 3 of the common program.

```
pin(x)  = x < 0 ? 0 : x > 1 ? 1 : x       (a NaN passes through)
R(n)    = k > n ? 0x20 : 0
S(a, b) = 0xd00 | (k > a ? 0x20000 : k > b ? 0x10000 : 0)
S0      = 0xd00 | (k > 0 ? 0x10000 : 0)
fl(x)   = (long)floor(x)
B(x)    = fl(x) < 0 ? 0 : fl(x) > 0xff ? 0xff : fl(x)
w       = B(slope * 256.0 / 2^k) << 24 | B(slope * 0.5)
```

B evaluates fl up to three times, as written; each floor is followed by
`__ftol2`.

### The range (`0x3a5c0`–`0x3a7bc`)

1. r = 1.0 / `g_48565c` (`0x3a5dd`); start = arg2 × r and end = arg3 × r
   (`0x3a5e8`, `0x3a5ee`).
2. The jump table on arg4 (`0x3a5fd`–`0x3a64b`) sets the slope cap: 512.0 in
   mode 0, 64.0 in mode 1, 128.0 in modes 2 and 3, and 1.0 for any other value
   (the bound check at `0x3a616` is unsigned). Modes 0 and 2 allow the rescale
   of step 3, and modes 0 and 1 the cut-off alpha of step 6.
3. When the rescale is allowed and end × 256.0 is not above 1.0
   (`0x3a651`–`0x3a664`; an unordered compare skips it), start and end are
   multiplied by 256.0 and "rescaled" is set (`0x3a666`–`0x3a676`).
4. Each is rounded down to a multiple of 1/65536, as (real)(long)floor(x ×
   65536.0) × (1/65536) with `_floor` and `__ftol2` (`0x3a67b`–`0x3a6cb`). It
   returns false when end is not above start, or the two are unordered
   (`0x3a6d6`).
5. slope = 1.0 / (end − start) (`0x3a6dc`–`0x3a6ee`). When slope is above the
   cap (`0x3a6f8`), slope becomes the cap (`0x3a710`). Then it returns false if
   "rescaled" is set (`0x3a716`); otherwise, with arg5, start = (start + end) ×
   0.5 − (1.0 / cap) × 0.5 (`0x3a724`–`0x3a73c`); else with arg6, start = end −
   1.0 / cap (`0x3a74c`–`0x3a756`); else with arg7 the start stays; and with
   none of the three it returns false (`0x3a761`).
6. When start is below 0.0 (`0x3a770`): in modes 0 and 1, a0 = pin(0.0 − slope
   × start) (`0x3a779`–`0x3a7a2`); in any other mode it returns false unless
   arg6 and arg7 are both set (`0x3a7a4`–`0x3a7b6`). Either way, start = 0.0
   (`0x3a7bc`).

### The render state (`0x3a7c2`–`0x3a984`)

Unless arg8 is set (`0x3a7c2`, `0x3a7c9`), it makes these calls, in this order,
and the notes below follow the same order. "inline" marks a state that retail
sets without calling `function_0222d0`: it writes the `g_4b81fc` slot and the
body of `D3DDevice_SetRenderState` for that state in place.

```c
function_14bc0((short)g_4858b8, 0, true);           /* 0x3a7dc */
function_14f60(0, 3);                               /* 0x3a7e3 */
/* eleven stage-0 texture states, inline: 0x3a7e8-0x3a855 */
function_0222d0(D3DRS_COLORWRITEENABLE, 0x1010101); /* inline, 0x3a85b */
function_0222d0(D3DRS_ALPHABLENDENABLE, 0);         /* inline, 0x3a873 */
function_0222d0(D3DRS_ALPHATESTENABLE, 0);          /* inline, 0x3a88b */
function_0222d0(D3DRS_CULLMODE, 0);                 /* inline, 0x3a89d */
function_0222d0(D3DRS_STENCILENABLE, 0);            /* inline, 0x3a8a9 */
function_0222d0(D3DRS_ZENABLE, 0);                  /* inline, 0x3a8ba */
function_0222d0(D3DRS_ZWRITEENABLE, 0);             /* inline, 0x3a93a */
function_0222d0(D3DRS_ZFUNC, D3DCMP_ALWAYS);        /* 0x3a955 */
function_0222d0(D3DRS_ZBIAS, 0);                    /* inline, 0x3a95b */
function_0222d0(D3DRS_DEPTHCLIPCONTROL, 0x10);      /* 0x3a970 */
function_1cc30(12);                                 /* 0x3a97a */
function_1c710(g_51f0f0);                           /* 0x3a984 */
```

1. `function_14bc0` takes its index and element on the stack and use_depth in
   `al` (`ret 8`): here the word at `g_4858b8`, 0 and true
   (`0x3a7cf`–`0x3a7dc`). `function_14f60` takes the stage in `bx` (0,
   `0x3a7e1`) and the index in `di` (3 since `0x3a5f4`). It records `g_509368`,
   index 3's texture, as stage 0's in `g_51f3c8[1][0]`, and its size in
   `g_485af4[0]` and `g_485b04[0]` (`src/unknown_01cf50.cpp:506`).
   `function_1cf50` and `function_1cf80` are the source's functions that pass
   `g_51f3c8[1]` to `D3DDevice_SetTexture` (`src/unknown_01cf50.cpp:641`,
   `656`).
2. The eleven texture states are the stores that `function_276d0` makes for the
   eleven `D3DDevice_SetTextureStageState(0, ...)` calls of its source
   (`src/unknown_023540.cpp:1011`–`1021`: ADDRESSU, ADDRESSV and ADDRESSW
   `D3DTADDRESS_CLAMP`, MAGFILTER and MINFILTER `D3DTEXF_POINT`, and six states
   0), to the same D3D words with the same values. The order differs: here the
   two stores of 1 (`0x3a80d`, `0x3a813`) come before the three stores of 3
   (`0x3a820`–`0x3a82c`), and `function_276d0` writes the three first
   (`0x2772f`–`0x2773b`) and the two after (`0x27741`, `0x27747`). The last
   seven stores are in the same order in both: six of 0 and one of a D3D word
   that both functions OR with 1 eleven times first (`0x3a7e8`–`0x3a806`,
   `0x3a850`). `function_276d0` is `todo`, so its source is not proven.
3. The inline ZENABLE (`0x3a8b4`–`0x3a931`) is the body of the D3D library's
   `0x3f54f0`, which `function_0222d0` reaches for that state, specialised for
   a value of zero. It writes two push-buffer methods (`0x3a8d6`–`0x3a8f2`),
   first calling `0x3faae0` when the write pointer has reached the buffer's
   limit (`0x3a8c0`–`0x3a8c5`), and stores the new value. When the old value
   was 2 (`0x3a8f9`, `0x3a902`) it calls `0x3f6840`, `0x3f6f40`, `0x3f6390` and
   `0x3f6190`, as `0x3f54f0` does (`0x3f555b`–`0x3f5583`).
4. The other inline states write their method and value through `0x3f5cf0`
   (COLORWRITEENABLE, ALPHABLENDENABLE, ALPHATESTENABLE and ZWRITEENABLE:
   `0x3a861`, `0x3a879`, `0x3a891`, `0x3a940`) or push the value for `0x3f53d0`
   (CULLMODE, `0x3a8a3`), `0x3f5590` (STENCILENABLE, `0x3a8af`) and `0x3f59b0`
   (ZBIAS, `0x3a961`), the routines `function_0222d0` uses for those states.
5. `function_1cc30(12)`, with the index in `edx` (`0x3a975`), passes the tag of
   shader slot 12 (in the array of 8-byte slots, tag at `+4`, that the dword at
   `g_485a80 +0x5c` points to) to `function_1c590` with `g_51f0f0` and then to
   `function_1cb70` (`src/unknown_01cf50.cpp:1004`); its result is not read.
   `function_1c710` is the "Apply pending render state" of
   `docs/flexible_surface.md`.

### The common program (`0x3a989`–`0x3aae5`)

1. n = (long)floor(start × 65536.0) (`0x3a989`–`0x3a9a2`). lo is n & 0xff and
   hi is n >> 8, an arithmetic shift; when hi is above 0xff, both become 0xff
   (`0x3a9ab`–`0x3a9dc`). When "rescaled" is set, c is 5 and e is 4; otherwise
   c is 4 and e is 0x18 (`0x3a9b2`–`0x3a9d3`).
2. It clears `g_484f68` (`0x3a9ee`–`0x3a9f8`) and writes the stages 0 to 3 that
   every mode uses:

```c
g_484f68.PSTextureModes = 1;                    /* 0x3aa40 */
g_484f68.PSCombinerCount = 0x11108;             /* 0x3aa4a */
g_484f68.PSRGBInputs[0] = 0x8010802;            /* 0x3aa68 */
g_484f68.PSRGBOutputs[0] = 0x3045;              /* 0x3aa72 */
g_484f68.PSConstant0[0] = 0xff0000;             /* 0x3aa54 */
g_484f68.PSConstant1[0] = 0xff00;               /* 0x3aa5e */
g_484f68.PSRGBInputs[1] = c << 24 | 0x201140;   /* 0x3aa18 */
g_484f68.PSRGBOutputs[1] = 0xc00;               /* 0x3aa7c */
g_484f68.PSAlphaInputs[1] = c << 24 | 0x209140; /* 0x3aa23 */
g_484f68.PSAlphaOutputs[1] = 0xc00;             /* 0x3aa82 */
g_484f68.PSConstant0[1] = lo << 24;             /* 0x3aa12 */
g_484f68.PSRGBInputs[2] = 0x2020;               /* 0x3aa8e */
g_484f68.PSRGBOutputs[2] = 0x4d00;              /* 0x3aa94 */
g_484f68.PSAlphaInputs[2] = e << 24 | 0x201140; /* 0x3aa9e */
g_484f68.PSAlphaOutputs[2] = 0xd00;             /* 0x3aaa4 */
g_484f68.PSConstant0[2] = hi << 24;             /* 0x3aa88 */
g_484f68.PSRGBInputs[3] = 0xcc202d20;           /* 0x3aab3 */
g_484f68.PSRGBOutputs[3] = 0xc00;               /* 0x3aabd */
g_484f68.PSAlphaInputs[3] = 0xdd202df1;         /* 0x3aac3 */
g_484f68.PSAlphaOutputs[3] = 0xd00;             /* 0x3aacd */
g_484f68.PSConstant0[3] = 0x1000000;            /* 0x3aaa9 */
```

3. k = 0 (`0x3aa3b`). When slope is above 1.0 (`0x3aa3d`, `0x3aad2`), the loop
   at `0x3aad4`–`0x3aae5` raises k while slope is above (real)(1 << k). So k is
   0 for a slope up to 1.0, and otherwise the least k with slope ≤ 2^k: at most
   9 in mode 0, 6 in mode 1 and 7 in modes 2 and 3, and always 0 when arg4 is
   outside 0 to 3.

Stages 4 to 7, `PSAlphaInputs[0]`, `PSAlphaOutputs[0]` and the final combiner
are left to the mode. With arg4 outside 0 to 3, the chain goes straight to
`0x3b817`, and they stay 0.

### Mode 0 (`0x3aae7`–`0x3ae56`)

1. f = (arg1 == −1) is kept as a byte in the arg6 slot (`0x3aaf3`–`0x3aafd`).
2. When f is set and arg8 is clear (`0x3ab01`, `0x3ab07`), it sets these states
   (`0x3ab13`–`0x3ab85`):

```c
function_0222d0(D3DRS_ALPHABLENDENABLE, 1);
function_0222d0(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
function_0222d0(D3DRS_DESTBLEND, D3DBLEND_ONE);
function_0222d0(D3DRS_BLENDOP, D3DBLENDOP_ADD);
function_0222d0(D3DRS_ZENABLE, 2);
function_0222d0(D3DRS_ZWRITEENABLE, 0);
function_0222d0(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
function_0222d0(D3DRS_ZBIAS, 0);                /* inline */
```

3. When f is set, `PSTextureModes = 0x21`, `PSAlphaInputs[0] = 0x19140000` and
   `PSAlphaOutputs[0] = 0x90` (`0x3ab98`–`0x3abac`); otherwise they stay 1, 0
   and 0.
4. t = pin((((1.0 / A) / Bf) / (slope / (real)(1 << k))) × a0), in that order
   (`0x3ac84`–`0x3accd`), with A = 4.0 when k > 8, 2.0 when k > 7 and 1.0
   otherwise, and Bf = 2.0 when k > 0 and 1.0 otherwise (`0x3ac50`–`0x3ac8a`).
   The arg5 slot gets (real)(1 << k) (`0x3aca2`) and the arg7 slot t. A and Bf
   change at the same values of k as S(8, 7) and S0 below (and in mode 1 A
   changes with S(5, 4)), so they appear to undo what those bits select
   (inferred).
5. The stages:

```c
g_484f68.PSConstant0[4] = 0x80000000;                    /* 0x3abbe */
g_484f68.PSRGBInputs[4] = 0x1d201d00 | R(3);             /* 0x3abd4 */
g_484f68.PSRGBOutputs[4] = S(2, 1);                      /* 0x3ac03 */
g_484f68.PSAlphaInputs[4] = 0xdd201120;                  /* 0x3ac08 */
g_484f68.PSAlphaOutputs[4] = 0xc00;                      /* 0x3ac12 */
g_484f68.PSRGBInputs[5] = 0x2020;                        /* 0x3ac18 */
g_484f68.PSRGBOutputs[5] = 0x4400;                       /* 0x3ac1e */
g_484f68.PSAlphaInputs[5] = 0xd200d00 | R(6);            /* 0x3ac35 */
g_484f68.PSAlphaOutputs[5] = S(5, 4);                    /* 0x3ac63 */
g_484f68.PSConstant1[6] = function_131fc0(t);            /* 0x3ace3 */
g_484f68.PSConstant0[6] = 0x7f000000;                    /* 0x3ace8 */
g_484f68.PSRGBInputs[6] = 0x1d201220;                    /* 0x3acf2 */
g_484f68.PSRGBOutputs[6] = S(8, 7);                      /* 0x3ad26 */
g_484f68.PSAlphaInputs[6] = 0x18201120;                  /* 0x3ad2f */
g_484f68.PSAlphaOutputs[6] = 0xc00;                      /* 0x3ad39 */
g_484f68.PSConstant1[7] = w;                             /* 0x3ade6 */
g_484f68.PSRGBOutputs[7] = 0x4500;                       /* 0x3adec */
g_484f68.PSAlphaInputs[7] = 0xd120c02;                   /* 0x3adf6 */
g_484f68.PSRGBInputs[7] = rescaled ? 0x2020 : 0x20;      /* 0x3ae12 */
g_484f68.PSAlphaOutputs[7] = S0;                         /* 0x3ae24 */
g_484f68.PSFinalCombinerConstant0 = function_131fc0(a0); /* 0x3ae2e */
g_484f68.PSFinalCombinerInputsABCD = 0x41d1105;          /* 0x3ae3c */
g_484f68.PSFinalCombinerInputsEFG = f ? 0x1900 : 0x2000; /* 0x3ae51 */
```

Here w divides by the real in the arg5 slot (`0x3ad2b`), and the first floor of
each byte calls `_floor` on the product as the x87 unit holds it (`0x3ad46`,
`0x3ad99`). Every other floor, here and in the other modes, is `function_2b480`
on the product stored as a real in the arg7 slot.

### Mode 1 (`0x3ae5b`–`0x3b291`)

1. With p = arg1, colour A = {pin(`+0x28`), pin(`+0x1c` × `+0x28`), pin(`+0x20`
   × `+0x28`), pin(`+0x24` × `+0x28`)} and colour B = {pin(`+0x68`),
   pin(`+0x5c`), pin(`+0x60`), pin(`+0x64`)}, each as {alpha, red, green, blue}
   and each offset a real of p (`0x3ae64`–`0x3afb4`).
2. Unless arg8 is set (`0x3afba`), it sets these states (`0x3afc6`–`0x3b047`):

```c
function_0222d0(D3DRS_ALPHABLENDENABLE, 1);
function_0222d0(D3DRS_SRCBLEND, D3DBLEND_CONSTANTCOLOR);
function_0222d0(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
function_0222d0(D3DRS_BLENDOP, D3DBLENDOP_ADD);
function_0222d0(D3DRS_BLENDCOLOR, 0xffffff);
function_0222d0(D3DRS_ZENABLE, 2);
function_0222d0(D3DRS_ZWRITEENABLE, 0);
function_0222d0(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
function_0222d0(D3DRS_ZBIAS, 0);                /* inline */
```

With the render-state block before them, these leave every state that
`function_26520` sets (`src/unknown_023540.cpp:902`–`914`) at the value it sets
there, after the same `function_14bc0` call (line 901). The exit may still set
ZENABLE to 0 afterwards, when arg9 is not NULL, arg8 is clear and the real at
`0x485658` is not below v, which `0x25e50` allows. `0x25d50` registers
`function_26520` in place of `0x25e50` when `g_485a75` is clear, `g_48574c` is
2 or 3 and the byte at `0x485860` is clear.

3. t is computed as in mode 0, but with A = 4.0 when k > 5, 2.0 when k > 4 and
   1.0 otherwise (`0x3b0a1`–`0x3b0f7`), and kept in the arg6 slot
   (`0x3b108`–`0x3b11b`); the arg5 slot gets (real)(1 << k) (`0x3b100`). The
   stages:

```c
g_484f68.PSAlphaInputs[0] = 0x8200000;                   /* 0x3b065 */
g_484f68.PSAlphaOutputs[0] = 0x40;                       /* 0x3b06f */
g_484f68.PSAlphaInputs[4] = 0x1d201d00 | R(3);           /* 0x3b086 */
g_484f68.PSAlphaOutputs[4] = S(2, 1);                    /* 0x3b0b4 */
g_484f68.PSConstant1[5] = function_131fc0(t);            /* 0x3b131 */
g_484f68.PSAlphaInputs[5] = 0x1d201220;                  /* 0x3b136 */
g_484f68.PSAlphaOutputs[5] = S(5, 4);                    /* 0x3b162 */
g_484f68.PSConstant0[6] = pack_color4f(&colour_a);       /* 0x3b179 */
g_484f68.PSConstant1[6] = w;                             /* 0x3b21d */
g_484f68.PSRGBInputs[6] = 0x1341134;                     /* 0x3b223 */
g_484f68.PSRGBOutputs[6] = 0x45;                         /* 0x3b22d */
g_484f68.PSAlphaInputs[6] = 0x1d120c02;                  /* 0x3b237 */
g_484f68.PSAlphaOutputs[6] = S0;                         /* 0x3b24d */
g_484f68.PSConstant0[7] = pack_color4f(&colour_b);       /* 0x3b25c */
g_484f68.PSAlphaInputs[7] = 0x11141d05;                  /* 0x3b261 */
g_484f68.PSAlphaOutputs[7] = 0xc00;                      /* 0x3b26b */
g_484f68.PSFinalCombinerConstant0 = pack_color4f(&colour_b); /* 0x3b812 */
g_484f68.PSFinalCombinerInputsABCD = 0x114000f;          /* 0x3b27d */
g_484f68.PSFinalCombinerInputsEFG = 0x1d041c00;          /* 0x3b287 */
```

`pack_color4f` is called twice on colour B (`0x3b252`, `0x3b275`); the second
result is stored after the jump to `0x3b812`. w divides by the real in the arg5
slot (`0x3b17e`). `PSTextureModes` stays 1, and of stages 4 to 7 only stage 6
has RGB values.

### Mode 2 (`0x3b296`–`0x3b548`)

1. Unless arg8 is set (`0x3b29f`), it sets the same nine states as mode 1
   (`0x3b2ab`–`0x3b32c`).
2. With r0 and r1 the reals at arg1 `+0` and `+4`:

```c
g_484f68.PSTextureModes = 0x21;                          /* 0x3b337 */
g_484f68.PSConstant0[4] = rescaled ? 0x7f0000ff : 0;     /* 0x3b350 */
g_484f68.PSRGBInputs[4] = 0x1d201d00 | R(3);             /* 0x3b362 */
g_484f68.PSRGBOutputs[4] = S(2, 1);                      /* 0x3b389 */
g_484f68.PSAlphaInputs[4] = 0x18011120;                  /* 0x3b38e */
g_484f68.PSAlphaOutputs[4] = 0xc00;                      /* 0x3b398 */
g_484f68.PSConstant0[5] = function_131fc0(r0);           /* 0x3b3aa */
g_484f68.PSConstant1[5] = function_131fc0(r1);           /* 0x3b3c3 */
g_484f68.PSRGBInputs[5] = 0x39111912;                    /* 0x3b3c8 */
g_484f68.PSRGBOutputs[5] = 0xd00;                        /* 0x3b3d2 */
g_484f68.PSAlphaInputs[5] = 0xd200d00 | R(6);            /* 0x3b3e9 */
g_484f68.PSAlphaOutputs[5] = S(5, 4);                    /* 0x3b41f */
g_484f68.PSConstant1[6] = w;                             /* 0x3b4c2 */
g_484f68.PSAlphaInputs[6] = 0x1d120c02;                  /* 0x3b4c8 */
g_484f68.PSAlphaInputs[7] = 0x1d0d0d20;                  /* 0x3b4d2 */
g_484f68.PSAlphaOutputs[7] = 0x4d00;                     /* 0x3b4dc */
g_484f68.PSAlphaOutputs[6] = S0;                         /* 0x3b4f2 */
```

Here w divides by the integer 1 << k, which the arg6 slot holds (`0x3b424`,
`fidiv` at `0x3b428`); mode 3 does the same.

3. When the byte at arg1 `+0x14` is set (`0x3b4f7`): unless arg8 is set
   (`0x3b4fe`), it calls `function_14bc0(3, 0, true)` (`0x3b50c`),
   `function_0222d0(D3DRS_COLORWRITEENABLE, 1)` (`0x3b51b`) and
   `function_0222d0(D3DRS_ALPHABLENDENABLE, 0)` (`0x3b527`). Then
   `PSFinalCombinerInputsABCD = 0x1d` and `PSFinalCombinerInputsEFG = 0`
   (`0x3b52c`, `0x3b536`), and `PSFinalCombinerConstant0` stays 0.
4. Otherwise it joins mode 3's end with arg1 + 8 (`0x3b545`, `0x3b548`), so
   `PSFinalCombinerConstant0` packs the copy of `g_4857bc`. `function_276d0`,
   which `0x25dc0` registers instead of `0x26e50`, packs `g_4857bc` itself
   there, with `PSFinalCombinerInputsABCD = 0x1c010000` and
   `PSFinalCombinerInputsEFG = 0x1c00` (`src/unknown_023540.cpp:1039`–`1041`);
   the end of mode 3 has `0x1d` in place of `0x1c`.

### Mode 3 (`0x3b54d`–`0x3b812`)

1. Unless arg8 is set (`0x3b556`), it sets the same nine states as mode 1
   (`0x3b562`–`0x3b5e3`).
2. With p = arg1 as a byte pointer and q the byte at p (`0x3b612`, `0x3b699`):

```c
g_484f68.PSTextureModes = 0x8421;                        /* 0x3b5f0 */
g_484f68.PSConstant0[4] = pack_color3f(p + 4);           /* 0x3b603 */
g_484f68.PSConstant1[4] = pack_color3f(p + 0x10);        /* 0x3b60d */
g_484f68.PSRGBOutputs[4] = 0x3045;                       /* 0x3b61b */
g_484f68.PSRGBInputs[4] = q ? 0x19011a02 : 0x09010a02;   /* 0x3b64d */
g_484f68.PSAlphaInputs[4] = 0x1d201d00 | R(3);           /* 0x3b660 */
g_484f68.PSAlphaOutputs[4] = S(2, 1);                    /* 0x3b683 */
g_484f68.PSConstant0[5] = pack_color3f(p + 0x1c);        /* 0x3b694 */
g_484f68.PSRGBOutputs[5] = 0x1045;                       /* 0x3b69d */
g_484f68.PSRGBInputs[5] = q ? 0x24251b01 : 0x24250b01;   /* 0x3b6c0 */
g_484f68.PSAlphaInputs[5] = 0x1d201d00 | R(6);           /* 0x3b6d3 */
g_484f68.PSAlphaOutputs[5] = S(5, 4);                    /* 0x3b700 */
g_484f68.PSConstant1[6] = w;                             /* 0x3b7b1 */
g_484f68.PSRGBInputs[6] = 0x4250000;                     /* 0x3b7b7 */
g_484f68.PSRGBOutputs[6] = 0xd0;                         /* 0x3b7c1 */
g_484f68.PSAlphaInputs[6] = 0x1d120c02;                  /* 0x3b7c6 */
g_484f68.PSAlphaInputs[7] = 0x1d2d0000;                  /* 0x3b7d0 */
g_484f68.PSAlphaOutputs[6] = S0;                         /* 0x3b7e7 */
g_484f68.PSAlphaOutputs[7] = 0xd0;                       /* 0x3b7ed */
/* the end that mode 2 shares, 0x3b7f5-0x3b812, with x = p + 0x28 */
g_484f68.PSFinalCombinerConstant0 = pack_color3f(x);     /* 0x3b812 */
g_484f68.PSFinalCombinerInputsABCD = 0x1d010000;         /* 0x3b7fe */
g_484f68.PSFinalCombinerInputsEFG = 0x1d00;              /* 0x3b808 */
```


### Binding and the start (`0x3b817`–`0x3b89b`)

1. `function_1ccf0(&g_484f68)`, with the pointer in `edx` (`0x3b817`,
   `0x3b81c`), binds the program; its result is not read.
2. When arg9 is not NULL (`0x3b828`): v = `g_48565c` × start
   (`0x3b830`–`0x3b838`), times 1/256 when "rescaled" is set (`0x3b840`). When
   the real at `0x485658` is not below v (`0x3b850`, `0x3b853`; an unordered
   compare keeps v), v becomes that real (`0x3b869`), and unless arg8 is set
   (`0x3b855`) it first calls `function_0222d0(D3DRS_ZENABLE, 0)` (`0x3b864`).
   Then *arg9 = v (`0x3b871`).
3. It returns true (`0x3b877`, `ret 0x24`). The false exit at `0x3b880` pops
   the two registers saved by then, `edi` and `ebx`, and returns 0 (`0x3b881`).

The jump table (`0x3b88c`–`0x3b89b`) holds `0x3a61f`, `0x3a62d`, `0x3a63b` and
`0x3a63f`, for modes 0 to 3. Mode 2's entry allows the rescale and joins mode
3's at `0x3a641`.

## Callees without source

The inventory lists these as C library (`xdk:libcmt`) and D3D (`xdk:d3d8`)
code.

- `0x321099` (`_floor`) takes a double on the stack, which the caller removes
  (`0x3a6b9`, `0x3a99f`, `0x3ad4b`, `0x3ad9e`), and returns its floor in `st0`.
  It is called five times (`0x3a68b`, `0x3a6b4`, `0x3a99a`, `0x3ad46`,
  `0x3ad99`). `0x322b0c` (`__ftol2`) turns `st0` into an integer in `eax`; it
  follows every floor, 27 calls in all.
- `0x3f5cf0` writes a push-buffer method (`ecx`) and its value (`edx`) and
  returns with a plain `ret`. `function_0222d0` calls it for simple states
  (`0x222e5`).
- `0x3f53d0`, `0x3f5590` and `0x3f59b0` take the value on the stack (`ret 4`).
  Matched code reaches them for CULLMODE (`0x49732`), STENCILENABLE (`0x4a793`)
  and ZBIAS (`0x496e6`). Here they are called at `0x3a8a3`, at `0x3a8af`, and
  at `0x3a961`, `0x3ab85`, `0x3b047`, `0x3b32c` and `0x3b5e3`.
- `0x3faae0` is called when the push buffer's write pointer has reached its
  limit (`0x3a8c0`–`0x3a8c5`, `0x3a90e`–`0x3a913`). It takes one value in `eax`
  and one on the stack (`ret 4`), and returns a write pointer in `eax`
  (`0x3a8d1`, `0x3a91f`).
- `0x3f6840`, `0x3f6f40`, `0x3f6390` and `0x3f6190` are called only on the
  inline ZENABLE's path for an old value of 2 (`0x3a904`–`0x3a92c`). `0x3f6840`
  takes nothing this function sets. `0x3f6f40` and `0x3f6190` use the device
  pointer in `esi` (`0x3a8b4`), and `0x3f6190` also a push-buffer pointer in
  `eax`, returning the new one. `0x3f6390` takes a push-buffer pointer in `edx`
  and the device on the stack (`ret 4`), writes one method and returns the
  pointer past it.

## Existing declarations

Nothing declares, stubs or calls `0x3a5c0` in the source at `9feed5f`, and its
four callers have no source. Retail fits a `bool` result and nine stack
parameters, popped by the callee: a pointer that mode 0 compares with −1, two
`real`s, a `long`, four `bool`s read as bytes, and a `real *`. Names such as
center_if_capped, anchor_end_if_capped, keep_start_if_capped and program_only
would fit arg5 to arg8 (inferred), but arg6 and arg7 also decide whether modes
other than 0 and 1 accept a negative start.

The callees with source are defined with the parameters retail passes:

| Retail | Declared as | Where | What retail shows |
| --- | --- | --- | --- |
| `0x14bc0` | `void function_14bc0(short index, short element, bool use_depth)` | `src/unknown_01cf50.cpp:421` | index and element on the stack, use_depth in `al` (`ret 8`) |
| `0x14f60` | `void function_14f60(short stage, short index)` | `src/unknown_01cf50.cpp:506` | stage in `bx`, index in `di`; the function is `todo` |
| `0x1c710` | `void __stdcall function_1c710(void *memory)` | `src/unknown_01cf50.cpp:860` | `ret 4`; other files declare it with `void *state`, and `include/flexible_surface_calls.h:13` with `void *` |
| `0x1cc30` | `dword function_1cc30(long index)` | `src/unknown_01cf50.cpp:1004` | index in `edx` |
| `0x1ccf0` | `bool function_1ccf0(D3DPIXELSHADERDEF const *program)` | `src/unknown_01cf50.cpp:634` | program in `edx`; true in `al` |
| `0x222d0` | `void function_0222d0(D3DRENDERSTATETYPE state, dword value)` | `src/unknown_020560.cpp:611` | state in `esi`, value in `edi` |
| `0x2b480` | `double __stdcall function_2b480(real value)` | `src/unknown_02b400.cpp:292` | `ret 4`, the floor of value |
| `0x131e50` | `dword __cdecl pack_color4f(const color4f *color)` | `src/unknown_131e50.cpp:8` | as declared |
| `0x131ed0` | `dword __cdecl pack_color3f(const color3f *color)` | `src/unknown_131e50.cpp:46` | as declared |
| `0x131fc0` | `dword __cdecl function_131fc0(real alpha)` | `src/unknown_131e50.cpp:115` | as declared. It converts alpha × 255.0 with `fistp`, which rounds by the FPU's mode instead of truncating, and shifts the result into the top byte |

The render-state block calls `function_0222d0` only for ZFUNC and
DEPTHCLIPCONTROL and sets its other states inline, and every mode calls it for
each state but ZBIAS. A matching source may still write only `function_0222d0`
calls and leave the inlining to the compiler (inferred, not tested). The source
of `function_1e930` (`src/unknown_01cf50.cpp:2636`, `todo`) writes only such
calls, and its retail code inlines most of them but keeps two calls (`0x1ea94`,
`0x1eaa0`); `0x25e50`, `0x26e50` and `0x37a60` keep some too. `function_276d0`
and `function_26520` inline every one of theirs. The matched code at
`src/unknown_0494b0.cpp:138`–`169` instead writes the `g_4b81fc` slot under its
own name and calls `D3DDevice_SetRenderState` directly.

## Evidence

- The function and its callees were disassembled from the retail XBE with
  capstone 5.0.9, through `tools/xbe.py`. Conventions, stack slots, offsets and
  constants were read from retail code and data, with a stack-depth trace over
  every path that found no conflicts, and the callees with source were read
  from it. A reader analysed the function, and a verifier checked each claim
  against its own disassembly and trace: 29 confirmed and 6 corrected, with the
  corrections applied here. A third agent wrote this text from that work, and a
  fourth checked the finished document against retail and the repository: 520
  claims confirmed and 6 corrected or reworded, with the changes applied here.
- Callers come from an image-wide scan for `E8` and `E9` calls and jumps,
  `0F 80`–`8F` branches and absolute dwords in every section. The four calls
  above are the only references to the function, and no dword equals `0x3a5c0`.
  The only absolute references into the range are the function's own: the
  operand of the jump at `0x3a618`, which addresses the jump table, and the
  table's four entries; other dwords whose values fall in the range are parts
  of instructions or entries of numeric tables, not pointers.
- Constants are read from retail `.rdata`: `0x45dbc0` (1.0), `0x45e440`
  (512.0), `0x45dc90` (64.0), `0x45dd60` (128.0), `0x45dd5c` (256.0),
  `0x45dcb0` (65536.0), `0x45dcac` (1/65536), `0x45dbbc` (0.5), `0x45dc0c`
  (2.0), `0x45dc10` (4.0) and `0x45dfa0` (1/256). These are all of the
  function's `.rdata` reads.
- No document covered the function before this one. Source line numbers are at
  `9feed5f`.
- No SDK or outside dataset was used. Names are the repository's own, or
  describe behaviour.
