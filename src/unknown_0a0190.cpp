// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_0A0190.CPP: real math validity checks (decompiled by lane N for
   0x143120, which calls both) */

#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include <math.h>

#define k_real_tolerance 0.001f

static inline bool function_x41b793(real value)
{
	return (*(long *)&value & 0x7f800000) != 0x7f800000;
}

// @retail 0xa0190
bool function_a0190(vector3f const *vector)
{
	real difference = length_sq3f(vector) - 1.0f;

	return function_x41b793(difference) && fabs(difference) < k_real_tolerance;
}

// @retail 0xa0200
bool function_a0200(real a, real b)
{
	real difference = a - b;

	return function_x41b793(difference) && fabs(difference) < k_real_tolerance;
}
