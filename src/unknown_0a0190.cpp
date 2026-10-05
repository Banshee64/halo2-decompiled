// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_0A0190.CPP: real math validity checks (decompiled by lane N for
   0x143120, which calls both) */

#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include <math.h>

#define k_real_tolerance 0.001f

static inline bool function_x41b793(real value)
{
	return (*(long *)&value & 0x7f800000) != 0x7f800000;
}

// @retail 0xa0190
bool function_a0190(vector3f const *vector)
{
	real difference = length_sq3f(vector) - 1.0f;

	return function_x41b793(difference) && fabs(difference) < k_real_tolerance;
}

// @retail 0xa0200
bool function_a0200(real a, real b)
{
	real difference = a - b;

	return function_x41b793(difference) && fabs(difference) < k_real_tolerance;
}

// @retail 0xa7570
bool function_a7570(vector3f const *vector)
{
	return function_x41b793(vector->i) && function_x41b793(vector->j) && function_x41b793(vector->k);
}


// @retail 0xaba40
bool function_aba40(real const *values)
{
	if (function_x41b793(values[0]) && function_x41b793(values[1]) &&
		values[0] >= 0.0f && values[0] <= 6.2831854820251465f &&
		values[1] >= -1.5707963705062866f && values[1] <= 1.5707963705062866f)
		return true;
	return false;
}


// @retail 0xa75d0
bool __stdcall function_a75d0(vector3f *vector, real maximum)
{
	real squared = length_sq3f(vector);
	if (squared > maximum * maximum)
	{
		real scale = (real)(maximum / sqrt(squared));
		vector->i = (real)(scale * vector->i);
		vector->j = (real)(scale * vector->j);
		vector->k = (real)(scale * vector->k);
		return true;
	}
	return false;
}


bool function_a74c0(vector3f const *forward, vector3f const *up);

struct s_z_transform_state
{
	long identifier;
	vector3f position;
	vector3f forward;
	vector3f up;
	vector3f linear_velocity;
	vector3f angular_velocity;
};

// @retail 0xab9f0
bool function_ab9f0(s_z_transform_state const *state)
{
	if (state && state->identifier != NONE && function_a7570(&state->position) &&
		function_a74c0(&state->forward, &state->up) && function_a7570(&state->linear_velocity) &&
		function_a7570(&state->angular_velocity))
		return true;
	return false;
}

