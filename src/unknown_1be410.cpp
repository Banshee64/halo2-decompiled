// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"

/* slot type 0x20 */

struct s_slot_20
{
	s_slot_header header;
	byte unknown0c[4];
	long unknown10;
	long unknown14;
	s_location_view unknown18;
	byte unknown2c[0x40 - 0x2c];
};

short __stdcall function_1be4b0(long actor_index);
short __stdcall function_1be6e0(long actor_index, s_slot *slot, bool active);
bool __stdcall function_1be630(long actor_index, s_slot *slot);
void __stdcall function_1c1520(long actor_index, s_slot *slot, long index);
void __stdcall function_1be840(long actor_index, s_slot *slot);
void __stdcall function_1be8f0(long actor_index, s_slot *slot);
void function_26bfa0(long object_index, long *location_index, s_location_view *location);
void function_1f86a0(long index);

// @retail 0x1be630
bool __stdcall function_1be630(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);
	bool result = false;

	if (actor->prop_index != NONE && actor->unknown348 != NONE)
	{
		s_slot_20 *state = (s_slot_20 *)slot;

		function_26bfa0(actor->unknown348, &state->unknown14, &state->unknown18);
		if (state->unknown14 != NONE)
		{
			state->unknown10 = actor->unknown348;
			function_1f86a0(actor_index);
			return true;
		}
	}
	return result;
}

// @retail 0x1be6b0
void __stdcall function_1be6b0(long actor_index, s_slot *slot)
{
	actor_get(actor_index)->unknown228 = false;
}

// @retail 0x1be9d0
void __stdcall function_1be9d0(long actor_index, s_slot *slot)
{
	s_slot_20 *state = (s_slot_20 *)slot;

	state->unknown10 = NONE;
}

s_slot_handler_2 g_47ed58 =
{
	{
		0x20, 2, 0, -2, 0,
		function_1be4b0, function_1be6e0, function_1be630, function_1be6b0, NONE, {0},
		0, 0, function_1c1520, function_1be9d0, 0, 0, 0
	},
	function_1be840, slot_proc_nothing, function_1be8f0
};
