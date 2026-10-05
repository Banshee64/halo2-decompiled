// @flags /O1 /arch:SSE /Gr
/* UNKNOWN_24AE45.CPP: the gamertag select screen (vtable 0x459fb0)
   and its list (vtable 0x459f58) */

#include "unknown_11c920.h"
#include <string.h>
#include "unknown_24b5bc.h"
#include "unknown_19b510.h"
#include "unknown_19b516.h"

void profile_edit_end();
long function_11cbb0();
void function_236299(long sound);
bool online_user_requires_passcode(const XONLINE_USER *user);
c_class_1473c9 *__stdcall function_2ba45b(s_screen_parameters *parameters);
bool __stdcall function_24b407(long controller_index);
/* unknown_22376b.cpp: never returns */
void function_2238f4(long page, dword context, dword parameter1, dword parameter2);

extern bool g_54d5a0;

/* a gamertag the list shows (0x74 bytes) */
struct s_gamertag_datum
{
	word salt;
	word type;
	XONLINE_USER user;
};

/* ---- the list ---- */

/* the gamertag select list (vtable 0x459f58): the gamertags to choose from */
class c_gamertag_select_list : public c_class_1474e8
{
public:
	c_gamertag_select_list(word user_flags);

	virtual void v20(c_class_1a2c81 *widget, long index);

	void handle_item(s_controller_reference **controller, long *item);
	void reload_if_changed();

	c_class_14750b items[4];
	word gamertags[4][0x40];
	byte unknown488[0x1388 - 0x488];
	c_list_item_handler handler;
	long value13a0;
};

// @retail 0x24ae4b
c_gamertag_select_list::c_gamertag_select_list(word user_flags) :
	c_class_1474e8(user_flags),
	handler(this, (list_item_method)&c_gamertag_select_list::handle_item)
{
	value13a0 = 0;
	data = user_interface_data_new("gamertag list", 0x22, 0x74);
	function_16b790(data);
	delegate_register(&item_handlers, &handler);
}

// @retail 0x24aee4 destructor c_gamertag_select_list
// @retail 0x24aec6 deleting c_gamertag_select_list

/* signs the controller's player slot in with the chosen gamertag, or creates
   a new profile */
// @retail 0x24b2ea
void c_gamertag_select_list::handle_item(s_controller_reference **controller, long *item)
{
	if (*item != NONE)
	{
		s_gamertag_datum *datum = &((s_gamertag_datum *)data->data)[*item & 0xffff];

		if (datum->type == 0x1971)
		{
			dialog_choice_show(3, 0x35, 4, 1 << (*controller)->controller_index, function_24b407, 0, 0);
		}
		else if (datum->type == 0x1023)
		{
			if (g_54d5a0)
			{
				function_236299(2);
			}
			else
			{
				long player = (*controller)->controller_index;

				if (!TEST_FIELD_BIT(((s_player_slot_sign_in_view *)g_54e8e0)[player].signed_in))
				{
					player_slot_profile_get(player)->sign_in(0);
				}
				get_screen()->start_animation(3);
			}
		}
		else
		{
			s_player_slot_profile *profile = player_slot_profile_get((*controller)->controller_index);

			profile->user = datum->user;
			if (online_user_requires_passcode(&datum->user))
			{
				s_screen_parameters parameters;

				parameters.field_c = 0;
				function_149f49((s_message *)&parameters, 6, 0, 1 << (*controller)->controller_index, 3, 4, (long)function_2ba45b);
				parameters.load(&parameters);
			}
			else
			{
				profile->sign_in(0);
				get_screen()->start_animation(3);
			}
		}
	}
}

// @retail 0x24b2ac
void c_gamertag_select_list::v20(c_class_1a2c81 *widget, long index)
{
	c_class_1a2c81 *text = widget->find_child(6, 0, false);

	if (text)
	{
		short gamertag = (short)widget_item(widget)->value70;

		text->function_22f52e()->set_text(gamertags[gamertag]);
	}
}

/* the screen's id of the signed in users (not decompiled yet) */
long g_4e6364;

c_class_1473c9 *__stdcall function_24b4a9(s_screen_parameters *parameters);

/* reloads the screen when the signed in users changed */
// @retail 0x24b1bf
void c_gamertag_select_list::reload_if_changed()
{
	if (g_4e6364 != value13a0)
	{
		s_screen_parameters parameters;

		parameters.field_c = 0;
		function_149f49((s_message *)&parameters, 6, 0, (short)user_flags, 3, 4, (long)function_24b4a9);
		parameters.load(&parameters);
	}
}

/* ---- the screen ---- */

class c_gamertag_select_screen : public c_screen_with_menu
{
public:
	c_gamertag_select_screen(long a, long b, word user_flags);

	virtual void v3();
	virtual bool v10(s_widget_event *event);
	virtual screen_load_proc get_load_proc();

	c_gamertag_select_list list;
};

// @retail 0x24b4a9
c_class_1473c9 *__stdcall function_24b4a9(s_screen_parameters *parameters)
{
	c_gamertag_select_screen *screen = new c_gamertag_select_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x24b4e7
c_gamertag_select_screen::c_gamertag_select_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0x14, a, b, user_flags, &list),
	list(user_flags)
{
}

// @retail 0x24b537 destructor c_gamertag_select_screen
// @retail 0x24b519 deleting c_gamertag_select_screen

// @retail 0x24b54c
bool c_gamertag_select_screen::v10(s_widget_event *event)
{
	bool result;

	if (event->type == 5 && (event->param == 0xd || event->param == 1))
	{
		start_animation(3);
		if (g_54e5d0.profile_index != NONE)
		{
			profile_edit_end();
		}
		result = true;
	}
	else
	{
		result = c_class_1473c9::v10(event);
	}
	return result;
}

// @retail 0x24b584
void c_gamertag_select_screen::v3()
{
	long region;
	c_class_1a2c81 *text;

	list.reload_if_changed();
	region = function_11cbb0();
	text = find_child(6, 2, false);
	if (text)
	{
		text->value6e = region == 0;
	}
	c_class_1a2c81::v3();
}

// @retail 0x24ae45
screen_load_proc c_gamertag_select_screen::get_load_proc()
{
	return function_24b4a9;
}

/* the dashboard's new account sign up */
// @retail 0x24b407
bool __stdcall function_24b407(long controller_index)
{
	function_2238f4(3, 0, 0, 0);
	return true;
}

bool xuid_equal(XUID const *a, XUID const *b, bool compare_guest_number);

/* the guest number a new guest of this user takes: the lowest one none of
   the signed in users with the same account has (0 when the user is not
   signed in, or no number is free) */
/* a user's xuid (NULL for no user) */
inline XUID const *online_user_get_xuid(XONLINE_USER const *user)
{
	XUID const *xuid = NULL;

	if (user)
	{
		xuid = &user->xuid;
	}
	return xuid;
}

// @retail 0x24b416
word function_24b416(XONLINE_USER const *user, XONLINE_USER const *users)
{
	word guest_number = 0;
	bool signed_in = false;

	for (long candidate = 1; candidate <= 3 && guest_number == 0; candidate++)
	{
		guest_number = (word)candidate;
		for (long index = 0; index != NONE && guest_number != 0; index = next_controller_index(index))
		{
			if (xuid_equal(online_user_get_xuid(&users[index]), online_user_get_xuid(user), false))
			{
				signed_in = true;
				if ((users[index].xuid.dwUserFlags & 3) == candidate)
				{
					guest_number = 0;
				}
			}
		}
	}
	return signed_in ? (word)guest_number : 0;
}
