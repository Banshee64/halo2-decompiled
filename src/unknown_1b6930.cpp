// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"

/* slot type 0x26, the slot tests 0x35, 0x34, 0x32, 0x33, 0x55, 0x30, 0x57
   and 0x56, and slot group 0x2a */

struct s_slot_2a
{
	s_slot_header header;
	bool unknown0c;
	bool unknown0d;
	byte unknown0e[0x40 - 0xe];
};

/* a tag element of the actor's tag (unknown_1e5450.cpp) */
struct s_tag_element;
s_tag_element *function_1e5450(long owner_index, long key);

struct s_tag_element_view
{
	byte unknown00[0x94];
	real unknown94;
};

short __stdcall function_1ad550(long actor_index);
short __stdcall function_1b6c90(long actor_index, s_slot *slot, bool active);
bool __stdcall function_1b6930(long actor_index, s_slot *slot);
void __stdcall function_1b6d40(long actor_index, s_slot *slot);
short __stdcall function_1b6ef0(long actor_index, s_slot *slot);
short __stdcall function_1b6f80(long actor_index, s_slot *slot);
short __stdcall function_1b7000(long actor_index, s_slot *slot);
short __stdcall function_1b70f0(long actor_index, s_slot *slot);
short __stdcall function_1b7210(long actor_index, s_slot *slot);
short __stdcall function_1b73b0(long actor_index, s_slot *slot);
short __stdcall function_1b74c0(long actor_index, s_slot *slot);

// @retail 0x1b7190
short __stdcall function_1b7190(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);
	s_object_view *object = object_get(actor->unknown26c);
	short result = g_46fbe4;
	s_tag_element_view *element = (s_tag_element_view *)function_1e5450(actor_index, object->tag_index);

	if (element && element->unknown94 > 0.0f && object->unknown100 > element->unknown94)
		result = 0x2a;
	return result;
}

// @retail 0x1b75a0
short __stdcall function_1b75a0(long actor_index)
{
	return 0;
}

// @retail 0x1b75b0
short __stdcall function_1b75b0(long actor_index, s_slot *slot, bool active)
{
	short result = g_46fbe4;
	s_actor_view *actor = actor_get(actor_index);

	if (actor->prop_index != NONE && !actor->unknown225)
	{
		s_slot_2a *state = (s_slot_2a *)slot;

		if (state->unknown0d)
		{
			s_slot_handler *handler = g_46eeb8[0x36];

			if (handler->unknown8 != g_46f348 && (handler->mask & g_4ee4ec) == g_4ee4ec &&
				(g_557c40[0x36 >> 5] & (1 << (0x36 & 31))) != 0)
			{
				result = 0x36;
			}
		}
		else if (prop_node_get(actor->prop_index)->type == 1)
		{
			result = g_46fbe8;
		}
	}
	return result;
}

// @retail 0x1b7650
bool __stdcall function_1b7650(long actor_index, s_slot *slot)
{
	bool result = false;
	s_actor_view *actor = actor_get(actor_index);

	if (!actor->unknown225)
	{
		s_slot_2a *state = (s_slot_2a *)slot;

		actor->unknown220 = false;
		actor->unknown084 = 4;
		state->unknown0c = true;

		s_actor_view *actor_again = actor_get(actor_index);
		short i;

		actor_again->unknown3fe = 3;
		for (i = 0; i < 4; i++)
			actor_again->unknown400[i].reference = g_470fa0;
		result = true;
	}
	return result;
}

// @retail 0x1b76d0
short __stdcall function_1b76d0(long actor_index, short level, bool active)
{
	s_actor_view *actor = actor_get(actor_index);

	if (actor->unknown225)
		return g_46fbe4;

	short result = function_1a79e0(actor_index, level, active);

	if (result == g_46fbe4)
	{
		((s_slot_2a *)&actor->slots[level])->unknown0d = true;
		result = 2;
	}
	return result;
}

s_slot_handler_2 g_47e6b8 =
{
	{
		0x26, 2, 0, -2, 0,
		function_1ad550, function_1b6c90, function_1b6930, 0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	0, 0, function_1b6d40
};

s_slot_handler_0 g_47e704 =
{
	0x35, 0, 0x7ff, -2, 0, function_1b6ef0
};

s_slot_handler_0 g_47e718 =
{
	0x34, 0, NONE, -2, 0, function_1b6f80
};

s_slot_handler_0 g_47e72c =
{
	0x32, 0, 0, -2, 0, function_1b7000
};

s_slot_handler_0 g_47e740 =
{
	0x33, 0, 0x7ff, -2, 0, function_1b70f0
};

s_slot_handler_0 g_47e754 =
{
	0x55, 0, 0, -2, 0, function_1b7190
};

s_slot_handler_0 g_47e768 =
{
	0x30, 0, 0, -2, 0, function_1b7210
};

s_slot_handler_0 g_47e77c =
{
	0x57, 0, 0xbff, -2, 0, function_1b73b0
};

s_slot_handler_0 g_47e790 =
{
	0x56, 0, 0, -2, 0, function_1b74c0
};

/* the children of slot group 0x2a */
s_slot_child g_46fad8[8] =
{
	{0x50, 1, 13, {0}, 1.0f, 0, 0},
	{0x32, 1, NONE, {0}, 0.0f, 0, 0},
	{0x14, 1, NONE, {0}, 0.0f, 0, 0},
	{0x31, 1, NONE, {0}, 0.0f, 0, 0},
	{0x58, 1, NONE, {0}, 0.0f, 0, 0},
	{0x2e, 1, NONE, {0}, 0.0f, 0, 0},
	{0x54, 1, NONE, {0}, 0.0f, 0, 0},
	{0x2b, 0, NONE, {0}, 0.0f, 0, 0},
};

s_slot_handler_1 g_47e7a8 =
{
	{
		0x2a, 1, 0, -2, 0,
		function_1b75a0, function_1b75b0, function_1b7650, 0, 1, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1b76d0, 8, g_46fad8
};
