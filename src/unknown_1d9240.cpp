// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1D9240.CPP */

#include "cseries.h"
#include "globals.h"

struct s_1d9240
{
	char value;
	char count;
	char unknown02;
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

/* advances the counter one tick towards its end (up when flag bit 0 is set,
   down otherwise); false once it has finished */
// @retail 0x1d9320
bool function_1d9320(s_1d9240 *p)
{
	bool result = false;

	if (p->count && !(p->flag & 2))
	{
		if (p->flag & 1)
		{
			p->value++;
			if (p->value >= p->count)
			{
				p->flag |= 2;
				p->value = p->count;
			}
		}
		else
		{
			p->value--;
			if (p->value <= 0)
			{
				p->flag |= 2;
				p->value = 0;
			}
		}
		result = true;
	}
	return result;
}

/* the seconds the counter has left to run */
// @retail 0x1d9430
real function_1d9430(s_1d9240 const *p)
{
	real result = 0.0f;

	if (p->count)
	{
		if (p->flag & 1)
		{
			result = (real)(p->count - p->value) * g_510c54->rate;
		}
		else
		{
			result = (real)p->value * g_510c54->rate;
		}
	}
	return result;
}

#define PIN(value, lower, upper) ((value) < (lower) ? (lower) : (value) > (upper) ? (upper) : (value))

/* 6t^5 - 15t^4 + 10t^3, clamped to [0, 1] */
// @retail 0x1d9670
real function_1d9670(real t)
{
	real t3 = t * t * t;
	real t4 = t3 * t;
	real result = t4 * t * 6.0f - t4 * 15.0f + t3 * 10.0f;

	return PIN(result, 0.0f, 1.0f);
}

real function_10e8e0(real t);

/* the counter's position through its curve: value / count shaped by the
   counter's curve type, clamped to [0, 1] */
// @retail 0x1d9370
real function_1d9370(s_1d9240 const *p)
{
	real result = 0.0f;

	if (p->count)
	{
		real count = (real)p->count;
		real t;

		if (!(count > 1.0f))
		{
			count = 1.0f;
		}
		t = (real)p->value / count;
		switch (p->unknown02)
		{
		case 1:
			t = function_10e8e0(t);
			break;
		case 2:
			t = function_1d9670(t);
			break;
		case 3:
			t -= 1.0f;
			t = 1.0f - t * t;
			break;
		case 4:
		{
			real u = 1.0f - t;

			t = 1.0f - u * u * u;
			break;
		}
		}
		result = PIN(t, 0.0f, 1.0f);
	}
	return result;
}
