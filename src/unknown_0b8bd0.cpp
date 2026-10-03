// @flags /O2 /Ob1 /Gr
/* UNKNOWN_0B8BD0.CPP: object queries: a node matrix, whether an object or one
   of its parents is hidden, the location of an object's ultimate parent.
   Decompiled by lane F: 0x18c3b0 calls them with register arguments. */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include "object_queries.h"

struct s_object_query_view
{
	byte unknown00[4];
	byte flags;
	byte unknown05[0x14 - 0x5];
	long parent_index;
	byte unknown18[0x28 - 0x18];
	s_location location;
	byte unknown30[0x116 - 0x30];
	short node_matrices_offset;
};

struct s_object_query_header
{
	byte unknown00[2];
	byte flags;
	byte type;
	byte unknown04[4];
	s_object_query_view *object;
};

static inline s_object_query_header *object_query_header(long object_index)
{
	return (s_object_query_header *)g_4e0300->data + (object_index & 0xffff);
}

// @retail 0xb8bd0
real_matrix4x3 *object_get_node_matrix(long object_index, short node_index)
{
	s_object_query_view *object = object_query_header(object_index)->object;
	return (real_matrix4x3 *)((byte *)object + object->node_matrices_offset) + node_index;
}

// @retail 0xb9ce0
bool object_or_parent_hidden(long object_index)
{
	do
	{
		s_object_query_header *header = object_query_header(object_index);
		s_object_query_view *object = header->object;
		if ((header->flags & 0x10) || (object->flags & 1))
		{
			return true;
		}
		object_index = object->parent_index;
	} while (object_index != NONE);

	return false;
}

// @retail 0xba300
void object_get_root_location(long object_index, s_location *location)
{
	long root_index = NONE;

	while (object_index != NONE)
	{
		root_index = object_index;
		object_index = object_query_header(object_index)->object->parent_index;
	}
	*location = object_query_header(root_index)->object->location;
}
