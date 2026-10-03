// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_0A0190.CPP: the vector validity checks that release builds keep
   (lane M, for 0xdf380) */

#include "cseries.h"
#include "real_math.h"
#include <math.h>

static inline bool valid_real(real value)
{
	return (*(long *)&value & 0x7f800000) != 0x7f800000;
}

// @retail 0xa0190
bool valid_real_normal3d(real_vector3d const *vector)
{
	real error = magnitude_squared3d(vector) - 1.0f;

	return valid_real(error) && fabs(error) < 0.001f;
}

// @retail 0xa0200
bool valid_realcmp(real a, real b)
{
	real difference = a - b;

	return valid_real(difference) && fabs(difference) < 0.001f;
}
