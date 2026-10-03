// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"

/* slot type 0x12 */

struct s_slot_12
{
	s_slot_header header;
	bool unknown0c;
	byte unknown0d[3];
	long element_index;
	byte unknown14[0x1c - 0x14];
	long unknown1c;
	long unknown20;
	byte unknown24[0x40 - 0x24];
};

short __stdcall function_1b7e40(long actor_index);
short __stdcall function_1b81c0(long actor_index, s_slot *slot, bool active);
bool __stdcall function_26e4b0(long actor_index, s_slot *slot);
void __stdcall function_26e600(long actor_index, s_slot *slot);
void __stdcall function_26e650(long actor_index, s_slot *slot);
void __stdcall function_26e6d0(long actor_index, s_slot *slot);
void __stdcall function_26e710(long actor_index, s_slot *slot);
void __stdcall function_1b8070(long actor_index, s_slot *slot);
bool __stdcall function_1b82d0(long actor_index, s_slot *slot, long index);
struct s_invite_data;
short __stdcall function_1b83b0(long actor_index, long leader_index, s_slot *slot, s_invite_data *data);
bool __stdcall function_1f4810(long actor_index, long prop_index, real distance, long unknown);
void function_265c30(long prop_index, long actor_index, bool unknown);

struct s_invite_data
{
	long unknown0;
	long unknown4;
};

bool invite_actor(long actor_index, long other_index, long a, short type, real weight);

// @retail 0x1b8360
void __stdcall function_1b8360(long actor_index, s_slot *slot, long index)
{
	s_slot_12 *state = (s_slot_12 *)slot;

	state->unknown20--;
}

// @retail 0x1b8370
void __stdcall function_1b8370(long actor_index, s_slot *slot, s_slot_target_list *list)
{
	s_actor_view *actor = actor_get(actor_index);

	actor->unknown41c = 4;
	actor->unknown420 = 2;
	actor->unknown449 = true;
	actor->unknown44a = true;
}

// @retail 0x1b8460
void __stdcall function_1b8460(long actor_index, s_slot *slot, long index)
{
	s_slot_12 *state = (s_slot_12 *)slot;
	s_502424_element *element = element_502424_get(state->element_index);

	if (element->target.unknown0 == index)
		element->target.unknown0 = NONE;
}

// @retail 0x1b82d0
bool __stdcall function_1b82d0(long actor_index, s_slot *slot, long index)
{
	s_actor_view *actor = actor_get(actor_index);
	s_slot_12 *state = (s_slot_12 *)slot;

	if (actor->prop_index != state->unknown1c)
		return false;

	bool result = true;

	if (actor->unknown040)
	{
		result = function_1f4810(actor_index, actor->prop_index, state->unknown0c ? 0.3f : 3.5f, 0);
		if (!result)
		{
			function_265c30(actor->prop_index, actor_index, true);
			actor_get(actor_index)->unknown040 = result;
		}
	}
	return result;
}

// @retail 0x1b83b0
short __stdcall function_1b83b0(long actor_index, long leader_index, s_slot *slot, s_invite_data *data)
{
	s_actor_view *actor = actor_get(actor_index);
	long count = 0;

	data->unknown4 = ((s_slot_12 *)slot)->unknown1c;
	if (actor->unknown07c == NONE)
		return 0;

	long index = element_502420_get(actor->unknown07c)->first_actor_index;

	while (index != NONE)
	{
		s_actor_view *other = actor_get(index);
		long other_index = index;

		index = other->next_index;
		if (actor != other && invite_actor(leader_index, other_index, 3, 0, 1.0f))
			count++;
	}
	return (short)count;
}

s_slot_handler_2x g_47e898 =
{
	{
		{
			0x12, 2, 0, -2, 0,
			function_1b7e40, function_1b81c0, function_26e4b0, function_26e600, NONE, {0},
			0, 0, function_1b8460, 0, 0, 0, 1
		},
		function_26e650, function_26e6d0, function_26e710
	},
	function_1b8070, 0, (t_slot_release)function_1b82d0, function_1b8360, function_1b8370, (t_slot_proc4)function_1b83b0,
	1, 10, 1.0f, 0
};
