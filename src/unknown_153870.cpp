// @flags /O2 /Gr
/* UNKNOWN_153870.CPP: a lifecycle callback (entry 49, initialize) */

#include "cseries.h"
#include "game_state.h"

struct s_unknown_153870
{
	byte unknown00[0x2b0];
};

s_unknown_153870 *g_510c5c;

// @retail 0x153870
void function_153870(void)
{
	g_510c5c = (s_unknown_153870 *)game_state_malloc("unknown", "unknown", sizeof(s_unknown_153870));
}
