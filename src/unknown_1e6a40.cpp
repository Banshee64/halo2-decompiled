// @flags /O2 /Gr
/* UNKNOWN_1E6A40.CPP: the lifecycle callbacks of entry 21 */

#include "cseries.h"
#include "game_state.h"
#include <string.h>

struct s_unknown_slot
{
	byte unknown000[0x80];
	long unknown080;
	byte unknown084[0x10c];
	long unknown190;
	byte unknown194[0x1c];
};

struct s_unknown_1e6a40
{
	s_unknown_slot slots[4];
	long unknown6c0;
	byte unknown6c4;
	byte unknown6c5;
	byte unknown6c6[2];
};

s_unknown_1e6a40 *g_51e9c0;

// @retail 0x1e6a40
void function_1e6a40(void)
{
	g_51e9c0 = (s_unknown_1e6a40 *)function_123d40("unknown", "unknown", sizeof(s_unknown_1e6a40));
}

// @retail 0x1e6a80
void function_1e6a80(void)
{
	s_unknown_1e6a40 *data = g_51e9c0;

	memset(data, 0, sizeof(*data));
	data->unknown6c5 = 1;
	for (long i = 0; i < 4; i++)
	{
		data->slots[i].unknown080 = NONE;
		data->slots[i].unknown190 = NONE;
	}
	data->unknown6c0 = 0;
	data->unknown6c4 = 0;
}
