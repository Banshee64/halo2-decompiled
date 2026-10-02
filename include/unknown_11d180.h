/* UNKNOWN_11D180.H: quaternion type used by the vector math at 0x11d180 */

#ifndef UNKNOWN_11D180_H
#define UNKNOWN_11D180_H

#include "real_math.h"

union real_quaternion_11d180
{
	real n[4];
	struct { real i, j, k, w; };
};

#endif
