/* SCREEN_ONLINE_Y_MENU_PLAYER_SELECTED_LIST.H: the online Y menu's player
   selected list (unknown_2b116a.cpp) and the screen that holds it
   (screen_online_y_menu_player_selected_list.cpp) */

#ifndef SCREEN_ONLINE_Y_MENU_PLAYER_SELECTED_LIST_H
#define SCREEN_ONLINE_Y_MENU_PLAYER_SELECTED_LIST_H

#include "cseries.h"
#include "screen_widgets.h"

/* "Y-menu player selected list" (vtable 0x45bb78): what can be done to the
   player the online Y menu selected */
class c_y_menu_player_selected_list : public c_class_1474e8
{
public:
	c_y_menu_player_selected_list(word user_flags);

	virtual void v1();

	void handle_item(s_controller_reference **controller, long *item);

	c_class_14750b items[6];
	c_list_item_handler handler;
	long value3a0;
	bool value3a4;
};

/* opens the clan member screen on the selected player (unknown_2b116a.cpp) */
c_class_1473c9 *function_2b61ce(long user_flags, long value);

#endif
