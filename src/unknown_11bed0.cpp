// @flags /O2 /Gr
/* UNKNOWN_11BED0.CPP: the location (leaf and cluster of the structure bsp) of
   a point. Decompiled by lane F for 0x18c3b0. */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include "object_queries.h"

struct s_bsp3d;
long function_14a280(s_bsp3d *bsp, real_point3d *point, long index);

/* the structure bsp's collision bsp */
s_bsp3d *g_4e033c;

struct s_structure_leaf
{
	short cluster_index;
	byte unknown02[6];
};

struct s_structure_bsp_leaves_view
{
	byte unknown00[0x30];
	s_structure_leaf *leaves;
};

// @retail 0x11bed0
void function_11bed0(real_point3d const *point, s_location *location)
{
	if (g_4686c4 == NONE)
	{
		location->cluster_index = g_4686c4;
		location->bsp_index = g_4686c4;
		location->leaf_index = NONE;
	}
	else
	{
		long leaf_index = function_14a280(g_4e033c, (real_point3d *)point, 0);
		location->leaf_index = leaf_index;
		long cluster_index = leaf_index != NONE ? ((s_structure_bsp_leaves_view *)g_4e0348)->leaves[leaf_index].cluster_index : NONE;
		location->bsp_index = g_4686c4;
		location->cluster_index = cluster_index;
	}
}
