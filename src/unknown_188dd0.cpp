// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_188DD0.CPP: unit vectors packed into 32 bits (11, 11 and 10 bits)
   for the sound code */

#include "cseries.h"
#include "real_math.h"

real function_30bf0(real_vector3d *v);

static __forceinline long real_to_long_round(real value)
{
	long result;

	__asm
	{
		fld value
		fistp result
	}
	return result;
}

#define PIN(value, minimum, maximum) ((value) < (minimum) ? (minimum) : ((value) > (maximum) ? (maximum) : (value)))

static inline long quantize_real(real value, real minimum, real maximum, long maximum_value)
{
	return real_to_long_round((value - minimum) * ((real)maximum_value / (maximum - minimum)));
}

static inline real dequantize_real(long value, real minimum, real maximum, long maximum_value)
{
	if (value == 0)
	{
		return minimum;
	}
	if (value >= maximum_value)
	{
		return maximum;
	}
	return ((real)value * maximum + (real)(maximum_value - value) * minimum) * (1.0f / maximum_value);
}

// @retail 0x188dd0
dword vector3d_compress(real_vector3d const *vector)
{
	real k = PIN(vector->k, -1.0f, 1.0f);
	real j = PIN(vector->j, -1.0f, 1.0f);
	real i = PIN(vector->i, -1.0f, 1.0f);
	long qk = quantize_real(k, -1.0f, 1.0f, 0x3ff);
	long qj = quantize_real(j, -1.0f, 1.0f, 0x7ff);
	long qi = quantize_real(i, -1.0f, 1.0f, 0x7ff);

	return (((qk << 11) | qj) << 11) | qi;
}

// @retail 0x188ea0
real_vector3d *vector3d_decompress(dword compressed, real_vector3d *vector)
{
	vector->i = dequantize_real(compressed & 0x7ff, -1.0f, 1.0f, 0x7ff);
	vector->j = dequantize_real((compressed >> 11) & 0x7ff, -1.0f, 1.0f, 0x7ff);
	vector->k = dequantize_real(compressed >> 22, -1.0f, 1.0f, 0x3ff);
	function_30bf0(vector);
	return vector;
}
