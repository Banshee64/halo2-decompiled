/*
GAME_STATE_OS.CPP: callers optimized for size (retail 0x24c819 and
0x165cc3), which call game_state_malloc rather than inline it.
*/

#include "game_state.h"

void *g_24c819_data;
void *g_165cc3_data;
void *g_165cc3_aligned_data;

void game_state_initialize_24c819(void)
{
	g_24c819_data = game_state_malloc("unknown", "unknown", 0x13a8);
}

void game_state_initialize_165cc3(void)
{
	g_165cc3_data = game_state_malloc("unknown", "unknown", 0x8330);
	g_165cc3_aligned_data = game_state_malloc_aligned("unknown", "unknown", 0x8000, 4);
}
