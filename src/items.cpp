// @flags /O2 /arch:SSE /Gr
/* ITEMS.CPP: the item object type (weapons, equipment and garbage; its
   definition is at 0x467c08) */

#include "cseries.h"
#include "globals.h"
#include "object_markers.h"

/* the item (the object data) */
struct s_item
{
	long definition_index;
	byte unknown004[0x12c - 4];
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
};

struct s_item_header
{
	byte unknown00[8];
	s_item *item;
};

#define ITEM_GET(index) (((s_item_header *)g_4e0300->data)[(index) & 0xffff].item)

void function_b9b90(void *object, bool flag, long index);
real_point3d *function_b9dd0(long object_index, real_point3d *result);

// @retail 0x10c850
void function_10c850(long item_index)
{
	s_item *item = ITEM_GET(item_index);

	function_b9b90(0, false, item_index);
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

		function_b8d30(false, item->unit_index, 0x4000095, 1, &marker);
		*position = marker.matrix.position;
	}
	else
	{
		function_b9dd0(item_index, position);
	}
}
