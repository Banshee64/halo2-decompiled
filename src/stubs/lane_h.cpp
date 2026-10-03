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

// @stub 0x159130
void function_159130(long score, unsigned short *buffer)
{
}

// @stub 0x23ef80
void function_23ef80(long sound_index, long delay, s_event *event, bool flag)
{
}
/* the button widget of unknown_19b510.h, declared here without its base so the
   stub constructs nothing (a call from this file, built without /GL, would
   give the widget constructors their standard convention) */
class c_dialog_button
{
public:
	c_dialog_button(short index, unsigned short user_flags);
};

// @stub 0x253c8b
c_dialog_button::c_dialog_button(short index, unsigned short user_flags)
{
}

struct s_dialog_definition;

// @stub 0x23661f
void function_23661f(s_dialog_definition *definition, long dialog_id)
{
}

struct s_screen_parameters;
class c_screen_widget;

// @stub 0x18f42d
c_screen_widget *__stdcall function_18f42d(s_screen_parameters *parameters)
{
	return 0;
}

// @stub 0x18f474
c_screen_widget *__stdcall function_18f474(s_screen_parameters *parameters)
{
	return 0;
}

// @stub 0x2365f7
bool function_2365f7(void)
{
	return false;
}
