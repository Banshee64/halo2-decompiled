// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_219880.CPP: a randomized sound parameter: a value interpolated
   between two bounds, plus an offset and a random variation */

#include "unknown_11c920.h"

/* (16 bytes) */
struct s_sound_random_parameter
{
	real lower;
	real upper;
	real offset;
	real variation;
};

static inline real sound_random_range(dword *seed, real lower, real upper)
{
	*seed = *seed * 0x19660d + 0x3c6ef35f;
	return lower + (upper - lower) * ((real)(*seed >> 16) * (1.0f / 65535.0f));
}

static inline real interpolate_linear(real lower, real upper, real interpolation)
{
	return lower + (upper - lower) * interpolation;
}

// @retail 0x219880
real function_219880(s_sound_random_parameter const *parameter, dword *seed, real interpolation)
{
	real value = interpolate_linear(parameter->lower, parameter->upper, interpolation);

	real random = sound_random_range(seed, -parameter->variation, parameter->variation);

	return random + parameter->offset + value;
}
