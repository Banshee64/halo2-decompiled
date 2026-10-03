// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_2229D0.CPP: the lifecycle callbacks of entry 48 */

#include "cseries.h"
#include "game_state.h"
#include "globals.h"
#include <string.h>

s_speed_table *g_502120;

// @retail 0x2229d0
void function_2229d0(void)
{
	g_502120 = (s_speed_table *)game_state_malloc("unknown", "unknown", sizeof(s_speed_table));
}

// @retail 0x222a10
void function_222a10(void)
{
	s_speed_table *data = g_502120;

	memset(data, 0, sizeof(*data));
	for (long i = 0; i < 4; i++)
	{
		for (long j = 0; j < 8; j++)
		{
			data->entries[i].items[j].a = NONE;
			data->entries[i].items[j].b = NONE;
			data->entries[i].items[j].scale = 1.0f;
		}
	}
}
