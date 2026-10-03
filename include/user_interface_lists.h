/* USER_INTERFACE_LISTS.H: the lists of 0x2b0000..0x2cb8c0 that the screens
   of other files hold as members */

#ifndef USER_INTERFACE_LISTS_H
#define USER_INTERFACE_LISTS_H

#include "cseries.h"
#include "screen_widgets.h"

/* "settings list" (vtable 0x45b060; unknown_2b116a.cpp) */
class c_settings_list : public c_list_widget
{
public:
	c_settings_list(word user_flags);

	virtual long get_item_count();

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[3];
	c_list_item_handler handler;
	bool extended;
};

/* "squad privacy setting list" (vtable 0x45ba38; unknown_2b116a.cpp) */
class c_squad_privacy_setting_list : public c_list_widget
{
public:
	c_squad_privacy_setting_list(word user_flags);

	virtual long get_item_count();

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[3];
	c_list_item_handler handler;
};

/* "xbox live menu list" (vtable 0x458b48; unknown_23068b.cpp) */
class c_xbox_live_menu_list : public c_list_widget
{
public:
	c_xbox_live_menu_list(word user_flags);

	virtual void v20(c_user_interface_widget *item, long unused);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[4];
	c_list_item_handler handler;
};

/* "mp controller settings game list" (vtable 0x45b778; unknown_2b116a.cpp) */
class c_mp_controller_settings_game_list : public c_list_widget
{
public:
	c_mp_controller_settings_game_list(word user_flags);

	virtual void v20(c_user_interface_widget *item, long unused);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[5];
	c_list_item_handler handler;
};

/* "handicap settings edit list" (vtable 0x45b8d8; unknown_2b116a.cpp) */
class c_handicap_settings_edit_list : public c_list_widget
{
public:
	c_handicap_settings_edit_list(word user_flags);

	virtual void v20(c_user_interface_widget *item, long unused);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[4];
	c_list_item_handler handler;
};

/* "variant editing options list" (vtable 0x45bdb8; unknown_2b116a.cpp) */
class c_variant_editing_options_list : public c_list_widget
{
public:
	c_variant_editing_options_list(word user_flags);

	virtual void v20(c_user_interface_widget *item, long unused);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[6];
	c_list_item_handler handler;
};

/* "player profile edit list" (vtable 0x45bea0; unknown_2b116a.cpp) */
class c_player_profile_edit_list : public c_list_widget
{
public:
	c_player_profile_edit_list(word user_flags);

	virtual void v20(c_user_interface_widget *item, long unused);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[6];
	c_list_item_handler handler;
};

/* "friends options list" (vtable 0x45b370; unknown_2b116a.cpp) */
class c_friends_options_list : public c_list_widget
{
public:
	c_friends_options_list(word user_flags);

	virtual void v20(c_user_interface_widget *item, long unused);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[6];
	long value388;
	long value38c;
	long value390;
	long value394;
	c_list_item_handler handler;
};

/* the items of the list at 0x459018 (vtable 0x459070): a press of start
   animates the screen */
class c_pause_game_list_item : public c_list_item_widget
{
public:
	c_pause_game_list_item();

	virtual bool v10(s_widget_event *event);
};

/* "pause game list" (vtable 0x459018; unknown_23068b.cpp) */
class c_pause_game_list : public c_list_widget
{
public:
	c_pause_game_list(word user_flags);

	void handle_item(s_controller_reference **controller, long *item);

	c_pause_game_list_item items[5];
	c_list_item_handler handler;
};

#endif
