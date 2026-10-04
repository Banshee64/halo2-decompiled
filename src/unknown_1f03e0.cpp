// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1F03E0.CPP: the state of a moving physics shape (0x74 bytes) and
   the side of a contact it touches */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_0259d0.h"
#include "data_array.h"
#include <math.h>

#define PIN(x, lower, upper) ((x) < (lower) ? (lower) : ((x) > (upper) ? (upper) : (x)))

struct s_shape_state
{
	point3f point;
	long unknown0c;
	byte unknown10[0x1c - 0x10];
	long unknown1c;
	long unknown20;
	transform4x3f matrix;
	long unknown58;
	long unknown5c;
	short material;
	bool unknown62;
	byte unknown63;
	vector3f normal;
	real unknown70;
};

transform4x3f *g_4687d0;
extern short g_47d8e0;
extern short g_54e898;

// @retail 0x1f03e0
void function_1f03e0(s_shape_state *state)
{
	state->unknown0c = NONE;
	state->point = *(point3f *)g_4687a4;
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
	vector3f normal;
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
void function_1f1df0(long component_index, long material_index, vector3f const *normal, s_shape_state *state)
{
	s_component *component = (s_component *)(g_51e9b8->data + (component_index & 0xffff) * sizeof(s_component));

	state->normal = *normal;
	if (material_index != NONE)
		state->material = component->materials[material_index].material;
	else
		state->material = g_54e898;
}

/* the surfaces' minimum and maximum heights: whether the shape stands at a
   height it can step to */
struct s_shape_ground
{
	byte unknown00[0x1c];
	point3f point;
	byte unknown28[0x34 - 0x28];
	real height;
};

// @retail 0x1f2e60
bool function_1f2e60(bool moving, s_shape_ground const *ground, vector3f const *velocity, bool stepping, point3f const *base, real height)
{
	real top = base->z + height;

	if (top - 0.001f > base->z)
	{
		point3f point = ground->point;
		point3f center;
		vector3f v;
		real distance;
		real lower, upper;

		if (moving)
		{
			point.x = velocity->i * g_510c54->rate + point.x;
			point.y = velocity->j * g_510c54->rate + point.y;
			point.z = velocity->k * g_510c54->rate + point.z;
		}
		center.x = base->x;
		center.y = base->y;
		center.z = height + base->z;
		vector3d_from_points3d(&center, &point, &v);
		distance = (real)sqrt(v.k * v.k + v.j * v.j + v.i * v.i) - height;
		lower = moving ? -0.25f : -0.1f;
		upper = stepping ? 0.1f : 0.2f;
		return PIN(distance, lower, upper) == distance;
	}

	return PIN(ground->height, -0.1f, 0.0328f) == ground->height;
}