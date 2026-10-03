// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"

/* slot type 5, the slot test 0x6e and the slot group 0x6a */

struct s_slot_05
{
	s_slot_header header;
	byte unknown0c;
	bool unknown0d;
	byte unknown0e[2];
	short unknown10;
	bool unknown12;
	byte unknown13;
	s_reference reference;
	byte unknown18[0x40 - 0x18];
};

void __stdcall function_1b1f70(long actor_index, s_slot *slot);

// @retail 0x1b1eb0
bool __stdcall function_1b1eb0(long actor_index, s_slot *slot)
{
	s_slot_05 *state = (s_slot_05 *)slot;
	s_actor_view *actor = actor_get(actor_index);

	if (!state->unknown0d)
		state->unknown10 = 3;
	state->reference = actor->unknown418;
	actor->unknown3f2 = false;
	actor_reset_state(actor_index);
	return true;
}

// @retail 0x1b1f30
short __stdcall function_1b1f30(long actor_index, s_slot *slot, bool active)
{
	s_slot_05 *state = (s_slot_05 *)slot;
	short result = g_46fbe8;

	if (state->unknown12 && actor_get(actor_index)->unknown504 == 2)
		result = g_46fbe4;
	return result;
}

// @retail 0x1b21f0
void __stdcall function_1b21f0(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	if (actor->unknown086 > 1 && actor->prop_index != NONE)
	{
		s_prop_node_view *node = prop_node_get(actor->prop_index);

		if (node->unknown24 >= 1 && node->unknown24 <= 2)
		{
			actor->unknown420 = 2;
			actor->unknown488 = true;
			actor->unknown41c = 3;
			return;
		}

		s_prop_view_fields *view = prop_node_view(node);

		if (view && (view->unknown00 >= 5 || view->unknown69 && view->unknown4c))
		{
			actor->unknown41c = 2;
			actor->unknown420 = 2;
		}
	}
	actor->unknown484 = true;
	actor->unknown4a1 = true;
}

// @retail 0x1b22e0
void __stdcall function_1b22e0(long actor_index, s_slot *slot, bool active)
{
	s_slot_05 *state = (s_slot_05 *)slot;

	if (!active || (state->reference.unknown2 & 0x8000))
		state->reference = g_470fa0;
}

// @retail 0x1b2300
short __stdcall function_1b2300(long actor_index, s_slot *slot)
{
	return g_46fbe4;
}

// @retail 0x1b2310
bool __stdcall function_1b2310(long actor_index, s_slot *slot)
{
	actor_get(actor_index)->unknown084 = 3;
	return true;
}

s_slot_handler_2 g_47dfd0 =
{
	{
		5, 2, NONE, -2, 0,
		function_1a8370, function_1b1f30, function_1b1eb0, 0, NONE, {0},
		0, 0, 0, 0, function_1b22e0, 0, 0
	},
	function_1b1f70, 0, function_1b21f0
};

s_slot_handler_0 g_47e01c =
{
	0x6e, 0, NONE, -2, 0, function_1b2300
};

/* the children of slot group 0x6a */
s_slot_child g_46f670[5] =
{
	{0x6e, 1, NONE, {0}, 0, 0, 0},
	{0x6d, 1, NONE, {0}, 0, 0, 0},
	{0x6b, 1, NONE, {0}, 0, 0, 0},
	{5, 1, NONE, {0}, 0, 0, 0},
	{2, 1, NONE, {0}, 0, 0, 0},
};

s_slot_handler_1 g_47e030 =
{
	{
		0x6a, 1, 0, -2, 0,
		function_1a8370, function_1bced0, function_1b2310, 0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1a79e0, 5, g_46f670
};
