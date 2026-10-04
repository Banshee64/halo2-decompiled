/* UNKNOWN_1428B0.H: matrix types used by the matrix math at 0x1428b0 */

#ifndef UNKNOWN_1428B0_H
#define UNKNOWN_1428B0_H

#include "unknown_0259d0.h"

matrix3x3 *function_142b80(matrix3x3 const *in, matrix3x3 *out);
point3f *transform4x3f_apply_point(transform4x3f const *matrix, point3f const *point, point3f *result);
/* unknown_141590.cpp */
vector3f *function_142640(transform4x3f const *matrix, vector3f const *vector, vector3f *out);
point3f *function_142700(transform4x3f const *matrix, point3f const *point, point3f *out);
vector3f *function_1427f0(transform4x3f const *matrix, vector3f const *vector, vector3f *out);

#endif
