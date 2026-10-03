// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"

/* slot type 0x24 */

short __stdcall function_1b2630(long actor_index, s_slot *slot, bool active);
bool __stdcall function_1b23c0(long actor_index, s_slot *slot);
void __stdcall function_1b2770(long actor_index, s_slot *slot);
void __stdcall function_1b2bb0(long actor_index, s_slot *slot);
bool function_26ba60(long prop_index, long actor_index, long clump_index);

// @retail 0x1b2340
short __stdcall function_1b2340(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = 0;

	if (actor->unknown007)
		return 0;

	if (actor->prop_index != NONE)
	{
		s_prop_node_view *node = prop_node_get(actor->prop_index);

		if (prop_node_view(node) && node->unknown24 >= 3)
			result = 3;
	}
	return result;
}

struct s_slot_24
{
	s_slot_header header;
	bool unknown0c;
	byte unknown0d[0x40 - 0xd];
};

// @retail 0x1b2630
short __stdcall function_1b2630(long actor_index, s_slot *slot, bool active)
{
	s_actor_view *actor = actor_get(actor_index);
	s_slot_24 *state = (s_slot_24 *)slot;

	if (state->unknown0c)
	{
		if (actor->prop_index != NONE)
		{
			s_prop_node_view *node = prop_node_get(actor->prop_index);
			s_prop_view_fields *view = prop_node_view(node);

			if (view && function_26ba60(node->unknown08, actor_index, actor->unknown07c))
			{
				if (view->unknown70 == 0)
					function_1fb7e0(actor_index, 0x2e, NULL, node->object_index, NONE);
				else
					function_1fb7e0(actor_index, 0x30, NULL, node->object_index, NONE);
			}
		}
	}
	else if (actor->prop_index != NONE && prop_node_view(prop_node_get(actor->prop_index)))
	{
		return g_46fbe8;
	}
	return g_46fbe4;
}

s_slot_handler_2 g_47e080 ={
	{
		0x24, 2, 0, -2, 0,
		function_1b2340, function_1b2630, function_1b23c0, 0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1b2770, 0, function_1b2bb0
};
