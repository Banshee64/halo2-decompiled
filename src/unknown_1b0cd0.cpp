// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"
#include "joint_behavior.h"

/* slot type 0x7e */

struct s_slot_7e
{
	s_slot_header header;
	byte unknown0c[4];
	long element_index;
	byte unknown14[0x40 - 0x14];
};

short __stdcall function_1b0cd0(long actor_index);
void __stdcall function_1b0d50(long actor_index, s_slot *slot);
void __stdcall function_1b13b0(long actor_index, s_slot *slot, long index);
void __stdcall function_1b1a90(long actor_index, s_slot *slot, s_slot_target_list *list);
void __stdcall function_1b1d90(long actor_index, s_slot *slot, long a, long b);

// @retail 0x1b0d10
short __stdcall function_1b0d10(long actor_index, s_slot *slot, bool active)
{
	s_slot_7e *state = (s_slot_7e *)slot;
	short result = g_46fbe8;

	if (element_502424_get(state->element_index)->unknown04 == NONE)
		result = g_46fbe4;
	return result;
}

s_slot_handler_2x g_47df60 =
{
	{
		{
			0x7e, 2, 0, -2, 0,
			function_1b0cd0, function_1b0d10, joint_initiate, joint_leave, NONE, {0},
			0, 0, 0, 0, 0, 0, 1
		},
		(t_slot_proc)joint_update, joint_activate, joint_deactivate
	},
	function_1b0d50, 0, function_1b13b0, 0, function_1b1a90, function_1b1d90,
	3, 10, 1.0f, 0
};
