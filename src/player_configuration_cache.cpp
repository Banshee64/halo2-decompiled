// @flags /O2 /Gr
/* PLAYER_CONFIGURATION_CACHE.CPP: the cache of the configurations of the
   players met recently (entries of 0x68 bytes at 0x4cf98c, chained through
   their next index). Most of the file is outside lane D's region
   (0x7fb50..0x80be7); lane D has the recent-player iterator. */

#include "cseries.h"
#include "screen_online_y_menu.h"
#include <string.h>

struct s_player_configuration_cache_entry
{
	s_recent_player player;
	short unknown58;
	short next;
	byte unknown5c[4];
	dword flags;
	byte unknown64[4];
};

s_player_configuration_cache_entry g_4cf98c[32];

/* whether a XUID is an offline (machine-local) one */
static inline bool xuid_is_offline(unsigned __int64 xuid)
{
	return (xuid >> 48) == 0xfefe;
}

/* the entry at the iterator, moving the iterator to the next one */
static inline s_player_configuration_cache_entry *player_configuration_cache_iterate(long *iterator)
{
	s_player_configuration_cache_entry *entry = NULL;
	if (*iterator != NONE)
	{
		entry = &g_4cf98c[*iterator];
		*iterator = entry->next;
	}
	return entry;
}

/* 0xa2020 in the debug build */
static inline bool player_configuration_is_excluded(s_recent_player const *player)
{
	return (player->unknown08[0] & 3) != 0;
}

/* the next recent player after the iterator's: a valid, online, known player
   with a name; iterator starts at the first entry and ends at NONE */
// @retail 0x805e0
bool player_configuration_cache_next_recent_player(s_recent_player *player, long *iterator)
{
	bool result = false;
	long index = *iterator;
	if (index != NONE)
	{
		do
		{
			s_player_configuration_cache_entry *entry = &g_4cf98c[index];
			index = entry->next;
			*iterator = index;
			if (!(entry->flags & 4) && (entry->flags & 0x10) &&
			*(unsigned __int64 *)entry->player.xuid != 0 &&
			!xuid_is_offline(*(unsigned __int64 *)entry->player.xuid) &&
			!player_configuration_is_excluded(&entry->player) &&
			*(word *)&entry->player.unknown08[4] != 0)
			{
				if (player)
					memcpy(player, &entry->player, sizeof(s_recent_player));
				return true;
			}
		} while (index != NONE);
	}
	return result;
}
