// @flags /O1 /Gr
/* UNKNOWN_250155.CPP: the screens and lists of 0x250155..0x2541b2 (the
   matchmaking screens) */

#include "cseries.h"
#include "data_array.h"
#include "screen_widgets.h"
#include "unknown_19b510.h"
#include "unknown_19b516.h"
#include "unknown_234c64.h"

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

	virtual void v18(void *parameters);
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

struct s_widget_view_2b0a;
void function_2b0a14(s_widget_view_2b0a *widget, short index);
bool voice_port_flag0_only(long port);
bool function_53750(long player_index);

/* the item's voice icon hides */
// @retail 0x251963
void function_251963(c_user_interface_widget *item)
{
	c_user_interface_widget *bitmap = item->find_child(8, 0, false);

	if (bitmap)
	{
		bitmap->value6e = false;
	}
}

/* the item's voice icon shows the player's voice state */
// @retail 0x251977
void function_251977(long player, c_user_interface_widget *item)
{
	c_user_interface_widget *bitmap = item->find_child(8, 0, false);

	if (bitmap && player != NONE)
	{
		short index;

		if (voice_port_flag0_only(player))
		{
			index = !function_53750(player);
		}
		else
		{
			index = 2;
		}
		function_2b0a14((s_widget_view_2b0a *)bitmap, index);
		bitmap->value6e = true;
	}
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

/* a message box callback that accepts */
// @retail 0x2523b7
bool __stdcall function_2523b7(long controller)
{
	return true;
}

/* ---- the network squad browser ---- */

/* "network squad list" (vtable 0x45a500): the squads found on the system
   link */
class c_network_squad_list : public c_list_widget
{
public:
	c_network_squad_list(word user_flags, bool alternate);

	/* forgets the focused squad */
	virtual void v2();
	/* folded with c_widget's v2 */
	virtual void *get_item_data() { return items; }

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[5];
	long value308;
	c_list_item_handler handler;
	bool searching;
	bool alternate;
};

/* the network squad browser screen (vtable 0x45a558) */
class c_network_squad_browser_screen : public c_screen_with_menu
{
public:
	c_network_squad_browser_screen(long a, long b, word user_flags, bool alternate);

	virtual screen_load_proc get_load_proc();

	c_network_squad_list list;
	bool alternate;
	bool value93d;
	bool value93e;
};

void __stdcall function_252ed8(void *list);
bool function_199df9(bool offline, bool system_link);
void function_199a03(long mode);
void function_199c47(long index);
bool function_199ba5(long index);
byte *function_199bbf(long index);
word function_1901fc(void);
void function_236299(long sound);

/* a datum of the list: a squad, or the item that creates one */
struct s_network_squad_datum
{
	short salt;
	short unknown02;
	long index;
	bool create;
	byte unknown09[3];
};

/* a squad found on the system link */
struct s_network_squad
{
	byte unknown00[0xbc];
	short player_count;
};

/* the squad focused last */
long g_470b18 = NONE;

// @retail 0x252b72
c_network_squad_list::c_network_squad_list(word user_flags, bool alternate) :
	c_list_widget(user_flags),
	value308(0),
	handler(this, (list_item_method)&c_network_squad_list::handle_item)
{
	g_470b18 = NONE;
	searching = false;
	this->alternate = alternate;
	data = user_interface_data_new("network squad list", 0x21, 0xc);
	data_make_valid(data);
	function_252ed8(this);
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2530ec
void c_network_squad_list::handle_item(s_controller_reference **controller, long *item)
{
	if (data && *item != NONE)
	{
		s_network_squad_datum *datum = &((s_network_squad_datum *)data->data)[*item & 0xffff];

		if (datum->create)
		{
			if (function_199df9(0, alternate))
			{
				function_199a03(2);
			}
		}
		else
		{
			long index = datum->index;
			s_network_squad *squad;

			if (index != NONE && function_199ba5(index) && (squad = (s_network_squad *)function_199bbf(index)) != 0)
			{
				if (squad->player_count >= 16)
				{
					dialog_ok_show(1, 0x41, 4, function_1901fc(), 0, 0);
				}
				else
				{
					function_199c47(index);
					return;
				}
			}
			function_236299(2);
		}
	}
}

// @retail 0x252c0a deleting c_network_squad_list
// @retail 0x252c28 destructor c_network_squad_list

// @retail 0x252c5e
void c_network_squad_list::v2()
{
	g_470b18 = NONE;
	((c_widget *)this)->c_widget::v10();
}

c_screen_widget *__stdcall function_253185(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2531c9(s_screen_parameters *parameters);

// @retail 0x253185
c_screen_widget *__stdcall function_253185(s_screen_parameters *parameters)
{
	c_network_squad_browser_screen *screen = new c_network_squad_browser_screen(parameters->a, parameters->b, parameters->user_flags, false);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2531c9
c_screen_widget *__stdcall function_2531c9(s_screen_parameters *parameters)
{
	c_network_squad_browser_screen *screen = new c_network_squad_browser_screen(parameters->a, parameters->b, parameters->user_flags, true);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x253221
c_network_squad_browser_screen::c_network_squad_browser_screen(long a, long b, word user_flags, bool alternate) :
	c_screen_with_menu(0xd, a, b, user_flags, &list),
	list(user_flags, alternate)
{
	this->alternate = alternate;
	value93d = false;
	value93e = false;
}

// @retail 0x253268 deleting c_network_squad_browser_screen
// @retail 0x253286 destructor c_network_squad_browser_screen

// @retail 0x25320d
screen_load_proc c_network_squad_browser_screen::get_load_proc()
{
	return alternate ? function_2531c9 : function_253185;
}


long function_1480ff(long screen_id);

// @retail 0x251aa6
void c_matchmaking_screen::v18(void *parameters)
{
	volatile long definition_index = function_1480ff(screen_id);
	s_screen_layout layout =
	{
		0,
		1,
		{
			{ 0, 0, &list, 0 }
		}
	};

	build(&layout);
	c_user_interface_widget::v1();
}

bool function_592f0(void);
byte function_199eaa(void);
bool function_199e7e(byte value);
/* (screen_multiplayer_pregame_lobby.cpp): retail inlined the window test of
   channel 3, index 4 */
// @retail 0x2507dc
void function_2507dc(void)
{
	if (function_592f0() && function_199eaa() && !function_1473b6(&g_54d598.windows_3[4]))
	{
		function_199e7e(0);
	}
}
