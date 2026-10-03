// @flags /O2 /arch:SSE /Gr
/* DAMAGE.CPP: object damage

The functions follow damage.obj in Bungie's May 2003 builds
(halo-symbol-atlas); the profile build's order and sizes are closest to
retail. */

#include "cseries.h"
#include "globals.h"
#include "data_array.h"
#include "real_math.h"
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
	byte unknownab[0xc2 - 0xab];
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
		word unknown5 : 7;
		word unknown12 : 1;
		word unknown13 : 1;
	} damage_flags;
	byte unknown10c[0x12c - 0x10c];
	long unknown12c;
	byte unknown130[0x138 - 0x130];
	short team;
	byte unknown13a[2];
	long player_index;
	byte unknown140[0x1fc - 0x140];
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

/* a damage event (0x88 bytes) */
struct damage_data
{
	long definition_index;
	byte unknown04[4];
	long unknown08;
	long owner_object_index;
	short owner_team;
	byte unknown12[2];
	long unknown14;
	long unknown18;
	long unknown1c;
	short unknown20;
	short unknown22;
	real_point3d position;
	byte unknown30[0x54 - 0x30];
	real unknown54;
	real unknown58;
	real unknown5c;
	byte unknown60[0x7c - 0x60];
	short unknown7c;
	short unknown7e;
	byte unknown80[4];
	bool unknown84;
	byte unknown85[3];
};

short g_47d8e0 = NONE;

/* who is responsible for damage */
struct s_damage_owner
{
	long player_index;
	long object_index;
	short team;
};

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
struct s_damage_info_region
{
	byte unknown00[4];
	dword flags;
	byte unknown08[0x38 - 0x8];
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
	byte unknown00[4];
	real radius04;
	byte unknown08[4];
	dword flags0c;
	byte unknown10[4];
	dword flags14;
	byte unknown18[0x58 - 0x18];
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

enum
{
	k_maximum_area_of_effect_objects = 64
};

real __stdcall function_1e9700(long kind);
real function_259a0(dword *seed);
extern struct s_random_globals *g_4e7408;

bool function_d0690(s_object_child_iterator *iterator);
void function_d0620(s_object_child_iterator *iterator, long object_index);
void function_b7360(long object_index);
void __stdcall function_b8540(long a);
void function_b8b70(long object_index);
void __stdcall function_dae60(s_damage_info *info, long object_index, s_damage_owner const *owner, long region_index,
	s_damage_region_accumulator *accumulator);
void __stdcall function_dbfb0(s_damage_object *object, s_damage_owner const *owner, long a, long b, long c);
void __stdcall function_dbc80(long object_index, short a, short b);
void __stdcall function_e6460(long object_index);
void function_176780(long effect_index, long object_index, s_damage_owner const *owner, long a, long b, long c);
void __stdcall function_ba7f0(long object_index, long a, long b, long c);
short function_bb050(long a, dword type_mask, void const *location, real_point3d const *position, real radius,
	long *objects, short maximum_count);
void __stdcall area_of_effect_cause_damage_to_object(damage_data *data, long object_index, bool unknown);
void __stdcall function_184250(damage_data const *data);

long function_d5b60(long object_index);
real function_1e9720(long kind, short team);
bool function_108fd0(long object_index);
void function_b58c0(long index, dword mask);

typedef long (__stdcall *t_bsearch_compare_function)(const void *, const void *, const void *);
long bsearch_elements(const void *key, const void *base, long count, long element_size, t_bsearch_compare_function compare, const void *context);
long __stdcall function_122cf0(const void *a, const void *b, const void *context);

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
			sizeof(s_damage_group), function_122cf0, NULL);

		if (group_index == NONE)
			continue;

		s_damage_group *group = &table->damage_groups.elements[group_index];
		for (dword j = 0; j < 2; j++)
		{
			string_id armor_key = armors[j];
			long modifier_index = bsearch_elements(&armor_key, group->armor_modifiers.elements, group->armor_modifiers.count,
				sizeof(s_armor_modifier), function_122cf0, NULL);

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
	data->owner_object_index = NONE;
	data->unknown08 = NONE;
	data->owner_team = NONE;
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
			function_176780(*(long *)(damage_info + 0xb0), object_index, g_467420, 0, 0, 0);
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
					function_dae60(info, object_index, owner, i, &accumulator);
			}
		}
	}

	if (g_4e6948->mode != 4 && object->type == 1)
	{
		for (long child_index = object->first_child_object_index; child_index != NONE;)
		{
			s_damage_object *child = DAMAGE_OBJECT(child_index);

			if (child->type == 0 && child->unknown1fc != NONE)
				function_dbfb0(child, owner, 0, 0, 0);
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

			function_d0620(&iterator, object->parent_object_index);
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
				function_dae60(info, object_index, g_467420, i, &accumulator);
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
	if ((flags & 1) && object_index == data->owner_object_index)
		return result;

	if ((1 << object->type) & 3)
	{
		if ((flags & 0x8000) && g_4e6948->state == 1 && object->player_index != NONE)
			return result;
		if ((flags & 8) && !game_team_is_enemy(object->team, data->owner_team))
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
			object_index != data->owner_object_index)
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
		for (long player_index = data_next_absolute_index_inlined(g_4e8c24, 0); player_index != NONE;
			player_index = data_next_absolute_index_inlined(g_4e8c24, player_index + 1))
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
