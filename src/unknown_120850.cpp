// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_120850.CPP */

#include "cseries.h"
#include "real_math.h"
#include <math.h>

// @retail 0x120850
short function_120850(real_vector3d const *v)
{
	real x = (real)fabs(v->i);
	real y = (real)fabs(v->j);
	real z = (real)fabs(v->k);

	if (z >= y && z >= x)
		return 2;
	if (y >= x)
		return 1;
	return 0;
}