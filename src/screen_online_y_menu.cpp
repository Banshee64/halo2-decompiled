// @flags /O1 /arch:SSE /Gr
/* SCREEN_ONLINE_Y_MENU.CPP: the online Y menu, its three tabs (the friends,
   the players met and the recent players) and the user's pending online
   messages */

#include "cseries.h"
#include <xtl.h>
#include <xonline.h>
#include <string.h>
#include "globals.h"
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
bool friends_list_contains(XUID const *xuid);
bool players_list_contains(XUID const *xuid);
bool friends_list_task_running();
bool function_1a325a();
void function_1a31ff();
void function_1a303b(long controller_index);
void __stdcall online_messages_enumerate(DWORD controller_index, s_entry *entries, long *count);
void online_message_delete(DWORD controller_index, DWORD message_id, bool block_sender);
long function_1480ff(long screen_id);
void function_18ff47(long player, dword *out);
struct s_widget_view_2b0a;
void function_2b0a14(s_widget_view_2b0a *widget, short index);

static inline void widget_set_user_flags(c_class_1a2c81 *widget, word user_flags)
{
	widget->user_flags = user_flags;
}

/* a user's slot: set when the user's messages changed */
struct s_player_slot_messages_view
{
	byte unknown000[0x46d];
	bool messages_changed;
};

extern char g_54d5a8;

c_class_1473c9 *__stdcall function_2312c2(s_screen_parameters *parameters);
c_class_1473c9 *__stdcall function_2313a8(s_screen_parameters *parameters);
c_class_1473c9 *__stdcall function_231995(s_screen_parameters *parameters);

/* ---- the tabs ---- */

// @retail 0x23148e
c_y_menu_tab_screen::c_y_menu_tab_screen(long a, long b, word user_flags) :
	c_class_1473c9(g_54d5a8 == 2 ? 0x1a : 0x1c, a, b, user_flags),
	value610(false)
{
}

// @retail 0x2312b4
void c_y_menu_tab_screen::v18(void *parameters)
{
	c_class_1a2c81::v1();
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
	return c_class_1473c9::v10(event);
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
	return c_class_1473c9::v10(event);
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
	return c_class_1473c9::v10(event);
}

/* ---- the menu ---- */

// @retail 0x231995
c_class_1473c9 *__stdcall function_231995(s_screen_parameters *parameters)
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
	c_class_1473c9(g_54d5a8 == 2 ? 0x1a : 0x1c, a, b, user_flags),
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
	c_class_1a2c81::v2();
	function_148995(0);
	friends_list_reset(true);
}

// @retail 0x231ac4
void c_online_y_menu_screen::v17()
{
	c_class_1473c9 *tab;

	tab = &friends;
	tab->v17();
	tab = &players;
	tab->v17();
	tab = &recent_players;
	tab->v17();
}

/* deletes the messages from players who are neither friends nor players met
   (once, when the friends list is ready) */
// @retail 0x231ca0
void function_231ca0(s_entry *messages, long count, c_online_y_menu_screen *screen)
{
	for (; count; count--, messages++)
	{
		XUID const *xuid = messages ? (XUID const *)&messages->unknown0 : 0;

		if ((messages->flags & 0x10000) && !friends_list_contains(xuid) && !players_list_contains(xuid))
		{
			online_message_delete(screen->controller_index, messages->unknown20, false);
		}
	}
}

/* the current tab's icon shows which tab it is */
// @retail 0x231cea
void c_online_y_menu_screen::v3()
{
	c_class_1a2c81 *current = tab_bar.focused;

	function_1a31ff();
	message_count = 0x7d;
	online_messages_enumerate(controller_index, messages, &message_count);
	if (!value55ac && friends_list_task_running() && function_1a325a())
	{
		function_231ca0(messages, message_count, this);
		value55ac = true;
	}
	if (current)
	{
		c_class_1a2c81 *bitmap = current->find_child(8, 1, false);

		if (bitmap)
		{
			short index;

			if (tab_is_current(&friends))
			{
				index = 0;
			}
			else if (tab_is_current(&players))
			{
				index = 1;
			}
			else if (tab_is_current(&recent_players))
			{
				index = 2;
			}
			else
			{
				goto done;
			}
			function_2b0a14((s_widget_view_2b0a *)bitmap, index);
		}
	}
done:
	c_class_1a2c81::v3();
}

/* the user's controller, the tabs and their lists, and the user's messages */
// @retail 0x231ae9
void c_online_y_menu_screen::v18(void *parameters)
{
	word user_flags = ((s_screen_parameters *)parameters)->user_flags;

	if (user_flags & 1)
	{
		controller_index = 0;
	}
	else if (user_flags & 2)
	{
		controller_index = 1;
	}
	else if (user_flags & 4)
	{
		controller_index = 2;
	}
	else if (user_flags & 8)
	{
		controller_index = 3;
	}
	else
	{
		controller_index = 0;
	}
	value55ac = false;
	friends_list_reset(true);
	function_1a303b(controller_index);
	friends.value814 = controller_index;
	friends.list.value88 = controller_index;
	widget_set_user_flags(&friends.list, 1 << controller_index);
	players.value814 = controller_index;
	players.list.value88 = controller_index;
	widget_set_user_flags(&players.list, 1 << controller_index);
	recent_players.value814 = controller_index;
	recent_players.list.value88 = controller_index;
	widget_set_user_flags(&recent_players.list, 1 << controller_index);
	parameters = (void *)function_1480ff(screen_id);
	{
		s_screen_layout layout =
		{
			&tab_bar,
			3,
			{
				{ 0, 0, (c_class_1474e8 *)(parameters = &friends.list), 0 },
				{ 0, 0, &players.list, 0 },
				{ 0, 0, &recent_players.list, 0 }
			}
		};

		tab_bar.add_child(&friends);
		tab_bar.add_child(&players);
		tab_bar.add_child(&recent_players);
		build(&layout);
	}
	{
		s_window_manager_e94 user;

		function_18ff47(controller_index, user.data);
		function_148995(&user);
	}
	message_count = 0x7d;
	online_messages_enumerate(controller_index, messages, &message_count);
	((s_player_slot_messages_view *)&g_54e8e0[controller_index])->messages_changed = false;
	c_class_1a2c81::v1();
	friends.v7((c_class_1a2c81 *)parameters);
}
