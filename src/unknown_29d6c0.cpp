#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include "globals.h"
#include "unknown_20fe20.h"
#include "unknown_2626b0.h"
#include "unknown_1fa590.h"
#include <math.h>
// @flags /O2 /arch:SSE /Gr

real function_30bf0(vector3f *arg_0);

struct s_29d6c0
{
	byte field_0[0xc];
	short field_c;
	byte field_e;
	byte field_f[9];
	real field_18;
	real field_1c;
};

// @retail 0x29d6c0
bool function_29d6c0(vector3f *arg_0, s_reference arg_1)
{
	s_29d6c0 *local_0 = (s_29d6c0 *)function_262b40(arg_1);
	if (local_0->field_e & 0x80)
	{
		*arg_0 = *g_4687b0;
		return true;
	}
	vector3f local_1;
	real local_2 = (real)cos(local_0->field_1c);
	local_1.i = (real)cos(local_0->field_18) * local_2;
	local_1.j = (real)sin(local_0->field_18) * local_2;
	local_1.k = (real)sin(local_0->field_1c);
	return function_2105b0(local_0->field_c, &local_1, arg_0);
}

// @retail 0x29d740
bool function_29d740(vector3f *arg_0, s_pathfinding_data const *arg_1, long arg_2)
{
	s_pathfinding_edge const *local_0 = &arg_1->edges[arg_2];
	point3f const *local_1 = &arg_1->vertices[local_0->vertices[0]];
	point3f const *local_2 = &arg_1->vertices[local_0->vertices[1]];
	real local_3 = local_2->x - local_1->x;
	real local_4 = local_2->y - local_1->y;
	arg_0->i = local_4;
	arg_0->j = 0.0f - local_3;
	arg_0->k = 0.0f;
	bool local_5 = false;
	if (function_30bf0(arg_0) > 0.0f)
		local_5 = true;
	return local_5;
}
