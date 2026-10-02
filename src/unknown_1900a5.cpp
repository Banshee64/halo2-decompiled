// @flags /O1 /Gr
/* UNKNOWN_1900A5.CPP: player slot query */

#include "cseries.h"
#include "globals.h"

// @retail 0x1900a5
bool function_1900a5(long player)
{
	s_player_slot_flags *slot = &g_54e8e0[player];
	return (slot->flags & 3) != 0;
}
