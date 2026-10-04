/* IMPACTS.H: what creates a havok contact impact (src/impacts.cpp) */

#ifndef IMPACTS_H
#define IMPACTS_H

#include "unknown_11c920.h"
#include "unknown_0259d0.h"

/* a global material type (a 16 bit index) */
class c_type_47f957
{
public:
	c_type_47f957() : m_index(NONE) {}
	explicit c_type_47f957(short index) : m_index(index) {}

	bool operator==(c_type_47f957 other) const
	{
		return m_index == other.m_index;
	}

	short m_index;
};

/* a shape of a physics model: its type (sphere, pill, box ...) and index */
struct s_physics_model_shape_key
{
	short type;
	short index;
};

/* a tag block: its element count and address */
struct s_impact_tag_block
{
	long count;
	byte *address;
};

/* what creates an impact: the two havok components in contact, their
   materials, the contact point and normal */
struct s_impact_data
{
	bool unknown00;
	byte unknown01[3];
	long component_a;
	long unknown08;
	c_type_47f957 material_a;
	byte unknown0e[2];
	long component_b;
	long unknown14;
	c_type_47f957 material_b;
	byte unknown1a[2];
	point3f position;
	vector3f normal;
	long type;
	bool unknown38;
	byte unknown39;
	s_physics_model_shape_key shape;
};

void impact_data_set(s_impact_data *data, bool unknown00, long component_a, long unknown08, c_type_47f957 material_a,
	long component_b, long unknown14, c_type_47f957 material_b, point3f const *position, vector3f const *normal,
	long type, s_physics_model_shape_key const *shape);

s_impact_tag_block *physics_model_shape_block_get(byte *physics_model, s_physics_model_shape_key const *key, long *element_size);

#endif
