// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_146240.CPP: the global random seed, random unit vectors and the game
time globals */

#include "cseries.h"
#include "globals.h"
#include "game_state.h"
#include <xtl.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct s_random_globals
{
	dword unknown0;
	dword seed;
};

struct s_game_time_view
{
	bool active;
	byte unknown01;
	short ticks_per_second;
	real rate;
	long game_time;
	real unknownc;
	byte unknown10[0x10];
	long unknown20;
};

struct s_game_options_view
{
	byte unknown00[0xe];
	short ticks_per_second;
};

s_random_globals *g_4e7408;
s_game_options_view *g_4e6948;
real_vector3d g_4417f0[1026];

// @retail 0x146240
void random_initialize(void)
{
	s_random_globals *globals = (s_random_globals *)game_state_malloc("random", "random", sizeof(s_random_globals));

	g_4e7408 = globals;
	globals->unknown0 = 0x78a8;
	dword seed = (dword)time(0);
	seed = (dword)rand() ^ GetTickCount() ^ seed;
	g_4e7408->seed = seed;
}

// @retail 0x1462b0
dword random_seed_generate(void)
{
	dword seed = (dword)time(0);
	seed = (dword)rand() ^ GetTickCount() ^ seed;
	return seed;
}

__inline dword random_next(dword *seed, char const *file, long line)
{
	*seed = 1664525 * *seed + 1013904223;
	return *seed >> 16;
}

__inline real random_real_range(dword *seed, char const *file, long line, real lower_bound, real upper_bound)
{
	real random = (real)random_next(seed, file, line) * (1.f / 65535.f);
	return lower_bound + (upper_bound - lower_bound) * random;
}

// @retail 0x1462e0
real_vector3d *random_unit_vector(real_vector3d *result, dword *seed)
{
	*seed = 1664525 * *seed + 1013904223;
	short index = (short)(((*seed >> 16) * 0x402) >> 16);
	*result = g_4417f0[index];
	return result;
}

// @retail 0x146320
void random_vector_in_cone(real_vector3d const *forward, real_vector3d *result, dword *seed, real min_angle, real max_angle)
{
	real_vector3d random_vector;
	real_vector3d axis;

	*result = *forward;

	*seed = 1664525 * *seed + 1013904223;
	random_vector = g_4417f0[(short)(((*seed >> 16) * 0x402) >> 16)];

	axis.i = random_vector.k * forward->j - random_vector.j * forward->k;
	axis.j = random_vector.i * forward->k - random_vector.k * forward->i;
	axis.k = random_vector.j * forward->i - random_vector.i * forward->j;

	real magnitude = (real)sqrt(axis.k * axis.k + axis.j * axis.j + axis.i * axis.i);
	if (fabs(magnitude) >= 0.0001f)
	{
		real scale = 1.0f / magnitude;
		axis.i *= scale;
		axis.j *= scale;
		axis.k *= scale;

		if (magnitude > 0.0001f)
		{
			real angle = random_real_range(seed, __FILE__, __LINE__, min_angle, max_angle);
			real s = (real)sin(angle);
			real c = (real)cos(angle);

			real dot = result->k * axis.k + result->j * axis.j + result->i * axis.i;
			real t = dot * (1.0f - c);
			real x = result->i;
			real y = result->j;
			real z = result->k;

			result->i = x * c + axis.i * t - (y * axis.k - z * axis.j) * s;
			result->j = y * c + axis.j * t - (z * axis.i - x * axis.k) * s;
			result->k = z * c + axis.k * t - (x * axis.j - y * axis.i) * s;
		}
	}
}

// @retail 0x146550
void game_time_globals_allocate(void)
{
	s_game_time_view *globals = (s_game_time_view *)game_state_malloc("game time", "game time", sizeof(s_game_time_view));

	g_510c54 = (s_game_time_globals *)globals;
	memset(globals, 0, sizeof(s_game_time_view));
}

// @retail 0x1465b0
void game_time_globals_initialize(void)
{
	s_game_time_view *globals = (s_game_time_view *)g_510c54;

	memset(globals, 0, sizeof(s_game_time_view));
	short ticks = g_4e6948->ticks_per_second;
	globals->ticks_per_second = ticks;
	globals->rate = 1.0f / (real)ticks;
	globals->unknownc = 1.0f;
	globals->active = true;
}

// @retail 0x146610
void game_time_stop(void)
{
	s_game_time_view *globals = (s_game_time_view *)g_510c54;

	if (globals && globals->active)
	{
		globals->active = false;
	}
}

// @retail 0x146630
real game_time_get_seconds(void)
{
	s_game_time_view *globals = (s_game_time_view *)g_510c54;

	if (globals && globals->active)
	{
		return (real)globals->game_time * globals->rate;
	}
	return 0.0f;
}

// @retail 0x146650
long game_time_get(void)
{
	return ((s_game_time_view *)g_510c54)->game_time;
}
