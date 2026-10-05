// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_14B560.CPP: the players subsystem entries (initialize, dispose,
   initialize for new map, dispose from old map; the table at 0x441084) */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_123b30.h"
#include "data_array.h"
#include "engine_peer.h"
#include "unknown_067e10.h"
#include <string.h>

/* the players globals (0x130 bytes; globals.h views them as s_index_table) */
struct s_players_globals
{
	long unknown00;
	byte unknown04[4];
	short unknown08;
	short unknown0a;
	long field_x9b462b[4];
	long unknown1c[4];
	/* the machines in the game: which are valid, and their addresses */
	dword machine_valid_mask;
	s_machine_address machine_addresses[16];
	bool local_machine_valid;
	s_machine_address local_machine_address;
	byte unknown97;
	long local_machine_index;
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
	memset(indices, NONE, 4 * sizeof(long));
}

static inline void players_globals_reset(s_players_globals *globals)
{
	globals->unknown08 = 0;
	globals->unknown0a = 0;
	player_index_array_reset(globals->field_x9b462b);
	player_index_array_reset(globals->unknown1c);
}

// @retail 0x14b4b0
void function_14b4b0(void)
{
	g_4e8c24 = data_new_inlined("players", 0x10, 0x21c, 0, g_510c2c);
	s_players_globals *globals = (s_players_globals *)function_123d40("players globals", "players_global", sizeof(s_players_globals));
	g_4e8c20 = (s_index_table *)globals;
	players_globals_reset(globals);
}

// @retail 0x14b560
void function_14b560(void)
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
void function_14b580(void)
{
	if (!g_4ed39c)
	{
		s_players_globals *globals = (s_players_globals *)g_4e8c20;
		s_record_pool *players = g_4e8c24;

		memset(globals, 0, sizeof(s_players_globals));
		players_globals_reset(globals);
		globals->unknowna4 = NONE;
		globals->unknownac = NONE;
		globals->unknown9c = 0;
		players->valid = true;
		record_pool_release_all(players);
		ai_players_reset();
		((s_players_globals *)g_4e8c20)->local_machine_index = NONE;
	}
}

// @retail 0x14b600
void function_14b600(void)
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

#define PIN(value, lower, upper) ((value) < (lower) ? (lower) : ((value) > (upper) ? (upper) : (value)))

/* the configuration of a player (0x90 bytes) */
struct s_type_b07538
{
	byte unknown00[0x44];
	char representation_index;
	byte unknown45[0x7c - 0x45];
	char team_index;
	char unknown7d;
	char unknown7e;
	char unknown7f;
	byte unknown80;
	char unknown81;
	byte unknown82[2];
	long unknown84;
	short unknown88;
	short unknown8a;
	long unknown8c;
};

/* the scenario's starting profiles (0x34 bytes each) */
struct s_scenario_starting_profile_view
{
	byte unknown00[0x2c];
	short representation_index;
	byte unknown2e[0x34 - 0x2e];
};

struct s_scenario_starting_profiles_view
{
	byte unknown000[0x100];
	long starting_profile_count;
	s_scenario_starting_profile_view *starting_profiles;
};

/* the globals tag's player representations (0xbc bytes each) */
struct s_player_representation
{
	byte unknown00[0xb4];
	long field_b4;
	long third_person;
};

struct s_globals_representations_view
{
	byte unknown000[0x138];
	long representation_count;
	s_player_representation *representations;
};

/* the appearance of a player (at +0x84 in a player) */
struct s_player_appearance
{
	byte unknown00[4];
	char representation_index;
	byte primary_change;
	byte secondary_change;
	byte flags;
};

/* a player (0x21c bytes) */
struct s_player
{
	short salt;
	union
	{
		struct
		{
			word connected : 1;
			word field_2_2 : 1;
			word unknown : 14;
		} flags;
		word flags_word;
	};
	byte unknown04[0x14 - 0x4];
	s_machine_address machine_address;
	/* the player's machine, and the user and controller it plays with there */
	short machine_index;
	short machine_user_index;
	byte unknown1e[2];
	long machine_controller_index;
	long controller_index;
	short user_index;
	byte unknown2a[2];
	long unit_index;
	long previous_unit_index;
	byte unknown34[0x38 - 0x34];
	dword latched_buttons;
	word latched_flags;
	byte unknown3e[0x84 - 0x3e];
	s_player_appearance appearance;
	byte unknown8c[0x17c - 0x8c];
	short field_17c;
	short field_17e;
	byte unknown180[0x218 - 0x180];
	short unknown218;
	byte unknown21a[2];
};

/* an object header (12 bytes; globals.h's g_4e0300) */
struct s_object_header_entry
{
	byte unknown00[8];
	byte *object;
};

#define FLAG(bit) (1 << (bit))
#define TEST_FLAG(flags, bit) (((flags) & FLAG(bit)) != 0)
#define SET_FLAG(flags, bit, value) ((value) ? ((flags) |= FLAG(bit)) : ((flags) &= ~FLAG(bit)))

static inline s_player *player_get(long player_index)
{
	return (s_player *)g_4e8c24->data + (player_index & 0xffff);
}

PRIVATE bool player_appearance_get_function_value(long name, s_player_appearance const *appearance, real *value);

// @retail 0x14bc80
void player_configuration_verify(s_type_b07538 *configuration)
{
	s_scenario_starting_profiles_view *scenario = (s_scenario_starting_profiles_view *)g_4e0350;
	s_globals_representations_view *globals = (s_globals_representations_view *)g_4e034c;

	if (g_4e6948->state == 1)
	{
		char representation_index = 0;

		for (long i = 0; i < scenario->starting_profile_count; i++)
		{
			if (scenario->starting_profiles[i].representation_index != NONE)
			{
				representation_index = (char)scenario->starting_profiles[i].representation_index;
				break;
			}
		}
		configuration->team_index = 1;
		configuration->representation_index = representation_index;
	}

	if (configuration->representation_index != NONE)
	{
		configuration->representation_index = PIN(configuration->representation_index, 0, globals->representation_count - 1);
	}
	if (configuration->unknown7e != NONE)
	{
		configuration->unknown7e = PIN(configuration->unknown7e, 0, 0x7f);
	}
	if (configuration->unknown7f != NONE)
	{
		configuration->unknown7f = PIN(configuration->unknown7f, 0, 0x7f);
	}

	if (configuration->unknown84 != NONE && configuration->unknown84 >= 0 && configuration->unknown84 < 16)
	{
		configuration->unknown8c = PIN(configuration->unknown8c, 0, 0x3fffffff);
		configuration->unknown88 = PIN(configuration->unknown88, 0, 0x7f);
		configuration->unknown8a = PIN(configuration->unknown8a, 0, 0x7f);
	}
	else
	{
		configuration->unknown84 = NONE;
		configuration->unknown8c = NONE;
		configuration->unknown8a = NONE;
		configuration->unknown88 = NONE;
	}

	configuration->unknown7d = PIN(configuration->unknown7d, 0, 3);
	if (PIN(configuration->unknown81, 0, 7) != configuration->unknown81)
	{
		configuration->unknown81 = 0;
	}

	if (g_55e4d0[g_4e9ae8->engine_index])
	{
		bool teams = TEST_FIELD_BIT(g_4e6948->flags184.bit0);

		if (teams && configuration->team_index != NONE && !(g_4e9ae8->wa & (1 << configuration->team_index)))
		{
			configuration->team_index = NONE;
		}
	}
}

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
	real ticks = g_510c54->field_2_3 * seconds;
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
	dword buttons;
	real facing_yaw;
	real facing_pitch;
	real throttle_i;
	real throttle_j;
	real field_14_3;
	real field_18_2;
	union
	{
		struct
		{
			byte unknown1c;
			byte flags1d;
		};
		word flags;
	};
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

static inline bool function_x41b793(real value)
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
bool function_14d0a0(s_player_action const *action)
{
	if (!action)
	{
		return false;
	}

	if (!function_x41b793(action->facing_pitch) || !function_x41b793(action->facing_yaw) ||
		!function_x41b793(action->field_14_3) || !function_x41b793(action->field_18_2))
	{
		return false;
	}

	if (action->field_14_3 < 0.0f || action->field_14_3 > 1.0f)
	{
		return false;
	}

	if (!function_x41b793(action->throttle_i) || !function_x41b793(action->throttle_j))
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
		s_record_pool_iterator iterator;
		s_player *player;

		iterator.data = g_4e8c24;
		iterator.index = NONE;
		while ((player = (s_player *)data_iterator_next_inlined(&iterator)) != NULL)
		{
			if (player->flags.field_2_2)
			{
				continue;
			}

			bool connected = simulation_machine_is_ready(&player->machine_address);

			if (TEST_FIELD_BIT(player->flags.connected) != connected)
			{
				player->flags.connected = connected;
				if (connected)
				{
					c_engine_peer *engine = g_55e4d0[g_4e9ae8->engine_index];
					if (engine)
					{
						engine->p6(iterator.datum_index);
					}
				}
				player_connection_changed(iterator.datum_index);
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
		if (globals->field_x9b462b[i] != NONE)
		{
			result = i;
			break;
		}
	}
	return result;
}

// @retail 0x14de10
long function_14de10(long index)
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
		if (((s_players_globals *)g_4e8c20)->field_x9b462b[i] != NONE)
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

struct s_unit_flags_view
{
	byte unknown000[0x10a];
	word unknown10a_bit0 : 1;
	word unknown10a_bit1 : 1;
	word unknown10a_bit2 : 1;
	word unknown10a_bit3 : 1;
};

static inline byte *object_get_unchecked(long object_index)
{
	return ((s_object_header_entry *)g_4e0300->data)[object_index & 0xffff].object;
}

// @retail 0x14deb0
bool __stdcall function_14deb0(long *unit_index)
{
	s_record_pool_iterator iterator;
	s_player *player;

	iterator.data = g_4e8c24;
	iterator.index = NONE;
	while ((player = (s_player *)data_iterator_next_inlined(&iterator)) != NULL)
	{
		if (player->unit_index != NONE &&
			TEST_FIELD_BIT(((s_unit_flags_view *)object_get_unchecked(player->unit_index))->unknown10a_bit3))
		{
			*unit_index = player->unit_index;
			return true;
		}
	}
	return false;
}

/* the fields of a unit, its vehicle and their root object read below */
struct s_unit_vehicle_view
{
	long definition_index;
	struct
	{
		dword unknown_bits : 18;
		dword bit18 : 1;
	} flags;
	byte unknown08[0x14 - 0x8];
	long parent_index;
	byte unknown18[0xaa - 0x18];
	byte unknownaa;
	byte unknownab[0x348 - 0xab];
	byte unknown348;
	byte unknown349[3];
	byte unknown34c;
};

struct s_vehicle_definition_view
{
	byte unknown000[0x1ec];
	struct
	{
		dword unknown_bits : 6;
		dword bit6 : 1;
	} flags;
};

struct s_tag_instance_view
{
	byte unknown00[8];
	byte *data;
	byte unknown0c[4];
};

long function_baf40(long object_index);
bool function_e4050(long object_index);

// @retail 0x14df40
bool __stdcall function_14df40(long *unit_index)
{
	s_record_pool_iterator iterator;
	s_player *player;

	iterator.data = g_4e8c24;
	iterator.index = NONE;
	while ((player = (s_player *)data_iterator_next_inlined(&iterator)) != NULL)
	{
		long unit = player->unit_index;

		if (unit == NONE)
		{
			continue;
		}

		s_unit_vehicle_view *unit_object = (s_unit_vehicle_view *)object_get_unchecked(unit);
		long root_index = NONE;

		for (long object_index = unit; object_index != NONE; object_index = ((s_unit_vehicle_view *)object_get_unchecked(object_index))->parent_index)
		{
			root_index = object_index;
		}

		if (TEST_FIELD_BIT(((s_unit_vehicle_view *)object_get_unchecked(root_index))->flags.bit18))
		{
			*unit_index = player->unit_index;
			return true;
		}

		if (unit_object->parent_index == NONE)
		{
			if (unit_object->unknownaa == 0)
			{
				if (function_e4050(unit) || (unit_object->unknown348 & 1))
				{
					*unit_index = player->unit_index;
					return true;
				}
			}
			else if (unit_object->unknownaa == 1 && unit_object->unknown34c > 0)
			{
				*unit_index = player->unit_index;
				return true;
			}
		}
		else
		{
			s_unit_vehicle_view *vehicle = (s_unit_vehicle_view *)function_badc0(function_baf40(unit_object->parent_index), 2);

			if (vehicle &&
				TEST_FIELD_BIT(((s_vehicle_definition_view *)g_4e3b44[vehicle->definition_index & 0xffff].bytes)->flags.bit6) &&
				vehicle->unknown34c > 0)
			{
				*unit_index = unit_object->parent_index;
				return true;
			}
		}
	}
	return false;
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
		s_player *player = player_get(player_index);

		if (player->user_index != NONE)
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
			player_speed_request(player->user_index, &request);
		}
	}
}

// @retail 0x14eb10
void player_speed_request_14eb10(long player_index)
{
	if (player_index != NONE)
	{
		s_player *player = player_get(player_index);

		if (player->user_index != NONE)
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
			player_speed_request(player->user_index, &request);
		}
	}
}

// @retail 0x14ec00
void player_speed_request_14ec00(long player_index)
{
	if (player_index != NONE)
	{
		s_player *player = player_get(player_index);

		if (player->user_index != NONE)
		{
			s_speed_request request = { 0 };

			request.duration = 2.0f;
			request.curve = 1;
			request.amount = 0.5f;
			request.shake.scale = 1.0f;
			request.type = 6;
			request.priority = 2;
			request.shake.vector.i = 0.9176470041275024f;
			request.shake.vector.j = 0.9176470041275024f;
			request.shake.vector.k = 0.9176470041275024f;
			player_speed_request(player->user_index, &request);
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
	s_record_pool_iterator iterator;
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
	s_record_pool_iterator iterator;
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


// @retail 0x14f040
void player_get_representation(long player_index, long *field_b4, long *third_person)
{
	s_player *player = player_get(player_index);
	s_globals_representations_view *globals = (s_globals_representations_view *)g_4e034c;
	long first = NONE;
	long third = NONE;
	long index;

	if (player->appearance.representation_index < 0)
	{
		index = 0;
	}
	else
	{
		index = player->appearance.representation_index;
		if (index > globals->representation_count - 1)
		{
			index = globals->representation_count - 1;
		}
	}

	if (g_4e6948->state == 2)
	{
		if (index == 0)
		{
			index = 2;
		}
		else if (index == 1)
		{
			index = 3;
		}
	}

	if (index >= 0 && index < globals->representation_count)
	{
		s_player_representation *representation = &globals->representations[index];

		first = representation->field_b4;
		third = representation->third_person;
	}

	if (field_b4)
	{
		*field_b4 = first;
	}
	if (third_person)
	{
		*third_person = third;
	}
}

void player_control_set_unit(long player_index, long unit_index);
void function_1682bf(long unit_index, long user_index, long representation_index);

/* this and function_14f270 keep retail's stack arguments (ret 8) when
   declared __stdcall, unlike most functions under LTCG; perhaps because they
   are self-recursive */
// @retail 0x14f190
void __stdcall player_set_local_user(long player_index, long user_index)
{
	s_player *player = &((s_player *)g_4e8c24->data)[player_index & 0xffff];

	if (player->user_index != user_index)
	{
		if (user_index == NONE)
		{
			player_control_set_unit(player->user_index, NONE);
			function_1682bf(NONE, player->user_index, NONE);
			((s_players_globals *)g_4e8c20)->field_x9b462b[player->user_index] = NONE;
			((s_players_globals *)g_4e8c20)->unknown08--;
			player->user_index = NONE;
		}
		else
		{
			player_set_local_user(player_index, NONE);
			player->user_index = (short)user_index;
			((s_players_globals *)g_4e8c20)->field_x9b462b[user_index] = player_index;
			((s_players_globals *)g_4e8c20)->unknown08++;
			player_control_set_unit(player->user_index, player->unit_index);

			long representation_index = player->appearance.representation_index;

			if (g_4e6948->state == 2)
			{
				if (representation_index == 0)
				{
					representation_index = 2;
				}
				else if (representation_index == 1)
				{
					representation_index = 3;
				}
			}
			function_1682bf(player->unit_index, player->user_index, representation_index);
		}
	}
}

// @retail 0x14f270
void __stdcall function_14f270(long player_index, long controller_index)
{
	s_player *player = &((s_player *)g_4e8c24->data)[player_index & 0xffff];

	if (player->controller_index != controller_index)
	{
		if (controller_index == NONE)
		{
			((s_players_globals *)g_4e8c20)->unknown1c[player->controller_index] = NONE;
			((s_players_globals *)g_4e8c20)->unknown0a--;
			player->controller_index = NONE;
		}
		else
		{
			function_14f270(player_index, NONE);
			player->controller_index = controller_index;
			((s_players_globals *)g_4e8c20)->unknown1c[controller_index] = player_index;
			((s_players_globals *)g_4e8c20)->unknown0a++;
		}
	}
}

/* sets the local machine: the players lose their local users and controllers,
   and the local machine's players take theirs */
// @retail 0x14c880
void players_set_local_machine(s_machine_address const *machine_address)
{
	bool valid = machine_address != NULL;
	long machine_index = NONE;

	if (valid)
	{
		s_players_globals *globals = (s_players_globals *)g_4e8c20;

		for (long i = 0; i < 16; i++)
		{
			if (TEST_FLAG(globals->machine_valid_mask, i) &&
				memcmp(machine_address, &globals->machine_addresses[i], sizeof(s_machine_address)) == 0)
			{
				machine_index = i;
			}
		}
	}

	s_players_globals *globals = (s_players_globals *)g_4e8c20;

	if (valid != globals->local_machine_valid ||
		valid && memcmp(machine_address, &globals->local_machine_address, sizeof(s_machine_address)) != 0 ||
		machine_index != globals->local_machine_index)
	{
		s_record_pool_iterator iterator;
		s_player *player;

		iterator.data = g_4e8c24;
		iterator.index = NONE;
		while ((player = (s_player *)data_iterator_next_inlined(&iterator)) != NULL)
		{
			player_set_local_user(iterator.datum_index, NONE);
			function_14f270(iterator.datum_index, NONE);
		}

		globals = (s_players_globals *)g_4e8c20;
		globals->local_machine_valid = valid;
		globals->local_machine_index = machine_index;
		if (valid)
		{
			globals->local_machine_address = *machine_address;
		}
		else
		{
			memset(&globals->local_machine_address, 0, sizeof(s_machine_address));
		}

		if (globals->local_machine_index != NONE)
		{
			iterator.data = g_4e8c24;
			iterator.index = NONE;
			while ((player = (s_player *)data_iterator_next_inlined(&iterator)) != NULL)
			{
				if (player->machine_index == ((s_players_globals *)g_4e8c20)->local_machine_index)
				{
					player_set_local_user(iterator.datum_index, player->machine_user_index);
					function_14f270(iterator.datum_index, player->machine_controller_index);
				}
			}
		}
	}
}

/* buttons held since the last tick don't press again, and two flags latch
   their presses and releases */
// @retail 0x14f6f0
void player_action_update_latches(long player_index, s_player_action *action)
{
	s_player *player = player_get(player_index);
	dword buttons = action->buttons;
	bool primary_was_down;
	bool primary_down;
	bool secondary_was_down;
	bool secondary_down;

	action->buttons = buttons & ~player->latched_buttons;
	player->latched_buttons = buttons & 0x16100074;
	primary_was_down = TEST_FLAG(player->latched_flags, 0);
	primary_down = TEST_FLAG(action->flags, 0);
	secondary_was_down = TEST_FLAG(player->latched_flags, 4);
	secondary_down = TEST_FLAG(action->flags, 4);

	SET_FLAG(action->flags, 2, primary_down && !primary_was_down);
	SET_FLAG(action->flags, 3, !primary_down && primary_was_down);
	SET_FLAG(player->latched_flags, 0, primary_down);
	SET_FLAG(action->flags, 6, secondary_down && !secondary_was_down);
	SET_FLAG(action->flags, 7, !secondary_down && secondary_was_down);
	SET_FLAG(player->latched_flags, 4, secondary_down);
	if (TEST_FLAG(player->flags_word, 4))
	{
		action->flags &= ~0xf;
		SET_FLAG(player->flags_word, 4, primary_down);
	}
	if (TEST_FLAG(player->flags_word, 5))
	{
		action->flags &= ~0xf0;
		SET_FLAG(player->flags_word, 5, secondary_down);
	}
}

void function_13ac42(long index);
void __stdcall function_cbf60(long unit_index, bool active);
void function_1584c0(long index);
real function_1588b0(long player_index, long type);
void function_152340(void);

struct s_unit_player_assignment
{
	byte unknown00[0xd4];
	long field_d4;
	byte unknownd8[0xf0 - 0xd8];
	real field_f0;
	byte unknownf4[0x13c - 0xf4];
	long player_index;
	long previous_player_index;
};

// @retail 0x14cad0
void function_14cad0(long player_index, long unit_index)
{
	s_player *player = player_get(player_index);
	if (player->unit_index != unit_index)
	{
		if (player->unit_index != NONE)
		{
			s_unit_player_assignment *unit = (s_unit_player_assignment *)object_get_unchecked(player->unit_index);
			if (player->user_index != NONE)
			{
				player_control_set_unit(player->user_index, NONE);
				function_1682bf(NONE, player->user_index, NONE);
			}
			unit->player_index = NONE;
			unit->previous_player_index = player_index;
			long target = ((s_unit_player_assignment *)object_get_unchecked(player->unit_index))->field_d4;
			if (target != NONE)
			{
				function_b58c0(target, 0x400);
			}
			function_cbf60(player->unit_index, false);
			player->unit_index = NONE;
		}
		if (player->user_index != NONE)
		{
			function_13ac42(player->user_index);
		}
		player->latched_buttons = 0;
		player->latched_flags = 0;
		player->field_17c = 0;
		player->field_17e = 0;
		if (unit_index != NONE)
		{
			s_unit_player_assignment *unit = (s_unit_player_assignment *)object_get_unchecked(unit_index);
			unit->player_index = player_index;
			unit->previous_player_index = NONE;
			long target = ((s_unit_player_assignment *)object_get_unchecked(unit_index))->field_d4;
			if (target != NONE)
			{
				function_b58c0(target, 0x400);
			}
			function_cbf60(unit_index, true);
			player->unit_index = unit_index;
			player->previous_unit_index = NONE;
			if (player->user_index != NONE)
			{
				player_control_set_unit(player->user_index, unit_index);
				long representation = player->appearance.representation_index;
				if (g_4e6948->state == 2)
				{
					if (representation == 0)
					{
						representation = 2;
					}
					else if (representation == 1)
					{
						representation = 3;
					}
				}
				function_1682bf(unit_index, player->user_index, representation);
				function_1584c0(player->user_index);
			}
			if (g_55e4d0[g_4e9ae8->engine_index])
			{
				unit->field_f0 = function_1588b0(player_index, 2);
			}
		}
		function_152340();
	}
}

void function_196470(void);
void function_bb8f0(long player_index);

// @retail 0x14c540
void function_14c540(long player_index)
{
	if (g_4e6948->state == 1)
	{
		for (long i = 0; i < MAXIMUM_AI_PLAYERS; i++)
		{
			if (g_4f55cc[i].player_index == player_index)
			{
				g_4f55cc[i].player_index = NONE;
			}
		}
	}
	function_14cad0(player_index, NONE);
	player_set_local_user(player_index, NONE);
	function_14f270(player_index, NONE);
	record_pool_release(g_4e8c24, player_index);
	if (g_55e4d0[g_4e9ae8->engine_index])
	{
		long slot = g_4e9ae8->slots[(short)player_index];
		if (slot != NONE)
		{
			function_b58c0(slot, 0x7ff);
		}
		function_196470();
	}
	function_bb8f0(player_index);
}
