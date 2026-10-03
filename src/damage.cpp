// @flags /O2 /Ob1 /arch:SSE /Gr
/* DAMAGE.CPP: object damage

The functions follow damage.obj in Bungie's May 2003 builds
(halo-symbol-atlas); the profile build's order and sizes are closest to
retail. */

#include "cseries.h"
#include "globals.h"
#include "data_array.h"
#include "real_math.h"
#include <math.h>
#include <string.h>

typedef long string_id;

/* a tag block: a count and the elements' address */
template <typename t_element>
struct s_tag_block_of
{
	long count;
	t_element *elements;
};

/* the damage table (the game globals' at +0xd4): per damage group, the
   multiplier each armor type applies */
struct s_armor_modifier
{
	string_id armor;
	real multiplier;
};

struct s_damage_group
{
	string_id name;
	s_tag_block_of<s_armor_modifier> armor_modifiers;
};

struct s_damage_table
{
	s_tag_block_of<s_damage_group> damage_groups;
};

/* the object fields the damage code reads (g_4e0300 holds 12 byte headers
   with the object at +8) */
struct s_damage_object
{
	long tag_index;
	byte unknown04[8];
	long next_object_index;
	long first_child_object_index;
	long parent_object_index;
	byte unknown18[0x30 - 0x18];
	real_point3d bounding_sphere_center;
	real bounding_sphere_radius;
	byte unknown40[0xaa - 0x40];
	byte type;
	byte unknownab[0xb4 - 0xab];
	long node_index;
	byte unknownb8[0xc2 - 0xb8];
	short owner_team;
	long owner_player_index;
	long owner_object_index;
	byte unknowncc[0xd4 - 0xcc];
	long unknownd4;
	byte unknownd8[0xe4 - 0xd8];
	real maximum_body_vitality;
	real maximum_shield_vitality;
	real body_vitality;
	real shield_vitality;
	real unknownf4;
	byte unknownf8[0x104 - 0xf8];
	short shield_stun_ticks;
	byte unknown106[4];
	struct
	{
		word unknown0 : 2;
		word body_depleted : 1;
		word shield_depleted : 1;
		word shield_double_charged : 1;
		word unknown5 : 2;
		word unknown7 : 1;
		word unknown8 : 4;
		word unknown12 : 1;
		word unknown13 : 1;
		word unknown14 : 1;
	} damage_flags;
	byte unknown10c[0x122 - 0x10c];
	short region_states_offset;
	byte unknown124[0x12c - 0x124];
	long unknown12c;
	byte unknown130[0x138 - 0x130];
	short team;
	byte unknown13a[2];
	long player_index;
	byte unknown140[0x168 - 0x140];
	real_vector3d unknown168;
	byte unknown174[0x1fc - 0x174];
	short unknown1fc;
	byte unknown1fe[0x248 - 0x1fe];
	long unknown248;
};

struct s_damage_object_header
{
	byte unknown00[8];
	s_damage_object *object;
};

#define DAMAGE_OBJECT(index) (((s_damage_object_header *)g_4e0300->data)[(index) & 0xffff].object)

/* who is responsible for damage */
struct s_damage_owner
{
	long player_index;
	long object_index;
	short team;
};

/* a damage event (0x88 bytes) */
struct damage_data
{
	long definition_index;
	byte unknown04[4];
	s_damage_owner owner;
	long unknown14;
	long unknown18;
	long unknown1c;
	short unknown20;
	short unknown22;
	real_point3d position;
	real_point3d origin;
	real_vector3d direction;
	real_vector3d node_direction;
	real unknown54;
	real unknown58;
	real unknown5c;
	real distance;
	real distance_scale;
	bool in_unknown_radius;
	byte unknown69[3];
	real_vector3d cone_direction;
	byte unknown78[0x7c - 0x78];
	short unknown7c;
	short unknown7e;
	byte unknown80[4];
	bool unknown84;
	byte unknown85[3];
};

extern short g_47d8e0;

s_damage_owner const g_440564 = { NONE, NONE, NONE };
s_damage_owner const *g_467420 = &g_440564;

/* what destroying an object's damage regions collects (0x34 bytes) */
struct s_damage_region_accumulator
{
	byte unknown00[0x2c];
	short unknown2c;
	short unknown2e;
	dword flags;
};

/* a damage info region (0x38 bytes) */
/* a damage info region's permutation (0x50 bytes) */
struct s_damage_info_permutation
{
	byte unknown00[4];
	dword flags;
	real unknown08;
	byte unknown0c[0x2c - 0xc];
	real delay;
	byte unknown30[4];
	long effect_index;
	long unknown38;
	byte unknown3c[0x44 - 0x3c];
	real skip_chance;
	byte unknown48[4];
	real body_threshold;
};

struct s_damage_info_region
{
	byte unknown00[4];
	dword flags;
	byte unknown08[4];
	long permutation_count;
	s_damage_info_permutation *permutations;
	byte unknown14[0x38 - 0x14];
};

/* an object's per-region damage state (8 bytes, at object + object->+0x122) */
struct s_object_region_state
{
	word destroyed_permutations;
	byte unknown02;
	byte unknown03;
	word pending;
	byte unknown06[2];
};

struct s_damage_info
{
	byte unknown00[0xbc];
	long region_count;
	s_damage_info_region *regions;
};

/* the object child iterator (unknown_0d0690.cpp) */
struct s_object_child_iterator
{
	long root;
	long current;
	long next;
	long child_value;
	long child_index;
	short child_short;
};

struct s_object;
s_object *function_badc0(long object_index, dword type_mask);
bool game_team_is_enemy(short team_a, short team_b);

/* the object header data's elements (g_4e0300, 12 bytes) */
struct s_damage_object_datum
{
	short salt;
	byte flags;
	byte type;
	byte unknown04[4];
	s_damage_object *object;
};

/* the object definition tag fields read here */
struct s_damage_object_definition
{
	byte unknown00[0xbc];
	struct
	{
		dword unknown0 : 1;
		dword can_be_instant_killed : 1;
	} flags;
};

/* the damage definition tag (jpt!) fields read here */
struct s_damage_definition
{
	real minimum_radius;
	real radius04;
	byte unknown08[4];
	dword flags0c;
	byte unknown10[4];
	dword flags14;
	byte unknown18[0x28 - 0x18];
	real cone_inner_angle;
	real cone_outer_angle;
	byte unknown30[0x44 - 0x30];
	real unknown44;
	byte unknown48[0x58 - 0x48];
	real radius58;
	byte unknown5c[0x64 - 0x5c];
	real player_radius;
	real radius68;
};

/* the players (g_4e8c24, 0x21c byte elements): the unit at +0x2c */
struct s_damage_player
{
	byte unknown00[0x2c];
	long unit_index;
};

#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif
#ifndef MIN
#define MIN(a,b) ((a)>(b)?(b):(a))
#endif
#ifndef CEILING
#define CEILING(n,ceiling) ((n)>(ceiling)?(ceiling):(n))
#endif
#ifndef FLOOR
#define FLOOR(n,floor) ((n)<(floor)?(floor):(n))
#endif
#ifndef PIN
#define PIN(n,floor,ceiling) ((n)<(floor) ? (floor) : CEILING((n),(ceiling)))
#endif

enum
{
	k_maximum_area_of_effect_objects = 64
};

real function_1e9700(short row);
real function_259a0(dword *seed);

bool function_d0690(s_object_child_iterator *iterator);
void function_d0620(long object_index, s_object_child_iterator *iterator);
void function_b7360(long object_index);
void __stdcall function_b8540(long a);
void function_b8b70(long object_index);
void object_destroy_region(s_damage_info *info, long object_index, s_damage_owner const *owner, long region_index,
	s_damage_region_accumulator *accumulator);
void function_da110(long permutation_index, s_damage_info *info, long object_index, s_damage_owner const *owner,
	long region_index, s_damage_region_accumulator *accumulator);
void function_d9d60(bool flag, long marker, long object_index, long effect_index, s_damage_owner const *owner);
void function_ba690(long object_index, byte **states, long *state_count, long *a, long *b);
void function_a8360(long object_index, long region_index, long permutation_index, bool a);
void function_dbfb0(long object_index, s_damage_owner const *owner, bool a, bool b, bool c);
void __stdcall function_d7b80(damage_data *data, long object_index, long a, long b, long c, long d);
void __stdcall function_dbc80(long object_index, short a, short b);
void __stdcall function_e6460(long object_index);
void function_176780(long object_index, real_vector3d const *velocity, real scale_a, long tag_index, real scale_b, real_point3d const *origin, real_vector3d const *direction);
void __stdcall function_ba7f0(long object_index, long a, long b, long c);
short __stdcall function_bb050(long a, dword type_mask, void const *location, real_point3d const *position, real radius,
	long *objects, short maximum_count);
void area_of_effect_cause_damage_to_object(damage_data *data, long object_index, bool child);
real function_30bf0(real_vector3d *v);
void function_baff0(long object_index, real_point3d const *origin, real_point3d *closest_point, real_vector3d *normal);
bool __stdcall function_d6f90(long object_index, real_point3d const *point, damage_data *data);
long unit_get_player_index(long unit_index);
void __stdcall function_153d10(short team, long definition_index, void *a, void *b, long c, real d, real e, long f);
bool function_d74b0(byte const *owner);

void __stdcall function_184250(damage_data const *data);

long function_d5b60(long object_index);
real function_1e9720(long kind, short team);
bool function_108fd0(long object_index);
void function_b58c0(long index, dword mask);

typedef long (__stdcall *t_bsearch_compare_function)(const void *, const void *, const void *);
long bsearch_elements(const void *key, const void *base, long count, long element_size, t_bsearch_compare_function compare, const void *context);
long __stdcall cache_tag_group_compare(void const *a, void const *b, void const *context);

// @retail 0xd5bc0
real damage_armor_table_lookup(string_id group_a, string_id group_b, string_id armor_a, string_id armor_b)
{
	s_damage_table *table = g_4e034c->damage_table;
	real result = 1.0f;
	string_id groups[2] = { group_a, group_b };
	string_id armors[2] = { armor_a, armor_b };

	for (dword i = 0; i < 2; i++)
	{
		string_id group_key = groups[i];
		long group_index = bsearch_elements(&group_key, table->damage_groups.elements, table->damage_groups.count,
			sizeof(s_damage_group), cache_tag_group_compare, NULL);

		if (group_index == NONE)
			continue;

		s_damage_group *group = &table->damage_groups.elements[group_index];
		for (dword j = 0; j < 2; j++)
		{
			string_id armor_key = armors[j];
			long modifier_index = bsearch_elements(&armor_key, group->armor_modifiers.elements, group->armor_modifiers.count,
				sizeof(s_armor_modifier), cache_tag_group_compare, NULL);

			if (modifier_index != NONE)
				result *= group->armor_modifiers.elements[modifier_index].multiplier;
		}
	}
	return result;
}

// @retail 0xd5c90
void object_initialize_vitality(long object_index, real *body_vitality, real *shield_vitality)
{
	s_damage_object *object = DAMAGE_OBJECT(object_index);
	byte *damage_info = (byte *)function_d5b60(object_index);
	real maximum_body = 0.0f;
	real maximum_shield = 0.0f;

	if (damage_info)
	{
		maximum_body = *(real *)(damage_info + 0x28);
		maximum_shield = *(real *)(damage_info + 0x8c);
	}
	if (body_vitality)
		maximum_body = *body_vitality;
	if (shield_vitality)
		maximum_shield = *shield_vitality;

	object->maximum_body_vitality = maximum_body;
	object->maximum_shield_vitality = maximum_shield;
	object->body_vitality = maximum_body > 0.0f ? 1.0f : 0.0f;
	object->shield_vitality = maximum_shield > 0.0f ? 1.0f : 0.0f;
}

// @retail 0xd5d20
real object_get_maximum_body_vitality(long object_index, bool ignore_difficulty)
{
	s_damage_object *object = DAMAGE_OBJECT(object_index);
	real result = object->maximum_body_vitality;

	if (!ignore_difficulty && ((1 << object->type) & 3))
		result = function_1e9720(1, object->team) * result;
	return result;
}

// @retail 0xd5d80
real object_get_maximum_shield_vitality(long object_index, bool ignore_difficulty)
{
	s_damage_object *object = DAMAGE_OBJECT(object_index);
	real result = object->maximum_shield_vitality;

	if (!ignore_difficulty && ((1 << object->type) & 3))
		result = function_1e9720(2, object->team) * result;
	return result;
}

// @retail 0xd6660
void damage_data_new(damage_data *data, long definition_index)
{
	memset(data, 0, sizeof(*data));
	data->definition_index = definition_index;
	data->unknown7c = g_47d8e0;
	data->owner.object_index = NONE;
	data->owner.player_index = NONE;
	data->owner.team = NONE;
	data->unknown14 = NONE;
	data->unknown1c = NONE;
	data->unknown20 = NONE;
	data->unknown22 = g_4686c4;
	data->unknown18 = NONE;
	data->unknown54 = 1.0f;
	data->unknown58 = 1.0f;
	data->unknown5c = 1.0f;
	data->unknown7e = NONE;
	data->unknown84 = false;
}

// @retail 0xd6790
bool object_restore_body(long object_index)
{
	s_damage_object *object = DAMAGE_OBJECT(object_index);
	bool result = false;

	if (!TEST_FIELD_BIT(object->damage_flags.body_depleted) && object->body_vitality < 1.0f)
	{
		object->body_vitality = 1.0f;
		if (DAMAGE_OBJECT(object_index)->unknownd4 != NONE)
			function_b58c0(DAMAGE_OBJECT(object_index)->unknownd4, 0x40);
		result = true;
	}
	return result;
}

// @retail 0xd6af0
bool object_double_charge_shield(long object_index)
{
	s_damage_object *object = DAMAGE_OBJECT(object_index);

	if (object->shield_vitality <= 1.0f)
	{
		bool result = true;

		object->damage_flags.shield_double_charged = true;
		if (object->shield_vitality == 0.0f)
			object->shield_vitality = 0.01f;
		object->shield_stun_ticks = 0;
		return result;
	}
	return false;
}

// @retail 0xd6b60
void object_destroy_notify_children(long object_index)
{
	long child_index = DAMAGE_OBJECT(object_index)->first_child_object_index;

	while (child_index != NONE)
	{
		long next_index = DAMAGE_OBJECT(child_index)->next_object_index;

		if (!function_108fd0(child_index))
			object_destroy_notify_children(child_index);
		child_index = next_index;
	}
}

/* new since 2003: who an object's damage is credited to */
// @retail 0xd66d0
void object_get_damage_owner(long object_index, s_damage_owner *owner)
{
	if (object_index == NONE)
	{
		*owner = *g_467420;
		return;
	}

	s_damage_object *object = DAMAGE_OBJECT(object_index);

	if ((1 << object->type) & 3)
	{
		if (object->unknown248 != NONE)
		{
			object_get_damage_owner(object->unknown248, owner);
			return;
		}
		if (object->unknown12c != NONE || object->player_index != NONE)
		{
			owner->object_index = object_index;
			owner->player_index = object->player_index;
			owner->team = object->team;
			return;
		}
	}
	owner->object_index = object->owner_object_index;
	owner->player_index = object->owner_player_index;
	owner->team = object->owner_team;
}

// @retail 0xd6a70
void object_deplete_shield(long object_index)
{
	s_damage_object *object = DAMAGE_OBJECT(object_index);

	if (!TEST_FIELD_BIT(object->damage_flags.shield_depleted))
	{
		long damage_info = function_d5b60(object_index);

		if (damage_info && object->maximum_shield_vitality > 0.0f)
			function_176780(object_index, (real_vector3d const *)g_467420, 0.0f, *(long *)(damage_info + 0xb0), 0.0f, NULL, NULL);
		object->unknownf4 = 0.0f;
		object->damage_flags.shield_depleted = true;
		function_ba7f0(object_index, NONE, 2, NONE);
	}
}

// @retail 0xd6800
void object_deplete_body(long object_index, s_damage_owner const *owner, bool notify_parent, bool unknown)
{
	s_damage_object *object = DAMAGE_OBJECT(object_index);
	s_damage_region_accumulator accumulator;

	if (TEST_FIELD_BIT(object->damage_flags.body_depleted))
		return;

	object->damage_flags.body_depleted = true;
	function_b8b70(object_index);
	memset(&accumulator, 0, sizeof(accumulator));
	if (unknown)
		accumulator.flags |= 0x400;
	else
		accumulator.flags &= ~0x400;

	if (DAMAGE_OBJECT(object_index)->unknownd4 != NONE)
		function_b58c0(DAMAGE_OBJECT(object_index)->unknownd4, 1);

	if (g_4e6948->mode != 4)
	{
		s_damage_info *info = (s_damage_info *)function_d5b60(object_index);

		if (info)
		{
			for (long i = 0; i < info->region_count; i++)
			{
				if (info->regions[i].flags & 2)
					object_destroy_region(info, object_index, owner, i, &accumulator);
			}
		}
	}

	if (g_4e6948->mode != 4 && object->type == 1)
	{
		for (long child_index = object->first_child_object_index; child_index != NONE;)
		{
			s_damage_object *child = DAMAGE_OBJECT(child_index);

			if (child->type == 0 && child->unknown1fc != NONE)
				function_dbfb0(child_index, owner, false, false, false);
			child_index = child->next_object_index;
		}
	}

	object_deplete_shield(object_index);

	if (g_4e6948->mode != 4 && object->parent_object_index != NONE && notify_parent)
	{
		s_damage_object *parent = DAMAGE_OBJECT(object->parent_object_index);

		if (TEST_FIELD_BIT(parent->damage_flags.unknown12) && ((1 << parent->type) & 2))
		{
			s_object_child_iterator iterator;
			bool last = true;

			function_d0620(object->parent_object_index, &iterator);
			while (function_d0690(&iterator))
			{
				if (iterator.child_short != NONE && iterator.child_index != object_index)
					last = false;
			}
			if (last)
			{
				parent->damage_flags.unknown13 = true;
				function_b7360(object->parent_object_index);
			}
		}
	}

	if (accumulator.unknown2c || accumulator.unknown2e)
		function_dbc80(object_index, accumulator.unknown2c, accumulator.unknown2e);
	if ((accumulator.flags & 4) && g_4e6948->mode != 4)
		function_b8540(object_index);
}

// @retail 0xd6bc0
void object_destroy(long object_index)
{
	s_damage_object *object = DAMAGE_OBJECT(object_index);
	s_damage_region_accumulator accumulator;

	object_deplete_body(object_index, g_467420, true, false);

	s_damage_info *info = (s_damage_info *)function_d5b60(object_index);

	memset(&accumulator, 0, sizeof(accumulator));
	if (info)
	{
		for (long i = 0; i < info->region_count; i++)
		{
			if (info->regions[i].flags & 8)
				object_destroy_region(info, object_index, g_467420, i, &accumulator);
		}
	}
	if (object->type == 0)
		function_e6460(object_index);
	object_destroy_notify_children(object_index);
	function_b8540(object_index);
}

/* whether a damage event can affect an object */
// @retail 0xd72e0
bool function_d72e0(long object_index, damage_data const *data)
{
	s_damage_object_datum *datum = &((s_damage_object_datum *)g_4e0300->data)[object_index & 0xffff];
	s_damage_definition *definition = (s_damage_definition *)g_4e3b44[data->definition_index & 0xffff].bytes;
	s_damage_object *object = datum->object;
	bool result = false;

	if ((datum->flags & 0x10) || (object->unknown04[0] & 1))
		return result;

	if (definition->flags0c & 2)
	{
		s_damage_object *unit = (s_damage_object *)function_badc0(object_index, 3);
		long player_index = unit ? unit->player_index : NONE;

		return player_index != NONE;
	}

	dword flags = definition->flags14;
	if ((flags & 1) && object_index == data->owner.object_index)
		return result;

	if ((1 << object->type) & 3)
	{
		if ((flags & 0x8000) && g_4e6948->state == 1 && object->player_index != NONE)
			return result;
		if ((flags & 8) && !game_team_is_enemy(object->team, data->owner.team))
			return result;
	}
	return true;
}

/* whether any entry of the block at +0x70 (0x60 byte entries) has the flag
   at +0x40 of the structure it points to */
// @retail 0xd74b0
bool function_d74b0(byte const *owner)
{
	long count = *(long const *)(owner + 0x74);
	byte const *entries = *(byte const *const *)(owner + 0x70);

	bool result = false;

	for (long i = 0; i < count; i++)
	{
		if ((*(byte const *const *)(entries + i * 0x60 + 0x40))[0x40])
			return true;
	}
	return result;
}

// @retail 0xd7ae0
long get_player_index_from_object_or_parents(long object_index)
{
	long result = NONE;

	while (object_index != NONE)
	{
		s_damage_object_datum *datum = (s_damage_object_datum *)datum_get_inlined(g_4e0300, object_index);

		if (datum && ((1 << datum->type) & 3) && datum->object)
		{
			s_damage_object *unit = (s_damage_object *)function_badc0(object_index, 3);

			return unit ? unit->player_index : NONE;
		}
		object_index = DAMAGE_OBJECT(object_index)->parent_object_index;
	}
	return result;
}

/* the creature instant-kill roll: clears *instant_kill unless the damage may
   kill the object outright, and returns the outcome */
// @retail 0xd73c0
bool function_d73c0(long object_index, damage_data const *data, bool *instant_kill)
{
	s_damage_object *object = DAMAGE_OBJECT(object_index);
	s_damage_definition *definition = (s_damage_definition *)g_4e3b44[data->definition_index & 0xffff].bytes;
	bool result = false;

	if (*instant_kill && (definition->flags14 & 0x1000))
	{
		*instant_kill = result;
		if (((1 << object->type) & 0x1000) &&
			TEST_FIELD_BIT(((s_damage_object_definition *)g_4e3b44[object->tag_index & 0xffff].bytes)->flags.can_be_instant_killed) &&
			object_index != data->owner.object_index)
		{
			real chance = function_1e9700(8);

			*instant_kill = true;
			if ((definition->flags14 & 0x400) && (data->unknown04[0] & 0x40))
				*instant_kill = false;
			if (chance > 0.0f && function_259a0((dword *)g_4e7408) < chance * 0.5f)
			{
				*instant_kill = false;
				return result;
			}
			if (*instant_kill)
				return true;
		}
	}
	return result;
}

/* damages everything in the damage's radius; returns the first player unit
   hit, else the last object hit */
// @retail 0xd6c80
long area_of_effect_cause_damage(damage_data *data, long ignore_object_index)
{
	s_damage_definition *definition = (s_damage_definition *)g_4e3b44[data->definition_index & 0xffff].bytes;
	real radius = MAX(definition->radius04, MAX(definition->radius58, definition->radius68));
	long objects[k_maximum_area_of_effect_objects];
	long object_count = function_bb050(0, (definition->flags0c & 2) ? 3 : 0, &data->unknown1c, &data->position, radius,
		objects, k_maximum_area_of_effect_objects);
	long first_object_index = NONE;
	long last_object_index = NONE;

	*(dword *)data->unknown04 |= 1;

	if (definition->player_radius > radius)
	{
		long player_index = NONE;

		while ((player_index = data_next_absolute_index_inlined(g_4e8c24, player_index + 1)) != NONE)
		{
			s_damage_player *player = (s_damage_player *)(g_4e8c24->data + g_4e8c24->size * player_index);

			if (!player)
				break;
			if (player->unit_index == NONE)
				continue;

			long root_index = player->unit_index;
			while (DAMAGE_OBJECT(root_index)->parent_object_index != NONE)
				root_index = DAMAGE_OBJECT(root_index)->parent_object_index;

			s_damage_object *root = DAMAGE_OBJECT(root_index);
			if (object_count >= k_maximum_area_of_effect_objects)
				continue;

			real dx = root->bounding_sphere_center.x - data->position.x;
			real dy = root->bounding_sphere_center.y - data->position.y;
			real dz = root->bounding_sphere_center.z - data->position.z;
			real inner = root->bounding_sphere_radius + radius;
			if (inner * inner >= dz * dz + dy * dy + dx * dx)
				continue;

			real outer = definition->player_radius + root->bounding_sphere_radius;
			if (outer * outer >= dz * dz + dy * dy + dx * dx)
				objects[object_count++] = player->unit_index;
		}
	}

	for (long i = 0; i < object_count; i++)
	{
		long object_index = objects[i];

		if (definition->flags0c & 2)
		{
			s_damage_object *unit = (s_damage_object *)function_badc0(object_index, 3);
			if (!unit || unit->player_index == NONE)
				continue;
		}
		if (object_index == ignore_object_index)
			continue;

		s_damage_object *unit = (s_damage_object *)function_badc0(object_index, 3);
		damage_data copy = *data;

		area_of_effect_cause_damage_to_object(&copy, object_index, false);
		last_object_index = object_index;
		if (unit)
		{
			if (first_object_index == NONE)
			{
				first_object_index = object_index;
			}
			else
			{
				s_damage_object *other = (s_damage_object *)function_badc0(object_index, 3);
				if (other && other->player_index != NONE)
					first_object_index = object_index;
			}
		}
	}

	if (g_4e6948->mode != 4)
		function_184250(data);

	return first_object_index != NONE ? first_object_index : last_object_index;
}

// @retail 0xd74e0
void area_of_effect_cause_damage_to_object(damage_data *data, long object_index, bool child)
{
	for (;;)
	{
		s_damage_object *object = DAMAGE_OBJECT(object_index);
		bool affects = function_d72e0(object_index, data);
		bool instant_kill = function_d73c0(object_index, data, &affects);

		if (affects)
		{
			long damage_info = function_d5b60(object_index);
			s_damage_definition *definition = (s_damage_definition *)g_4e3b44[data->definition_index & 0xffff].bytes;
			real_point3d closest_point;
			real_vector3d normal;
			bool outside = false;
			real scale;

			function_baff0(object_index, &data->origin, &closest_point, &normal);
			data->direction.i = closest_point.x - data->origin.x;
			data->direction.j = closest_point.y - data->origin.y;
			data->direction.k = closest_point.z - data->origin.z;
			data->distance = function_30bf0(&data->direction);

			if (object->node_index == NONE)
			{
				data->node_direction = data->direction;
			}
			else
			{
				byte *node = g_51e9b8->data + (object->node_index & 0xffff) * 0xa0;
				real_point3d center;

				if (node && *(long *)(node + 0x74) && !function_d74b0(node))
					center = *(real_point3d *)(*(byte **)(*(byte **)(*(byte **)(node + 0x70) + 0x40) + 0x3c) + 0x70);
				else
					center = object->bounding_sphere_center;

				data->node_direction.i = center.x - data->origin.x;
				data->node_direction.j = center.y - data->origin.y;
				data->node_direction.k = center.z - data->origin.z;
				function_30bf0(&data->node_direction);

				real dx = data->origin.x - closest_point.x;
				real dy = data->origin.y - closest_point.y;
				real dz = data->origin.z - closest_point.z;
				if (0.0025f > dz * dz + dy * dy + dx * dx)
				{
					data->node_direction.i += 0.0f - normal.i;
					data->node_direction.j += 0.0f - normal.j;
					data->node_direction.k += 0.0f - normal.k;
				}
				else
				{
					data->node_direction.i += data->direction.i;
					data->node_direction.j += data->direction.j;
					data->node_direction.k += data->direction.k;
					function_30bf0(&data->node_direction);
				}
			}

			if (definition->cone_outer_angle != 0.0f)
			{
				real_vector3d *cone = &data->cone_direction;

				if (fabs(1.0f - (cone->i * cone->i + cone->j * cone->j + cone->k * cone->k)) <= 0.0001f)
				{
					real dot = cone->k * data->direction.k + cone->j * data->direction.j + cone->i * data->direction.i;
					real angle = (real)acos(PIN(dot, -1.0f, 1.0f));

					if (definition->cone_inner_angle + 0.0001f <= angle)
					{
						if (definition->cone_outer_angle > angle)
						{
							data->unknown5c = PIN(1.0f - (angle - definition->cone_inner_angle) /
								(definition->cone_outer_angle - definition->cone_inner_angle), 0.0f, 1.0f);
						}
						else
						{
							scale = 0.0f;
							outside = true;
							goto distance_done;
						}
					}
				}
			}

			scale = 1.0f;
			if (definition->radius04 - definition->minimum_radius > 0.0f)
			{
				scale = 1.0f - (data->distance - definition->minimum_radius) / (definition->radius04 - definition->minimum_radius);
				if (0.0f > scale)
				{
					outside = true;
					scale = 0.0f;
				}
				else if (scale > 1.0f)
				{
					scale = 1.0f;
				}
			}

		distance_done:
			data->distance_scale = scale;
			data->unknown58 = scale;
			if (outside)
			{
				*(dword *)data->unknown04 |= 0x2000;
				data->unknown5c = 0.0f;
			}
			if (!(definition->flags0c & 1))
				data->unknown54 = scale;

			if (definition->player_radius > 0.0f)
			{
				s_damage_object *unit = (s_damage_object *)function_badc0(object_index, 3);

				if (unit && unit->player_index != NONE)
					data->unknown58 = 1.0f - PIN(data->distance / definition->player_radius, 0.0f, 1.0f);
				else
					data->unknown58 = 0.0f;
			}

			data->in_unknown_radius = definition->radius68 > data->distance;
			if (scale > 0.0f || data->in_unknown_radius || data->unknown58 > 0.0f)
			{
				if (!child && function_d6f90(object_index, &closest_point, data))
					return;

				if (g_4e6948->mode == 4)
				{
					long definition_index = data->definition_index;

					if (((1 << object->type) & 3) && definition_index != NONE && unit_get_player_index(object_index) != NONE)
					{
						long player_index = unit_get_player_index(object_index);
						short team = *(short *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c + 0x28);

						if (team != NONE)
						{
							real_vector3d direction;
							s_damage_owner owner = { NONE, NONE, NONE };

							direction.i = 0.0f - object->unknown168.i;
							direction.j = 0.0f - object->unknown168.j;
							direction.k = 0.0f - object->unknown168.k;
							function_153d10(team, definition_index, &owner, &direction, 0, 1.0f, 1.0f, 0);
						}
					}
				}
				else
				{
					function_d7b80(data, object_index, NONE, NONE, NONE, 0);
					if (instant_kill)
						*(dword *)data->unknown04 |= 0x40;
					if (damage_info && (*(byte *)damage_info & 8) && object->first_child_object_index != NONE)
						area_of_effect_cause_damage_to_object(data, object->first_child_object_index, true);
				}
			}
		}

		if (!child || object->next_object_index == NONE)
			return;
		child = true;
		object_index = object->next_object_index;
	}
}

/* the material of one of an object's model regions */
// @retail 0xd8ae0
void function_d8ae0(long object_index, long region_index, short *material)
{
	short result = g_47d8e0;

	if (object_index != NONE)
	{
		long model_index = *(long *)(g_4e3b44[DAMAGE_OBJECT(object_index)->tag_index & 0xffff].bytes + 0x38);

		if (model_index != NONE)
		{
			byte *regions = *(byte **)(g_4e3b44[model_index & 0xffff].bytes + 0x5c);

			*material = *(short *)(regions + region_index * 0x14 + 0x10);
			return;
		}
	}
	*material = result;
}

/* the bits of the entries (0x14 bytes, at +0xe4, counted at +0xe0) that
   match the target's value at +0x3c and are not masked */
// @retail 0xdaa50
void function_daa50(byte const *owner, byte const *target, word mask, word *bits)
{
	for (long i = 0; i < *(long const *)(owner + 0xe0); i++)
	{
		byte const *entry = *(byte const *const *)(owner + 0xe4) + i * 0x14;

		if (*(long const *)(entry + 8) == *(long const *)(target + 0x3c) && !(mask & (1 << i)))
			*bits |= (word)(1 << i);
	}
}

/* recent damage fades: after its delay (in seconds, from the damage info at
   +0x30/+0x38) each accumulator decays to zero over its decay time
   (+0x34/+0x3c); the timer stops at NONE once both are zero */
// @retail 0xd8b50
bool function_d8b50(real *recent_a, real *recent_b, char *timer, byte const *damage_info, bool reset)
{
	bool result = false;

	if (*timer != NONE)
	{
		real delay_a = 0.0f;
		real decay_a = 2.0f;
		real delay_b = 2.0f;
		real decay_b = 2.0f;

		if (damage_info)
		{
			delay_a = *(real const *)(damage_info + 0x30);
			decay_a = *(real const *)(damage_info + 0x34);
			delay_b = *(real const *)(damage_info + 0x38);
			decay_b = *(real const *)(damage_info + 0x3c);
		}

		*timer = (char)MIN(*timer + 1, 0x7f);

		if (*recent_a > 0.0f)
		{
			real seconds = g_510c54->ticks_per_second * delay_a;
			long ticks;

			__asm
			{
				fld seconds
				fistp ticks
			}
			if (*timer >= ticks)
			{
				if (decay_a > 0.0001f)
				{
					*recent_a -= 1.0f / (g_510c54->ticks_per_second * decay_a);
					*recent_a = FLOOR(*recent_a, 0.0f);
				}
				else
				{
					*recent_a = 0.0f;
				}
			}
		}

		if (*recent_b > 0.0f)
		{
			real seconds = g_510c54->ticks_per_second * delay_b;
			long ticks;

			__asm
			{
				fld seconds
				fistp ticks
			}
			if (*timer >= ticks)
			{
				if (decay_b > 0.0001f)
				{
					*recent_b -= 1.0f / (g_510c54->ticks_per_second * decay_b);
					*recent_b = FLOOR(*recent_b, 0.0f);
				}
				else
				{
					*recent_b = 0.0f;
				}
			}
		}

		if (reset)
			*recent_a = 0.0f;
		if ((reset || *recent_a == 0.0f) && *recent_b == 0.0f)
			*timer = NONE;
		result = true;
	}
	return result;
}

/* a weighted random choice among the entries function_daa50 matches: sets
   the chosen entry's bit */
// @retail 0xda860
void function_da860(byte const *owner, byte const *target, word mask, word *bits)
{
	long count = *(long const *)(owner + 0xe0);
	real total = 0.0f;
	long i;

	for (i = 0; i < count; i++)
	{
		byte const *entry = *(byte const *const *)(owner + 0xe4) + i * 0x14;

		if (!(mask & (1 << i)) && *(long const *)(entry + 8) == *(long const *)(target + 0x3c))
			total += *(real const *)(entry + 0xc);
	}

	real choice = (real)random_next(&g_4e7408->unknown0) * (1.f / 65535.f) * total;
	real sum = 0.0f;

	for (i = 0; i < count; i++)
	{
		byte const *entry = *(byte const *const *)(owner + 0xe4) + i * 0x14;

		if (!(mask & (1 << i)) && *(long const *)(entry + 8) == *(long const *)(target + 0x3c))
		{
			sum += *(real const *)(entry + 0xc);
			if (sum >= choice)
			{
				*bits |= (word)(1 << i);
				return;
			}
		}
	}
}

/* how much of a vehicle's damage reaches its rider: the rider's seat's
   entry in the vehicle's damage info, scaled by the definition */
// @retail 0xdc230
real function_dc230(long rider_index, long vehicle_index, damage_data const *data)
{
	s_damage_definition *definition = (s_damage_definition *)g_4e3b44[data->definition_index & 0xffff].bytes;
	s_damage_object *rider = DAMAGE_OBJECT(rider_index);
	s_damage_object *vehicle = DAMAGE_OBJECT(vehicle_index);
	byte *damage_info = (byte *)function_d5b60(vehicle_index);
	real result = 1.0f;

	if (damage_info && ((1 << rider->type) & 3) && ((1 << vehicle->type) & 3) && !(definition->flags14 & 0x2000) &&
		rider->unknown1fc != NONE)
	{
		byte *seats = *(byte **)(g_4e3b44[vehicle->tag_index & 0xffff].bytes + 0x1cc);
		long seat_key = *(long *)(seats + rider->unknown1fc * 0xb0 + 4);
		long count = *(long *)(damage_info + 0xd8);
		byte *entries = *(byte **)(damage_info + 0xdc);

		for (long i = 0; i < count; i++)
		{
			if (*(long *)(entries + i * 0x14) == seat_key)
				return *(real *)(entries + i * 0x14 + 4) * definition->unknown44;
		}
	}
	return result;
}

/* damages an object with the globals' default damage (from +0x144), credited
   to an owner if one is given */
// @retail 0xdbfb0
void function_dbfb0(long object_index, s_damage_owner const *owner, bool a, bool b, bool c)
{
	if (TEST_FIELD_BIT(DAMAGE_OBJECT(object_index)->damage_flags.body_depleted))
		return;

	long definition_index = *(long *)(*(byte **)((byte *)g_4e034c + 0x144) + 0x14);
	if (definition_index == NONE)
		return;

	damage_data data;

	data.unknown7c = NONE;
	damage_data_new(&data, definition_index);
	data.unknown54 = 1.0f;
	if (owner)
		data.owner = *owner;

	dword flags = *(dword *)data.unknown04 | 4;
	if (a)
		flags |= 0x10;
	else
		flags &= ~0x10;
	if (b)
		flags |= 0x80;
	else
		flags &= ~0x80;
	if (c)
		flags |= 0x800;
	else
		flags &= ~0x800;
	*(dword *)data.unknown04 = flags;
	function_d7b80(&data, object_index, NONE, NONE, NONE, 0);
}

/* whether an object, or anything seated in or attached to it, is a unit (a
   player's unit, if players_only) */
// @retail 0xdb110
bool object_is_or_contains_player(long object_index, bool players_only, bool walk_siblings)
{
	s_damage_object_datum *datum = &((s_damage_object_datum *)g_4e0300->data)[object_index & 0xffff];
	s_damage_object *object = datum->object;
	bool result = false;

	if ((1 << datum->type) & 2)
	{
		s_object_child_iterator iterator;

		function_d0620(object_index, &iterator);
		while (function_d0690(&iterator))
		{
			if (iterator.child_short != NONE)
			{
				if (!players_only)
				{
					result = true;
					break;
				}

				s_damage_object *unit = (s_damage_object *)function_badc0(iterator.child_index, 3);
				if (unit && unit->player_index != NONE)
				{
					result = true;
					break;
				}
			}
		}
	}

	if (object->first_child_object_index != NONE)
	{
		if (result)
			result = true;
		else
			result = object_is_or_contains_player(object->first_child_object_index, players_only, true);
	}

	if (object->next_object_index != NONE && walk_siblings)
	{
		if (result)
			return true;
		return object_is_or_contains_player(object->next_object_index, players_only, true);
	}
	return result;
}

/* the globals' material table (+0x150 count, +0x154 elements of 0xb4 bytes) */
PRIVATE inline byte *global_material_get(short index)
{
	byte *result = 0;

	if (index != NONE && index >= 0 && index < *(long *)((byte *)g_4e034c + 0x150))
		result = *(byte **)((byte *)g_4e034c + 0x154) + index * 0xb4;
	return result;
}

/* the damage multiplier of a resistance against a source on a material: the
   resistance's scale (+0x1c), times the armor table's entry for the source's
   damage groups and the struck material's armor types */
// @retail 0xd9020
real function_d9020(long object_index, byte const *resistance, byte const *source, damage_data const *data)
{
	real result = *(real const *)(resistance + 0x1c);

	if (TEST_FIELD_BIT(DAMAGE_OBJECT(object_index)->damage_flags.unknown7))
		result = 0.0f;
	if ((**(byte const *const *)(resistance + 4) & 0x20) && !(source[4] & 0x20))
		result = 0.0f;

	short material = data->unknown7c;
	if (global_material_get(material))
	{
		byte *armor_a = global_material_get(material);
		byte *armor_b = global_material_get(material);

		result = damage_armor_table_lookup(*(string_id const *)(source + 0x40), *(string_id const *)(source + 0x44),
			*(string_id *)(armor_b + 0x10), *(string_id *)(armor_a + 0x14)) * result;
	}
	return result;
}

/* destroys a damage region: picks the permutations whose chance and body
   threshold allow it, and destroys each now or schedules it after its
   delay */
// @retail 0xdae60
void object_destroy_region(s_damage_info *info, long object_index, s_damage_owner const *owner, long region_index,
	s_damage_region_accumulator *accumulator)
{
	s_damage_object *object = DAMAGE_OBJECT(object_index);
	s_damage_info_region *region = &info->regions[region_index];
	s_object_region_state *state = (s_object_region_state *)((byte *)object + object->region_states_offset) + region_index;

	for (long i = 0; i < region->permutation_count; i++)
	{
		s_damage_info_permutation *permutation = &region->permutations[i];

		if (state->destroyed_permutations & (1 << i))
			continue;
		if (permutation->unknown08 != 0.0f)
			continue;
		if (permutation->skip_chance != 0.0f &&
			permutation->skip_chance > (real)random_next(&g_4e7408->unknown0) * (1.f / 65535.f))
			continue;
		if (1.0f > permutation->body_threshold && (permutation->flags & 0x200000))
			continue;
		if (permutation->body_threshold > 1.0f && !(permutation->flags & 0x200000))
			continue;

		dword exclusive = accumulator->flags & 0x400;
		if (exclusive && (permutation->flags & 0x1000000))
			continue;
		if (!exclusive && (permutation->flags & 0x800000))
			continue;

		if (permutation->delay > 0.0f)
		{
			if ((state->pending & 0xfff8) && (state->pending & 7) != i)
				function_da110(state->pending & 7, info, object_index, owner, region_index, accumulator);

			state->pending ^= (state->pending ^ i) & 7;

			real seconds = g_510c54->ticks_per_second * permutation->delay;
			long ticks;

			__asm
			{
				fld seconds
				fistp ticks
			}
			state->pending = (word)((state->pending & 7) | (ticks << 3));

			if (permutation->effect_index != NONE)
			{
				function_d9d60((permutation->flags >> 20) & 1, permutation->unknown38, object_index,
					permutation->effect_index, owner);
				function_a8360(object_index, region_index, i, true);
			}
		}
		else
		{
			function_da110(i, info, object_index, owner, region_index, accumulator);
		}
	}
	state->unknown02 = 0xff;
}

/* whether a permutation's model state allows it: its state index (+0x22)
   is out of range, or that state's level is below the permutation's (+0x20) */
// @retail 0xd9f70
bool function_d9f70(long region_index, long permutation_index, s_damage_info *info, long object_index)
{
	s_damage_info_permutation *permutation = &info->regions[region_index].permutations[permutation_index];
	byte *states;
	long state_count;
	long a;
	long b;
	bool result = true;

	function_ba690(object_index, &states, &state_count, &a, &b);
	short state_index = *(short *)((byte *)permutation + 0x22);
	if (state_index >= 0 && state_index < state_count &&
		(short)(char)states[state_index * 8 + 1] >= *(short *)((byte *)permutation + 0x20))
		return false;
	return result;
}
