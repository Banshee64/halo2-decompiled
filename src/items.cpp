// @flags /O2 /arch:SSE /Gr
/* ITEMS.CPP: the item object type (weapons, equipment and garbage; its
   definition is at 0x467c08) */

#include "unknown_11c920.h"
#include "globals.h"
#include "object_markers.h"
#include "object_iterator.h"

/* the item (the object data) */
struct s_item
{
	long definition_index;
	dword object_flags;
	byte unknown008[0x14 - 8];
	long parent_index;
	byte unknown018[0x64 - 0x18];
	point3f position;
	byte unknown070[0x94 - 0x70];
	vector3f angular_velocity;
	byte unknown0a0[0xc1 - 0xa0];
	byte flags_c1;
	short location_c2;
	long location_c4;
	long location_c8;
	byte unknown0cc[0x12c - 0xcc];
	union
	{
		byte flags_12c;
		struct
		{
			word flag0 : 1;
			word flag1 : 1;
			word flag2 : 1;
			word flag3 : 1;
			word flag4 : 1;
			word flag5 : 1;
			word flag6 : 1;
			word flag7 : 1;
			word : 8;
		};
	};
	short value_12e;
	short bsp_index;
	short surface_index;
	short material_index;
	byte value_136;
	byte value_137;
	byte unknown138[0x13a - 0x138];
	byte value_13a;
	byte unknown13b[0x14c - 0x13b];
	long ignore_object_index;
	long creation_time;
	long unit_index;
	vector3f spin_axis;
	real spin_sine;
	real spin_cosine;
	word flag16c_0 : 1;
	word flag16c_1 : 1;
	word flag16c_2 : 1;
	word flag16c_3 : 1;
	word flag16c_4 : 1;
	word flag16c_5 : 1;
	word flag16c_6 : 1;
	word flag16c_7 : 1;
	word : 8;
};

struct s_item_header
{
	byte unknown00[3];
	byte type;
	byte unknown04[4];
	s_item *item;
};

/* the unit holding an item (a view of the unit) */
struct s_item_unit
{
	byte unknown000[0x13c];
	long player_index;
};

#define ITEM_GET(index) (((s_item_header *)g_4e0300->data)[(index) & 0xffff].item)

void function_b9b90(long object_index, bool disable);
point3f *function_b9dd0(long object_index, point3f *result);

// @retail 0x10c850
void function_10c850(long item_index)
{
	s_item *item = ITEM_GET(item_index);

	function_b9b90(item_index, false);
	item->value_13a = 0;
	item->flag5 = false;
}

// @retail 0x10cf50
bool function_10cf50(long item_index)
{
	s_item *item = ITEM_GET(item_index);
	bool result = false;

	if (TEST_FIELD_BIT(item->flag0) && !TEST_FIELD_BIT(item->flag1))
		result = true;
	return result;
}

// @retail 0x10da60
void function_10da60(long item_index, point3f *position)
{
	s_item *item = ITEM_GET(item_index);

	if (TEST_FIELD_BIT(item->flag0))
	{
		s_object_marker marker;

		function_b8d30(item->unit_index, 0x4000095, &marker, 1, false);
		*position = marker.matrix.position;
	}
	else
	{
		function_b9dd0(item_index, position);
	}
}

// @retail 0x10d5f0
void function_10d5f0(long item_index)
{
	s_item *item = ITEM_GET(item_index);
	vector3f axis = item->angular_velocity;
	real length = (real)sqrt(axis.i * axis.i + axis.j * axis.j + axis.k * axis.k);

	if (fabs(length) < 0.0001f)
	{
		length = 0.0f;
	}
	else
	{
		real inverse = 1.0f / length;
		axis.i = inverse * axis.i;
		axis.j = inverse * axis.j;
		axis.k = inverse * axis.k;
	}

	if (length > 0.001f && !(item->flags_c1 & 1))
	{
		real angle = length * g_510c54->rate;

		item->flag4 = true;
		item->spin_axis = axis;
		item->spin_sine = (real)sin(angle);
		item->spin_cosine = (real)cos(angle);
	}
	else
	{
		item->flag4 = false;
		item->spin_sine = 0.0f;
		item->spin_cosine = 1.0f;
	}
}

/* a collision result of function_1697c0 (as items read it) */
struct s_collision_result_1697c0
{
	long type;
	byte unknown04[0x24 - 4];
	short unknown24;
	byte unknown26[0x3c - 0x26];
	short surface_index;
	byte unknown3e[0x50 - 0x3e];
	short material_index;
	byte unknown52[0x58 - 0x52];
	byte value_58;
	byte value_59;
	byte unknown5a[2];
};

bool __stdcall function_1697c0(long flags, point3f const *point, vector3f const *vector,
	long ignore_object_index, long ignore_unit_index, s_collision_result_1697c0 *result);
extern vector3f *g_4687bc;

// @retail 0x10b190
void __stdcall function_10b190(long item_index)
{
	s_item *item = ITEM_GET(item_index);

	if (item->parent_index == NONE && (item->flags_c1 & 1) && TEST_FIELD_BIT(item->flag5) && item->bsp_index != g_4686c4)
	{
		vector3f vector;
		s_collision_result_1697c0 collision;

		vector.i = g_4687bc->i * 0.1f;
		vector.j = g_4687bc->j * 0.1f;
		vector.k = g_4687bc->k * 0.1f;
		collision.unknown24 = NONE;
		if (function_1697c0(0x24909c0d, &item->position, &vector, item->ignore_object_index, NONE, &collision) &&
			(collision.type == 1 || collision.type == 3))
		{
			item->bsp_index = g_4686c4;
			item->surface_index = collision.surface_index;
			item->material_index = collision.material_index;
			item->value_136 = collision.value_58;
			item->value_137 = collision.value_59;
		}
		else
		{
			function_10c850(item_index);
		}
	}
}

void function_10dad0(long item_index);

// @retail 0x10b2c0
bool __stdcall function_10b2c0(long item_index, long a, long b)
{
	s_item *item = ITEM_GET(item_index);

	item->object_flags |= 0x1000;
	item->spin_sine = 0.0f;
	item->spin_cosine = 1.0f;
	item->ignore_object_index = NONE;
	item->unit_index = NONE;
	item->spin_axis = *g_4687b0;
	ITEM_GET(item_index)->creation_time = g_510c54->game_time;
	function_10dad0(item_index);
	return true;
}

/* the item object type definition */
struct s_item_type_definition
{
	char const *name;
	dword group_tag;
	short datum_size;
	short unknown0a;
	short unknown0c;
	short unknown0e;
	void *unknown10[4];
	void (__stdcall *handler20)(long);
	void *unknown24[2];
	bool (__stdcall *handler2c)(long, long, long);
};

s_item_type_definition g_467c08 =
{
	"item",
	'item',
	0x16c,
	NONE,
	NONE,
	NONE,
	{ 0, 0, 0, 0 },
	function_10b190,
	{ 0, 0 },
	function_10b2c0
};

void function_b7680(long object_index, real scale, real seconds);

struct s_item_definition
{
	byte unknown000[0xc4];
	real scale_multiplayer;
	real scale;
	byte unknown0cc[0x114 - 0xcc];
	real delay_lower;
	real delay_upper;
	byte unknown11c[4];
	long effect_tag_index;
};

// @retail 0x10dad0
void function_10dad0(long item_index)
{
	s_item *item = ITEM_GET(item_index);
	real scale = 1.0f;

	if (!TEST_FIELD_BIT(item->flag0))
	{
		s_item_definition *definition = (s_item_definition *)g_4e3b44[item->definition_index & 0xffff].bytes;
		real value = g_4e6948->state == 2 ? definition->scale_multiplayer : definition->scale;

		if (value > 0.0f)
			scale = value < 0.5f ? 0.5f : (value > 3.0f ? 3.0f : value);
	}
	function_b7680(item_index, scale, 0.0f);
}

/* starts an iteration over the items (weapons, equipment and garbage) */
PRIVATE inline void item_iterator_new(s_type_f1af8e *iterator)
{
	iterator->signature = 0x86868686;
	iterator->type_mask = 0x1c;
	iterator->flags = 1;
	iterator->index = 0;
	iterator->object_index = NONE;
}

// @retail 0x10ca00
bool __stdcall function_10ca00(long *item_index)
{
	struct
	{
		s_item *item;
		s_type_f1af8e iterator;
	} iteration;

	bool result = false;

	item_iterator_new(&iteration.iterator);
	while ((iteration.item = (s_item *)function_baeb0(&iteration.iterator)) != NULL)
	{
		if (iteration.item->value_12e > 0)
		{
			*item_index = iteration.iterator.object_index;
			result = true;
			break;
		}
	}
	return result;
}

void function_15e300(long object_index);

// @retail 0x10ccc0
void function_10ccc0(long item_index)
{
	s_item *item = ITEM_GET(item_index);
	bool held_by_player = false;

	if (item->flags_12c & 1)
		held_by_player = ((s_item_unit *)ITEM_GET(item->unit_index))->player_index != NONE;
	if (held_by_player)
		item->flags_12c |= 8;
	else
		item->flags_12c &= ~8;
	if (((1 << ((s_item_header *)g_4e0300->data)[item_index & 0xffff].type) & 4) && TEST_FIELD_BIT(item->flag16c_6))
		function_15e300(item_index);
}

#include "effects.h"

/* function_259d0 (0x259d0) on the first seed of g_4e7408, inlined */
inline real item_real_random_range(real lower, real upper)
{
	dword *seed = &g_4e7408->unknown0;
	*seed = *seed * 0x19660d + 0x3c6ef35f;
	return lower + (upper - lower) * ((real)(*seed >> 16) * (1.0f / 65535.0f));
}

// @retail 0x10d4e0
void function_10d4e0(long item_index)
{
	s_item *item = ITEM_GET(item_index);
	s_item_definition *definition = (s_item_definition *)g_4e3b44[item->definition_index & 0xffff].bytes;

	if (item->value_12e == 0)
	{
		real delay = item_real_random_range(definition->delay_lower, definition->delay_upper);
		s_item *owner_item = ITEM_GET(item_index);
		s_effect_owner owner;
		long ticks;

		owner.unknown4 = owner_item->location_c8;
		owner.unknown0 = owner_item->location_c4;
		owner.unknown8 = owner_item->location_c2;
		function_176780(item_index, &owner, 0.0f, definition->effect_tag_index, 0.0f, NULL, NULL);
		delay = (real)g_510c54->field_2_3 * delay;
		__asm
		{
			fld delay
			fistp ticks
		}
		item->value_12e = (short)ticks;
	}
}

void function_b9a90(long object_index);
void __stdcall function_bef30(long object_index, long a, long b, long c, long d);
void function_b8b70(long object_index);
void function_b7300(long object_index);
void __stdcall function_b87b0(long object_index);
void function_b7290(long object_index);
bool function_b9d20(long object_index);

/* puts an item in an inventory: detaches it and takes it out of the world */
// @retail 0x10cd50
void function_10cd50(long item_index)
{
	s_item *item = ITEM_GET(item_index);

	if (item->parent_index != NONE)
		function_b9a90(item_index);
	item->flags_12c |= 2;
	item = ITEM_GET(item_index);
	if (!(item->object_flags & 1))
	{
		if (function_b9d20(item_index))
			function_bef30(item_index, 1, 0, 0, 0);
		item->object_flags |= 1;
		function_b8b70(item_index);
	}
	item = ITEM_GET(item_index);
	function_b7300(item_index);
	if ((item->object_flags >> 8) & 1)
		function_b87b0(item_index);
	item->object_flags |= 0x80;
	function_b7290(item_index);
}