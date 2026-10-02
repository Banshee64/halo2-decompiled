// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"

/* slot type 0x7e */

struct s_slot_7e
{
	s_slot_header header;
	byte unknown0c[4];
	long element_index;
	byte unknown14[0x40 - 0x14];
};

short __stdcall function_1b0cd0(long actor_index);
bool __stdcall function_26e4b0(long actor_index, s_slot *slot);
void __stdcall function_26e600(long actor_index, s_slot *slot);
void __stdcall function_26e650(long actor_index, s_slot *slot);
void __stdcall function_26e6d0(long actor_index, s_slot *slot);
void __stdcall function_26e710(long actor_index, s_slot *slot);
void __stdcall function_1b0d50(long actor_index, s_slot *slot);
void __stdcall function_1b13b0(long actor_index, s_slot *slot, long index);
void __stdcall function_1b1a90(long actor_index, s_slot *slot, long index);
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
			function_1b0cd0, function_1b0d10, function_26e4b0, function_26e600, NONE, {0},
			0, 0, 0, 0, 0, 0, 1
		},
		function_26e650, function_26e6d0, function_26e710
	},
	function_1b0d50, 0, function_1b13b0, 0, function_1b1a90, function_1b1d90,
	3, 10, 1.0f, 0
};
