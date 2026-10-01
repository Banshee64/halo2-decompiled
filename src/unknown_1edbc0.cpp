/* UNKNOWN_1EDBC0.CPP: a caller optimized for speed (default /O2), which has
game_state_malloc inlined. */

#include "cseries.h"
#include "game_state.h"

static void *g_1edbc0_data;

// @retail 0x1edbc0
void game_state_initialize_1edbc0(void)
{
	if (!g_1edbc0_data)
	{
		g_1edbc0_data = game_state_malloc("unknown", "unknown", 8000);
	}
}
