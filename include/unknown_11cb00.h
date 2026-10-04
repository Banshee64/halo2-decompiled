/* UNKNOWN_11CB00.H: types for the quaternion and ray math */

#ifndef UNKNOWN_11CB00_H
#define UNKNOWN_11CB00_H

#include "unknown_0259d0.h"

/* a quaternion rotation, a position and a uniform scale: 8 reals */
struct real_quaternion_transform
{
	quaternionf rotation;
	point3f position;
	real scale;
};

struct box3f
{
	real x0, x1;
	real y0, y1;
	real z0, z1;
};

#endif
