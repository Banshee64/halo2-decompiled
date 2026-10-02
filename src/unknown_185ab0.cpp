// @flags /O2 /Gr
/* UNKNOWN_185AB0.CPP: a lifecycle callback (entry 20, initialize) */

#include "cseries.h"
#include "game_state.h"

struct s_unknown_185ab0
{
	byte flag;
	byte unknown01[0x263];
};

s_unknown_185ab0 *g_4ed284;

// @retail 0x185ab0
void function_185ab0(void)
{
	g_4ed284 = (s_unknown_185ab0 *)game_state_malloc("unknown", "unknown", sizeof(s_unknown_185ab0));
	g_4ed284->flag = 0;
}
