/*
UNKNOWN_0259D0.CPP: two header inline functions that retail keeps out of line
in some callers: distance3d (0x3ea30) and function_259d0 (0x259d0).

Their bodies match when compiled out of line. __declspec(noinline) stands in
for whatever kept Bungie's copies out of line; it does not change the body.
The source's shape decides the match:
- distance3d squares k, then j, then i: written i*i + j*j + k*k, the
  compiler evaluates the operands in another order.
- function_259d0 takes the random value into a local first. LTCG also
  deletes its unused file and line arguments, as in retail.

    python tools/match.py "/O2 /Gr" spike/unknown_0259d0.cpp spike/unknown_0259d0_test.cpp -- \
        "?distance3d@@YIMPBTreal_point3d@@0@Z=3ea30" \
        "?function_259d0@@YIMPAKPBDJMM@Z=259d0"
*/

#include <math.h>

typedef float real;

union point3f
{
	real n[3];
	struct { real x, y, z; };
};

union vector3f
{
	real n[3];
	struct { real i, j, k; };
};

__inline real square_root(
	real x)
{
	return (real)sqrt(x);
}

__inline vector3f *subtract_points3f(
	point3f const *a,
	point3f const *b,
	vector3f *result)
{
	result->i = b->x - a->x;
	result->j = b->y - a->y;
	result->k = b->z - a->z;
	return result;
}

__declspec(noinline) real distance3d(
	point3f const *a,
	point3f const *b)
{
	vector3f v;
	subtract_points3f(a, b, &v);
	return square_root(v.k*v.k + v.j*v.j + v.i*v.i);
}

__inline unsigned long _random(
	unsigned long *seed,
	char const *file,
	long line)
{
	*seed = 1664525 * *seed + 1013904223;
	return *seed >> 16;
}

__inline real function_x82e52f(
	unsigned long *seed,
	char const *file,
	long line)
{
	return (real)_random(seed, file, line) * (1.f / 65535.f);
}

__declspec(noinline) real function_259d0(
	unsigned long *seed,
	char const *file,
	long line,
	real lower_bound,
	real upper_bound)
{
	real random = function_x82e52f(seed, file, line);
	return lower_bound + (upper_bound - lower_bound) * random;
}
