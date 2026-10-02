// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"

/* slot type 0x24 */

short __stdcall function_1b2630(long actor_index, s_slot *slot, bool active);
bool __stdcall function_1b23c0(long actor_index, s_slot *slot);
void __stdcall function_1b2770(long actor_index, s_slot *slot);
void __stdcall function_1b2bb0(long actor_index, s_slot *slot);

// @retail 0x1b2340
short __stdcall function_1b2340(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);

	if (actor->unknown007)
		return 0;

	short result = 0;

	if (actor->prop_index != NONE)
	{
		s_prop_node_view *node = prop_node_get(actor->prop_index);

		if (prop_node_view(node) && node->unknown24 >= 3)
			result = 3;
	}
	return result;
}

s_slot_handler_2 g_47e080 =
{
	{
		0x24, 2, 0, -2, 0,
		function_1b2340, function_1b2630, function_1b23c0, 0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1b2770, 0, function_1b2bb0
};
