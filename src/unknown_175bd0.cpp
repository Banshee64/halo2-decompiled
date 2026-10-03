// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_175BD0.CPP: the effects (entry 40 of the lifecycle table) */

#include "cseries.h"
#include "globals.h"
#include "data_array.h"
#include "effects.h"
#include <string.h>

s_data_array *g_51ec8c;
s_data_array *g_51ec88;
s_data_array *g_51ec84;
s_data_array *g_510c74;
s_data_array *g_4ea93c;
s_data_array *g_4ea938;

/* the up vector of the effect markers (unknown_11d180.cpp) */
extern real_vector3d *g_4687bc;

/* the source of the effect being started (s_effect_parameters::source) */
s_effect_source *g_510c78;
bool g_510c7c;

struct s_object;
s_object *function_badc0(long object_index, dword type_mask);
bool object_or_parent_hidden(long object_index);
real_point3d *function_b9dd0(long object_index, real_point3d *result);
byte *datum_get(s_data_array *data, long datum_index);
long function_18a750(long tag_index, long value);
void function_11bed0(real_point3d const *point, s_location *location);
void function_c40f0(long tag_index, long object_index, real value);
real _real_random_range(dword *seed, char const *file, long line, real lower_bound, real upper_bound);
int __fastcall function_142a60(real_matrix4x3 const *a, real_matrix4x3 const *b, real_matrix4x3 *result);
real_matrix4x3 *function_1664a5(long group_index, long key, short node_index);
void function_1664da(long group_index, long key, short node_index, real_matrix4x3 *out);

/* the views of objects the effects read */
struct s_effect_object_header
{
	short identifier;
	byte flags;
	byte type;
	byte unknown04[4];
	byte *object;
};

struct s_effect_attachment
{
	byte type;
	byte unknown01[3];
	long index;
};

struct s_effect_object
{
	long tag_index;
	byte unknown04[0x10a - 4];
	word unknown10a_0 : 2;
	word flag10a_2 : 1;
	word : 13;
	byte unknown10c[0x116 - 0x10c];
	short nodes_offset;
	byte unknown118[4];
	short attachments_size;
	short attachments_offset;
	byte unknown120[0x13c - 0x120];
	long player_index;
	byte unknown140[0x2e0 - 0x140];
	long unknown2e0;
};

struct s_effect_object_definition
{
	byte unknown00[2];
	word flag0 : 1;
	word flag1 : 1;
	word flag2 : 1;
	word flag3 : 1;
	word flag4 : 1;
	word flag5 : 1;
	word flag6 : 1;
	word flag7 : 1;
	word flag8 : 1;
	word flag9 : 1;
	word flag10 : 1;
	word flag11 : 1;
	word flag12 : 1;
	word flag13 : 1;
	word : 2;
};

#define OBJECT_GET(index) ((s_effect_object *)((s_effect_object_header *)g_4e0300->data)[(index) & 0xffff].object)

/* an object looping sound (g_4ed28c, 0x18 bytes) */
struct s_effect_looping_sound
{
	short salt;
	short unknown02;
	byte flag0 : 1;
	byte flag1 : 1;
	byte : 6;
	byte unknown05[0x18 - 5];
};

/* a player (g_4e8c24, 0x21c bytes) */
struct s_effect_player
{
	byte unknown00[0x28];
	short unknown28;
	byte unknown2a[0x21c - 0x2a];
};

struct s_effect_looping_sound;

/* the effect of an index that may be stale (the salt is checked) */
static inline s_effect_datum *effect_try_and_get(long effect_index)
{
	s_effect_datum *result = 0;

	if (effect_index != NONE)
	{
		s_data_array *data = g_4ea93c;
		long index = effect_index & 0xffff;

		if (index < data->high_water_index)
		{
			s_effect_datum *effect = (s_effect_datum *)(data->data + data->size * index);

			if (effect->salt != 0 && effect->salt == (effect_index >> 16))
				result = effect;
		}
	}
	return result;
}

static inline void effect_stop_looping_sound(s_effect_datum *effect)
{
	if (effect->looping_sound_index != NONE)
	{
		DATUM(g_4ed28c, s_effect_looping_sound, effect->looping_sound_index)->flag1 = true;
		effect->looping_sound_index = NONE;
	}
}

static inline void effect_owner_set_none(s_effect_owner *owner)
{
	owner->unknown4 = NONE;
	owner->unknown0 = NONE;
	owner->unknown8 = NONE;
}

static inline void effect_parameters_initialize_inline(s_effect_parameters *parameters)
{
	memset(parameters, 0, sizeof(*parameters));
	parameters->tag_index = NONE;
	parameters->unknown18 = NONE;
	parameters->object_index = NONE;
	effect_owner_set_none(&parameters->owner);
	parameters->unknown34 = 0;
	parameters->unknown38 = 0;
	parameters->scale_a = 1.0f;
	parameters->scale_b = 1.0f;
	parameters->unknown3c = 0;
	parameters->unknown30 = 0;
	parameters->color_a = 0xff808080;
	parameters->color_b = 0xff808080;
	parameters->source = 0;
}

long effect_new_from_parameters(s_effect_parameters const *parameters);
bool function_176210(s_effect_parameters const *parameters);
long effect_new(long tag_index, s_effect_owner const *owner, bool force);
void function_178240(real_point3d const *origin, real_vector3d const *direction, s_effect_datum *effect, real scale_a, real scale_b);
bool function_175f50(long object_index);
void function_17b750(long *values, long value);
void function_178360(long effect_index, short unknown18, long object_index, long unknown58, s_effect_marker *markers, long marker_count);
bool function_1789f0(s_effect_datum *effect);
void function_17b5d0(s_effect_datum *effect, long object_index, s_effect_parameters const *parameters, long unknown);
void function_177310(long effect_index);
void function_179850(long effect_index, real value);
void function_1771a0(s_effect_datum *effect);
void function_1773a0(long effect_index);
void function_177460(s_effect_datum *effect);
void function_177590(long effect_index);
bool function_177610(long effect_index);
dword *function_177c20(long tag_index);
bool function_178060(void);
void function_178ad0(s_effect_datum *effect);
void function_1782a0(long effect_index, short event_index);
void function_17add0(s_effect_datum *effect);
s_effect_location_datum *__stdcall effect_location_next(s_effect_datum *effect, long *location_index, short mode);
void function_176a50(s_effect_parameters *parameters, long tag_index, long marker_count, s_effect_marker *markers, long mode);
s_effect_marker *function_176330(s_effect_marker *markers, real_point3d const *point);

// @retail 0x175b50
void effects_initialize(void)
{
	g_4ea93c = data_new_inlined("effect", 0x100, sizeof(s_effect_datum), 0, g_510c2c);
	g_4ea938 = data_new_inlined("effect location", 0x200, sizeof(s_effect_location_datum), 0, g_510c2c);
	particle_systems_initialize();
}

// @retail 0x175bd0
void effects_dispose(void)
{
	g_51ec8c = 0;
	g_51ec88 = 0;
	g_51ec84 = 0;
	g_510c74 = 0;
	if (g_4ea93c)
	{
		g_4ea93c = 0;
	}
	if (g_4ea938)
	{
		g_4ea938 = 0;
	}
}

// @retail 0x175c10
void effects_initialize_for_new_map(void)
{
	data_make_valid_inlined(g_4ea93c);
	data_make_valid_inlined(g_4ea938);
	particle_systems_initialize_for_new_map();
}

// @retail 0x175c70
void effects_dispose_from_old_map(void)
{
	g_51ec8c->valid = false;
	g_51ec88->valid = false;
	g_51ec84->valid = false;
	g_510c74->valid = false;
	g_4ea93c->valid = false;
	g_4ea938->valid = false;
}

// @retail 0x175cb0
void effects_delete_all(void)
{
	s_data_array *array = g_4ea93c;
	long index = NONE;

	for (;;)
	{
		index = data_find_index(array, index + 1);
		if (index == NONE)
			break;
		effect_delete(data_datum_index(array, index));
	}
	particle_systems_dispose_from_old_map();
	particle_systems_initialize_for_new_map();
}

// @retail 0x175ee0
void effect_parameters_initialize(s_effect_parameters *parameters)
{
	effect_parameters_initialize_inline(parameters);
}

// @retail 0x175f50
bool function_175f50(long object_index)
{
	bool result = false;
	s_effect_object *object = (s_effect_object *)function_badc0(object_index, 1);

	if (object && TEST_FIELD_BIT(object->flag10a_2))
	{
		if (object->unknown2e0 != NONE && object->unknown2e0 + g_510c54->ticks_per_second < g_510c54->game_time)
			return true;
		return false;
	}
	return result;
}

// @retail 0x176330
s_effect_marker *function_176330(s_effect_marker *markers, real_point3d const *point)
{
	markers[0].forward = *g_4687bc;
	markers[0].position = *point;
	markers[0].name = 0x70000c0;
	markers[1].forward = *g_4687b0;
	markers[1].position = *point;
	markers[1].name = 0x20000ca;
	return markers;
}

// @retail 0x1763a0
void function_1763a0(s_effect_marker *markers, real_point3d const *point, real_vector3d const *direction, real_vector3d const *normal)
{
	markers[0].position = *point;
	markers[0].name = 0x60000b8;
	markers[1].position = *point;
	markers[1].name = 0x700054c;
	markers[2].position = *point;
	markers[2].name = 0x8000550;
	markers[3].position = *point;
	markers[3].name = 0xa0000bf;
	markers[4].position = *point;
	markers[4].name = 0x70000c0;
	markers[5].position = *point;
	markers[5].name = 0x20000ca;

	markers[0].forward = *normal;
	markers[1].forward = *direction;
	markers[2].forward.i = direction->i * -1.0f;
	markers[2].forward.j = direction->j * -1.0f;
	markers[2].forward.k = direction->k * -1.0f;

	real_vector3d *incident = &markers[1].forward;
	real_vector3d *surface = &markers[0].forward;
	real twice = (incident->j * surface->j + incident->k * surface->k + incident->i * surface->i) * 2.0f;

	markers[3].forward.i = incident->i - surface->i * twice;
	markers[3].forward.j = incident->j - twice * surface->j;
	markers[3].forward.k = incident->k - surface->k * twice;

	real magnitude = (real)sqrt(incident->i * incident->i + incident->j * incident->j + incident->k * incident->k);
	if (!(fabs(magnitude) < 0.0001f))
	{
		real inverse = 1.0f / magnitude;

		incident->i = inverse * incident->i;
		incident->j = inverse * incident->j;
		incident->k = inverse * incident->k;
	}
	markers[0].forward = *normal;
	markers[4].forward = *g_4687bc;
	markers[5].forward = *g_4687b0;
}

// @retail 0x176a50
void function_176a50(s_effect_parameters *parameters, long tag_index, long marker_count, s_effect_marker *markers, long mode)
{
	memset(parameters, 0, sizeof(*parameters));
	parameters->unknown34 = 0;
	parameters->unknown38 = 0;
	parameters->unknown3c = 0;
	parameters->unknown30 = 0;
	parameters->source = 0;
	parameters->flags = 0;
	parameters->tag_index = tag_index;
	parameters->unknown18 = NONE;
	parameters->object_index = NONE;
	effect_owner_set_none(&parameters->owner);
	parameters->markers = markers;
	parameters->color_a = 0xff808080;
	parameters->color_b = 0xff808080;
	parameters->scale_a = 1.0f;
	parameters->scale_b = 1.0f;
	parameters->marker_count = marker_count;
	if (mode == 1)
		parameters->flags = 4;
}

// @retail 0x177260
void function_177260(long effect_index, bool flag)
{
	s_effect_datum *effect = effect_try_and_get(effect_index);

	if (effect)
	{
		effect_stop_looping_sound(effect);
		function_1771a0(effect);
		if (TEST_FIELD_BIT(effect->flag1))
		{
			effect->flag4 = flag;
			effect->flag2 = true;
		}
		else
		{
			function_177610(effect_index);
		}
	}
}

// @retail 0x1771a0
void function_1771a0(s_effect_datum *effect)
{
	s_effect_definition *definition = TAG_GET(s_effect_definition, effect->tag_index);

	if (TEST_FIELD_BIT(definition->flag5))
	{
		for (long i = 0; i < definition->event_count; i++)
		{
			s_effect_event *event = &definition->events[i];

			for (long j = 0; j < event->part_count; j++)
			{
				s_effect_part *part = &event->parts[j];

				if (part->group_tag == 'tdtl' && part->tag_index != NONE)
					function_c40f0(part->tag_index, effect->object_index, 0.0f);
			}
		}
	}
}

// @retail 0x177310
void function_177310(long effect_index)
{
	s_effect_datum *effect = DATUM(g_4ea93c, s_effect_datum, effect_index);
	s_effect_definition *definition = TAG_GET(s_effect_definition, effect->tag_index);

	effect->flag2 = false;
	function_1782a0(effect_index, 0);
	if (definition->looping_sound_tag_index != NONE && definition->looping_sound_location != NONE && effect->looping_sound_index == NONE)
	{
		long location_index = effect->location_indices[definition->looping_sound_location];

		if (effect_location_next(effect, &location_index, 3))
			effect->looping_sound_index = function_18a750(definition->looping_sound_tag_index, effect_index);
	}
}

// @retail 0x1773a0
void function_1773a0(long effect_index)
{
	s_effect_datum *effect = effect_try_and_get(effect_index);

	if (effect)
	{
		s_effect_definition *definition = TAG_GET(s_effect_definition, effect->tag_index);

		for (long i = 0; i < definition->location_count; i++)
		{
			long index = effect->location_indices[i];

			while (index != NONE)
			{
				long next_index = DATUM(g_4ea938, s_effect_location_datum, index)->next_index;

				datum_delete(g_4ea938, index);
				index = next_index;
			}
			effect->location_indices[i] = NONE;
		}
	}
}

// @retail 0x177460
void function_177460(s_effect_datum *effect)
{
	long index = effect->last_particle_system_index;

	while (index != NONE)
	{
		s_particle_system_datum *particle_system = DATUM(g_510c74, s_particle_system_datum, index);
		long previous_index = particle_system->previous_index;

		particle_system_unlink(particle_system, &effect->first_particle_system_index, &effect->last_particle_system_index);
		particle_system_delete(index);
		index = previous_index;
	}
	effect->first_particle_system_index = NONE;
	effect->last_particle_system_index = NONE;
	function_178ad0(effect);
}

// @retail 0x1774f0
void effect_delete(long effect_index)
{
	s_effect_datum *effect = effect_try_and_get(effect_index);

	if (effect)
	{
		function_177590(effect_index);
		effect_stop_looping_sound(effect);
		function_1771a0(effect);
		function_1773a0(effect_index);
		function_177460(effect);
		datum_delete(g_4ea93c, effect_index);
	}
}

// @retail 0x177590
void function_177590(long effect_index)
{
	s_effect_datum *effect = effect_try_and_get(effect_index);

	if (effect)
	{
		s_effect_object *object = (s_effect_object *)function_badc0(effect->object_index, NONE);

		if (object)
		{
			long count = (long)((dword)(long)object->attachments_size / sizeof(s_effect_attachment));
			s_effect_attachment *attachments = (s_effect_attachment *)((byte *)object + object->attachments_offset);

			for (long i = 0; i < count; i++)
			{
				if (attachments[i].index == effect_index && attachments[i].type == 2)
				{
					attachments[i].index = NONE;
					return;
				}
			}
		}
	}
}

// @retail 0x177610
bool function_177610(long effect_index)
{
	s_effect_datum *effect = effect_try_and_get(effect_index);
	bool result = true;

	if (effect)
	{
		function_177590(effect_index);
		effect_stop_looping_sound(effect);
		if (effect->first_particle_system_index == NONE)
		{
			effect_delete(effect_index);
		}
		else
		{
			for (long index = effect->last_particle_system_index; index != NONE; )
			{
				s_particle_system_datum *particle_system = DATUM(g_510c74, s_particle_system_datum, index);

				particle_system->flag0 = false;
				index = particle_system->previous_index;
			}
			effect->flag2 = true;
			effect->flag6 = true;
			return false;
		}
	}
	return result;
}

// @retail 0x177c20
dword *function_177c20(long tag_index)
{
	if (TEST_FIELD_BIT(TAG_GET(s_effect_definition, tag_index)->flag2))
		return &g_4e7408->unknown0;
	return &g_4e7408->seed;
}

// @retail 0x178060
bool function_178060(void)
{
	s_data_array *array = g_4ea93c;
	bool result = false;
	long datum = data_datum_index(array, data_next_absolute_index(array, 0));

	while (datum != NONE)
	{
		s_effect_datum *effect = DATUM(array, s_effect_datum, datum);

		if (!TEST_FIELD_BIT(TAG_GET(s_effect_definition, effect->tag_index)->flag2))
		{
			effect_delete(datum);
			result = true;
			break;
		}
		datum = data_datum_index(array, data_next_absolute_index(array, datum == NONE ? 0 : (datum & 0xffff) + 1));
	}
	return result;
}

// @retail 0x178120
long effect_new(long tag_index, s_effect_owner const *owner, bool force)
{
	long effect_index = NONE;

	if (tag_index != NONE)
	{
		s_effect_definition *definition = TAG_GET(s_effect_definition, tag_index);

		if ((force || !TEST_FIELD_BIT(definition->flag2)) && definition->event_count > 0)
		{
			s_data_array *effects = g_4ea93c;

			effect_index = datum_new(effects);
			if (effect_index == NONE)
			{
				if (TEST_FIELD_BIT(definition->flag2) && function_178060())
					effect_index = datum_new(effects);
			}
			if (effect_index != NONE)
			{
				s_effect_datum *effect = DATUM(effects, s_effect_datum, effect_index);

				effect->tag_index = tag_index;
				if (owner)
				{
					effect->owner = *owner;
				}
				else
				{
					effect->flag8 = true;
					effect_owner_set_none(&effect->owner);
				}
				function_178ad0(effect);
				effect->looping_sound_index = NONE;
				effect->object_index = NONE;
				effect->unknown58 = NONE;
				effect->flags = 0;
				effect->first_particle_system_index = NONE;
				effect->last_particle_system_index = NONE;
				effect->color_a = 0xff808080;
				effect->color_b = 0xff808080;
				effect->unknown5e = NONE;
				effect->unknown74 = 0.0f;
				effect->unknown78 = 0.0f;
				function_1782a0(effect_index, 0);
			}
		}
	}
	return effect_index;
}

// @retail 0x178240
void function_178240(real_point3d const *origin, real_vector3d const *direction, s_effect_datum *effect, real scale_a, real scale_b)
{
	effect->scale_a = scale_a;
	effect->scale_b = scale_b;
	if (!origin)
		origin = g_468710;
	effect->origin = *origin;
	if (direction)
	{
		effect->direction = *direction;
	}
	else
	{
		effect->unknown40 = 0;
		effect->unknown44 = 0;
	}
}

// @retail 0x1782a0
void function_1782a0(long effect_index, short event_index)
{
	s_effect_datum *effect = effect_try_and_get(effect_index);

	if (effect)
	{
		s_effect_definition *definition = TAG_GET(s_effect_definition, effect->tag_index);

		if (event_index >= 0 && event_index < definition->event_count)
		{
			s_effect_event *event = &definition->events[event_index];

			effect->flag0 = false;
			effect->event_index = event_index;
			effect->unknown60 = 0.0f;
			if (event->delay_lower == event->delay_upper)
				effect->event_delay = event->delay_lower;
			else
				effect->event_delay = _real_random_range(function_177c20(effect->tag_index), __FILE__, __LINE__, event->delay_lower, event->delay_upper);
			function_17add0(effect);
		}
	}
}

// @retail 0x1789f0
bool function_1789f0(s_effect_datum *effect)
{
	s_effect_definition *definition = TAG_GET(s_effect_definition, effect->tag_index);

	for (long i = 0; i < definition->location_count; i++)
	{
		long location_index = effect->location_indices[i];

		if (location_index != NONE)
		{
			s_effect_location_datum *location = effect_location_next(effect, &location_index, 0);

			if (location)
			{
				function_11bed0(&location->matrix.position, &effect->location);
				return effect->location.cluster_index != NONE;
			}
			break;
		}
	}
	effect->location.bsp_index = g_4686c4;
	effect->location.leaf_index = NONE;
	effect->location.cluster_index = NONE;
	return effect->location.cluster_index != NONE;
}

// @retail 0x178ad0
void function_178ad0(s_effect_datum *effect)
{
	for (long i = 0; i < 16; i++)
	{
		effect->event_slots[i].unknown4 = NONE;
		effect->event_slots[i].unknown0 = 0;
	}
}

// @retail 0x178af0
bool function_178af0(long effect_index)
{
	s_effect_datum *effect = DATUM(g_4ea93c, s_effect_datum, effect_index);
	bool result = false;

	for (dword i = 0; i < 16; i++)
	{
		if (effect->event_slots[i].unknown4 == NONE && effect->event_slots[i].unknown0 == 0)
		{
			result = true;
			break;
		}
	}
	return result;
}

// @retail 0x178b30
void function_178b30(long effect_index, long unknown0, long unknown4)
{
	s_effect_datum *effect = DATUM(g_4ea93c, s_effect_datum, effect_index);

	for (dword i = 0; i < 16; i++)
	{
		s_effect_event_slot *slot = &effect->event_slots[i];

		if (slot->unknown4 == NONE && slot->unknown0 == 0)
		{
			slot->unknown4 = unknown4;
			slot->unknown0 = unknown0;
			return;
		}
	}
}

// @retail 0x178b80
void effect_remove_event_slot(long effect_index, long value)
{
	s_effect_datum *effect = DATUM(g_4ea93c, s_effect_datum, effect_index);

	for (dword i = 0; i < 16; i++)
	{
		s_effect_event_slot *slot = &effect->event_slots[i];

		if (slot->unknown4 == value)
		{
			slot->unknown4 = NONE;
			slot->unknown0 = 0;
			return;
		}
	}
}

// @retail 0x1794a0
bool function_1794a0(long object_index, long other_object_index)
{
	bool result = false;

	if (((s_effect_object_header *)g_4e0300->data)[object_index & 0xffff].flags & 1)
	{
		if (!object_or_parent_hidden(other_object_index))
			result = true;
	}
	return result;
}

// @retail 0x17add0
void function_17add0(s_effect_datum *effect)
{
	long index = effect->first_particle_system_index;

	while (index != NONE)
	{
		s_particle_system_datum *particle_system = DATUM(g_510c74, s_particle_system_datum, index);

		if (particle_system->unknown14 == effect->event_index)
		{
			real delay = effect->event_delay;

			particle_system->flag0 = true;
			particle_system->unknown04 = 0.0f;
			if (delay > 0.0f)
				particle_system->unknown08 = 1.0f / delay;
			else
				particle_system->unknown08 = 1.0f;
		}
		else if (particle_system->unknown14 == effect->unknown5e)
		{
			particle_system->flag0 = false;
		}
		index = particle_system->next_index;
	}
}

// @retail 0x17ae50
s_effect_location_datum *__stdcall effect_location_next(s_effect_datum *effect, long *location_index, short mode)
{
	s_effect_location_datum *location = 0;

	if (*location_index != NONE)
	{
		bool wanted;

		location = DATUM(g_4ea938, s_effect_location_datum, *location_index);
		*location_index = location->next_index;
		bool on_first_person = location->node_index != NONE && (location->node_index & 0x8000);

		switch (mode)
		{
		case 1:
			wanted = true;
			break;
		case 3:
			return location;
		default:
			wanted = false;
			break;
		}
		if (on_first_person != wanted)
			location = effect_location_next(effect, location_index, mode);
	}
	return location;
}

// @retail 0x17aec0
void function_17aec0(real_matrix4x3 *matrix, s_effect_datum *effect, short node_index)
{
	if (node_index != NONE && (node_index & 0x8000) && effect->unknown58 != NONE)
	{
		function_1664da(effect->unknown58, effect->object_index, node_index & 0x7fff, matrix);
	}
	else
	{
		short index = node_index == NONE ? NONE : (short)(node_index & 0x7fff);
		s_effect_object *object = OBJECT_GET(effect->object_index);

		*matrix = ((real_matrix4x3 *)((byte *)object + object->nodes_offset))[index];
	}
}

// @retail 0x17af30
void function_17af30(real_matrix4x3 *matrix, s_effect_datum *effect, bool first_person, short node_index)
{
	if (first_person && node_index != NONE && (node_index & 0x8000) && effect->unknown58 != NONE)
		*matrix = *function_1664a5(effect->unknown58, effect->object_index, node_index & 0x7fff);
	else
		function_17aec0(matrix, effect, node_index);
}

// @retail 0x17af80
void function_17af80(long effect_index, long particle_system_index)
{
	s_effect_datum *effect = DATUM(g_4ea93c, s_effect_datum, effect_index);
	s_particle_system_datum *particle_system = DATUM(g_510c74, s_particle_system_datum, particle_system_index);

	particle_system_unlink(particle_system, &effect->first_particle_system_index, &effect->last_particle_system_index);
	effect_remove_event_slot(effect_index, particle_system_index);
}

// @retail 0x17afd0
bool function_17afd0(long effect_index, long value)
{
	bool result = false;
	s_effect_object *object = (s_effect_object *)function_badc0(DATUM(g_4ea93c, s_effect_datum, effect_index)->object_index, 3);

	if (object && object->player_index != NONE)
		return DATUM(g_4e8c24, s_effect_player, object->player_index)->unknown28 == value;
	return result;
}

// @retail 0x17b270
bool function_17b270(short const *values, s_effect_datum *effect, long mode)
{
	bool result = TEST_FIELD_BIT(effect->flag5) ? values[8] != 1 : values[8] != 2;

	if (*(short *)&g_4e8c20->unknown00[8] == 1 && result)
	{
		short value = values[9];

		if ((value != 2 || mode == 0) && (value != 1 || mode == 1))
			return true;
		return false;
	}
	return result;
}

// @retail 0x17b750
void function_17b750(long *values, long value)
{
	for (long i = 0; i < 32; i++)
		values[i] = value;
}
