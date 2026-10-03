// @flags /O2 /Gr
#include "cseries.h"
#include "slot_handler.h"

/* slot type 0x3a (its first callbacks, from 0x1afde0, precede the region) */

struct s_slot_3a
{
	s_slot_header header;
	byte unknown0c[4];
	s_reference reference;
	byte unknown14;
	bool unknown15;
	bool unknown16;
	byte unknown17[0x40 - 0x17];
};

short __stdcall function_1afde0(long actor_index);
bool __stdcall function_1afe50(long actor_index, s_slot *slot);
void __stdcall function_1c0b60(long actor_index, s_slot *slot, long index);
void __stdcall function_1aff10(long actor_index, s_slot *slot);
void __stdcall function_1b0020(long actor_index, s_slot *slot);
void __stdcall function_1b0110(long actor_index, s_slot *slot);

// @retail 0x1b0270
short __stdcall function_1b0270(long actor_index, s_slot *slot, bool active)
{
	s_slot_3a *state = (s_slot_3a *)slot;
	short result = g_46fbe8;

	if (state->unknown15 || state->unknown16)
	{
		g_46eeb8[0x3a]->unknown8 = g_46f348;
		result = g_46fbe4;
	}
	return result;
}

// @retail 0x1b06f0
void __stdcall function_1b06f0(long actor_index, s_slot *slot, bool active)
{
	s_slot_3a *state = (s_slot_3a *)slot;

	if (!active || (state->reference.unknown2 & 0x8000))
		state->reference = g_470fa0;
}

s_slot_handler_2 g_47dec0 =
{
	{
		0x3a, 2, 0, -2, 0,
		function_1afde0, function_1b0270, function_1afe50, slot_proc_nothing, 0x38, {0},
		function_1c0b60, 0, 0, 0, function_1b06f0, 0, 0
	},
	function_1aff10, function_1b0020, function_1b0110
};
