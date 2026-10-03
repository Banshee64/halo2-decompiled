// @flags /O1 /Gr
/* UNKNOWN_250155.CPP: the screens and lists of 0x250155..0x2541b2 (the
   matchmaking screens) */

#include "cseries.h"
#include "data_array.h"
#include "screen_widgets.h"
#include "unknown_19b516.h"

bool function_1999b3(void);
long function_19a161(void);

/* the screen of 0x24fd74 (not written yet) */
class c_screen_24fd74 : public c_screen_widget
{
public:
	void function_250155();

	byte unknown610[0x142c - 0x610];
	short mode;
};

/* shows the network connection's state in the mode's text */
// @retail 0x250155
void c_screen_24fd74::function_250155()
{
	long index;

	switch (mode)
	{
	case 0:
		index = 12;
		break;
	case 1:
		index = 7;
		break;
	case 2:
		index = 12;
		break;
	default:
		index = NONE;
		break;
	}

	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)find_text((short)index);
	if (text)
	{
		if (function_1999b3())
		{
			text->value6e = false;
		}
		else
		{
			text->value6e = true;
			switch (function_19a161())
			{
			case 0:
				text->set_string(0x4000201);
				break;
			case 1:
				text->set_string(0xf000202);
				break;
			case 2:
				text->set_string(0x6000203);
				break;
			}
		}
	}
}

/* "matchmaking list" (vtable 0x45a340): sixteen items, sorted by slot 3 */
class c_matchmaking_list : public c_list_widget
{
public:
	c_matchmaking_list(word user_flags);

	virtual void v1();

	c_list_item_widget items[16];
};

// @retail 0x251469
c_matchmaking_list::c_matchmaking_list(word user_flags) :
	c_list_widget(user_flags)
{
}

// @retail 0x2514d7
void c_matchmaking_list::v1()
{
	data = user_interface_data_new("matchmaking list", 16, 4);
	if (data)
	{
		data_make_valid(data);
	}
	((c_widget *)this)->c_widget::v9();
}

// @retail 0x251497 deleting c_matchmaking_list
// @retail 0x2514b5 destructor c_matchmaking_list

c_screen_widget *__stdcall function_2519bb(s_screen_parameters *parameters);
long network_time_get(void); /* 0x75870, network_observer.cpp */

bool g_51ec99;

/* the matchmaking screen (vtable 0x45a398) */
class c_matchmaking_screen : public c_screen_widget
{
public:
	c_matchmaking_screen(long a, long b, word user_flags);

	virtual screen_load_proc get_load_proc();

	long value610;
	long value614;
	long value618;
	long value61c;
	bool value620;
	s_text_256 texts[6];
	short value1222;
	byte unknown1224[0x1424 - 0x1224];
	c_matchmaking_list list;
};

// @retail 0x25137e
screen_load_proc c_matchmaking_screen::get_load_proc()
{
	return function_2519bb;
}

// @retail 0x2519bb
c_screen_widget *__stdcall function_2519bb(s_screen_parameters *parameters)
{
	c_matchmaking_screen *screen = new c_matchmaking_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2519f7
c_matchmaking_screen::c_matchmaking_screen(long a, long b, word user_flags) :
	c_screen_widget(0xd2, a, b, user_flags),
	value610(0),
	value618(0),
	value61c(network_time_get()),
	value620(false),
	value1222(0),
	list(user_flags)
{
	g_51ec99 = false;
}

// @retail 0x251a73 deleting c_matchmaking_screen
// @retail 0x251a91 destructor c_matchmaking_screen
