// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_2221D0.CPP: a real in [-1, 1] to the DSP's 24 bit fixed point.
   Part of the sound transmission file (src/unknown_221da0.cpp); retail calls
   it out of line from 0x2222c0, which this file's /Ob1 reproduces. */

#include "unknown_11c920.h"

#define PIN(n, floor, ceiling) ((n) < (floor) ? (floor) : ((n) > (ceiling) ? (ceiling) : (n)))

// @retail 0x2221d0
long function_2221d0(real value)
{
	value = PIN(value, -1.0f, 1.0f);
	if (value < 0.0f)
	{
		return 0x1000000 - (long)(value * -8388608.0);
	}
	return (long)(value * 8388607.0);
}
