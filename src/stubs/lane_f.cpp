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
