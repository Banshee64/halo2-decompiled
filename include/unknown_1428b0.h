/* UNKNOWN_1428B0.H: matrix types used by the matrix math at 0x1428b0 */

#ifndef UNKNOWN_1428B0_H
#define UNKNOWN_1428B0_H

#include "real_math.h"

matrix3x3 *matrix3x3_transpose(matrix3x3 const *in, matrix3x3 *out);
real_point3d *matrix4x3_transform_point(real_matrix4x3 const *matrix, real_point3d const *point, real_point3d *result);

#endif
