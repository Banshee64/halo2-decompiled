// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1600F0.CPP: the end of the game engine code (0x1600f0..0x163080):
   queries on the multiplayer globals (g_4e9ae8) and the current engine
   object (g_55e4d0), and the time text the engines draw */

#include "cseries.h"
#include "globals.h"
#include "engine_peer.h"
#include <string.h>

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
	byte unknown02a[0xc0 - 0x2a];
	char team;
	byte unknown0c1[0x21c - 0xc1];
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

real function_242140(long object_index);
void function_1a0180(long tag_index, long string_handle, word *buffer);

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
			real fraction = (real)g_4e9af0.timers[local_user_index] * g_510c54->rate * 4.0f;

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

/* 0x161e10 and 0x161eb0 are in unknown_15e410.cpp, 0x161e60 and 0x162030 in
   unknown_161e60.cpp */

// @retail 0x161ef0
void function_161ef0(long string_handle, word *buffer)
{
	s_tag_header_globals *globals = g_4e034c;

	if (globals && globals->index != NONE)
	{
		long string_list_index = *(long *)(*(byte **)(g_4e3b44[globals->index & 0xffff].bytes + 4) + 0x1c);

		if (string_list_index != NONE)
		{
			function_1a0180(string_list_index, string_handle, buffer);
		}
	}
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
struct s_game_engine_respawn_player
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

static inline s_game_engine_respawn_player *game_engine_player_get(long player_index)
{
	return (s_game_engine_respawn_player *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c);
}

/* the multiplayer globals' per player entries (0x18 bytes at +0x558) */
struct s_game_engine_player_info
{
	bool active;
	byte unknown01[3];
	point3f position;
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
				ticks = g_510c54->field_2_3 * 0.5f;
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
	s_game_engine_respawn_player *player = game_engine_player_get(player_index);

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

byte *record_pool_lookup(s_record_pool *data, long datum_index);

// @retail 0x162c50
bool function_162c50(long player_index, long *spectated_player_index)
{
	s_game_engine_respawn_player *player = game_engine_player_get(player_index);

	if (player->unit_index == NONE)
	{
		long other_index = player->spectated_player_index;

		if (other_index != NONE)
		{
			s_game_engine_player_view *other = (s_game_engine_player_view *)record_pool_lookup(g_4e8c24, other_index);

			if (other && other->active && other->local_user_index == NONE && !(other->flags & 2))
			{
				long ticks = g_510c54->field_2_3 * 10;

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
	s_record_pool *data;
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

/* the voice state (lane D's network code): whether it runs, and per player
   the masks of the players heard */
extern bool g_4c99b8;
extern bool g_476fcc;
bool function_53750(long player_index);

static inline long local_user_next(long user_index)
{
	long result = NONE;
	long i;

	for (i = user_index == NONE ? 0 : user_index + 1; i < 4; i++)
	{
		if (g_4e8c20->entries[i] != NONE)
		{
			result = i;
			break;
		}
	}
	return result;
}

static inline bool voice_player_hears(long listener_index, long talker_index)
{
	dword muted = g_527104.settings.initialized ? g_527104.settings.unknownc8[listener_index] : 0;
	dword heard = g_527104.settings.initialized ? g_527104.settings.unknown108[listener_index] : 0;

	return ((muted | heard) & (1 << talker_index)) != 0;
}

/* counts, per local user, the frames each other player has been talking */
// @retail 0x162de0
void function_162de0(void)
{
	dword users = 0;
	long user_index;

	for (user_index = local_user_next(NONE); user_index != NONE; user_index = local_user_next(user_index))
	{
		long player_index = g_4e8c20->entries[user_index];

		if (user_index != NONE && player_index != NONE)
		{
			s_game_engine_player_iterator iterator;
			dword talking;
			long other_index;

			iterator.index = NONE;
			iterator.datum_index = NONE;
			talking = 0;
			iterator.data = g_4e8c24;
			while (function_19f240((long *)&iterator))
			{
				long talker = iterator.datum_index;

				if (talker != player_index && *(short *)(iterator.datum + 0x28) == NONE)
				{
					long talker_index = talker & 0xffff;
					long listener_index = player_index & 0xffff;

					if (g_4c99b8 && g_476fcc && function_53750(talker_index) &&
						voice_player_hears(listener_index, talker_index))
					{
						g_4e9b38[user_index][talker_index]++;
						talking |= 1 << talker_index;
					}
				}
			}
			for (other_index = 0; other_index < 16; other_index++)
			{
				if (!(talking & (1 << other_index)))
				{
					g_4e9b38[user_index][other_index] = 0;
				}
			}
			users |= 1 << user_index;
		}
	}
	for (user_index = 0; user_index < 4; user_index++)
	{
		if (!(users & (1 << user_index)))
		{
			memset(g_4e9b38[user_index], 0, sizeof(g_4e9b38[user_index]));
		}
	}
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
#include "data_array.h"

// @retail 0x162470
long function_162470(bool flag)
{
	long time_limit = ((s_game_options_time_view *)g_4e6948)->time_limit;
	s_mp_globals *globals = g_4e9ae8;
	long elapsed = g_510c54->game_time - ((s_mp_globals_time_view *)globals)->start_time;
	long time_left;
	long result;

	elapsed = elapsed < 0 ? 0 : elapsed;
	time_left = g_510c54->field_2_3 * time_limit - elapsed;
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

point3f *function_b9dd0(long object_index, point3f *result);
real function_11ce20(vector3f const *a, vector3f const *b);
bool function_19f300(long *iterator);
long function_187450(long player_index);

struct s_game_engine_unit_view
{
	byte unknown000[0x88];
	vector3f velocity;
};

// @retail 0x161cd0
bool function_161cd0(long object_index)
{
	s_game_engine_player_iterator iterator;
	point3f position;
	bool result = false;

	function_b9dd0(object_index, &position);
	iterator.data = g_4e8c24;
	iterator.index = NONE;
	iterator.datum_index = NONE;
	while (function_19f300((long *)&iterator))
	{
		long unit_index = ((s_game_engine_respawn_player *)iterator.datum)->unit_index;
		s_game_engine_unit_view *unit = (s_game_engine_unit_view *)((s_game_engine_object_header *)g_4e0300->data)[unit_index & 0xffff].object;
		vector3f direction;
		vector3f velocity = unit->velocity;
		point3f unit_position;

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

	ticks = 256.0f / (g_510c54->field_2_3 * 0.25f);
	__asm
	{
		fld ticks
		fistp fast_ticks
	}
	ticks = 256.0f / g_510c54->field_2_3;
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
			ticks = g_510c54->field_2_3 * 0.5f;
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
						ticks = g_510c54->field_2_3 * 0.5f;
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
			ticks = g_510c54->field_2_3 * 0.5f;
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

class c_class_58d20;
struct s_session_machine_address;
long network_session_find_member_by_machine(c_class_58d20 *session, const s_session_machine_address *address);
bool network_session_host_boot_member(c_class_58d20 *session, long member_index);

/* the simulation watcher (g_4cf780), as read here */
struct s_162d00_watcher
{
	byte unknown00[0xc];
	c_class_58d20 *session;
};

// @retail 0x162d00
void game_engine_boot_player(long player_index)
{
	byte *player = datum_get_inlined(g_4e8c24, player_index);

	if (player)
	{
		s_event event;
		c_class_58d20 *session;
		long member_index;

		game_engine_event_initialize_inline(&event, 0, 0x26);
		game_engine_event_set_effect_player_inline(&event, player_index);
		game_engine_event_send_inline(&event);
		session = ((s_162d00_watcher *)g_4cf780)->session;
		member_index = network_session_find_member_by_machine(session, (s_session_machine_address const *)(player + 0x14));
		if (member_index != NONE)
		{
			network_session_host_boot_member(session, member_index);
		}
	}
}

/* the scenario's kill planes, as read here (the first one's height at +8) */
struct s_162b90_kill_plane
{
	byte unknown00[8];
	real height;
};

struct s_162b90_scenario_view
{
	byte unknown000[0x318];
	long kill_plane_count;
	s_162b90_kill_plane *kill_planes;
};

void function_10da60(long item_index, point3f *position);

/* true when an item fell below the scenario's kill height */
// @retail 0x162b90
bool function_162b90(long item_index)
{
	bool result = false;
	s_162b90_scenario_view *scenario = (s_162b90_scenario_view *)g_4e0350;

	if (scenario->kill_plane_count > 0)
	{
		s_162b90_kill_plane *kill_plane = scenario->kill_planes;

		if (kill_plane->height != 0.0f && item_index)
		{
			point3f position;

			function_10da60(item_index, &position);
			if (kill_plane->height > position.z)
			{
				result = true;
			}
		}
	}
	return result;
}

#include "unknown_163110.h"

/* a score row of the game engine's scoreboard: the place, the name, the
   score and the time */
struct s_game_engine_score_row
{
	s_text_widget_a place;
	s_text_widget_b name;
	s_text_widget_a score;
	s_text_widget_a time;
};

struct s_score_row_columns
{
	bool time;
	bool score;
};

struct s_160xxx_options_view
{
	byte unknown000[0x180];
	long mode;
};

/* builds a row of the scoreboard at a position */
// @retail 0x1600f0
void game_engine_score_row_build(short const *position, s_score_row_columns const *columns, color3f const *color, real alpha,
	word const *name, long place, long score, long seconds, bool dim, s_game_engine_score_row *row)
{
	long score_width;
	long time_width;
	long name_width;
	color4f local_e2f2b4;
	color4f field_24;
	s_short_rectangle bounds;
	s_text_buffer buffer;
	word time_text[0x100];

	buffer.text[0] = 0;
	time_text[0] = 0;
	score_width = columns->score ? 0x32 : 0;
	time_width = columns->time ? 0x36 : 0;
	name_width = 0xec - time_width - score_width;
	local_e2f2b4.alpha = alpha * 0.25f;
	local_e2f2b4.red = color->red;
	local_e2f2b4.green = color->green;
	local_e2f2b4.blue = color->blue;
	field_24.alpha = alpha;
	field_24.red = color->red * 0.3f + 0.7f;
	field_24.green = color->green * 0.3f + 0.7f;
	field_24.blue = color->blue * 0.3f + 0.7f;
	if (dim)
	{
		field_24.red *= 0.4f;
		field_24.green *= 0.4f;
		field_24.blue *= 0.4f;
	}

	text_buffer_format(&buffer, (const word *)L"%d", place);
	bounds.left = position[0];
	bounds.top = position[1];
	bounds.right = bounds.left + 0x19;
	bounds.bottom = bounds.top + 0x14;
	row->place.initialize(&bounds, &local_e2f2b4, &field_24, buffer.text, 6, true);

	bounds.left = position[0] + 0x19;
	bounds.top = position[1];
	bounds.right = bounds.left + (short)name_width;
	bounds.bottom = bounds.top + 0x14;
	row->name.initialize(&bounds, &local_e2f2b4, &field_24, name, 0x14, false);

	if (score_width)
	{
		text_buffer_format(&buffer, (const word *)L"%d", score);
		bounds.left = position[0] + (short)name_width + 0x19;
		bounds.top = position[1];
		bounds.right = bounds.left + (short)score_width;
		bounds.bottom = bounds.top + 0x14;
		row->score.initialize(&bounds, &local_e2f2b4, &field_24, buffer.text, 6, true);
	}
	else
	{
		row->score.valid = false;
	}

	if (time_width)
	{
		long mode = ((s_160xxx_options_view *)g_4e6948)->mode;

		if (mode >= 3 && (mode <= 4 || mode == 8))
		{
			game_engine_format_time(seconds, time_text);
		}
		else
		{
			function_1630e0(time_text, (const word *)L"%d", seconds);
		}
		bounds.left = position[0] + (short)score_width + (short)name_width + 0x19;
		bounds.top = position[1];
		bounds.right = bounds.left + (short)time_width;
		bounds.bottom = bounds.top + 0x14;
		row->time.initialize(&bounds, &local_e2f2b4, &field_24, time_text, 6, true);
	}
	else
	{
		row->time.valid = false;
	}
}

/* a player's row of the scoreboard: the place, the player's icon, the name,
   the score, the time and the connection quality */
struct s_game_engine_player_row
{
	s_text_widget_a place;
	s_text_widget_a icon;
	s_text_widget_b name;
	s_text_widget_a score;
	s_text_widget_a time;
	s_text_widget_c connection;
};

struct s_player_row_columns
{
	bool time;
	byte unknown01;
	bool score;
};

#include <wchar.h>

void function_159130(long seconds, word *buffer);
void function_22d2ee(word *string, long maximum_length);
bool function_15b2f0(void);
struct s_68a90_entry;
bool function_68a90(s_68a90_entry *entry, long *quality);

struct s_68a90_entry
{
	byte unknown00[0x10];
};

struct s_game_engine_globals_connections_view
{
	byte unknown000[0x6dc];
	s_68a90_entry connections[16];
};

/* builds a player's row of the scoreboard at a position */
// @retail 0x1603f0
void game_engine_player_row_build(short const *position, real alpha, long player_index, word const *name, long place,
	long score, long seconds, bool hide_connection, bool show_icon, bool hide_place, color3f const *color, bool dim,
	s_game_engine_player_row *row, s_player_row_columns const *columns)
{
	long score_width;
	long time_width;
	long name_width;
	long quality;
	color4f local_e2f2b4;
	color4f field_24;
	s_short_rectangle bounds;
	s_text_buffer buffer;
	word name_text[0x100];
	word parsed_name[0x100];
	word time_text[0x100];

	buffer.text[0] = 0;
	time_text[0] = 0;
	score_width = columns->score ? 0x32 : 0;
	time_width = columns->time ? 0x36 : 0;
	name_width = 0xd8 - time_width - score_width;
	local_e2f2b4.alpha = alpha * 0.25f;
	local_e2f2b4.red = color->red;
	local_e2f2b4.green = color->green;
	local_e2f2b4.blue = color->blue;
	field_24.alpha = alpha;
	field_24.red = color->red * 0.3f + 0.7f;
	field_24.green = color->green * 0.3f + 0.7f;
	field_24.blue = color->blue * 0.3f + 0.7f;
	if (dim)
	{
		field_24.red *= 0.4f;
		field_24.green *= 0.4f;
		field_24.blue *= 0.4f;
	}

	if (!hide_place)
	{
		text_buffer_format(&buffer, (const word *)L"%d", place);
		bounds.left = position[0];
		bounds.top = position[1];
		bounds.right = bounds.left + 0x19;
		bounds.bottom = bounds.top + 0x14;
		row->place.initialize(&bounds, &local_e2f2b4, &field_24, buffer.text, 6, true);

		bounds.left = position[0] + 0x19;
		bounds.top = position[1];
		bounds.right = bounds.left + 0x14;
		bounds.bottom = bounds.top + 0x14;
		row->icon.initialize(&bounds, &local_e2f2b4, &field_24, (const word *)L"", 1, false);
		if (show_icon && function_15b2f0())
		{
			row->icon.text[0] = 1;
		}
		else
		{
			row->icon.text[0] = dim ? 2 : 0;
		}
		*(long *)row->icon.unknown34 = player_index;
		row->icon.color_b.alpha = alpha;
	}
	else
	{
		row->place.valid = false;
		row->icon.valid = false;
	}

	name_text[0] = 0;
	wcsncpy((wchar_t *)name_text, (const wchar_t *)name, 0xff);
	parsed_name[0] = 0;
	wcsncpy((wchar_t *)parsed_name, (const wchar_t *)name_text, 0xff);
	function_22d2ee(parsed_name, 0x100);
	wcsncpy((wchar_t *)name_text, (const wchar_t *)parsed_name, 0xff);
	bounds.left = position[0] + 0x2d;
	bounds.top = position[1];
	bounds.right = bounds.left + (short)name_width;
	bounds.bottom = bounds.top + 0x14;
	row->name.initialize(&bounds, &local_e2f2b4, &field_24, name_text, 0x14, false);

	if (score_width && !hide_place)
	{
		text_buffer_format(&buffer, (const word *)L"%d", score);
		bounds.left = position[0] + (short)name_width + 0x2d;
		bounds.top = position[1];
		bounds.right = bounds.left + (short)score_width;
		bounds.bottom = bounds.top + 0x14;
		row->score.initialize(&bounds, &local_e2f2b4, &field_24, buffer.text, 6, true);
	}
	else
	{
		row->score.valid = false;
	}

	if (time_width && !hide_place)
	{
		function_159130(seconds, time_text);
		bounds.left = position[0] + (short)name_width + (short)score_width + 0x2d;
		bounds.top = position[1];
		bounds.right = bounds.left + (short)time_width;
		bounds.bottom = bounds.top + 0x14;
		row->time.initialize(&bounds, &local_e2f2b4, &field_24, time_text, 6, true);
	}
	else
	{
		row->time.valid = false;
	}

	row->connection.valid = false;
	if (!hide_connection)
	{
		s_68a90_entry *entry =
			&((s_game_engine_globals_connections_view *)g_4e9ae8)->connections[player_index & 0xffff];

		quality = 1;
		if (function_68a90(entry, &quality))
		{
			bounds.left = position[0] + (short)time_width + (short)name_width + (short)score_width + 0x2d;
			bounds.top = position[1];
			bounds.right = bounds.left + 7;
			bounds.bottom = bounds.top + 0x14;
			row->connection.initialize(&bounds, &local_e2f2b4, &field_24, (const word *)L"", 2, true);
			row->connection.text[0] = (word)quality;
			row->connection.color_b.alpha = alpha;
		}
	}
}
