// @flags /O1 /arch:SSE /Gr
/* UNKNOWN_2300CF.CPP: the screen widget's own methods and the screen with a
   list (the vtables at 0x458840 and 0x4588c0); the constructors of the user
   interface widget (0x22e27b) and of the screen widget (0x22f5ca) are here
   too, decompiled by lane G for the screens, because a source that marks a
   class's constructor is the only one whose stand-ins do not copy-construct
   it */

#include "cseries.h"
#include <string.h>
#include "screen_widget.h"

struct s_screen_definition
{
	byte unknown00[0x1c];
	long string_list_index;
};

struct s_event_view
{
	long type;
	long unknown04;
	long param;
};

s_screen_definition *function_22f871(c_widget *widget);
/* 0x1a0180 takes the string list in eax, the string in edi and the buffer on
   the stack; src/stubs/game_engine.cpp stubs it with two arguments */
void function_1a0180(long a, long b);
void function_22fba9(c_screen_widget *screen);
void __stdcall function_22fc08(c_screen_widget *screen);
void function_14800c(long window_type, long index);

// @retail 0x2300ea
bool c_screen_widget::v10(s_event *event)
{
	s_event_view *view = (s_event_view *)event;
	if (view->type == 5)
	{
		switch (view->param)
		{
		case 1:
		case 13:
			function_14800c(v20(), v21());
			return true;
		}
	}
	return ((c_widget *)this)->function_22ec73(event);
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
	short value = m5f3 + *delta;
	function_22fba9(this);
	m5f0 = value;
	function_22fc08(this);
}

// @retail 0x230451
c_screen_with_menu::c_screen_with_menu(long id, long value, long data, short index, void *list) :
	c_screen_widget(id, value, data, index),
	list(list)
{
}

/* the event the widget constructor sends itself (0x34 bytes) */
struct s_widget_event
{
	long type;
	byte unknown04[8];
	short value;
	byte unknown0e[6];
	long data;
	byte unknown18[0x1c];
};

/* the default bounds of a screen */
short g_485a92[4];

// @retail 0x22e27b
c_user_interface_widget::c_user_interface_widget(long type, short index) :
	ma(NONE),
	identifier(NONE),
	type(type),
	m8(index),
	parent(0),
	child(0),
	next(0),
	prev(0),
	m68(NONE),
	m6a(0),
	allocated(false),
	m6d(true),
	m6e(true)
{
	memset(bounds, 0, sizeof(bounds));
	memset(scale, 0, sizeof(scale));
	memset(bounds, 0, sizeof(bounds));
	scale[0] = 1.f;

	s_widget_event event;
	memset(&event, 0, sizeof(event));
	event.type = NONE;
	scale[2] = 1.f;
	scale[1] = 1.f;
	event.value = 1;
	event.data = 0;
	((c_widget *)this)->function_22e89c((s_event *)&event);
}

// @retail 0x22f5ca
c_screen_widget::c_screen_widget(long id, long value, long data, short index) :
	c_user_interface_widget(0, index),
	m7c(NONE),
	m70(id),
	m74(value),
	m78(data),
	member80(0),
	member338(0),
	m5f0(NONE),
	m5f2(0),
	m5f3(0),
	m5f4(0),
	callback(this, &c_screen_widget::function_230427)
{
	m7c++;
	type = 0;
	identifier = m7c;
	m6d = true;
	*(long *)&bounds[0] = *(long *)&g_485a92[0];
	*(long *)&bounds[2] = *(long *)&g_485a92[2];
}
