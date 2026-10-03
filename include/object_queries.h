/* OBJECT_QUERIES.H: object queries decompiled by lane F (unknown_0b8bd0.cpp) */
#ifndef OBJECT_QUERIES_H
#define OBJECT_QUERIES_H

#include "cseries.h"
#include "real_math.h"

/* where an object is: a leaf and a cluster of the structure bsp */
struct s_location
{
	long leaf_index;
	short cluster_index;
	short bsp_index;
};

real_matrix4x3 *object_get_node_matrix(long object_index, short node_index);
bool object_or_parent_hidden(long object_index);
void object_get_root_location(long object_index, s_location *location);

#endif
