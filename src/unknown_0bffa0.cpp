#include "unknown_11c920.h"
#include "globals.h"
#include <string.h>
#include <math.h>

// @flags /O2 /arch:SSE /Gr

/* Light pool queries and the shapes copied from definitions and placements. */
s_record_pool *g_4e030c;
long g_4e0308;

extern point3f g_4b9da0;

// @retail 0xc0a50
real function_c0a50(point3f const *point, long index, real range, real radius, real const *table)
{
	real x = point->x - g_4b9da0.x;
	real y = point->y - g_4b9da0.y;
	real z = point->z - g_4b9da0.z;
	radius = (real)sqrt((z * z + x * x) + y * y) - radius - table[index];
#define LIGHT_FADE_AB (radius / (range > 0.1f ? range : 0.1f))
	return 1.0f - (LIGHT_FADE_AB < 0.0f ? 0.0f : LIGHT_FADE_AB > 1.0f ? 1.0f : LIGHT_FADE_AB);
#undef LIGHT_FADE_AB
}

struct s_5107e8
{
	bool flag0;
	byte unknown01[3];
	long time4;
	bool flag8;
};

extern s_5107e8 *g_5107e8;
real function_17ca10(real x, short curve);

extern void *g_4e0310;
extern void *g_4e0314;
extern void *g_4e0318;

// @retail 0xc0040
void function_c0040()
{
	s_record_pool *array = g_4e030c;
	array->valid = true;
	record_pool_release_all(array);
	g_5107e8->flag8 = true;
	memset(g_4e0310, 0xff, 512 * sizeof(long));
	array = (s_record_pool *)g_4e0318;
	array->valid = true;
	record_pool_release_all(array);
	array = (s_record_pool *)g_4e0314;
	array->valid = true;
	record_pool_release_all(array);
	g_5107e8->flag0 = false;
	g_5107e8->time4 = 0;
}

struct s_light_ab
{
	short salt;
	word flags;
	long tag_index;
	long scenario_index;
	long stamp;
	byte unknown10[0x24 - 0x10];
	real radius;
	byte unknown28[0xd4 - 0x28];
	vector3f colour_a;
	vector3f colour_b;
	byte unknownec[0x100 - 0xec];
	real fade;
	byte unknown104[0x110 - 0x104];
};

struct s_light_cone_ab
{
	real values[5];
};

struct s_light_shape_ab
{
	long kind;
	union
	{
		struct { real radius_a, radius_b; } sphere;
		s_light_cone_ab cone;
	};
	byte unknown18[0x7c - 0x18];
};

struct s_light_definition_ab
{
	dword flags;
	short kind;
	byte unknown06[0x18 - 6];
	real radius_a;
	real radius_b;
	s_light_cone_ab cone;
};

struct s_light_placement_ab
{
	short palette_index;
	short name_index;
	byte unknown04[0x3c - 4];
	short kind;
	byte unknown3e[0x58 - 0x3e];
	s_light_cone_ab cone;
};

// @retail 0xc1930
void function_c1930(s_light_definition_ab const *definition, s_light_shape_ab *shape)
{
	memset(shape, 0, sizeof(*shape));
	shape->kind = definition->kind;
	if ((short)shape->kind == 0)
	{
		shape->sphere.radius_a = definition->radius_a;
		shape->sphere.radius_b = definition->radius_b;
	}
	else
	{
		shape->cone = definition->cone;
	}
}

// @retail 0xc1980
void function_c1980(s_light_placement_ab const *placement, s_light_definition_ab const *definition, s_light_shape_ab *shape)
{
	memset(shape, 0, sizeof(*shape));
	shape->kind = placement->kind;
	if ((short)shape->kind == 0)
	{
		shape->sphere.radius_a = placement->cone.values[4];
		shape->sphere.radius_b = placement->cone.values[4];
		if (definition->radius_b != 0.0f)
			shape->sphere.radius_b *= definition->radius_b / definition->radius_a;
	}
	else
	{
		shape->cone.values[0] = placement->cone.values[0];
		shape->cone.values[1] = placement->cone.values[1];
		shape->cone.values[2] = placement->cone.values[2];
		shape->cone.values[3] = placement->cone.values[3];
		shape->cone.values[4] = placement->cone.values[4];
	}
}

// @retail 0xc18e0
real function_c18e0()
{
	s_5107e8 *globals = g_5107e8;
	real time = (g_510c54->game_time - globals->time4) * g_510c54->rate;
	if (!(1.0f > time))
		time = 1.0f;
	real result = function_17ca10(time, 5);
	if (!globals->flag0)
		result = 1.0f - result;
	return result;
}

// @retail 0xc3110
bool function_c3110(long index)
{
	s_light_ab *light = (s_light_ab *)g_4e030c->data + (index & 0xffff);
	if (light->stamp != g_4e0308)
	{
		light->stamp = g_4e0308;
		return true;
	}
	return false;
}

static inline real light_colour_magnitude_squared(vector3f const *colour)
{
	return colour->i * colour->i + colour->j * colour->j + colour->k * colour->k;
}

// @retail 0xc3140
bool function_c3140(long index)
{
	s_light_ab *light = (s_light_ab *)g_4e030c->data + (index & 0xffff);
	bool result = false;
	if (light->flags & 2)
	{
		result = light->radius > 0.0001f && light->fade > 0.0001f;
		result &= light_colour_magnitude_squared(&light->colour_a) > 0.05f ||
			light_colour_magnitude_squared(&light->colour_b) > 0.05f;
	}
	return result;
}

struct s_light_scenario_ab
{
	byte unknown00[0xec];
	s_light_placement_ab *lights;
};

// @retail 0xc38e0
long function_c38e0(short name_index)
{
	long result = NONE;
	if (name_index >= 0 && name_index < 0x280)
	{
		s_record_pool *array = g_4e030c;
		s_light_scenario_ab *scenario = (s_light_scenario_ab *)g_4e0350;
		s_record_pool_iterator iterator;
		iterator.data = array;
		iterator.index = NONE;
		iterator.datum_index = NONE;
		s_light_ab *light;
		while ((light = (s_light_ab *)data_iterator_next_calling(&iterator)) != 0)
		{
			if (light->scenario_index != NONE && scenario->lights[light->scenario_index].name_index == name_index)
			{
				result = iterator.datum_index;
				break;
			}
		}
	}
	return result;
}
