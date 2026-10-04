// @flags /O2 /Gr
/* UNKNOWN_11B040.CPP: a new object list of the units attached to an object
   (objects.cpp; an outside function lane A's script function 0x2a4230 needs) */

#include "cseries.h"
#include "globals.h"

struct s_object_11b040
{
	byte unknown00[0xc];
	long next_object_index;
	long first_object_index;
	byte unknown14[0xaa - 0x14];
	byte object_type;
	byte unknownab[0x1fc - 0xab];
	short unknown1fc;
};

struct s_object_header_11b040
{
	byte unknown00[8];
	s_object_11b040 *object;
};

long object_list_new(void);
void object_list_add(long list_index, long object_index);

// @retail 0x11b040
long function_11b040(long object_index)
{
	long list_index = NONE;

	if (object_index != NONE)
	{
		s_data_array *objects = g_4e0300;
		s_object_11b040 *object = ((s_object_header_11b040 *)objects->data)[object_index & 0xffff].object;

		list_index = object_list_new();
		if (list_index != NONE)
		{
			long child_index = object->first_object_index;
			while (child_index != NONE)
			{
				s_object_11b040 *child = ((s_object_header_11b040 *)objects->data)[child_index & 0xffff].object;
				if ((1 << child->object_type) & 3 && child->unknown1fc != NONE)
					object_list_add(list_index, child_index);
				child_index = child->next_object_index;
			}
		}
	}
	return list_index;
}
