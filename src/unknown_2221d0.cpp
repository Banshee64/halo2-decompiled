// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_2221D0.CPP: a real in [-1, 1] to the DSP's 24 bit fixed point.
   Part of the sound transmission file (src/unknown_221da0.cpp); retail calls
   it out of line from 0x2222c0, which this file's /Ob1 reproduces. */

#include "unknown_11c920.h"

#define PIN(n, floor, ceiling) ((n) < (floor) ? (floor) : ((n) > (ceiling) ? (ceiling) : (n)))

const real g_45dc40 = -8388608.0f;
const real g_45dc3c = 8388607.0f;

PRIVATE __forceinline double function_2221e9(real arg_0)
{
	__asm
	{
		fld arg_0
		fmul g_45dc40
	}
}

PRIVATE __forceinline double function_222229(real arg_0)
{
	__asm
	{
		fld arg_0
		fmul g_45dc3c
	}
}

// @retail 0x2221d0
long function_2221d0(real value)
{
	if (value < -1.0f)
	{
		value = -1.0f;
local_0:
		return 0x1000000 - (long)function_2221e9(value);
	}
	if (value > 1.0f)
	{
		value = 1.0f;
		return (long)function_222229(value);
	}
	if (value >= 0.0f)
	{
		return (long)function_222229(value);
	}
	goto local_0;
}
