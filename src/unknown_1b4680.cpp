// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"
#include "joint_behavior.h"

/* slot group 0x1b and the slot tests 0x44, 0x43, 0x3d, 0x40, 0x3c, 0x41,
   0x3e, 0x3f, 0x4a and 0x39 */

struct s_slot_1b
{
	s_slot_header header;
	byte unknown0c[4];
	long element_index;
	byte unknown14[0x24 - 0x14];
	long unknown24;
	byte unknown28[0x40 - 0x28];
};

/* the state the slot tests fill in for slot type 0x38 */
struct s_slot_38
{
	s_slot_header header;
	bool unknown0c;
	byte unknown0d[3];
	long unknown10;
	short unknown14;
	byte unknown16[0x40 - 0x16];
};

short __stdcall function_1b4680(long actor_index);
short __stdcall function_1b4bd0(long actor_index, s_slot *slot, bool active);
void __stdcall function_1b47b0(long actor_index, s_slot *slot);
void __stdcall function_1b4d90(long actor_index, s_slot *slot, long index);
short __stdcall function_1b4d10(long actor_index, short level, long a, long b);
short __stdcall function_1b4e70(long actor_index, long leader_index, long a, long b);
short __stdcall function_1b4fe0(long actor_index, s_slot *slot);
short __stdcall function_1b50e0(long actor_index, s_slot *slot);
short __stdcall function_1b5180(long actor_index, s_slot *slot);
short __stdcall function_1b51f0(long actor_index, s_slot *slot);
short __stdcall function_1b52a0(long actor_index, s_slot *slot);
short __stdcall function_1b53a0(long actor_index, s_slot *slot);
short __stdcall function_1b5470(long actor_index, s_slot *slot);
short __stdcall function_1afde0(long actor_index);
short __stdcall function_1b54d0(long actor_index, s_slot *slot);

/* the block of the actor's character tag function_1e4ef0 returns */
struct s_character_ef0
{
	byte unknown00[4];
	real unknown4;
	byte unknown08[4];
	real unknown0c;
	real unknown10;
};

void *function_1e4ef0(long actor_index);
short function_1a77a0(long actor_index, long a, short level);

// @retail 0x1b4d10
short __stdcall function_1b4d10(long actor_index, short level, long a, long b)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = g_46fbe4;

	actor->unknown41c = 3;
	actor->unknown420 = 2;
	if (actor->prop_index != NONE)
	{
		s_prop_view_fields *view = prop_view_fields_get(actor->prop_index);

		if (view->unknown6d)
		{
			view->unknown6d = false;
			actor->slots[level].unknown4 = NONE;
		}
		result = function_1a77a0(actor_index, a, level);
	}
	return result;
}

// @retail 0x1b4e70
short __stdcall function_1b4e70(long actor_index, long leader_index, long a, long b)
{
	s_actor_view *actor = actor_get(actor_index);
	long count = 0;

	if (actor->unknown07c == NONE)
		return 0;

	long index = element_502420_get(actor->unknown07c)->first_actor_index;

	while (index != NONE)
	{
		s_actor_view *other = actor_get(index);
		long other_index = index;

		index = other->next_index;
		if (actor != other && invite_actor(leader_index, other_index, 3, 1.0f))
			count++;
	}
	return (short)count;
}

// @retail 0x1b4f10
void __stdcall function_1b4f10(long actor_index, s_slot *slot, long index)
{
	s_slot_1b *state = (s_slot_1b *)slot;

	if (state->unknown24 == index)
		state->unknown24 = NONE;
	if (state->element_index != NONE)
	{
		s_502424_element *element = element_502424_get(state->element_index);

		if (element->target.unknown8 == index)
			element->target.unknown8 = NONE;
	}
}

// @retail 0x1b4f60
short __stdcall function_1b4f60(long actor_index, s_slot *slot)
{
	short result = g_46fbe4;

	if (actor_get(actor_index)->unknown6fc & 1)
		result = 0x38;
	return result;
}

// @retail 0x1b4f90
short __stdcall function_1b4f90(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = g_46fbe4;

	if (actor->unknown3b0 > 0 && actor->unknown3b4 != NONE)
	{
		s_slot_38 *state = (s_slot_38 *)slot;

		state->unknown10 = actor->unknown3b4;
		state->unknown14 = NONE;
		state->unknown0c = true;
		result = 0x38;
	}
	return result;
}

// @retail 0x1b50e0
short __stdcall function_1b50e0(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = g_46fbe4;
	s_character_ef0 *character = (s_character_ef0 *)function_1e4ef0(actor_index);

	if (actor->prop_index != NONE && character && actor->unknown3d8 >= character->unknown0c)
	{
		s_slot_handler *handler = g_46eeb8[0x38];

		if (handler->unknown8 == g_46f348 || (handler->mask & g_4ee4ec) != g_4ee4ec ||
			(g_557c40[0x38 >> 5] & (1 << (0x38 & 31))) == 0)
		{
			return result;
		}

		s_slot_38 *state = (s_slot_38 *)slot;

		state->unknown10 = actor->prop_index;
		state->unknown14 = 0x3d;
		state->unknown0c = true;
		result = 0x38;
	}
	return result;
}

// @retail 0x1b5180
short __stdcall function_1b5180(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = g_46fbe4;
	s_character_ef0 *character = (s_character_ef0 *)function_1e4ef0(actor_index);

	if (character && character->unknown4 >= actor->unknown2d4)
	{
		s_slot_38 *state = (s_slot_38 *)slot;

		state->unknown10 = actor->prop_index;
		state->unknown14 = 0x41;
		state->unknown0c = true;
		result = 0x38;
	}
	return result;
}

// @retail 0x1b51f0
short __stdcall function_1b51f0(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = g_46fbe4;
	s_character_ef0 *character = (s_character_ef0 *)function_1e4ef0(actor_index);

	if (character)
	{
		long prop_index = actor->prop_index;

		if (prop_index != NONE && !actor->unknown223)
		{
			s_prop_node_view *node = prop_node_get(prop_index);

			if (node->unknown24 >= 1 && node->unknown24 <= 2 && character->unknown10 > node->unknown28)
			{
				s_slot_38 *state = (s_slot_38 *)slot;

				state->unknown10 = prop_index;
				state->unknown14 = 0x40;
				state->unknown0c = true;
				result = 0x38;
			}
		}
	}
	return result;
}

// @retail 0x1b5470
short __stdcall function_1b5470(long actor_index, s_slot *slot)
{
	short result = g_46fbe4;

	if (function_1afde0(actor_index) >= 3 && g_46eeb8[0x3a]->unknown8 == g_46f348)
	{
		actor_get(actor_index)->unknown222 = true;
		result = 0x46;
	}
	return result;
}

/* the children of slot group 0x1b */
s_slot_child g_46f758[6] =
{
	{0x20, 0, -2, {0}, -1.0f, 0, 0},
	{0x1f, 0, 7, {0}, 1.0f, 0, 0},
	{0x1d, 0, NONE, {0}, 0.0f, 0, 0},
	{0x1c, 0, NONE, {0}, 0.0f, 0, 0},
	{0x1e, 0, NONE, {0}, 0.0f, 0, 0},
	{5, 0, NONE, {0}, 0.0f, 0, 0},
};

s_slot_handler_1x g_47e3c8 =
{
	{
		{
			0x1b, 1, 0, -2, 0,
			function_1b4680, function_1b4bd0, joint_initiate_b, joint_leave, NONE, {0},
			0, function_1b4f10, 0, 0, 0, 0, 1
		},
		function_26e8a0, 6, g_46f758
	},
	function_1b47b0, function_1b4d90, (t_slot_proc4)function_1b4d10, (t_slot_proc4)function_1b4e70,
	1, 10, 50.0f
};

s_slot_handler_0 g_47e42c =
{
	0x44, 0, 0, -2, 0, function_1b4f60
};

s_slot_handler_0 g_47e440 =
{
	0x43, 0, 0x7ff, -2, 0, function_1b4f90
};

s_slot_handler_0 g_47e454 =
{
	0x3d, 0, 0, -2, 0, function_1b4fe0
};

s_slot_handler_0 g_47e468 =
{
	0x40, 0, 0, -2, 0, function_1b50e0
};

s_slot_handler_0 g_47e47c =
{
	0x3c, 0, 0, -2, 0, function_1b5180
};

s_slot_handler_0 g_47e490 =
{
	0x41, 0, 0, -2, 0, function_1b51f0
};

s_slot_handler_0 g_47e4a4 =
{
	0x3e, 0, 0x7ff, -2, 0, function_1b52a0
};

s_slot_handler_0 g_47e4b8 =
{
	0x3f, 0, 0x7ff, -2, 0, function_1b53a0
};

s_slot_handler_0 g_47e4cc =
{
	0x4a, 0, 0, -2, 0, function_1b5470
};

s_slot_handler_0 g_47e4e0 =
{
	0x39, 0, NONE, -2, 0, function_1b54d0
};
