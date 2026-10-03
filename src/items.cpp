// @flags /O2 /arch:SSE /Gr
/* ITEMS.CPP: the item object type (weapons, equipment and garbage; its
   definition is at 0x467c08) */

#include "cseries.h"
#include "globals.h"

struct s_object_marker
{
	short node_index;
	short unknown02;
	real_matrix4x3 node_matrix;
	real_matrix4x3 matrix;
	real unknown6c;
};

/* the item (the object data) */
struct s_item
{
	long definition_index;
	byte unknown004[0x94 - 4];
	real_vector3d angular_velocity;
	byte unknown0a0[0xc1 - 0xa0];
	byte flags_c1;
	byte unknown0c2[0x12c - 0xc2];
	word flag0 : 1;
	word flag1 : 1;
	word flag2 : 1;
	word flag3 : 1;
	word flag4 : 1;
	word flag5 : 1;
	word flag6 : 1;
	word flag7 : 1;
	word : 8;
	byte unknown12e[0x13a - 0x12e];
	byte value_13a;
	byte unknown13b[0x154 - 0x13b];
	long unit_index;
	real_vector3d spin_axis;
	real spin_sine;
	real spin_cosine;
};

struct s_item_header
{
	byte unknown00[8];
	s_item *item;
};

#define ITEM_GET(index) (((s_item_header *)g_4e0300->data)[(index) & 0xffff].item)

void function_b9b90(long object_index, bool disable);
short function_b8d30(long object_index, long marker_name, s_object_marker *markers, short count, bool flag);
real_point3d *function_b9dd0(long object_index, real_point3d *result);

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
void function_10da60(long item_index, real_point3d *position)
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
	real_vector3d axis = item->angular_velocity;
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
