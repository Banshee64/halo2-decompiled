#include <string.h>
#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_1523c0.h"
#include "game_engine_events.h"

// @flags /O2 /arch:SSE /Gr

/* UNKNOWN_2BD0B0.CPP: the game engine whose vtable is at 0x45c878 (the third
   engine object at 0x47fc88): its slots 0..28, which unknown_1523c0.h numbers
   v22..v50 (its slots from 30 on are c_game_engine_a's in
   unknown_2bd960.cpp), and the helpers they use. The engine keeps a list of
   the distinct marker groups (marker types 11..18) to pick from at random,
   and its state (g_51ecc8, defined by unknown_2bd960.cpp) in the multiplayer
   globals at +0xfc. */

struct s_state_2bd;
struct s_polygon_2be;
extern s_state_2bd *g_51ecc8;

/* the players (0x21c bytes) */
struct s_player_2bd0
{
	short identifier;
	byte unknown02[0x2c - 2];
	long object_index;
	byte unknown30[0xc0 - 0x30];
	char team;
	byte unknownc1[0x1b8 - 0xc1];
	short time_inside;
	byte unknown1ba[0x21c - 0x1ba];
};

struct s_game_options_2bd0
{
	byte unknown00[0x1128];
	bool flag1128;
};

short g_5092e8;
short g_5092f0[8];

void function_2bd960();
bool function_2be880(long player_index, s_polygon_2be *polygon);

class c_game_engine_45c878 : public c_game_engine
{
public:
	virtual bool v23();
	virtual void v28(long);
	virtual void v37(long);
};

static inline s_player_2bd0 *player_get_2bd0(long player_index)
{
	return (s_player_2bd0 *)(g_4e8c24->data + (player_index & 0xffff) * sizeof(s_player_2bd0));
}

// @retail 0x2bd0b0
long function_2bd0b0(long excluded)
{
	dword *seed = &g_4e7408->unknown0;
	short count = g_5092e8;
	long result = NONE;

	*seed = *seed * 0x19660d + 0x3c6ef35f;
	short first = (short)(((*seed >> 16) * count) >> 16);

	for (short i = 0; i < count; i++)
	{
		short index = (short)((first + i) % count);

		if (excluded != g_5092f0[index])
		{
			result = g_5092f0[index];
			break;
		}
	}
	return result;
}

// @retail 0x2bd140
bool c_game_engine_45c878::v23()
{
	s_palette_source_globals *globals = g_4e0350;
	byte *state = (byte *)g_4e9ae8 + 0xfc;

	g_51ecc8 = (s_state_2bd *)state;
	memset(state, 0, 0x1dc);
	g_5092e8 = 0;
	for (short i = 0; i < globals->marker_count; i++)
	{
		short type = globals->marker_entries[i].key_a;

		if (type >= 11 && type <= 18)
		{
			bool found = false;

			for (short j = 0; j < g_5092e8; j++)
			{
				if (g_5092f0[j] == type - 11)
				{
					found = true;
					break;
				}
			}
			if (!found)
				g_5092f0[g_5092e8++] = type - 11;
		}
	}
	function_2bd960();
	return true;
}

// @retail 0x2bd260
void c_game_engine_45c878::v28(long a)
{
	s_event event;

	game_engine_event_initialize_inline(&event, 6, 0);
	event.a = a;
	game_engine_event_send_inline(&event);
}

// @retail 0x2bd2c0
void c_game_engine_45c878::v37(long player_index)
{
	s_player_2bd0 *player = player_get_2bd0(player_index);

	if (player->object_index != NONE &&
		!((s_game_options_2bd0 *)g_4e6948)->flag1128 &&
		g_4e6948->mode != 4 &&
		function_2be880(player_index, (s_polygon_2be *)g_51ecc8))
	{
		player->time_inside++;
	}
	else
	{
		player->time_inside = 0;
	}
}

// @retail 0x2bd330
long function_2bd330(word player_mask)
{
	s_record_pool *players = g_4e8c24;
	long team_mask = 0;

	for (long i = 0; i < 16; i++)
	{
		if (player_mask & (1 << i))
		{
			if (i != NONE && i >= 0 && i < players->high_water_index)
			{
				s_player_2bd0 *player = (s_player_2bd0 *)(players->data + players->size * i);

				if (player->identifier != 0 && player->team != NONE)
					team_mask |= 1 << player->team;
			}
		}
	}
	return team_mask;
}
