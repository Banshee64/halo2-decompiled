// stubs for lane F (0x180000-0x18ffff): callees not decompiled yet
#include "cseries.h"
#include "real_math.h"
#include "physical_memory.h"

// @stub 0xb5920
void function_b5920(long identifier)
{
}

// @stub 0x185630
void function_185630(void)
{
}

struct s_object_query_havok_component;

// @stub 0x1d09d0
void function_1d09d0(short rigid_body_index, s_object_query_havok_component *component, real_vector3d *linear_velocity)
{
}

// @stub 0x1d0ad0
void function_1d0ad0(short rigid_body_index, s_object_query_havok_component *component, real_vector3d *angular_velocity)
{
}

// @stub 0x14a5b0
short __stdcall function_14a5b0(short cluster_index, real_point3d const *point, real radius, long maximum_count, short *clusters)
{
	return 0;
}

// @stub 0x17d100
long __stdcall function_17d100(long cluster_index, long datum_index)
{
	return NONE;
}
struct s_sound_play_state;

// @stub 0x21d110
long function_21d110(s_sound_play_state *state, long tag_index)
{
	return NONE;
}

// @stub 0x13d370
long __stdcall function_13d370(s_physical_object *physical, long size, long type)
{
	return NONE;
}
