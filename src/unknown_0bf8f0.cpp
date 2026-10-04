// @flags /O2 /Gr
/* UNKNOWN_0BF8F0.CPP: the objects below an object in the attachment tree,
   collected into a new object list (objects.cpp; outside functions lane A's
   script functions need) */

#include "cseries.h"
#include "globals.h"

/* an object's place in the attachment tree */
struct s_object_tree_view
{
	long definition_index;
	byte unknown04[0xc - 4];
	long next_object_index;
	long first_object_index;
};

struct s_object_tree_header_view
{
	byte unknown00[8];
	s_object_tree_view *object;
};

long function_1ded60(void);
void function_1dedb0(long list_index, long object_index);

static inline s_object_tree_view *object_tree_get(long object_index)
{
	return ((s_object_tree_header_view *)g_4e0300->data)[object_index & 0xffff].object;
}

/* adds an object, its next objects and everything below them (of a definition,
   or of any for NONE) to an object list */
// @retail 0xbf8f0
void __stdcall function_bf8f0(long object_index, long definition_index, long list_index)
{
	do
	{
		s_object_tree_view *object = object_tree_get(object_index);
		if (object->definition_index == definition_index || definition_index == NONE)
			function_1dedb0(list_index, object_index);
		if (object->next_object_index != NONE)
			function_bf8f0(object->next_object_index, definition_index, list_index);
		object_index = object->first_object_index;
	} while (object_index != NONE);
}

/* a new object list of the objects (of a definition) below an object */
// @retail 0xbf950
long function_bf950(long object_index, long definition_index)
{
	long list_index = NONE;

	if (object_index != NONE)
	{
		list_index = function_1ded60();
		if (list_index != NONE)
		{
			long first_object_index = object_tree_get(object_index)->first_object_index;
			if (first_object_index != NONE)
				function_bf8f0(first_object_index, definition_index, list_index);
		}
	}
	return list_index;
}
