// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_21E270.CPP: DirectSound filter coefficients (the same object as
   unknown_21e230.cpp in retail; /Ob1 keeps them out of line, as retail's
   other callers do) */

#include "cseries.h"
#include <xtl.h>
#include <math.h>
#include "unknown_21e230.h"

// @retail 0x21e270
long sound_filter_frequency_coefficient(real frequency)
{
	if (frequency < 30.0f)
	{
		return 0x8000;
	}
	if (frequency > 8000.0f)
	{
		return 0;
	}

	frequency = logf(2.0f * sinf(frequency / 48000.0f * 3.14159265f)) * 5909.2788f;
	return (long)frequency & 0xffff;
}

// @retail 0x21e2d0
dword sound_filter_gain_coefficient(real decibels)
{
	if (decibels > 22.5f)
	{
		decibels = 22.5f;
	}

	dword coefficient = (dword)(expf(decibels * -0.115129255f) * 32768.0f);
	if (coefficient > 0xffff)
	{
		coefficient = 0xffff;
	}
	return coefficient;
}
