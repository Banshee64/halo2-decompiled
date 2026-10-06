#include "unknown_11c920.h"
#include "globals.h"
#include "slot_handler.h"
#include <math.h>
// @flags /O2 /arch:SSE /Gr

struct s_298c30
{
	byte field_0[0x20];
	real field_20;
	real field_24;
};

struct s_298c46
{
	byte field_0;
	bool field_1;
	byte field_2[2];
	real field_4;
	real field_8;
	point2f field_c;
	byte field_14[4];
	short field_18;
	byte field_1a[0xe];
};

// @retail 0x298c30
void function_298c30(long arg_0, s_298c30 const *arg_1, real arg_2,
	real arg_3, s_298c46 *arg_4, point2f const *arg_5)
{
	s_slot_object_view *local_0 = object_get(arg_0);
	bool local_1 = arg_4->field_4 < 0.0f;
	bool local_2 = arg_3 > 0.0;
	real local_3 = 10.0f;
	real local_4;
	if (local_0->type == 1)
	{
		local_4 = *(real *)(g_4e3b44[(*(long *)local_0) & 0xffff].bytes + 0x1f4);
		if (local_4 <= 0.0f)
			local_4 = 15.0f;
	}
	else
		local_4 = 15.0f;
	if ((local_1 || arg_4->field_1) && local_2)
		local_3 /= (arg_3 / local_4) * 0.5f + 1.0f;
	if (local_1 || arg_4->field_1)
		local_3 /= arg_1->field_20 * arg_2 * 0.03333333507180214f + 1.0f;
	if (arg_4->field_1)
		local_3 *= 0.8f;
	local_3 = (real)(local_3 / (fabs(arg_4->field_4) * 0.2f + 1.0f));
	if (arg_5)
		arg_4->field_8 = (arg_4->field_c.y * arg_5->y + arg_4->field_c.x * arg_5->x + 3.0f) * local_3 * 0.25f;
	else
		arg_4->field_8 = local_3;
}

s_298c46 g_5047f8[8];
short g_504938;

__forceinline real function_298df3(point2f *arg_0)
{
	real local_0 = (real)sqrt(arg_0->x * arg_0->x + arg_0->y * arg_0->y);
	if (!(0.0001f > fabs(local_0)))
	{
		real local_1 = 1.0f / local_0;
		arg_0->x *= local_1;
		arg_0->y *= local_1;
	}
	return local_0;
}

__forceinline real function_2990b0(real arg_0)
{
	real local_0;
	if (-1.0f > arg_0)
		local_0 = -1.0f;
	else
		local_0 = arg_0 > 1.0f ? 1.0f : arg_0;
	return (real)acos(local_0);
}

__forceinline void function_2992ba(short arg_0, bool arg_1, real arg_2, point2f const *arg_3,
	point2f const *arg_4, long arg_5, s_298c30 const *arg_6, real arg_7, real arg_8, point2f const *arg_9)
{
	g_5047f8[arg_0].field_0 = arg_1;
	g_5047f8[arg_0].field_4 = arg_2;
	point2f local_0;
	if (arg_2 < 0.0f)
	{
		local_0.x = arg_3->x * -1.0f;
		local_0.y = arg_3->y * -1.0f;
	}
	else
		local_0 = *arg_3;
	g_5047f8[arg_0].field_1 = local_0.y * arg_4->y + local_0.x * arg_4->x < 0.0f;
	g_5047f8[arg_0].field_c = *arg_3;
	function_298c30(arg_5, arg_6, arg_7, arg_8, &g_5047f8[arg_0], arg_9);
	g_5047f8[arg_0].field_18 = arg_0;
}

// @retail 0x298d90
void function_298d90(long arg_0, long arg_1, s_298c30 const *arg_2, point2f const *arg_3,
	point2f const *arg_4, point2f const *arg_5, real arg_6, point2f const *arg_7, point2f const *arg_8)
{
	s_slot_object_view *local_0 = object_get(arg_1);
	point2f local_1 = { arg_7->x + arg_8->x, arg_7->y + arg_8->y };
	point2f local_2 = *arg_8;
	function_298df3(&local_2);
	point2f local_3[2];
	local_3[0].x = arg_4->x * -arg_6 + arg_7->x;
	local_3[0].y = arg_4->y * -arg_6 + arg_7->y;
	local_3[1].x = arg_4->x * arg_6 + arg_7->x;
	local_3[1].y = arg_4->y * arg_6 + arg_7->y;
	bool local_4 = local_2.x * arg_3->x + local_2.y * arg_3->y >= 0.0;
	real local_5 = *(real *)((byte *)local_0 + 0x8c) * arg_3->y + arg_3->x * *(real *)((byte *)local_0 + 0x88);
	short local_6 = 0;
	for (short local_7 = 0; local_7 < 2; local_7++)
	{
		point2f *local_8 = &local_3[local_7];
		real local_9 = (real)sqrt((local_1.x - local_8->x) * (local_1.x - local_8->x) + (local_1.y - local_8->y) * (local_1.y - local_8->y));
		bool local_10 = local_7 == 1;
		if (local_9 < arg_2->field_24)
		{
			real local_11 = (real)cos(1.5707963705062866);
			real local_12 = (real)sin(local_7 == 0 ? -1.5707963705062866 : 1.5707963705062866);
			g_5047f8[local_6].field_4 = 1.5707963705062866f;
			g_5047f8[local_6].field_c.x = local_11 * arg_3->x - local_12 * arg_3->y;
			g_5047f8[local_6].field_c.y = arg_3->y * local_11 + arg_3->x * local_12;
			g_5047f8[local_6].field_0 = local_10;
			g_5047f8[local_6].field_1 = false;
			g_5047f8[local_6].field_8 = 1.0f;
			g_5047f8[local_6].field_18 = local_6;
			local_6++;
			local_11 = (real)cos(-1.5707963705062866);
			local_12 = (real)sin(local_7 == 0 ? 1.5707963705062866 : -1.5707963705062866);
			g_5047f8[local_6].field_4 = -1.5707963705062866f;
			g_5047f8[local_6].field_c.x = local_11 * arg_3->x - local_12 * arg_3->y;
			g_5047f8[local_6].field_c.y = arg_3->y * local_11 + arg_3->x * local_12;
			g_5047f8[local_6].field_0 = local_10;
			g_5047f8[local_6].field_1 = false;
			g_5047f8[local_6].field_8 = 1.0f;
			g_5047f8[local_6].field_18 = local_6;
			local_6++;
		}
		else
		{
			real local_13 = arg_6 / local_9;
			if (local_13 > 1.0)
				local_13 = 1.0f;
			real local_14 = function_2990b0(local_13);
			point2f local_15 = { local_1.x - local_8->x, local_1.y - local_8->y };
			function_298df3(&local_15);
			real local_16 = local_15.y * arg_4->y + local_15.x * arg_4->x;
			if (local_16 > 1.0)
				local_16 = 1.0f;
			else if (local_16 < -1.0)
				local_16 = -1.0f;
			real local_17 = function_2990b0(local_16);
			real local_18;
			if (local_7 != 0)
				local_18 = local_4 ? 3.1415927410125732f - local_17 : local_17 + 3.1415927410125732f;
			else
				local_18 = local_4 ? local_17 : 6.2831854820251465f - local_17;
			real local_19[2];
			local_19[0] = local_18 + local_14;
			if (local_19[0] > 6.2831854820251465f)
				local_19[0] -= 6.2831854820251465f;
			local_19[1] = local_18 - local_14;
			long local_20 = 0;
			do
			{
				real local_21 = local_19[local_20];
				real local_22 = local_21 < 0.0f ? local_21 + 6.2831854820251465f : local_21 - 6.2831854820251465f;
				real local_23 = (real)sin(local_7 == 0 ? -local_22 : local_22);
				real local_24 = (real)cos(local_22);
				point2f local_25 = { local_24 * arg_3->x - local_23 * arg_3->y, local_24 * arg_3->y + local_23 * arg_3->x };
				function_2992ba(local_6, local_10, local_21, &local_25, &local_2, arg_1, arg_2, local_13, local_5, arg_5);
				local_6++;
				function_2992ba(local_6, local_10, local_22, &local_25, &local_2, arg_1, arg_2, local_13, local_5, arg_5);
				local_6++;
				local_20++;
			} while (local_20 < 2);
		}
	}
	g_504938 = local_6;
}
