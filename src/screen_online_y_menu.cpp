// @flags /O1 /Gr
/* SCREEN_ONLINE_Y_MENU.CPP: the online Y menu, its three tabs (the friends,
   the players met and the recent players) and the user's pending online
   messages */

#include "cseries.h"
#include "screen_widgets.h"
#include "user_interface_lists.h"
#include "unknown_234c64.h"
#include "screen_online_y_menu.h"

struct s_message;
void function_149f49(s_message *message, word a, dword *id, word b, long c, long d, long e);
void function_236299(long sound);
void function_148523();
void voice_initialize_menu_pool(void);
void voice_dispose_menu_pool(void);
void function_148995(s_window_manager_e94 *value);
void friends_list_reset(bool dispose);
struct s_friend_request
{
	byte data[0x6a2];
};
bool friend_request_get(s_friend_request *request);

extern byte g_54d5a8;

c_screen_widget *__stdcall function_2312c2(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2313a8(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_231995(s_screen_parameters *parameters);

/* ---- the tabs ---- */

// @retail 0x23148e
c_y_menu_tab_screen::c_y_menu_tab_screen(long a, long b, word user_flags) :
	c_screen_widget(g_54d5a8 == 2 ? 0x1a : 0x1c, a, b, user_flags),
	value610(false)
{
}

// @retail 0x2312b4
void c_y_menu_tab_screen::v18(void *parameters)
{
	c_user_interface_widget::v1();
}

// @retail 0x23169c
c_y_menu_friends_screen::c_y_menu_friends_screen(long a, long b, word user_flags) :
	c_y_menu_tab_screen(a, b, user_flags),
	value814(NONE),
	list(user_flags)
{
}

/* the players tab's destructors are the friends tab's */
// @retail 0x2318a0 deleting c_y_menu_friends_screen
// @retail 0x2318bc destructor c_y_menu_friends_screen
// @retail 0x2316d0 destructor c_y_menu_friends_list

/* a press of X opens the friends options; back, B and start close the menu */
// @retail 0x231703
bool c_y_menu_friends_screen::v10(s_widget_event *event)
{
	if (event->type == 5)
	{
		switch (event->param)
		{
		case 1:
		case 3:
		case 0xd:
			function_148523();
			return true;
		case 2:
		{
			s_screen_parameters parameters;
			c_friends_options_screen *screen;

			parameters.field_c = 0;
			function_149f49((s_message *)&parameters, 0, 0, 1 << event->controller_index, 3, 4, (long)function_2313a8);
			screen = (c_friends_options_screen *)parameters.load(&parameters);
			if (screen)
			{
				list.name[0] = 0;
				screen->list.name = list.name;
				screen->list.entries = list.entries;
				screen->list.entry_count = 100;
				screen->list.source = list.data;
			}
			return true;
		}
		}
	}
	return c_screen_widget::v10(event);
}

// @retail 0x231865
c_y_menu_players_screen::c_y_menu_players_screen(long a, long b, word user_flags) :
	c_y_menu_tab_screen(a, b, user_flags),
	value814(NONE),
	list(user_flags)
{
	value610 = true;
}

/* a press of X opens the clan options when a friend request is pending */
// @retail 0x2318d1
bool c_y_menu_players_screen::v10(s_widget_event *event)
{
	if (event->type == 5)
	{
		switch (event->param)
		{
		case 1:
		case 3:
		case 0xd:
			function_148523();
			return true;
		case 2:
		{
			s_friend_request request;

			if (friend_request_get(&request))
			{
				s_screen_parameters parameters;
				c_clan_options_screen *screen;

				parameters.field_c = 0;
				function_149f49((s_message *)&parameters, 0, 0, 1 << event->controller_index, 3, 4, (long)function_2312c2);
				screen = (c_clan_options_screen *)parameters.load(&parameters);
				if (screen)
				{
					list.name[0] = 0;
					screen->list.name = list.name;
					screen->list.entries = list.entries;
					screen->list.entry_count = 100;
					screen->list.source = list.data;
				}
			}
			else
			{
				function_236299(2);
			}
			return true;
		}
		}
	}
	return c_screen_widget::v10(event);
}

// @retail 0x23179c
c_y_menu_recent_players_screen::c_y_menu_recent_players_screen(long a, long b, word user_flags) :
	c_y_menu_tab_screen(a, b, user_flags),
	value814(NONE),
	list(user_flags)
{
}

// @retail 0x2317d2 deleting c_y_menu_recent_players_screen
// @retail 0x231826 destructor c_y_menu_recent_players_screen
// @retail 0x2317f0 destructor c_y_menu_recent_players_list

// @retail 0x23183b
bool c_y_menu_recent_players_screen::v10(s_widget_event *event)
{
	if (event->type == 5)
	{
		switch (event->param)
		{
		case 1:
		case 3:
		case 0xd:
			function_148523();
			return true;
		}
	}
	return c_screen_widget::v10(event);
}

/* ---- the menu ---- */

// @retail 0x231995
c_screen_widget *__stdcall function_231995(s_screen_parameters *parameters)
{
	c_online_y_menu_screen *screen = new c_online_y_menu_screen(parameters->a, parameters->b, parameters->user_flags);

	if (screen)
	{
		screen->m6c = true;
		screen->function_147f6d(parameters);
	}
	return screen;
}

// @retail 0x2319d6
c_online_y_menu_screen::c_online_y_menu_screen(long a, long b, word user_flags) :
	c_screen_widget(g_54d5a8 == 2 ? 0x1a : 0x1c, a, b, user_flags),
	controller_index(NONE),
	tab_bar(user_flags),
	friends(a, b, user_flags),
	players(a, b, user_flags),
	recent_players(a, b, user_flags),
	message_count(0)
{
	voice_initialize_menu_pool();
}

// @retail 0x231a7b
c_online_y_menu_screen::~c_online_y_menu_screen()
{
	voice_dispose_menu_pool();
}

// @retail 0x231a5d deleting c_online_y_menu_screen

// @retail 0x2312bc
screen_load_proc c_online_y_menu_screen::get_load_proc()
{
	return function_231995;
}

// @retail 0x231c8c
void c_online_y_menu_screen::v2()
{
	c_user_interface_widget::v2();
	function_148995(0);
	friends_list_reset(true);
}

// @retail 0x231ac4
void c_online_y_menu_screen::v17()
{
	c_screen_widget *tab;

	tab = &friends;
	tab->v17();
	tab = &players;
	tab->v17();
	tab = &recent_players;
	tab->v17();
}
