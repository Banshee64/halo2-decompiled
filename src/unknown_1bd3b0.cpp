// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"
#include "joint_behavior.h"

/* slot type 0x5d */

struct s_slot_5d
{
	s_slot_header header;
	byte unknown0c[4];
	long element_index;
	byte unknown14[0x1c - 0x14];
	short flags;
	byte unknown1e[2];
	long unknown20;
	byte unknown24[0x38 - 0x24];
	long unknown38;
	byte unknown3c[0x40 - 0x3c];
};

short __stdcall function_1bd850(long actor_index);
short __stdcall function_1bda60(long actor_index, s_slot *slot, bool active);
void __stdcall function_1bd890(long actor_index, s_slot *slot);
void __stdcall function_1bdad0(long actor_index, s_slot *slot, long index);
void __stdcall function_1bde80(long actor_index, s_slot *slot, long a, long b);

// @retail 0x1bdd70
void __stdcall function_1bdd70(long actor_index, s_slot *slot, s_slot_target_list *list)
{
	s_actor_view *actor = actor_get(actor_index);
	s_slot_5d *state = (s_slot_5d *)slot;
	s_502424_target *target = &element_502424_get(state->element_index)->target;

	if (list->count == 0)
	{
		actor->unknown41c = 2;
		actor->unknown420 = 2;
		if (actor->unknown086 >= 7)
		{
			actor->unknown488 = true;
			actor->unknown41c = 3;
			actor->unknown420 = 2;
		}
	}
	else if ((state->flags & 2) && (state->flags & 0x10))
	{
		actor->unknown41c = 3;
		actor->unknown420 = 4;
		actor->unknown424.point = target->point;
		actor->unknown44d = true;
		actor->unknown4cc = 0.2f;
	}
	else
	{
		actor->unknown430 = 2;
		actor->unknown434 = 6;
		actor->unknown438.object_index = state->unknown20;
		actor->unknown444 = NONE;
	}
}

// @retail 0x1bdfd0
void __stdcall function_1bdfd0(long actor_index, s_slot *slot, long index)
{
	s_slot_5d *state = (s_slot_5d *)slot;
	s_502424_element *element = element_502424_get(state->element_index);

	if ((state->flags & 1) && state->unknown20 == index || element->target.unknown0 == index)
	{
		state->unknown20 = NONE;
		state->flags &= ~0x11;
		element->target.unknown0 = NONE;
	}
}

// @retail 0x1be020
void __stdcall function_1be020(long actor_index, s_slot *slot)
{
	s_slot_5d *state = (s_slot_5d *)slot;

	state->flags &= ~0x10;
	state->unknown38 = 0;
}

s_slot_handler_2x g_47ec48 =
{
	{
		{
			0x5d, 2, 0, -2, 0,
			function_1bd850, function_1bda60, joint_initiate, joint_leave, 1, {0},
			0, 0, function_1bdfd0, function_1be020, 0, 0, 1
		},
		(t_slot_proc)joint_update, joint_activate, joint_deactivate
	},
	function_1bd890, 0, function_1bdad0, slot_release_nothing, function_1bdd70, function_1bde80,
	1, 3, 0.2f, 0
};
