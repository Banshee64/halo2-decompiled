/* UNKNOWN_1FA590.H: the pathfinding data of the structure bsp (g_4e0348
   +0xc4) and the lookups of src/unknown_1fa590.cpp on it */

#ifndef UNKNOWN_1FA590_H
#define UNKNOWN_1FA590_H

#include "cseries.h"

/* a node: flags in the first word, then the first of its surfaces (8 bytes) */
struct s_pathfinding_node
{
	word flags;
	short first_surface;
	byte unknown4[4];
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

struct s_pathfinding_data
{
	byte unknown00[4];
	s_pathfinding_node *nodes;
	byte unknown08[0x3c - 0x8];
	s_pathfinding_surface *surfaces;
};

bool function_1fa6b0(s_pathfinding_node const *node, s_pathfinding_data const *pathfinding);

#endif
