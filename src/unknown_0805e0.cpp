// @flags /O2 /Gr
/* UNKNOWN_0805E0.CPP: the cache of the configurations of the
   players met recently (entries of 0x68 bytes at 0x4cf98c, chained through
   their next index). Most of the file is outside lane D's region
   (0x7fb50..0x80be7); lane D has the recent-player iterator. */

#include "unknown_11c920.h"
#include "unknown_2312b4.h"
#include <string.h>

struct s_player_configuration_cache_entry
{
	s_recent_player player;
	short unknown58;
	short next;
	short previous_other;
	short next_other;
	dword flags;
	byte unknown64[4];
};

s_player_configuration_cache_entry g_4cf98c[350];

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

long g_4cf97c;
long g_4cf980;
extern long g_4cf984;
long g_4cf988;
long g_4cf978;

// @retail 0x7fd10
void function_7fd10(long index)
{
	s_player_configuration_cache_entry *entry = &g_4cf98c[index];
	if (entry->previous_other != NONE)
		g_4cf98c[entry->previous_other].next_other = entry->next_other;
	if (entry->next_other != NONE)
		g_4cf98c[entry->next_other].previous_other = entry->previous_other;
	if (g_4cf97c == index)
		g_4cf97c = entry->next_other;
	if (g_4cf980 == index)
		g_4cf980 = entry->previous_other;
}

// @retail 0x7fd80
void function_7fd80(long index)
{
	s_player_configuration_cache_entry *entry = &g_4cf98c[index];
	if (entry->unknown58 != NONE)
		g_4cf98c[entry->unknown58].next = entry->next;
	if (entry->next != NONE)
		g_4cf98c[entry->next].unknown58 = entry->unknown58;
	if (g_4cf984 == index)
		g_4cf984 = entry->next;
	if (g_4cf988 == index)
		g_4cf988 = entry->unknown58;
}

// @retail 0x7fc80
long __stdcall function_7fc80(const void *identity, long *position)
{
	long middle = 0;
	long lower = 0;
	long upper = g_4cf978 - 1;
	long result = NONE;
	bool last = false;
	while (result == NONE && !last && lower <= upper)
	{
		last = lower == upper;
		middle = (lower + upper) >> 1;
		long comparison = memcmp(identity, &g_4cf98c[middle].player, 12);
		if (comparison == 0)
			result = middle;
		else if (comparison < 0)
			upper = middle - 1;
		else
			lower = ++middle;
	}
	if (position)
		*position = middle;
	return result;
}

struct s_cached_player_identity
{
	dword values[3];
};
struct s_cached_player_source
{
	word name[32];
	dword field40;
	union
	{
		dword field44_47;
		struct
		{
			byte field44;
			byte field45;
			byte field46;
			byte field47;
		};
	};
	unsigned __int64 field48;
	byte unknown50[0x70 - 0x50];
	long field70;
	long field74;
	long field78;
	byte unknown7c[3];
	byte field7f;
	byte unknown80[0x10];
};
struct s_cached_player_view
{
	s_cached_player_identity identity;
	word name[32];
	dword field4c;
	byte field50;
	byte field51;
	byte field52;
	byte field53;
	byte unknown54[2];
	byte field56;
	byte unknown57;
};

// @retail 0x7f8b0
void function_7f8b0(const s_cached_player_identity *identity, s_cached_player_view *result,
	const s_cached_player_source *source, const s_cached_player_view *previous)
{
	memset(result, 0, sizeof(*result));
	result->identity = *identity;
	memcpy(result->name, source->name, sizeof(result->name));
	result->field4c = source->field40;
	result->field50 = source->field44;
	result->field51 = source->field45;
	result->field52 = source->field46;
	result->field53 = source->field47;
	byte index = source->field7f;
	if (index == 0xff && previous)
		result->field56 = previous->field56;
	else
		result->field56 = index;
}

// @retail 0x7fdf0
void function_7fdf0(long index)
{
	if (g_4cf97c != index)
	{
		function_7fd10(index);
		if (g_4cf97c != NONE)
			g_4cf98c[g_4cf97c].previous_other = (short)index;
		s_player_configuration_cache_entry *entry = &g_4cf98c[index];
		entry->previous_other = NONE;
		entry->next_other = (short)g_4cf97c;
		g_4cf97c = index;
	}
}

// @retail 0x7fe40
void function_7fe40(long index)
{
	if (g_4cf984 != index)
	{
		function_7fd80(index);
		if (g_4cf984 != NONE)
			g_4cf98c[g_4cf984].unknown58 = (short)index;
		s_player_configuration_cache_entry *entry = &g_4cf98c[index];
		entry->unknown58 = NONE;
		entry->next = (short)g_4cf984;
		g_4cf984 = index;
	}
}

long g_4cf970;
long g_4cf974;

// @retail 0x7f930
void function_7f930(void)
{
	g_4cf970 = 8;
	g_4cf974 = 0;
	g_4cf978 = 0;
	g_4cf980 = NONE;
	g_4cf97c = NONE;
	g_4cf984 = NONE;
	g_4cf988 = NONE;
	for (long i = 0; i < 350; i++)
	{
		s_cached_player_identity identity;
		memset(&identity, 0, sizeof(identity));
		s_cached_player_source source;
		memset(&source, 0, sizeof(source));
		source.field40 = 0;
		source.field44_47 = 0;
		source.field48 = 0;
		source.field70 = 0;
		source.field74 = 0;
		source.field78 = 0;
		source.field7f = 0xff;
		s_player_configuration_cache_entry *entry = &g_4cf98c[i];
		function_7f8b0(&identity, (s_cached_player_view *)&entry->player, &source, NULL);
		entry->next = entry->next_other = entry->unknown58 = entry->previous_other = NONE;
		entry->flags = 0;
		*(long *)entry->unknown64 = 0;
	}
}
