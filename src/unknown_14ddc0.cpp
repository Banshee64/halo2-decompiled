// @flags /O2 /Gr
/* UNKNOWN_14DDC0.CPP: the local player table (g_4e8c20): whether a local
   player is in use, and its player index */

#include "unknown_11c920.h"
#include "globals.h"

// @retail 0x14ddc0
bool function_14ddc0(long local_player_index)
{
	bool result = false;
	if (local_player_index != NONE)
		result = g_4e8c20->entries[local_player_index] != NONE;
	return result;
}

// @retail 0x14de70
long function_14de70(long local_player_index)
{
	long result = NONE;
	if (local_player_index != NONE)
		result = g_4e8c20->entries[local_player_index];
	return result;
}
