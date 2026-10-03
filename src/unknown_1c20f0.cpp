// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"

/* slot handler 0x81 (g_47eff8) */

struct s_slot_81
{
	s_slot_header header;
	long unknown0c;
	bool unknown10;
	byte unknown11[0x40 - 0x11];
};

/* the priority this handler always reports */
short g_47ffa8 = 3;

void __stdcall function_1c2130(long actor_index, s_slot *slot);
void __stdcall function_1c22c0(long object_index, long unused);

// @retail 0x1c20f0
short __stdcall function_1c20f0(long actor_index)
{
	return g_47ffa8;
}

// @retail 0x1c2100
short __stdcall function_1c2100(long actor_index, s_slot *slot, bool active)
{
	s_slot_81 *state = (s_slot_81 *)slot;
	short result = g_46fbe8;

	if (state->unknown10)
	{
		result = g_46fbe4;
	}
	return result;
}

// @retail 0x1c2120
bool __stdcall function_1c2120(long actor_index, s_slot *slot)
{
	s_slot_81 *state = (s_slot_81 *)slot;

	state->unknown0c = 0;
	return true;
}

s_slot_handler_2 g_47eff8 =
{
	{
		0x81, 2, 0xfff, -2, 0,
		function_1c20f0, function_1c2100, function_1c2120, 0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1c2130, 0, (t_slot_proc)function_1c22c0
};