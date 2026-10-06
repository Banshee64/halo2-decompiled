#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_1cec30.h"
#include "object_queries.h"
#include <math.h>
// @flags /O2 /arch:SSE /Gr

class c_material_shape;
void function_182b90(c_material_shape *arg_0, hkEntity const *arg_1,
	real *arg_2, short *arg_3, real *arg_4);

struct s_277ec0
{
	c_material_shape *field_0;
	byte field_4[8];
	s_277ec0 *field_c;
	byte field_10[8];
	long field_18;
	long field_1c;
	hkEntity *field_20;
};

__forceinline hkEntity *function_277efd(s_277ec0 *arg_0)
{
	while (arg_0->field_c)
		arg_0 = arg_0->field_c;
	return arg_0->field_18 == 1 ? arg_0->field_20 : NULL;
}

__forceinline real function_278005(real arg_0, real arg_1)
{
	return (real)sqrt(arg_0 * arg_1);
}

// @retail 0x277ec0
void function_277ec0(real *arg_0, real *arg_1, short *arg_4, short *arg_5,
	s_277ec0 *arg_2, s_277ec0 *arg_3, long arg_6, long arg_7)
{
	s_277ec0 *local_0[2];
	short *local_1[2];
	local_1[0] = arg_4;
	local_1[1] = arg_5;
	local_0[0] = arg_2;
	local_0[1] = arg_3;
	real local_2[2];
	real local_3[2];
	long local_4 = 0;
	do
	{
		s_277ec0 *local_5 = local_0[local_4];
		c_material_shape *local_13 = local_5->field_0;
		hkEntity *local_6 = function_277efd(local_5);
		function_182b90(local_13, local_6, &local_2[local_4], local_1[local_4], &local_3[local_4]);
		local_4++;
	} while (local_4 < 2);
	if (arg_6 != NONE && arg_7 != NONE)
	{
		s_havok_component *local_7 = havok_component_get(arg_6);
		s_havok_component *local_8 = havok_component_get(arg_7);
		long local_9 = *((byte *)g_4e0300->data + (local_7->object_index & 0xffff) * 12 + 3);
		long local_10 = *((byte *)g_4e0300->data + (local_8->object_index & 0xffff) * 12 + 3);
		if (local_9 == 0 && local_10 == 1 || local_10 == 0 && local_9 == 1)
		{
			vector3f local_11;
			function_ba1d0((local_9 == 0 ? local_8 : local_7)->object_index, &local_11, NULL);
			real local_12 = local_11.k * local_11.k;
			local_12 += local_11.j * local_11.j;
			local_12 += local_11.i * local_11.i;
			if (local_12 > 1.0f)
			{
				*arg_0 = 0.0f;
				*arg_1 = 1.0f;
				return;
			}
		}
	}
	*arg_0 = function_278005(local_2[0], local_2[1]);
	*arg_1 = function_278005(local_3[0], local_3[1]);
}
