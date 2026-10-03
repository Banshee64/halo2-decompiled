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
struct s_sound_effect_definition;
struct s_sound_location;

// @stub 0x126000
long __stdcall function_126000(long tag_index, s_sound_effect_definition *definition, s_sound_play_state *state, long permutation_index)
{
	return NONE;
}

// @stub 0x126c30
bool __stdcall function_126c30(s_sound_play_state *state, long tag_index, s_sound_effect_definition **definition, long flags)
{
	return false;
}

// @stub 0x126ec0
long __stdcall function_126ec0(long tag_index, long *permutation_index)
{
	return 0;
}

// @stub 0x127f10
void __stdcall function_127f10(long sound_index, word *flags)
{
}

// @stub 0x21d630
void __stdcall function_21d630(long effect_index, long mode)
{
}

// @stub 0x18cbc0
void function_18cbc0(long looping_sound_index, s_sound_location *location)
{
}

// @stub 0x13d370
long __stdcall function_13d370(s_physical_object *physical, long size, long type)
{
	return NONE;
}

// @stub 0x21d110
long function_21d110(s_sound_play_state *state, long tag_index)
{
	return NONE;
}

// @stub 0x10cf50
bool function_10cf50(long object_index)
{
	return false;
}

// @stub 0x1d8f00
short function_1d8f00(long render_model_index, long marker_name)
{
	return NONE;
}

struct s_object_marker;

// @stub 0x1d8f50
short function_1d8f50(short marker_index, long render_model_index, real_matrix4x3 const *original_node_matrices, long a4, real_matrix4x3 const *node_matrices, bool mirrored, s_object_marker *markers, short maximum_count)
{
	return 0;
}
