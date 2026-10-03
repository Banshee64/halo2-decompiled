// @flags /O2 /Gr
#include "cseries.h"
#include "slot_handler.h"

/* slot group 4 */

struct s_slot_04
{
	s_slot_header header;
	byte unknown0c[5];
	bool unknown11;
	bool unknown12;
	byte unknown13[0x40 - 0x13];
};

void __stdcall function_1b2e00(long actor_index, s_slot *slot);
short __stdcall function_1b2e80(long actor_index, short level, bool active);

// @retail 0x1b2da0
short __stdcall function_1b2da0(long actor_index)
{
	short result = 0;

	if (actor_get(actor_index)->unknown85c != NONE)
		result = 3;
	return result;
}

// @retail 0x1b2de0
bool __stdcall function_1b2de0(long actor_index, s_slot *slot)
{
	s_slot_04 *state = (s_slot_04 *)slot;

	state->unknown12 = false;
	state->unknown11 = false;
	return true;
}

// @retail 0x1b2e40
short __stdcall function_1b2e40(long actor_index, s_slot *slot, bool active)
{
	s_slot_04 *state = (s_slot_04 *)slot;
	short result = g_46fbe8;

	if (actor_get(actor_index)->unknown85c == NONE)
	{
		state->unknown11 = true;
		result = g_46fbe4;
	}
	return result;
}

s_slot_handler_1 g_47e170 =
{
	{
		4, 1, NONE, -2, 0,
		function_1b2da0, function_1b2e40, function_1b2de0, function_1b2e00, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1b2e80, 0, 0
};
