// @flags /O2 /Gr
/* SIMULATION_WATCHER.CPP: the simulation watcher (g_4cf780), which tracks the
   game's players for the simulation world (lane D) */

#include "cseries.h"
#include <string.h>
#include "simulation_world.h"

// @retail 0x83a40
bool simulation_watcher_get_players(s_simulation_world_owner *watcher, long *unknown1c, dword *player_mask, dword *in_game_mask, dword *state, t_player_key *keys, bool force)
{
	bool result = false;

	if (force || watcher->unknownc30)
	{
		*unknown1c = watcher->unknown1c;
		memcpy(state, watcher->unknown24, sizeof(watcher->unknown24));
		*player_mask = watcher->players.player_mask;
		*in_game_mask = simulation_player_collection_get_in_game_mask(&watcher->players);
		memset(keys, 0, 16 * sizeof(keys[0]));
		for (long i = 0; i < 16; i++)
		{
			if (watcher->players.player_mask & (1 << i))
				memcpy(keys[i], watcher->players.players[i].key, sizeof(keys[i]));
		}
		watcher->unknownc30 = false;
		result = true;
	}
	return result;
}

// @retail 0x83bd0
bool simulation_watcher_player_valid(long player_index, const s_simulation_world_owner *watcher, const t_player_key *key)
{
	bool result = false;
	if (watcher->players.player_mask & (1 << player_index))
	{
		const s_simulation_owner_player *player = &watcher->players.players[player_index];
		if (!memcmp(key, player->key, sizeof(t_player_key)) && !player->flag0c)
			result = true;
	}
	return result;
}