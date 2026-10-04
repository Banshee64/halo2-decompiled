// @flags /O1 /Gr
/* UNKNOWN_165CC3.CPP: a caller optimized for size (/O1), which calls
function_123d40 rather than inlining it. */

#include "unknown_11c920.h"
#include "unknown_123b30.h"

void *g_165cc3_data;
void *g_165cc3_aligned_data;

// @retail 0x165cc3
void game_state_initialize_165cc3(void)
{
	g_165cc3_data = function_123d40("unknown", "unknown", 0x8330);
	g_165cc3_aligned_data = game_state_malloc_aligned("unknown", "unknown", 0x8000, 4);
}
