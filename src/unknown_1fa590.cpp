// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1FA590.CPP: lookups in the structure's 2d trees and surfaces, and
   the surface a contact lies on */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_0259d0.h"
#include "unknown_1fa590.h"
#include <math.h>

void function_11d180(vector3f *left, vector3f const *in, vector3f *out, vector3f const *up, vector3f *forward);

struct s_motion_adjustment
{
	byte unknown00[0x20];
	real scale;
	byte unknown24[0xf4 - 0x24];
	vector3f forward;
	byte unknown100[0x124 - 0x100];
	vector3f current;
	byte unknown130[8];
	vector3f target;
	real rate;
};

struct s_motion_adjustment_result
{
	byte unknown00[0xc];
	vector3f velocity;
};

// @retail 0x1fa810
void __stdcall function_1fa810(long *ticks, s_motion_adjustment_result *out, s_motion_adjustment const *state)
{
	vector3f left;
	vector3f up;
	vector3f forward;
	function_11d180(&left, &state->forward, &up, g_4687b0, &forward);
	vector3f target;
	target.i = state->target.i * state->scale;
	target.j = state->target.j * state->scale;
	target.k = state->target.k * state->scale;
	vector3f offset;
	offset.i = state->forward.i * target.i + left.i * target.j + up.i * target.k - state->current.i;
	offset.j = state->forward.j * target.i + left.j * target.j + up.j * target.k - state->current.j;
	offset.k = state->forward.k * target.i + left.k * target.j + up.k * target.k - state->current.k;
	vector3f normalized = offset;
	real length = (real)sqrt(normalized.i * normalized.i + (normalized.j * normalized.j + normalized.k * normalized.k));
	if (!(fabs(length) < 0.0001f))
	{
		real inverse = 1.0f / length;
		normalized.i *= inverse;
		normalized.j *= inverse;
		normalized.k *= inverse;
	}
	else
		length = 0.0f;
	real step = g_510c54->rate * state->rate;
	if (length > step)
	{
		offset.i = normalized.i * step;
		offset.j = normalized.j * step;
		offset.k = normalized.k * step;
	}
	out->velocity.i = state->current.i + offset.i;
	out->velocity.j = state->current.j + offset.j;
	out->velocity.k = state->current.k + offset.k;
	long remaining = *ticks - 1;
	*ticks = remaining < 0 ? 0 : remaining;
}

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
long function_1fa590(long index, s_tree2d const *tree, point2f const *point)
{
	while (index != NONE)
	{
		s_tree2d_node *node;

		if (!(index & 0x80000000))
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
void function_1fa600(vector3f *vector, word flags)
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
bool function_1fa6b0(s_pathfinding_node const *node, s_pathfinding_data const *pathfinding)
{
	bool result = false;
	long index;

	for (index = node->first_surface; index != NONE; index = pathfinding->surfaces[index].next)
	{
		s_pathfinding_surface *record = &pathfinding->surfaces[index];

		if (record->type == 7)
		{
			s_surface_flags *flags = &((s_4e0348_view *)g_4e0348)->structure->flags[record->index];

			if (flags->flags & 8)
			{
				dword bit = flags->bit;
				dword *bits = (dword *)function_183fc0(record->object_index);

				result = (bits[bit >> 5] & (1 << (bit & 31))) == 0;
			}
			break;
		}
	}

	return result;
}

// @retail 0x1fa720
s_pathfinding_edge *function_1fa720(s_edge_iterator *iterator)
{
	s_pathfinding_edge *edge = 0;

	while (iterator->surface_index != NONE)
	{
		s_pathfinding_surface *surface;

		iterator->previous_surface_index = iterator->surface_index;
		iterator->edge_index = (word)NONE;
		surface = &iterator->pathfinding->surfaces[iterator->previous_surface_index];
		iterator->surface_index = surface->next;
		if (surface->type == 0)
		{
			iterator->edge_index = (word)surface->index;
			edge = &iterator->pathfinding->edges[iterator->edge_index];
			iterator->forward = edge->surface == iterator->surface;
			goto found;
		}
	}

	if (iterator->next_edge_index != (word)NONE)
	{
		iterator->edge_index = iterator->next_edge_index;
		iterator->previous_surface_index = NONE;
		edge = &iterator->pathfinding->edges[iterator->edge_index];
		iterator->forward = edge->surface == iterator->surface;
		iterator->next_edge_index = edge->next_edges[iterator->forward];
		if (iterator->next_edge_index == iterator->first_edge_index)
			iterator->next_edge_index = (word)NONE;
found:
		if (iterator->forward)
			iterator->vertex = edge->vertices[0];
		else
			iterator->vertex = edge->vertices[1];
	}

	if (++iterator->count > 2000)
		edge = 0;
	return edge;
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
	vector3f normal;
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
	vector3f vector;
};

// @retail 0x1faf30
void function_1faf30(s_1faf30_timer *timer)
{
	real seconds = (real)g_510c54->field_2_3 * 1.5f;
	long ticks;

	__asm
	{
		fld seconds
		fistp ticks
	}
	timer->ticks = ticks;
	timer->vector = *g_4687a4;
}
