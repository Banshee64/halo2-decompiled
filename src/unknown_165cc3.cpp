/* UNKNOWN_165CC3.CPP: a caller optimized for size (/O1), which calls
game_state_malloc rather than inlining it. */

#include "cseries.h"
#include "game_state.h"

void *g_165cc3_data;
void *g_165cc3_aligned_data;

// @retail 0x165cc3
void game_state_initialize_165cc3(void)
{
	g_165cc3_data = game_state_malloc("unknown", "unknown", 0x8330);
	g_165cc3_aligned_data = game_state_malloc_aligned("unknown", "unknown", 0x8000, 4);
}
