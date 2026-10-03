// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"

/* slot type 0x6b, and the evaluate callback many handlers share */

/* globals of the slot handlers (slot_handler.h) */
short g_46fbe4 = -1;
short g_46fbe8 = -2;
s_reference g_470fa0 = {NONE, NONE};
long g_46f348 = NONE;
dword g_4ee4ec;
dword g_557c40[5];
s_data_array *g_502424;
s_data_array *g_51e9d8;

short __stdcall function_1bcc90(long actor_index);
void __stdcall function_1bcd00(long actor_index, s_slot *slot);

// @retail 0x1bce80
void __stdcall function_1bce80(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	actor->unknown4ae = true;
	actor->unknown484 = true;
	actor->unknown485 = true;
	if (actor->unknown26c != NONE)
		actor->unknown4b4 = 0.6f;
}

// @retail 0x1bced0
short __stdcall function_1bced0(long actor_index, s_slot *slot, bool active)
{
	return g_46fbe8;
}

s_slot_handler_2 g_47eb58 =
{
	{
		0x6b, 2, 0xfff, -2, 0,
		function_1bcc90, function_1bced0, slot_start_true, 0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1bcd00, 0, function_1bce80
};
