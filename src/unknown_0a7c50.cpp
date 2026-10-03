// @flags /O2 /Gr
/* UNKNOWN_0A7C50.CPP: sends a game engine event to the other machines of a
   distributed game, with its player indices reduced to absolute indices */

#include "cseries.h"
#include "globals.h"
#include "game_engine_events.h"

long g_4cedf0;

void __stdcall function_b5a70(long entity_index, long type, long a, long b, long size, void const *data, long c);

// @retail 0xa7c50
void function_a7c50(s_event *event)
{
	long mode = g_4e6948->mode;

	if (mode >= 4 && mode <= 5 && mode != 2 && mode != 4)
	{
		s_event message = *event;

		if (message.a != NONE)
			message.a &= 0xffff;
		if (message.cause_player_index != NONE)
			message.cause_player_index &= 0xffff;
		if (message.effect_player_index != NONE)
			message.effect_player_index &= 0xffff;
		function_b5a70(NONE, 7, 0, 0, sizeof(message), &message, g_4cedf0);
	}
}
