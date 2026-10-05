// @flags /O2 /Gr
#include "unknown_11c920.h"
#include "globals.h"
#include <string.h>

struct s_player_bytes6
{
	byte data[6];
};

struct s_player_bytes12
{
	byte data[12];
};

struct s_player_settings_source
{
	short salt;
	byte flags;
	byte unknown03;
	s_player_bytes12 identifier;
	byte unknown10[4];
	s_player_bytes6 address;
	byte unknown1a[2];
	short value1c;
	byte unknown1e[2];
	long value20;
	byte unknown24[0x44 - 0x24];
	byte settings[0x90];
};

struct s_player_settings_entry
{
	bool valid;
	bool flag;
	short value02;
	long value04;
	s_player_bytes6 address;
	s_player_bytes12 identifier;
	byte unknown1a[2];
	byte settings[0x90];
	byte unknownac[0xe4 - 0xac];
};

struct s_player_settings_globals
{
	byte unknown00[0x2c];
	long value2c;
	byte settings[0x60];
	byte value90;
	s_player_bytes6 address;
};

struct s_player_settings_snapshot
{
	byte unknown00[0x268];
	long value268;
	byte settings[0x60];
	byte value2cc;
	s_player_bytes6 address;
	byte unknown2d3;
	s_player_settings_entry players[16];
};

// @retail 0x152450
void function_152450(s_player_settings_snapshot *snapshot)
{
	unsigned long count = 0;
	s_player_settings_globals *source = (s_player_settings_globals *)g_4e8c20;
	snapshot->value268 = source->value2c;
	memcpy(snapshot->settings, source->settings, sizeof(snapshot->settings));
	snapshot->value2cc = source->value90;
	snapshot->address = source->address;
	memset(snapshot->players, 0, sizeof(snapshot->players));

	long index = NONE;
	for (;;)
	{
		long next = data_find_index(g_4e8c24, index + 1);
		if (next == NONE)
			break;
		s_record_pool *players = *(s_record_pool *volatile *)&g_4e8c24;
		s_player_settings_source *player = (s_player_settings_source *)(players->data + players->size * next);
		index = next;
		if (!player || count >= 16)
			break;

		s_player_settings_entry *entry = &snapshot->players[count++];
		entry->valid = true;
		entry->flag = (bool)((player->flags >> 1) & 1);
		entry->identifier = player->identifier;
		entry->address = player->address;
		entry->value02 = player->value1c;
		entry->value04 = player->value20;
		memcpy(entry->settings, player->settings, sizeof(entry->settings));
	}
}
