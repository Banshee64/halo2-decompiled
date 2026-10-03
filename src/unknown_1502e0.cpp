// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1502E0.CPP: player helpers at the end of the players code (target
   candidates, the local players' view state, the players' census) */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include <string.h>

/* the players (g_4e8c24, 0x21c bytes each) */
struct s_tail_player
{
	short salt;
	word flags;
	byte unknown04[0x28 - 4];
	short local_index;
	byte unknown2a[2];
	long unit_index;
	long previous_unit_index;
	byte unknown34[0x174 - 0x34];
	long object_index174;
	byte unknown178[0x21c - 0x178];
};

/* the object header data (g_4e0300, 12 bytes each) */
struct s_tail_object_header
{
	short salt;
	byte flags;
	byte type;
	byte unknown04[4];
	void *object;
};

/* the objects as the players code sees them */
struct s_tail_object
{
	byte unknown00[0xaa];
	byte type;
};

/* a candidate: its priority and its object */
struct s_target_candidate
{
	short priority;
	byte unknown02[2];
	long object_index;
};

/* g_4e8c20 seen as the players' census */
struct s_player_census
{
	byte unknown00[4];
	bool all_without_units;
	bool any_available;
};

/* the view entries of g_4ed284 (0x94 bytes from +0x14) */
struct s_view_entry
{
	byte unknown00[0x28];
	short index28;
	byte unknown2a[0x8e - 0x2a];
	bool flag8e;
	bool flag8f;
	byte unknown90[0x94 - 0x90];
};

struct s_view_globals
{
	byte unknown00[0x14];
	s_view_entry entries[4];
};

struct s_object;
s_object *function_bae20(long object_index, dword type_mask);
real_point3d *function_b9dd0(long object_index, real_point3d *result);

static inline s_tail_player *tail_player_get(long player_index)
{
	return (s_tail_player *)g_4e8c24->data + (player_index & 0xffff);
}

static inline s_tail_object *tail_object_get(long object_index)
{
	return (s_tail_object *)((s_tail_object_header *)g_4e0300->data)[object_index & 0xffff].object;
}

void function_14cad0(long player_index, long unit_index);
void function_187a60(long datum_index);
void function_152340(void);

// @retail 0x151320
bool function_151320(s_target_candidate *candidate, s_target_candidate *best, long player_index)
{
	bool result = false;

	if (candidate->priority <= best->priority)
	{
		real_point3d player_position;
		real_point3d best_position;
		real_point3d candidate_position;
		real_vector3d to_candidate;
		real_vector3d to_best;

		if (candidate->priority != best->priority)
		{
			return result;
		}
		function_b9dd0(tail_player_get(player_index)->unit_index, &player_position);
		function_b9dd0(best->object_index, &best_position);
		function_b9dd0(candidate->object_index, &candidate_position);
		vector3d_from_points3d(&player_position, &candidate_position, &to_candidate);
		vector3d_from_points3d(&player_position, &best_position, &to_best);
		if (magnitude_squared3d(&to_best) <= magnitude_squared3d(&to_candidate))
		{
			return false;
		}
	}
	*best = *candidate;
	return true;
}

// @retail 0x1518f0
bool function_1518f0(s_target_candidate *candidate, dword type_mask)
{
	bool result = false;
	long object_index = candidate->object_index;

	if (function_bae20(object_index, NONE))
	{
		s_tail_object_header *header = (s_tail_object_header *)g_4e0300->data + (object_index & 0xffff);

		result = (type_mask & (1 << header->type)) != 0;
	}
	return result;
}

// @retail 0x1520f0
void function_1520f0(long player_index)
{
	short local_index = tail_player_get(player_index)->local_index;

	if (local_index != NONE)
	{
		s_view_entry *entry = &((s_view_globals *)g_4ed284)->entries[local_index];

		if (entry->flag8e)
		{
			entry->flag8f = true;
		}
		entry->index28 = NONE;
	}
}

// @retail 0x152340
void function_152340(void)
{
	s_player_census *census = (s_player_census *)g_4e8c20;
	s_data_iterator iterator;
	s_tail_player *player;

	census->all_without_units = true;
	census->any_available = false;
	iterator.data = g_4e8c24;
	iterator.index = NONE;
	while ((player = (s_tail_player *)data_iterator_next_inlined(&iterator)) != 0)
	{
		if (player->unit_index != NONE)
		{
			census->all_without_units = false;
		}
		else if (player->flags & 8)
		{
			census->all_without_units = false;
		}
		else
		{
			census->any_available = true;
		}
	}
}

/* the game options' players (16 of 0xe4 bytes at +0x2dc): an identifier
   and two sets of traits */
struct s_tail_traits
{
	byte unknown00[0x1c];
};

struct s_tail_options_player
{
	byte unknown00[0xe];
	byte identifier[12];
	byte unknown1a[0xac - 0x1a];
	s_tail_traits traits[2];
};

struct s_tail_options
{
	byte unknown00[0x2dc];
	s_tail_options_player players[16];
};

struct s_tail_scenario_entry
{
	byte unknown00[0x2c];
	short value2c;
};

struct s_tail_scenario
{
	byte unknown00[0x100];
	long entry_count;
	s_tail_scenario_entry *entries;
};

struct s_tail_player_identifier
{
	byte unknown00[4];
	byte identifier[12];
};

// @retail 0x1529a0
s_tail_traits *function_1529a0(long player_index)
{
	s_tail_player_identifier *player = (s_tail_player_identifier *)tail_player_get(player_index);
	s_tail_scenario *scenario = (s_tail_scenario *)g_4e0350;
	bool alternate = false;
	s_tail_options_player *entry;
	long i;

	if (scenario->entry_count > 0 && scenario->entries[0].value2c == 1)
	{
		alternate = true;
	}
	entry = ((s_tail_options *)g_4e6948)->players;
	for (i = 0; i < 16; i++, entry++)
	{
		if (memcmp(player->identifier, entry->identifier, sizeof(entry->identifier)) == 0)
		{
			return &entry->traits[alternate ? 1 : 0];
		}
	}
	return 0;
}

/* the units and their definitions as the census of 0x152580 reads them */
struct s_tail_unit
{
	long definition_index;
	byte unknown04[0x184 - 4];
	real value184;
	byte unknown188[0x22a - 0x188];
	short value22a;
	short value22c;
};

struct s_tail_seat
{
	byte unknown00[0x88];
	byte value88;
	byte unknown89[0x90 - 0x89];
	long tag90;
	byte unknown94[4];
	long tag98;
};

struct s_tail_unit_definition
{
	byte unknown00[0x1fc];
	byte value1fc;
	byte unknown1fd[0x2c0 - 0x1fd];
	long count2c0;
	byte unknown2c4[0x2d0 - 0x2c4];
	long seat_count;
	s_tail_seat *seats;
};

struct s_tail_seat_definition
{
	byte unknown00[0x128];
	byte value128;
};

struct s_tail_unit_state
{
	short value0;
	short value2;
	short value4;
	short value6;
};

// @retail 0x152580
void function_152580(long unit_index, s_tail_unit_state *state)
{
	s_tail_unit *unit = (s_tail_unit *)((s_tail_object_header *)g_4e0300->data)[unit_index & 0xffff].object;
	s_tail_unit_definition *definition = (s_tail_unit_definition *)g_4e3b44[unit->definition_index & 0xffff].bytes;
	long value = 0;
	real scaled;
	long rounded;

	if (definition->seat_count > 0)
	{
		s_tail_seat *seat = definition->seats;

		if (seat->tag90 != NONE)
		{
			value = ((s_tail_seat_definition *)g_4e3b44[seat->tag90 & 0xffff].bytes)->value128;
		}
		else if (seat->tag98 != NONE)
		{
			value = seat->value88;
		}
	}
	if (!value)
	{
		value = definition->value1fc;
	}
	state->value0 = (short)value;
	scaled = (1.0f - unit->value184) * 100.0f;
	__asm
	{
		fld scaled
		fistp rounded
	}
	state->value6 = (short)rounded;
	if (definition->count2c0 > 0)
	{
		state->value2 = unit->value22a;
		state->value4 = unit->value22c;
	}
}

/* a horizontal forward vector and its left */
struct s_horizontal_axes
{
	real_vector3d forward;
	real_vector3d left;
};

real function_30bf0(real_vector3d *v);

// @retail 0x1502e0
void function_1502e0(real_vector3d const *forward, real_vector3d const *fallback, s_horizontal_axes *axes)
{
	axes->forward = *forward;
	axes->forward.k = 0.0f;
	if (function_30bf0(&axes->forward) == 0.0f)
	{
		axes->forward = *fallback;
		axes->forward.k = 0.0f;
		if (function_30bf0(&axes->forward) == g_45dbd8)
		{
			axes->forward = *g_4687a8;
		}
	}
	axes->left.i = 0.0f - axes->forward.j;
	axes->left.j = axes->forward.i;
	axes->left.k = 0.0f;
}

/* an object is being deleted: the players forget it (one of the object
   deletion callbacks, g_468664) */
// @retail 0x152cf0
void __stdcall function_152cf0(long object_index)
{
	if ((1 << tail_object_get(object_index)->type) & 3)
	{
		s_data_iterator iterator;
		s_tail_player *player;

		iterator.data = g_4e8c24;
		iterator.index = NONE;
		while ((player = (s_tail_player *)data_iterator_next_inlined(&iterator)) != 0)
		{
			if (player->unit_index == object_index)
			{
				s_tail_player *owner = tail_player_get(iterator.datum_index);

				owner->previous_unit_index = owner->unit_index;
				function_14cad0(iterator.datum_index, NONE);
				function_152340();
			}
			if (player->object_index174 == object_index)
			{
				player->object_index174 = NONE;
			}
			if (player->previous_unit_index == object_index)
			{
				player->previous_unit_index = NONE;
			}
		}
	}
	function_187a60(object_index);
}
