// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"

/* slot type 0x23 */

struct s_slot_23
{
	s_slot_header header;
	bool unknown0c;
	bool unknown0d;
	short unknown0e;
	byte unknown10[0x40 - 0x10];
};

short __stdcall function_1b84a0(long actor_index);
void __stdcall function_1b85a0(long actor_index, s_slot *slot);
void __stdcall function_1b89d0(long actor_index, s_slot *slot);
void __stdcall function_1b8ae0(long actor_index, s_slot *slot);
bool function_25da00(s_prop_node_view *node);

// @retail 0x1b84a0
short __stdcall function_1b84a0(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = 0;

	if (actor->prop_index != NONE && !actor->unknown223)
	{
		s_prop_node_view *node = prop_node_get(actor->prop_index);
		s_prop_view_fields *view = prop_node_view(node);

		if (view && node->unknown24 >= 3 && (view->unknown70 == 1 || !function_25da00(node)))
			result = 3;
	}
	return result;
}

// @retail 0x1b8540
bool __stdcall function_1b8540(long actor_index, s_slot *slot)
{
	bool result = false;
	s_actor_view *actor = actor_get(actor_index);

	if (actor->prop_index != NONE && prop_node_get(actor->prop_index)->unknown27 < 2)
	{
		s_slot_23 *state = (s_slot_23 *)slot;

		state->unknown0c = false;
		state->unknown0d = false;
		state->unknown0e = 0;
		result = true;
	}
	return result;
}

// @retail 0x1b8c60
short __stdcall function_1b8c60(long actor_index, s_slot *slot, bool active)
{
	s_slot_23 *state = (s_slot_23 *)slot;
	short result = g_46fbe8;

	if (state->unknown0c)
		result = g_46fbe4;
	return result;
}

s_slot_handler_2 g_47e908 =
{
	{
		0x23, 2, 0, -2, 0,
		function_1b84a0, function_1b8c60, function_1b8540, slot_proc_nothing, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1b85a0, function_1b89d0, function_1b8ae0
};
