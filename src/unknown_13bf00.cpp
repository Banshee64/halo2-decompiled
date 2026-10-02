// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_13BF00.CPP: the lifecycle callbacks of entry 53 */

#include "cseries.h"
#include "game_state.h"
#include "globals.h"
#include <string.h>

struct s_unknown_ids
{
	long values[4];
};

struct s_unknown_13bf00
{
	real unknown0;
	bool flag4;
	bool flag5;
	byte unknown06[2];
	s_unknown_ids ids;
	byte unknown18[0xa];
	bool flag22;
	byte unknown23;
};

s_unknown_13bf00 *g_510c50;

PRIVATE void ids_clear(s_unknown_ids *ids)
{
	for (long i = 0; i < 4; i++)
	{
		ids->values[i] = NONE;
	}
}

// @retail 0x13bf00
void function_13bf00(void)
{
	g_510c50 = (s_unknown_13bf00 *)game_state_malloc("unknown", "unknown", sizeof(s_unknown_13bf00));
	memset(g_510c50, 0, sizeof(*g_510c50));
}

// @retail 0x13bf60
void function_13bf60(void)
{
	s_unknown_13bf00 *data = g_510c50;

	memset(data, 0, sizeof(*data));
	ids_clear(&data->ids);
	if (g_4e6948->state == 1)
	{
		data->flag4 = true;
		data->unknown0 = 1.0f;
	}
}

// @retail 0x13bfc0
void function_13bfc0(void)
{
	if (g_4e6948->state != 1)
	{
		g_510c50->flag4 = false;
	}
	g_510c50->flag5 = false;
	g_510c50->flag22 = false;
}
