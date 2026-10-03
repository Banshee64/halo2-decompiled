#include "cseries.h"
#include "globals.h"
#include "game_engine_events.h"

// @flags /O2 /Gr

/* Sends a game engine event to the clients (as a simulation event of type 7,
   with the player indices made absolute). Decompiled by lane O because the
   engines (unknown_2420a0.cpp, juggernaut.cpp, unknown_19de80.cpp) pass the
   event in a register. */

long g_4cedf0;

void __stdcall function_b5a70(long a, long type, long b, long c, long size, void const *data, long d);

// @retail 0xa7c50
void function_a7c50(s_event *event)
{
	long mode = g_4e6948->mode;

	if (mode >= 4 && mode <= 5 && mode != 2 && mode != 4)
	{
		s_event copy = *event;

		if (copy.a != NONE)
			copy.a &= 0xffff;
		if (copy.cause_player_index != NONE)
			copy.cause_player_index &= 0xffff;
		if (copy.effect_player_index != NONE)
			copy.effect_player_index &= 0xffff;
		function_b5a70(NONE, 7, 0, 0, sizeof(copy), &copy, g_4cedf0);
	}
}
