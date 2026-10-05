// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "unknown_11cc90.h"

void function_141ce0(real a, real b, real c, transform4x3f *out);

// @retail 0x11df60
void function_11df60(vector3f const *rotation,
	vector3f *forward, vector3f *up)
{
	transform4x3f matrix;
	function_141ce0(rotation->i, rotation->j, rotation->k, &matrix);
	*forward = matrix.forward;
	*up = matrix.up;
}

// @retail 0x11dfb0
void function_11dfb0(vector2f const *rotation,
	vector3f *forward, vector3f *up)
{
	transform4x3f matrix;
	function_141ce0(rotation->i, rotation->j, 0.f, &matrix);
	*forward = matrix.forward;
	*up = matrix.up;
}
