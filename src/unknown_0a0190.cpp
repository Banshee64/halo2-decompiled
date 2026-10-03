// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_0A0190.CPP: real math validity checks (decompiled by lane N for
   0x143120, which calls both) */

#include "cseries.h"
#include "real_math.h"
#include <math.h>

#define k_real_tolerance 0.001f

static inline bool valid_real(real value)
{
	return (*(long *)&value & 0x7f800000) != 0x7f800000;
}

// @retail 0xa0190
bool valid_real_normal3d(real_vector3d const *vector)
{
	real difference = magnitude_squared3d(vector) - 1.0f;

	return valid_real(difference) && fabs(difference) < k_real_tolerance;
}

// @retail 0xa0200
bool valid_realcmp(real a, real b)
{
	real difference = a - b;

	return valid_real(difference) && fabs(difference) < k_real_tolerance;
}
