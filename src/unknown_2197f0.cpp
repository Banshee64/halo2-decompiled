// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_2197F0.CPP: converting a linear gain to decibels.
   Decompiled by lane F: 0x188cd0 and 0x188d60 call it with the gain in xmm0.
   The result comes back as the bits of a real, in eax. */

#include "unknown_11c920.h"
#include <math.h>

static inline long decibels_pin_bits(real decibels)
{
	if (decibels < -64.0f)
		return 0xc2800000;
	else if (decibels > 0.0f)
		return 0;
	else
		return *(long *)&decibels;
}

static inline long decibels_pin(long decibels)
{
	long result;

	if (*(real *)&decibels < -64.0f)
		result = 0xc2800000;
	else if (*(real *)&decibels > 0.0f)
		result = 0;
	else
		result = decibels;
	return result;
}

/* a gain in decibels to a linear gain */
// @retail 0x2195f0
real function_2195f0(real decibels)
{
	long pinned = decibels_pin(*(long *)&decibels);

	real scaled = *(real *)&pinned * 0.05f;

	return (real)exp(scaled * 2.3025851f);
}

// @retail 0x2197f0
long function_2197f0(real gain)
{
	real value = gain < 0.0f ? 0.0f : (gain > 1.0f ? 1.0f : gain);
	real decibels;

	if (gain > 0.0f && (value = (real)(20.0f * log10(value)), !(value < -64.0f)))
	{
		decibels = value > 0.0f ? 0.0f : value;
	}
	else
	{
		decibels = -64.0f;
	}

	return decibels_pin_bits(decibels);
}
#define PIN(value, lower, upper) ((lower) > (value) ? (lower) : ((value) > (upper) ? (upper) : (value)))

/* a gain in decibels between two others, along a curve: linear in decibels,
   linear in gain, the gain's power, or a smooth step */
// @retail 0x219650
long function_219650(long curve, real lower, real upper, real t)
{
	long result = *(long *)&upper;

	switch (curve)
	{
	case 0:
	{
		real decibels = (upper - lower) * t + lower;

		result = *(long *)&decibels;
		break;
	}
	case 2:
	{
		real lower_gain = function_2195f0(lower);
		real upper_gain = function_2195f0(upper);
		real fraction;

		if (upper_gain > lower_gain)
			fraction = (real)sqrt(t);
		else
			fraction = 1.0f - (real)sqrt(1.0f - t);
		result = function_2197f0((upper_gain - lower_gain) * fraction + lower_gain);
		break;
	}
	case 1:
	{
		real lower_gain = function_2195f0(lower);
		real upper_gain = function_2195f0(upper);

		result = function_2197f0((upper_gain - lower_gain) * t + lower_gain);
		break;
	}
	case 3:
	{
		real lower_gain = function_2195f0(lower);
		real upper_gain = function_2195f0(upper);
		real fraction = t * t * 3.0f - t * t * t * 2.0f;

		fraction = PIN(fraction, 0.0f, 1.0f);
		result = function_2197f0((upper_gain - lower_gain) * fraction + lower_gain);
		break;
	}
	}
	return result;
}

/* silence, in decibels */
real g_44a0b4 = -64.0f;

/* a gain in decibels from silence (-64 dB) to the gain, along a curve.
   Retail keeps all three arguments on the stack, and this body keeps them
   there without any escape. */
// @retail 0x2197b0
long __stdcall function_2197b0(long curve, real gain, real scale)
{
	scale = PIN(scale, 0.0f, 1.0f);
	return function_219650(curve, g_44a0b4, gain, scale);
}
