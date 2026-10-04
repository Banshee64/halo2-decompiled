// stubs for the game functions outside 0x160000..0x16ffff that lane T's code
// calls and that are not decompiled yet

#include "cseries.h"
#include "real_math.h"

struct s_first_person_marker;

// @stub 0x1d90b0
short function_1d90b0(long render_model_index, long marker_name, long unknown0, long model_index, long const *node_map,
	long node_map_count, real_matrix4x3 const *nodes, long unknown1, s_first_person_marker *markers, long marker_count)
{
	return 0;
}

struct s_animation_state;
struct s_animation;

struct s_bitmap_data;

// @stub 0x3bcb0
long function_3bcb0(s_bitmap_data *bitmap)
{
	return 0;
}

/* in the region: the first person weapon update (not decompiled yet) */
// @stub 0x167e86
void __stdcall function_167e86(long user_index, long weapon_slot)
{
}

// @stub 0x166d75
void __stdcall function_166d75(long user_index)
{
}

/* lane S's region */
// @stub 0x105c20
void __stdcall function_105c20(long weapon_index, long animation_name)
{
}

