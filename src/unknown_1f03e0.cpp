// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1F03E0.CPP: the state of a moving physics shape (0x74 bytes) and
   the side of a contact it touches */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include "data_array.h"
#include <math.h>

struct s_shape_state
{
	real_point3d point;
	long unknown0c;
	byte unknown10[0x1c - 0x10];
	long unknown1c;
	long unknown20;
	real_matrix4x3 matrix;
	long unknown58;
	long unknown5c;
	short material;
	bool unknown62;
	byte unknown63;
	real_vector3d normal;
	real unknown70;
};

real_matrix4x3 *g_4687d0;
extern short g_47d8e0;
extern short g_54e898;

// @retail 0x1f03e0
void function_1f03e0(s_shape_state *state)
{
	state->unknown0c = NONE;
	state->point = *(real_point3d *)g_4687a4;
	state->unknown62 = false;
	state->normal = *g_4687b0;
	state->material = g_47d8e0;
	state->unknown58 = NONE;
	state->unknown5c = NONE;
	state->unknown1c = NONE;
	state->unknown20 = NONE;
	state->unknown70 = 0.0f;
	state->matrix = *g_4687d0;
}

/* the contact at a surface and the side it faces */
struct s_shape_contact
{
	byte unknown00[0xdc];
	real_vector3d normal;
};

struct s_shape_side
{
	long unknown0;
	long side;
};

// @retail 0x1f1930
void function_1f1930(s_shape_contact const *contact, s_shape_side *side)
{
	if (contact->normal.i != 0.0f || contact->normal.j != 0.0f || contact->normal.k != 0.0f)
	{
		if (fabs(contact->normal.i) < fabs(contact->normal.j))
		{
			if (contact->normal.j < 0.0f)
				side->side = 2;
			else
				side->side = 3;
		}
		else
		{
			if (contact->normal.i < 0.0f)
				side->side = 5;
			else
				side->side = 4;
		}
	}
}

/* the havok components (g_51e9b8, 0xa0 bytes) and their materials */
struct s_component_material
{
	byte unknown00[0x12];
	short material;
	byte unknown14[0x48 - 0x14];
};

struct s_component
{
	byte unknown00[0x88];
	s_component_material *materials;
	byte unknown8c[0xa0 - 0x8c];
};

// @retail 0x1f1df0
void function_1f1df0(long component_index, long material_index, real_vector3d const *normal, s_shape_state *state)
{
	s_component *component = (s_component *)(g_51e9b8->data + (component_index & 0xffff) * sizeof(s_component));

	state->normal = *normal;
	if (material_index != NONE)
		state->material = component->materials[material_index].material;
	else
		state->material = g_54e898;
}
