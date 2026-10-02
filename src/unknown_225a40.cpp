// @flags /O2 /Gr
/* UNKNOWN_225A40.CPP: the lifecycle callbacks of entry 47 */

#include "cseries.h"
#include "game_state.h"
#include <string.h>

struct s_unknown_225a40
{
	long values[10];
};

s_unknown_225a40 *g_51ebf4;

// @retail 0x225a40
void function_225a40(void)
{
	g_51ebf4 = (s_unknown_225a40 *)game_state_malloc("unknown", "unknown", sizeof(s_unknown_225a40));
}

// @retail 0x225a80
void function_225a80(void)
{
	memset(g_51ebf4, 0, sizeof(*g_51ebf4));
}
