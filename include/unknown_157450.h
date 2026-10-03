/* UNKNOWN_157450.H: lane Q's view of the game engine globals (g_4e9ae8,
   0xc9c bytes) and the small inline helpers the game engine files share.
   globals.h's s_mp_globals is the same data; this view adds the fields the
   game engine code reads. */

#ifndef UNKNOWN_157450_H
#define UNKNOWN_157450_H

#include "cseries.h"
#include "globals.h"
#include "engine_peer.h"

/* the statistics of the engine (+0x304, 0x254 bytes): a valid flag, a
   record per player and one per team */
struct s_statborg_player
{
	short score;
	byte unknown02[0x1c - 2];
};

struct s_statborg_team
{
	short score;
	byte unknown02[0x12 - 2];
};

struct s_statborg
{
	bool valid;
	byte unknown01[3];
	s_statborg_player players[16];
	s_statborg_team teams[8];
};

struct s_game_engine_globals
{
	dword flags;
	word present_teams;
	word active_teams;
	word assigned_teams;
	word team_flags;
	word unknown0c;
	word team_mask;
	short team_designators[9];
	byte unknown22[2];
	long index24;
	long index28;
	long slots[16];
	byte unknown6c[0xe0 - 0x6c];
	short value_e0;
	byte unknowne2[2];
	real timers[4];
	byte timer_flags;
	byte unknownf5[0x304 - 0xf5];
	s_statborg statborg;
	byte unknown558[0x7dc - 0x558];
	byte unknown7dc[0xc14 - 0x7dc];
	long engine_index;
	byte unknownc18[0x84];
};

static inline s_game_engine_globals *game_engine_globals()
{
	return (s_game_engine_globals *)g_4e9ae8;
}

static inline c_engine_peer *game_engine_get()
{
	return g_55e4d0[game_engine_globals()->engine_index];
}

static inline bool game_engine_has_teams()
{
	bool result = false;

	if (game_engine_get())
	{
		result = TEST_FIELD_BIT(g_4e6948->flags184.bit0);
	}
	return result;
}

static inline s_statborg *game_engine_statborg_inline()
{
	s_statborg *result = 0;

	if (game_engine_get())
	{
		result = &game_engine_globals()->statborg;
	}
	return result;
}

s_statborg *game_engine_get_statborg();
bool game_engine_team_is_active(long team);

#endif
