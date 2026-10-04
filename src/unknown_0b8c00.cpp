// @flags /O2 /Gr
/* UNKNOWN_0B8C00.CPP: an object's node matrices and their count (objects.cpp
   in the original). Decompiled by lane T for the first person weapons. */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"

/* the object, as read here: the size and offset of its node matrices */
struct s_0b8c00_object
{
	byte unknown000[0x114];
	short node_matrices_size;
	short node_matrices_offset;
};

struct s_0b8c00_object_header
{
	byte unknown00[8];
	s_0b8c00_object *object;
};

// @retail 0xb8c00
transform4x3f *function_b8c00(long object_index, long *node_count)
{
	s_0b8c00_object *object = ((s_0b8c00_object_header *)g_4e0300->data)[object_index & 0xffff].object;

	*node_count = object->node_matrices_size / sizeof(transform4x3f);
	return (transform4x3f *)((byte *)object + object->node_matrices_offset);
}
