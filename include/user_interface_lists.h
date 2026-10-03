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

#endif
