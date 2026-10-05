// @flags /O2 /Gr
/* UNKNOWN_050690.CPP: random seeds drawn from the game's random generator
   (lane D) */

#include "unknown_11c920.h"
#include "globals.h"
#include <string.h>

/* nine random words and two random reals in [0, 1] (0x50 bytes) */
struct s_random_draw
{
	dword words[9];
	byte unknown24[0x48 - 0x24];
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
