// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_14B560.CPP: the players subsystem entries (initialize, dispose,
   initialize for new map, dispose from old map; the table at 0x441084) */

#include "cseries.h"
#include "globals.h"
#include "game_state.h"
#include "data_array.h"
#include "engine_peer.h"
#include "simulation_world.h"
#include <string.h>

/* the players globals (0x130 bytes; globals.h views them as s_index_table) */
struct s_players_globals
{
	long unknown00;
	byte unknown04[4];
	short unknown08;
	short unknown0a;
	long local_players[4];
	long unknown1c[4];
	byte unknown2c[0x98 - 0x2c];
	long unknown98;
	short unknown9c;
	bool unknown9e;
	bool unknown9f;
	long unknowna0;
	short unknowna4;
	byte unknowna6[0xac - 0xa6];
	long unknownac;
	byte unknownb0[0x130 - 0xb0];
};

/* set while a saved game is being restored: the map entries leave the
   players alone */
bool g_4ed39c;

void ai_players_reset(void);

static inline void player_index_array_reset(long *indices)
{
	for (long i = 0; i < 4; i++)
	{
		indices[i] = NONE;
	}
}

static inline void players_globals_reset(s_players_globals *globals)
{
	globals->unknown08 = 0;
	globals->unknown0a = 0;
	player_index_array_reset(globals->local_players);
	player_index_array_reset(globals->unknown1c);
}

// @retail 0x14b4b0
void players_initialize(void)
{
	g_4e8c24 = data_new_inlined("players", 0x10, 0x21c, 0, g_510c2c);
	s_players_globals *globals = (s_players_globals *)game_state_malloc("players globals", "players_global", sizeof(s_players_globals));
	g_4e8c20 = (s_index_table *)globals;
	players_globals_reset(globals);
}

// @retail 0x14b560
void players_dispose(void)
{
	if (g_4e8c24)
	{
		g_4e8c24 = 0;
	}
	if (g_4e8c20)
	{
		g_4e8c20 = 0;
	}
}

// @retail 0x14b580
void players_initialize_for_new_map(void)
{
	if (!g_4ed39c)
	{
		s_players_globals *globals = (s_players_globals *)g_4e8c20;
		s_data_array *players = g_4e8c24;

		memset(globals, 0, sizeof(s_players_globals));
		players_globals_reset(globals);
		globals->unknowna4 = NONE;
		globals->unknownac = NONE;
		globals->unknown9c = 0;
		players->valid = true;
		data_delete_all(players);
		ai_players_reset();
		((s_players_globals *)g_4e8c20)->unknown98 = NONE;
	}
}

// @retail 0x14b600
void players_dispose_from_old_map(void)
{
	if (!g_4ed39c)
	{
		g_4e8c24->valid = false;
		ai_players_reset();

		s_players_globals *globals = (s_players_globals *)g_4e8c20;

		globals->unknown00 = 0;
		players_globals_reset(globals);
	}
}

/* the appearance of a player (at +0x84 in a player) */
struct s_player_appearance
{
	byte unknown00[5];
	byte primary_change;
	byte secondary_change;
	byte flags;
};

/* a player (0x21c bytes) */
struct s_player
{
	short salt;
	word flags;
	byte unknown04[0x14 - 0x4];
	s_machine_address machine_address;
	byte unknown1a[0x28 - 0x1a];
	short user_index;
	byte unknown2a[2];
	long unit_index;
	byte unknown30[0x84 - 0x30];
	s_player_appearance appearance;
	byte unknown8c[0x21c - 0x8c];
};

/* an object header (12 bytes; globals.h's g_4e0300) */
struct s_object_header_entry
{
	byte unknown00[8];
	byte *object;
};

PRIVATE bool player_appearance_get_function_value(long name, s_player_appearance const *appearance, real *value);

// @retail 0x14cca0
bool player_get_function_value(long player_index, long name, real *value)
{
	real result = 0.0f;
	bool found = false;

	if (player_index != NONE)
	{
		s_player *player = &((s_player *)g_4e8c24->data)[player_index & 0xffff];

		found = player_appearance_get_function_value(name, &player->appearance, &result);
	}

	*value = result;
	return found;
}

// @retail 0x14ccf0
PRIVATE bool player_appearance_get_function_value(long name, s_player_appearance const *appearance, real *value)
{
	real result = 0.0f;

	switch (name)
	{
	case 0x0d0005ff:
		result = 1.0f;
		break;
	case 0x170005fa:
		result = (real)appearance->primary_change * 0.015625f;
		break;
	case 0x170005fb:
		result = (real)appearance->secondary_change * 0.03125f;
		break;
	case 0x180005fc:
		if (!(appearance->flags & 1))
		{
			result = 1.0f;
		}
		break;
	case 0x1d0005fd:
		result = (appearance->flags & 2) ? 1.0f : -1.0f;
		break;
	case 0x1d0005fe:
		result = (appearance->flags & 4) ? 1.0f : -1.0f;
		break;
	}

	*value = result;
	return true;
}

static inline long game_seconds_to_ticks_round(real seconds)
{
	real ticks = g_510c54->ticks_per_second * seconds;
	long result;

	__asm
	{
		fld ticks
		fistp result
	}
	return result;
}

// @retail 0x14cd80
void players_start_countdown(void)
{
	s_players_globals *globals = (s_players_globals *)g_4e8c20;

	if (!globals->unknown9e)
	{
		globals->unknown9e = true;
		globals->unknown9f = false;
		globals->unknowna0 = game_seconds_to_ticks_round(3.0f);
	}
}

/* the input of a player for one tick (0x5c bytes) */
struct s_player_action_target
{
	long type;
	long index;
	long unknown08;
};

struct s_player_action
{
	byte unknown00[4];
	real facing_yaw;
	real facing_pitch;
	real throttle_i;
	real throttle_j;
	real primary_trigger;
	real secondary_trigger;
	byte unknown1c;
	byte flags1d;
	short unknown1e;
	char weapon_index;
	char grenade_index;
	short unknown22;
	short unknown24;
	byte unknown26[2];
	s_player_action_target target;
	long unknown34;
	long unknown38;
	long unknown3c;
	byte unknown40[0x4c - 0x40];
	word flags4c;
	byte unknown4e[2];
	real unknown50;
	real unknown54;
	byte unknown58[4];
};

static inline void player_action_target_clear(s_player_action_target *target)
{
	memset(target, 0, sizeof(s_player_action_target));
	target->type = 0;
	target->index = 0;
}

// @retail 0x14d040
void player_action_initialize(s_player_action *action)
{
	memset(action, 0, sizeof(s_player_action));
	action->unknown1e = NONE;
	action->weapon_index = NONE;
	action->grenade_index = NONE;
	action->unknown22 = NONE;
	action->unknown24 = NONE;
	action->flags4c = 0;
	action->unknown50 = 0.0f;
	action->unknown54 = 0.0f;
	action->unknown34 = NONE;
	action->unknown38 = NONE;
	action->unknown3c = NONE;
	s_player_action_target *target = &action->target;
	player_action_target_clear(target);
	target->index = NONE;
	target->unknown08 = NONE;
}

static inline bool valid_real(real value)
{
	return (*(long *)&value & 0x7f800000) != 0x7f800000;
}

static inline bool player_action_target_valid(s_player_action_target const *target)
{
	short type;

	if (!target)
	{
		return false;
	}
	type = (short)target->type;
	if (type < 0 || type >= 9)
	{
		return false;
	}
	if (type != 0 && target->index == NONE)
	{
		return false;
	}
	return true;
}

// @retail 0x14d0a0
bool player_action_valid(s_player_action const *action)
{
	if (!action)
	{
		return false;
	}

	if (!valid_real(action->facing_pitch) || !valid_real(action->facing_yaw) ||
		!valid_real(action->primary_trigger) || !valid_real(action->secondary_trigger))
	{
		return false;
	}

	if (action->primary_trigger < 0.0f || action->primary_trigger > 1.0f)
	{
		return false;
	}

	if (!valid_real(action->throttle_i) || !valid_real(action->throttle_j))
	{
		return false;
	}

	if (action->throttle_i < -1.0f || action->throttle_i > 1.0f ||
		action->throttle_j < -1.0f || action->throttle_j > 1.0f)
	{
		return false;
	}

	if (action->weapon_index != NONE && (action->weapon_index < 0 || action->weapon_index >= 4))
	{
		return false;
	}
	if (action->grenade_index != NONE && (action->grenade_index < 0 || action->grenade_index >= 4))
	{
		return false;
	}
	if (action->weapon_index != NONE && action->weapon_index == action->grenade_index)
	{
		return false;
	}
	if (action->unknown22 != NONE && (action->unknown22 < 0 || action->unknown22 >= 2))
	{
		return false;
	}
	if (action->unknown24 != NONE && (action->unknown24 < 0 || action->unknown24 >= 4))
	{
		return false;
	}
	if (action->flags1d & 0xfe)
	{
		return false;
	}

	bool valid_values = action->unknown50 >= 0.0f && action->unknown50 <= 1.0f &&
		action->unknown54 >= 0.0f && action->unknown54 <= 1.0f;

	if (action->unknown50 != 0.0f || action->unknown54 != 0.0f || (action->flags4c & 1))
	{
		if (!valid_values || action->unknown34 == NONE)
		{
			return false;
		}
	}
	else if (!valid_values)
	{
		return false;
	}

	return player_action_target_valid(&action->target);
}

bool simulation_machine_is_ready(const s_machine_address *address);
void function_b58c0(long index, dword mask);

static inline void player_connection_changed(long player_index)
{
	if (g_55e4d0[g_4e9ae8->engine_index])
	{
		long slot = g_4e9ae8->slots[(short)player_index];

		if (slot != NONE)
		{
			function_b58c0(slot, 0x200);
		}
	}
}

// @retail 0x14d2a0
void players_update_connections(void)
{
	if (g_4e6948->mode != 4)
	{
		s_data_iterator iterator;
		s_player *player;

		iterator.data = g_4e8c24;
		iterator.index = NONE;
		while ((player = (s_player *)data_iterator_next_inlined(&iterator)) != NULL)
		{
			if (player->flags & 2)
			{
				continue;
			}

			bool connected = simulation_machine_is_ready(&player->machine_address);
			word flags = player->flags;

			if ((bool)(flags & 1) != connected)
			{
				if (connected)
				{
					player->flags = flags | 1;

					c_engine_peer *engine = g_55e4d0[g_4e9ae8->engine_index];
					if (engine)
					{
						engine->p6(iterator.datum_index);
					}
					player_connection_changed(iterator.datum_index);
				}
				else
				{
					player->flags = flags & ~1;
					player_connection_changed(iterator.datum_index);
				}
			}
		}
	}
}

// @retail 0x14dde0
long players_first_active_local_player(void)
{
	s_players_globals *globals = (s_players_globals *)g_4e8c20;
	long result = NONE;

	for (long i = 0; i < 4; i++)
	{
		if (globals->local_players[i] != NONE)
		{
			result = i;
			break;
		}
	}
	return result;
}

// @retail 0x14de10
long players_next_active_local_player(long index)
{
	long result = NONE;

	if (index == NONE)
	{
		index = 0;
	}
	else
	{
		index++;
	}

	for (long i = index; i < 4; i++)
	{
		if (((s_players_globals *)g_4e8c20)->local_players[i] != NONE)
		{
			result = i;
			break;
		}
	}
	return result;
}

// @retail 0x14de50
long player_index_from_absolute_index(long index)
{
	return data_datum_index(g_4e8c24, index);
}

struct s_object;
s_object *function_badc0(long object_index, dword type_mask);

struct s_unit_player_view
{
	byte unknown000[0x13c];
	long player_index;
};

// @retail 0x14de90
long unit_get_player_index(long unit_index)
{
	s_unit_player_view *unit = (s_unit_player_view *)function_badc0(unit_index, 3);

	long player_index = NONE;

	if (unit)
	{
		player_index = unit->player_index;
	}
	return player_index;
}

void function_154d70(s_speed_request *request, s_speed_slot *slot, real scale);

static inline void player_speed_request(short user_index, s_speed_request *request)
{
	long index = user_index;

	if (index != NONE)
	{
		function_154d70(request, &g_510c5c->slots[index], 1.0f);
	}
}

// @retail 0x14ea20
void player_speed_request_14ea20(long player_index)
{
	if (player_index != NONE)
	{
		short user_index = ((s_player *)g_4e8c24->data)[player_index & 0xffff].user_index;

		if (user_index != NONE)
		{
			static short type = 5;
			static real duration = 2.0f;
			static short curve;
			static real amount = 1.0f;
			static real scale;
			static real vector_i = 0.8f;
			static real vector_j;
			static real vector_k = 0.8f;
			s_speed_request request = { 0 };

			request.duration = duration;
			request.curve = curve;
			request.amount = amount;
			request.shake.scale = scale;
			request.shake.vector.i = vector_i;
			request.shake.vector.j = vector_j;
			request.type = type;
			request.priority = 2;
			request.shake.vector.k = vector_k;
			player_speed_request(user_index, &request);
		}
	}
}

// @retail 0x14eb10
void player_speed_request_14eb10(long player_index)
{
	if (player_index != NONE)
	{
		short user_index = ((s_player *)g_4e8c24->data)[player_index & 0xffff].user_index;

		if (user_index != NONE)
		{
			static short type = 2;
			static real duration = 2.0f;
			static short curve;
			static real amount = 1.0f;
			static real scale;
			static real vector_i = 0.35f;
			static real vector_j = 0.35f;
			static real vector_k;
			s_speed_request request = { 0 };

			request.duration = duration;
			request.curve = curve;
			request.amount = amount;
			request.shake.scale = scale;
			request.shake.vector.i = vector_i;
			request.shake.vector.j = vector_j;
			request.type = type;
			request.priority = 2;
			request.shake.vector.k = vector_k;
			player_speed_request(user_index, &request);
		}
	}
}

// @retail 0x14ec00
void player_speed_request_14ec00(long player_index)
{
	if (player_index != NONE)
	{
		short user_index = ((s_player *)g_4e8c24->data)[player_index & 0xffff].user_index;

		if (user_index != NONE)
		{
			s_speed_request request = { 0 };

			request.duration = 2.0f;
			request.curve = 1;
			request.amount = 0.5f;
			request.shake.scale = 1.0f;
			request.shake.vector.i = 0.9176470041275024f;
			request.shake.vector.j = 0.9176470041275024f;
			request.type = 6;
			request.priority = 2;
			request.shake.vector.k = 0.9176470041275024f;
			player_speed_request(user_index, &request);
		}
	}
}

struct s_unit_speed_view
{
	byte unknown000[0x264];
	real value264;
	byte unknown268[0x2b0 - 0x268];
	real value2b0;
};

static inline s_unit_speed_view *player_unit_get(long unit_index)
{
	return (s_unit_speed_view *)((s_object_header_entry *)g_4e0300->data)[unit_index & 0xffff].object;
}

// @retail 0x14ece0
bool function_14ece0(void)
{
	s_data_iterator iterator;
	s_player *player;

	iterator.data = g_4e8c24;
	iterator.index = NONE;
	while ((player = (s_player *)data_iterator_next_inlined(&iterator)) != NULL)
	{
		if (player->unit_index != NONE && player_unit_get(player->unit_index)->value2b0 == 1.0f)
		{
			return true;
		}
	}
	return false;
}

// @retail 0x14ed80
bool function_14ed80(void)
{
	s_data_iterator iterator;
	s_player *player;

	iterator.data = g_4e8c24;
	iterator.index = NONE;
	while ((player = (s_player *)data_iterator_next_inlined(&iterator)) != NULL)
	{
		if (player->unit_index != NONE && player_unit_get(player->unit_index)->value264 == 1.0f)
		{
			return true;
		}
	}
	return false;
}
