#include "cseries.h"
#include "globals.h"

// @flags /O2 /Gr

/* UNKNOWN_2C4D60.CPP: the channel decoders of the uncompressed animation
   codec (the definition at 0x47fb68 holds them: rotation, translation and
   scale, sampled at the current frame without interpolation) */

struct s_animation_data
{
	byte unknown00[0xc];
	long vector_offset;
	long scale_offset;
	long rotation_stride;
	long vector_stride;
	long scale_stride;
	byte rotations[1];
};

struct s_animation_output
{
	real_quaternion rotation;
	real_vector3d vector;
	real scale;
};

// @retail 0x2c4d60
void function_2c4d60()
{
	s_animation_data *data = g_504480;
	real_quaternion *quaternions = (real_quaternion *)(data->rotations + data->rotation_stride * g_5044b4);

	g_5044c0->rotation = quaternions[g_504464];
}

// @retail 0x2c4da0
void function_2c4da0()
{
	s_animation_data *data = g_504480;
	real_vector3d *vectors = (real_vector3d *)((byte *)data + data->vector_stride * g_5044b8 + g_504464 * 12 + data->vector_offset);

	g_5044c0->vector = *vectors;
}

// @retail 0x2c4de0
void function_2c4de0()
{
	s_animation_data *data = g_504480;

	g_5044c0->scale = *(real *)((byte *)data + data->scale_stride * g_5044bc + g_504464 * 4 + data->scale_offset);
}
