// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1D9240.CPP */

#include "cseries.h"
#include "globals.h"

struct s_1d9240
{
	char value;
	char count;
	byte unknown02;
	byte flag;
};

// @retail 0x1d9240
void function_1d9240(s_1d9240 *p, char flag, real x)
{
	long value;
	long n;
	long q;

	x = g_510c54->ticks_per_second * x;
	__asm
	{
		fld x
		fistp q
	}
	n = q;

	if (n < 1)
		n = 1;
	else if (n > 0x7f)
		n = 0x7f;
	n--;

	if (n > 0)
	{
		if (p->count && (p->flag & 1) == flag && n == p->count)
			return;

		if (p->count)
		{
			real denominator = (real)p->count;

			if (!(denominator > 1.0f))
				denominator = 1.0f;
			x = (real)p->value / denominator * (real)(n - 1);
			value = (long)x;
		}
		else
			value = flag ? 1 : n - 1;

		p->count = (char)n;
		p->value = (char)value;
		p->flag = flag != 0;
	}
	else
	{
		p->count = 0;
		p->value = 0;
		p->flag = 0;
	}
}
