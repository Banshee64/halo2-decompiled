// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"

/* slot type 0x12 */

struct s_slot_12
{
	s_slot_header header;
	byte unknown0c[4];
	long element_index;
	byte unknown14[0x20 - 0x14];
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
void __stdcall function_1b82d0(long actor_index, s_slot *slot, long index);
void __stdcall function_1b83b0(long actor_index, s_slot *slot, long a, long b);

// @retail 0x1b8360
void __stdcall function_1b8360(long actor_index, s_slot *slot, long index)
{
	s_slot_12 *state = (s_slot_12 *)slot;

	state->unknown20--;
}

// @retail 0x1b8370
void __stdcall function_1b8370(long actor_index, s_slot *slot, long index)
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

	if (element->unknown80 == index)
		element->unknown80 = NONE;
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
	function_1b8070, 0, function_1b82d0, function_1b8360, function_1b8370, function_1b83b0,
	1, 10, 1.0f, 0
};
