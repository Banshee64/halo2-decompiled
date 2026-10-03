// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_175BD0.CPP: the effects (entry 40 of the lifecycle table) */

#include "cseries.h"
#include "globals.h"
#include "data_array.h"
#include "effects.h"
#include "unknown_1765e0.h"
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
	dword : 8;
	dword flag4_8 : 1;
	dword : 23;
	byte unknown08[0x28 - 8];
	s_location location;
	byte unknown30[0x64 - 0x30];
	real_point3d position;
	byte unknown70[0x88 - 0x70];
	real_vector3d velocity;
	byte unknown94[0x10a - 0x94];
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

/* a marker of an object (0x70 bytes; function_b8d30) */
struct s_effect_object_marker
{
	short node_index;
	byte unknown02[2];
	real_matrix4x3 matrix;
	real_matrix4x3 unknown38;
	byte unknown6c[4];
};

struct s_object_marker;
short function_b8d30(bool flag, long object_index, long marker_name, short count, s_object_marker *markers);
short __stdcall function_1662c1(long group_index, long name, s_effect_object_marker *markers, short count);

/* the zones of the structure bsp (function_11c120) */
struct s_effect_zone_cluster
{
	byte unknown00[0x70];
	byte zone;
	byte unknown71[0xb0 - 0x71];
};

struct s_effect_zone
{
	byte unknown00[2];
	short index;
	real_plane3d plane;
	byte unknown14[4];
};

struct s_effect_zone_bsp
{
	byte unknown00[0x68];
	s_effect_zone *zones;
	byte unknown6c[0xa0 - 0x6c];
	s_effect_zone_cluster *clusters;
};

/* a color query of the effects (function_17b5d0) */
struct s_effect_color_query
{
	real unknown00;
	real unknown04;
	real unknown08;
	dword color_a;
	dword color_b;
};

long function_d2bb0(void *source, s_effect_color_query *query);
long function_d2a50(long a, long b, long c, s_effect_color_query *query, long d, real_point3d const *point);
bool function_11c050(s_location const *location);
bool function_11c080(s_location const *location);
bool function_11c120(s_location const *location, real_point3d const *point, short *zone_index);
bool function_3eb20(long cluster_index);
bool __stdcall function_3ebd0(real_vector3d const *offset, real_matrix4x3 const *matrices, real_matrix4x3 *out, long count);
long __stdcall function_3ddd0(long object_index);
bool __stdcall function_bab40(long object_index, long name, real *value);
bool function_bad50(long object_index, long index, real_point3d *out);
long function_baf80(long object_index);
long function_155760(long index);
bool function_163080(void);
long function_166244(long key);
bool function_166283(long group_index, long key);
s_player_state *function_16f3a0(long index);
real_vector3d *function_11d000(real_vector3d const *v, real_vector3d *out);
void function_179fb0(s_effect_datum *effect);
void __stdcall function_17a380(s_effect_datum *effect);
extern s_data_array *g_509434;
struct s_unknown_13bf00;
extern s_unknown_13bf00 *g_510c50;

/* the clusters whose effects are visible */
dword g_4c56c0[64];

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

struct s_bsp3d;
extern s_bsp3d *g_4e033c;
long function_14a280(s_bsp3d *bsp, real_point3d *point, long index);

struct s_effect_structure_leaf
{
	short cluster_index;
	byte unknown02[6];
};

struct s_effect_structure_bsp
{
	byte unknown00[0x30];
	s_effect_structure_leaf *leaves;
};

/* the leaf and cluster of the structure bsp a point is in */
static inline void effect_location_from_point(s_location *location, real_point3d *point)
{
	short bsp_index = g_4686c4;

	if (bsp_index == NONE)
	{
		location->bsp_index = bsp_index;
		location->leaf_index = NONE;
		location->cluster_index = NONE;
	}
	else
	{
		long leaf_index = function_14a280(g_4e033c, point, 0);

		location->leaf_index = leaf_index;
		long cluster_index = leaf_index != NONE ? ((s_effect_structure_bsp *)g_4e0348)->leaves[leaf_index].cluster_index : NONE;
		location->cluster_index = (short)cluster_index;
		location->bsp_index = bsp_index;
	}
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

long effect_new_from_parameters(s_effect_parameters *parameters);
bool function_176210(s_effect_parameters *parameters);
long effect_new(long tag_index, s_effect_owner const *owner, bool force);
void function_178240(real_point3d const *origin, real_vector3d const *direction, s_effect_datum *effect, real scale_a, real scale_b);
struct s_effect_marker_source;
void function_1786f0(s_effect_marker_source const *source, s_effect_object_marker *out, short marker_index);
long function_1785c0(s_effect_object_marker const *marker, s_effect_datum *effect, long location_index, bool first_person);
real_matrix4x3 *function_178bc0(s_effect_location_datum *location, s_effect_datum *effect, real_matrix4x3 *matrix, bool first_person);
void function_178c80(real scale, s_effect_datum *effect, s_particle_system_datum *particle_system, s_effect_particle_system_definition *definition, real unknown, bool first_person);
bool function_178020(short placement, real_point3d const *point, s_location const *location);
bool function_17b270(short const *values, s_effect_datum *effect, long mode);
void function_17af30(real_matrix4x3 *matrix, s_effect_datum *effect, bool first_person, short node_index);
void function_177260(long effect_index, bool flag);
bool function_1794a0(long object_index, long other_object_index);
void function_179020(long effect_index, real scale, real unknown);
long function_179190(s_effect_datum *effect);
bool function_1792b0(long effect_index, real dt);
bool __stdcall function_1794e0(long effect_index);
bool function_179730(long effect_index);
bool function_179810(long effect_index);
bool function_175f50(long object_index);
void function_17b750(long *values, long value);
void function_178360(long effect_index, short unknown18, long object_index, long unknown58, s_effect_marker *markers, long marker_count);
bool function_1789f0(s_effect_datum *effect);
void function_17b5d0(s_effect_datum *effect, long object_index, s_effect_parameters *parameters, bool search);
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
		index = data_next_absolute_index_inlined(array, index + 1);
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
	s_effect_object *object = (s_effect_object *)function_badc0(object_index, 1);
	bool result = false;

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
void function_1763a0(real_point3d const *point, real_vector3d const *direction, s_effect_marker *markers, real_vector3d const *normal)
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
	s_effect_location_datum *location = 0;

	for (long i = 0; i < definition->location_count; i++)
	{
		long location_index = effect->location_indices[i];

		if (location_index != NONE)
		{
			location = effect_location_next(effect, &location_index, 0);
			break;
		}
	}
	if (location)
	{
		effect_location_from_point(&effect->location, &location->matrix.position);
	}
	else
	{
		effect->location.bsp_index = g_4686c4;
		effect->location.leaf_index = NONE;
		effect->location.cluster_index = NONE;
	}
	return effect->location.cluster_index != NONE;
}

// @retail 0x178ad0
void function_178ad0(s_effect_datum *effect)
{
	s_effect_event_slot *slot = effect->event_slots;

	for (long i = 16; i; i--, slot++)
	{
		slot->unknown0 = 0;
		slot->unknown4 = NONE;
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

		if (particle_system->event_index == effect->event_index)
		{
			real delay = effect->event_delay;

			particle_system->flag0 = true;
			particle_system->unknown04 = 0.0f;
			if (delay > 0.0f)
				particle_system->unknown08 = 1.0f / delay;
			else
				particle_system->unknown08 = 1.0f;
		}
		else if (particle_system->event_index == effect->unknown5e)
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

	if (object)
	{
		long player_index = object->player_index;

		if (player_index != NONE)
			result = DATUM(g_4e8c24, s_effect_player, player_index)->unknown28 == value;
	}
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

// @retail 0x175fa0
long effect_new_from_parameters(s_effect_parameters *parameters)
{
	bool force = TEST_FIELD_BIT(parameters->flag2);
	long effect_index = NONE;

	if (force || function_176210(parameters))
	{
		effect_index = effect_new(parameters->tag_index, &parameters->owner, force);
		if (effect_index != NONE)
		{
			s_effect_datum *effect = DATUM(g_4ea93c, s_effect_datum, effect_index);
			s_effect_definition *definition = TAG_GET(s_effect_definition, effect->tag_index);

			effect->unknown0c = parameters->unknown30;
			if (g_4e6948->state == 2 && function_163080() && parameters->object_index != NONE)
			{
				s_effect_object *object = OBJECT_GET(parameters->object_index);

				effect->flag10 = TEST_FIELD_BIT(TAG_GET(s_effect_object_definition, object->tag_index)->flag13);
			}
			if (TEST_FIELD_BIT(parameters->attached))
			{
				long object_index = parameters->object_index;
				long group_index = function_166244(object_index);

				if (function_badc0(object_index, NONE))
					effect->object_index = object_index;
				if (group_index != NONE && function_166283(group_index, parameters->object_index))
					effect->unknown58 = group_index;
				effect->flag7 = true;
			}
			else
			{
				effect->object_index = NONE;
				effect->velocity = parameters->velocity;
			}
			if (TEST_FIELD_BIT(parameters->flag1))
			{
				effect->flag1 = true;
				effect->unknown10 = parameters->unknown34;
				effect->unknown14 = parameters->unknown38;
				effect->unknown18 = parameters->unknown3c;
				effect->unknown40 = 0;
				effect->unknown44 = 0;
			}
			else
			{
				function_178240(parameters->origin, parameters->direction, effect, parameters->scale_a, parameters->scale_b);
			}
			if (effect->object_index != NONE && g_510c7c && function_175f50(effect->object_index))
				effect->flag5 = true;
			function_17b750(effect->location_indices, NONE);
			function_178360(effect_index, parameters->unknown18, effect->object_index, effect->unknown58, parameters->markers, parameters->marker_count);
			if (!TEST_FIELD_BIT(parameters->attached))
				function_1789f0(effect);
			if (TEST_FIELD_BIT(definition->flag3) || TEST_FIELD_BIT(definition->flag4))
				function_17b5d0(effect, parameters->object_index, parameters, true);
			if (parameters->unknown50)
			{
				*(long *)&effect->unknown74 = parameters->unknown50[0];
				*(long *)&effect->unknown78 = parameters->unknown50[1];
			}
			function_177310(effect_index);
			g_510c78 = parameters->source;
			function_179850(effect_index, (TEST_FIELD_BIT(definition->flag1) && !TEST_FIELD_BIT(effect->flag1) && !TEST_FIELD_BIT(effect->flag9)) ? 1.0f : 0.0f);
			g_510c78 = 0;
			if (!datum_get(g_4ea93c, effect_index))
				return NONE;
		}
	}
	return effect_index;
}

// @retail 0x176210
bool function_176210(s_effect_parameters *parameters)
{
	bool result = true;

	if (parameters->tag_index != NONE)
	{
		s_effect_source *source = parameters->source;

		if (source && source->index != NONE && !(g_510c50 && ((byte *)g_510c50)[5]))
		{
			s_effect_definition *definition = TAG_GET(s_effect_definition, parameters->tag_index);
			real lower = definition->distance_lower;
			real upper = definition->distance_upper;

			if (lower != 0.0f && upper != 0.0f)
			{
				real minimum = 3.4028235e38f;

				for (long i = 0; i < 4; i++)
				{
					s_player_state *player = function_16f3a0(i);

					if (player)
					{
						real dx = source->position.x - player->position.x;
						real dy = source->position.y - player->position.y;
						real dz = source->position.z - player->position.z;
						real distance_squared = dx * dx + dy * dy + dz * dz;

						if (minimum > distance_squared)
							minimum = distance_squared;
					}
				}
				if (!(lower * lower > minimum))
				{
					if (minimum > upper * upper)
						result = false;
					else
						result = (g_4c56c0[source->index >> 5] & (1 << (source->index & 31))) != 0;
				}
			}
		}
	}
	return result;
}

// @retail 0x1765e0
long function_1765e0(real_point3d const *point, real_vector3d const *direction, real_vector3d const *normal, long tag_index, long mode, long deterministic)
{
	if (tag_index != NONE && point)
	{
		s_effect_marker markers[7];
		s_effect_parameters parameters;
		long marker_count = 6;

		function_1763a0(point, direction, markers, normal);
		if (mode == 1)
		{
			markers[6].position = *point;
			markers[6].forward = *direction;
			markers[6].name = 0x30000d9;
			marker_count = 7;
		}
		parameters.flags = 0;
		function_176a50(&parameters, tag_index, marker_count, markers, deterministic);
		effect_new_from_parameters(&parameters);
	}
	return NONE;
}

// @retail 0x1766b0
void function_1766b0(long object_index, long tag_index, long unknown34, long unknown38, short unknown3c)
{
	s_effect_parameters parameters;
	real_point3d point;
	s_effect_marker markers[2];

	effect_parameters_initialize_inline(&parameters);
	parameters.tag_index = tag_index;
	parameters.object_index = object_index;
	parameters.attached = true;
	parameters.flag1 = true;
	parameters.flag2 = true;
	parameters.unknown34 = unknown34;
	parameters.unknown38 = unknown38;
	parameters.unknown3c = unknown3c;
	function_b9dd0(object_index, &point);
	parameters.markers = function_176330(markers, &point);
	parameters.marker_count = 2;
	effect_new_from_parameters(&parameters);
}

// @retail 0x176780
void function_176780(long object_index, real_vector3d const *velocity, real scale_a, long tag_index, real scale_b, real_point3d const *origin, real_vector3d const *direction)
{
	s_effect_parameters parameters;
	real_point3d point;
	s_effect_marker markers[2];

	effect_parameters_initialize_inline(&parameters);
	parameters.tag_index = tag_index;
	parameters.object_index = object_index;
	parameters.attached = true;
	parameters.flag2 = true;
	parameters.scale_a = scale_a;
	parameters.scale_b = scale_b;
	parameters.origin = origin;
	parameters.direction = direction;
	function_b9dd0(object_index, &point);
	parameters.markers = function_176330(markers, &point);
	parameters.marker_count = 2;
	if (velocity)
		parameters.velocity = *velocity;
	effect_new_from_parameters(&parameters);
}

// @retail 0x176870
void function_176870(long object_index, s_effect_owner const *owner, long marker_name, real scale_a, long tag_index, short unknown18, real scale_b, real_point3d const *origin, real_vector3d const *direction)
{
	s_effect_parameters parameters;
	s_effect_marker markers[2];
	s_effect_object_marker object_markers[1];

	effect_parameters_initialize_inline(&parameters);
	parameters.tag_index = tag_index;
	parameters.attached = true;
	parameters.flag2 = true;
	if (owner)
		parameters.owner = *owner;
	parameters.object_index = object_index;
	parameters.unknown18 = unknown18;
	parameters.unknown30 = marker_name;
	parameters.scale_a = scale_a;
	parameters.scale_b = scale_b;
	parameters.origin = origin;
	parameters.direction = direction;
	function_b8d30(false, object_index, marker_name, 1, (s_object_marker *)object_markers);
	parameters.markers = function_176330(markers, &object_markers[0].unknown38.position);
	parameters.marker_count = 2;
	effect_new_from_parameters(&parameters);
}

// @retail 0x176970
void function_176970(s_effect_owner const *owner, real scale_a, long tag_index, long object_index, short unknown18, short marker_count, s_effect_marker *markers, real scale_b, real_point3d const *origin, real_vector3d const *direction, bool flag1)
{
	s_effect_parameters parameters;

	effect_parameters_initialize_inline(&parameters);
	parameters.flags = 5;
	parameters.tag_index = tag_index;
	if (owner)
		parameters.owner = *owner;
	parameters.object_index = object_index;
	parameters.unknown18 = unknown18;
	parameters.marker_count = marker_count;
	parameters.markers = markers;
	parameters.scale_a = scale_a;
	parameters.scale_b = scale_b;
	parameters.origin = origin;
	parameters.direction = direction;
	if (flag1)
		parameters.flags = 7;
	effect_new_from_parameters(&parameters);
}

// @retail 0x176ad0
void function_176ad0(long marker_count, s_effect_marker *markers, s_effect_owner const *owner, real_vector3d const *velocity, long tag_index, long unknown30, real scale_a, real scale_b, real_point3d const *origin, real_vector3d const *direction, long mode)
{
	s_effect_parameters parameters;

	parameters.flags = 0;
	function_176a50(&parameters, tag_index, marker_count, markers, mode);
	if (owner)
		parameters.owner = *owner;
	if (velocity)
		parameters.velocity = *velocity;
	parameters.origin = origin;
	parameters.scale_a = scale_a;
	parameters.scale_b = scale_b;
	parameters.direction = direction;
	parameters.unknown30 = unknown30;
	effect_new_from_parameters(&parameters);
}

// @retail 0x176b60
void function_176b60(long marker_count, s_effect_marker *markers, long mode, s_effect_owner const *owner, long tag_index)
{
	s_effect_parameters parameters;

	parameters.flags = 0;
	function_176a50(&parameters, tag_index, marker_count, markers, mode);
	if (owner)
		parameters.owner = *owner;
	effect_new_from_parameters(&parameters);
}


// @retail 0x178020
bool function_178020(short placement, real_point3d const *point, s_location const *location)
{
	switch (placement)
	{
	case 0:
		return true;
	case 1:
		return !function_11c120(location, point, 0);
	case 2:
		return function_11c120(location, point, 0);
	case 3:
		return false;
	default:
		__assume(0);
	}
}

/* the markers an effect is started at (function_178360) */
struct s_effect_marker_source
{
	short node_index;
	real_matrix4x3 *node_matrix;
	long marker_count;
	s_effect_marker *markers;
};

static inline long effect_marker_find(s_effect_marker const *markers, long marker_count, dword name)
{
	long result = NONE;

	for (long i = 0; i < marker_count; i++)
	{
		if (markers[i].name == name)
		{
			result = i;
			break;
		}
	}
	return result;
}

// @retail 0x178360
void function_178360(long effect_index, short unknown18, long object_index, long unknown58, s_effect_marker *markers, long marker_count)
{
	s_effect_datum *effect = DATUM(g_4ea93c, s_effect_datum, effect_index);
	long default_marker_index = effect_marker_find(markers, marker_count, 0x30000d9);
	s_effect_definition *definition = TAG_GET(s_effect_definition, effect->tag_index);
	s_effect_marker_source source;
	s_effect_object_marker object_markers[16];

	function_1773a0(effect_index);
	source.marker_count = marker_count;
	source.markers = markers;
	if (object_index != NONE)
	{
		s_effect_object *object = OBJECT_GET(object_index);

		source.node_index = unknown18 == NONE ? 0 : unknown18;
		source.node_matrix = &((real_matrix4x3 *)((byte *)object + object->nodes_offset))[source.node_index];
	}
	else
	{
		source.node_index = NONE;
		source.node_matrix = 0;
	}
	for (long location_index = 0; location_index < definition->location_count; location_index++)
	{
		dword name = definition->locations[location_index];
		long count = 0;
		dword first_person_mask = 0;

		if (name == 0x700054e)
			name = effect->unknown0c;
		if (name)
		{
			long marker_index = NONE;

			if (marker_count > 0)
				marker_index = effect_marker_find(markers, marker_count, name);
			if (marker_index != NONE)
			{
				function_1786f0(&source, object_markers, (short)marker_index);
				count = 1;
			}
			else
			{
				if (object_index != NONE)
				{
					count = function_b8d30(false, object_index, name, 16, (s_object_marker *)object_markers);
					if (count == 0 && name == 0x400054f)
						count = 1;
					if (unknown58 != NONE)
					{
						short first_person_count = function_1662c1(unknown58, name, &object_markers[count], (short)(16 - count));

						for (long i = 0; i < first_person_count; i++)
							first_person_mask |= 1 << (i + count);
						count += first_person_count;
					}
				}
				if (count == 0 && default_marker_index != NONE)
				{
					function_1786f0(&source, object_markers, (short)default_marker_index);
					count = 1;
				}
			}
		}
		for (long i = 0; i < count; i++)
		{
			if (function_1785c0(&object_markers[i], effect, location_index, (first_person_mask & (1 << i)) != 0) == NONE)
				break;
		}
	}
}

/* the last time effect locations ran out */
long g_47ff8c;

// @retail 0x1785c0
long function_1785c0(s_effect_object_marker const *marker, s_effect_datum *effect, long location_index, bool first_person)
{
	s_data_array *locations = g_4ea938;
	long index = datum_new(locations);

	if (index != NONE)
	{
		s_effect_location_datum *location = DATUM(locations, s_effect_location_datum, index);
		short node_index = marker->node_index;

		location->node_index = node_index == NONE ? NONE : (short)((node_index & 0x7fff) | (first_person ? 0x8000 : 0));
		location->matrix = marker->matrix;
		if (function_3eb20(effect->location.cluster_index) && effect->object_index != NONE)
		{
			s_effect_object *object = OBJECT_GET(effect->object_index);
			real_vector3d offset;

			offset.i = g_468788->x - object->position.x;
			offset.j = g_468788->y - object->position.y;
			offset.k = g_468788->z - object->position.z;
			function_3ebd0(&offset, &location->matrix, &location->matrix, 1);
		}
		location->next_index = effect->location_indices[location_index];
		effect->location_indices[location_index] = index;
	}
	else
	{
		long game_time = g_510c54->game_time;

		if (game_time - g_47ff8c > g_510c54->ticks_per_second * 60)
			g_47ff8c = game_time;
	}
	return index;
}

// @retail 0x1786f0
void function_1786f0(s_effect_marker_source const *source, s_effect_object_marker *out, short marker_index)
{
	real_matrix4x3 const *matrix = source->node_matrix;
	real_point3d position;
	real_vector3d forward;
	real_vector3d up;

	out->node_index = source->node_index;
	if (matrix)
	{
		s_effect_marker const *marker = &source->markers[marker_index];

		if (matrix->scale != 0.0f)
		{
			real_vector3d offset;

			offset.i = marker->position.x - matrix->position.x;
			offset.j = marker->position.y - matrix->position.y;
			offset.k = marker->position.z - matrix->position.z;
			if (matrix->scale != 1.0f)
			{
				real inverse = 1.0f / matrix->scale;

				offset.i = inverse * offset.i;
				offset.j = inverse * offset.j;
				offset.k = inverse * offset.k;
			}
			up.i = matrix->forward.k * offset.k + matrix->forward.j * offset.j + matrix->forward.i * offset.i;
			up.j = matrix->left.k * offset.k + matrix->left.j * offset.j + matrix->left.i * offset.i;
			up.k = matrix->up.k * offset.k + matrix->up.j * offset.j + matrix->up.i * offset.i;
		}
		else
		{
			up.i = 0.0f;
			up.j = 0.0f;
			up.k = 0.0f;
		}
		position.x = up.i;
		position.y = up.j;
		position.z = up.k;
		forward.i = matrix->forward.k * marker->forward.k + matrix->forward.j * marker->forward.j + matrix->forward.i * marker->forward.i;
		forward.j = matrix->left.k * marker->forward.k + matrix->left.j * marker->forward.j + matrix->left.i * marker->forward.i;
		forward.k = matrix->up.k * marker->forward.k + matrix->up.j * marker->forward.j + matrix->up.i * marker->forward.i;
	}
	else
	{
		s_effect_marker const *marker = &source->markers[marker_index];

		position = marker->position;
		forward = marker->forward;
	}
	function_11d000(&forward, &up);

	real magnitude = (real)sqrt(up.k * up.k + up.j * up.j + up.i * up.i);
	if (!(fabs(magnitude) < 0.0001f))
	{
		real inverse = 1.0f / magnitude;

		up.i = inverse * up.i;
		up.j = up.j * inverse;
		up.k = up.k * inverse;
	}
	out->matrix.scale = 1.0f;
	out->matrix.forward = forward;
	out->matrix.left.i = up.j * forward.k - up.k * forward.j;
	out->matrix.left.j = up.k * forward.i - forward.k * up.i;
	out->matrix.left.k = forward.j * up.i - up.j * forward.i;
	out->matrix.up = up;
	out->matrix.position.x = 0.0f;
	out->matrix.position.y = 0.0f;
	out->matrix.position.z = 0.0f;
	out->matrix.position = position;
}

// @retail 0x178bc0
real_matrix4x3 *function_178bc0(s_effect_location_datum *location, s_effect_datum *effect, real_matrix4x3 *matrix, bool first_person)
{
	if ((word)location->node_index != 0xffff && effect->object_index != NONE)
	{
		real_matrix4x3 node_matrix;

		function_17af30(&node_matrix, effect, first_person, (word)location->node_index);
		function_142a60(&node_matrix, &location->matrix, matrix);
		if (function_3eb20(effect->location.cluster_index) && effect->object_index != NONE)
		{
			s_effect_object *object = OBJECT_GET(effect->object_index);
			real_vector3d offset;

			offset.i = g_468788->x - object->position.x;
			offset.j = g_468788->y - object->position.y;
			offset.k = g_468788->z - object->position.z;
			function_3ebd0(&offset, matrix, matrix, 1);
		}
		return matrix;
	}
	return &location->matrix;
}

/* whether a point is inside the zone of a cluster (an inline copy of
   function_11c120) */
static inline bool effect_location_in_zone(s_location const *location, real_point3d const *point)
{
	bool result = false;
	short cluster_index = location->cluster_index;

	if (cluster_index != NONE)
	{
		s_effect_zone_bsp *bsp = (s_effect_zone_bsp *)g_4e0348;
		byte zone = bsp->clusters[cluster_index].zone;

		if (zone != 0xff)
		{
			s_effect_zone *entry = &bsp->zones[zone & 0x7f];

			if (entry->index != NONE)
			{
				if (!(zone & 0x80) || 0.0f > entry->plane.k * point->z + entry->plane.j * point->y + entry->plane.i * point->x - entry->plane.d)
					result = true;
			}
		}
	}
	return result;
}

// @retail 0x178c80
void function_178c80(real scale, s_effect_datum *effect, s_particle_system_datum *particle_system, s_effect_particle_system_definition *definition, real unknown, bool first_person)
{
	long location_index = effect->location_indices[definition->location_index];
	s_effect_location_datum *location;
	s_particle_system_spawn spawn;

	if (effect->location.cluster_index != NONE)
		particle_system->set_location(&effect->location);
	dword color_a = effect->color_a;

	spawn.scale = scale;
	spawn.unknown = unknown;
	spawn.location_index = particle_system->location_index;
	{
		c_particle_system *particle_definition = function_137bd0(particle_system->get_definition()->tag_index);
		bool tinted = particle_definition->tinted();
		bool multiplied = particle_definition->multiplied();

		function_175a80(tinted, color_a, effect->color_b, particle_system, multiplied);
	}
	while ((location = effect_location_next(effect, &location_index, definition->location_mode)) != 0)
	{
		bool spawn_here;

		switch (definition->placement)
		{
		case 0:
			spawn_here = true;
			break;
		case 1:
			spawn_here = !effect_location_in_zone(&effect->location, &location->matrix.position);
			break;
		case 2:
			spawn_here = effect_location_in_zone(&effect->location, &location->matrix.position);
			break;
		case 3:
			spawn_here = false;
			break;
		default:
			__assume(0);
		}
		if (spawn_here)
		{
			real_matrix4x3 matrix;
			real_matrix4x3 *location_matrix = function_178bc0(location, effect, &matrix, first_person);
			bool node_first_person = location->node_index != NONE && (location->node_index & 0x8000);

			function_175270(particle_system, &spawn, location_matrix, node_first_person);
		}
	}
}


// @retail 0x178eb0
s_particle_system_datum *function_178eb0(short definition_index, long effect_index)
{
	s_effect_datum *effect = DATUM(g_4ea93c, s_effect_datum, effect_index);
	s_effect_event *event = &TAG_GET(s_effect_definition, effect->tag_index)->events[effect->event_index];
	s_effect_particle_system_definition *definition = &event->particle_systems[definition_index];
	long mode = 0;

	if (effect->unknown58 != NONE && !function_155760(effect->unknown58))
		mode = 1;
	if (function_17b270(&definition->unknown10, effect, mode))
	{
		long location_index = definition->location_index;

		if (location_index != NONE)
		{
			long first_location_index = effect->location_indices[location_index];
			s_effect_location_datum *location = effect_location_next(effect, &first_location_index, definition->location_mode);

			if (location && function_178020(definition->placement, &location->matrix.position, &effect->location))
			{
				long particle_system_index = function_173fd0(definition, effect_index, effect->tag_index, definition_index, effect->event_index);

				if (particle_system_index != NONE)
				{
					s_particle_system_datum *particle_system = DATUM(g_510c74, s_particle_system_datum, particle_system_index);
					real delay = effect->event_delay;

					particle_system->flag0 = true;
					particle_system->unknown04 = 0.0f;
					if (delay > 0.0f)
						particle_system->unknown08 = 1.0f / delay;
					else
						particle_system->unknown08 = 1.0f;
					particle_system_link(particle_system, &effect->last_particle_system_index, &effect->first_particle_system_index);
					particle_system->unknown24 = location_index;
					function_178c80(0.0f, effect, particle_system, definition, 0.0f, definition->unknown0c != 0);
					return particle_system;
				}
			}
		}
	}
	return 0;
}

// @retail 0x179020
void function_179020(long effect_index, real scale, real unknown)
{
	s_effect_datum *effect = DATUM(g_4ea93c, s_effect_datum, effect_index);
	s_effect_event *event = &TAG_GET(s_effect_definition, effect->tag_index)->events[effect->event_index];
	long mode = 0;

	if (effect->unknown58 != NONE && !function_155760(effect->unknown58))
		mode = 1;
	for (long i = 0; i < event->particle_system_count; i++)
	{
		s_effect_particle_system_definition *definition = &event->particle_systems[i];
		bool first_person = definition->unknown0c != 0;

		if (function_17b270(&definition->unknown10, effect, mode))
		{
			s_particle_system_datum *particle_system = 0;

			for (dword j = 0; j < 16; j++)
			{
				if ((s_effect_particle_system_definition *)effect->event_slots[j].unknown0 == definition)
				{
					particle_system = DATUM(g_510c74, s_particle_system_datum, effect->event_slots[j].unknown4);
					break;
				}
			}
			if (particle_system || (particle_system = function_178eb0((short)i, effect_index)) != 0)
			{
				if (definition->location_index != NONE)
					function_178c80(scale, effect, particle_system, definition, unknown, first_person);
			}
		}
	}
}

// @retail 0x179190
long function_179190(s_effect_datum *effect)
{
	s_effect_definition *definition = TAG_GET(s_effect_definition, effect->tag_index);
	long restart_index = definition->restart_event_index;
	long last_index = definition->event_count - 1;
	long event_index = effect->event_index;

	if ((TEST_FIELD_BIT(effect->flag1) || TEST_FIELD_BIT(effect->flag9)) && restart_index == NONE)
		restart_index = 0;
	for (;;)
	{
		if ((TEST_FIELD_BIT(effect->flag1) || TEST_FIELD_BIT(effect->flag9)) && event_index == last_index && restart_index != NONE)
			event_index = restart_index;
		else
			event_index++;
		if (event_index >= definition->event_count)
			break;
		real skip_chance = definition->events[event_index].skip_chance;

		if (!(skip_chance > 0.0f))
			break;
		if (!(skip_chance > _real_random(function_177c20(effect->tag_index), __FILE__, __LINE__)))
			break;
	}
	return event_index;
}

// @retail 0x1792b0
bool function_1792b0(long effect_index, real dt)
{
	s_effect_datum *effect = DATUM(g_4ea93c, s_effect_datum, effect_index);
	s_effect_definition *definition = TAG_GET(s_effect_definition, effect->tag_index);

	if (!TEST_FIELD_BIT(effect->flag6))
	{
		long iterations = 0;

		while (dt >= 0.0f && !TEST_FIELD_BIT(effect->flag2) && iterations < 8)
		{
			real remaining = effect->event_delay - effect->unknown60;
			real start = effect->unknown60;
			real step = dt > remaining ? remaining : dt;
			bool finished;

			if (dt >= remaining)
			{
				effect->unknown60 = effect->event_delay;
				finished = true;
				dt -= remaining;
			}
			else
			{
				effect->unknown60 += dt;
				finished = false;
				dt = -1.0f;
			}
			if (TEST_FIELD_BIT(effect->flag0))
			{
				if (!TEST_FIELD_BIT(effect->flag3))
					function_179020(effect_index, start, step);
				if (!TEST_FIELD_BIT(effect->flag3))
					function_179fb0(effect);
				if (finished)
				{
					long event_index = function_179190(effect);

					if (event_index >= definition->event_count)
					{
						if (function_177610(effect_index))
							return true;
						break;
					}
					function_1782a0(effect_index, (short)event_index);
				}
			}
			else if (finished)
			{
				s_effect_event *event = &definition->events[effect->event_index];

				effect->flag0 = true;
				effect->unknown60 = 0.0f;
				effect->unknown68 = -1.0f;
				if (event->duration_lower == event->duration_upper)
					effect->event_delay = event->duration_lower;
				else
					effect->event_delay = _real_random_range(function_177c20(effect->tag_index), __FILE__, __LINE__, event->duration_lower, event->duration_upper);
				if (!TEST_FIELD_BIT(effect->flag3))
					function_17a380(effect);
				function_17add0(effect);
				effect->unknown5e = effect->event_index;
			}
			iterations++;
		}
		effect->flag9 = false;
	}
	return effect == 0;
}

// @retail 0x1794e0
bool __stdcall function_1794e0(long effect_index)
{
	s_effect_datum *effect = DATUM(g_4ea93c, s_effect_datum, effect_index);
	s_effect_definition *definition = TAG_GET(s_effect_definition, effect->tag_index);

	if (effect->object_index != NONE && !function_badc0(effect->object_index, NONE))
	{
		effect_delete(effect_index);
		effect = 0;
	}
	else if (effect->object_index != NONE)
	{
		s_effect_object *root = OBJECT_GET(function_baf80(effect->object_index));

		if (TEST_FIELD_BIT(root->flag4_8))
		{
			effect->velocity = root->velocity;
			effect->location = root->location;
		}
		else
		{
			effect->location.leaf_index = NONE;
			effect->location.cluster_index = NONE;
			effect->location.bsp_index = g_4686c4;
		}
		if (TEST_FIELD_BIT(effect->flag1) || TEST_FIELD_BIT(effect->flag9))
		{
			bool visible;

			if (function_1794a0(effect->object_index, effect->object_index))
			{
				visible = !effect->unknown10 || function_bab40(effect->object_index, effect->unknown10, &effect->scale_a);
				function_bab40(effect->object_index, effect->unknown14, &effect->scale_b);
				if (effect->unknown18)
					function_bad50(effect->object_index, effect->unknown18, &effect->origin);
			}
			else
			{
				effect->scale_a = 0.0f;
				effect->scale_b = 0.0f;
				if (effect->unknown18)
					effect->origin = *g_468710;
				visible = false;
			}
			if (visible)
			{
				if (TEST_FIELD_BIT(effect->flag2))
				{
					if (!TEST_FIELD_BIT(effect->flag4))
						function_177310(effect_index);
					else if (function_177610(effect_index))
						return true;
				}
			}
			else if (TEST_FIELD_BIT(definition->flag0))
			{
				if (function_177610(effect_index))
					return true;
			}
			else if (!TEST_FIELD_BIT(effect->flag2))
			{
				function_177260(effect_index, false);
			}
		}
		if (TEST_FIELD_BIT(definition->flag3) || TEST_FIELD_BIT(definition->flag4))
		{
			long object_index = effect->object_index;

			if (function_badc0(object_index, NONE))
			{
				long index = function_3ddd0(function_baf80(object_index));

				if (index != NONE)
				{
					byte *datum = g_509434->data + (index & 0xffff) * 0x100;

					if (datum[2])
					{
						effect->color_a = *(dword *)(datum + 0x2c);
						effect->color_b = *(dword *)(datum + 0x28);
					}
				}
			}
		}
	}
	return effect == 0;
}

// @retail 0x179730
bool function_179730(long effect_index)
{
	s_effect_datum *effect = DATUM(g_4ea93c, s_effect_datum, effect_index);
	s_effect_definition *definition = TAG_GET(s_effect_definition, effect->tag_index);
	bool hidden = true;

	if (effect->location.cluster_index != NONE)
	{
		if (function_3eb20(effect->location.cluster_index) && effect->object_index != NONE)
			hidden = false;
		else if (g_510c50 && ((byte *)g_510c50)[5])
			hidden = false;
		else if (TEST_FIELD_BIT(definition->flag2))
			hidden = !function_11c080(&effect->location);
		else
			hidden = !function_11c050(&effect->location);
	}
	if (hidden != TEST_FIELD_BIT(effect->flag3))
	{
		if (hidden)
		{
			effect->flag3 = true;
			if (!TEST_FIELD_BIT(effect->flag1) && !TEST_FIELD_BIT(effect->flag9) && function_177610(effect_index))
				effect = 0;
		}
		else
		{
			effect->flag3 = false;
		}
	}
	return effect == 0;
}

// @retail 0x179810
bool function_179810(long effect_index)
{
	s_effect_datum *effect = DATUM(g_4ea93c, s_effect_datum, effect_index);

	if (TEST_FIELD_BIT(effect->flag6) && effect->first_particle_system_index == NONE)
	{
		effect_delete(effect_index);
		effect = 0;
	}
	return effect == 0;
}

// @retail 0x179850
void function_179850(long effect_index, real dt)
{
	if (!function_1794e0(effect_index) && !function_179730(effect_index) && !function_179810(effect_index))
		function_1792b0(effect_index, dt);
}

// @retail 0x17b5d0
void function_17b5d0(s_effect_datum *effect, long object_index, s_effect_parameters *parameters, bool search)
{
	bool found = false;

	if (parameters && TEST_FIELD_BIT(parameters->colors_set))
	{
		effect->color_a = parameters->color_a;
		effect->color_b = parameters->color_b;
		found = true;
	}
	else if (function_badc0(object_index, NONE))
	{
		long index = function_3ddd0(function_baf80(object_index));

		if (index != NONE)
		{
			byte *datum = g_509434->data + (index & 0xffff) * 0x100;

			if (datum[2])
			{
				effect->color_a = *(dword *)(datum + 0x2c);
				effect->color_b = *(dword *)(datum + 0x28);
				found = true;
			}
		}
	}
	if (search && !found)
	{
		s_effect_color_query query;
		long location_index = effect->location_indices[0];

		query.unknown00 = 0.0f;
		query.unknown04 = 0.0f;
		query.unknown08 = -1.0f;
		query.color_a = 0xff404040;
		query.color_b = 0xff404040;
		if (!parameters || !parameters->source || !function_d2bb0(parameters->source, &query))
		{
			s_effect_location_datum *location = effect_location_next(effect, &location_index, 3);

			if (!location)
				goto done;
			{
				real_matrix4x3 matrix;
				real_matrix4x3 *location_matrix = function_178bc0(location, effect, &matrix, false);

				if (function_d2a50(0, NONE, 0, &query, 0, &location_matrix->position))
					goto done;
			}
		}
		found = true;
		effect->color_a = query.color_b;
		effect->color_b = query.color_a;
	}
done:
	if (parameters && !TEST_FIELD_BIT(parameters->colors_set) && found)
	{
		parameters->colors_set = true;
		parameters->color_a = effect->color_a;
		parameters->color_b = effect->color_b;
	}
}
