// @flags /O2 /Gr
/* UNKNOWN_2055F0.CPP: what creates a havok contact impact, and the impact
   list of a havok component (src/impacts.cpp) */

#include "cseries.h"
#include "globals.h"
#include "data_array.h"
#include "unknown_1cec30.h"
#include "impacts.h"

/* a component's impacts (g_51ec00, 0x40 bytes) */
struct s_havok_component_impacts
{
	short salt;
	short count;
	long impact_indices[15];
};

extern s_data_array *g_51ec00;

// @retail 0x2055f0
void impact_data_set(
	s_impact_data *data,
	bool unknown00,
	long component_a,
	long unknown08,
	c_global_material_type material_a,
	long component_b,
	long unknown14,
	c_global_material_type material_b,
	real_point3d const *position,
	real_vector3d const *normal,
	long type,
	s_physics_model_shape_key const *shape)
{
	data->unknown00 = unknown00;
	data->component_a = component_a;
	data->unknown08 = unknown08;
	data->material_a = material_a;
	data->component_b = component_b;
	data->unknown14 = unknown14;
	data->material_b = material_b;
	data->position = *position;
	data->normal = *normal;
	data->type = type;
	data->unknown38 = false;
	if (shape)
	{
		data->shape = *shape;
	}
	else
	{
		data->shape.type = NONE;
		data->shape.index = NONE;
	}
}

// @retail 0x205680
void havok_component_impact_add(
	s_havok_component *component,
	long impact_index)
{
	s_havok_component_impacts *impacts;

	if (component->unknown20 == NONE)
	{
		long impacts_index = datum_new(g_51ec00);

		((s_havok_component_impacts *)g_51ec00->data)[impacts_index & 0xffff].count = 0;
		component->unknown20 = impacts_index;
	}
	impacts = &((s_havok_component_impacts *)g_51ec00->data)[component->unknown20 & 0xffff];
	impacts->impact_indices[impacts->count] = impact_index;
	impacts->count++;
}
