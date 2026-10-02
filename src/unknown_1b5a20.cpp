// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"

/* slot group 0x22 and slot type 0x27 */

short __stdcall function_1b5aa0(long actor_index, s_slot *slot, bool active);
bool __stdcall function_1b5c00(long actor_index, s_slot *slot);
short __stdcall function_1b5e00(long actor_index, short level, bool active);
short __stdcall function_1b5ee0(long actor_index, s_slot *slot, bool active);
bool __stdcall function_1b09b0(long actor_index, s_slot *slot);
void __stdcall function_1b5f30(long actor_index, s_slot *slot);

// @retail 0x1b5a20
short __stdcall function_1b5a20(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = 0;

	if (actor->prop_index != NONE)
	{
		s_prop_node_view *node = prop_node_get(actor->prop_index);
		s_prop_view_fields *view = prop_node_view(node);

		if (view && node->unknown24 >= 3 && !view->unknown69)
			result = 3;
	}
	return result;
}

// @retail 0x1b5dd0
void __stdcall function_1b5dd0(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	actor->unknown220 = false;
	actor->unknown221 = false;
}

// @retail 0x1b5fe0
void __stdcall function_1b5fe0(long actor_index, s_slot *slot)
{
	actor_get(actor_index)->unknown484 = true;
}

/* the children of slot group 0x22 */
s_slot_child g_46fa50[5] =
{
	{0x23, 0, NONE, {0}, 0.0f, 0, 0},
	{0x28, 0, NONE, {0}, 0.0f, 0, 0},
	{0x24, 0, NONE, {0}, 0.0f, 0, 0},
	{0x25, 0, NONE, {0}, 0.0f, 0, 0},
	{0x26, 0, NONE, {0}, 0.0f, 0, 0},
};

s_slot_handler_1 g_47e5a8 =
{
	{
		0x22, 1, 0, -2, 0,
		function_1b5a20, function_1b5aa0, function_1b5c00, function_1b5dd0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1b5e00, 5, g_46fa50
};

s_slot_handler_2 g_47e5f8 =
{
	{
		0x27, 2, 0, -2, 0,
		0, function_1b5ee0, function_1b09b0, 0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1b5f30, 0, function_1b5fe0
};
