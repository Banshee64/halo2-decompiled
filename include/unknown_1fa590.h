/* UNKNOWN_1FA590.H: the pathfinding data of the structure bsp (g_4e0348
   +0xc4) and the lookups of src/unknown_1fa590.cpp on it */

#ifndef UNKNOWN_1FA590_H
#define UNKNOWN_1FA590_H

#include "cseries.h"
#include "real_math.h"

/* a node: flags in the first word, then the first of its surfaces (8 bytes) */
struct s_pathfinding_node
{
	word flags;
	short first_surface;
	union
	{
		byte unknown4[4];
		long first_edge;
	};
};

/* a surface of a node, chained by next (0x14 bytes) */
struct s_pathfinding_surface
{
	short type;
	short next;
	long index;
	long object_index;
	byte unknown0c[0x14 - 0xc];
};

struct s_pathfinding_edge;

struct s_pathfinding_data
{
	long node_count;
	s_pathfinding_node *nodes;
	byte unknown08[4];
	s_pathfinding_edge *edges;
	byte unknown10[0x2c - 0x10];
	real_point3d *vertices;
	byte unknown30[0x38 - 0x30];
	long surface_count;
	s_pathfinding_surface *surfaces;
};

bool function_1fa6b0(s_pathfinding_node const *node, s_pathfinding_data const *pathfinding);

#endif
