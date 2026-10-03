// stubs for the game functions outside 0x1a0000..0x1affff that lane M's code
// calls and that are not decompiled yet

#include "cseries.h"
#include "real_math.h"

// @stub 0x53750
bool function_53750(long index)
{
	return false;
}

// @stub 0xcafc0
void function_cafc0(long unit_index, real_point3d *position)
{
}

// @stub 0x254200
void function_254200(void)
{
}

// @stub 0x254490
void __stdcall function_254490(real_point2d const *point, real scale, real alpha, real_rgb_color const *color, bool pulse)
{
}

// @stub 0x2548f0
void __stdcall function_2548f0(real_point2d const *center, real scale)
{
}

// @stub 0x11e130
real function_11e130(real_point3d const *a0, real_vector3d const *a, real_point3d const *b0, real_vector3d const *b)
{
	return 0.0f;
}

struct s_slot;

// @stub 0x1a8c30
short __stdcall function_1a8c30(long actor_index, s_slot *slot)
{
	return 0;
}

// @stub 0x1a9400
bool __stdcall function_1a9400(long actor_index, s_slot *slot)
{
	return true;
}

// @stub 0x1a9760
void __stdcall function_1a9760(long actor_index, s_slot *slot)
{
}

// @stub 0x1aa750
bool __stdcall function_1aa750(long actor_index, s_slot *slot, bool flag)
{
	return false;
}

// @stub 0x1a9ec0
short __stdcall function_1a9ec0(long actor_index)
{
	return 0;
}

// @stub 0x1aa0d0
void __stdcall function_1aa0d0(long actor_index, s_slot *slot)
{
}

// @stub 0x1aab50
void __stdcall function_1aab50(long actor_index, s_slot *slot)
{
}
