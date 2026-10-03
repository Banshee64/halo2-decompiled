#include "cseries.h"
#include "globals.h"
#include "slot_handler.h"

// @flags /O2 /Gr

/* an actor slot-owner (g_4f55f0, 0x888 bytes each) names a tag at +0x54; the
   tag has a block of 0xb4 byte elements (count at +0xe4, elements at +0xe8)
   and the index of the tag it inherits from at +8 */
struct s_owner_view
{
	byte unknown00[0x54];
	long tag_index;
	byte unknown58[0x888 - 0x58];
};

struct s_tag_with_elements
{
	byte unknown00[8];
	long parent_index;
	byte unknown0c[0xe4 - 0xc];
	long element_count;
	s_tag_element *elements;
};

// @retail 0x1e5450
s_tag_element *function_1e5450(long owner_index, long key)
{
	long tag_index = ((s_owner_view *)g_4f55f0->data)[owner_index & 0xffff].tag_index;
	s_tag_element *element;

	while (tag_index != NONE)
	{
		s_tag_with_elements *tag = (s_tag_with_elements *)g_4e3b44[tag_index & 0xffff].bytes;
		short i = 0;

		while (i < tag->element_count)
		{
			element = &tag->elements[i];
			if (element->key == key)
				return element;
			i++;
		}

		tag_index = tag->parent_index;
	}

	return 0;
}
