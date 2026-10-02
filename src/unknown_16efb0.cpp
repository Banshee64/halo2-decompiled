// @flags /O2 /Gr
/* UNKNOWN_16EFB0.CPP: a lifecycle callback (entry 36, initialize) */

#include "cseries.h"
#include "game_state.h"

struct s_unknown_16efb0
{
	byte unknown00[0x14];
};

s_unknown_16efb0 *g_510c70;

// @retail 0x16efb0
void function_16efb0(void)
{
	g_510c70 = (s_unknown_16efb0 *)game_state_malloc("unknown", "unknown", sizeof(s_unknown_16efb0));
}
