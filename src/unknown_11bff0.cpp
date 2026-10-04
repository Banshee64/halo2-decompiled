// @flags /O2 /Ob1 /Gr
/* UNKNOWN_11BFF0.CPP: the cluster of a leaf of the structure bsp (the file of
   unknown_11bed0.cpp; its own /Ob1 file because retail calls it out of line
   from the camera code) */

#include "cseries.h"
#include "globals.h"

struct s_11bff0_leaf
{
	short cluster_index;
	byte unknown02[6];
};

struct s_11bff0_leaves_view
{
	byte unknown00[0x30];
	s_11bff0_leaf *leaves;
};

// @retail 0x11bff0
long structure_leaf_cluster_get(long leaf_index)
{
	long result;

	if (leaf_index != NONE)
	{
		result = ((s_11bff0_leaves_view *)g_4e0348)->leaves[leaf_index].cluster_index;
	}
	else
	{
		result = NONE;
	}
	return result;
}
