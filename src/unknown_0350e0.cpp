// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_0350E0.CPP: format conversion and geometry helpers */

#include "unknown_11c920.h"
#include "globals.h"
#include <string.h>

real g_4670e4 = 0.85f;

struct s_355e0_function
{
	long size;
	byte *data;
};

struct s_355e0_curve
{
	real period;
	long input;
	real duration;
	s_355e0_function function;
};

real function_13b390(void const *function, real input, real range);

PRIVATE __forceinline real curve_endpoint(s_355e0_function const *curve, real input)
{
	real result = function_13b390(curve, input, 0.0f);
	byte *data = curve->data;
	if (!(data[1] & 0xf0))
	{
		real lower = *(real *)(data + 4);
		real upper = *(real *)(data + 8);
		real clamped = 0.0f > result ? 0.0f : result > 1.0f ? 1.0f : result;
		return (upper - lower) * clamped + lower;
	}
	return result;
}

// @retail 0x355e0
bool function_355e0(long tag, long index, real *first, real *second)
{
	long count = 0;
	(void)&index;
	(void)&first;
	(void)&second;
	byte *definition = *(byte **)(g_4e3b44[tag & 0xffff].bytes + 0x24);
	*first = 0.0f;
	*second = 0.0f;
	long selection = (*(short **)(definition + 0x60))[index * 2 + 1];
		if (selection != NONE)
	{
		word const *range = *(word **)(definition + 0x50) + selection;
		short const *entry = *(short **)(definition + 0x48) + (*range & 0x1ff) * 2;
		for (long i = 0; i < (*range >> 9); entry += 2, ++i)
		{
			if (entry[1] == 4 || entry[1] == 5)
			{
				s_355e0_curve const *curve = *(s_355e0_curve **)(definition + 0x40) + entry[0];
				real duration = 1.0f;
				if (curve->duration != 0.0f) duration = curve->duration;
				s_355e0_function const *function = &curve->function;
				real initial = curve_endpoint(function, 0.0f);
				real final = curve_endpoint(function, 1.0f);
				real slope = (final - initial) / duration;
				switch (entry[1])
				{
				case 4: *first = slope; break;
				case 5: *second = slope; break;
				}
				++count;
			}
		}
	}
	return count > 0;
}

struct s_4ca40_colors
{
	byte unknown00[0x14];
	dword first;
	dword second;
	real color_amount;
	real light_amount;
};

color3f *unpack_color3f(dword pixel, color3f *color);

struct s_fade_record
{
	long state;
	real current;
	real target;
	long next_state;
	real rate;
	dword marker;
};

extern s_fade_record *g_50942c;
extern long g_509420, g_509424;
extern real g_4670fc, g_467100, g_509428;

// @retail 0x398d0
void function_398d0(real seconds)
{
	real const *time_reference = &seconds;
	s_fade_record *record = g_50942c;
	if (record && record->marker == 0xdeadbeef)
	{
		g_509420 = record->state;
		g_4670fc = record->current;
		g_467100 = record->target;
		g_509424 = record->next_state;
		g_509428 = record->rate;
	}
	if (g_509424)
	switch (g_509424)
	{
	case 1:
		g_4670fc += g_509428 * *time_reference;
		if (g_4670fc >= g_467100)
		{
			g_4670fc = g_467100;
			g_509424 = 0;
		}
		g_509420 = 0;
		break;
	case 2:
		g_4670fc += g_509428 * *time_reference;
		if (g_4670fc <= 0.0f)
		{
			g_4670fc = 0.0f;
			g_509424 = 0;
			g_509420 = 1;
		}
		break;
	case 3:
		g_4670fc += g_509428 * *time_reference;
		if (g_509428 > 0.0f && g_4670fc >= g_467100)
		{
			g_4670fc = g_467100;
			g_509424 = 0;
		}
		else if (g_509428 < 0.0f && g_4670fc <= g_467100)
		{
			g_4670fc = g_467100;
			g_509424 = 0;
			if (g_467100 == 0.0f)
				g_509420 = 1;
		}
		break;
	}
	if (record)
	{
		record->state = g_509420;
		record->current = g_4670fc;
		record->target = g_467100;
		record->next_state = g_509424;
		record->rate = g_509428;
		record->marker = 0xdeadbeef;
	}
}

// @retail 0x4ca40
void function_4ca40(s_4ca40_colors const *settings, long step, color3f *light, color3f *color)
{
	(void)&step;
	real blend = step * (1.0f / 7.0f);
	real amount = settings->color_amount < 0.0f ? 0.0f : settings->color_amount > 1.0f ? 1.0f : settings->color_amount;
	real light_amount = settings->light_amount < 0.0f ? 0.0f : settings->light_amount > 1.0f ? 1.0f : settings->light_amount;
	color3f first, second;
	unpack_color3f(settings->first, &first);
	unpack_color3f(settings->second, &second);
	color3f mixed;
	mixed.red = (1.0f - blend) * first.red + second.red * blend;
	mixed.green = (1.0f - blend) * first.green + second.green * blend;
	mixed.blue = (1.0f - blend) * first.blue + second.blue * blend;
	color->red = (color->red * amount + (1.0f - amount)) * mixed.red;
	color->green = (color->green * amount + (1.0f - amount)) * mixed.green;
	color->blue = (color->blue * amount + (1.0f - amount)) * mixed.blue;
	light->red = light->red * light_amount + (1.0f - light_amount) * 0.5f;
	light->green = light->green * light_amount + (1.0f - light_amount) * 0.5f;
	light->blue = light->blue * light_amount + (1.0f - light_amount) * 0.5f;
}

// @retail 0x4ca00
void function_4ca00(s_4ca40_colors const *settings, byte const *data, color3f *light, color3f *color)
{
	s_4ca40_colors const *const *settings_reference = &settings;
	unpack_color3f(*(dword const *)(data + 0xc), light);
	unpack_color3f(*(dword const *)(data + 8), color);
	function_4ca40(*settings_reference, (*(dword const *)data >> 13) & 7, light, color);
}

struct s_2e3f0_record
{
	long tag;
	point3f position;
	byte direction[3];
	byte amount;
	dword color;
};

dword __cdecl function_131f40(real alpha, color3f const *color);

PRIVATE inline byte placement_byte(real value)
{
	real clamped = 0.0f > value ? 0.0f : value > 255.0f ? 255.0f : value;
	return (byte)clamped;
}

// @retail 0x2e230
void function_2e230(long tag, point3f const *position, vector3f const *direction,
	color3f const *color, real alpha, real amount, real scale, s_2e3f0_record *record)
{
	(void)&color;
	(void)&alpha;
	(void)&amount;
	(void)&scale;
	byte *definition = g_4e3b44[tag & 0xffff].bytes;
	record->tag = tag;
	record->position = *position;
	record->direction[0] = placement_byte((direction->i + 1.0f) * 128.0f);
	record->direction[1] = placement_byte((direction->j + 1.0f) * 128.0f);
	record->direction[2] = placement_byte((direction->k + 1.0f) * 128.0f);
	if (definition[0x28] & 0x40)
	{
		record->amount = placement_byte(alpha * 256.0f);
		record->color = *(dword *)&scale;
	}
	else
	{
		record->amount = placement_byte(amount * 256.0f);
		record->color = function_131f40(alpha, color);
	}
}

// @retail 0x2e3f0
void function_2e3f0(s_2e3f0_record const *record, long *tag, point3f *position,
	vector3f *direction, color3f *color, real *alpha, real *amount, real *scale)
{
	(void)&direction;
	(void)&alpha;
	(void)&amount;
	(void)&scale;
	byte *definition = g_4e3b44[record->tag & 0xffff].bytes;
	if (tag)
		*tag = record->tag;
	if (position)
		*position = record->position;
	if (direction)
	{
		direction->i = record->direction[0] * (2.0f / 255.0f) - 1.0f;
		direction->j = record->direction[1] * (2.0f / 255.0f) - 1.0f;
		direction->k = record->direction[2] * (2.0f / 255.0f) - 1.0f;
	}
	if (definition[0x28] & 0x40)
	{
		if (color)
			*color = *(color3f const *)((byte const *)g_4686cc + 4);
		if (alpha)
			*alpha = record->amount * (1.0f / 255.0f);
		if (amount)
			*amount = 1.0f;
		if (scale)
			*scale = *(real const *)&record->color;
	}
	else
	{
		if (color)
			unpack_color3f(record->color & 0xffffff, color);
		if (alpha)
			*alpha = (record->color >> 24) * (1.0f / 255.0f);
		if (amount)
			*amount = record->amount * (1.0f / 255.0f);
		if (scale)
			*scale = 1.0f;
	}
}
long g_4ba01c;
byte g_4ba020, g_4ba021, g_4ba022, g_4ba023, g_4ba024, g_4ba025;
real g_4ba028, g_4ba02c;

// @retail 0x2c490
void function_2c490(long mode)
{
    bool enabled;
    real value;
    switch (mode)
    {
    case 2:
        g_4670e4 = 0.3f;
        g_4ba02c = 0.5f;
        value = 0.8f;
        g_4ba01c = 3;
        enabled = false;
        g_4ba021 = true;
        break;
    case 3:
        g_4670e4 = 0.3f;
        g_4ba02c = 0.5f;
        value = 0.8f;
        g_4ba01c = 3;
        enabled = false;
        g_4ba021 = true;
        break;
    case 4:
        g_4670e4 = 0.25f;
        g_4ba02c = 0.25f;
        value = 0.7f;
        g_4ba01c = 2;
        enabled = false;
        g_4ba021 = true;
        break;
    default:
        g_4670e4 = 0.85f;
        value = 1.0f;
        enabled = true;
        g_4ba021 = false;
        g_4ba02c = 1.0f;
        g_4ba01c = 4;
        break;
    }
    g_4ba020 = enabled;
    g_4ba022 = enabled;
    g_4ba024 = enabled;
    g_4ba023 = enabled;
    g_4ba025 = enabled;
    g_4ba028 = value;
}

struct s_object;
s_object *function_badc0(long object_index, dword type_mask);

struct s_scalar_object_header
{
    byte unknown00[8];
    byte *object;
};

// @retail 0x33670
real function_33670(long index)
{
    byte *object = (byte *)function_badc0(index, NONE);
    real result = 0.0f;
    while (object)
    {
        if (!object[0xaa])
        {
            byte *current = ((s_scalar_object_header *)g_4e0300->data)[index & 0xffff].object;
            result = *(real *)(current + 0x2b0);
            result = result < 0.0f ? 0.0f : result > 1.0f ? 1.0f : result;
            break;
        }
        long parent = *(long *)(object + 0x14);
        if (parent == NONE)
            break;
        index = parent;
        object = ((s_scalar_object_header *)g_4e0300->data)[index & 0xffff].object;
    }
    return result;
}

union s_transition_scalar
{
	real value;
	long bits;
};

struct s_transition_state
{
	union
	{
		long flags;
		struct { byte active; byte changed; word unknown02; };
	};
	long index;
	long unknown08;
	s_transition_scalar start;
	s_transition_scalar elapsed;
	s_transition_scalar output;
};

s_transition_state g_4670cc = {0, 0, 0, {0.0f}, {-1.0f}, {0.0f}};
struct s_transition_filter
{
	long unknown00;
	s_transition_scalar previous;
	s_transition_scalar delta;
};
s_transition_filter g_4b99a0;
byte g_4b99b0[0x4c];

// @retail 0x28950
void function_28950(void)
{
	memset(&g_4670cc, 0, sizeof(g_4670cc));
	memset(g_4b99b0, 0, sizeof(g_4b99b0));
}

// @retail 0x28980
void function_28980(long index)
{
	memset(&g_4b99a0, 0, sizeof(g_4b99a0));
	g_4670cc.active = true;
	g_4670cc.changed = true;
	g_4670cc.index = index;
	real now;
	if (g_510c54 && g_510c54->active)
		now = g_510c54->game_time * g_510c54->rate;
	else
		now = 0.0f;
	g_4670cc.start.value = now;
	g_4670cc.elapsed.value = 0.0f;
	g_4670cc.output.value = 0.0f;
}

extern bool g_4b9ee9;
extern long g_4b9eec;

struct s_cluster_tag_entry
{
	dword unknown00;
	long tag;
};

struct s_cluster_tag_table
{
	byte unknown00[8];
	long count;
	s_cluster_tag_entry *entries;
};

struct rigid_transform_scaled
{
	quaternionf rotation;
	point3f position;
	real scale;
};

struct s_render_model_definition;
void render_model_get_default_orientations(s_render_model_definition const *definition, rigid_transform_scaled *orientations);
void render_model_build_node_matrices(vector3f const *forward, vector3f const *up, point3f const *position,
	s_render_model_definition const *definition, transform4x3f *matrices, rigid_transform_scaled const *orientations);

bool g_4c5038;
long g_4c503c;
transform4x3f g_4c5040[255];

// @retail 0x3ea60
void function_3ea60(void)
{
	struct
	{
		long count;
		rigid_transform_scaled orientations[255];
	} scratch;
	if (g_4b9ee9 && g_4b9eec != NONE)
	{
		long index = NONE;
		s_cluster_tag_table *table = (s_cluster_tag_table *)g_4e0350;
		if ((short)g_4b9eec >= 0 && (short)g_4b9eec < table->count)
			index = table->entries[(short)g_4b9eec].tag;
		byte *data = NULL;
		if (index != NONE)
			data = g_4e3b44[index & 0xffff].bytes;
		byte *definition = g_4e3b44[*(long *)(data + 4) & 0xffff].bytes;
		scratch.count = *(long *)(definition + 0x48);
		render_model_get_default_orientations((s_render_model_definition *)definition, scratch.orientations);
		g_4c5038 = true;
		g_4c503c = *(long *)(definition + 0x48);
		render_model_build_node_matrices(g_4687a8, g_4687b0, g_468788,
			(s_render_model_definition *)definition, g_4c5040, scratch.orientations);
	}
}

// @retail 0x3eb70
real function_3eb70(void)
{
	real result = 1.0f;
	if (g_4b9ee9 && g_4b9eec != NONE)
	{
		long index = NONE;
		s_cluster_tag_table *table = (s_cluster_tag_table *)g_4e0350;
		if ((short)g_4b9eec >= 0 && (short)g_4b9eec < table->count)
			index = table->entries[(short)g_4b9eec].tag;
		byte *data = 0;
		if (index != NONE)
			data = g_4e3b44[index & 0xffff].bytes;
		result = *(real *)(data + 0x14);
	}
	return result;
}

// @retail 0x350e0
void function_350e0(real *out, real const *a, real const *b, real x)
{
	real scale;
	real v;
	real w;

	out[0] = (a[1] - a[0]) * b[0] + a[0];
	out[1] = (a[3] - a[2]) * b[1] + a[2];

	scale = g_485ad4.hi / (g_485ad4.hi - g_485ad4.lo);
	v = (scale * x - g_485ad4.lo * scale) / x * 16777215.0f;
	if (0.0f > v)
		v = 0.0f;
	else if (v > 16777215.0f)
		v = 16777215.0f;
	out[2] = v;

	w = x / g_485ad4.hi * 16777215.0f;
	if (0.0f > w)
		w = 0.0f;
	else if (w > 16777215.0f)
		w = 16777215.0f;
	out[3] = w;
}

// @retail 0x35510
long function_35510(long format)
{
	switch (format)
	{
	case 0: return 1;
	case 1: return 8;
	case 2: return 7;
	case 3: return 2;
	case 4: return 15;
	case 5: return 3;
	case 12: return 5;
	case 14: return 6;
	case 6: return 12;
	case 7: return 13;
	case 18: return 14;
	case 8: return 10;
	case 9: return 11;
	case 10: return 17;
	case 11: return 16;
	case 13: return 18;
	case 16: return 19;
	case 15: return 20;
	case 17: return 21;
	default: __assume(0);
	}
}

// @retail 0x35790
long function_35790(long format)
{
	switch (format)
	{
	case 0: return 0x12;
	case 1: return 0x22;
	case 2: return 0x32;
	case 3: return 0x42;
	case 4: return 0x14;
	case 5: return 0x24;
	case 6: return 0x34;
	case 7: return 0x44;
	case 8: return 0x15;
	case 9: return 0x25;
	case 10: return 0x35;
	case 11: return 0x45;
	case 12: return 0x11;
	case 13: return 0x21;
	case 14: return 0x31;
	case 15: return 0x41;
	case 16: return 0x16;
	case 17: return 0x40;
	default: __assume(0);
	}
}

// @retail 0x35850
long function_35850(long format)
{
	switch (format)
	{
	case 2: return 12;
	case 3: return 16;
	case 4: return 1;
	case 5: return 2;
	case 6: return 3;
	case 10: return 6;
	case 1: return 8;
	case 0: return 4;
	case 7: return 4;
	case 8: return 2;
	case 9: return 4;
	case 11: return 8;
	case 12: return 2;
	case 13: return 4;
	case 14: return 6;
	case 15: return 8;
	case 16: return 4;
	case 17: return 4;
	default: __assume(0);
	}
}

// @retail 0x4dcd0
vector3f *function_4dcd0(vector3f *out, dword packed)
{
	vector3f value;
	value.i = (packed & 15) * (1.0f / 15.0f);
	value.j = ((packed >> 4) & 15) * (1.0f / 15.0f);
	value.k = ((packed >> 8) & 15) * (1.0f / 15.0f);
	value.i = value.i * 2.0f - 1.0f;
	value.j = value.j * 2.0f - 1.0f;
	value.k = value.k * 2.0f - 1.0f;
	*out = value;
	return out;
}

// @retail 0x4dd70
vector3f *function_4dd70(vector3f *out, dword packed)
{
	vector3f value;
	value.i = (packed & 2047) * (1.0f / 2047.0f);
	value.j = ((packed >> 11) & 2047) * (1.0f / 2047.0f);
	value.k = (packed >> 22) * (1.0f / 1023.0f);
	value.i = value.i * 2.0f - 1.0f;
	value.j = value.j * 2.0f - 1.0f;
	value.k = value.k * 2.0f - 1.0f;
	*out = value;
	return out;
}

struct s_format_element
{
	byte unknown00[0x2c];
	short format;
	byte unknown2e[0x2e];
};

struct s_format_element_table
{
	byte unknown00[0x28];
	s_format_element *elements;
};

// @retail 0x4cb80
long function_4cb80(long element_index, long tag_index)
{
	long result = NONE;
	if (tag_index != NONE)
	{
		s_format_element_table *table = (s_format_element_table *)g_4e3b44[tag_index & 0xffff].bytes;
		if (table)
			result = table->elements[element_index].format;
	}
	return result;
}

struct s_format_block
{
	byte unknown00[0xc];
	long size;
	byte unknown10[0xc];
};

struct s_format_block_table
{
	dword unknown00;
	dword count;
	s_format_block *blocks;
};

// @retail 0x4cbb0
long function_4cbb0(long tag_index, dword block_index)
{
	if (tag_index != NONE)
	{
		s_format_block_table *table = (s_format_block_table *)g_4e3b44[tag_index & 0xffff].bytes;
		if (block_index < table->count && (dword)(table->blocks[block_index].size >> 4) > 0)
			return 1;
	}
	return 0;
}

// @retail 0x3fd20
bool function_3fd20(plane3f const *planes, point3f const *point, real radius)
{
	real extent = radius * 1.7320508f;
	for (long i = 0; i < 6; ++i)
	{
		if (plane_distance_to_point(&planes[i], point) >= extent)
			return true;
	}
	return false;
}

struct s_primitive_header
{
	byte type;
	byte unknown01[3];
	long size;
	long capacity;
};

struct s_primitive_storage
{
	dword unknown00;
	byte *data;
};

struct s_primitive_buffer
{
	byte flags;
	byte unknown01[0x33];
	s_primitive_storage *storage;
};

struct s_primitive_group
{
	word unknown00;
	word buffer_index;
	dword unknown04;
	word *counts;
};

struct s_primitive_definition
{
	byte unknown00[0x44];
	s_primitive_buffer *buffers;
	byte unknown48[0xc];
	s_primitive_group *groups;
	byte unknown58[0xc];
	s_primitive_group *groups_alt;
};

// @retail 0x4dc60
s_primitive_header *function_4dc60(s_primitive_definition *data, long index, bool first, long element)
{
	s_primitive_header *result = 0;
	s_primitive_group *group;
	word const *count;
	if (first)
	{
		group = &data->groups[index];
		count = group->counts;
	}
	else
	{
		group = &data->groups_alt[index];
		count = &group->counts[element];
	}
	s_primitive_buffer *buffer = &data->buffers[group->buffer_index];
	if (buffer->flags & 2)
	{
		result = (s_primitive_header *)(buffer->storage->data + 0x20);
		long size = result->type == 0x2e ? 4 * *count : 3 * *count;
		result->size = size;
		result->capacity = size;
	}
	return result;
}

// @retail 0x288e0
real function_288e0(real value)
{
    real change = value - g_4b99a0.previous.value;
    if (0.0f > change)
        change = 0.0f;
    g_4b99a0.delta.value = g_4b99a0.delta.value * 0.92f + change * (1.0f - 0.92f);
    real result = g_4b99a0.delta.value + g_4b99a0.previous.value;
    if (value >= 0.999f)
        return 1.0f;
    if (0.0f > result)
        result = 0.0f;
    else if (result > 1.0f)
        result = 1.0f;
    return result;
}

// @retail 0x36a40
real function_36a40(real mean, real spread, real minimum, real maximum)
{
    real const *mean_reference = &mean;
    real const *spread_reference = &spread;
    real const *minimum_reference = &minimum;
    dword sample = random_next(&g_4e7408->seed);
    real random = (real)sample * (1.0f / 65535.0f);
    real result = (random + random - 1.0f) * *spread_reference + *mean_reference;
    if (result < *minimum_reference)
        return *minimum_reference;
    if (result > maximum)
        result = maximum;
    return result;
}


real g_45dd38 = 1.0f / 1023.0f;
real g_45dd44 = 1.0f / 4095.0f;


color3f *unpack_color3f(dword pixel, color3f *color);
dword __cdecl pack_color3f(color3f const *color);

PRIVATE __forceinline real color_delta(real value, real lower, real upper)
{
 return value < lower ? lower : value > upper ? upper : value;
}

// @retail 0x3e4e0
void function_3e4e0(dword *current, dword const *target, real step)
{
	(void)&target;
	color3f a, b;
	unpack_color3f(*current, &a);
	unpack_color3f(*target, &b);
	a.red += color_delta(b.red - a.red, 0.0f - step, step);
	a.green += color_delta(b.green - a.green, 0.0f - step, step);
	a.blue += color_delta(b.blue - a.blue, 0.0f - step, step);
	*current = pack_color3f(&a);
}

struct s_4b160_entry
{
	dword unknown00[2];
	long key;
	dword unknown0c;
	real depth;
	dword unknown14;
	real distance;
	dword unknown1c[2];
};

// @retail 0x4b160
void function_4b160(long first, s_4b160_entry *entries, long mode, long last)
{
	(void)&entries;
	(void)&mode;
	long const *last_reference = &last;
	long begin = first + 1;
	for (long i = first; i <= *last_reference; ++i)
	{
		for (long j = begin; j <= *last_reference; ++j)
		{
			bool swap;
			switch (mode)
			{
			case 0: swap = entries[j].distance > entries[j - 1].distance; break;
			case 1: swap = entries[j - 1].key < entries[j].key; break;
			default: swap = entries[j].depth > entries[j - 1].depth; break;
			}
			if (swap)
			{
				s_4b160_entry temporary = entries[j - 1];
				entries[j - 1] = entries[j];
				entries[j] = temporary;
			}
		}
	}
}

real g_509418;
real function_30bf0(vector3f *vector);

struct s_vector_perturbation
{
	dword unknown00;
	vector3f direction;
	real speed_spread;
	real speed_minimum;
	real speed_maximum;
	real speed_fraction;
	vector3f velocity;
	vector3f base_direction;
	real direction_spread;
	real magnitude_spread;
	dword unknown40;
	real magnitude;
	real direction_scale;
	real first_fraction;
	real second_fraction;
	real magnitude_fraction;
	vector3f first_axis;
	vector3f second_axis;
	vector3f perturbation;
	real step_scale;
	byte unknown80[0x18];
	real step;
};

// @retail 0x4b3d0
void function_4b3d0(s_vector_perturbation *state)
{
	state->speed_fraction = function_36a40(state->speed_fraction, state->speed_spread * g_509418, 0.0f, 1.0f);
	real speed = (state->speed_maximum - state->speed_minimum) * state->speed_fraction + state->speed_minimum;
	state->velocity.i = state->direction.i * speed;
	state->velocity.j = state->direction.j * speed;
	state->velocity.k = state->direction.k * speed;
	state->first_fraction = function_36a40(state->first_fraction, state->direction_spread * g_509418, -1.0f, 1.0f);
	state->second_fraction = function_36a40(state->second_fraction, state->direction_spread * g_509418, -1.0f, 1.0f);
	state->magnitude_fraction = function_36a40(state->magnitude_fraction, state->magnitude_spread * g_509418, 0.0f, 1.0f);
	real first = state->first_fraction * state->direction_scale;
	state->perturbation.i = state->first_axis.i * first + state->base_direction.i;
	state->perturbation.j = state->first_axis.j * first + state->base_direction.j;
	state->perturbation.k = state->first_axis.k * first + state->base_direction.k;
	real second = state->second_fraction * state->direction_scale;
	state->perturbation.i = state->second_axis.i * second + state->perturbation.i;
	state->perturbation.j = state->second_axis.j * second + state->perturbation.j;
	state->perturbation.k = state->second_axis.k * second + state->perturbation.k;
	function_30bf0(&state->perturbation);
	real magnitude = state->magnitude * state->magnitude_fraction;
	state->perturbation.i *= magnitude;
	state->perturbation.j *= magnitude;
	state->perturbation.k *= magnitude;
	state->velocity.i += state->perturbation.i;
	state->velocity.j += state->perturbation.j;
	state->velocity.k += state->perturbation.k;
	state->step = state->step_scale * g_509418;
}

struct s_33a0b_view;
extern s_33a0b_view *g_485a58;
extern real g_485774, g_485a6c;
extern long g_4b9ed8;
real g_485a30, g_4857f8, g_485864, g_485858;
real g_48578c, g_485868, g_4857a4;
long g_4b9f5c;
real g_4b9f84, g_4b9f80;
real g_4e69c0[4];
real g_4c19b0, g_4c19b4;

// @retail 0x336f0
real function_336f0(long selector)
{
    switch (selector)
    {
    case 0: return 0.0f;
    case 2: return 0.0f;
    case 13: return 0.0f;
    case 38: return 0.0f;
    case 16: return 1.0f > g_485a30 ? g_485a30 : 1.0f;
    case 23:
        if (g_485a58)
        {
            real const *values = (real const *)g_485a58;
            real red = values[4] + values[12];
            real green = values[5] + values[13];
            real blue = values[6] + values[14];
            real value = blue * 0.114f + green * 0.587f + red * 0.299f;
            return 0.0f > value ? 0.0f : value > 1.0f ? 1.0f : value;
        }
        return 1.0f;
    case 12: return 1.0f;
    case 22: return 1.0f;
    case 17: return g_485a30;
    case 10: return g_485774;
    case 24: return g_485774;
    case 3: return g_4857f8;
    case 33: return g_4857f8;
    case 4: return g_485864;
    case 25: return g_485864;
    case 18: return g_485864 * g_485774;
    case 27: return g_485864 * g_485774;
    case 19: return g_485858 * g_4857f8;
    case 36: return g_485858 * g_4857f8;
    case 20: return (1.0f - g_485858) * g_4857f8;
    case 35: return (1.0f - g_485858) * g_4857f8;
    case 21: return g_485a6c;
    case 26: return (1.0f - g_485864) * g_4857f8;
    case 28: return g_48578c;
    case 29: return g_485868;
    case 30: return (1.0f - g_485868) * g_48578c;
    case 31: return g_485868 * g_485774;
    case 32: return g_4857a4;
    case 34: return g_485858;
    case 37: return function_33670(*(long *)g_485a58);
    case 39:
        if (g_4b9f5c != NONE && g_4b9f84 > g_4b9f80)
            return 0.0f - (1.0f / (g_4b9f84 - g_4b9f80)) * g_4b9f80;
        return 0.0f;
    case 40:
        if (g_4b9ed8 >= 0 && g_4b9ed8 < 4)
            return g_4e69c0[g_4b9ed8];
        return 1.0f;
    case 41: return g_4c19b0;
    case 42: return g_4c19b4;
    default: __assume(0);
    }
}

struct s_slot_key
{
    byte a;
    long b, c, d, e;
};
long function_0209b0(s_slot_key *key, void const *data, long size);
void function_020b40(long value, long index);
extern point3f g_4b9da0;
extern vector3f g_4b9dac;
long g_4b9ed4;

// @retail 0x2dba0
bool function_2dba0(long tag, vector3f const *direction, bool alternate,
    long c, long d, long e, point3f const *position, color3f const *color,
    real alpha, real amount, real scale)
{
    bool result = false;
    if (g_4b9ed4 != NONE && tag != NONE && alpha > 0.0f)
    {
        byte *definition = g_4e3b44[tag & 0xffff].bytes;
        if ((amount > 0.0f || !(definition[0x2a] & 1)) &&
            !(definition[0x28] & (alternate ? 8 : 4)))
        {
            vector3f delta;
            delta.i = position->x - g_4b9da0.x;
            delta.j = position->y - g_4b9da0.y;
            delta.k = position->z - g_4b9da0.z;
            real near_distance = *(real *)(definition + 0x18);
            real far_distance = *(real *)(definition + 0x1c);
            if (near_distance == far_distance || far_distance >
                delta.k * g_4b9dac.k + g_4b9dac.j * delta.j + g_4b9dac.i * delta.i)
            {
                s_slot_key key;
                memset(&key, 0, sizeof(key));
                key.a = alternate;
                key.b = g_4b9ed4;
                key.c = c;
                key.d = d;
                key.e = e;
                s_2e3f0_record record;
                function_2e230(tag, position, direction, color, alpha, amount, scale, &record);
                long index = function_0209b0(&key, &record, sizeof(record));
                if (index != NONE)
                {
                    long next = NONE;
                    if (*(short *)(definition + 0x16))
                    {
                        key.c = 4;
                        next = function_0209b0(&key, &record, sizeof(record));
                    }
                    function_020b40(next, index);
                    result = true;
                }
            }
        }
    }
    return result;
}

extern bool g_4ba019;
long function_baf80(long object_index);

// @retail 0x4baf0
void function_4baf0(long object_index, real distance, byte *first, byte *second)
{
    byte *object = ((s_scalar_object_header *)g_4e0300->data)[object_index & 0xffff].object;
    bool opaque = (bool)((*(dword *)(object + 4) >> 19) & 1) | g_4ba019;
    *first = 0;
    *second = 0;
    if (opaque)
    {
        *first = 0xff;
        *second = 0xff;
        return;
    }
    byte *definition = g_4e3b44[*(long *)object & 0xffff].bytes;
    long model = *(long *)(definition + 0x38);
    if (model != NONE)
    {
        byte *settings = g_4e3b44[model & 0xffff].bytes + 0x28;
        long parent = function_baf80(object_index);
        if (parent != object_index)
        {
            byte *parent_object = ((s_scalar_object_header *)g_4e0300->data)[parent & 0xffff].object;
            byte *parent_definition = g_4e3b44[*(long *)parent_object & 0xffff].bytes;
            long parent_model = *(long *)(parent_definition + 0x38);
            if (parent_model != NONE)
                settings = g_4e3b44[parent_model & 0xffff].bytes + 0x28;
        }
        long level = 4 - *(word *)(settings + 0x24);
        real width = *(real *)(object + 0x3c) * 7.0f;
        real a = 1.0f;
        real b = 1.0f;
        real limit = level > 0 ? ((real *)settings)[(level > 4 ? 4 : level) + 3] : 3.402823466e38f;
        if (distance >= limit + width)
            a = 0.0f;
        else if (distance > limit)
        {
            a = 1.0f - (distance - limit) / width;
            if (a < 0.0f) a = 0.0f;
            else if (a > 1.0f) a = 1.0f;
            else if (a <= 0.095f) a = 0.0f;
        }
        if (*(real *)settings > 0.0f)
        {
            if (distance >= *(real *)settings)
                b = 0.0f;
            else if (distance > *(real *)(settings + 4))
                b = 1.0f - (distance - *(real *)(settings + 4)) / (*(real *)settings - *(real *)(settings + 4));
        }
        *first = (byte)(b * 255.0f);
        *second = (byte)(a * 255.0f);
    }
}

#include <math.h>

extern transform4x3f *g_4687d0;
void function_146de0(void);
void function_146b80(void);
int __fastcall function_142a60(transform4x3f const *a, transform4x3f const *b, transform4x3f *result);

// @retail 0x3ebd0
bool __stdcall function_3ebd0(vector3f const *offset, transform4x3f const *matrices, transform4x3f *out, long count)
{
    bool result = false;
    bool restore = g_47989c != NULL;
    if (restore)
        function_146de0();
    if (g_4b9ee9 && g_4b9eec != NONE)
    {
        s_cluster_tag_table *table = (s_cluster_tag_table *)g_4e0350;
        long index = NONE;
        if ((short)g_4b9eec >= 0 && (short)g_4b9eec < table->count)
            index = table->entries[(short)g_4b9eec].tag;
        byte *definition = NULL;
        if (index != NONE)
            definition = g_4e3b44[index & 0xffff].bytes;
        real heading = *(real *)(definition + 0x80);
        vector3f forward;
        forward.i = (real)(cos(heading) * cos(0.0));
        forward.j = (real)(sin(heading) * cos(0.0));
        forward.k = (real)sin(0.0);
        vector3f left;
        left.i = forward.k * g_4687b0->j - forward.j * g_4687b0->k;
        left.j = forward.i * g_4687b0->k - forward.k * g_4687b0->i;
        left.k = forward.j * g_4687b0->i - forward.i * g_4687b0->j;
        transform4x3f transform = *g_4687d0;
        transform.forward = forward;
        transform.left = left;
        transform.up = *g_4687b0;
        transform.scale = *(real *)(definition + 0x14);
        transform.position.x = *(real *)(definition + 0x18) * g_4b9da0.x;
        transform.position.y = *(real *)(definition + 0x18) * g_4b9da0.y;
        transform.position.z = *(real *)(definition + 0x18) * g_4b9da0.z;
        /* The existing assembly callee reads its first matrix through ECX.
           Keep these local stores visible across that assembly boundary. */
        transform4x3f applied;
        for (long component = 0; component < 13; ++component)
            ((volatile real *)&applied)[component] = ((real const *)&transform)[component];
        for (long i = 0; i < count; ++i)
        {
            out[i] = matrices[i];
            out[i].position.x += offset->i;
            out[i].position.y += offset->j;
            out[i].position.z += offset->k;
            function_142a60(&applied, &out[i], &out[i]);
        }
        result = true;
    }
    if (restore)
        function_146b80();
    return result;
}
