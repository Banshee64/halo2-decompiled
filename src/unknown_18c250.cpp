// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_18C250.CPP: the sound source callbacks built with /arch:SSE
   (the tables are in unknown_18c810.cpp) */

#include "cseries.h"
#include "globals.h"
#include "sound_sources.h"
#include "unknown_249e20.h"
#include "local_cameras.h"
#include "unknown_11a4d0.h"
#include <math.h>

/* an object, as the sound source code reads it */
struct s_sound_object_view
{
	byte unknown00[0x10a];
	word flag0 : 1;
	word flag1 : 1;
	word flag2 : 1;
	word unknown10a : 13;
	byte unknown10c[0x1da - 0x10c];
	short machine_index;
};

struct s_sound_object_header
{
	byte unknown00[3];
	byte type;
	byte unknown04[4];
	s_sound_object_view *object;
};

struct s_sound_class_flags
{
	byte unknown00[0xa];
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
	word unknown0a : 4;
};

struct s_sound_tag_class
{
	byte unknown00[2];
	char class_index;
};

struct s_unknown_5c;
s_object *function_badc0(long object_index, dword type_mask);
s_unknown_5c *function_221810(short index);
real_point3d *function_142570(real_matrix4x3 const *matrix, real_point3d const *point, real_point3d *out);
dword vector3d_compress(real_vector3d const *vector);
void function_ba1d0(long object_index, real_vector3d *linear_velocity, real_vector3d *angular_velocity);
void __stdcall function_11bed0(void *in, void *out);
char function_18d4b0(long tag_index, char audible, long object_index, long *local_player_index);

static inline s_sound_object_header *sound_object_header(long object_index)
{
	return (s_sound_object_header *)g_4e0300->data + (object_index & 0xffff);
}

static inline real_vector3d *matrix4x3_transform_normal(real_matrix4x3 const *matrix, real_vector3d const *vector, real_vector3d *out)
{
	out->i = matrix->up.i * vector->k + matrix->left.i * vector->j + matrix->forward.i * vector->i;
	out->j = matrix->up.j * vector->k + matrix->left.j * vector->j + matrix->forward.j * vector->i;
	out->k = matrix->up.k * vector->k + matrix->left.k * vector->j + matrix->forward.k * vector->i;
	return out;
}

static inline real_vector3d *cross_product3d(real_vector3d const *a, real_vector3d const *b, real_vector3d *result)
{
	result->i = b->k * a->j - a->k * b->j;
	result->j = a->k * b->i - a->i * b->k;
	result->k = a->i * b->j - a->j * b->i;
	return result;
}

static inline real_vector3d *rotate_vector_about_axis(real_vector3d *vector, real_vector3d const *axis, real sine, real cosine)
{
	real_vector3d cross;

	cross_product3d(axis, vector, &cross);
	vector->i = cosine * vector->i + sine * cross.i;
	vector->j = cosine * vector->j + sine * cross.j;
	vector->k = cosine * vector->k + sine * cross.k;
	return vector;
}

// @retail 0x18c250
bool __stdcall function_18c250(long object_index, long tag_index, s_sound_marker const *marker, s_sound_location *location)
{
	bool result = object_index == NONE || function_18c3b0(object_index, tag_index, marker, location);

	if (object_index == NONE || result)
	{
		s_sound_tag *sound = (s_sound_tag *)((s_tag_instance_view *)g_4e3b44)[tag_index & 0xffff].data;
		s_sound_class_view *sound_class = &((s_sound_globals_view *)g_51ebd4)->classes[sound->class_index];
		s_sound_class_spatialization *class_spatialization = sound_class_get_spatialization(sound_class);

		if (class_spatialization && (class_spatialization->flags & 1))
		{
			location->flag0 = true;
			location->spatial.velocity = *g_4687a4;
			location->spatial.position = *(real_point3d *)g_4687a8;
			rotate_vector_about_axis((real_vector3d *)&location->spatial.position, g_4687b0, (real)sin(class_spatialization->angle), (real)cos(class_spatialization->angle));
		}
	}
	return result;
}


// @retail 0x18c3b0
bool __stdcall function_18c3b0(long object_index, long tag_index, s_sound_marker const *marker, s_sound_location *location)
{
	bool result = false;
	s_sound_object_view *object = (s_sound_object_view *)function_badc0(object_index, NONE);

	if (object && !object_or_parent_hidden(object_index))
	{
		s_sound_tag_class *sound = (s_sound_tag_class *)g_4e3b44[tag_index & 0xffff].bytes;
		if (!TEST_FIELD_BIT(object->flag2) || !TEST_FIELD_BIT(((s_sound_class_flags *)function_221810(sound->class_index))->flag11))
		{
			s_location object_location;
			object_get_root_location(object_index, &object_location);

			if (object_location.cluster_index != NONE)
			{
				long object_type = sound_object_header(object_index)->type;
				real_matrix4x3 *matrix = object_get_node_matrix(object_index, marker->node_index < 0xff ? marker->node_index : 0);
				real_vector3d forward;

				function_142570(matrix, &marker->position, &location->spatial.position);
				location->spatial.compressed_forward = vector3d_compress(matrix4x3_transform_normal(matrix, &marker->forward, &forward));
				function_ba1d0(object_index, &location->spatial.velocity, NULL);

				if ((1 << object_type) & 3)
				{
					if (function_11b930(object_index))
					{
						function_11bed0(&location->spatial.position, &object_location);
					}
				}
				else if (object_type == 7)
				{
					short machine_index = sound_object_header(object_index)->object->machine_index;
					if (machine_index != NONE)
					{
						s_local_camera *camera = function_125360();
						if (camera && camera->active)
						{
							long camera_index = camera->index;
							if (camera_index != NONE)
							{
								s_structure_bsp_view *bsp = (s_structure_bsp_view *)g_4e0348;
								long audibility_index = function_249e20(bsp, machine_index);
								if (audibility_index != NONE)
								{
									long row = function_249e60(camera_index, bsp, audibility_index);
									if (row != NONE)
									{
										object_location.cluster_index = bsp->sound_clusters[row + 18 * bsp->cluster_map->indices[(word)machine_index]];
									}
								}
							}
						}
					}
				}

				location->spatial.location = object_location;
				location->audible = function_18d4b0(tag_index, location->requested_audible, object_index, NULL);
				result = true;
			}
		}
	}
	return result;
}

struct s_sound_unit_view
{
	byte unknown00[0x208];
	real sound_value;
};

// @retail 0x18c9b0
void function_18c9b0(long object_index, real target)
{
	if (g_4ed28c->valid && function_badc0(object_index, 3))
	{
		real *value = &((s_sound_unit_view *)sound_object_header(object_index)->object)->sound_value;
		real delta = target - *value;

		if (delta < -0.3f)
		{
			delta = -0.3f;
		}
		else if (delta > 0.3f)
		{
			delta = 0.3f;
		}
		*value += delta;
	}
}

struct s_sound_cluster_view
{
	byte unknown00[0x8c];
	long sound_count;
	short *sounds;
	byte unknown94[0xb0 - 0x94];
};

struct s_sound_environment_view
{
	byte unknown00[8];
	real_point3d position;
	byte unknown14[0x24 - 0x14];
};

struct s_sound_bsp_view
{
	byte unknown00[0x60];
	s_sound_environment_view *environments;
	byte unknown64[0xa0 - 0x64];
	s_sound_cluster_view *clusters;
};

// @retail 0x18cfd0
long function_18cfd0(long cluster_index, real_point3d const *point, real *distance)
{
	*distance = 3.4028235e38f;

	s_sound_cluster_view *cluster = &((s_sound_bsp_view *)g_4e0348)->clusters[cluster_index];
	long result = NONE;

	for (long i = 0; i < cluster->sound_count; i++)
	{
		short index = cluster->sounds[i];
		real_point3d *position = &((s_sound_bsp_view *)g_4e0348)->environments[index].position;
		real_vector3d vector;
		vector3d_from_points3d(position, point, &vector);
		real length = (real)sqrt(vector.j * vector.j + (vector.k * vector.k + vector.i * vector.i));
		if (*distance > length)
		{
			*distance = length;
			result = index;
		}
	}
	return result;
}

/* a sound tag, as the promotion callbacks read it */
struct s_sound_promotion_tag
{
	byte unknown00;
	byte flags;
	char class_index;
	byte unknown03[5];
	short permutation_base;
	byte unknown0a[4];
	short promotion_index;
};

struct s_sound_class_play_bits
{
	byte unknown00[8];
	byte flag0 : 1;
	byte flag1 : 1;
	byte unknown08 : 6;
};

struct s_sound_promotion_rule
{
	byte unknown00[5];
	char index;
	byte unknown06[6];
	long count;
};

struct s_sound_promotion_rules
{
	byte unknown00[8];
	long first_offset;
	s_sound_promotion_rule *rules;
};

struct s_sound_promotion
{
	long count;
	s_sound_promotion_rules *rules;
	byte timer[0x2c - 8];
};

struct s_sound_permutation_set
{
	byte unknown00[8];
	short first_permutation;
	byte unknown0a[2];
};

struct s_sound_permutation
{
	byte unknown00[0x10];
};

struct s_sound_globals_promotion_view
{
	byte unknown00[0x24];
	s_sound_permutation_set *sets;
	byte unknown28[4];
	s_sound_permutation *permutations;
	byte unknown30[0x54 - 0x30];
	s_sound_promotion *promotions;
};

struct s_sound_promotion_state
{
	long count;
	long offset;
};

bool function_12de70(void *timer, long type);
void function_218e50(long tag_index, long permutation_index, long ticks);
void function_10e480(long object_index, long tag_index, s_sound_promotion_state *state, real scale);

static inline s_sound_promotion *sound_promotion_get(s_sound_promotion_tag const *sound)
{
	short promotion_index = sound->promotion_index;
	s_sound_promotion *result = NULL;

	if (promotion_index != NONE)
	{
		result = &((s_sound_globals_promotion_view *)g_51ebd4)->promotions[promotion_index];
	}
	return result;
}

// @retail 0x18c630
void __stdcall function_18c630(long object_index, long tag_index, long a, long b)
{
	s_sound_promotion_tag *sound = (s_sound_promotion_tag *)g_4e3b44[tag_index & 0xffff].bytes;

	if (function_badc0(object_index, 3) && TEST_FIELD_BIT(((s_sound_class_play_bits *)function_221810(sound->class_index))->flag1))
	{
		s_sound_promotion *promotion = sound_promotion_get(sound);
		if (promotion)
		{
			function_12de70(promotion->timer, 2);
		}
	}
}

// @retail 0x18ca20
void __stdcall function_18ca20(long object_index, long tag_index, s_sound_promotion_rule const *rule, real scale)
{
	s_sound_promotion_tag *sound = (s_sound_promotion_tag *)g_4e3b44[tag_index & 0xffff].bytes;
	s_sound_promotion_rules *rules = ((s_sound_globals_promotion_view *)g_51ebd4)->promotions[sound->promotion_index].rules;
	s_sound_promotion_rule *indexed = &rules->rules[rule->index];
	long count = indexed->count;

	if (count > 0 && g_4ed28c->valid && function_badc0(object_index, 3))
	{
		s_sound_promotion_state state;
		state.count = count;
		state.offset = rules->first_offset + *(long *)((byte *)indexed + 8);
		function_10e480(object_index, tag_index, &state, scale);
	}
}

static __forceinline long real_to_long_round(real value)
{
	long result;

	__asm
	{
		fld value
		fistp result
	}
	return result;
}

// @retail 0x18c720
void function_18c720(long tag_index, long object_index, long set_index, long permutation_index, real scale)
{
	s_sound_promotion_tag *sound = (s_sound_promotion_tag *)g_4e3b44[tag_index & 0xffff].bytes;

	if (!(sound->flags & 2) && TEST_FIELD_BIT(((s_sound_class_play_bits *)function_221810(sound->class_index))->flag1))
	{
		s_sound_promotion *promotion = sound_promotion_get(sound);
		if (promotion && (function_12de70(promotion->timer, 2) || promotion->count > 0))
		{
			s_sound_globals_promotion_view *globals = (s_sound_globals_promotion_view *)g_51ebd4;
			s_sound_permutation_set *set = &globals->sets[sound->permutation_base + set_index];
			real value = scale * 30.0f;

			function_218e50(tag_index, permutation_index, real_to_long_round(value));
			function_18c9b0(object_index, value);
			function_18ca20(object_index, tag_index, (s_sound_promotion_rule *)&globals->permutations[set->first_permutation + permutation_index], scale);
		}
	}
}

// @retail 0x18c6a0
void __stdcall function_18c6a0(long object_index, long unused, long tag_index, long set_index, long permutation, long scale)
{
	if (object_index != NONE && permutation && function_badc0(object_index, 3))
	{
		s_sound_promotion_tag *sound = (s_sound_promotion_tag *)g_4e3b44[tag_index & 0xffff].bytes;
		s_sound_globals_promotion_view *globals = (s_sound_globals_promotion_view *)g_51ebd4;
		s_sound_permutation *first = &globals->permutations[globals->sets[sound->permutation_base + set_index].first_permutation];

		function_18c720(tag_index, object_index, set_index, (s_sound_permutation *)permutation - first, *(real *)&scale);
	}
}
