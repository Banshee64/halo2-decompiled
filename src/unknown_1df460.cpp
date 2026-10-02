// @flags /O2 /Gr
/* UNKNOWN_1DF460.CPP: the lifecycle callbacks of entry 18 */

#include "cseries.h"
#include "game_state.h"
#include <string.h>

struct s_unknown_1df460
{
	byte unknown00[0xc4];
	long bits[8];
};

s_unknown_1df460 *g_4f55ec;

// @retail 0x1df460
void function_1df460(void)
{
	s_unknown_1df460 *data = (s_unknown_1df460 *)game_state_malloc("unknown", "unknown", sizeof(s_unknown_1df460));

	memset(data, 0, sizeof(*data));
	g_4f55ec = data;
}

// @retail 0x1df4b0
void function_1df4b0(void)
{
	s_unknown_1df460 *data = g_4f55ec;

	memset(data, 0, sizeof(*data));
	long index = 0;
	for (long count = 16; count; count--)
	{
		data->bits[index >> 5] |= 1 << (index & 0x1f);
		index += 17;
	}
}
