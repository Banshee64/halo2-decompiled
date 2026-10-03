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

// @stub 0xb3610
bool function_b3610(void)
{
	return false;
}

// @stub 0xb3670
void function_b3670(void)
{
}

// @stub 0x59570
long function_59570(void)
{
	return 0;
}

// @stub 0x594a0
bool __stdcall function_594a0(long a, long b, long c)
{
	return false;
}

// @stub 0x1487c3
void function_1487c3(long a, long b, long load, long c, long d)
{
}

// @stub 0x63e90
long __stdcall function_63e90(long index)
{
	return 0;
}

// @stub 0x6cb60
void function_6cb60(void)
{
}

// @stub 0x139130
void function_139130(void)
{
}

// @stub 0x641a0
bool function_641a0(void)
{
	return false;
}

// @stub 0x7f0d0
void __stdcall function_7f0d0(const unsigned char *data)
{
}

// @stub 0x64610
long __stdcall function_64610(unsigned long *xuid)
{
	return 0;
}

// @stub 0x73b10
long __stdcall function_73b10(long a, long b)
{
	return 0;
}

// @stub 0x73ca0
void function_73ca0(unsigned char *results)
{
}

// @stub 0xb3e90
void __stdcall function_b3e90(unsigned char *results)
{
}

// @stub 0x232d77
void function_232d77(void)
{
}
