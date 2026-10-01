/* REAL_MATH.CPP: two header inline functions that retail keeps out of line
in some callers: distance3d (0x3ea30) and _real_random_range (0x259d0).

The source's shape decides the match:
- distance3d squares k, then j, then i: written i*i + j*j + k*k, the
  compiler evaluates the operands in another order.
- _real_random_range takes the random value into a local first. LTCG also
  deletes its unused file and line arguments, as in retail. */

#include "cseries.h"
#include "real_math.h"
#include <math.h>

__inline real square_root(
	real x)
{
	return (real)sqrt(x);
}

__inline real_vector3d *vector_from_points3d(
	real_point3d const *a,
	real_point3d const *b,
	real_vector3d *result)
{
	result->i = b->x - a->x;
	result->j = b->y - a->y;
	result->k = b->z - a->z;
	return result;
}

// @retail 0x3ea30
real distance3d(
	real_point3d const *a,
	real_point3d const *b)
{
	real_vector3d v;
	vector_from_points3d(a, b, &v);
	return square_root(v.k*v.k + v.j*v.j + v.i*v.i);
}

__inline dword _random(
	dword *seed,
	char const *file,
	long line)
{
	*seed = 1664525 * *seed + 1013904223;
	return *seed >> 16;
}

__inline real _real_random(
	dword *seed,
	char const *file,
	long line)
{
	return (real)_random(seed, file, line) * (1.f / 65535.f);
}

// @retail 0x259d0
real _real_random_range(
	dword *seed,
	char const *file,
	long line,
	real lower_bound,
	real upper_bound)
{
	real random = _real_random(seed, file, line);
	return lower_bound + (upper_bound - lower_bound) * random;
}
