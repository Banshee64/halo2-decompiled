// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"
#include "unknown_2626b0.h"

/* the slot tests 0x5b and 0x5c, and the slot types 0x5a, 0x59 and 0x51 */

struct s_slot_5a
{
	s_slot_header header;
	long unknown0c;
	byte unknown10[0x40 - 0x10];
};

void function_1f86a0(long index);

short __stdcall function_1bbf40(long actor_index, s_slot *slot);
short __stdcall function_1bc2a0(long actor_index, s_slot *slot);
short __stdcall function_1bc6d0(long actor_index);
short __stdcall function_1bc850(long actor_index, s_slot *slot, bool active);
bool __stdcall function_1bc810(long actor_index, s_slot *slot);
void __stdcall function_1c1520(long actor_index, s_slot *slot, long index);
void __stdcall function_1c1990(long actor_index, s_slot *slot, long index);
bool __stdcall function_1bc980(long actor_index, s_slot *slot);
void __stdcall function_1bcab0(long actor_index, s_slot *slot);
void __stdcall function_1bcc10(long actor_index, s_slot *slot);

/* the state of slot type 0x59 */
struct s_slot_59
{
	s_slot_header header;
	real unknown0c;
	long unknown10;
	bool unknown14;
	bool unknown15;
	bool unknown16;
	bool unknown17;
	byte unknown18[0x40 - 0x18];
};

inline real distance3d_fast(real_point3d const *a, real_point3d const *b)
{
	real i = a->x - b->x;
	real j = a->y - b->y;
	real k = a->z - b->z;

	return (real)sqrt(i * i + j * j + k * k);
}

void function_26c180(long actor_index);
bool function_1f4460(long actor_index, void *data, long a, long b, long c);

// @retail 0x1bc420
short __stdcall function_1bc420(long actor_index)
{
	short result = 0;
	s_actor_view *actor = actor_get(actor_index);

	if (actor->unknown024 == 1 || !team_is_enemy(actor->unknown024, 1))
	{
		if (actor->unknown31c != NONE)
		{
			real seconds = g_510c54->ticks_per_second * 2.0f;
			long ticks;

			__asm
			{
				fld seconds
				fistp ticks
			}
			if (actor->unknown31e > ticks)
				result = 3;
		}
	}
	return result;
}

// @retail 0x1bc4f0
bool __stdcall function_1bc4f0(long actor_index, s_slot *slot)
{
	short team = actor_get(actor_index)->unknown024;
	bool result = false;

	if (team == 1 || !team_is_enemy(team, 1))
	{
		function_1f86a0(actor_index);
		return true;
	}
	return result;
}

// @retail 0x1bc580
short __stdcall function_1bc580(long actor_index, s_slot *slot, bool active)
{
	s_slot_5a *state = (s_slot_5a *)slot;
	short result = g_46fbe8;

	if (state->unknown0c == NONE || actor_get(actor_index)->unknown31c == NONE)
		result = g_46fbe4;
	return result;
}

/* the nearest actor of the actor's group not in a vehicle */
// @retail 0x1bc5d0
long function_1bc5d0(long actor_index, long ignore_index)
{
	s_actor_view *actor = actor_get(actor_index);
	long result = NONE;

	if (actor->unknown07c != NONE)
	{
		real best_distance = 3.4028235e38f;
		long index = element_502420_get(actor->unknown07c)->first_actor_index;

		while (index != NONE)
		{
			long other_index = index;
			s_actor_view *other = actor_get(other_index);

			index = other->next_index;
			if (other_index != actor_index && other_index != ignore_index && other->unknown26c == NONE)
			{
				real distance = distance3d_fast(&actor->position, &other->position);

				if (best_distance > distance)
				{
					best_distance = distance;
					result = other_index;
				}
			}
		}
	}
	return result;
}

// @retail 0x1bc810
bool __stdcall function_1bc810(long actor_index, s_slot *slot)
{
	s_slot_59 *state = (s_slot_59 *)slot;
	bool result = true;

	if (!state->unknown14)
	{
		state->unknown15 = true;
		state->unknown0c = 0.0f;
		state->unknown10 = function_1bc5d0(actor_index, NONE);
		state->unknown16 = true;
		state->unknown14 = true;
		result = state->unknown10 != NONE;
	}
	state->unknown17 = false;
	return result;
}

// @retail 0x1bc980
bool __stdcall function_1bc980(long actor_index, s_slot *slot)
{
	s_slot_59 *state = (s_slot_59 *)slot;
	bool result = true;

	if (state->unknown10 == NONE)
		return false;
	if (actor_get(actor_index)->unknown040)
	{
		s_actor_view *other = actor_get(state->unknown10);

		state->unknown17 = true;
		function_26c180(state->unknown10);
		if (other->unknown27c.unknown10 == NONE)
			return false;
		if (!function_1f4460(actor_index, &other->unknown27c, other->unknown27c.unknown10, NONE, 0) && state->unknown15)
		{
			state->unknown10 = function_1bc5d0(actor_index, state->unknown10);
			state->unknown17 = false;
			return state->unknown10 != NONE;
		}
	}
	return result;
}

// @retail 0x1bca20
void __stdcall function_1bca20(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	if (actor->unknown50c)
	{
		actor->unknown41c = 2;
		actor->unknown420 = 0;
	}
}

// @retail 0x1bca60
short __stdcall function_1bca60(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = 0;

	if (actor->prop_index != NONE && prop_node_get(actor->prop_index)->unknown26 >= 2)
		result = 3;
	return result;
}

// @retail 0x1bcc10
void __stdcall function_1bcc10(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	if (actor->unknown504 == 2)
		function_262800(actor_index, actor->unknown418, false);
}

// @retail 0x1bcc50
void __stdcall function_1bcc50(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	actor->unknown41c = 3;
	actor->unknown420 = 2;
	actor->unknown4ae = true;
	actor->unknown488 = true;
}

s_slot_handler_0 g_47ea3c =
{
	0x5b, 0, 0, -2, 0, function_1bbf40
};

s_slot_handler_0 g_47ea50 =
{
	0x5c, 0, 0, -2, 0, function_1bc2a0
};

s_slot_handler_2 g_47ea68 =
{
	{
		0x5a, 2, 0xbff, -2, 0,
		function_1bc420, function_1bc580, function_1bc4f0, slot_proc_nothing, NONE, {0},
		0, function_1c1990, 0, 0, 0, 0, 0
	},
	(t_slot_proc)slot_start_true, 0, slot_proc_nothing
};

s_slot_handler_2 g_47eab8 =
{
	{
		0x59, 2, 0xbff, -2, 0,
		function_1bc6d0, function_1bc850, function_1bc810, 0, NONE, {0},
		function_1c1520, 0, 0, 0, 0, 0, 0
	},
	(t_slot_proc)function_1bc980, 0, function_1bca20
};

s_slot_handler_2 g_47eb08 =
{
	{
		0x51, 2, 0, -2, 0,
		function_1bca60, function_1bced0, 0, 0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1bcab0, function_1bcc10, function_1bcc50
};
