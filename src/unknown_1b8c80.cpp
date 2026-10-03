// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"
#include "joint_behavior.h"

/* the slot tests 0x5f, 0x60, 0x5e, 0x4d, 0x4e and 0x4f, and slot type 0x4c */

struct s_slot_4c
{
	s_slot_header header;
	byte unknown0c[4];
	long element_index;
	byte unknown14[0x1c - 0x14];
	long unknown1c;
	byte unknown20[2];
	byte flags;
	byte unknown23[0x34 - 0x23];
	real_point3d point;
};

short __stdcall function_1b94c0(long actor_index, s_slot *slot);
short __stdcall function_1b9500(long actor_index, s_slot *slot);
short __stdcall function_1b9540(long actor_index, s_slot *slot);
short __stdcall function_1b9640(long actor_index, s_slot *slot);
short __stdcall function_1b9890(long actor_index, s_slot *slot);
short __stdcall function_1b99d0(long actor_index, s_slot *slot);
short __stdcall function_1b9fc0(long actor_index);
short __stdcall function_1ba4e0(long actor_index, s_slot *slot, bool active);
void __stdcall function_1ba090(long actor_index, s_slot *slot);
void __stdcall function_1ba3f0(long actor_index, s_slot *slot, long index);
void __stdcall function_1ba5c0(long actor_index, s_slot *slot, long index);
void __stdcall function_1bb3a0(long actor_index, s_slot *slot, long a, long b);

// @retail 0x1ba8c0
void __stdcall function_1ba8c0(long actor_index, s_slot *slot, s_slot_target_list *list)
{
	s_slot_4c *state = (s_slot_4c *)slot;
	s_actor_view *actor = actor_get(actor_index);

	if (actor->unknown26c == NONE)
	{
		if (state->flags & 4)
		{
			actor->unknown420 = 4;
			actor->unknown41c = 3;
			actor->unknown424.point = state->point;
			actor->unknown44d = true;
		}
		else if (actor->unknown086 >= 7)
		{
			actor->unknown41c = 3;
			actor->unknown420 = 2;
			actor->unknown488 = true;
		}
		else if (actor->unknown50c)
		{
			actor->unknown41c = 2;
			actor->unknown420 = 0;
		}
	}
	actor->unknown450 = 0x6000086;
}

// @retail 0x1bb530
void __stdcall function_1bb530(long actor_index, s_slot *slot, long index)
{
	s_slot_4c *state = (s_slot_4c *)slot;
	s_502424_element *element = element_502424_get(state->element_index);

	if (state->unknown1c == index || element->target.unknown0 == index)
	{
		state->unknown1c = NONE;
		element->target.unknown0 = NONE;
	}
}

s_slot_handler_0 g_47e954 =
{
	0x5f, 0, 0xbff, -2, 0, function_1b94c0
};

s_slot_handler_0 g_47e968 =
{
	0x60, 0, 0xbff, -2, 0, function_1b9500
};

s_slot_handler_0 g_47e97c =
{
	0x5e, 0, 0, -2, 0, function_1b9540
};

s_slot_handler_0 g_47e990 =
{
	0x4d, 0, 0, -2, 0, function_1b9640
};

s_slot_handler_0 g_47e9a4 =
{
	0x4e, 0, 0, -2, 0, function_1b9890
};

s_slot_handler_0 g_47e9b8 =
{
	0x4f, 0, 0, -2, 0, function_1b99d0
};

s_slot_handler_2x g_47e9d0 =
{
	{
		{
			0x4c, 2, 0, -2, 0,
			function_1b9fc0, function_1ba4e0, joint_initiate, joint_leave, 1, {0},
			0, 0, function_1bb530, 0, 0, 0, 1
		},
		(t_slot_proc)joint_update, joint_activate, joint_deactivate
	},
	function_1ba090, function_1ba3f0, function_1ba5c0, slot_release_nothing, function_1ba8c0, function_1bb3a0,
	1, 10, 1.5f, 0x5b
};
