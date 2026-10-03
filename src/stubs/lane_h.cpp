// stubs for the game functions outside 0x190000..0x19ffff that lane H's code
// calls and that are not decompiled yet

#include <xtl.h>
#include <xonline.h>

struct s_controller_profile;
struct s_controller_event;

// @stub 0x18fc44
void function_18fc44(long controller, s_controller_profile *profile, long *profile_index)
{
}

// @stub 0x18fcc4
void __stdcall function_18fcc4(long controller, s_controller_profile *profile, long profile_index)
{
}

// @stub 0x18fc08
void function_18fc08(long controller)
{
}

// @stub 0x147dbe
void function_147dbe(s_controller_event *event)
{
}

// @stub 0x148f36
bool __stdcall function_148f36(long controller)
{
	return false;
}

// @stub 0x1a0660
bool function_1a0660(long profile_index, s_controller_profile *profile)
{
	return false;
}

// @stub 0x2153dd
void __stdcall function_2153dd(long controller, long profile_index, s_controller_profile *profile, long value)
{
}

// @stub 0xabc70
long function_abc70(long index, XONLINE_USER *user)
{
	return 0;
}
