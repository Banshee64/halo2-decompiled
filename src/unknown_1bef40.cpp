// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"
#include "real_math.h"

/* slot types 0xa and 0xb */

struct s_slot_0a
{
	s_slot_header header;
	s_reference reference;
	short unknown10;
	short unknown12;
	byte unknown14[0x40 - 0x14];
};

struct s_slot_0b
{
	s_slot_header header;
	word unknown0c;
	byte unknown0e[2];
	long unknown10;
	byte unknown14[0x40 - 0x14];
};

short function_1a6fe0(long owner_index, short type);
short __stdcall function_1b7920(long actor_index);

short __stdcall function_1bef40(long actor_index);
short __stdcall function_1bf0f0(long actor_index, s_slot *slot, bool active);
void __stdcall function_1bf230(long actor_index, s_slot *slot);
bool __stdcall function_1bf4e0(long actor_index, s_slot *slot);
void __stdcall function_1c1520(long actor_index, s_slot *slot, long index);
void __stdcall function_1bf5c0(long actor_index, s_slot *slot);

/* the block of the actor's character tag function_1e4d10 returns */
struct s_character_d10
{
	byte unknown00[0x2c];
	real unknown2c;
};

void *function_1e4d10(long actor_index);

// @retail 0x1bef40
short __stdcall function_1bef40(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = g_46fbe4;

	if (actor->prop_index != NONE)
	{
		s_character_d10 *character = (s_character_d10 *)function_1e4d10(actor_index);

		if (character)
		{
			s_prop_node_view *node = prop_node_get(actor->prop_index);

			if (node->unknown24 >= 1 && node->unknown24 <= 2 && character->unknown2c * 2.0f > node->unknown28)
				result = 3;
		}
	}
	return result;
}

// @retail 0x1befd0
bool __stdcall function_1befd0(long actor_index, s_slot *slot)
{
	s_slot_0a *state = (s_slot_0a *)slot;
	real delay = _real_random_range(&g_4e7408->unknown0, __FILE__, __LINE__, 3.0f, 5.0f);
	real wait = _real_random_range(&g_4e7408->unknown0, __FILE__, __LINE__, 0.0f, 0.5f);
	real ticks;
	long rounded;

	actor_reset_state(actor_index);
	state->reference = g_470fa0;

	ticks = g_510c54->ticks_per_second * delay;
	__asm
	{
		fld ticks
		fistp rounded
	}
	state->unknown10 = (short)rounded;

	ticks = g_510c54->ticks_per_second * wait;
	__asm
	{
		fld ticks
		fistp rounded
	}
	state->unknown12 = (short)rounded;
	return true;
}

// @retail 0x1bf360
void __stdcall function_1bf360(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	if (actor->prop_index != NONE)
	{
		short value = prop_node_get(actor->prop_index)->unknown24;

		if (value >= 1 && value <= 2)
		{
			actor->unknown41c = 3;
			actor->unknown420 = 2;
			actor->unknown488 = true;
		}
	}
}

// @retail 0x1bf3d0
void __stdcall function_1bf3d0(long actor_index, s_slot *slot, bool active)
{
	s_slot_0a *state = (s_slot_0a *)slot;

	if (!active || (state->reference.unknown2 & 0x8000))
		state->reference = g_470fa0;
}

// @retail 0x1bf3f0
bool function_1bf3f0(long other_index, long actor_index, short type)
{
	s_actor_view *actor = actor_get(actor_index);
	bool result = false;

	if (other_index != NONE)
	{
		long other_prop_index = actor_get(other_index)->prop_index;

		if (other_prop_index != NONE && actor->prop_index != NONE &&
			prop_node_get(actor->prop_index)->unknown08 == prop_node_get(other_prop_index)->unknown08 &&
			function_1a6fe0(other_index, type) != NONE)
		{
			return true;
		}
	}
	return result;
}

// @retail 0x1bf480
short __stdcall function_1bf480(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = 0;

	if (function_1b7920(actor_index) && function_1bf3f0(actor->unknown3cc, actor_index, actor->unknown3d0))
		result = 3;
	return result;
}

// @retail 0x1bf590
short __stdcall function_1bf590(long actor_index, s_slot *slot, bool active)
{
	s_slot_0b *state = (s_slot_0b *)slot;
	short result = g_46fbe4;

	if (function_1bf3f0(state->unknown10, actor_index, state->unknown0c))
		result = g_46fbe8;
	return result;
}

s_slot_handler_2 g_47ee18 =
{
	{
		0xa, 2, 0x7ff, -2, 0,
		function_1bef40, function_1bf0f0, function_1befd0, slot_proc_nothing, NONE, {0},
		0, 0, 0, 0, function_1bf3d0, 0, 0
	},
	function_1bf230, 0, function_1bf360
};

s_slot_handler_2 g_47ee68 =
{
	{
		0xb, 2, 0, -2, 0,
		function_1bf480, function_1bf590, function_1bf4e0, 0, NONE, {0},
		function_1c1520, 0, 0, 0, 0, 0, 0
	},
	0, 0, function_1bf5c0
};
