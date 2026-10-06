// stubs for the game functions outside 0x160000..0x16ffff that lane T's code
// calls and that are not decompiled yet

#include "unknown_11c920.h"
#include "unknown_0259d0.h"

struct s_animation_state;
struct s_animation;

struct s_bitmap_data;

/* 0x12ce00 (lane L) is stubbed in lane_a.cpp */

/* unowned: the weapon's procedural node adjustments */
struct s_16760c_render_model;

/* takes the camera matrix in eax in retail */
// @stub 0x3f660
void function_3f660(transform4x3f const *matrix)
{
}

/* lane Q's region */
// @stub 0x1554b0
void function_1554b0(long unknown)
{
}

// @stub 0x16ebf0
void function_16ebf0(long user_index)
{
}

/* lane R's region */
// @stub 0x170fd0
void __stdcall function_170fd0(long user_index)
{
}

/* unowned */

/* the loading screen's callees (loading.cpp) */
// @stub 0x8df50
void function_8df50(void)
{
}


// @stub 0x2232a0
void function_2232a0(void)
{
}


// @stub 0x12b6f0
void function_12b6f0(real progress)
{
}

/* lane O's region */
// @stub 0x246c60
void function_246c60(void *block, long unknown)
{
}

/* the UI lane's region; retail passes the object in ecx */
/* the UI lane's region; retail passes the command in eax and the object in ecx */
struct s_observer_command;
// @stub 0x23c0e0
void function_23c0e0(long object_index, s_observer_command *command)
{
}

/* lane D's region: a machine's connection quality */
struct s_68a90_entry;
/* in the region: the collision test of one object (not decompiled yet) */
struct s_collision_result_1697c0;
// @stub 0x1691a0
bool function_1691a0(long object_index, dword flags, dword test_flags, point3f const *point,
	vector3f const *vector, s_collision_result_1697c0 *collision)
{
	return false;
}
