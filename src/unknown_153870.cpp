// @flags /O2 /Gr
/* UNKNOWN_153870.CPP: a lifecycle callback (entry 49, initialize) */

#include "unknown_11c920.h"
#include "unknown_123b30.h"
#include "globals.h"

s_game_speed *g_510c5c;

// @retail 0x153870
void function_153870(void)
{
	g_510c5c = (s_game_speed *)function_123d40("unknown", "unknown", sizeof(s_game_speed));
}
