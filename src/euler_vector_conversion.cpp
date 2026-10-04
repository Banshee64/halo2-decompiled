// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "unknown_11cc90.h"

void function_141ce0(real a, real b, real c, real_matrix4x3 *out);

// @retail 0x11df60
void function_11df60(real_vector3d const *rotation,
	real_vector3d *forward, real_vector3d *up)
{
	real_matrix4x3 matrix;
	function_141ce0(rotation->i, rotation->j, rotation->k, &matrix);
	*forward = matrix.forward;
	*up = matrix.up;
}

// @retail 0x11dfb0
void function_11dfb0(real_vector2d const *rotation,
	real_vector3d *forward, real_vector3d *up)
{
	real_matrix4x3 matrix;
	function_141ce0(rotation->i, rotation->j, 0.f, &matrix);
	*forward = matrix.forward;
	*up = matrix.up;
}
