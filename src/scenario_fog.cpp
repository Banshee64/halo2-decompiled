// @flags /O2 /arch:SSE /Gr
/* SCENARIO_FOG.CPP: the fog state blended each tick (lane L's 0x12e5e0
   updates it through these) */

#include "cseries.h"
#include "real_math.h"

#define PIN(n,floor,ceiling) ((n)<(floor) ? (floor) : ((n)>(ceiling)?(ceiling):(n)))

/* unknown_033a0b.cpp: a global colour pointer (its colour at +4) */
struct s_33a0b_default;
extern s_33a0b_default *g_4686d4;

struct s_fog_layer
{
	real_rgb_color color;
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
	real_rgb_color color70;
	real_rgb_color color7c;
	real value88;
	real value8c;
	real value90;
	real value94;
	byte unknown98[0xa0 - 0x98];
	s_fog_layer pending;
	byte unknownb8[0x10c - 0xb8];
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
		real_rgb_color const *black = (real_rgb_color const *)((byte const *)g_4686d4 + 4);

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
	long result;

	if (fog->pending.intensity > 0.0f)
	{
		real distance;

		result = 3;
		if (fog->value10c != 1.0f || fog->value118 != 0.0f)
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
	}
	else if (fog->layers[0].intensity > 0.0f || fog->layers[1].intensity > 0.0f)
	{
		result = 1;
	}
	else if (fog->layers[2].intensity > 0.0f)
	{
		result = 2;
	}
	else
	{
		result = 0;
	}

	return result;
}
