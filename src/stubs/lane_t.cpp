// stubs for the game functions outside 0x160000..0x16ffff that lane T's code
// calls and that are not decompiled yet

#include "cseries.h"
#include "real_math.h"

// @stub 0x1776e0
void __stdcall function_1776e0(long user_index, long object_index, bool add)
{
}

struct s_first_person_marker;

// @stub 0x1d90b0
short function_1d90b0(long render_model_index, long marker_name, long unknown0, long model_index, long const *node_map,
	long node_map_count, real_matrix4x3 const *nodes, long unknown1, s_first_person_marker *markers, long marker_count)
{
	return 0;
}

struct s_animation_state;
struct s_animation;

// @stub 0x1cb0d0
bool function_1cb0d0(s_animation_state *state, long graph_tag_index, long unknown, bool unknown_flag)
{
	return false;
}

// @stub 0x1cba80
s_animation const *function_1cba80(s_animation_state *state, long mode, long weapon_class, long name)
{
	return 0;
}

