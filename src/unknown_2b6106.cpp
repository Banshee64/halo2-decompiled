// @flags /O1 /Oi /Gr
/* UNKNOWN_2B6106.CPP: the screen of what can be
   done to the player the online Y menu selected (vtable 0x45bbd0): its list, the selected player's friend
   state and the load procedures of its five modes */

#include "unknown_11c920.h"
#include <xtl.h>
#include <xonline.h>
#include "screen_widgets.h"
#include "unknown_19b516.h"
#include "unknown_234c64.h"
#include "unknown_2b6106.h"

void function_18ff47(long player, dword *out);
void function_148995(s_window_manager_e94 *value);
bool window_manager_channel_window_in_use(long channel, long index);
void function_1a303b(long controller_index);
void function_14887e(s_screen_settings_54dc6c *settings);
void function_6b640(long task_index);
void qos_release(long handle);
void friends_list_reset(bool dispose);
bool online_messages_find_from(const XUID *sender, DWORD controller_index, long kind);

/* the window manager's selected player (function_14887e copies it out) */
#pragma pack(push, 4)
struct s_name_request
{
	long type;
	union
	{
		XUID user_xuid;
		XONLINE_FRIEND field_xb3bdcf;
	};
	byte unknown5c[0x78 - 0x5c];
};
#pragma pack(pop)

/* the selected player's id */
static inline XUID *selection_get_xuid(s_name_request *selection)
{
	XUID *result = NULL;

	switch (selection->type)
	{
	case 1:
		result = &selection->user_xuid;
		break;
	case 2:
		result = &selection->field_xb3bdcf.xuid;
		break;
	}
	return result;
}

void __stdcall function_148893(s_name_request *request, long mode);

/* the flags friends_lists_get_user reports */
struct s_friend_user_flags
{
	dword online : 1;
	dword bits1 : 3;
	dword bit4 : 1;
	dword bit5 : 1;
	dword bits6 : 4;
	dword bit10 : 1;
	dword bit11 : 1;
	dword bits12 : 20;
};

void friends_lists_get_user(XUID const *xuid, bool *arg_a721be, bool *is_player, dword *flags, DWORD *title_id, bool *in_session, XONLINE_FRIEND *field_xb3bdcf);

/* the player selected screen (vtable 0x45bbd0) */
class c_y_menu_player_selected_screen : public c_screen_with_menu
{
public:
	c_y_menu_player_selected_screen(long a, long b, word user_flags);
	~c_y_menu_player_selected_screen();

	virtual screen_load_proc get_load_proc();

	void update_selection();
	void update_join_text();

	c_y_menu_player_selected_list list;
	long task_index;
	long qos_handle;
	byte unknown9c4[0x10d8 - 0x9c4];
	long value10d8;
	long value10dc;
	long value10e0;
	bool value10e4;
	bool value10e5;
	bool value10e6;
	bool in_session;
	long mode;
};

c_class_1473c9 *__stdcall function_2b7152(s_screen_parameters *parameters);
c_class_1473c9 *__stdcall function_2b71f0(s_screen_parameters *parameters);
c_class_1473c9 *__stdcall function_2b7201(s_screen_parameters *parameters);
c_class_1473c9 *__stdcall function_2b7212(s_screen_parameters *parameters);
c_class_1473c9 *__stdcall function_2b7223(s_screen_parameters *parameters);

// @retail 0x2b6106
c_y_menu_player_selected_screen::c_y_menu_player_selected_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0x1f, a, b, user_flags, &list),
	list(user_flags),
	task_index(NONE),
	qos_handle(NONE),
	value10d8(0),
	value10dc(0),
	value10e0(0),
	value10e4(false),
	value10e5(false),
	value10e6(false),
	in_session(false),
	mode(0)
{
	s_window_manager_e94 value;

	function_18ff47(get_controller_index(), value.data);
	function_148995(&value);
	if (!window_manager_channel_window_in_use(4, 4))
	{
		function_1a303b(get_controller_index());
	}
	update_selection();
}

// @retail 0x2b61b2 deleting

// @retail 0x2b6301
c_y_menu_player_selected_screen::~c_y_menu_player_selected_screen()
{
	if (task_index != NONE)
	{
		function_6b640(task_index);
		task_index = NONE;
	}
	if (qos_handle != NONE)
	{
		qos_release(qos_handle);
		qos_handle = NONE;
	}
	if (!window_manager_channel_window_in_use(4, 4))
	{
		friends_list_reset(true);
	}
}

/* looks the selected player up in the friends lists: whether the player can
   be invited and joined */
// @retail 0x2b6208
void c_y_menu_player_selected_screen::update_selection()
{
	s_name_request selection;
	XONLINE_FRIEND field_xb3bdcf;
	s_friend_user_flags flags;
	DWORD title_id;
	bool arg_a721be;
	bool is_player;
	XUID *xuid;

	value10e5 = false;
	value10e6 = false;
	function_14887e((s_screen_settings_54dc6c *)&selection);
	xuid = selection_get_xuid(&selection);
	if (xuid && xuid->qwUserID != 0)
	{
		friends_lists_get_user(xuid, &arg_a721be, &is_player, (dword *)&flags, &title_id, &in_session, &field_xb3bdcf);
		if (!in_session)
		{
			value10e4 = false;
		}
		if (arg_a721be || is_player)
		{
			bool bit10 = TEST_FIELD_BIT(flags.bit10);
			bool bit11 = TEST_FIELD_BIT(flags.bit11);

			bool joinable = arg_a721be && !TEST_FIELD_BIT(flags.bit5) && !TEST_FIELD_BIT(flags.bit4) ||
				is_player && !bit11 && !bit10;

			value10e6 = joinable;
			value10e5 = joinable && TEST_FIELD_BIT(flags.online);
			selection.field_xb3bdcf = field_xb3bdcf;
			selection.type = 2;
		}
		function_148893(&selection, 2);
	}
}

// @retail 0x2b7234
screen_load_proc c_y_menu_player_selected_screen::get_load_proc()
{
	screen_load_proc result = function_2b7152;

	switch (mode)
	{
	case 1:
		result = function_2b71f0;
		break;
	case 2:
		result = function_2b7201;
		break;
	case 3:
		result = function_2b7212;
		break;
	case 4:
		result = function_2b7223;
		break;
	}
	return result;
}

/* the first controller of a set of user flags (none when no flag is set) */
static __forceinline long user_flags_get_controller(word user_flags)
{
	long controller_index;

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
	return controller_index;
}

/* loads the screen in a mode */
// @retail 0x2b71a6
c_class_1473c9 *function_2b71a6(s_screen_parameters *parameters, long mode)
{
	c_y_menu_player_selected_screen *screen = new c_y_menu_player_selected_screen(parameters->a, parameters->b, parameters->user_flags);

	if (screen)
	{
		screen->m6c = true;
		screen->function_147f6d(parameters);
		screen->mode = mode;
		screen->list.value3a0 = mode;
	}
	return screen;
}

/* the same, unless the selected player sent the user a message of the
   mode's kind: then the clan member screen opens instead */
// @retail 0x2b70a3
c_class_1473c9 *function_2b70a3(s_screen_parameters *parameters, long mode)
{
	long controller_index = user_flags_get_controller(parameters->user_flags);
	s_name_request selection;
	c_y_menu_player_selected_screen *screen;

	XUID *xuid;

	function_14887e((s_screen_settings_54dc6c *)&selection);
	xuid = selection_get_xuid(&selection);
	if (xuid && xuid->qwUserID != 0 && online_messages_find_from(xuid, controller_index, mode))
	{
		screen = (c_y_menu_player_selected_screen *)function_2b61ce((short)parameters->user_flags, mode);
	}
	else
	{
		screen = new c_y_menu_player_selected_screen(parameters->a, parameters->b, parameters->user_flags);

		if (screen)
		{
			screen->m6c = true;
			screen->function_147f6d(parameters);
			screen->mode = mode;
			screen->list.value3a0 = mode;
		}
	}
	return screen;
}

// @retail 0x2b7152
c_class_1473c9 *__stdcall function_2b7152(s_screen_parameters *parameters)
{
	return function_2b71a6(parameters, 0);
}

// @retail 0x2b7162
c_class_1473c9 *__stdcall function_2b7162(s_screen_parameters *parameters)
{
	return function_2b71a6(parameters, 1);
}

// @retail 0x2b7173
c_class_1473c9 *__stdcall function_2b7173(s_screen_parameters *parameters)
{
	return function_2b71a6(parameters, 2);
}

// @retail 0x2b7184
c_class_1473c9 *__stdcall function_2b7184(s_screen_parameters *parameters)
{
	return function_2b71a6(parameters, 3);
}

// @retail 0x2b7195
c_class_1473c9 *__stdcall function_2b7195(s_screen_parameters *parameters)
{
	return function_2b71a6(parameters, 4);
}

// @retail 0x2b71f0
c_class_1473c9 *__stdcall function_2b71f0(s_screen_parameters *parameters)
{
	return function_2b70a3(parameters, 1);
}

// @retail 0x2b7201
c_class_1473c9 *__stdcall function_2b7201(s_screen_parameters *parameters)
{
	return function_2b70a3(parameters, 2);
}

// @retail 0x2b7212
c_class_1473c9 *__stdcall function_2b7212(s_screen_parameters *parameters)
{
	return function_2b70a3(parameters, 3);
}

// @retail 0x2b7223
c_class_1473c9 *__stdcall function_2b7223(s_screen_parameters *parameters)
{
	return function_2b70a3(parameters, 4);
}

/* the second text says whether the player can be joined */
// @retail 0x2b6e21
void c_y_menu_player_selected_screen::update_join_text()
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)find_child(6, 2, false);

	if (text)
	{
		text->value6e = true;
		if (value10e6)
		{
			text->function_253b1a(0x700024e);
		}
		else
		{
			text->function_253b1a(0x12000280);
		}
	}
}

/* a time left in minutes and seconds (a game's or a session's) */
struct s_time_left
{
	byte unknown00[0xc];
	short game_seconds;
	short session_seconds;
};

word *function_1630e0(word *buffer, const word *format, ...);

/* formats the time left with the screen's string for it */
// @retail 0x2b6db1
bool function_2b6db1(s_time_left *time, c_widget *screen, word *result_string)
{
	bool result = false;
	word format[0x100];
	long string_handle;
	long minutes;
	long seconds;

	format[0] = 0;
	if (time->game_seconds > 0)
	{
		minutes = time->game_seconds / 60;
		seconds = time->game_seconds % 60;
		string_handle = 0x13000264;
	}
	else if (time->session_seconds > 0)
	{
		minutes = time->session_seconds / 60;
		seconds = time->session_seconds % 60;
		string_handle = 0x15000265;
	}
	else
	{
		return result;
	}
	screen->function_230134(string_handle, format);
	function_1630e0(result_string, format, minutes, seconds);
	result = true;
	return result;
}

dword online_friend_get_flags(XONLINE_FRIEND const *friend_);
void online_friend_get_title_name(XONLINE_FRIEND const *friend_, wchar_t *name, short name_length);
dword function_217b70(word const *text);

// @retail 0x2b635f
void function_2b635f(XONLINE_FRIEND const *friend_, c_widget *screen)
{
    word message[256], format[256], title[256];
    message[0] = 0;
    format[0] = 0;
    title[0] = 0;
    dword flags = online_friend_get_flags(friend_);
    c_class_1a2c81 *text = ((c_class_1a2c81 *)screen)->find_child(6, 2, false);
    online_friend_get_title_name(friend_, (wchar_t *)title, 256);
    if ((long)function_217b70(title) > 0)
    {
        if ((bool)((flags >> 2) & 1))
        {
            screen->function_230134(0x1500025d, format);
            function_1630e0(message, format, title);
        }
        else
        {
            screen->function_230134(0x1400025c, format);
            function_1630e0(message, format, title);
        }
    }
    else
        screen->function_230134(0x0e00024f, message);
    if (text)
    {
        text->value6e = true;
        text->function_22f52e()->set_text(message);
    }
}
