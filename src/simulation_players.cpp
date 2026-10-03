// @flags /O2 /Gr
/* SIMULATION_PLAYERS.CPP: the player collections the simulation watcher keeps
   (lane D) */

#include "cseries.h"
#include "simulation_world.h"

// @retail 0x84be0
dword simulation_player_collection_get_in_game_mask(const s_player_collection *collection)
{
	dword mask = 0;
	for (long i = 0; i < 16; i++)
	{
		if (collection->player_mask & (1 << i))
		{
			if (!collection->players[i].flag0c)
				mask |= 1 << i;
			else
				mask &= ~(1 << i);
		}
	}
	return mask;
}
