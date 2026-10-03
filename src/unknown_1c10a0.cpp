// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"

/* slot handler 0x82 (g_47ef58) */

struct s_slot_82
{
	s_slot_header header;
	bool unknown0c;
	byte unknown0d[3];
	long unknown10;
	real_point3d unknown14;
	bool unknown20;
	byte unknown21[0x38 - 0x21];
	short unknown38;
	byte unknown3a[0x40 - 0x3a];
};

short __stdcall function_1c1160(long actor_index, s_slot *slot, bool active);
void __stdcall function_1c1320(long actor_index, s_slot *slot);

// @retail 0x1c10a0
short __stdcall function_1c10a0(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = 0;

	if (actor->unknown22a && !actor->unknown223 && actor->prop_index != NONE)
	{
		result = 3;
	}
	return result;
}

// @retail 0x1c10f0
bool __stdcall function_1c10f0(long actor_index, s_slot *slot)
{
	s_slot_82 *state = (s_slot_82 *)slot;

	actor_reset_state(actor_index);
	state->unknown0c = false;
	state->unknown10 = NONE;
	state->unknown38 = 0;
	state->unknown20 = false;
	return true;
}

// @retail 0x1c14b0
void __stdcall function_1c14b0(long actor_index, s_slot *slot)
{
	s_slot_82 *state = (s_slot_82 *)slot;
	s_actor_view *actor = actor_get(actor_index);

	if (actor->unknown504 == 2 && state->unknown20)
	{
		actor->unknown41c = 4;
		actor->unknown420 = 4;
		actor->unknown424.point = state->unknown14;
		actor->unknown44d = true;
	}
}

// @retail 0x1c1520
void __stdcall function_1c1520(long actor_index, s_slot *slot, long index)
{
	s_slot_82 *state = (s_slot_82 *)slot;

	if (state->unknown10 == index)
	{
		state->unknown10 = NONE;
	}
}

s_slot_handler_2 g_47ef58 =
{
	{
		0x82, 2, 0x1ff8, -2, 0,
		function_1c10a0, function_1c1160, function_1c10f0, 0, NONE, {0},
		0, 0, function_1c1520, 0, 0, 0, 0
	},
	function_1c1320, slot_proc_nothing, function_1c14b0
};