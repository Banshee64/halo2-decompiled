/* UNKNOWN_11CC90.H: 2d vector type */

#ifndef UNKNOWN_11CC90_H
#define UNKNOWN_11CC90_H

#include "real_math.h"

union real_vector2d
{
	real n[2];
	struct { real i, j; };
};

#endif
