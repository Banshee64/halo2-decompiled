// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1FA590.CPP: lookups in the structure's 2d trees and surfaces, and
   the surface a contact lies on */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include <math.h>

/* a node of a 2d tree: a line and the two children (negative: a leaf) */
struct s_tree2d_node
{
	real i;
	real j;
	real d;
	long front;
	long back;
};

struct s_tree2d
{
	byte unknown00[0x10];
	long leaf_count;
	long *leaves;
	long node_count;
	s_tree2d_node *nodes;
};

/* the leaf of the tree the point is in, or NONE */
// @retail 0x1fa590
long function_1fa590(long index, s_tree2d const *tree, real_point2d const *point)
{
	while (index != NONE)
	{
		s_tree2d_node *node;

		if (index >= 0)
			return index;

		index &= 0x7fffffff;
		if (index < 0 || index >= tree->node_count)
			break;

		node = &tree->nodes[index];
		if (node->i * point->x + node->j * point->y - node->d > 0.0)
			index = node->front;
		else
			index = node->back;
	}

	return NONE;
}

// @retail 0x1fa5e0
long *function_1fa5e0(long index, s_tree2d const *tree, long unknown)
{
	long *result = 0;

	if (unknown == NONE && index >= 0 && index < tree->leaf_count)
		result = &tree->leaves[index];
	return result;
}

/* the unit vector of the first axis direction set in the flags */
// @retail 0x1fa600
void function_1fa600(real_vector3d *vector, word flags)
{
	if (flags & 0x10)
	{
		vector->i = 0.0f;
		vector->j = 0.0f;
		vector->k = 1.0f;
	}
	else if (flags & 0x20)
	{
		vector->i = 0.0f;
		vector->j = 0.0f;
		vector->k = -1.0f;
	}
	else if (flags & 0x40)
	{
		vector->i = 0.0f;
		vector->j = 1.0f;
		vector->k = 0.0f;
	}
	else if (flags & 0x80)
	{
		vector->i = 0.0f;
		vector->j = -1.0f;
		vector->k = 0.0f;
	}
	else if (flags & 0x100)
	{
		vector->i = 1.0f;
		vector->j = 0.0f;
		vector->k = 0.0f;
	}
	else if (flags & 0x200)
	{
		vector->i = -1.0f;
		vector->j = 0.0f;
		vector->k = 0.0f;
	}
}

/* the structure's surfaces as function_1fa6b0 reads them */
struct s_surface_record
{
	short type;
	short next;
	long index;
	long object_index;
	byte unknown0c[0x14 - 0xc];
};

struct s_surface_owner
{
	short unknown0;
	short first;
};

struct s_surface_set
{
	byte unknown00[0x3c];
	s_surface_record *records;
};

struct s_surface_flags
{
	byte unknown0[4];
	byte flags;
	byte bit;
	byte unknown6[2];
};

struct s_structure_view
{
	byte unknown00[0x2c];
	s_surface_flags *flags;
};

struct s_4e0348_view
{
	byte unknown00[0x18];
	s_structure_view *structure;
	byte unknown1c[0xc4 - 0x1c];
	long count;
	long first;
};

byte *function_183fc0(long index);

// @retail 0x1fa6b0
bool function_1fa6b0(s_surface_owner const *owner, s_surface_set const *set)
{
	bool result = false;
	long index = owner->first;

	while (index != NONE)
	{
		s_surface_record *record = &set->records[index];

		if (record->type == 7)
		{
			s_surface_flags *flags = &((s_4e0348_view *)g_4e0348)->structure->flags[record->index];

			if (flags->flags & 8)
			{
				long bit = flags->bit;
				dword *bits = (dword *)function_183fc0(record->object_index);

				result = (bits[bit >> 5] & (1 << (bit & 31))) == 0;
			}
			return result;
		}
		index = record->next;
	}

	return result;
}

// @retail 0x1fa7f0
long function_1fa7f0(void)
{
	s_4e0348_view *globals = (s_4e0348_view *)g_4e0348;
	long result = 0;

	if (globals->count > 0)
		result = globals->first;
	return result;
}

/* the contact at a surface and its orientation (2..5 by the larger of i and j) */
struct s_contact
{
	byte unknown00[0xdc];
	real_vector3d normal;
};

struct s_contact_result
{
	long unknown0;
	long orientation;
};

// @retail 0x1faeb0
void function_1faeb0(s_contact const *contact, s_contact_result *result)
{
	result->orientation = 1;
	if (fabs(contact->normal.i) < fabs(contact->normal.j))
	{
		if (contact->normal.j < -0.0001f)
			result->orientation = 2;
		else if (contact->normal.j > 0.0001f)
			result->orientation = 3;
	}
	else
	{
		if (contact->normal.i < -0.0001f)
			result->orientation = 5;
		else if (contact->normal.i > 0.0001f)
			result->orientation = 4;
	}
}

/* a timer of 1.5 seconds and a zero vector */
struct s_1faf30_timer
{
	long ticks;
	real_vector3d vector;
};

// @retail 0x1faf30
void function_1faf30(s_1faf30_timer *timer)
{
	real seconds = (real)g_510c54->ticks_per_second * 1.5f;
	long ticks;

	__asm
	{
		fld seconds
		fistp ticks
	}
	timer->ticks = ticks;
	timer->vector = *g_4687a4;
}
