#include "unknown_11c920.h"
#include "globals.h"

// @flags /O2 /Gr

/* Two state queries of the multiplayer globals (g_4e9ae8). Decompiled by
   lane O because the engine at 0x459d18 (unknown_2420a0.cpp) inlines them. */

struct s_engine_state_globals
{
	byte unknown00[0x6c];
	short w6c;
	byte unknown6e[0xc04 - 0x6e];
	long lc04;
	byte unknownc08[0xc14 - 0xc08];
	long engine_index;
};

/* true while the engine runs the game (always on a client) */
// @retail 0x15b2f0
bool function_15b2f0()
{
	s_engine_state_globals *g = (s_engine_state_globals *)g_4e9ae8;
	bool result = false;

	if (g_55e4d0[g->engine_index] && g->w6c == 1 && (g_4e6948->mode == 4 || g->lc04 == 1))
		result = true;

	return result;
}

/* true when the game has teams */
// @retail 0x15eaf0
bool function_15eaf0()
{
	bool result = false;

	if (g_55e4d0[g_4e9ae8->engine_index])
		result = g_4e6948->flags184.bit0;

	return result;
}
