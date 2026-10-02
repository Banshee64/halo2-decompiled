// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_17CA10.CPP: lookup-table curve evaluation */

#include "cseries.h"
#include <math.h>

byte g_46bd48[16][1024];
// @retail 0x17ca10
real function_17ca10(real x, short curve)
{
	if (0.0f > x)
		x = 0.0f;
	else if (x > 1.0f)
		x = 1.0f;

	if (curve)
	{
		byte const *table = g_46bd48[curve];
		real scaled = x * 1023.0f;
		real fraction = (real)fmod((double)scaled, 1.0);
		real rounded = scaled - 0.1f;
		long q;
		short i;

		__asm
		{
			fld rounded
			fistp q
		}
		i = (short)q;

		if (i == 0x3ff)
			x = table[0x3ff] * (1.0f / 255.0f);
		else
		{
			real a = table[i] * (1.0f / 255.0f);
			real b = table[i + 1] * (1.0f / 255.0f);

			x = a * (1.0f - fraction) + b * fraction;
		}

		if (0.0f > x)
			x = 0.0f;
		else if (x > 1.0f)
			x = 1.0f;
	}

	return x;
}
