// @flags /O1 /Oi /Gr
/* SCREEN_ONLINE_Y_MENU_FRIENDS_LIST.CPP: the online Y menu's friends list
   and the base it shares with the players list */

#include "cseries.h"
#include <xtl.h>
#include <xonline.h>
#include "data_array.h"
#include "globals.h"
#include "screen_widgets.h"
#include "unknown_19b516.h"
#include "screen_online_y_menu.h"

struct s_message;
void function_149f49(s_message *message, word a, dword *id, word b, long c, long d, long e);
void function_238c21(long controller, long type, word *name, long maximum_count);
struct s_friend;
void friend_get_online_friend(s_friend const *player, XONLINE_FRIEND *result);
c_screen_widget *__stdcall function_2b71f0(s_screen_parameters *parameters);

/* what the name lookups take: a friend (type 2), a player (type 1) or
   nobody (no request) */
struct s_name_request
{
	long type;
	XONLINE_FRIEND online_friend;
	byte unknown[0x78 - 4 - sizeof(XONLINE_FRIEND)];
};

void __stdcall function_148893(s_name_request *request, long flag);

/* a friend of the friends list (unknown_1a2ca7.cpp) */
#pragma pack(push, 4)
struct s_friend_view
{
	byte unknown00[4];
	unsigned __int64 xuid;
};
#pragma pack(pop)

/* the scenario's type at +0x10 (2 is the main menu) */
struct s_scenario_type_view
{
	byte unknown00[0x10];
	short type;
};

s_data_array *g_46e7bc;

// @retail 0x2b2d9a
c_y_menu_friends_list::c_y_menu_friends_list(word user_flags) :
	c_y_menu_list(user_flags),
	item_count(NONE),
	handler(this, (list_item_method)&c_y_menu_friends_list::handle_item)
{
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2b2e6b
c_y_menu_list::c_y_menu_list(word user_flags) :
	c_list_widget_with_items(user_flags),
	value88(NONE)
{
	if (g_4e0350 && ((s_scenario_type_view *)g_4e0350)->type != 2)
	{
		g_4e647b = true;
	}
}

// @retail 0x2b2ebd
c_y_menu_list::~c_y_menu_list()
{
	g_4e647b = false;
}

// @retail 0x2b2e9f deleting c_y_menu_list

/* the players list's deleting destructor is this one */
// @retail 0x2b2dfd deleting c_y_menu_friends_list

/* slot 1 of the list (c_widget::v9 in unknown_19b516.h's view) */
// @retail 0x2b2e19
void c_y_menu_friends_list::v1()
{
	data = g_46e7bc;
	item_count = NONE;
	((c_widget *)this)->m7f = 0;
	((c_widget *)this)->c_widget::v9();
}

/* gives the items new data when the friends list's count changed */
// @retail 0x2b2e31
void c_y_menu_friends_list::v3()
{
	data = g_46e7bc;
	if (g_46e7bc)
	{
		long count = g_46e7bc->actual_count;

		if (item_count != count)
		{
			item_count = count;
			function_24c0c4((c_widget *)this);
		}
	}
	else
	{
		item_count = NONE;
	}
	((c_widget *)this)->c_widget::v11();
}

// @retail 0x2b3ef5
void *c_y_menu_friends_list::get_item_data()
{
	return items;
}

// @retail 0x2b2d7d
long c_y_menu_friends_list::get_item_count()
{
	return 8;
}

/* the user's pending online messages, held by the menu three levels up */
// @retail 0x2b419f
void *c_y_menu_friends_list::get_items(long *count)
{
	c_online_y_menu_screen *screen = (c_online_y_menu_screen *)parent->parent->parent;

	*count = screen->message_count;
	return screen->messages;
}

/* a friend opens the friend's screen; the empty item asks for a gamertag to
   send a friend request to */
// @retail 0x2b386f
void c_y_menu_friends_list::handle_item(s_controller_reference **controller, long *item)
{
	s_friend_view *player = data ? (s_friend_view *)datum_get(data, *item) : 0;

	if (player)
	{
		if (player->xuid == 0)
		{
			function_148893(0, 1);
			name[0] = 0;
			function_238c21((*controller)->controller_index, 0xc, name, 0x10);
		}
		else
		{
			s_name_request request;
			s_screen_parameters parameters;

			parameters.field_c = 0;
			request.type = 2;
			friend_get_online_friend((s_friend const *)player, &request.online_friend);
			function_148893(&request, 1);
			function_149f49((s_message *)&parameters, 0, 0, 1 << (*controller)->controller_index, 3, 4, (long)function_2b71f0);
			parameters.load(&parameters);
		}
	}
}
