// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_2229D0.CPP: the lifecycle callbacks of entry 48 */

#include "cseries.h"
#include "game_state.h"
#include <string.h>

struct s_unknown_item
{
	long unknown0;
	long unknown4;
	real unknown8;
};

struct s_unknown_group
{
	s_unknown_item items[8];
	byte unknown60[0x28];
};

struct s_unknown_2229d0
{
	s_unknown_group groups[4];
	byte unknown220[0xc];
};

s_unknown_2229d0 *g_502120;

// @retail 0x2229d0
void function_2229d0(void)
{
	g_502120 = (s_unknown_2229d0 *)game_state_malloc("unknown", "unknown", sizeof(s_unknown_2229d0));
}

// @retail 0x222a10
void function_222a10(void)
{
	s_unknown_2229d0 *data = g_502120;

	memset(data, 0, sizeof(*data));
	for (long i = 0; i < 4; i++)
	{
		for (long j = 0; j < 8; j++)
		{
			data->groups[i].items[j].unknown0 = NONE;
			data->groups[i].items[j].unknown4 = NONE;
			data->groups[i].items[j].unknown8 = 1.0f;
		}
	}
}
