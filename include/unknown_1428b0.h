/* UNKNOWN_1428B0.H: matrix types used by the matrix math at 0x1428b0 */

#ifndef UNKNOWN_1428B0_H
#define UNKNOWN_1428B0_H

#include "real_math.h"

struct matrix3x3
{
	real_vector3d forward;
	real_vector3d left;
	real_vector3d up;
};

struct real_matrix4x3
{
	real scale;
	matrix3x3 rotation;
	real_point3d position;
};

struct real_plane3d
{
	real_vector3d n;
	real d;
};

matrix3x3 *matrix3x3_transpose(matrix3x3 const *in, matrix3x3 *out);

#endif
