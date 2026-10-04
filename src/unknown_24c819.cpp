// @flags /O1 /Gr
/* UNKNOWN_24C819.CPP: a caller optimized for size (/O1), which calls
function_123d40 rather than inlining it. */

#include "cseries.h"
#include "game_state.h"

void *g_24c819_data;

// @retail 0x24c819
void game_state_initialize_24c819(void)
{
	g_24c819_data = function_123d40("unknown", "unknown", 0x13a8);
}
