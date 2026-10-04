// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_257160.CPP: slot handler 0x79 (handler at 0x47fa48) */

#include "unknown_11c920.h"
#include "slot_handler.h"
#include "unknown_2551c0.h"

/* the slot state of handler 0x79 */
struct s_slot_79_state
{
	s_slot_header header;
	short timer;
	byte unknown0e[0x40 - 0xe];
};

/* what function_1e4b10 returns for the actor's character */
struct s_character_79_view
{
	byte unknown0[4];
	real threshold;
	real duration;
};

/* the actor's value at +0x318 */
struct s_actor_79_view
{
	byte unknown000[0x318];
	real unknown318;
};

short g_470c48 = -1;
short g_470c4c = -2;

long function_1e4b10(long index);
bool function_10f630(long object_index, long *first, long *second);
bool function_e68c0(long type, long unit_index);

short __stdcall function_257160(long actor_index);
short __stdcall function_257290(long actor_index, s_slot *slot, bool active);
bool __stdcall function_2571e0(long actor_index, s_slot *slot);

s_slot_handler_2 g_47fa48 =
{
	{
		0x79, 2, NONE, -2, 0,
		function_257160, function_257290, function_2571e0, 0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	0, 0, slot_proc_nothing
};

// @retail 0x257160
short __stdcall function_257160(long actor_index)
{
	short result = 0;
	s_actor_view *actor = actor_get(actor_index);
	long unit_index = actor->unknown018;

	if (!function_110ab0(unit_index) && !actor->unknown264)
	{
		s_character_79_view *character = (s_character_79_view *)function_1e4b10(actor->unknown054);

		if (character)
		{
			real threshold = 0.35f;
			if (character->threshold > 0.f)
			{
				threshold = character->threshold;
			}

			if (((s_actor_79_view *)actor)->unknown318 > threshold)
			{
				result = 3;
			}
		}
	}
	return result;
}

// @retail 0x2571e0
bool __stdcall function_2571e0(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);
	s_slot_79_state *state = (s_slot_79_state *)slot;
	s_character_79_view *character = (s_character_79_view *)function_1e4b10(actor->unknown054);

	if (character)
	{
		real ticks;
		long rounded;

		((s_actor_79_view *)actor)->unknown318 = 0.f;
		if (character->duration > 0.f)
		{
			ticks = g_510c54->field_2_3 * character->duration;
			__asm
			{
				fld ticks
				fistp rounded
			}
			state->timer = (short)rounded;
			return true;
		}

		ticks = g_510c54->field_2_3 * 5.f;
		__asm
		{
			fld ticks
			fistp rounded
		}
		state->timer = (short)rounded;
	}
	return true;
}

// @retail 0x257290
short __stdcall function_257290(long actor_index, s_slot *slot, bool active)
{
	short result = g_470c4c;
	s_actor_view *actor = actor_get(actor_index);
	s_slot_79_state *state = (s_slot_79_state *)slot;
	long first;
	long second;

	if (function_10f630(actor->unknown018, &first, &second))
	{
		if (state->timer > 0)
		{
			state->timer--;
		}

		if (second != 0xc0006b3)
		{
			if (state->timer <= 0)
			{
				return g_470c48;
			}
			function_e68c0(0x34, actor->unknown018);
		}
	}
	return result;
}
