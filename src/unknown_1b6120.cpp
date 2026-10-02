// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"

/* slot type 0x25 */

struct s_slot_25
{
	s_slot_header header;
	byte unknown0c[4];
	long element_index;
	byte unknown14[0x40 - 0x14];
};

short __stdcall function_1b6450(long actor_index, s_slot *slot, bool active);
bool __stdcall function_26e4b0(long actor_index, s_slot *slot);
void __stdcall function_26e600(long actor_index, s_slot *slot);
void __stdcall function_26e650(long actor_index, s_slot *slot);
void __stdcall function_26e6d0(long actor_index, s_slot *slot);
void __stdcall function_26e710(long actor_index, s_slot *slot);
void __stdcall function_1b6120(long actor_index, s_slot *slot);
void __stdcall function_1b6740(long actor_index, s_slot *slot, long a, long b);

// @retail 0x1b6880
void __stdcall function_1b6880(long actor_index, s_slot *slot, long index)
{
	actor_get(actor_index)->unknown4a1 = true;
}

// @retail 0x1b68b0
void __stdcall function_1b68b0(long actor_index, s_slot *slot, long index)
{
	s_slot_25 *state = (s_slot_25 *)slot;
	s_502424_element *element = element_502424_get(state->element_index);

	if (element->unknown84 == index)
		element->unknown84 = NONE;
}

// @retail 0x1b68f0
void __stdcall function_1b68f0(long actor_index, s_slot *slot)
{
	s_slot_25 *state = (s_slot_25 *)slot;
	s_502424_element *element = element_502424_get(state->element_index);

	if (element->unknown94 == 1)
		element->unknown84 = NONE;
}

s_slot_handler_2x g_47e648 =
{
	{
		{
			0x25, 2, 0, -2, 0,
			function_1adcd0, function_1b6450, function_26e4b0, function_26e600, NONE, {0},
			0, 0, function_1b68b0, function_1b68f0, 0, 0, 1
		},
		function_26e650, function_26e6d0, function_26e710
	},
	function_1b6120, 0, 0, 0, function_1b6880, function_1b6740,
	2, 10, 150.0f, 0
};
