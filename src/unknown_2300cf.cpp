// @flags /O1 /arch:SSE /Gr
/* UNKNOWN_2300CF.CPP: the screen widget's own methods and the screen with a
   list (the vtables at 0x458840 and 0x4588c0); the widget and screen
   constructors they build on (0x22e27b, 0x22f5ca) are in unknown_22e27b.cpp */

#include "cseries.h"
#include "screen_widgets.h"
#include "unknown_19b516.h"

struct s_screen_definition
{
	byte unknown00[0x1c];
	long string_list_index;
};

s_screen_definition *function_22f871(c_widget *widget);
/* 0x1a0180 takes the string list in eax, the string in edi and the buffer on
   the stack; src/stubs/game_engine.cpp stubs it with two arguments */
void function_1a0180(long a, long b);
void function_22fba9(c_screen_widget *screen);
void __stdcall function_22fc08(c_screen_widget *screen);

// @retail 0x2300ea
bool c_screen_widget::v10(s_widget_event *event)
{
	if (event->type == 5)
	{
		switch (event->param)
		{
		case 1:
		case 13:
			function_14800c(v20(), v21());
			return true;
		}
	}
	return ((c_widget *)this)->function_22ec73((s_event *)event);
}

// @retail 0x230134
void c_widget::function_230134(long string_id, word *buffer)
{
	buffer[0] = 0;
	if (string_id != NONE)
	{
		s_screen_definition *definition = function_22f871(this);
		if (definition)
			function_1a0180(definition->string_list_index, (long)buffer);
	}
}

// @retail 0x230427
void c_screen_widget::function_230427(short *delta)
{
	short value = value5f3 + *delta;
	function_22fba9(this);
	value5f0 = value;
	function_22fc08(this);
}

// @retail 0x230451
c_screen_with_menu::c_screen_with_menu(long screen_id, long a, long b, word user_flags, void *list) :
	c_screen_widget(screen_id, a, b, user_flags),
	list(list)
{
}
