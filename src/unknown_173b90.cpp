// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_173B90.CPP: the particle systems effects start (g_510c74) and
   their particles, emitters and locations */

#include "cseries.h"
#include "globals.h"
#include "data_array.h"
#include "effects.h"

struct s_bsp3d;
extern s_bsp3d *g_4e033c;
long function_14a280(s_bsp3d *bsp, real_point3d *point, long index);

struct s_structure_leaf_173b90
{
	short cluster_index;
	byte unknown02[6];
};

struct s_structure_bsp_173b90
{
	byte unknown00[0x30];
	s_structure_leaf_173b90 *leaves;
};

void __stdcall function_2486e0(long particle_location_index);
void effect_remove_event_slot(long effect_index, long value);
void function_1753f0(s_particle_system_datum *particle_system);

// @retail 0x173b90
long function_173b90(s_particle_system_datum *particle_system)
{
	return particle_system->flag10;
}

// @retail 0x173de0
void particle_systems_initialize(void)
{
	g_510c74 = data_new_inlined("particle_system", 0x80, sizeof(s_particle_system_datum), 0, g_510c2c);
	g_51ec84 = data_new_inlined("particles", 0x400, 0x40, 0, g_510c2c);
	g_51ec88 = data_new_inlined("particle_emitter", 0x100, 0x4c, 0, g_510c2c);
	g_51ec8c = data_new_inlined("particle_location", 0x100, sizeof(s_particle_location_datum), 0, g_510c2c);
}

// @retail 0x173ee0
void particle_systems_update_locations(void)
{
	s_data_iterator iterator;
	short bsp_index = g_4686c4;
	s_particle_system_datum *particle_system;

	iterator.data = g_510c74;
	iterator.index = NONE;
	while ((particle_system = (s_particle_system_datum *)data_iterator_next_inlined(&iterator)) != 0)
	{
		if (particle_system->location.bsp_index != bsp_index)
		{
			s_location location;

			if (particle_system->location_index != NONE)
			{
				s_particle_location_datum *particle_location = DATUM(g_51ec8c, s_particle_location_datum, particle_system->location_index);

				if (bsp_index != NONE)
				{
					location.leaf_index = function_14a280(g_4e033c, &particle_location->position, 0);
					if (location.leaf_index != NONE)
						location.cluster_index = ((s_structure_bsp_173b90 *)g_4e0348)->leaves[location.leaf_index].cluster_index;
					else
						location.cluster_index = NONE;
				}
				else
				{
					location.leaf_index = NONE;
					location.cluster_index = NONE;
				}
			}
			else
			{
				location.leaf_index = NONE;
				location.cluster_index = NONE;
			}
			location.bsp_index = bsp_index;
			particle_system->set_location(&location);
		}
	}
}

// @retail 0x174180
void __stdcall particle_system_delete(long particle_system_index)
{
	s_particle_system_datum *particle_system = DATUM(g_510c74, s_particle_system_datum, particle_system_index);
	long child_index;

	function_1753f0(particle_system);
	child_index = particle_system->first_child_index;
	while (child_index != NONE)
	{
		s_particle_system_datum *child = DATUM(g_510c74, s_particle_system_datum, child_index);
		long next_index = child->next_index;

		particle_system_unlink(child, &particle_system->first_child_index, &particle_system->last_child_index);
		particle_system_delete(child_index);
		child_index = next_index;
	}
	if (particle_system->effect_index != NONE && TAG_GROUP(particle_system->tag_index) == 'effe')
		effect_remove_event_slot(particle_system->effect_index, particle_system_index);
	datum_delete(g_510c74, particle_system_index);
}

// @retail 0x175070
void particle_system_link(s_particle_system_datum *particle_system, long *last_index, long *first_index)
{
	s_data_array *data = g_510c74;
	long particle_system_index = (particle_system->salt << 16) | (particle_system - (s_particle_system_datum *)data->data);

	particle_system->next_index = NONE;
	if (*first_index == NONE)
		*first_index = particle_system_index;
	if (*last_index != NONE)
	{
		s_particle_system_datum *last = DATUM(data, s_particle_system_datum, *last_index);

		last->next_index = particle_system_index;
		particle_system->previous_index = (last->salt << 16) | (last - (s_particle_system_datum *)data->data);
		*last_index = particle_system_index;
	}
	else
	{
		*last_index = particle_system_index;
		particle_system->previous_index = NONE;
	}
}

// @retail 0x175100
void particle_system_unlink(s_particle_system_datum *particle_system, long *first_index, long *last_index)
{
	s_data_array *data = g_510c74;
	s_particle_system_datum *elements = (s_particle_system_datum *)data->data;
	long particle_system_index = (particle_system - elements) | (particle_system->salt << 16);

	if (particle_system->next_index != NONE)
		DATUM(data, s_particle_system_datum, particle_system->next_index)->previous_index = particle_system->previous_index;
	if (particle_system->previous_index != NONE)
		DATUM(data, s_particle_system_datum, particle_system->previous_index)->next_index = particle_system->next_index;
	if (*first_index == particle_system_index)
		*first_index = particle_system->next_index;
	if (*last_index == particle_system_index)
		*last_index = particle_system->previous_index;
	particle_system->next_index = NONE;
	particle_system->previous_index = NONE;
}

// @retail 0x175180
void s_particle_system_datum::set_location(s_location const *location)
{
	long child_index = first_child_index;

	this->location = *location;
	while (child_index != NONE)
	{
		s_particle_system_datum *child = DATUM(g_510c74, s_particle_system_datum, child_index);

		child->set_location(location);
		child_index = child->next_index;
	}
}

// @retail 0x1753f0
void function_1753f0(s_particle_system_datum *particle_system)
{
	long index = particle_system->location_index;

	while (index != NONE)
	{
		long next_index = DATUM(g_51ec8c, s_particle_location_datum, index)->next_index;

		function_2486e0(index);
		index = next_index;
	}
	particle_system->location_index = NONE;
	particle_system->unknown34 = NONE;
}
