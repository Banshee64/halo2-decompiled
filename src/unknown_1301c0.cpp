// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1301C0.CPP: the fog state blended each tick (lane L's 0x12e5e0
   updates it through these) */

#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include "globals.h"

#define PIN(n,floor,ceiling) ((n)<(floor) ? (floor) : ((n)>(ceiling)?(ceiling):(n)))

/* unknown_033a0b.cpp: a global colour pointer (its colour at +4) */
struct s_33a0b_default;
extern s_33a0b_default *g_4686d4;

struct s_fog_layer
{
	color3f color;
	real intensity;
	real distance;
	real height;
};

struct s_fog_state
{
	byte unknown00[0x1c];
	s_fog_layer layers[3];
	byte unknown64[0x6c - 0x64];
	long index6c;
	color3f color70;
	color3f color7c;
	real value88;
	real value8c;
	real value90;
	real value94;
	byte unknown98[0xa0 - 0x98];
	s_fog_layer pending;
	real valueb8;
	real valuebc;
	bool flagc0;
	byte unknownc1[0xf0 - 0xc1];
	dword flags;
	byte unknownf4[0x10c - 0xf4];
	real value10c;
	real value110;
	byte unknown114[0x118 - 0x114];
	real value118;
};

// @retail 0x130bb0
void function_130bb0(s_fog_state *fog)
{
	if (fog->index6c != NONE && fog->value8c > 0.0001f && fog->value94 > 0.0001f)
	{
		real value90;

		fog->color70.red = PIN(fog->color70.red, 0.0f, 1.0f);
		fog->color70.green = PIN(fog->color70.green, 0.0f, 1.0f);
		fog->color70.blue = PIN(fog->color70.blue, 0.0f, 1.0f);
		fog->color7c.red = PIN(fog->color7c.red, 0.0f, 1.0f);
		fog->color7c.green = PIN(fog->color7c.green, 0.0f, 1.0f);
		fog->color7c.blue = PIN(fog->color7c.blue, 0.0f, 1.0f);
		fog->value88 = PIN(fog->value88, 0.0f, 1.0f);
		fog->value8c = PIN(fog->value8c, fog->value88, 1.0f);
		value90 = fog->value90 + 0.0001f;
		fog->value94 = value90 > fog->value94 ? value90 : fog->value94;
		if (fog->value8c > 0.9999f)
		{
			fog->value8c = 1.0f;
		}
	}
	else
	{
		fog->value8c = 0.0f;
	}

	if (fog->value8c == 0.0f)
	{
		color3f const *black = (color3f const *)((byte const *)g_4686d4 + 4);

		fog->color70 = *black;
		fog->color7c = *black;
		fog->value88 = 0.0f;
		fog->value8c = 0.0f;
		fog->value90 = 0.0f;
		fog->value94 = 0.0f;
	}
}

// @retail 0x130de0
long function_130de0(s_fog_state *fog)
{
	if (fog->pending.intensity > 0.0f)
	{
		long result = 3;
		real distance;

		if (fog->value10c != 1.0f)
		{
			return result;
		}
		if (fog->value118 != 0.0f)
		{
			return result;
		}

		if (fog->layers[0].intensity == 0.0f)
		{
			goto use_layer0;
		}
		if (fog->layers[1].intensity == 0.0f)
		{
			goto use_layer1;
		}
		if (fog->pending.intensity != 1.0f)
		{
			return result;
		}
		distance = fog->pending.distance > fog->pending.height ? fog->pending.distance : fog->pending.height;
		if (fog->layers[0].distance >= distance)
		{
use_layer0:
			fog->layers[0].color = fog->pending.color;
			fog->layers[0].intensity = fog->pending.intensity;
			fog->layers[0].distance = fog->value110;
			fog->layers[0].height = fog->pending.distance;
			result = 4;
		}
		else if (fog->layers[1].distance >= distance)
		{
use_layer1:
			fog->layers[1].color = fog->pending.color;
			fog->layers[1].intensity = fog->pending.intensity;
			fog->layers[1].distance = fog->value110;
			fog->layers[1].height = fog->pending.distance;
			result = 5;
		}
		else
		{
			return result;
		}
		fog->pending.intensity = 0.0f;
		fog->value10c = 0.0f;
		fog->value118 = 1.0f;
		return result;
	}
	if (fog->layers[0].intensity > 0.0f || fog->layers[1].intensity > 0.0f)
	{
		return 1;
	}
	if (fog->layers[2].intensity > 0.0f)
	{
		return 2;
	}
	return 0;
}

static __forceinline color3f const *fog_black(void)
{
	return (color3f const *)((byte const *)g_4686d4 + 4);
}

/* pins a layer's colour, keeps its height above its distance and clears a
   faded layer */
#define FOG_LAYER_VALIDATE(layer) \
	if ((layer).intensity > 0.0001f) \
	{ \
		real height; \
		(layer).color.red = PIN((layer).color.red, 0.0f, 1.0f); \
		(layer).color.green = PIN((layer).color.green, 0.0f, 1.0f); \
		(layer).color.blue = PIN((layer).color.blue, 0.0f, 1.0f); \
		height = (layer).distance + 0.0001f; \
		(layer).height = height > (layer).height ? height : (layer).height; \
		if ((layer).intensity > 0.9999f) \
		{ \
			(layer).intensity = 1.0f; \
		} \
	} \
	else \
	{ \
		(layer).intensity = 0.0f; \
	} \
	if ((layer).intensity == 0.0f) \
	{ \
		(layer).color = *fog_black(); \
		(layer).distance = 0.0f; \
		(layer).height = 0.0f; \
	}

// @retail 0x1301c0
void function_1301c0(s_fog_state *fog)
{
	if (fog->flags & 1)
	{
		fog->layers[0].intensity = 0.0f;
	}
	if (fog->flags & 2)
	{
		fog->layers[1].intensity = 0.0f;
	}
	if (fog->flags & 4)
	{
		fog->pending.intensity = 0.0f;
	}

	FOG_LAYER_VALIDATE(fog->layers[0]);
	FOG_LAYER_VALIDATE(fog->layers[1]);

	if (fog->pending.intensity > 0.0001f)
	{
		fog->pending.color.red = PIN(fog->pending.color.red, 0.0f, 1.0f);
		fog->pending.color.green = PIN(fog->pending.color.green, 0.0f, 1.0f);
		fog->pending.color.blue = PIN(fog->pending.color.blue, 0.0f, 1.0f);
		fog->pending.distance = 0.0001f > fog->pending.distance ? 0.0001f : fog->pending.distance;
		fog->pending.height = 0.0001f > fog->pending.height ? 0.0001f : fog->pending.height;
		if (!fog->flagc0)
		{
			fog->valuebc = 0.0001f > fog->valuebc ? 0.0001f : fog->valuebc;
			fog->valueb8 = fog->valuebc * 0.125f;
		}
		if (fog->pending.intensity > 0.9999f)
		{
			fog->pending.intensity = 1.0f;
		}
	}
	else
	{
		fog->pending.intensity = 0.0f;
	}

	if (fog->pending.intensity == 0.0f)
	{
		fog->pending.color = *fog_black();
		fog->pending.distance = 0.0f;
		fog->pending.height = 0.0f;
		fog->valueb8 = 0.0f;
		fog->valuebc = 0.0f;
	}

	if (fog->layers[2].intensity > 0.0001f)
	{
		fog->layers[2].color.red = PIN(fog->layers[2].color.red, 0.0f, 1.0f);
		fog->layers[2].color.green = PIN(fog->layers[2].color.green, 0.0f, 1.0f);
		fog->layers[2].color.blue = PIN(fog->layers[2].color.blue, 0.0f, 1.0f);
		if (fog->layers[2].intensity > 0.9999f)
		{
			fog->layers[2].intensity = 1.0f;
		}
	}
	else
	{
		fog->layers[2].intensity = 0.0f;
	}

	if (fog->layers[2].intensity == 0.0f)
	{
		fog->layers[2].color = *fog_black();
	}
}

bool function_16e210(long cluster_index, long value);

PRIVATE __forceinline real function_1305d1(real arg_1)
{
	arg_1 = 0.0f > arg_1 ? 0.0f : (arg_1 > 1.0f ? 1.0f : arg_1);
	if (0.0001f > arg_1)
		arg_1 = 0.0f;
	else if (arg_1 > 0.9999f)
		arg_1 = 1.0f;
	return arg_1;
}

// @retail 0x1305d0
void function_1305d0(s_fog_state *arg_1, long arg_2, point3f const *arg_3, bool arg_4)
{
	(void)&arg_2;
	if (arg_1->pending.intensity > 0.0f)
	{
		if (arg_4)
		{
			*(real *)((byte *)arg_1 + 0x108) = 0.0f - arg_1->pending.height;
			arg_1->value10c = 1.0f;
		}
		else
		{
			*(real *)((byte *)arg_1 + 0x108) = plane_distance_to_point((plane3f *)((byte *)arg_1 + 0xf4), arg_3);
			arg_1->value10c = function_1305d1(0.0f - *(real *)((byte *)arg_1 + 0x108) / arg_1->pending.height);
			real local_1 = 0.0f > *(real *)((byte *)arg_1 + 0x108) ? 0.0f : *(real *)((byte *)arg_1 + 0x108);
			arg_1->value110 = 0.0f - (*(real *)((byte *)arg_1 + 0xc4) / arg_1->pending.distance) * local_1;
			if (0.0f > *(real *)((byte *)arg_1 + 0x108) && !function_16e210(arg_2, *(long *)((byte *)arg_1 + 0x9c)))
			{
				*(real *)((byte *)arg_1 + 0x108) = 0.0f;
				arg_1->value10c = 0.0f;
				arg_1->value110 = 0.0f;
			}
		}
		long local_2 = *(long *)((byte *)arg_1 + 0x9c);
		if (local_2 != NONE)
		{
			byte *local_3 = g_4e3b44[local_2 & 0xffff].bytes;
			if (*(long *)(local_3 + 0x30) > 0)
			{
				real *local_4 = *(real **)(local_3 + 0x34);
				real local_5 = 0.0f > local_4[10] ? 0.0f : (local_4[10] > 0.9999f ? 0.9999f : local_4[10]);
				real local_6 = PIN((arg_1->value10c - local_5) / (1.0f - local_5), 0.0f, 1.0f);
				if (local_6 > 0.0f)
				{
					arg_1->color70.red += (local_4[0] - arg_1->color70.red) * local_6;
					arg_1->color70.green += (local_4[1] - arg_1->color70.green) * local_6;
					arg_1->color70.blue += (local_4[2] - arg_1->color70.blue) * local_6;
					arg_1->color7c.red += (local_4[3] - arg_1->color7c.red) * local_6;
					arg_1->color7c.green += (local_4[4] - arg_1->color7c.green) * local_6;
					arg_1->color7c.blue += (local_4[5] - arg_1->color7c.blue) * local_6;
					arg_1->value88 += (local_4[6] - arg_1->value88) * local_6;
					arg_1->value8c += (local_4[7] - arg_1->value8c) * local_6;
					arg_1->value90 += (local_4[8] - arg_1->value90) * local_6;
					arg_1->value94 += (local_4[9] - arg_1->value94) * local_6;
					if (arg_1->index6c == NONE)
						arg_1->index6c = *(long *)(local_4 + 12);
				}
			}
		}
		arg_1->value110 += *(real *)((byte *)arg_1 + 0xdc) * *(real *)((byte *)arg_1 + 0xd8);
		if (arg_1->flagc0)
		{
			arg_1->valuebc = 1024.0f;
			arg_1->valueb8 = 1023.0f;
		}
		arg_1->valueb8 = arg_1->valueb8 > 0.0f ? arg_1->valueb8 : 0.0f;
		arg_1->valuebc = arg_1->valuebc > arg_1->valueb8 + 0.0001f ? arg_1->valuebc : arg_1->valueb8 + 0.0001f;
		if (arg_1->layers[0].intensity > 0.0f || arg_1->layers[1].intensity > 0.0f)
			arg_1->value118 = function_1305d1((*(real *)((byte *)arg_1 + 0x108) - arg_1->valueb8) / (arg_1->valuebc - arg_1->valueb8));
		else
			arg_1->value118 = 0.0f;
	}
	else
		arg_1->value118 = 1.0f;

	real local_7 = PIN(arg_1->value10c * arg_1->pending.intensity, 0.0f, 1.0f);
	if (arg_1->value10c > 0.0f)
	{
		real local_8 = (0.0f > arg_1->layers[2].intensity ? 0.0f : (arg_1->layers[2].intensity > 1.0f ? 1.0f : arg_1->layers[2].intensity)) * PIN(1.0f - local_7, 0.0f, 1.0f);
		*(real *)((byte *)arg_1 + 0x5c) = PIN(arg_1->pending.color.red * local_7 + arg_1->layers[2].color.red * local_8, 0.0f, 1.0f);
		*(real *)((byte *)arg_1 + 0x60) = PIN(arg_1->pending.color.green * local_7 + arg_1->layers[2].color.green * local_8, 0.0f, 1.0f);
		*(real *)((byte *)arg_1 + 0x64) = PIN(arg_1->pending.color.blue * local_7 + arg_1->layers[2].color.blue * local_8, 0.0f, 1.0f);
		*(real *)((byte *)arg_1 + 0x68) = function_1305d1(1.0f - PIN(1.0f - arg_1->layers[2].intensity, 0.0f, 1.0f) * PIN(1.0f - local_7, 0.0f, 1.0f));
	}
	else
	{
		real local_9 = (0.0f > arg_1->layers[2].intensity ? 0.0f : (arg_1->layers[2].intensity > 1.0f ? 1.0f : arg_1->layers[2].intensity));
		*(real *)((byte *)arg_1 + 0x5c) = PIN(arg_1->layers[2].color.red * local_9, 0.0f, 1.0f);
		*(real *)((byte *)arg_1 + 0x60) = PIN(arg_1->layers[2].color.green * local_9, 0.0f, 1.0f);
		*(real *)((byte *)arg_1 + 0x64) = PIN(arg_1->layers[2].color.blue * local_9, 0.0f, 1.0f);
		*(real *)((byte *)arg_1 + 0x68) = function_1305d1(arg_1->layers[2].intensity);
	}
}
