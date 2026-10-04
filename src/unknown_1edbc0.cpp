// @flags /O2 /Ob1 /Gr
/* UNKNOWN_1EDBC0.CPP: a caller optimized for speed, which has
function_123d40 inlined. /Ob1 keeps this function out of line in its
caller function_1c25a0 (0x1c25a0), as retail does. */

#include "cseries.h"
#include "game_state.h"

static void *g_1edbc0_data;

// @retail 0x1edbc0
void game_state_initialize_1edbc0(void)
{
	if (!g_1edbc0_data)
	{
		g_1edbc0_data = function_123d40("unknown", "unknown", 8000);
	}
}
