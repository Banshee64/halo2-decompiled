// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1FC2F0.CPP: a tracked point (an aim or look target) and its
   updates */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"

struct s_tracked_point
{
	real_point3d point;
	real_vector3d offset;
	real_vector3d velocity;
	short unknown24;
	short unknown26;
	bool unknown28;
	bool unknown29;
};

// @retail 0x1fc2f0
void function_1fc2f0(s_tracked_point *tracked, real_point3d const *point, bool unknown)
{
	tracked->offset = *g_4687a4;
	tracked->point = *point;
	tracked->unknown29 = false;
	tracked->velocity = *g_4687a4;
	tracked->unknown28 = unknown;
	tracked->unknown24 = NONE;
	tracked->unknown26 = NONE;
}

struct s_tracking_source
{
	byte unknown00[0x28];
	bool unknown28;
};

struct s_tracking_target
{
	byte unknown00[0xe4];
	real unknowne4;
};

struct s_tracking_result
{
	long unknown0;
	long direction;
};

// @retail 0x1fc620
void function_1fc620(s_tracking_target const *target, s_tracking_source const *source, s_tracking_result *result)
{
	if (source->unknown28)
	{
		if (target->unknowne4 < 0.0f)
			result->direction = 0xb;
		else if (target->unknowne4 > 0.0f)
			result->direction = 0xa;
		else
			result->direction = 1;
	}
}

struct s_tracking_state
{
	byte unknown00[0x2c];
	real_point3d unknown2c;
	real_point3d unknown38;
	byte unknown44[0x50 - 0x44];
	short unknown50;
	short unknown52;
	byte unknown54[0xe8 - 0x54];
	real_vector3d unknowne8;
};

struct s_tracking_output
{
	dword flags;
	byte unknown04[0x54 - 0x4];
	bool unknown54;
	byte unknown55[3];
	real_point3d point;
};

// @retail 0x1fc660
void function_1fc660(s_tracked_point *tracked, s_tracking_state const *state, s_tracking_output *output)
{
	if (tracked->unknown24 == state->unknown50 && tracked->unknown26 == state->unknown52)
	{
		real_vector3d delta;

		vector3d_from_points3d(&state->unknown38, &state->unknown2c, &delta);
		if (magnitude_squared3d(&delta) > 0.01f)
			output->flags |= 4;
	}
	if (tracked->unknown29)
	{
		output->point.x = state->unknowne8.i + tracked->offset.i;
		output->point.y = state->unknowne8.j + tracked->offset.j;
		output->point.z = state->unknowne8.k + tracked->offset.k;
		output->unknown54 = true;
		tracked->unknown29 = false;
	}
}
