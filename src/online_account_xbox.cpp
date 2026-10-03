// @flags /O2 /Gr
/* ONLINE_ACCOUNT_XBOX.CPP: Xbox Live logon users, services and passcodes
   (lane D) */

#include "cseries.h"
#include <xtl.h>
#include <xonline.h>
#include <string.h>
#include "online_tasks.h"

/* the services the logon asks for; the last one is not a Live service */
struct s_online_service
{
	DWORD service_id;
	long unknown04;
	bool connect;
};

#define k_online_service_count 13

s_online_service g_467178[k_online_service_count] =
{
	{ 0x2, 1, false },
	{ 0x4, 1, false },
	{ 0x6, 1, false },
	{ 0x7, 1, false },
	{ 0x8, 1, false },
	{ 0x9, 1, false },
	{ 0xd, 1, false },
	{ 0xf, 1, false },
	{ 0x10, 1, false },
	{ 0x12, 1, false },
	{ 0x13, 1, false },
	{ 0x14, 1, false },
	{ 0x4d530064, 0, false },
};

/* the presence an account publishes, packed into a dword */
struct s_online_presence_source
{
	long state;
	long unknown04;
	long unknown08;
	short minutes_a;
	short minutes_b;
};

union s_online_presence
{
	struct
	{
		dword magic : 16;
		dword time : 8;
		dword state : 8;
	};
	struct
	{
		dword unused : 28;
		dword unknown04 : 2;
		dword unknown08 : 2;
	};
};

void online_presence_build(s_online_presence *presence, const s_online_presence_source *source);

// @retail 0x6b590
void online_get_title_name(DWORD title_id, WCHAR *name, long name_length)
{
	DWORD language = XGetLanguage();
	if (FAILED(XOnlineFriendsGetTitleName(title_id, language, name_length, name)))
	{
		wcsncpy(name, L"?", name_length - 1);
		name[name_length - 1] = 0;
	}
}

// @retail 0x6ced0
void online_get_logon_users(XONLINE_USER *users)
{
	memset(users, 0, sizeof(XONLINE_USER) * XONLINE_MAX_LOGON_USERS);
	XONLINE_USER *logon_users = XOnlineGetLogonUsers();
	if (logon_users)
		memcpy(users, logon_users, sizeof(XONLINE_USER) * XONLINE_MAX_LOGON_USERS);
}

static inline long controller_index_next(long index)
{
	long next = NONE;
	if (index >= 0 && index < XONLINE_MAX_LOGON_USERS - 1)
		next = index + 1;
	return next;
}

/* publishes an empty presence for every signed-in, non-guest user */
// @retail 0x6c1d0
void online_presence_clear(XONLINE_USER *users)
{
	for (long i = 0; i != NONE; i = controller_index_next(i))
	{
		XONLINE_USER *user = &users[i];
		if (user->xuid.qwUserID != 0 && user->szGamertag[0] && SUCCEEDED(user->hr))
		{
			XUID *xuid = user ? &user->xuid : 0;
			if (!XOnlineIsUserGuest(xuid->dwUserFlags))
			{
				s_online_presence presence;
				XNKID session_id = { 0 };
				s_online_presence_source source;

				source.state = 1;
				source.minutes_a = 0;
				source.minutes_b = 0x7fff;
				source.unknown04 = 0;
				source.unknown08 = 0;
				online_presence_build(&presence, &source);
				if (online_logon_connected())
					XOnlineNotificationSetState(i, 0, session_id, sizeof(presence), (BYTE *)&presence);
			}
		}
	}
}

// @retail 0x6c6c0
long online_get_logon_users_status(void)
{
	XONLINE_USER users[XONLINE_MAX_LOGON_USERS];
	long status = 0;

	online_get_logon_users(users);
	for (long i = 0; i < XONLINE_MAX_LOGON_USERS; i++)
	{
		XONLINE_USER *user = &users[i];
		HRESULT hr = user->hr;
		if (user && user->xuid.qwUserID != 0 && hr != S_OK)
		{
			if (hr == XONLINE_S_LOGON_USER_HAS_MESSAGE)
				status = 1;
			else if (hr == XONLINE_E_LOGON_USER_ACCOUNT_REQUIRES_MANAGEMENT)
				return 2;
		}
	}
	return status;
}

// @retail 0x6c750
long online_task_get_change_logon_status(s_online_task *task)
{
	HRESULT results[XONLINE_MAX_LOGON_USERS];
	long status = 0;

	XOnlineChangeLogonUsersTaskGetResults((XONLINETASK_HANDLE)task->handle, results);
	for (long i = 0; i < XONLINE_MAX_LOGON_USERS; i++)
	{
		HRESULT hr = results[i];
		if (hr != S_OK)
		{
			if (hr == XONLINE_S_LOGON_USER_HAS_MESSAGE)
				status = 1;
			else if (hr == XONLINE_E_LOGON_USER_ACCOUNT_REQUIRES_MANAGEMENT)
				return 2;
		}
	}
	return status;
}

// @retail 0x6c7a0
void online_get_service_ids(DWORD *service_ids)
{
	for (long i = 0; i < k_online_service_count; i++)
		service_ids[i] = g_467178[i].service_id;
}

// @retail 0x6c800
DWORD online_get_users(XONLINE_USER *users)
{
	DWORD user_count;

	GetTickCount();
	HRESULT hr = XOnlineGetUsers(users, &user_count);
	GetTickCount();
	if (FAILED(hr))
		user_count = 0;
	return user_count;
}

// @retail 0x6c830
bool online_user_requires_passcode(const XONLINE_USER *user)
{
	bool result = false;
	if (!XOnlineIsUserGuest(user->xuid.dwUserFlags) && (user->dwUserOptions & XONLINE_USER_OPTION_REQUIRE_PASSCODE))
		result = true;
	return result;
}

// @retail 0x6c850
byte online_gamepad_button_to_passcode_byte(byte button)
{
	switch (button)
	{
	case 2:
		return XONLINE_PASSCODE_GAMEPAD_X;
	case 3:
		return XONLINE_PASSCODE_GAMEPAD_Y;
	case 6:
		return XONLINE_PASSCODE_GAMEPAD_LEFT_TRIGGER;
	case 7:
		return XONLINE_PASSCODE_GAMEPAD_RIGHT_TRIGGER;
	case 8:
		return XONLINE_PASSCODE_DPAD_UP;
	case 9:
		return XONLINE_PASSCODE_DPAD_DOWN;
	case 10:
		return XONLINE_PASSCODE_DPAD_LEFT;
	case 11:
		return XONLINE_PASSCODE_DPAD_RIGHT;
	default:
		return 0;
	}
}

/* minutes in a byte: exact below an hour, then 5- and 10-minute steps */
#define ENCODE_MINUTES(result, minutes) \
	if ((minutes) < 60) \
		(result) = (byte)(minutes); \
	else if ((minutes) < 180) \
		(result) = (byte)(((minutes) - 60) / 5 + 60); \
	else if ((minutes) < 600) \
		(result) = (byte)(((minutes) - 180) / 10 + 84); \
	else \
		(result) = 126

// @retail 0x6cf00
void online_presence_build(s_online_presence *presence, const s_online_presence_source *source)
{
	*(dword *)presence = 0;
	presence->magic = 0xfcf1;

	byte time = 0;
	if (source->minutes_a > 0)
	{
		ENCODE_MINUTES(time, source->minutes_a);
	}
	else if (source->minutes_b > 0)
	{
		ENCODE_MINUTES(time, source->minutes_b);
		time += 127;
	}
	presence->time = time;

	byte state = 0;
	switch (source->state)
	{
	case 1:
		state = 0;
		break;
	case 2:
		state = 4;
		break;
	case 3:
		state = 8;
		break;
	case 4:
		state = 12;
		break;
	case 5:
		state = 1;
		break;
	case 6:
		state = 3;
		break;
	}
	presence->state = state;
	presence->unknown08 = source->unknown08;
	presence->unknown04 = source->unknown04;
}

// @retail 0x6d210
bool online_get_accepted_game_invite(XONLINE_ACCEPTED_GAMEINVITE *invite)
{
	return SUCCEEDED(XOnlineFriendsGetAcceptedGameInvite(invite));
}

// @retail 0x6d220
void online_connect_to_service_20(void)
{
	bool connect = false;

	for (long i = 0; i < k_online_service_count; i++)
	{
		if (g_467178[i].service_id == 0x14)
			connect = g_467178[i].connect;
	}
	if (connect)
	{
		XONLINE_SERVICE_INFO info;
		if (SUCCEEDED(XOnlineGetServiceInfo(0x14, &info)))
			XNetConnect(info.serviceIP);
	}
}
