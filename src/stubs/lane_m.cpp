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
