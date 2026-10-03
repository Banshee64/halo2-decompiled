// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_13CBB0.CPP */

#include "cseries.h"
#include <math.h>

struct s_13cbb0
{
	byte unknown00[4];
	real value;
};

// @retail 0x13cbb0
real function_13cbb0(s_13cbb0 const *data)
{
	real result = 1.0f;
	if (1.5f >= data->value)
	{
		result = (real)pow(data->value * 0.6666667f, 1.9f);
	}
	return result;
}
