// @flags /O2 /Gr /arch:SSE
#include "unknown_2551c0.h"
#include "globals.h"

struct s_28f700
{
	byte field_0[0x1c];
	long field_1c;
	byte field_20[0x888 - 0x20];
};

struct s_28f701
{
	byte field_0[0x18];
	long field_18;
	short field_1c;
	byte field_1e[2];
	long field_20;
	byte field_24[0x10];
};

struct s_28f702
{
	byte field_0[0xc];
	long field_c;
};

void __stdcall function_28e770(long arg_0, long arg_1);
void function_28f3b0(long arg_0, long arg_1);
void function_28f600(long arg_0);

__forceinline s_28f702 *function_28f701(s_handler_object_view *arg_0)
{
	s_28f702 *local_0;
	if (!arg_0->flags134)
		local_0 = (s_28f702 *)((byte *)arg_0 + arg_0->ai_offset);
	else
		local_0 = NULL;
	return local_0;
}

// @retail 0x28f700
void __stdcall function_28f700(long arg_0)
{
	s_28f700 *local_0 = &((s_28f700 *)g_4f55f0->data)[arg_0 & 0xffff];
	s_28f701 *local_1 = (s_28f701 *)perception_get(local_0->field_1c);
	if ((real)(g_510c54->game_time - local_1->field_20) * g_510c54->rate > 2.0f)
		local_1->field_1c = 0;
	long local_2 = ((s_28f701 *)perception_get(local_0->field_1c))->field_18;
	while (local_2 != NONE)
	{
		long local_3 = local_2;
		s_28f702 *local_4 = function_28f701(handler_object_get(local_3));
		if (local_4)
		{
			local_2 = local_4->field_c;
			function_28e770(arg_0, local_3);
		}
		else
		{
			local_2 = NONE;
			function_28e770(arg_0, local_3);
		}
	}
	local_2 = ((s_28f701 *)perception_get(local_0->field_1c))->field_18;
	while (local_2 != NONE)
	{
		long local_3 = local_2;
		s_28f702 *local_4 = function_28f701(handler_object_get(local_3));
		if (local_4)
		{
			local_2 = local_4->field_c;
			function_28f3b0(arg_0, local_3);
		}
		else
		{
			local_2 = NONE;
			function_28f3b0(arg_0, local_3);
		}
	}
	local_2 = ((s_28f701 *)perception_get(local_0->field_1c))->field_18;
	while (local_2 != NONE)
	{
		long local_3 = local_2;
		s_28f702 *local_4 = function_28f701(handler_object_get(local_3));
		if (local_4)
		{
			local_2 = local_4->field_c;
			function_28f600(local_3);
		}
		else
		{
			local_2 = NONE;
			function_28f600(local_3);
		}
	}
}
