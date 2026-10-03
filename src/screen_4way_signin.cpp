// @flags /O1 /Gr
/* SCREEN_4WAY_SIGNIN.CPP: the screen where up to four users sign in (vtable
   0x45a490, screen 0x1e); its mode says what it signs them in for */

#include "cseries.h"
#include "screen_widgets.h"
#include "unknown_234c64.h"

long function_1480ff(long screen_id);

class c_4way_signin_screen : public c_screen_widget
{
public:
	c_4way_signin_screen(long a, long b, word user_flags);

	virtual void v18(void *parameters);
	virtual screen_load_proc get_load_proc();

	long mode;
};

// @retail 0x2524cc
c_4way_signin_screen::c_4way_signin_screen(long a, long b, word user_flags) :
	c_screen_widget(0x1e, a, b, user_flags),
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
c_screen_widget *__stdcall function_2524a8(s_screen_parameters *parameters)
{
	c_4way_signin_screen *screen = signin_screen_new(parameters);

	screen->mode = 0;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x25240c
c_screen_widget *__stdcall function_25240c(s_screen_parameters *parameters)
{
	c_4way_signin_screen *screen = signin_screen_new(parameters);

	screen->mode = 1;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x252433
c_screen_widget *__stdcall function_252433(s_screen_parameters *parameters)
{
	c_4way_signin_screen *screen = signin_screen_new(parameters);

	screen->mode = 2;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x25245a
c_screen_widget *__stdcall function_25245a(s_screen_parameters *parameters)
{
	c_4way_signin_screen *screen = signin_screen_new(parameters);

	screen->mode = 3;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x252481
c_screen_widget *__stdcall function_252481(s_screen_parameters *parameters)
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
	c_user_interface_widget::v1();
	g_54d598.m1224 = NONE;
	g_54d598.m1220 = false;
}
