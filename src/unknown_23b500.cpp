// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_23B500.CPP: the influences that weigh the game engine's spawn
   points (0x23b500..0x23bc40) */

#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include <math.h>
#include "globals.h"
#include "unknown_157450.h"

/* the influences' weights (g_502258, cleared by unknown_157450.cpp): two
   shared values, then three per influence type */
struct s_spawn_influence_type
{
	real value0;
	real value4;
	real value8;
};

struct s_spawn_influence_globals
{
	real value0;
	real value4;
	s_spawn_influence_type types[12];
};

extern dword g_502258[0x27];

/* an influence on the spawn points (0x20 bytes) */
struct s_spawn_influence
{
	point3f point;
	real value0c;
	real value10;
	real value14;
	real value18;
	real value1c;
};

/* the influences gathered for one spawn (at most 0x80) */
struct s_spawn_influence_list
{
	long count;
	s_spawn_influence influences[0x80];
};

__forceinline void spawn_influence_add(s_spawn_influence_list *list, point3f const *point,
	real value0c, real value10, real value14, real value18, real value1c)
{
	if (list->count < 0x80)
	{
		s_spawn_influence *influence = &list->influences[list->count++];

		influence->point = *point;
		influence->value0c = value0c;
		influence->value10 = value10;
		influence->value14 = value14;
		influence->value18 = value18;
		influence->value1c = value1c;
	}
}

/* adds an influence of this type at the point */
// @retail 0x23ba10
void function_23ba10(long type, s_spawn_influence_list *list, point3f const *point)
{
	s_spawn_influence_globals *globals = (s_spawn_influence_globals *)g_502258;
	s_spawn_influence_type *definition = &globals->types[type];

	spawn_influence_add(list, point, definition->value0, definition->value4, globals->value0,
		globals->value4, definition->value8);
}

/* the influences' weight at the point: an influence counts fully within
   its inner radius and less towards its outer radius (on the ground plane,
   between its heights above and below) */
// @retail 0x23b060
real function_23b060(s_spawn_influence_list *list, point3f const *point)
{
	real total = 0.0f;

	if (game_engine_get())
	{
		for (long i = 0; i < list->count; i++)
		{
			s_spawn_influence *influence = &list->influences[i];
			real weight = 0.0f;
			real dy = point->y - influence->point.y;
			real dx = point->x - influence->point.x;
			real outer_radius = influence->value10;
			real full_weight = influence->value1c;
			real inner_radius = influence->value0c;
			real dz = point->z - influence->point.z;
			real distance_squared = dx * dx + dy * dy;

			if (outer_radius * outer_radius > distance_squared &&
				influence->value14 >= dz && dz >= -influence->value18)
			{
				if (inner_radius * inner_radius > distance_squared || inner_radius >= outer_radius)
				{
					weight = full_weight;
				}
				else
				{
					weight = (real)((1.0f - (sqrt(distance_squared) - inner_radius) / (outer_radius - inner_radius)) * full_weight);
				}
			}
			total += weight;
		}
	}
	return total;
}

/* the spawn settings of the game variant (the two distances at +0x1c) */
struct s_spawn_settings_view
{
	byte unknown00[0x1c];
	real distance1c;
	real distance20;
};

/* the scenario's spawn data: its first block gives the distances */
struct s_spawn_scenario_view
{
	byte unknown000[0x318];
	long spawn_data_count;
	real *spawn_data;
};

struct s_spawn_tag_header_view
{
	byte unknown000[0x16c];
	long globals_index;
};

/* the globals' default distances */
struct s_spawn_globals_tag_view
{
	byte unknown00[0xc];
	byte *data;
};

/* the two distances spawning uses: the variant's, else the scenario's, else
   the globals' */
// @retail 0x23b3f0
void function_23b3f0(s_spawn_settings_view *settings, real *distance20, real *distance1c)
{
	s_spawn_scenario_view *scenario = (s_spawn_scenario_view *)g_4e0350;
	s_spawn_tag_header_view *header = (s_spawn_tag_header_view *)g_4e034c;
	s_tag_instance *tags = g_4e3b44;

	if (settings->distance1c == 0.0f)
	{
		byte *globals = ((s_spawn_globals_tag_view *)tags[header->globals_index & 0xffff].flags)->data;

		if (scenario->spawn_data_count <= 0 || scenario->spawn_data[0] == 0.0f)
		{
			*distance1c = (real)fabs(*(real *)(globals + 0x154));
		}
		else
		{
			*distance1c = (real)fabs(scenario->spawn_data[0]);
		}
	}
	else
	{
		*distance1c = (real)fabs(settings->distance1c);
	}
	if (settings->distance20 == 0.0f)
	{
		byte *globals = ((s_spawn_globals_tag_view *)tags[header->globals_index & 0xffff].flags)->data;

		if (scenario->spawn_data_count <= 0 || scenario->spawn_data[1] == 0.0f)
		{
			*distance20 = (real)fabs(*(real *)(globals + 0x150));
		}
		else
		{
			*distance20 = (real)fabs(scenario->spawn_data[1]);
		}
	}
	else
	{
		*distance20 = (real)fabs(settings->distance20);
	}
}

static __forceinline real spawn_random(bool deterministic, dword *seed)
{
	dword random;

	if (deterministic)
	{
		random = _random(seed, NULL, 0);
	}
	else
	{
		random = _random(&g_4e7408->seed, NULL, 0);
	}
	return (real)random * (1.f / 65535.f);
}

/* the weight of a spawn point: the influences' at the point, plus a random
   share of the globals' random weight */
// @retail 0x23ba90
real function_23ba90(point3f const *point, long player_index, bool deterministic, s_spawn_influence_list *list, dword *seed)
{
	/* the player stays on the stack, unused */
	long *player_index_reference = &player_index;
	point3f position = *point;
	real weight = function_23b060(list, &position);

	if (game_engine_get())
	{
		s_spawn_tag_header_view *header = (s_spawn_tag_header_view *)g_4e034c;
		byte *globals = ((s_spawn_globals_tag_view *)g_4e3b44[header->globals_index & 0xffff].flags)->data;

		weight += **(real **)(globals + 0x534) * spawn_random(deterministic, seed);
	}
	return weight;
}

/* a player as spawning sees it */
struct s_spawn_player_view
{
	byte unknown000[0x2c];
	long unit_index;
	byte unknown030[0xc0 - 0x30];
	char team;
	byte unknown0c1[0x21c - 0xc1];
};

struct s_spawn_player_iterator
{
	s_spawn_player_view *player;
	s_record_pool *data;
	long index;
	long absolute_index;
};

bool function_19f240(long *iterator);

/* a unit as spawning sees it: its position, and whether it is dead */
struct s_spawn_unit_view
{
	byte unknown000[0x30];
	point3f position;
	byte unknown03c[0x10a - 0x3c];
	word flag0 : 1;
	word flag1 : 1;
	word dead : 1;
};

struct s_spawn_object_header_view
{
	byte unknown00[8];
	s_spawn_unit_view *object;
};

/* where each player last died (in the game engine globals at +0x558) */
struct s_spawn_player_state_view
{
	bool valid;
	byte unknown01[3];
	point3f position;
	short value10;
	byte unknown12[0x18 - 0x12];
};

struct s_spawn_engine_globals_view
{
	byte unknown000[0x558];
	s_spawn_player_state_view players[16];
};

/* adds the other players' influences: a living one as a friend or a foe, a
   dead one where it died */
// @retail 0x23b8e0
void function_23b8e0(long player_index, s_spawn_influence_list *list)
{
	s_spawn_player_view *player = &((s_spawn_player_view *)g_4e8c24->data)[player_index & 0xffff];
	s_spawn_player_iterator iterator;

	iterator.data = g_4e8c24;
	iterator.absolute_index = NONE;
	iterator.index = NONE;
	while (function_19f240((long *)&iterator))
	{
		s_spawn_player_view *other = iterator.player;
		long index = iterator.index;
		long unit_index = other->unit_index;

		if (unit_index != NONE && index != player_index)
		{
			s_spawn_unit_view *unit = ((s_spawn_object_header_view *)g_4e0300->data)[unit_index & 0xffff].object;

			if (!TEST_FIELD_BIT(unit->dead))
			{
				if (other->team == player->team)
				{
					function_23ba10(1, list, &unit->position);
				}
				else
				{
					function_23ba10(0, list, &unit->position);
				}
			}
		}
		else
		{
			c_engine_peer *engine = game_engine_get();

			if (!engine || !function_x340af0() || !engine->p27(player->team, other->team))
			{
				s_spawn_player_state_view *state = &((s_spawn_engine_globals_view *)g_4e9ae8)->players[index & 0xffff];

				if (state->valid && state->value10)
				{
					function_23ba10(10, list, &state->position);
				}
			}
		}
	}
}
