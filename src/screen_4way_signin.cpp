// @flags /O1 /Gr
/* SCREEN_4WAY_SIGNIN.CPP: the screen where up to four users sign in (vtable
   0x45a490, screen 0x1e); its mode says what it signs them in for */

#include "cseries.h"
#include "screen_widgets.h"
#include "unknown_234c64.h"
#include "unknown_19b510.h"
#include "unknown_19b516.h"

long function_1480ff(long screen_id);

class c_4way_signin_screen : public c_class_1473c9
{
public:
	c_4way_signin_screen(long a, long b, word user_flags);

	virtual void v18(void *parameters);
	virtual screen_load_proc get_load_proc();

	long mode;
};

// @retail 0x2524cc
c_4way_signin_screen::c_4way_signin_screen(long a, long b, word user_flags) :
	c_class_1473c9(0x1e, a, b, user_flags),
	mode(1)
{
}

// @retail 0x2523e3
c_4way_signin_screen *signin_screen_new(s_screen_parameters *parameters)
{
	c_4way_signin_screen *screen = new c_4way_signin_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	return screen;
}

// @retail 0x2524a8
c_class_1473c9 *__stdcall function_2524a8(s_screen_parameters *parameters)
{
	c_4way_signin_screen *screen = signin_screen_new(parameters);

	screen->mode = 0;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x25240c
c_class_1473c9 *__stdcall function_25240c(s_screen_parameters *parameters)
{
	c_4way_signin_screen *screen = signin_screen_new(parameters);

	screen->mode = 1;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x252433
c_class_1473c9 *__stdcall function_252433(s_screen_parameters *parameters)
{
	c_4way_signin_screen *screen = signin_screen_new(parameters);

	screen->mode = 2;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x25245a
c_class_1473c9 *__stdcall function_25245a(s_screen_parameters *parameters)
{
	c_4way_signin_screen *screen = signin_screen_new(parameters);

	screen->mode = 3;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x252481
c_class_1473c9 *__stdcall function_252481(s_screen_parameters *parameters)
{
	c_4way_signin_screen *screen = signin_screen_new(parameters);

	screen->mode = 4;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x252b2c
screen_load_proc c_4way_signin_screen::get_load_proc()
{
	switch (mode)
	{
	case 0:
		return function_2524a8;
	case 1:
		return function_25240c;
	case 2:
		return function_252433;
	case 3:
		return function_25245a;
	}
	return function_252481;
}

/* builds the screen; no user is signing in yet */
// @retail 0x2524f8
void c_4way_signin_screen::v18(void *parameters)
{
	volatile long definition_index = function_1480ff(screen_id);
	s_screen_layout layout =
	{
		0,
		1,
		{
			{ 0, 0, 0, 0 }
		}
	};

	build(&layout);
	c_class_1a2c81::v1();
	g_54d598.m1224 = NONE;
	g_54d598.m1220 = false;
}

c_class_1473c9 *__stdcall function_22f11e(s_screen_parameters *parameters);
short player_slot_count_active(void);
bool function_6c7e0();
void function_1905bf(long controller, bool flag);

/* back to the press start screen */
// @retail 0x252a32
bool __stdcall function_252a32(long controller)
{
	s_screen_parameters parameters;

	parameters.field_c = 0;
	function_149f49((s_message *)&parameters, 0, 0, (word)(1 << controller), 5, 4, (long)function_22f11e);
	parameters.load(&parameters);
	return true;
}

/* A signs the controller in; B asks whether to go back when no one is
   signed in */
// @retail 0x252a67
bool function_252a67(c_4way_signin_screen *screen, s_widget_event *event)
{
	long param = event->param;

	if (param == 0 || param == 12)
	{
		bool live = function_6c7e0() || screen->mode == 4;

		function_1905bf(event->controller_index, live);
	}
	else if ((param == 1 || param == 13) && player_slot_count_active() == 0)
	{
		dialog_choice_show(3, 0x7e, 4, (word)(1 << event->controller_index), function_252a32, 0, 0);
	}
	return true;
}
