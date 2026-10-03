// @flags /O2 /arch:SSE /Gr
/* DAMAGE.CPP: object damage

The functions follow damage.obj in Bungie's May 2003 builds
(halo-symbol-atlas); the profile build's order and sizes are closest to
retail. */

#include "cseries.h"
#include "globals.h"
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
	byte unknown14[0xaa - 0x14];
	byte type;
	byte unknownab[0xd4 - 0xab];
	long unknownd4;
	byte unknownd8[0xe4 - 0xd8];
	real maximum_body_vitality;
	real maximum_shield_vitality;
	real body_vitality;
	real shield_vitality;
	byte unknownf4[0x104 - 0xf4];
	short shield_stun_ticks;
	byte unknown106[4];
	struct
	{
		word unknown0 : 2;
		word cannot_restore_body : 1;
		word unknown3 : 1;
		word shield_double_charged : 1;
	} damage_flags;
	byte unknown10c[0x138 - 0x10c];
	short team;
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
	long unknown0c;
	short unknown10;
	byte unknown12[2];
	long unknown14;
	long unknown18;
	long unknown1c;
	short unknown20;
	short unknown22;
	byte unknown24[0x54 - 0x24];
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
	data->unknown0c = NONE;
	data->unknown08 = NONE;
	data->unknown10 = NONE;
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

	if (!TEST_FIELD_BIT(object->damage_flags.cannot_restore_body) && object->body_vitality < 1.0f)
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
