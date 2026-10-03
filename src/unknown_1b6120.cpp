// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"
#include "joint_behavior.h"

/* slot type 0x25 */

struct s_slot_25
{
	s_slot_header header;
	byte unknown0c[4];
	long element_index;
	byte unknown14[0x40 - 0x14];
};

short __stdcall function_1b6450(long actor_index, s_slot *slot, bool active);
void __stdcall function_1b6120(long actor_index, s_slot *slot);
void __stdcall function_1b6740(long actor_index, s_slot *slot, long a, long b);

// @retail 0x1b6880
void __stdcall function_1b6880(long actor_index, s_slot *slot, s_slot_target_list *list)
{
	actor_get(actor_index)->unknown4a1 = true;
}

// @retail 0x1b68b0
void __stdcall function_1b68b0(long actor_index, s_slot *slot, long index)
{
	s_slot_25 *state = (s_slot_25 *)slot;
	s_502424_element *element = element_502424_get(state->element_index);

	if (element->target.unknown4 == index)
		element->target.unknown4 = NONE;
}

// @retail 0x1b68f0
void __stdcall function_1b68f0(long actor_index, s_slot *slot)
{
	s_slot_25 *state = (s_slot_25 *)slot;
	s_502424_element *element = element_502424_get(state->element_index);

	if (element->target.unknown14 == 1)
		element->target.unknown4 = NONE;
}

s_slot_handler_2x g_47e648 =
{
	{
		{
			0x25, 2, 0, -2, 0,
			function_1adcd0, function_1b6450, joint_initiate, joint_leave, NONE, {0},
			0, 0, function_1b68b0, function_1b68f0, 0, 0, 1
		},
		(t_slot_proc)joint_update, joint_activate, joint_deactivate
	},
	function_1b6120, 0, 0, 0, function_1b6880, function_1b6740,
	2, 10, 150.0f, 0
};
