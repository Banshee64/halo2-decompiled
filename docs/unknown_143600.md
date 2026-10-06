# Spherical harmonics rotation (unknown_143600)

Retail range covered: `0x143600`–`0x146014`. This is one inventory entry,
10,773 retail bytes, the largest game function that is not matched. It
rotates the coefficients of a real spherical harmonics expansion, in the
basis that `spherical_harmonics_evaluate_direction` evaluates, by a
`matrix3x3`. **Analysis only:** this document adds no source, and nothing
in it has been built or checked against retail with the original compiler.
Names are provisional. It is written for lane N, whose claim covers the
function, and makes no claim of its own.

Almost all of the function is three closed-form matrices, 3×3, 5×5 and
7×7, whose elements are polynomials of degree 1, 2 and 3 in the nine reals
of the rotation. A symbolic replay of retail's straight-line code recovered
every element, and the C under [The band matrices](#the-band-matrices)
reproduces retail's expression trees exactly (see [Evidence](#evidence)).

## Boundary

- `0x143360`, just before the range, has source in
  `src/spherical_harmonics.cpp` (`spherical_harmonics_evaluate_direction`,
  `todo`). `0x146020`, just after it, is matched in the same file
  (`spherical_harmonics_evaluate_directional_light`). Both are excluded;
  the rotation would sit between them.
- The range is inside lane N's row of the Active claims table (issue #9,
  `0x140000`–`0x14ffff`). No open pull request touches it.
- `0x143600` is `todo`, with no `@retail` or `@stub` marker, declaration
  or stub anywhere at `4c377fb`.
- Its only callers, `0x22850` and `0x23690`, are in lane W's range and
  have no source.

## Conventions

LTCG chooses the registers (see `docs/DECOMPILING.md`), so they say
nothing about the order of the C parameters. "Callers" counts direct
callers; calls through pointers are not counted.

| Retail | Bytes | Arguments | Returns | Callers | What it does |
| --- | --- | --- | --- | --- | --- |
| `0x143600` | 10773 | `eax` rotation (`matrix3x3 const *`), `ecx` input coefficients, `esi` output coefficients; stack: `dword order` (`ret 4`) | the output pointer | 2 | Rotates the first order × order coefficients, for orders 2 to 4 |

## Data

The function reserves `0x300` bytes and saves `ebx`, `ebp` and `edi`. The
top 196 bytes of the frame, from the entry `esp` − `0xc4` up, are one
array of 49 reals: each band writes its matrix there, row-major, and
passes it to `function_143250`. The rest of the frame holds the
compiler's spills. The constants are listed under [Evidence](#evidence).

## The function

### Structure

```c
/* provisional name and parameter order */
real *spherical_harmonics_rotate(matrix3x3 const *rotation, dword order,
    real const *in, real *out)
{
    real r[49];

    out[0] = in[0];
    /* band 1: r[0] to r[8] */
    function_143250(out + 1, 3, r, in + 1);
    if (order > 2)
    {
        /* band 2: r[0] to r[24] */
        function_143250(out + 4, 5, r, in + 4);
        if (order > 3)
        {
            /* band 3: r[0] to r[48] */
            function_143250(out + 9, 7, r, in + 9);
        }
    }
    return out;
}
```

- `out[0] = in[0]` is a dword copy (`mov eax, [edi]`, `mov [esi], eax`).
- There is no test for order 1, so band 1 is always rotated, and orders
  above 4 rotate the same 16 coefficients as order 4.
- The tests are unsigned (`cmp ebp, 2` and `cmp ebp, 3`, each with
  `jbe`), like `dword order` in `spherical_harmonics_evaluate_direction`.
  Both jump to the shared epilogue.
- `function_143250(out, n, a, b)` sets `out[i]` to the sum over `j` of
  `b[j] * a[i * n + j]`, so row `i` of a band's matrix gives output `i`.
- Below, `m[0]` to `m[8]` stand for the nine reals of the `matrix3x3` in
  memory order: `forward.i`, `forward.j`, `forward.k`, `left.i`,
  `left.j`, `left.k`, `up.i`, `up.j` and `up.k`. Retail loads all nine
  during band 1 and keeps copies in the frame.

### What it computes

Read `in` as the coefficients of a function f in the basis of
`spherical_harmonics_evaluate_direction`, with the same order and signs.
The result holds the coefficients of the function whose value in
direction d is f(`forward`·d, `left`·d, `up`·d). Each band's matrix is
orthogonal, and band 0, the constant term, is copied unchanged.

### The callers

Every call site pushes 3, so retail rotates bands 0 to 2 only (nine
coefficients) and never runs band 3, which is most of the function's
bytes.

- `0x22850` calls it three times, at `0x22a6f`, `0x22a8a` and `0x22aa5`,
  on the 9-real arrays at `+0xc`, `+0x30` and `+0x54` of the structure in
  `ebp`, with one rotation built on its stack. Each result goes to a
  stack array and is copied back with `rep movsd` (nine dwords).
- `0x23690` makes the same three calls twice (`0x23a4d`, `0x23a66` and
  `0x23a7f`, then `0x23be9`, `0x23c02` and `0x23c1b`), from three stack
  arrays into three others, with one rotation.

### The band matrices

Each statement keeps retail's evaluation. `a * b * c` is (a × b) × c,
parentheses mark every other grouping, and `-x` is retail's `xorps`
followed by `subss`, which is how the compiler builds a negation in
matched code (for example `-direction` in `function_2054b0`,
`src/vehicles.cpp`). The `s` names are the square-root constants. Names
such as `m3m0` hold a product that retail computes once and reuses, as
`xx = x * x` does in `spherical_harmonics_evaluate_direction`. Retail
stores bands 2 and 3 in index order, apart from `r[16]` to `r[18]` of
band 2 and `r[35]` and `r[36]` of band 3. Band 1's stores are not in
index order.

`s5_15` multiplies the two roots on the x87 stack before rounding.
`s21_35` multiplies the unrounded root of 21 by the rounded `s35` (`fst`,
then `fmul dword`), the pattern of `band3_21 = (real)(sqrt(21.0) * sqrt2)`
in `spherical_harmonics_evaluate_direction`. The other `s` products are
float multiplies.

#### Band 1

```c
r[0] = m[4];
r[1] = -m[7];
r[2] = m[1];
r[3] = -m[5];
r[4] = m[8];
r[5] = -m[2];
r[6] = m[3];
r[7] = -m[6];
r[8] = m[0];
```

#### Band 2

```c
real s5_15 = (real)(sqrt(5.0) * sqrt(15.0));
real m3m0 = m[3] * m[0];
real m4m1 = m[4] * m[1];
real m6m3 = m[6] * m[3];
real m7m4 = m[7] * m[4];
real m6m6 = m[6] * m[6];
real m7m7 = m[7] * m[7];
real m6m0 = m[6] * m[0];
real m7m1 = m[7] * m[1];
real m0m0 = m[0] * m[0];
real m1m1 = m[1] * m[1];
real m3m3 = m[3] * m[3];
real m4m4 = m[4] * m[4];

r[0] = m[4] * m[0] + m[3] * m[1];
r[1] = -(m[7] * m[3]) - m[6] * m[4];
r[2] = s5_15 * m[7] * m[6] * 0.2f;
r[3] = -(m[7] * m[0]) - m[6] * m[1];
r[4] = m[1] * m[0] - m[4] * m[3];
r[5] = -(m[4] * m[2]) - m[5] * m[1];
r[6] = m[7] * m[5] + m[8] * m[4];
r[7] = s5_15 * m[8] * m[7] * -0.2f;
r[8] = m[7] * m[2] + m[8] * m[1];
r[9] = m[5] * m[4] - m[2] * m[1];
r[10] = (m[5] * m[2] * 0.13333334f - m3m0 * 0.06666667f) * s5_15
    - m4m1 * s5_15 * 0.06666667f;
r[11] = m6m3 * s5_15 * 0.06666667f - s5_15 * m[8] * m[5] * 0.13333334f
    + m7m4 * s5_15 * 0.06666667f;
r[12] = m[8] * m[8] - m6m6 * 0.5f - m7m7 * 0.5f;
r[13] = m6m0 * s5_15 * 0.06666667f - s5_15 * m[8] * m[2] * 0.13333334f
    + m7m1 * s5_15 * 0.06666667f;
r[14] = m[2] * m[2] * s5_15 * 0.06666667f
    - (m[5] * m[5] * 0.06666667f + m0m0 * 0.033333335f) * s5_15
    - m1m1 * s5_15 * 0.033333335f + (m4m4 + m3m3) * s5_15 * 0.033333335f;
r[15] = -(m[5] * m[0]) - m[3] * m[2];
r[16] = m[8] * m[3] + m[6] * m[5];
r[17] = s5_15 * m[8] * m[6] * -0.2f;
r[18] = m[8] * m[0] + m[6] * m[2];
r[19] = m[5] * m[3] - m[2] * m[0];
r[20] = m3m0 - m4m1;
r[21] = m7m4 - m6m3;
r[22] = m6m6 * s5_15 * 0.1f - m7m7 * s5_15 * 0.1f;
r[23] = m7m1 - m6m0;
r[24] = (m4m4 + m0m0) * 0.5f - m1m1 * 0.5f - m3m3 * 0.5f;
```

#### Band 3

```c
real s2 = (real)sqrt(2.0);
real s35 = (real)sqrt(35.0);
real s35_2 = s35 * s2;
real s105 = (real)sqrt(105.0);
real s105_35_2 = s105 * s35_2;
real s21 = (real)sqrt(21.0);
real s21_35 = (real)(sqrt(21.0) * s35);
real s7 = (real)sqrt(7.0);
real s105_2 = s105 * s2;
real s7_105 = s7 * s105;
real s105_2_21 = s105_2 * s21;
real s21_2 = s21 * s2;
real s7_2 = s7 * s2;
real s7_2_35 = s7_2 * s35;
real s7_2_21 = s7_2 * s21;
real m3m0 = m[3] * m[0];
real m4m1 = m[4] * m[1];
real m6m3 = m[6] * m[3];
real m6m6 = m[6] * m[6];
real m7m7 = m[7] * m[7];
real m6m0 = m[6] * m[0];
real m0m0 = m[0] * m[0];
real m1m1 = m[1] * m[1];
real m3m3 = m[3] * m[3];
real m4m4 = m[4] * m[4];
real m4m0 = m[4] * m[0];
real m3m1 = m[3] * m[1];
real m6m4 = m[6] * m[4];
real m6m1 = m[6] * m[1];
real m4m3 = m[4] * m[3];
real m1m0 = m[1] * m[0];
real m5m1 = m[5] * m[1];
real m4m2 = m[4] * m[2];
real m5m4 = m[5] * m[4];
real m2m1 = m[2] * m[1];
real m5m2 = m[5] * m[2];
real m8m8 = m[8] * m[8];
real m5m5 = m[5] * m[5];
real m2m2 = m[2] * m[2];
real m3m2 = m[3] * m[2];
real m5m0 = m[5] * m[0];
real m6m5 = m[6] * m[5];
real m6m2 = m[6] * m[2];
real m5m3 = m[5] * m[3];
real m2m0 = m[2] * m[0];
real m3m3m4 = m3m3 * m[4];
real m3m0m1 = m3m0 * m[1];
real m0m0m4 = m0m0 * m[4];
real m1m1m4 = m1m1 * m[4];
real m4m4m4 = m4m4 * m[4];
real m3m1m6 = m3m1 * m[6];
real m4m0m6 = m4m0 * m[6];
real m3m0m7 = m3m0 * m[7];
real m6m6m4 = m6m6 * m[4];
real m7m7m4 = m7m7 * m[4];
real m6m3m7 = m6m3 * m[7];
real m6m0m7 = m6m0 * m[7];
real m6m6m1 = m6m6 * m[1];
real m7m7m1 = m7m7 * m[1];
real m4m3m6 = m4m3 * m[6];
real m1m0m6 = m1m0 * m[6];
real m1m1m1 = m1m1 * m[1];
real m3m0m4 = m3m0 * m[4];
real m4m4m1 = m4m4 * m[1];
real m0m0m1 = m0m0 * m[1];
real m3m3m1 = m3m3 * m[1];
real m3m0m2 = m3m0 * m[2];
real m4m1m2 = m4m1 * m[2];
real m3m0m8 = m3m0 * m[8];
real m3m2m6 = m3m2 * m[6];
real m5m0m6 = m5m0 * m[6];
real m4m1m8 = m4m1 * m[8];
real m5m1m7 = m5m1 * m[7];
real m4m2m7 = m4m2 * m[7];
real m6m3m8 = m6m3 * m[8];
real m8m7 = m[8] * m[7];
real m8m7m4 = m8m7 * m[4];
real m7m7m8 = m7m7 * m[8];
real m6m6m8 = m6m6 * m[8];
real m8m7m1 = m8m7 * m[1];
real m6m0m8 = m6m0 * m[8];
real m2m1m7 = m2m1 * m[7];
real m3m3m8 = m3m3 * m[8];
real m5m3m6 = m5m3 * m[6];
real m0m0m8 = m0m0 * m[8];
real m4m4m8 = m4m4 * m[8];
real m1m1m8 = m1m1 * m[8];
real m2m0m6 = m2m0 * m[6];
real m5m4m7 = m5m4 * m[7];
real m3m0m5 = m3m0 * m[5];
real m4m1m5 = m4m1 * m[5];
real m4m0m1 = m4m0 * m[1];
real m4m4m3 = m4m4 * m[3];
real m0m0m3 = m0m0 * m[3];
real m3m3m3 = m3m3 * m[3];
real m1m1m3 = m1m1 * m[3];
real m3m0m6 = m3m0 * m[6];
real m4m1m6 = m4m1 * m[6];
real m3m1m7 = m3m1 * m[7];
real m4m0m7 = m4m0 * m[7];
real m6m6m3 = m6m6 * m[3];
real m6m4m7 = m6m4 * m[7];
real m7m7m3 = m7m7 * m[3];
real m6m1m7 = m6m1 * m[7];
real m6m6m0 = m6m6 * m[0];
real m4m3m7 = m4m3 * m[7];
real m1m0m7 = m1m0 * m[7];
real m3m1m4 = m3m1 * m[4];
real m7m7m0 = m7m7 * m[0];
real m0m0m0 = m0m0 * m[0];
real m3m3m0 = m3m3 * m[0];
real m4m4m0 = m4m4 * m[0];
real m1m1m0 = m1m1 * m[0];

r[0] = m0m0m4 * 0.75f + m3m0m1 * 1.5f - m3m3m4 * 0.75f - m1m1m4 * 0.75f
    + m4m4m4 * 0.25f;
r[1] = (m4m1 * m[7] * 0.014285714f - m3m1m6 * 0.014285714f) * s105_35_2
    - m4m0m6 * s105_35_2 * 0.014285714f
    - m3m0m7 * s105_35_2 * 0.014285714f;
r[2] = m6m6m4 * s21_35 * 0.035714287f - m7m7m4 * s21_35 * 0.035714287f
    + m6m3m7 * s21_35 * 0.071428575f;
r[3] = (s7 * m7m7 * m[7] * 0.035714287f - s7 * m[7] * m6m6 * 0.10714286f)
    * s35_2;
r[4] = (m6m6m1 * 0.035714287f + m6m0m7 * 0.071428575f) * s21_35
    - m7m7m1 * s21_35 * 0.035714287f;
r[5] = (s105 * m1m1 * m[7] + s105 * m3m3 * m[7]) * s35_2 * 0.007142857f
    + m4m3m6 * s105_35_2 * 0.014285714f - m1m0m6 * s105_35_2 * 0.014285714f
    - s105 * m0m0 * m[7] * s35_2 * 0.007142857f
    - s105 * m4m4 * m[7] * s35_2 * 0.007142857f;
r[6] = m0m0m1 * 0.75f - (m3m0m4 * 1.5f + m1m1m1 * 0.25f) + m4m4m1 * 0.75f
    - m3m3m1 * 0.75f;
r[7] = m4m3 * s105_35_2 * m[5] * 0.014285714f
    - m3m1 * s105_35_2 * m[2] * 0.014285714f
    - m5m0 * s105_35_2 * m[1] * 0.014285714f
    - m4m0 * s105_35_2 * m[2] * 0.014285714f;
r[8] = m3m2 * m[7] + m5m1 * m[6] + m5m0 * m[7] + m4m2 * m[6] + m4m0 * m[8]
    + m3m1 * m[8];
r[9] = m6m4 * s105_2_21 * m[8] * -0.023809524f
    - s105_2_21 * m[8] * m[7] * m[3] * 0.023809524f
    - m6m5 * s105_2_21 * m[7] * 0.023809524f;
r[10] = s7_105 * m[8] * m[7] * m[6] * 0.14285715f;
r[11] = m6m1 * s105_2_21 * m[8] * -0.023809524f
    - s105_2_21 * m[8] * m[7] * m[0] * 0.023809524f
    - m6m2 * s105_2_21 * m[7] * 0.023809524f;
r[12] = m2m0 * m[7] + m2m1 * m[6] + m1m0 * m[8] - m5m4 * m[6] - m5m3 * m[7]
    - m4m3 * m[8];
r[13] = (m3m2 * m[4] + m3m1 * m[5]) * s105_35_2 * 0.014285714f
    - m1m0 * s105_35_2 * m[2] * 0.014285714f
    + m4m0 * s105_35_2 * m[5] * 0.014285714f;
r[14] = (m2m2 * m[4] * 0.028571429f - m0m0m4 * 0.007142857f) * s21_35
    - s21_35 * m1m1m4 * 0.021428572f
    + (m5m1 * m[2] * 0.057142857f + m3m3m4 * 0.007142857f) * s21_35
    - m5m5 * s21_35 * m[4] * 0.028571429f + s21_35 * m4m4m4 * 0.007142857f
    - s21_35 * m3m0m1 * 0.014285714f;
r[15] = s105_2_21 * m4m0m6 * 0.004761905f
    - (s105_2_21 * m5m1 * m[8] + m5m2 * s105_2_21 * m[7]) * 0.01904762f
    - s105_2_21 * m4m2 * m[8] * 0.01904762f
    + ((m3m0m7 + m3m1m6) * 0.004761905f + m4m1 * m[7] * 0.014285714f)
    * s105_2_21;
r[16] = m8m8 * m[4] - m7m7m4 * 0.75f + m8m7 * m[5] * 2.0f - m6m3m7 * 0.5f
    - m6m6m4 * 0.25f;
r[17] = (s7 * m[7] * m6m6 * 0.035714287f + s7 * m7m7 * m[7] * 0.035714287f)
    * s21_2 - s21_2 * m8m8 * (s7 * m[7]) * 0.14285715f;
r[18] = m8m7 * m[2] * 2.0f - m6m6m1 * 0.25f - m7m7m1 * 0.75f + m8m8 * m[1]
    - m6m0m7 * 0.5f;
r[19] = m5m4 * s105_2_21 * m[8] * 0.01904762f
    + s21_2 * (s105 * m1m1 * m[7]) * 0.007142857f
    - s21_2 * (s105 * m4m4 * m[7]) * 0.007142857f
    + m5m5 * s105 * s21_2 * m[7] * 0.00952381f
    - s21_2 * (s105 * m3m3 * m[7]) * 0.0023809525f
    - m2m1 * s105_2_21 * m[8] * 0.01904762f
    + s105_2_21 * m1m0m6 * 0.004761905f
    - m2m2 * s105 * s21_2 * m[7] * 0.00952381f
    - s105_2_21 * m4m3m6 * 0.004761905f
    + s21_2 * (s105 * m0m0 * m[7]) * 0.0023809525f;
r[20] = (m3m3m1 * 0.007142857f - m4m2 * m[5] * 0.057142857f
    + m3m0m4 * 0.014285714f) * s21_35 - m0m0m1 * s21_35 * 0.007142857f
    + (m2m2 * m[1] * 0.028571429f + m4m4m1 * 0.021428572f) * s21_35
    - m5m5 * s21_35 * m[1] * 0.028571429f - m1m1m1 * s21_35 * 0.007142857f;
r[21] = m3m0m2 * s7_2_35 * 0.042857144f
    + s35 * m[5] * m1m1 * s7_2 * 0.021428572f
    - s35 * m[5] * s7_2 * m2m2 * 0.042857144f
    + m4m1m2 * s7_2_35 * 0.042857144f
    - s35 * m4m4 * m[5] * s7_2 * 0.021428572f
    - s35 * m3m3 * m[5] * s7_2 * 0.021428572f
    + (m5m5 * s35 * m[5] * 0.014285714f + s35 * m[5] * m0m0 * 0.021428572f)
    * s7_2;
r[22] = m5m2 * s7_105 * m[8] * 0.057142857f
    - m3m0m8 * s7_105 * 0.028571429f - m3m2m6 * s7_105 * 0.028571429f
    - m5m0m6 * s7_105 * 0.028571429f - m4m1m8 * s7_105 * 0.028571429f
    - m5m1m7 * s7_105 * 0.028571429f - m4m2m7 * s7_105 * 0.028571429f;
r[23] = m6m3m8 * s7_2_21 * 0.071428575f
    - s21 * m[5] * s7_2 * m8m8 * 0.071428575f
    + (s21 * m[5] * m7m7 + s21 * m[5] * m6m6) * s7_2 * 0.035714287f
    + m8m7m4 * s7_2_21 * 0.071428575f;
r[24] = m8m8 * m[8] - (m6m6m8 + m7m7m8) * 1.5f;
r[25] = m6m0m8 * s7_2_21 * 0.071428575f
    + s21 * m[2] * m7m7 * s7_2 * 0.035714287f
    + s21 * m[2] * m6m6 * s7_2 * 0.035714287f
    + m8m7m1 * s7_2_21 * 0.071428575f
    - s21 * m[2] * s7_2 * m8m8 * 0.071428575f;
r[26] = (m5m3m6 * 0.028571429f + m3m3m8 * 0.014285714f
    - m2m1m7 * 0.028571429f) * s7_105 - m0m0m8 * s7_105 * 0.014285714f
    + m4m4m8 * s7_105 * 0.014285714f - m1m1m8 * s7_105 * 0.014285714f
    - m2m0m6 * s7_105 * 0.028571429f
    + (m2m2 * m[8] + m5m4m7) * s7_105 * 0.028571429f
    - m5m5 * s7_105 * m[8] * 0.028571429f;
r[27] = s35 * m[2] * s7_2 * m5m5 * 0.042857144f
    - (m2m2 * s35 * m[2] * 0.014285714f + s35 * m[2] * m3m3 * 0.021428572f)
    * s7_2 - s35 * m[2] * m4m4 * s7_2 * 0.021428572f
    - m3m0m5 * s7_2_35 * 0.042857144f
    + (s35 * m1m1 * m[2] + s35 * m0m0 * m[2]) * s7_2 * 0.021428572f
    - m4m1m5 * s7_2_35 * 0.042857144f;
r[28] = m4m4m3 * s21_35 * 0.007142857f
    - (m5m5 * s21_35 * m[3] * 0.028571429f
    + m4m0m1 * s21_35 * 0.014285714f) - m0m0m3 * s21_35 * 0.021428572f
    + (m2m2 * m[3] * 0.028571429f + m5m0 * m[2] * 0.057142857f
    + m3m3m3 * 0.007142857f) * s21_35 - m1m1m3 * s21_35 * 0.007142857f;
r[29] = m3m0m6 * s105_2_21 * 0.014285714f
    - m5m2 * s105_2_21 * m[6] * 0.01904762f
    - s105_2_21 * m5m0 * m[8] * 0.01904762f
    + (m3m1m7 + m4m1m6) * s105_2_21 * 0.004761905f
    - s105_2_21 * m3m2 * m[8] * 0.01904762f
    + m4m0m7 * s105_2_21 * 0.004761905f;
r[30] = m6m5 * m[8] * 2.0f - m6m6m3 * 0.75f - m6m4m7 * 0.5f
    - m7m7m3 * 0.25f + m8m8 * m[3];
r[31] = (s7 * m[6] * m7m7 * 0.035714287f + s7 * m6m6 * m[6] * 0.035714287f)
    * s21_2 - s7 * m[6] * s21_2 * m8m8 * 0.14285715f;
r[32] = m8m8 * m[0] - m6m1m7 * 0.5f - m6m6m0 * 0.75f - m7m7m0 * 0.25f
    + m6m2 * m[8] * 2.0f;
r[33] = m5m3 * s105_2_21 * m[8] * 0.01904762f
    - (s105 * m4m4 * m[6] * s21_2 * 0.0023809525f
    + m2m2 * s105 * s21_2 * m[6] * 0.00952381f)
    - s105 * m3m3 * m[6] * s21_2 * 0.007142857f
    - m4m3m7 * s105_2_21 * 0.004761905f
    + s105 * m1m1 * m[6] * s21_2 * 0.0023809525f
    - m2m0 * s105_2_21 * m[8] * 0.01904762f
    + (m5m5 * s105 * m[6] * 0.00952381f
    + s105 * m0m0 * m[6] * 0.007142857f) * s21_2
    + m1m0m7 * s105_2_21 * 0.004761905f;
r[34] = m3m1m4 * s21_35 * 0.014285714f
    - m5m5 * s21_35 * m[0] * 0.028571429f - m0m0m0 * s21_35 * 0.007142857f
    + (m2m2 * m[0] * 0.028571429f + m3m3m0 * 0.021428572f
    + m4m4m0 * 0.007142857f) * s21_35 - m1m1m0 * s21_35 * 0.007142857f
    - m3m2 * s21_35 * m[5] * 0.057142857f;
r[35] = s35 * m[5] * m1m1 * s105_2 * 0.007142857f
    - s35 * m[5] * m0m0 * s105_2 * 0.007142857f
    + s35 * m3m3 * m[5] * s105_2 * 0.007142857f
    + m4m1m2 * s105_35_2 * 0.014285714f - m3m0m2 * s105_35_2 * 0.014285714f
    - s35 * m4m4 * m[5] * s105_2 * 0.007142857f;
r[36] = m5m0m6 + m3m2m6 + m3m0m8 - m4m1m8 - m4m2m7 - m5m1m7;
r[37] = s21 * m[5] * m7m7 * s105_2 * 0.011904762f
    - s21 * m[5] * m6m6 * s105_2 * 0.011904762f
    - m6m3m8 * s105_2_21 * 0.023809524f
    + m8m7m4 * s105_2_21 * 0.023809524f;
r[38] = m6m6m8 * s7_105 * 0.071428575f - m7m7m8 * s7_105 * 0.071428575f;
r[39] = (m8m7m1 * 0.023809524f - m6m0m8 * 0.023809524f) * s105_2_21
    - s21 * m[2] * m6m6 * s105_2 * 0.011904762f
    + s21 * m[2] * m7m7 * s105_2 * 0.011904762f;
r[40] = m4m4m8 * 0.5f - m1m1m8 * 0.5f - m3m3m8 * 0.5f + m0m0m8 * 0.5f
    - m5m3m6 - m2m1m7 + m5m4m7 + m2m0m6;
r[41] = (s35 * m1m1 * m[2] * 0.007142857f
    - s35 * m0m0 * m[2] * 0.007142857f + s35 * m[2] * m3m3 * 0.007142857f)
    * s105_2 + m3m0m5 * s105_35_2 * 0.014285714f
    - s35 * m[2] * m4m4 * s105_2 * 0.007142857f
    - m4m1m5 * s105_35_2 * 0.014285714f;
r[42] = m0m0m3 * 0.75f - m1m1m3 * 0.75f - m4m0m1 * 1.5f + m4m4m3 * 0.75f
    - m3m3m3 * 0.25f;
r[43] = (m3m1m7 + m4m1m6) * s105_35_2 * 0.014285714f
    - m3m0m6 * s105_35_2 * 0.014285714f
    + m4m0m7 * s105_35_2 * 0.014285714f;
r[44] = (m6m6m3 * 0.035714287f - m6m4m7 * 0.071428575f) * s21_35
    - m7m7m3 * s21_35 * 0.035714287f;
r[45] = (s7 * m[6] * m7m7 * 0.10714286f - s7 * m6m6 * m[6] * 0.035714287f)
    * s35_2;
r[46] = m6m6m0 * s21_35 * 0.035714287f
    - (m7m7m0 * 0.035714287f + m6m1m7 * 0.071428575f) * s21_35;
r[47] = m1m0m7 * s105_35_2 * 0.014285714f
    + s105 * m3m3 * m[6] * s35_2 * 0.007142857f
    - s105 * m0m0 * m[6] * s35_2 * 0.007142857f
    - s105 * m4m4 * m[6] * s35_2 * 0.007142857f
    + s105 * m1m1 * m[6] * s35_2 * 0.007142857f
    - m4m3m7 * s105_35_2 * 0.014285714f;
r[48] = m4m4m0 * 0.75f - m1m1m0 * 0.75f - m3m3m0 * 0.75f + m0m0m0 * 0.25f
    + m3m1m4 * 1.5f;
```

## Existing declarations

At `4c377fb` nothing declares, stubs or calls `0x143600` in the source.
The code it relies on:

| Retail | Declared as | Where | What retail shows |
| --- | --- | --- | --- |
| `0x143250` | `void function_143250(real *out, unsigned long n, real const *a, real const *b)` | `src/unknown_141590.cpp` | Matched. Called with `out` in `eax`, `n` in `ebx`, and `a` and `b` on the stack |
| `0x143360` | `void spherical_harmonics_evaluate_direction(vector3f const *direction, dword order, real *result)` | `src/spherical_harmonics.cpp` | `todo`. Its basis is the one the rotation works in |

`matrix3x3` (`forward`, `left` and `up`) is in `include/unknown_0259d0.h`.

## Evidence

- `0x143600` was disassembled from the retail XBE with capstone 5.0.9,
  through `tools/xbe.py`, at `4c377fb`. It is 1,768 instructions,
  straight-line apart from the two order tests, with x87 code only for
  the seven square roots and the two products taken on the x87 stack.
- A Python replay executed every instruction symbolically, tracking the
  registers, the frame and the x87 stack, and took each band's matrix as
  expression trees at its `call 0x143250`. The C above was printed from
  those trees and parsed back: all 209 statements give the same trees.
- Numerical checks, in double precision: the three matrices are
  orthogonal for random rotations (to 4e-7 with float rounding applied).
  With the basis ported line by line from
  `spherical_harmonics_evaluate_direction`, the result evaluated in
  direction d equals the input evaluated at (`forward`·d, `left`·d,
  `up`·d) to 5e-8, over 200 random rotations, directions and
  coefficients. Reading the matrix by columns instead fails. The residue
  is the rounding of the float constants.
- Constants are read from retail `.rdata`. The square roots are taken of
  the doubles at `0x45dec8` (5), `0x45dec0` (15), `0x45ded0` (2),
  `0x45deb0` (35), `0x45dea8` (105), `0x45dea0` (21) and `0x45de98` (7),
  in that order. The floats are `0x43fde4` (1/7), `0x43ff08` (1.5),
  `0x45dbbc` (0.5), `0x45dc0c` (2), `0x45dc20` (0.25), `0x45dc2c` (0.75),
  `0x45dc5c` (0.2), `0x45dc68` (0.1), `0x45dc6c` (1/30), `0x45dd34`
  (1/15), `0x45de58` (1/84), `0x45de5c` (3/70), `0x45de60` (1/105),
  `0x45de64` (1/420), `0x45de68` (1/210), `0x45de6c` (2/105), `0x45de70`
  (2/35), `0x45de74` (3/140), `0x45de78` (1/35), `0x45de7c` (1/42),
  `0x45de80` (-1/42), `0x45de84` (1/140), `0x45de88` (3/28), `0x45de8c`
  (1/14), `0x45de90` (1/28), `0x45de94` (1/70), `0x45deb8` (2/15) and
  `0x45debc` (-0.2), each the float nearest the fraction.
- Each of the nine calls is preceded by `push 3` in `0x22850` and
  `0x23690`.
- No emulator, runtime testing, SDK or outside dataset was used. Names
  are the repository's own, or describe behaviour.
