# Two-axis turn of a global matrix (unknown_039e50)

Retail range covered: `0x39e50`–`0x3a542`. This is one inventory entry,
1,779 retail bytes, `todo` with no source. Each tick it turns the three
vectors of the 3×3 matrix at `0x467104` about two axes, by 0.5 × the time
step about the vector at `0x485624` and then by 0.3 × the time step about
the vector at `0x485630`. **Analysis only:** this document adds no source,
and nothing in it has been built or checked against retail with the
original compiler. Names are provisional. It is written for lane W, whose
claim covers the function, and makes no claim of its own.

The function is straight-line code with no branches and no calls. A
symbolic replay of it recovered the nine results, and the C under
[Retail's evaluation](#retails-evaluation) reproduces retail's expression
trees exactly (see [Evidence](#evidence)).

## Boundary

- `0x39a80`, just before the range, and `0x3a550`, just after it, are `todo`
  with no source. Both are excluded.
- The range is inside lane W's row of the Active claims table (issue #9,
  `0x011000`–`0x04ffff`). No open pull request touches it.
- `0x39e50` has no `@retail` or `@stub` marker, declaration or stub at
  `e02c566`, and neither have the three globals it uses.

## Conventions

| Retail | Bytes | Arguments | Returns | Callers | What it does |
| --- | --- | --- | --- | --- | --- |
| `0x39e50` | 1779 | stack: `real dt` (`ret 4`) | | 1 | Turns the matrix at `0x467104` about the axes at `0x485624` and `0x485630` |

## Data

- `0x467104`: nine reals, a 3×3 matrix whose rows `m[0]`–`m[2]`,
  `m[3]`–`m[5]` and `m[6]`–`m[8]` are its three vectors. The image holds
  the identity. No other function refers to it directly.
- `0x485624` and `0x485630`: two vectors of three reals (written `a` and `b`
  below), zero in the image. Fifteen functions refer to `0x485624`, among
  them `0x1e4e0`, `0x1e5e0` and `0x27aa0`; `0x485630` is referred to only by
  `0x224fd0`, `0x248450`, `0x248df0` and `0x39e50`.
- Constants from `.rdata`: `0x45dbbc` (0.5), `0x44ae90` (0.3) and
  `0x45dbc0` (1.0).

## The function

### What it computes

With R(u, θ) = cos θ I + (1 − cos θ) u uᵀ + sin θ [u]×, the rotation by θ
about u in the right-handed sense, each row v of the matrix becomes

    B (A v),  A = R(a, 0.5 dt),  B = R(b, 0.3 dt)

that is, the matrix becomes M Aᵀ Bᵀ. The axes are used as stored; nothing
here normalizes them.

### The caller

`0x137ed0` is the only caller. When its time step (its stack argument) is
positive, it runs several updates, stores the step at `0x485b20` and calls
`0x39e50` with it. In every case it then calls `0x176f60` and adds the step
to the double `g_4ba040`.

### Retail's evaluation

`dt * 0.5f` and `dt * 0.3f` are formed on the x87 stack and passed to
`fsin` and `fcos` without rounding; each sine and cosine is rounded to a
real when it is stored. Everything else is SSE. Each statement keeps
retail's evaluation: `a * b * c` is (a × b) × c, and parentheses mark every
other grouping. The `t` names hold values that retail computes once and
reuses, declared in the order retail computes them. `a.i`, `a.j` and `a.k`
are the three reals at `0x485624`, `b` likewise at `0x485630`, and `m[0]`
to `m[8]` are the reals at `0x467104`, read before any is written.

```c
real sine_a = (real)sin(dt * 0.5f);
real cosine_a = (real)cos(dt * 0.5f);
real sine_b = (real)sin(dt * 0.3f);
real cosine_b = (real)cos(dt * 0.3f);
real t0 = a.i * a.i;
real t1 = a.j * a.j;
real t2 = a.k * a.k;
real t3 = a.i * sine_a;
real t4 = a.k * sine_a;
real t5 = a.j * sine_a;
real t6 = 1.0f - cosine_a;
real t7 = (1.0f - t0) * cosine_a + t0;
real t8 = t6 * a.j * a.i;
real t9 = t8 + t4;
real t10 = t8 - t4;
real t11 = t6 * a.k * a.i;
real t12 = t6 * a.k * a.j;
real t13 = (1.0f - t1) * cosine_a + t1;
real t14 = t11 - t5;
real t15 = (1.0f - t2) * cosine_a + t2;
real t16 = t11 + t5;
real t17 = t12 + t3;
real t18 = b.i * b.i;
real t19 = t12 - t3;
real t20 = b.j * b.j;
real t21 = b.k * b.k;
real t22 = b.i * sine_b;
real t23 = b.j * sine_b;
real t24 = b.k * sine_b;
real t25 = (1.0f - t18) * cosine_b + t18;
real t26 = 1.0f - cosine_b;
real t27 = t26 * b.j * b.i;
real t28 = t27 - t24;
real t29 = t27 + t24;
real t30 = (1.0f - t20) * cosine_b + t20;
real t31 = t26 * b.k * b.i;
real t32 = t31 + t23;
real t33 = t31 - t23;
real t34 = (1.0f - t21) * cosine_b + t21;
real t35 = t26 * b.k * b.j;
real t36 = t35 + t22;
real t37 = t35 - t22;
real t38 = t16 * m[2] + t10 * m[1] + t7 * m[0];
real t39 = t19 * m[2] + t13 * m[1] + t9 * m[0];
real t40 = t15 * m[2] + t17 * m[1] + t14 * m[0];
real t41 = t16 * m[5] + t10 * m[4] + t7 * m[3];
real t42 = t19 * m[5] + t13 * m[4] + t9 * m[3];
real t43 = t15 * m[5] + t17 * m[4] + t14 * m[3];
real t44 = t16 * m[8] + t10 * m[7] + t7 * m[6];
real t45 = t19 * m[8] + t13 * m[7] + t9 * m[6];
real t46 = t15 * m[8] + t17 * m[7] + t14 * m[6];

m[0] = t32 * t40 + t28 * t39 + t25 * t38;
m[1] = t37 * t40 + t30 * t39 + t29 * t38;
m[2] = t34 * t40 + t36 * t39 + t33 * t38;
m[3] = t32 * t43 + t28 * t42 + t25 * t41;
m[4] = t37 * t43 + t30 * t42 + t29 * t41;
m[5] = t34 * t43 + t36 * t42 + t33 * t41;
m[6] = t32 * t46 + t28 * t45 + t25 * t44;
m[7] = t37 * t46 + t30 * t45 + t29 * t44;
m[8] = t34 * t46 + t36 * t45 + t33 * t44;
```

## Existing declarations

At `e02c566` nothing declares, stubs or calls `0x39e50` in the source, and
the globals `0x467104`, `0x485624`, `0x485630` and `0x485b20` are not
declared. `g_4ba040`, the caller's time total, is defined in
`src/unknown_02b5a0.cpp`.

## Evidence

- `0x39e50` was disassembled from the retail XBE with capstone 5.0.9,
  through `tools/xbe.py`, at `e02c566`: 338 instructions with no branches
  and no calls, x87 only for the two angles and their sines and cosines.
- A Python replay executed every instruction symbolically, tracking the
  registers, the frame and the x87 stack, and took the nine stores to
  `0x467104`–`0x467124` as expression trees. The C above was printed from
  those trees and parsed back: all 60 statements give the same trees.
- Numerically, in double precision over 30 random time steps, unit axes
  and matrices, the result equals M Aᵀ Bᵀ to 8e-8. The orders M Bᵀ Aᵀ,
  M A B and M B A do not fit.
- `0x137ed0` calls `0x39e50` at `0x137f42`, after `push ebx`, the step it
  received; it stores the same step at `0x485b20` just before.
- No emulator, runtime testing, SDK or outside dataset was used. Names are
  the repository's own, or describe behaviour.
