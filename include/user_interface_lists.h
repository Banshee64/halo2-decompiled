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

/* "mp player settings game list" (vtable 0x45b9c0; unknown_2b116a.cpp) */
class c_mp_player_settings_game_list : public c_list_widget
{
public:
	c_mp_player_settings_game_list(word user_flags);

	virtual void v20(c_user_interface_widget *item, long unused);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[4];
	c_list_item_handler handler;
};

/* "clan options list" (vtable 0x45b1b0; unknown_2b116a.cpp) */
class c_clan_options_list : public c_list_widget
{
public:
	c_clan_options_list(word user_flags);

	virtual void v20(c_user_interface_widget *item, long unused);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[4];
	long value288;
	long value28c;
	long value290;
	long value294;
	c_list_item_handler handler;
};

/* "mp change teams list" (vtable 0x45b950; unknown_2b116a.cpp) */
class c_mp_change_teams_list : public c_list_widget
{
public:
	c_mp_change_teams_list(word user_flags);

	virtual void v20(c_user_interface_widget *item, long unused);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[5];
	c_list_item_handler handler;
};

/* "custom game profile list" (vtable 0x45d768; unknown_2c9ddb.cpp): the
   saved variants of one game type */
class c_custom_game_profile_list : public c_list_widget
{
public:
	c_custom_game_profile_list(word user_flags);

	virtual void v1();
	virtual void v3();
	virtual void v20(c_user_interface_widget *item, long unused);

	void handle_item(s_controller_reference **controller, long *item);
	/* rebuilds the items from the saved variants */
	void fill();
	void fill_and_select_first();
	void fill_and_keep_focus();
	void select_last_variant();

	c_list_item_widget items[15];
	long variants[0x1065];
	long game_type;
	long value49a0;
	long value49a4;
	bool flag_a;
	bool flag_b;
	bool flag_c;
	c_list_item_handler handler;
};

/* "voice mask list" (vtable 0x45cbd0; unknown_2c4e9c.cpp) */
class c_voice_mask_list : public c_list_widget
{
public:
	c_voice_mask_list(word user_flags);

	virtual void v1();
	virtual void v20(c_user_interface_widget *widget, long index);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[2];
	bool value188;
	byte unknown189[0x18c - 0x189];
	c_list_item_handler handler;
};

/* "voice through tv list" (vtable 0x45cc38; unknown_2c4e9c.cpp) */
class c_voice_through_tv_list : public c_list_widget
{
public:
	c_voice_through_tv_list(word user_flags);

	virtual void v1();
	virtual void v20(c_user_interface_widget *widget, long index);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[4];
	bool value288;
	byte unknown289[0x28c - 0x289];
	c_list_item_handler handler;
};

/* "thumbstick settings edit list" (vtable 0x45cca8; unknown_2c4e9c.cpp) */
class c_thumbstick_settings_edit_list : public c_list_widget
{
public:
	c_thumbstick_settings_edit_list(word user_flags);

	virtual void v20(c_user_interface_widget *widget, long index);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[4];
	bool value288;
	byte unknown289[0x28c - 0x289];
	c_list_item_handler handler;
};

/* "look sensitivity settings edit list" (vtable 0x45cd20; unknown_2c4e9c.cpp) */
class c_look_sensitivity_settings_edit_list : public c_list_widget
{
public:
	c_look_sensitivity_settings_edit_list(word user_flags);

	virtual void v20(c_user_interface_widget *widget, long index);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[4];
	bool value288;
	byte unknown289[0x28c - 0x289];
	c_list_item_handler handler;
};

/* "invert look settings edit list" (vtable 0x45ce38; unknown_2c4e9c.cpp) */
class c_invert_look_settings_edit_list : public c_list_widget
{
public:
	c_invert_look_settings_edit_list(word user_flags);

	virtual void v20(c_user_interface_widget *widget, long index);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[2];
	bool value188;
	byte unknown189[0x18c - 0x189];
	c_list_item_handler handler;
};

/* "button settings edit list" (vtable 0x45cdc0; unknown_2c4e9c.cpp) */
class c_button_settings_edit_list : public c_list_widget
{
public:
	c_button_settings_edit_list(word user_flags);

	virtual void v20(c_user_interface_widget *widget, long index);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[4];
	bool value288;
	byte unknown289[0x28c - 0x289];
	c_list_item_handler handler;
};

/* "auto level settings edit list" (vtable 0x45ce38; unknown_2c4e9c.cpp) */
class c_auto_level_settings_edit_list : public c_list_widget
{
public:
	c_auto_level_settings_edit_list(word user_flags);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[2];
	bool value188;
	byte unknown189[0x18c - 0x189];
	c_list_item_handler handler;
};

/* "vibration settings edit list" (vtable 0x45ceb0; unknown_2c4e9c.cpp) */
class c_vibration_settings_edit_list : public c_list_widget
{
public:
	c_vibration_settings_edit_list(word user_flags);

	virtual void v20(c_user_interface_widget *widget, long index);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[2];
	bool value188;
	byte unknown189[0x18c - 0x189];
	c_list_item_handler handler;
};

/* "subtitle setting list" (vtable 0x45d4f0; unknown_2c4e9c.cpp) */
class c_subtitle_setting_list : public c_list_widget
{
public:
	c_subtitle_setting_list(word user_flags);

	virtual void v1();
	virtual void v20(c_user_interface_widget *widget, long index);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[3];
	c_list_item_handler handler;
};

/* "choose model list" (vtable 0x45d8c8; unknown_2c4e9c.cpp) */
class c_choose_model_list : public c_list_widget
{
public:
	c_choose_model_list(word user_flags);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[2];
	c_list_item_handler handler;
};

#endif
