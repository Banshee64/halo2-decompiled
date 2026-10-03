// @flags /O2 /Gr
/* SIMULATION_PLAYERS.CPP: the player collections the simulation watcher keeps
   (lane D) */

#include "cseries.h"
#include <string.h>
#include "data_array.h"
#include "globals.h"
#include "simulation_world.h"

/* a game player (g_4e8c24, 0x21c bytes), as the collection reads it */
struct s_simulation_player_datum
{
	short salt;
	word flag0 : 1;
	word left_game : 1;
	word unknown02 : 14;
	dword key[3];
	long time;
	s_machine_address machine;
	short machine_index;
	short controller_index;
	byte unknown1e[2];
	long unknown20;
	byte unknown24[0xd4 - 0x24];
	dword configuration[0x24];
	byte unknown164[0x21c - 0x164];
};

// @retail 0x84a90
void simulation_player_collection_clear(s_player_collection *collection)
{
	memset(collection, 0, sizeof(*collection));
	for (long i = 0; i < 16; i++)
	{
		s_simulation_owner_player *player = &collection->players[i];
		player->flag0c = false;
		player->time = NONE;
		player->unknown20 = NONE;
		player->controller_index = NONE;
	}
}

/* data_datum_iterator_next (data_iterator.cpp), which retail inlines here */
static inline bool player_iterator_next(s_data_datum_iterator *iterator)
{
	s_data_array *data = iterator->data;
	long index = data_find_index(data, iterator->index + 1);
	byte *datum;

	if (index != NONE)
	{
		datum = data->data + data->size * index;
		iterator->index = index;
		iterator->datum_index = (*(short *)datum << 16) | index;
	}
	else
	{
		iterator->index = data->maximum_count;
		iterator->datum_index = NONE;
		datum = 0;
	}
	iterator->datum = datum;
	return datum != 0;
}

// @retail 0x84ad0
void simulation_player_collection_build(s_player_collection *collection)
{
	s_data_datum_iterator iterator;
	iterator.data = g_4e8c24;
	iterator.index = NONE;

	while (player_iterator_next(&iterator))
	{
		s_simulation_player_datum *datum = (s_simulation_player_datum *)iterator.datum;
		long player_index = iterator.datum_index & 0xffff;
		s_simulation_owner_player *player = &collection->players[player_index];

		collection->player_mask |= 1 << player_index;
		memcpy(player->key, datum->key, sizeof(player->key));
		if (datum->left_game)
		{
			player->flag0c = true;
			player->time = datum->time;
			memset(&player->machine, 0, sizeof(player->machine));
			player->unknown20 = NONE;
			player->controller_index = NONE;
		}
		else
		{
			player->flag0c = false;
			player->time = NONE;
			player->machine = datum->machine;
			player->unknown20 = datum->unknown20;
			player->controller_index = datum->controller_index;
			memcpy(player->configuration, datum->configuration, sizeof(player->configuration));
		}
	}
}

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
