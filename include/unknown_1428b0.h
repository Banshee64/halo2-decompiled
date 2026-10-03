/* UNKNOWN_1428B0.H: matrix types used by the matrix math at 0x1428b0 */

#ifndef UNKNOWN_1428B0_H
#define UNKNOWN_1428B0_H

#include "real_math.h"

matrix3x3 *matrix3x3_transpose(matrix3x3 const *in, matrix3x3 *out);
real_point3d *matrix4x3_transform_point(real_matrix4x3 const *matrix, real_point3d const *point, real_point3d *result);
/* unknown_141590.cpp */
real_vector3d *function_142640(real_matrix4x3 const *matrix, real_vector3d const *vector, real_vector3d *out);
real_point3d *function_142700(real_matrix4x3 const *matrix, real_point3d const *point, real_point3d *out);
real_vector3d *matrix4x3_inverse_transform_vector(real_matrix4x3 const *matrix, real_vector3d const *vector, real_vector3d *out);

#endif
