// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1469F0.CPP: game seconds to ticks, rounded (the out-of-line copy;
   unknown_29f5b0.cpp and unknown_1c0290.cpp inline their own) */

#include "unknown_11c920.h"
#include "globals.h"

// @retail 0x1469f0
long function_1469f0(real seconds)
{
	long ticks;

	seconds = (real)g_510c54->field_2_3 * seconds;
	__asm
	{
		fld seconds
		fistp ticks
	}
	return ticks;
}