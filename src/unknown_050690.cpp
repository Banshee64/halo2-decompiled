// @flags /O2 /Gr
/* UNKNOWN_050690.CPP: random seeds drawn from the game's random generator
   (lane D) */

#include "unknown_11c920.h"
#include "globals.h"
#include <string.h>
#include <math.h>

/* nine random words and two random reals in [0, 1] (0x50 bytes) */
struct s_random_draw
{
	dword words[9];
	real phases[9];
	real reals[2];
};

static inline dword random_draw_next(dword *seed)
{
	*seed = *seed * 0x19660d + 0x3c6ef35f;
	return *seed >> 16;
}

static inline real random_draw_real(dword *seed)
{
	return (real)random_draw_next(seed) * (1.f / 65535.f);
}

// @retail 0x50690
void function_50690(s_random_draw *draw)
{
	memset(draw, 0, sizeof(*draw));
	dword *seed = &g_4e7408->seed;
	for (long i = 0; i < 9; i++)
		draw->words[i] = random_draw_next(seed);
	draw->reals[0] = random_draw_real(seed);
	draw->reals[1] = random_draw_real(seed);
}

struct s_random_draw_definition
{
	short unknown00;
	short additional_word_count;
	byte unknown04[12];
	real period;
	byte unknown14[0x90 - 0x14];
	real rates[9];
};

s_random_draw g_4c9828;
bool g_50944d;

// @retail 0x507e0
void __stdcall function_507e0(s_random_draw *draw, const s_random_draw_definition *definition, real elapsed)
{
	long count = definition->additional_word_count + 2;
	if (!draw)
	{
		draw = &g_4c9828;
		if (g_50944d)
		{
			function_50690(draw);
			g_50944d = true;
		}
	}
	if (elapsed > 0.f)
	{
		for (long i = 0; i < count; i++)
		{
			if (definition->rates[i] > 0.f)
			{
				draw->phases[i] += definition->rates[i] * elapsed;
				if (draw->phases[i] >= 1.f)
				{
					draw->phases[i] -= (real)floor(draw->phases[i]);
					draw->words[i]++;
				}
			}
		}
		if (definition->period != 0.f)
			draw->reals[0] += elapsed / definition->period;
		draw->reals[1] += elapsed;
	}
}
