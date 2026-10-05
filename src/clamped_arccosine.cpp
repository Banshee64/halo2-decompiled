// @flags /O2 /arch:SSE /Gr
/* CLAMPED_ARCCOSINE.CPP: the arccosine of a cosine pinned to -1..1 (lane D) */

#include "unknown_11c920.h"
#include <math.h>

// @retail 0x50650
real function_50650(real cosine)
{
	real value;
	if (-1.0f > cosine)
		value = -1.0f;
	else
		value = cosine > 1.0f ? 1.0f : cosine;
	return (real)acos(value);
}
