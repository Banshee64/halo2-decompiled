// @flags /O1 /Oi /Gr
/* USER_INTERFACE_CONTROLLER_SIGN_IN.CPP: signing a player slot in with a
   profile and a Live user */

#include "cseries.h"
#include <string.h>
#include "user_interface_controller_sign_in.h"
#include "unknown_19b510.h"

bool function_1a0540(s_player_profile_settings *settings, long profile_index);
void __stdcall function_18fb34(long player, s_player_profile_settings *settings, long profile_index);
void function_120df0(long index, wchar_t const *name);
void function_190001(long index, char const *name);
bool window_manager_channel_in_use(long channel);
void voice_start_engine(void);
void voice_stop_engine(void);
long function_6c8b0(XONLINE_USER *user, long player);
void function_1487c3(long controller_index, long task_index, long callback, long value, long context);
void __stdcall function_24b869(c_screen_widget *screen);
bool __stdcall function_24ba7d(long controller_index);

/* the online task screen's (not decompiled yet) */
long g_475338;

/* whether two strings are equal */
// @retail 0x24b5bc
bool function_24b5bc(char const *a, char const *b)
{
	bool result = strcmp(a, b) == 0;

	return result;
}

// @retail 0x24b5ed
s_player_slot_profile::s_player_slot_profile()
{
	player = NONE;
	profile_index = NONE;
	memset(&settings, 0, sizeof(settings));
	memset(&user, 0, sizeof(user));
	memset(&value258, 0, sizeof(value258));
	callback = 0;
	value260 = 0;
}

// @retail 0x24b621
void s_player_slot_profile::initialize(long player)
{
	profile_index = NONE;
	this->player = player;
	memset(&settings, 0, sizeof(settings));
	memset(&user, 0, sizeof(user));
	memset(&value258, 0, sizeof(value258));
	callback = 0;
	value260 = 0;
}

// @retail 0x24b652
void s_player_slot_profile::set_profile_index(long profile_index)
{
	if (function_1a0540(&settings, profile_index))
	{
		this->profile_index = profile_index;
	}
}

/* whether a Live user is set */
inline bool xuid_valid(XUID const *xuid)
{
	return xuid && xuid->qwUserID != 0;
}

/* signs the slot in with its profile, or with its Live user when it has one */
// @retail 0x24b664
void s_player_slot_profile::sign_in(player_sign_in_callback callback)
{
	if (profile_index != NONE)
	{
		if (xuid_valid(&user.xuid))
		{
			value260 = 1;
			this->callback = callback;
			sign_in_live();
		}
		else
		{
			char name[16] = { 0 };

			value260 = 0;
			function_18fb34(player, &settings, profile_index);
			function_120df0(player, (wchar_t const *)settings.name);
			function_190001(player, name);
			if (callback)
			{
				callback(player, true);
			}
		}
	}
	else if (callback)
	{
		callback(player, false);
	}
}

// @retail 0x24b6ea
void s_player_slot_profile::show_dialog(player_sign_in_callback callback, long dialog_id)
{
	this->callback = callback;
	dialog_choice_show_default(3, 4, 1 << player, function_24ba7d, dialog_id);
}

/* starts the Live sign in task, with a screen that waits for it */
// @retail 0x24b824
void s_player_slot_profile::sign_in_live()
{
	if (!window_manager_channel_in_use(1))
	{
		long task;

		voice_stop_engine();
		task = function_6c8b0(&user, player);
		if (task != NONE)
		{
			g_475338 = NONE;
			function_1487c3(player, task, (long)function_24b869, 0, (long)this);
		}
		else
		{
			sign_in_failed();
		}
	}
}

// @retail 0x24ba33
void s_player_slot_profile::sign_in_failed()
{
	voice_start_engine();
	value260 = 0;
	if (callback)
	{
		callback(player, false);
	}
}
