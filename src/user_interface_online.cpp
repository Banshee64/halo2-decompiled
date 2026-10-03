#include "cseries.h"
#include <xtl.h>
#include <xonline.h>
#include "screen_widgets.h"
#include "unknown_19b510.h"
#include "unknown_234c64.h"

// @flags /O1 /Oi /Gr

/* USER_INTERFACE_ONLINE.CPP: what the online screens do with the player or
   friend the user chose (friend requests, game invites, messages) and the
   checks before a squad is made (named after the debug build's source
   file) */

struct s_message;
void function_149f49(s_message *message, word a, dword *id, word b, long c, long d, long e);
void function_14887e(s_screen_settings_54dc6c *settings);
void function_1487c3(long controller_index, long task_index, long callback, long value, long context);

void online_friend_from_user(XONLINE_FRIEND *friend_, const XONLINE_USER *user);
void online_friends_remove(DWORD controller_index, const XONLINE_FRIEND *friend_);
void online_friends_answer_request(DWORD controller_index, const XONLINE_FRIEND *friend_, long answer);
void online_friends_game_invite(DWORD controller_index, XONLINE_FRIEND *friend_);

bool network_session_manager_session_unready(void);
bool network_session_manager_session_a_established(void);
dword function_0592d0(void);
short network_session_interface_get_value_5dd0(void);
bool function_19a935(void);
long function_199fd6(void);
long function_19a161(void);

typedef bool (__stdcall *multiple_choice_callback)(long controller_index, long item);
void function_2b8c05(long a, long b, word user_flags, multiple_choice_callback callback, long title, long count, long *string_ids);
void __stdcall function_2395dc(XONLINE_FRIEND *friend_, long controller_index, long mode);

c_screen_widget *__stdcall function_2b8add(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b8aed(s_screen_parameters *parameters);

/* the player the online screens act on, as function_14887e copies it out:
   a user (type 1) or a friend (type 2) */
#pragma pack(push, 1)
struct s_online_selection
{
	long type;
	union
	{
		XONLINE_USER user;
		XONLINE_FRIEND friend_;
	};
	byte unknown[0x78 - 4 - sizeof(XONLINE_USER)];
};
#pragma pack(pop)

/* the selection as a friend */
static __forceinline void online_selection_get_friend(s_online_selection *selection, XONLINE_FRIEND *friend_)
{
	switch (selection->type)
	{
	case 1:
		online_friend_from_user(friend_, &selection->user);
		break;
	default:
		*friend_ = selection->friend_;
		break;
	}
}

// @retail 0x238ee3
void function_238ee3(long controller_index)
{
	s_screen_parameters parameters;

	parameters.field_c = 0;
	function_149f49((s_message *)&parameters, 0, 0, 1 << controller_index, 3, 4, (long)function_2b8add);
	parameters.load(&parameters);
}

// @retail 0x238f11
void function_238f11(long controller_index)
{
	s_screen_parameters parameters;

	parameters.field_c = 0;
	function_149f49((s_message *)&parameters, 0, 0, 1 << controller_index, 3, 4, (long)function_2b8aed);
	parameters.load(&parameters);
}

/* invites the selected player to the game */
// @retail 0x2390f8
void function_2390f8(long controller_index)
{
	s_online_selection selection;

	function_14887e((s_screen_settings_54dc6c *)&selection);
	switch (selection.type)
	{
	case 1:
	{
		XONLINE_FRIEND friend_;

		online_friend_from_user(&friend_, &selection.user);
		friend_.dwFriendState |= 0x4000000;
		selection.friend_ = friend_;
		selection.type = 2;
		break;
	}
	}
	online_friends_game_invite(controller_index, &selection.friend_);
}

/* accepts the selected player's friend request */
// @retail 0x23914b
void function_23914b(long controller_index)
{
	s_online_selection selection;
	XONLINE_FRIEND friend_;

	function_14887e((s_screen_settings_54dc6c *)&selection);
	online_selection_get_friend(&selection, &friend_);
	online_friends_answer_request(controller_index, &friend_, 1);
}

/* declines it */
// @retail 0x239197
void function_239197(long controller_index)
{
	s_online_selection selection;
	XONLINE_FRIEND friend_;

	function_14887e((s_screen_settings_54dc6c *)&selection);
	online_selection_get_friend(&selection, &friend_);
	online_friends_answer_request(controller_index, &friend_, 0);
}

/* blocks the player */
// @retail 0x2391e2
void function_2391e2(long controller_index)
{
	s_online_selection selection;
	XONLINE_FRIEND friend_;

	function_14887e((s_screen_settings_54dc6c *)&selection);
	online_selection_get_friend(&selection, &friend_);
	online_friends_answer_request(controller_index, &friend_, 2);
}

/* removes the friend */
// @retail 0x23922e
void function_23922e(long controller_index)
{
	s_online_selection selection;
	XONLINE_FRIEND friend_;

	function_14887e((s_screen_settings_54dc6c *)&selection);
	online_selection_get_friend(&selection, &friend_);
	online_friends_remove(controller_index, &friend_);
}

// @retail 0x2397b9
void function_2397b9(long controller_index)
{
	dialog_ok_show(3, 0x7f, 4, 1 << controller_index, 0, 0);
}

// @retail 0x23982a
void function_23982a(long controller_index)
{
	dialog_ok_show(1, 0xbf, 4, 1 << controller_index, 0, 0);
}

// @retail 0x239877
bool __stdcall function_239877(long controller_index, long item)
{
	s_online_selection selection;

	function_14887e((s_screen_settings_54dc6c *)&selection);
	switch ((short)item)
	{
	case 1:
		function_2395dc(&selection.friend_, controller_index, 1);
		break;
	}
	return true;
}

/* asks how to join the friend */
// @retail 0x239843
void function_239843(long controller_index)
{
	long string_ids[2];

	string_ids[0] = 0x1200068b;
	string_ids[1] = 0x1300068c;
	function_2b8c05(3, 4, 1 << controller_index, function_239877, 0x1000068a, 2, string_ids);
}

// @retail 0x2398dc
bool __stdcall function_2398dc(long controller_index, long item)
{
	s_online_selection selection;

	function_14887e((s_screen_settings_54dc6c *)&selection);
	switch ((short)item)
	{
	case 1:
		function_2395dc(&selection.friend_, controller_index, 2);
		break;
	case 2:
		function_2395dc(&selection.friend_, controller_index, 3);
		break;
	}
	return true;
}

// @retail 0x2398a0
void function_2398a0(long controller_index)
{
	long string_ids[3];

	string_ids[0] = 0x1200068e;
	string_ids[1] = 0x1300068f;
	string_ids[2] = 0x10000690;
	function_2b8c05(3, 4, 1 << controller_index, function_2398dc, 0x1000068d, 3, string_ids);
}

/* whether a squad can be made now; tells the user why not */
// @retail 0x239abe
bool function_239abe(long controller_index)
{
	bool result = false;

	if (network_session_manager_session_unready() &&
		function_19a935() &&
		!network_session_manager_session_a_established() &&
		network_session_interface_get_value_5dd0() == NONE)
	{
		if (function_0592d0() == 6 || function_0592d0() == 7 || function_0592d0() == 8)
		{
			dialog_ok_show(3, 0x86, 4, 1 << controller_index, 0, 0);
		}
		else if (!function_199fd6())
		{
			dialog_ok_show(3, 0x85, 4, 1 << controller_index, 0, 0);
		}
		else if (function_19a161() == 2)
		{
			dialog_ok_show(3, 0x83, 4, 1 << controller_index, 0, 0);
		}
		else
		{
			result = true;
		}
	}
	else
	{
		dialog_ok_show(3, 0x84, 4, 1 << controller_index, 0, 0);
	}
	return result;
}
