/*
GAME_STATE_O2.CPP: a caller optimized for speed (retail 0x1edbc0), which
has function_123d40 inlined, and the test image's entry point.
*/

#include "unknown_123b30.h"

static void *g_1edbc0_data;

void game_state_initialize_1edbc0(void)
{
	if (!g_1edbc0_data)
	{
		g_1edbc0_data = function_123d40("unknown", "unknown", 8000);
	}
}

void game_state_initialize_24c819(void);
void game_state_initialize_165cc3(void);

/* called through pointers, so the linker keeps them out of line */
void (*volatile g_initializers[])(void) =
{
	game_state_initialize_1edbc0,
	game_state_initialize_24c819,
	game_state_initialize_165cc3,
};

extern "C" int entry(void)
{
	for (long i = 0; i < sizeof(g_initializers) / sizeof(g_initializers[0]); i++)
	{
		g_initializers[i]();
	}
	return 0;
}
