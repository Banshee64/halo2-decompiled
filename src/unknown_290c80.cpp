// @flags /O2 /Gr
/* UNKNOWN_290C80.CPP: the ai's iterator over a chain of objects (an outside
   function lane I's handlers call) */

#include "cseries.h"
#include "unknown_2551c0.h"

/* the ai data of an object, at the object's ai_offset */
struct s_object_ai_data
{
	byte unknown00[0xc];
	long next_object_index;
};

// @retail 0x290c80
s_handler_object_view *function_290c80(s_ai_object_iterator *iterator)
{
	s_handler_object_view *object = NULL;

	if (iterator->next_index != NONE)
	{
		object = handler_object_get(iterator->next_index);
		iterator->index = iterator->next_index;

		s_object_ai_data *data;
		if (!object->flags134 && (data = (s_object_ai_data *)((byte *)object + object->ai_offset)) != NULL)
		{
			iterator->next_index = data->next_object_index;
		}
		else
		{
			iterator->next_index = NONE;
		}
	}

	return object;
}
