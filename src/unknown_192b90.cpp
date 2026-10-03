#include "cseries.h"

// @flags /O2 /arch:SSE /Gr

/* UNKNOWN_192B90.CPP: real helpers */

real function_12aff0(real a, real b, real c, bool flag);

// @retail 0x192d70
real function_192d70(real a, real b, real c)
{
	real value = function_12aff0(b, a, c, false);
	real maximum = c;

	if (a > maximum)
		maximum = a;

	return value * (a / maximum);
}
