// @flags /O1 /Oi /Gr
/* UNKNOWN_24F8C0.CPP: two screens of the network code (vtables 0x45a250 and
   0x45a2c0) */

#include <string.h>
#include "cseries.h"
#include "screen_widgets.h"
#include "unknown_19b516.h"

long function_199cfc(void);
word function_1901fc(void);
void network_session_manager_check_joining_leader(void);
bool network_session_manager_is_joining(void);
bool function_22f0ff(c_widget *widget);

/* shows an error message (not decompiled yet) */
void __stdcall function_19b527(long a, long string_id, long b, word user_flags, long c, long d);

c_screen_widget *__stdcall function_24f8c6(s_screen_parameters *parameters);

/* the screen of vtable 0x45a250 (screen 0xcc), shown while the network
   session is lost */
class c_screen_45a250 : public c_screen_widget
{
public:
	c_screen_45a250(long a, long b, word user_flags);

	virtual void v3();
	virtual screen_load_proc get_load_proc();
};

// @retail 0x24f8c0
screen_load_proc c_screen_45a250::get_load_proc()
{
	return function_24f8c6;
}

// @retail 0x24f8c6
c_screen_widget *__stdcall function_24f8c6(s_screen_parameters *parameters)
{
	c_screen_45a250 *screen = new c_screen_45a250(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x24f902
c_screen_45a250::c_screen_45a250(long a, long b, word user_flags) :
	c_screen_widget(0xcc, a, b, user_flags)
{
}

/* shows the error of the session, if any, and leaves the screen once the
   session no longer joins */
// @retail 0x24f927
void c_screen_45a250::v3()
{
	if (!function_22f0ff((c_widget *)this))
	{
		long string_id = NONE;
		bool leave = false;

		switch (function_199cfc())
		{
		case 0:
		case 1:
			break;
		case 2:
			string_id = 0x3d;
			break;
		case 3:
			leave = true;
			break;
		case 4:
			string_id = 0x3e;
			leave = true;
			break;
		case 5:
			string_id = 0x3f;
			leave = true;
			break;
		case 6:
			string_id = 0x40;
			leave = true;
			break;
		case 7:
			string_id = 0x41;
			leave = true;
			break;
		case 8:
			string_id = 0x42;
			leave = true;
			break;
		case 9:
			string_id = 0x43;
			leave = true;
			break;
		case 10:
			string_id = 0x45;
			leave = true;
			break;
		default:
			string_id = 0x44;
			leave = true;
			break;
		}

		if (string_id != NONE)
			function_19b527(1, string_id, 4, function_1901fc(), 0, 0);
		if (leave)
			network_session_manager_check_joining_leader();
		if (!network_session_manager_is_joining())
			start_animation(3);
	}

	((c_widget *)this)->function_22e391();
}

/* opens the screen */
// @retail 0x24f9d4
void function_24f9d4()
{
	s_screen_parameters parameters;

	parameters.field_c = 0;
	parameters.type = 0;
	parameters.user_flags = 0;
	parameters.a = 3;
	parameters.b = 4;
	memset(parameters.id, NONE, sizeof(parameters.id));
	parameters.load = function_24f8c6;
	function_24f8c6(&parameters);
}
