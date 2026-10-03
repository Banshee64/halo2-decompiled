// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"

/* slot type 0x6d */

short __stdcall function_1b2ff0(long actor_index);
bool __stdcall function_1b3360(long actor_index, s_slot *slot);
void __stdcall function_1b3380(long actor_index, s_slot *slot);

// @retail 0x1b3540
void __stdcall function_1b3540(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	actor->unknown4ae = true;
	actor->unknown484 = true;
	if (actor->unknown26c != NONE)
		actor->unknown4b4 = 0.6f;
}

s_slot_handler_2 g_47e1c0 =
{
	{
		0x6d, 2, 0xfff, -2, 0,
		function_1b2ff0, function_1bced0, function_1b3360, 0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1b3380, 0, function_1b3540
};
