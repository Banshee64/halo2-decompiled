// @flags /O2 /Gr
/* UNKNOWN_225F80.CPP: the lifecycle callbacks of entry 66 */

#include "cseries.h"
#include "game_state.h"

long *g_51ebf8;
long g_4701ec;

// @retail 0x225f80
void function_225f80(void)
{
	g_51ebf8 = (long *)game_state_malloc("unknown", "unknown", sizeof(long));
}

// @retail 0x225fc0
void function_225fc0(void)
{
	*g_51ebf8 = NONE;
	g_4701ec = 0;
}
