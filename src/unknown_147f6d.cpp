// @flags /O1 /Gr
/* UNKNOWN_147F6D.CPP: the screen windows (0x54d598..), where a newly loaded
   screen is placed.

   The window manager object at 0x54d598 (constructed by 0x147645) holds the
   windows of each channel: three per-controller arrays of five (the fifth,
   index 4, is shared by all controllers) and three single windows that only
   take index 4. 0x148262 maps a channel and an index to one of them. */

#include "cseries.h"
#include "screen_widgets.h"

/* a screen window; slot 0 initializes it, slot 6 takes a new screen */
class c_screen_window
{
public:
	virtual void initialize() {}
	virtual void v1() {}
	virtual void v2() {}
	virtual void v3() {}
	virtual void v4() {}
	virtual void v5() {}
	virtual void add_screen(c_screen_widget *screen, s_screen_parameters *parameters) {}
	virtual void v7() {}

	long index;
};

/* the windows of the per-controller arrays (vtables 0x4599a8 and 0x4599dc) */
class c_screen_window_40 : public c_screen_window
{
public:
	byte unknown08[0x40 - 0x8];
};

class c_screen_window_38 : public c_screen_window
{
public:
	byte unknown08[0x38 - 0x8];
};

c_screen_window_40 g_54d62c[5];
c_screen_window_38 g_54d76c[5];
c_screen_window_38 g_54d884[5];
c_screen_window g_54d99c;
c_screen_window g_54d9e0;
c_screen_window g_54dba8;

void function_236299(long sound);

// @retail 0x147f6d
void c_screen_widget::function_147f6d(s_screen_parameters *parameters)
{
	long index = v21();
	c_screen_window *window;

	switch (parameters->a)
	{
	case 0:
		window = index == 4 ? &g_54d99c : 0;
		break;
	case 1:
		window = &g_54d884[index];
		break;
	case 2:
		window = index == 4 ? &g_54dba8 : 0;
		break;
	case 3:
		window = &g_54d76c[index];
		break;
	case 4:
		window = index == 4 ? &g_54d9e0 : 0;
		break;
	case 5:
		window = &g_54d62c[index];
		break;
	default:
		window = 0;
		break;
	}

	if (window)
	{
		if (!(parameters->type & 1) && screen_id != 9)
		{
			function_236299(3);
		}
		window->add_screen(this, parameters);
	}
	else
	{
		this->~c_screen_widget();
		user_interface_free(this);
	}
}
