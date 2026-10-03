// @flags /O1 /Gr
/* UNKNOWN_147F6D.CPP: the screen windows (0x54d598..), where a newly loaded
   screen is placed.

   The window manager object at 0x54d598 (constructed by 0x147645) holds the
   windows of each channel: three per-controller arrays of five (the fifth,
   index 4, is shared by all controllers) and three single windows that only
   take index 4. 0x148262 maps a channel and an index to one of them. */

#include "cseries.h"
#include "screen_widgets.h"
#include "unknown_19b516.h"
#include "globals.h"

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
	byte unknown08[0x34 - 0x8];
	c_screen_widget *screen;
};

/* the windows of the per-controller arrays (vtables 0x4599a8 and 0x4599dc) */
class c_screen_window_40 : public c_screen_window
{
public:
	byte unknown38[0x40 - 0x38];
};

class c_screen_window_38 : public c_screen_window
{
};

c_screen_window_40 g_54d62c[5];
c_screen_window_38 g_54d76c[5];
c_screen_window_38 g_54d884[5];
c_screen_window g_54d99c;
c_screen_window g_54d9e0;
c_screen_window g_54dba8;
c_screen_window g_54d5c4;

/* the screen settings copied out by function_14887e (0x78 bytes) */
struct s_screen_settings_54dc6c
{
	dword data[0x1e];
};

s_screen_settings_54dc6c g_54dc6c;
long g_47ff54;

void *function_1482e8(void);

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

/* the screen definitions of the user interface globals tag: a block of tag
   references whose tags start with their screen id at +4 */
struct s_screen_reference
{
	dword group_tag;
	long tag_index;
};

struct s_screen_references
{
	byte unknown00[8];
	long count;
	s_screen_reference *references;
};

struct s_screen_definition
{
	byte unknown00[4];
	short screen_id;
};

// @retail 0x147f4f
long function_147f4f()
{
	long result = NONE;

	if (g_54d62c[4].screen)
	{
		c_screen_widget *screen = g_54d62c[4].screen->get_screen();
		if (screen)
		{
			result = screen->screen_id;
		}
	}
	return result;
}

// @retail 0x148098
long function_148098(long screen_id)
{
	s_screen_references *references = (s_screen_references *)function_1482e8();
	long result = NONE;

	if (references)
	{
		for (long i = 0; i < references->count; i++)
		{
			s_screen_reference *reference = &references->references[i];
			if (reference->tag_index != NONE &&
				((s_screen_definition *)g_4e3b44[reference->tag_index & 0xffff].bytes)->screen_id == screen_id)
			{
				result = reference->tag_index;
				break;
			}
		}
	}
	return result;
}

// @retail 0x1480ff
long function_1480ff(long screen_id)
{
	long result = function_148098(screen_id);

	if (result == NONE && screen_id != g_47ff54)
	{
		g_47ff54 = screen_id;
	}
	return result;
}

// @retail 0x148262
c_screen_window *function_148262(long channel, long index)
{
	switch (channel)
	{
	case 0:
		return index == 4 ? &g_54d99c : 0;
	case 1:
		return &g_54d884[index];
	case 2:
		return index == 4 ? &g_54dba8 : 0;
	case 3:
		return &g_54d76c[index];
	case 4:
		return index == 4 ? &g_54d9e0 : 0;
	case 5:
		return &g_54d62c[index];
	}
	return &g_54d5c4;
}

// @retail 0x14887e
void function_14887e(s_screen_settings_54dc6c *settings)
{
	if (settings)
	{
		*settings = g_54dc6c;
	}
}

/* the screen a window shows */
// @retail 0x148d91
c_screen_widget *function_148d91(long channel, long index)
{
	c_screen_window *window;

	switch (channel)
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
		window = &g_54d5c4;
		break;
	}
	return window->screen;
}
