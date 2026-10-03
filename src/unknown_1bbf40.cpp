// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"

/* the slot tests 0x5b and 0x5c, and the slot types 0x5a, 0x59 and 0x51 */

struct s_slot_5a
{
	s_slot_header header;
	long unknown0c;
	byte unknown10[0x40 - 0x10];
};

bool function_0bfe60(const dword *flags, long bit);
bool function_15e020(short a, short b);
void function_1f86a0(long index);

/* the game allegiance globals (game_allegiance.cpp): the peace bits are at
   +0xc4 */
struct s_game_allegiance_globals;
extern s_game_allegiance_globals *g_4f55ec;

struct s_allegiance_view
{
	byte unknown00[0xc4];
	dword peace_bits[8];
};

/* a copy of game_team_is_enemy (0x1df560): retail inlines it, game_allegiance.cpp is /Ob1 */
static inline bool team_is_enemy(short team_a, short team_b)
{
	bool result = true;

	if (team_a == NONE || team_b == NONE)
		return true;

	long mode = g_4e6948->state;

	if (mode == 1)
	{
		if (team_a >= 0 && team_a < 16 && team_b >= 0 && team_b < 16)
		{
			long bit = team_a * 16 + team_b;
			result = !function_0bfe60(((s_allegiance_view *)g_4f55ec)->peace_bits, bit);
		}
	}
	else if (mode == 2)
	{
		result = function_15e020(team_a, team_b);
	}
	else
	{
		result = team_a != team_b;
	}
	return result;
}

short __stdcall function_1bbf40(long actor_index, s_slot *slot);
short __stdcall function_1bc2a0(long actor_index, s_slot *slot);
short __stdcall function_1bc6d0(long actor_index);
short __stdcall function_1bc850(long actor_index, s_slot *slot, bool active);
bool __stdcall function_1bc810(long actor_index, s_slot *slot);
void __stdcall function_1c1520(long actor_index, s_slot *slot, long index);
void __stdcall function_1c1990(long actor_index, s_slot *slot, long index);
void __stdcall function_1bc980(long actor_index, s_slot *slot);
void __stdcall function_1bcab0(long actor_index, s_slot *slot);
void __stdcall function_1bcc10(long actor_index, s_slot *slot);

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
	function_1bc980, 0, function_1bca20
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
