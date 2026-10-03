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
long function_248620(s_particle_system_datum *particle_system);
void function_248d90(s_particle_location_datum *particle_location, long *first_index, long *last_index);
void function_248970(s_particle_location_datum *particle_location, bool first_person, real unknown, s_particle_system_datum *particle_system, real *values, real_matrix4x3 const *matrix);
real function_248df0(long index, void *a, void *b, void const *c);
bool function_178af0(long effect_index);
void function_178b30(long effect_index, long unknown0, long unknown4);
real_rgb_color *pixel32_to_real_rgb_color(dword pixel, real_rgb_color *color);
dword __cdecl real_rgb_color_to_pixel32(const real_rgb_color *color);

/* the last time particle systems ran out */
long g_47ff88;
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

// @retail 0x173ba0
void __stdcall function_173ba0(dword mask, void *a, void *b, void const *c, real *values)
{
	while (mask)
	{
		long index;

		__asm
		{
			bsf ecx, mask
			mov index, ecx
		}
		mask &= ~(1 << index);
		values[index] = function_248df0(index, a, b, c);
	}
}

// @retail 0x173fd0
long function_173fd0(s_effect_particle_system_definition *definition, long effect_index, long tag_index, short definition_index, long event_index)
{
	long particle_system_index = NONE;

	if (!TEST_FIELD_BIT(definition->flag3) && definition->tag_index != NONE && function_137bd0(definition->tag_index) && definition->unknown30 > 0)
	{
		if (effect_index == NONE || function_178af0(effect_index))
		{
			particle_system_index = datum_new(g_510c74);
			if (particle_system_index != NONE)
			{
				s_particle_system_datum *particle_system = DATUM(g_510c74, s_particle_system_datum, particle_system_index);

				*((word *)particle_system + 6) = 9;
				particle_system->flag4 = definition->flag0;
				particle_system->flag5 = definition->flag1;
				particle_system->flag6 = definition->flag2;
				particle_system->random_a = _real_random(&g_4e7408->seed, __FILE__, __LINE__);
				particle_system->random_b = _real_random(&g_4e7408->seed, __FILE__, __LINE__);
				particle_system->tag_index = tag_index;
				particle_system->effect_index = effect_index;
				particle_system->next_index = NONE;
				particle_system->previous_index = NONE;
				particle_system->location_index = NONE;
				particle_system->unknown34 = NONE;
				particle_system->definition_index = definition_index;
				particle_system->event_index = event_index;
				particle_system->first_child_index = NONE;
				particle_system->last_child_index = NONE;
				particle_system->parent = 0;
				particle_system->unknown4c = NONE;
				particle_system->color = 0xff808080;
			}
			else
			{
				long game_time = g_510c54->game_time;

				if (game_time - g_47ff88 > g_510c54->ticks_per_second * 60)
					g_47ff88 = game_time;
			}
			if (particle_system_index != NONE && effect_index != NONE && TAG_GROUP(tag_index) == 'effe')
				function_178b30(effect_index, (long)definition, particle_system_index);
		}
	}
	return particle_system_index;
}

// @retail 0x1751d0
s_effect_particle_system_definition *s_particle_system_datum::get_definition()
{
	switch (TAG_GROUP(tag_index))
	{
	case 'effe':
		return &TAG_GET(s_effect_definition, tag_index)->events[event_index].particle_systems[definition_index];
	case 'bsdt':
		return (s_effect_particle_system_definition *)(*(byte **)(g_4e3b44[tag_index & 0xffff].bytes + 0x18) + definition_index * sizeof(s_effect_particle_system_definition));
	case 'MTRP':
	case 'prt3':
		return ((c_particle_definition *)function_137bd0(parent->get_definition()->tag_index))->get_definition(definition_index);
	}
	return 0;
}

// @retail 0x175270
void function_175270(s_particle_system_datum *particle_system, s_particle_system_spawn *spawn, real_matrix4x3 const *matrix, bool first_person)
{
	struct
	{
		dword mask;
		s_particle_system_datum *particle_system;
		s_particle_location_datum *particle_location;
		void *unknown0c;
	} query;
	real values[17];
	s_particle_location_datum *particle_location;

	*((word *)particle_system + 6) |= 0x101;
	query.mask = 0;
	query.particle_system = 0;
	query.particle_location = 0;
	query.unknown0c = 0;
	if (spawn->location_index == NONE)
	{
		spawn->location_index = function_248620(particle_system);
		if (spawn->location_index == NONE)
			return;
		particle_location = DATUM(g_51ec8c, s_particle_location_datum, spawn->location_index);
		function_248d90(particle_location, &particle_system->location_index, &particle_system->unknown34);
		particle_location->position = matrix->position;
	}
	particle_system->unknown04 = (spawn->unknown + spawn->scale) * particle_system->unknown08;
	particle_location = DATUM(g_51ec8c, s_particle_location_datum, spawn->location_index);
	if (particle_system != query.particle_system)
	{
		query.particle_system = particle_system;
		query.mask &= 0xfffff98f;
	}
	if (particle_location != query.particle_location)
	{
		query.particle_location = particle_location;
		query.mask &= 0xfffecf7f;
	}
	function_173ba0(~query.mask & 0x1ffff, query.particle_system, query.particle_location, matrix, values);
	query.mask |= 0x1ffff;
	function_248970(particle_location, first_person, spawn->unknown, particle_system, values, matrix);
	spawn->location_index = particle_location->next_index;
}

// @retail 0x175a80
void function_175a80(bool tinted, dword color_a, dword color_b, s_particle_system_datum *particle_system, bool multiplied)
{
	if (!multiplied)
	{
		if (tinted)
		{
			real_rgb_color color;

			pixel32_to_real_rgb_color(color_b, &color);
			color.red *= 0.5f;
			color.green *= 0.5f;
			color.blue *= 0.5f;
			particle_system->color = real_rgb_color_to_pixel32(&color);
		}
	}
	else if (tinted)
	{
		real_rgb_color color;
		real_rgb_color other;

		pixel32_to_real_rgb_color(color_a, &color);
		pixel32_to_real_rgb_color(color_b, &other);
		color.red *= other.red;
		color.green *= other.green;
		color.blue *= other.blue;
		particle_system->color = real_rgb_color_to_pixel32(&color);
	}
	else
	{
		particle_system->color = color_a;
	}
}
