// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_2197F0.CPP: converting a linear gain to decibels.
   Decompiled by lane F: 0x188cd0 and 0x188d60 call it with the gain in xmm0.
   The result comes back as the bits of a real, in eax. */

#include "cseries.h"
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