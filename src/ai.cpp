// @flags /O2 /arch:SSE /Gr
/* AI.CPP: the ai globals, the ai's view of the players and units, and small
   ai helpers (0x1c7790..0x1caxxx; the atlas puts ai_get_responsible_unit,
   0x1c9580, in ai.obj) */

#include "cseries.h"
#include "globals.h"
#include <string.h>
#include <math.h>

/* the ai globals (0x374 bytes in the game state) */
struct s_ai_globals
{
	bool enabled;
	bool active;
	bool unknown02;
	byte unknown03[0x14 - 0x3];
	long unknown14;
	byte unknown18[0x20 - 0x18];
	bool unknown20;
	byte unknown21;
	short unknown22;
	byte unknown24[0x340 - 0x24];
	bool unknown340;
	byte unknown341[0x374 - 0x341];
};

/* what the ai tracks of each local player (2 entries of 0x1c bytes in the
   game state) */
struct s_ai_player
{
	long player_index;
	long unit_index;
	short unknown08;
	short unknown0a;
	byte unknown0c[0x1c - 0xc];
};

#define MAXIMUM_AI_PLAYERS 2

s_ai_globals *g_4f55d0;
s_ai_player *g_4f55cc;

// @retail 0x1c7fe0
void ai_players_reset(void)
{
	long i;

	for (i = 0; i < MAXIMUM_AI_PLAYERS; i++)
	{
		g_4f55cc[i].player_index = NONE;
		g_4f55cc[i].unit_index = NONE;
		g_4f55cc[i].unknown0a = 0;
	}
}

// @retail 0x1c7fa0
void ai_globals_clear(void)
{
	memset(g_4f55d0, 0, sizeof(s_ai_globals));
	ai_players_reset();
}

// @retail 0x1c80f0
void ai_player_add(long player_index)
{
	if (g_4e6948->state == 1)
	{
		bool added = false;
		long i;

		for (i = 0; i < MAXIMUM_AI_PLAYERS; i++)
		{
			s_ai_player *player = &g_4f55cc[i];

			if (player->player_index == NONE && !added)
			{
				memset(player, 0, sizeof(s_ai_player));
				player->player_index = player_index;
				player->unit_index = NONE;
				player->unknown08 = NONE;
				player->unknown0a = 0;
				added = true;
			}
		}
	}
}

// @retail 0x1c8150
short ai_player_index_get(long player_index)
{
	short index = NONE;
	long i;

	for (i = 0; i < MAXIMUM_AI_PLAYERS; i++)
	{
		if (g_4f55cc[i].player_index == player_index)
		{
			index = (short)i;
			break;
		}
	}
	return index;
}

// @retail 0x1c8180
s_ai_player *ai_player_get(long player_index)
{
	s_ai_player *player = NULL;
	long index = ai_player_index_get(player_index);

	if (index != NONE)
	{
		player = &g_4f55cc[index];
	}
	return player;
}

// @retail 0x1c8390
void ai_players_unit_deleted(long unit_index)
{
	long i = 0;

	do
	{
		s_ai_player *player = &g_4f55cc[i];

		if (player->unit_index == unit_index)
		{
			player->unit_index = NONE;
			player->unknown08 = NONE;
			player->unknown0a = 0;
		}
		i++;
	}
	while (i < MAXIMUM_AI_PLAYERS);
}

/* the objects (0bad50.cpp) */
struct s_object;
s_object *function_badc0(long object_index, dword type_mask);
long function_baf40(long object_index);

struct s_ai_object_header
{
	short identifier;
	byte flags;
	byte type;
	byte unknown04[4];
	s_object *object;
};

struct s_ai_unit
{
	byte unknown000[0x248];
	long unknown248;
	long unknown24c;
};

// @retail 0x1c9580
long ai_get_responsible_unit(long object_index, bool a)
{
	long result = NONE;

	if (object_index != NONE)
	{
		s_ai_unit *unit = (s_ai_unit *)function_badc0(object_index, 3);

		if (unit)
		{
			if (a && unit->unknown24c != NONE)
			{
				result = unit->unknown24c;
			}
			else if (unit->unknown248 != NONE)
			{
				result = unit->unknown248;
			}
			else
			{
				result = object_index;
			}
		}
	}
	return result;
}

/* the ai globals tag, a block at +0xc8 of the tag header globals */
struct s_ai_globals_definition
{
	real unknown00;
	byte unknown04[4];
	real unknown08;
	byte unknown0c[4];
	real unknown10;
	byte unknown14[4];
	real unknown18;
	byte unknown1c[4];
	real unknown20;
	real unknown24;
	real unknown28;
	real unknown2c;
};

struct s_ai_tag_header_globals
{
	byte unknown00[0xc8];
	long ai_globals_count;
	s_ai_globals_definition *ai_globals;
};

// @retail 0x1c9e50
real function_1c9e50(short index)
{
	s_ai_tag_header_globals *globals = (s_ai_tag_header_globals *)g_4e034c;
	real result = 0.0f;

	if (globals && globals->ai_globals_count > 0)
	{
		s_ai_globals_definition *definition = globals->ai_globals;

		switch (index)
		{
		case 0:
			result = 0.0f;
			break;
		case 1:
			result = definition->unknown00;
			break;
		case 2:
			result = definition->unknown08;
			break;
		case 3:
			result = definition->unknown10;
			break;
		case 4:
			result = definition->unknown18;
			break;
		case 5:
			result = definition->unknown20;
			break;
		case 6:
			result = definition->unknown24;
			break;
		case 7:
			result = definition->unknown28;
			break;
		case 8:
			result = definition->unknown2c;
			break;
		}
	}
	return result;
}

// @retail 0x1c9ee0
real function_1c9ee0(real fraction)
{
	if (fraction < 0.0f)
	{
		fraction = 0.0f;
	}
	else if (fraction > 1.0f)
	{
		fraction = 1.0f;
	}
	return 1.0f - (real)pow(1.0f - fraction, g_510c54->rate);
}

// @retail 0x1caa10
long function_1caa10(long object_index)
{
	long parent_index = function_baf40(object_index);

	if (((s_ai_object_header *)g_4e0300->data)[parent_index & 0xffff].type != 1)
	{
		parent_index = object_index;
	}
	return parent_index;
}
