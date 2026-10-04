// @flags /O2 /Gr
/* UNKNOWN_10A980.CPP: the lifecycle callbacks of entry 29 */

#include "cseries.h"
#include "game_state.h"
#include "unknown_10a980.h"

s_unknown_10a980 *g_5107f4;

// @retail 0x10a980
void function_10a980(void)
{
	g_5107f4 = (s_unknown_10a980 *)function_123d40("unknown", "unknown", sizeof(s_unknown_10a980));
	g_5107f4->flag = false;
}

// @retail 0x10a9d0
void function_10a9d0(void)
{
	g_5107f4 = 0;
}

// @retail 0x10a9e0
void function_10a9e0(void)
{
	for (long i = 0; i < 32; i++)
	{
		g_5107f4->values[i] = NONE;
	}
	g_5107f4->flag = true;
}

// @retail 0x10aab0
void function_10aab0(void)
{
	g_5107f4->flag = false;
}
