// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1600F0.CPP: the end of the game engine code (0x1600f0..0x163080):
   queries on the multiplayer globals (g_4e9ae8) and the current engine
   object (g_55e4d0), and the time text the engines draw */

#include "cseries.h"
#include "globals.h"
#include "engine_peer.h"

#define PIN(x, lo, hi) ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))

/* the multiplayer globals' fields read here that globals.h does not name */
struct s_mp_globals_view
{
	byte unknown000[0xf4];
	byte flagsf4;
	byte unknown0f5[0x304 - 0xf5];
	byte unknown304[4];
};

/* the players (0x21c bytes) */
struct s_game_engine_player
{
	byte unknown000[0x28];
	short local_user_index;
	byte unknown02a[0x21c - 0x2a];
};

/* the objects, as read here */
struct s_game_engine_object
{
	long definition_index;
};

struct s_game_engine_object_header
{
	byte unknown00[8];
	s_game_engine_object *object;
};

struct s_game_engine_object_definition
{
	byte unknown000[0x290];
	short unknown290;
};

/* per local user: a count of ticks, a quarter second each */
byte g_4e9af0[4];

real function_242140(long object_index);
void function_1a0180(long a, long b);

static inline c_engine_peer *game_engine_get(void)
{
	return g_55e4d0[g_4e9ae8->engine_index];
}

// @retail 0x161b60
bool function_161b60(long player_index)
{
	c_engine_peer *engine = game_engine_get();
	bool result = true;

	if (engine && player_index != NONE)
	{
		byte *players = g_4e8c24->data;
		s_game_engine_player *player = (s_game_engine_player *)(players + (player_index & 0xffff) * sizeof(s_game_engine_player));
		short local_user_index = player->local_user_index;

		if (local_user_index != NONE)
		{
			real fraction = (real)g_4e9af0[local_user_index] * g_510c54->rate * 4.0f;

			if (PIN(fraction, 0.0f, 1.0f) >= 1.0f)
			{
				result = false;
			}
		}
	}
	return result;
}

struct s_161c90
{
	byte unknown00[0x44];
	long type;
};

// @retail 0x161c90
long function_161c90(s_161c90 const *p)
{
	long result = 1;

	switch (p->type)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 7:
	case 8:
	case 9:
		result = 2;
		break;
	case 5:
	case 6:
		break;
	}
	return result;
}

/* the game options' flags at +0x184, read a byte at a time */
struct s_game_options_flags_view
{
	byte unknown000[0x184];
	byte teams : 1;
};

static inline bool game_engine_teams(void)
{
	return TEST_FIELD_BIT(((s_game_options_flags_view *)g_4e6948)->teams);
}

// @retail 0x161e10
bool function_161e10(long index)
{
	bool result = false;

	if (game_engine_get())
	{
		bool teams = game_engine_teams();
		volatile bool unused = teams;

		if (teams && index >= 0 && index < 8)
		{
			result = (g_4e9ae8->wc & (1 << index)) != 0;
		}
	}
	return result;
}

// @retail 0x161e60
bool function_161e60(long index)
{
	bool result = false;

	if (game_engine_get())
	{
		bool teams = game_engine_teams();
		volatile bool unused = teams;

		if (teams && index >= 0 && index < 8)
		{
			result = (g_4e9ae8->we & (1 << index)) != 0;
		}
	}
	return result;
}

// @retail 0x161eb0
long function_161eb0(long index)
{
	long mask = g_4e9ae8->wc;
	long result = NONE;
	long i;

	for (i = 0; i < 7; i++)
	{
		index++;
		if (index == 8)
		{
			index = 0;
		}
		if (mask & (1 << index))
		{
			result = index;
			break;
		}
	}
	return result;
}

// @retail 0x161ef0
void function_161ef0(long a)
{
	s_tag_header_globals *globals = g_4e034c;

	if (globals && globals->index != NONE)
	{
		long string_list_index = *(long *)(*(byte **)(g_4e3b44[globals->index & 0xffff].bytes + 4) + 0x1c);

		if (string_list_index != NONE)
		{
			function_1a0180(string_list_index, a);
		}
	}
}

// @retail 0x162030
void *function_162030(void)
{
	void *result = NULL;

	if (game_engine_get())
	{
		result = ((s_mp_globals_view *)g_4e9ae8)->unknown304;
	}
	return result;
}

// @retail 0x162420
void function_162420(void)
{
	long i;

	g_4e9ae8->value24 = NONE;
	g_4e9ae8->value28 = NONE;
	for (i = 0; i < 16; i++)
	{
		g_4e9ae8->slots[i] = NONE;
	}
}

// @retail 0x162b10
bool function_162b10(long object_index)
{
	bool result = false;

	if (g_4e6948->mode_180 == 9 && object_index != NONE)
	{
		s_game_engine_object *object = ((s_game_engine_object_header *)g_4e0300->data)[object_index & 0xffff].object;
		s_game_engine_object_definition *definition = (s_game_engine_object_definition *)g_4e3b44[object->definition_index & 0xffff].bytes;

		if (definition->unknown290 == 1)
		{
			result = true;
		}
	}
	return result;
}

// @retail 0x162b70
real function_162b70(long object_index)
{
	if (g_4e6948->mode_180 == 9)
	{
		return function_242140(object_index);
	}
	return 0.0f;
}

// @retail 0x163040
bool function_163040(long index)
{
	bool result = false;

	if (game_engine_get())
	{
		result = (((s_mp_globals_view *)g_4e9ae8)->flagsf4 & (1 << index)) != 0;
	}
	return result;
}

// @retail 0x1630b0
void function_1630b0(long *values, long value)
{
	long i;

	for (i = 0; i < 16; i++)
	{
		values[i] = value;
	}
}
