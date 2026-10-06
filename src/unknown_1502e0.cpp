// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1502E0.CPP: player helpers at the end of the players code (target
   candidates, the local players' view state, the players' census) */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_0259d0.h"
#include <string.h>
#include "object_iterator.h"

/* the players (g_4e8c24, 0x21c bytes each) */
struct s_tail_player
{
	short salt;
	word flags;
	byte unknown04[0x10 - 4];
	long leave_time;
	byte machine_address[6];
	short machine_index;
	short machine_user;
	byte unknown1e[2];
	long machine_controller;
	long controller_index;
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
	byte flags;
	byte unknown03;
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
point3f *function_b9dd0(long object_index, point3f *result);

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
	long const *player_reference = &player_index;

	if (candidate->priority <= best->priority)
	{
		point3f player_position;
		point3f best_position;
		point3f candidate_position;
		vector3f to_candidate;
		vector3f to_best;

		if (candidate->priority != best->priority)
		{
			goto done;
		}
		function_b9dd0(tail_player_get(*player_reference)->unit_index, &player_position);
		function_b9dd0(best->object_index, &best_position);
		function_b9dd0(candidate->object_index, &candidate_position);
		vector3d_from_points3d(&player_position, &candidate_position, &to_candidate);
		vector3d_from_points3d(&player_position, &best_position, &to_best);
		real candidate_distance = to_candidate.j * to_candidate.j + to_candidate.i * to_candidate.i + to_candidate.k * to_candidate.k;
		real best_distance = length_sq3f(&to_best);
		if (!(candidate_distance < best_distance))
		{
			result = false;
			goto done;
		}
	}
	*best = *candidate;
	result = true;
done:
	return result;
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
	s_record_pool_iterator iterator;
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
	vector3f forward;
	vector3f left;
};

real function_30bf0(vector3f *v);

// @retail 0x1502e0
void function_1502e0(vector3f const *forward, vector3f const *fallback, s_horizontal_axes *axes)
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
		s_record_pool_iterator iterator;
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

long __stdcall function_cdeb0(long unit_index, long state_name, long a, long b);
long function_cbe60(long unit_index);
bool function_cd660(long unit_index);

// @retail 0x150dd0
bool function_150dd0(long player_index)
{
	s_tail_player *player = tail_player_get(player_index);
	if ((short)function_cdeb0(player->unit_index, NONE, NONE, 0) == NONE)
		return true;
	if (g_4e6948->state != 2)
	{
		long unit_index = player->unit_index;
		if (function_cbe60(unit_index) == NONE && !function_cd660(unit_index))
			return true;
	}
	return false;
}

struct s_player_motion_unit
{
	byte unknown00[0x150];
	vector3f vector150;
	vector3f vector15c;
	byte unknown168[0x180 - 0x168];
	vector3f vector180;
};

struct s_player_motion_control
{
	byte unknown00[0x28];
	vector3f vector28;
	vector3f vector34;
	vector3f vector40;
	byte unknown4c[0x7c - 0x4c];
};

struct s_unit_state_c6ef0;
void function_c6ef0(s_unit_state_c6ef0 *state);
void function_c6de0(long object_index, void *control);

// @retail 0x150790
void function_150790(long unit_index)
{
	s_player_motion_control control;
	s_player_motion_unit *unit = (s_player_motion_unit *)tail_object_get(unit_index);
	function_c6ef0((s_unit_state_c6ef0 *)&control);
	control.vector28 = unit->vector150;
	control.vector34 = unit->vector15c;
	control.vector40 = unit->vector180;
	function_c6de0(unit_index, &control);
}

struct s_player_target_object
{
	byte unknown00[0x30];
	point3f position;
	real radius;
	byte unknown40[0x168 - 0x40];
	vector3f direction;
};

void function_cafc0(long unit_index, point3f *position);
bool function_11e5e0(point3f const *origin, point3f const *center, vector3f const *direction, real radius);
bool function_1078f0(long control_index, vector3f const *direction);
bool function_107870(long device_index);

// @retail 0x1516c0
void function_1516c0(long object_index, long player_index, s_target_candidate *best)
{
	long const *player_reference = &player_index;
	s_target_candidate *const *best_reference = &best;
	s_tail_player *player = tail_player_get(*player_reference);
	s_player_target_object *unit = (s_player_target_object *)tail_object_get(player->unit_index);
	s_player_target_object *object = (s_player_target_object *)tail_object_get(object_index);
	point3f eye;
	function_cafc0(player->unit_index, &eye);
	if (function_11e5e0(&eye, &object->position, &unit->direction, object->radius) &&
		function_1078f0(object_index, &unit->direction) && function_107870(object_index))
	{
		s_target_candidate candidate;
		memset(&candidate, 0, sizeof(candidate));
		candidate.priority = 3;
		candidate.object_index = object_index;
		function_151320(&candidate, *best_reference, *player_reference);
	}
}

struct s_player_pickup_object
{
	long definition_index;
	byte unknown04[0x30 - 4];
	point3f position;
	byte unknown3c[0x12c - 0x3c];
	dword flags;
	byte unknown130[0x14c - 0x130];
	long field_14c;
};

struct s_player_pickup_definition
{
	byte unknown00[0x290];
	short type;
};

bool __stdcall function_cd7b0(long unit_index, long weapon_index, bool *modes);

// @retail 0x151780
void function_151780(long object_index, long player_index, s_target_candidate *best)
{
	long const *player_reference = &player_index;
	s_target_candidate *const *best_reference = &best;
	s_tail_player *player = tail_player_get(*player_reference);
	s_player_target_object *unit = (s_player_target_object *)tail_object_get(player->unit_index);
	s_player_pickup_object *object = (s_player_pickup_object *)tail_object_get(object_index);
	s_player_pickup_definition *definition = (s_player_pickup_definition *)g_4e3b44[object->definition_index & 0xffff].bytes;
	bool special = definition->type != 0;
	real radius = unit->radius + (special ? 0.1f : 0.0f) + 0.3f;
	if (!(object->flags & 1) && object->field_14c != player->unit_index)
	{
		vector3f delta;
		vector3d_from_points3d(&unit->position, &object->position, &delta);
		real distance = delta.j * delta.j + delta.i * delta.i + delta.k * delta.k;
		bool modes[4];
		if (distance < radius * radius && function_cd7b0(player->unit_index, object_index, modes))
		{
			bool first = modes[0];
			bool second = modes[1];
			bool third = modes[2];
			bool fourth = modes[3];
			long flags = 0;
			if (first)
				flags = 1;
			else if (third)
				flags = 2;
			if (second)
				flags |= 4;
			else if (fourth)
				flags |= 8;
			s_target_candidate candidate;
			memset(&candidate, 0, sizeof(candidate));
			candidate.object_index = object_index;
			candidate.flags = (byte)flags;
			candidate.priority = special ? 7 : 1;
			function_151320(&candidate, *best_reference, *player_reference);
		}
	}
}

struct s_player_removed_unit
{
	byte unknown00[0x10a];
	word flags;
	byte unknown10c[0x2a8 - 0x10c];
	long player_index;
};

struct s_player_census_counts
{
	long count;
	byte unknown04[4];
	short user_count;
	short controller_count;
	long users[4];
	long controllers[4];
};

extern byte g_4cf77b;

class c_class_6a600;
void function_69580(c_class_6a600 *world, long player_index);
void function_b7360(long object_index);
void player_control_set_unit(long user_index, long unit_index);
void function_1682bf(long unit_index, long user_index, long character_index);
void function_157de0(long player_index);

// @retail 0x152df0
void function_152df0(long player_index)
{
	long const *player_reference = &player_index;
	s_tail_player *player = tail_player_get(*player_reference);
	if (player->unit_index != NONE)
	{
		s_player_removed_unit *unit = (s_player_removed_unit *)tail_object_get(player->unit_index);
		unit->flags |= 0x400;
		function_b7360(player->unit_index);
	}
	if (g_4e6948->mode == 4)
	{
		struct
		{
			s_player_removed_unit *unit;
			s_type_f1af8e iterator;
		} state;
		state.iterator.signature = 0x86868686;
		state.iterator.type_mask = 3;
		state.iterator.flags = 0;
		state.iterator.index = 0;
		state.iterator.object_index = NONE;
		while ((state.unit = (s_player_removed_unit *)function_baeb0(&state.iterator)) != 0)
		{
			if (state.unit->player_index == *player_reference)
				state.unit->player_index = NONE;
		}
	}
	function_14cad0(*player_reference, NONE);
	player->previous_unit_index = NONE;
	if (g_4cf770 && !g_4cf77b)
		function_69580((c_class_6a600 *)g_4cf77c, *player_reference);
	player->machine_index = NONE;
	memset(player->machine_address, 0, sizeof(player->machine_address));
	player->machine_user = NONE;
	player->machine_controller = NONE;
	s_tail_player *local_player = tail_player_get(*player_reference);
	if (local_player->local_index != NONE)
	{
		player_control_set_unit(local_player->local_index, NONE);
		function_1682bf(NONE, local_player->local_index, NONE);
		s_player_census_counts *globals = (s_player_census_counts *)g_4e8c20;
		globals->users[local_player->local_index] = NONE;
		globals->user_count--;
		local_player->local_index = NONE;
	}
	s_tail_player *controller_player = tail_player_get(*player_reference);
	if (controller_player->controller_index != NONE)
	{
		s_player_census_counts *globals = (s_player_census_counts *)g_4e8c20;
		globals->controllers[controller_player->controller_index] = NONE;
		globals->controller_count--;
		controller_player->controller_index = NONE;
	}
	player->flags |= 2;
	player->leave_time = g_510c54->game_time;
	player->flags &= ~1;
	((s_player_census_counts *)g_4e8c20)->count--;
	function_157de0(*player_reference);
}

struct s_player_target_unit
{
	byte unknown00[0x30];
	point3f position;
	real radius;
	byte unknown40[0x10a - 0x40];
	byte flags;
	byte unknown10b[0x248 - 0x10b];
	long parent_index;
	byte unknown24c[0x348 - 0x24c];
	byte flags348;
};

bool function_f5dc0(long object_index);
bool function_cc0c0(long unit_index);
short __stdcall function_c8ef0(long unit_index, long object_index, long *target_index, short *seat_index);

// @retail 0x151430
void function_151430(long object_index, long player_index, s_target_candidate *best)
{
	long const *player_reference = &player_index;
	s_target_candidate *const *best_reference = &best;
	s_tail_player *player = tail_player_get(*player_reference);
	s_player_target_unit *object = (s_player_target_unit *)tail_object_get(object_index);
	if (!(object->flags & 4))
	{
		if (!function_f5dc0(object_index))
		{
			long unit_index = player->unit_index;
			if (!function_cc0c0(unit_index) && !(player->flags & 0x800))
			{
				long target_index = NONE;
				long seat_index = NONE;
				short type = function_c8ef0(unit_index, object_index, &target_index, (short *)&seat_index);
				if (type)
				{
					s_target_candidate candidate;
					memset(&candidate, 0, sizeof(candidate));
					switch (type)
					{
					case 1: candidate.priority = 4; break;
					case 2: candidate.priority = 5; break;
					case 3: candidate.priority = 6; break;
					}
					candidate.object_index = target_index;
					*(short *)&candidate.flags = (short)seat_index;
					function_151320(&candidate, *best_reference, *player_reference);
				}
			}
		}
		else if (player->unit_index != NONE)
		{
			s_player_target_unit *unit = (s_player_target_unit *)tail_object_get(player->unit_index);
			if (!(object->flags348 & 0x40) && object->parent_index == NONE)
			{
				vector3f delta;
				vector3d_from_points3d(&unit->position, &object->position, &delta);
				real radius = unit->radius + object->radius + 0.2f;
				real distance = delta.j * delta.j + delta.i * delta.i + delta.k * delta.k;
				if (distance < radius * radius)
				{
					s_target_candidate candidate;
					memset(&candidate, 0, sizeof(candidate));
					candidate.priority = 8;
					candidate.object_index = object_index;
					function_151320(&candidate, *best_reference, *player_reference);
				}
			}
		}
	}
}

struct s_tail_transfer_options
{
	byte unknown00[0x2d4];
	s_tail_options_player players[16];
};

struct s_tail_transfer_unit
{
	byte unknown00[0x1fc];
	short seat_index;
	byte unknown1fe[0x212 - 0x1fe];
	char primary_slot;
	char secondary_slot;
	byte unknown214[4];
	long weapons[4];
	long weapon_times[4];
	byte unknown238[6];
	byte grenades[2];
};

struct s_tail_transfer_traits
{
	bool valid;
	byte unknown01;
	s_tail_unit_state weapons[3];
	byte grenades[2];
};

// @retail 0x152660
void function_152660(s_tail_transfer_options *options)
{
	bool alternate = false;
	for (long i = 0; i < 16; i++)
		memcpy(options->players[i].traits, ((s_tail_options *)g_4e6948)->players[i].traits, sizeof(options->players[i].traits));
	s_tail_scenario *scenario = (s_tail_scenario *)g_4e0350;
	if (scenario->entry_count > 0 && scenario->entries[0].value2c == 1)
		alternate = true;
	s_record_pool_iterator iterator;
	iterator.data = g_4e8c24;
	iterator.index = NONE;
	s_tail_player *player;
	while ((player = (s_tail_player *)data_iterator_next_inlined(&iterator)) != 0)
	{
		s_tail_transfer_traits *traits = 0;
		for (long i = 0; i < 16; i++)
		{
			if (memcmp(((s_tail_player_identifier *)player)->identifier, options->players[i].identifier, 12) == 0)
				traits = (s_tail_transfer_traits *)&options->players[i].traits[alternate ? 1 : 0];
		}
		if (traits)
		{
			long unit_index = player->unit_index;
			if (unit_index != NONE)
			{
				s_tail_transfer_unit *unit = (s_tail_transfer_unit *)tail_object_get(unit_index);
				long primary = NONE;
				long secondary = NONE;
				long other = NONE;
				if (unit->seat_index != NONE)
				{
					long now = g_510c54->game_time;
					for (long selection = 0; selection < 2; selection++)
					{
						long chosen = NONE;
						long shortest = 0x7fffffff;
						for (long slot = 0; slot < 4; slot++)
						{
							long weapon = unit->weapons[slot];
							long age = now - unit->weapon_times[slot];
							if (weapon != NONE && (!selection || weapon != primary) && age < shortest)
							{
								shortest = age;
								chosen = weapon;
							}
						}
						if (!selection)
							primary = chosen;
						else
							other = chosen;
					}
				}
				else
				{
					s_tail_transfer_unit *first = (s_tail_transfer_unit *)tail_object_get(unit_index);
					if (first->primary_slot != NONE)
						primary = first->weapons[first->primary_slot];
					s_tail_transfer_unit *second = (s_tail_transfer_unit *)tail_object_get(unit_index);
					if (second->secondary_slot != NONE)
						secondary = second->weapons[second->secondary_slot];
					other = function_cbe60(unit_index);
				}
				if (primary != NONE)
					function_152580(primary, &traits->weapons[0]);
				else
					traits->weapons[0].value0 = 0;
				if (secondary != NONE)
					function_152580(secondary, &traits->weapons[2]);
				else
					traits->weapons[2].value0 = 0;
				if (other != NONE)
					function_152580(other, &traits->weapons[1]);
				else
					traits->weapons[1].value0 = 0;
				traits->grenades[0] = unit->grenades[0];
				traits->grenades[1] = unit->grenades[1];
				traits->valid = true;
			}
			else
				memset(traits, 0, sizeof(*traits));
		}
	}
}

struct s_player_pickup_unit
{
	byte unknown00[0x14];
	long parent_index;
	byte unknown18[0x212 - 0x18];
	char held_slots[2];
	byte unknown214[2];
	char pending_slots[2];
	long weapons[4];
};

struct s_player_weapon_definition
{
	byte unknown00[0x12c];
	dword flags;
};

static __forceinline dword player_weapon_flags(long weapon_index)
{
	s_player_pickup_object *weapon = (s_player_pickup_object *)tail_object_get(weapon_index);
	return ((s_player_weapon_definition *)g_4e3b44[weapon->definition_index & 0xffff].bytes)->flags;
}

// @retail 0x1509e0
void function_1509e0(long player_index, long weapon_index, bool *modes)
{
	long const *weapon_reference = &weapon_index;
	bool *const *modes_reference = &modes;
	s_tail_player *player = tail_player_get(player_index);
	s_player_pickup_unit *unit = (s_player_pickup_unit *)tail_object_get(player->unit_index);
	long held[2];
	long pending[2];
	long count = 0;
	for (long hand = 0; hand < 2; hand++)
	{
		s_player_pickup_unit *current = (s_player_pickup_unit *)tail_object_get(player->unit_index);
		short slot = current->held_slots[hand];
		held[hand] = slot == NONE ? NONE : current->weapons[slot];
		s_player_pickup_unit *next = (s_player_pickup_unit *)tail_object_get(player->unit_index);
		short next_slot = next->pending_slots[hand];
		pending[hand] = next_slot == NONE ? NONE : next->weapons[next_slot];
		if (held[hand] != NONE && (bool)((player_weapon_flags(held[hand]) >> 3) & 1))
		{
			(*modes_reference)[hand] = false;
			(*modes_reference)[hand + 2] = false;
		}
	}
	if (unit->parent_index != NONE)
	{
		for (long hand = 0; hand < 2; hand++)
		{
			(*modes_reference)[hand] = false;
			(*modes_reference)[hand + 2] = false;
		}
	}
	for (long slot = 0; slot < 4; slot++)
	{
		long held_weapon = unit->weapons[slot];
		if (held_weapon != NONE && !(bool)((player_weapon_flags(held_weapon) >> 4) & 1))
		{
			dword flags = player_weapon_flags(*weapon_reference);
			if (!((bool)((flags >> 22) & 1) || (bool)((flags >> 23) & 1)) || held_weapon != held[1] || held_weapon != pending[1])
				count++;
		}
	}
	if (count >= 2)
	{
		for (long hand = 0; hand < 2; hand++)
		{
			if (hand == 1)
			{
				if (held[1] != NONE && !(bool)((player_weapon_flags(held[1]) >> 4) & 1))
					(*modes_reference)[1] = false;
			}
			else if (!(bool)((player_weapon_flags(*weapon_reference) >> 4) & 1))
			{
				(*modes_reference)[hand] = false;
				if (held[hand] != NONE && (bool)((player_weapon_flags(held[hand]) >> 4) & 1))
					(*modes_reference)[hand + 2] = false;
			}
		}
	}
}
