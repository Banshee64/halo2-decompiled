// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_02B400.CPP: 2d vector math */

#include "cseries.h"
#include "real_math.h"
#include <math.h>

#define k_real_epsilon 0.0001f

// @retail 0x2b400
real normalize2d(real_point2d *v)
{
	real m = (real)sqrt(v->x * v->x + v->y * v->y);
	if (!(fabs(m) < k_real_epsilon))
	{
		real inv = 1.f / m;
		v->x = v->x * inv;
		v->y = inv * v->y;
		return m;
	}
	return 0.f;
}
