// @flags /O2 /Gr
/* UNKNOWN_257070.CPP: slot handler 0x78 (handler at 0x47f9f8, children at
   0x470bf0) */

#include "unknown_11c920.h"
#include "slot_handler.h"
#include "props.h"

/* the ai's 0x24 bytes at g_5047f4 (ai.cpp allocates them) */
struct s_5047f4_view
{
	short unknown0;
	byte unknown2[0xe];
	bool unknown10;
};

extern void *g_5047f4;

short g_470c40 = -1;
short g_470c44 = -2;

short __stdcall function_257070(long actor_index);
short __stdcall function_2570b0(long actor_index, s_slot *slot, bool active);
short __stdcall function_2570f0(long actor_index, short level, bool active);

s_slot_child g_470bf0[4] =
{
	{0x32, 1, -1, {0}, 0.f, 0, 0},
	{0x2e, 1, -1, {0}, 0.f, 0, 0},
	{0x2b, 1, -1, {0}, 0.f, 0, 0},
	{5, 0, -1, {0}, 0.f, 0, 0},
};

s_slot_handler_1 g_47f9f8 =
{
	{
		0x78, 1, 0x7ff, -2, 0,
		function_257070, function_2570b0, slot_start_true, 0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_2570f0, 4, g_470bf0
};

// @retail 0x257070
short __stdcall function_257070(long actor_index)
{
	short result = 0;

	if (((s_5047f4_view *)g_5047f4)->unknown0 == 1 && actor_get(actor_index)->prop_index != NONE)
	{
		result = 3;
	}
	return result;
}

// @retail 0x2570b0
short __stdcall function_2570b0(long actor_index, s_slot *slot, bool active)
{
	short result = g_470c40;

	if (((s_5047f4_view *)g_5047f4)->unknown0 == 1 && actor_get(actor_index)->prop_index != NONE)
	{
		result = g_470c44;
	}
	return result;
}

// @retail 0x2570f0
short __stdcall function_2570f0(long actor_index, short level, bool active)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = function_1a79e0(actor_index, level, active);

	if (actor->prop_index != NONE)
	{
		s_prop_datum *prop = prop_ref_get(actor->prop_index);
		if (prop->state >= 1 && prop->state <= 2)
		{
			((s_5047f4_view *)g_5047f4)->unknown10 = true;
		}
	}
	return result;
}
