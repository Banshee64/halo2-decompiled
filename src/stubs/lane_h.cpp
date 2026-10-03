// stubs for the game functions outside 0x190000..0x19ffff that lane H's code
// calls and that are not decompiled yet

#include <xtl.h>
#include <xonline.h>

struct s_player_profile;
struct s_controller_event;

// @stub 0x18fcc4
void __stdcall function_18fcc4(long controller, s_player_profile *profile, long profile_index)
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
bool function_1a0660(long profile_index, s_player_profile *profile)
{
	return false;
}

struct s_event;
struct s_event_response;

// @stub 0x53750
bool function_53750(long player_index)
{
	return false;
}

// @stub 0x22acb4
bool function_22acb4(long player_index)
{
	return false;
}

// @stub 0x13de30
void qsort_4byte(long *base, long count, void *unused, bool (__stdcall *compare)(long, long, void *), void *context)
{
}

// @stub 0x1587b0
long function_1587b0(long player_index)
{
	return 0;
}

// @stub 0x1587f0
long function_1587f0(long team)
{
	return 0;
}

// @stub 0x159130
void function_159130(long score, unsigned short *buffer)
{
}

// @stub 0x161be0
void function_161be0(long score, unsigned short *buffer)
{
}

// @stub 0x22d2ee
void __stdcall function_22d2ee(unsigned short *text, long maximum_count)
{
}

// @stub 0x23ef80
void function_23ef80(long sound_index, long delay, s_event *event, bool flag)
{
}