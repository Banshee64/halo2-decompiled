// @flags /O1 /arch:SSE /Gr
/* UNKNOWN_2300CF.CPP: the screen widget's own methods and the screen with a
   list (the vtables at 0x458840 and 0x4588c0); the widget and screen
   constructors they build on (0x22e27b, 0x22f5ca) are in unknown_22e27b.cpp */

#include "cseries.h"
#include "screen_widgets.h"
#include "unknown_19b516.h"
#include "globals.h"

struct s_screen_definition
{
	byte unknown00[0x1c];
	long string_list_index;
};

long function_1480ff(long screen_id);
void unicode_string_list_get_string(long tag_index, long string_id, word *buffer);
void function_22fba9(c_screen_widget *screen);
void __stdcall function_22fc08(c_screen_widget *screen);

/* the screen's definition tag */
// @retail 0x22f871
s_screen_definition *function_22f871(c_screen_widget *screen)
{
	s_screen_definition *definition = 0;
	long tag_index = function_1480ff(screen->screen_id);

	if (tag_index != NONE)
	{
		definition = (s_screen_definition *)g_4e3b44[tag_index & 0xffff].bytes;
	}
	return definition;
}

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
		s_screen_definition *definition = function_22f871((c_screen_widget *)this);
		if (definition)
			unicode_string_list_get_string(definition->string_list_index, string_id, buffer);
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
