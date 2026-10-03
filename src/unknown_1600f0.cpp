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
void unicode_string_list_get_string(long tag_index, long string_id, word *buffer);
/* lane O's unknown_15e410.cpp */
bool function_161e10(long team);
long function_161eb0(long team);

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

// @retail 0x161ef0
void function_161ef0(long string_id, word *buffer)
{
	s_tag_header_globals *globals = g_4e034c;

	if (globals && globals->index != NONE)
	{
		long string_list_index = *(long *)(*(byte **)(g_4e3b44[globals->index & 0xffff].bytes + 4) + 0x1c);

		if (string_list_index != NONE)
		{
			unicode_string_list_get_string(string_list_index, string_id, buffer);
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

word *function_1630e0(word *buffer, const word *format, ...);

// @retail 0x161be0
void game_engine_format_time(long seconds, word *text)
{
	word minutes_text[0x100];
	word seconds_text[0x100];
	long minutes = seconds / 60;
	long remainder = seconds - minutes * 60;

	minutes_text[0] = 0;
	seconds_text[0] = 0;
	if (minutes == 0)
	{
		function_1630e0(minutes_text, (const word *)L" ");
	}
	else
	{
		function_1630e0(minutes_text, (const word *)L"%d", minutes);
	}
	if (remainder <= 9)
	{
		function_1630e0(seconds_text, (const word *)L"0%d", remainder);
	}
	else
	{
		function_1630e0(seconds_text, (const word *)L"%d", remainder);
	}
	function_1630e0(text, (const word *)L"%s:%s", minutes_text, seconds_text);
}
/* the players, as the respawn code reads them */
struct s_game_engine_player_state
{
	byte unknown000[2];
	word flags;
	byte unknown004[0x28 - 0x4];
	short local_user_index;
	byte unknown02a[2];
	long unit_index;
	byte unknown030[0x190 - 0x30];
	long respawn_time;
	byte unknown194[0x1b0 - 0x194];
	long spectated_player_index;
	byte unknown1b4[0xc4 + 0x100 - 0x1b4];
};

struct s_game_engine_player_view
{
	byte unknown000[2];
	word flags;
	byte unknown004[0x28 - 0x4];
	short local_user_index;
	byte unknown02a[0xc4 - 0x2a];
	bool active;
};

void function_b58c0(long index, dword mask);

static inline s_game_engine_player_state *game_engine_player_get(long player_index)
{
	return (s_game_engine_player_state *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c);
}

/* the multiplayer globals' per player entries (0x18 bytes at +0x558) */
struct s_game_engine_player_info
{
	bool active;
	byte unknown01[3];
	real_point3d position;
	short timer_active;
	short timer;
	char state;
	byte state_ticks;
	char next_state;
	byte unknown17;
};

// @retail 0x162a30
void function_162a30(long player_index)
{
	s_game_engine_player_info *info = (s_game_engine_player_info *)&g_4e9ae8->players[player_index & 0xffff];

	if (g_4e6948->mode != 4)
	{
		long previous = info->state;

		if (info->state_ticks != 0 && --info->state_ticks == 0)
		{
			if (info->next_state != 0)
			{
				real ticks;
				long ticks_long;

				info->state = info->next_state;
				ticks = g_510c54->ticks_per_second * 0.5f;
				__asm
				{
					fld ticks
					fistp ticks_long
				}
				info->state_ticks = (byte)ticks_long;
				info->next_state = 0;
			}
			else
			{
				info->state = 0;
				info->state_ticks = 0;
			}
		}
		if (previous != info->state && game_engine_get())
		{
			long slot = g_4e9ae8->slots[(short)player_index];

			if (slot != NONE)
			{
				function_b58c0(slot, 4);
			}
		}
	}
	if (info->timer_active != 0)
	{
		long time = info->timer - 1;

		time = time > 0 ? time : 0;
		info->timer = (short)time;
		if ((short)time == 0)
		{
			info->active = false;
			info->timer_active = 0;
			info->timer = 0;
		}
	}
}

// @retail 0x162bf0
void function_162bf0(long player_index, long spectated_player_index)
{
	s_game_engine_player_state *player = game_engine_player_get(player_index);

	if (player->spectated_player_index == NONE)
	{
		player->spectated_player_index = spectated_player_index;
		if (game_engine_get())
		{
			long slot = g_4e9ae8->slots[(short)player_index];

			if (slot != NONE)
			{
				function_b58c0(slot, 0x40);
			}
		}
	}
}

byte *datum_get(s_data_array *data, long datum_index);

// @retail 0x162c50
bool function_162c50(long player_index, long *spectated_player_index)
{
	s_game_engine_player_state *player = game_engine_player_get(player_index);

	if (player->unit_index == NONE)
	{
		long other_index = player->spectated_player_index;

		if (other_index != NONE)
		{
			s_game_engine_player_view *other = (s_game_engine_player_view *)datum_get(g_4e8c24, other_index);

			if (other && other->active && other->local_user_index == NONE && !(other->flags & 2))
			{
				long ticks = g_510c54->ticks_per_second * 10;

				if (!(player->flags & 0x1000))
				{
					player->flags |= 0x1000;
					player->respawn_time = g_510c54->game_time;
				}
				if (g_510c54->game_time - player->respawn_time < ticks)
				{
					*spectated_player_index = other_index;
					return true;
				}
				return false;
			}
		}
	}
	player->flags &= ~0x1000;
	player->respawn_time = 0;
	return false;
}

/* an iterator over the players (0x19f240 skips the inactive ones) */
struct s_game_engine_player_iterator
{
	byte *datum;
	s_data_array *data;
	long datum_index;
	long index;
};

bool function_19f240(long *iterator);

short g_4e9b38[8][16];

// @retail 0x162fd0
long function_162fd0(long index)
{
	s_game_engine_player_iterator iterator;
	long result = NONE;
	short best = 0x7fff;

	iterator.data = g_4e8c24;
	iterator.index = NONE;
	iterator.datum_index = NONE;
	while (function_19f240((long *)&iterator))
	{
		long value = g_4e9b38[index][iterator.datum_index & 0xffff];

		if (value >= 4 && value < best)
		{
			best = (short)value;
			result = iterator.datum_index;
		}
	}
	return result;
}
/* game options fields read here */
struct s_game_options_time_view
{
	byte unknown000[0x190];
	long time_limit;
};

struct s_mp_globals_time_view
{
	byte unknown000[0x70];
	long start_time;
};

bool g_55e754;

#include "game_engine_events.h"

// @retail 0x162470
long function_162470(bool flag)
{
	long time_limit = ((s_game_options_time_view *)g_4e6948)->time_limit;
	s_mp_globals *globals = g_4e9ae8;
	long elapsed = g_510c54->game_time - ((s_mp_globals_time_view *)globals)->start_time;
	long time_left;
	long result;

	elapsed = elapsed < 0 ? 0 : elapsed;
	time_left = g_510c54->ticks_per_second * time_limit - elapsed;
	time_left = time_left < 0 ? 0 : time_left;
	result = g_55e4d0[globals->engine_index]->p25(time_left, flag, true);
	if (flag)
	{
		if (result != time_left)
		{
			if (!g_55e754)
			{
				s_event event;

				event.type = 0;
				event.subtype = 0x25;
				event.a = NONE;
				event.cause_player_index = NONE;
				event.cause_team = NONE;
				event.effect_player_index = NONE;
				event.effect_team = NONE;
				event.f = 0;
				event.g = NONE;
				if (g_4e6948->mode != 4)
				{
					function_a7c50(&event);
					function_19eb30(&event);
				}
				g_55e754 = true;
			}
		}
		else
		{
			g_55e754 = false;
		}
	}
	return result;
}

real_point3d *function_b9dd0(long object_index, real_point3d *result);
real function_11ce20(real_vector3d const *a, real_vector3d const *b);
bool function_19f300(long *iterator);
long function_187450(long player_index);

struct s_game_engine_unit_view
{
	byte unknown000[0x88];
	real_vector3d velocity;
};

// @retail 0x161cd0
bool function_161cd0(long object_index)
{
	s_game_engine_player_iterator iterator;
	real_point3d position;
	bool result = false;

	function_b9dd0(object_index, &position);
	iterator.data = g_4e8c24;
	iterator.index = NONE;
	iterator.datum_index = NONE;
	while (function_19f300((long *)&iterator))
	{
		long unit_index = ((s_game_engine_player_state *)iterator.datum)->unit_index;
		s_game_engine_unit_view *unit = (s_game_engine_unit_view *)((s_game_engine_object_header *)g_4e0300->data)[unit_index & 0xffff].object;
		real_vector3d direction;
		real_vector3d velocity = unit->velocity;
		real_point3d unit_position;

		function_b9dd0(unit_index, &unit_position);
		direction.i = position.x - unit_position.x;
		direction.j = position.y - unit_position.y;
		direction.k = 0.0f;
		velocity.k = 0.0f;
		if (direction.i * direction.i + direction.j * direction.j < 100.0f &&
			velocity.i * velocity.i + velocity.j * velocity.j > 2.25f &&
			function_11ce20(&velocity, &direction) < 0.2617994f)
		{
			result = true;
			break;
		}
	}
	return result;
}

struct s_game_engine_player_time_view
{
	byte unknown000[0x28];
	short local_user_index;
	byte unknown02a[0x1a0 - 0x2a];
	long target_index;
	byte unknown1a4[0x1aa - 0x1a4];
	short target_time;
};

// @retail 0x161f30
void function_161f30(long player_index)
{
	long fast_ticks;
	real ticks;
	long slow_ticks;
	s_game_engine_player_time_view *player = (s_game_engine_player_time_view *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c);
	long target_index = NONE;

	ticks = 256.0f / (g_510c54->ticks_per_second * 0.25f);
	__asm
	{
		fld ticks
		fistp fast_ticks
	}
	ticks = 256.0f / g_510c54->ticks_per_second;
	__asm
	{
		fld ticks
		fistp slow_ticks
	}
	if (player->local_user_index != NONE)
	{
		target_index = function_187450(player->local_user_index);
	}
	if (player->target_index != target_index)
	{

		long time;

		if (player->target_time > 0 && target_index == NONE)
		{
			time = player->target_time - slow_ticks;
			player->target_time = (short)(time > 0 ? time : 0);
		}
		else if (player->target_time > 0 && target_index != NONE)
		{
			time = player->target_time - fast_ticks;
			player->target_time = (short)(time > 0 ? time : 0);
		}
		if (player->target_time == 0)
		{
			player->target_index = target_index;
		}
	}
	else
	{
		long time = player->target_time + fast_ticks;

		if (time > 0x100)
		{
			time = 0x100;
		}
		player->target_time = (short)time;
	}
}
// @retail 0x1628f0
void function_1628f0(long player_index, char state)
{
	s_game_engine_player_info *info = (s_game_engine_player_info *)&g_4e9ae8->players[player_index & 0xffff];

	if (g_4e6948->mode != 4)
	{
		long previous = info->state;
		real ticks;
		long ticks_long;

		if (state == 0)
		{
			info->state = 0;
			info->state_ticks = 0;
			info->next_state = 0;
		}
		else if (info->state == 0)
		{
			info->state = state;
			ticks = g_510c54->ticks_per_second * 0.5f;
			__asm
			{
				fld ticks
				fistp ticks_long
			}
			info->state_ticks = (byte)ticks_long;
		}
		else if (state != info->state)
		{
			if (state != info->next_state)
			{
				if (info->next_state == 0)
				{
					info->next_state = state;
				}
				else
				{
					if (info->state != 3)
					{
						info->state = info->next_state;
						ticks = g_510c54->ticks_per_second * 0.5f;
						__asm
						{
							fld ticks
							fistp ticks_long
						}
						info->state_ticks = (byte)ticks_long;
					}
					info->next_state = state;
				}
			}
		}
		else if (state == 3 || info->next_state != 3)
		{
			ticks = g_510c54->ticks_per_second * 0.5f;
			__asm
			{
				fld ticks
				fistp ticks_long
			}
			info->state_ticks = (byte)ticks_long;
		}
		if (previous != info->state && game_engine_get())
		{
			long slot = g_4e9ae8->slots[(short)player_index];

			if (slot != NONE)
			{
				function_b58c0(slot, 4);
			}
		}
	}
}