// @flags /O1 /Oi /arch:SSE /Gr
/* UNKNOWN_19987F.CPP: the menus' view of the network session: the network
   state the interface shows, the session members and the session queries
   (lane H) */

#include "cseries.h"
#include "data_array.h"
#include <xtl.h>
#include <xonline.h>
#include <string.h>
#include "global_preferences.h"
#include "globals.h"
#include "network_session.h"
#include "network_session_manager.h"

/* the membership block at +0x4c of the session (unknown_059670.cpp) */
struct s_network_session_membership
{
	long value4c;
	long value50;
	long member_count;
	s_session_member members[16];
	long player_count;
	dword player_mask;
	s_network_session_player players[16];
};

bool function_59670(c_network_session **session);
bool function_596a0(c_network_session **session);
bool function_592f0(void);
s_network_session_membership *function_5a680(c_network_session *session, long *current_member, long *member_index);
bool function_058d70(c_network_session *s);
dword function_0592d0(void);

long network_session_interface_get_value_18(void);
long network_session_interface_get_value_49a4(void);
long network_session_interface_get_value_498c(void);
byte *network_session_interface_get_data_49a1(void);
byte *network_session_interface_get_data_4db0(void);
bool network_session_interface_get_values_4d08(long *a, long *b, byte **c);
long network_session_interface_get_value_49b0(void);
bool network_session_interface_can_add_player(void);
long function_190262(long value);

bool network_session_get_membership(c_network_session *session, long *value4c, long *host_member_index, long *local_member_index, long *value50, long *member_count, s_session_member **members, long *player_count, dword *player_mask, s_network_session_player **players);

extern bool g_4d8ba0; /* network_connection.cpp */

bool network_session_interface_set_value49a1(const byte *value);
bool network_session_interface_set_value498c(long value);
bool network_session_interface_set_value49c4(void);
bool network_session_interface_set_value4d08_and_stop_countdown(long value4d08, long value4d0c, const char *string);
bool xuid_equal(XUID const *a, XUID const *b, bool compare_guest_number);
const char *levels_get_path(long campaign_id, long map_id);

// @retail 0x19a84e
bool function_19a84e(long *a, long *b)
{
	byte *c;
	return network_session_interface_get_values_4d08(a, b, &c);
}

// @retail 0x19989d
long function_19989d(void)
{
	long result = NONE;
	long value = network_session_interface_get_value_18();
	byte *data = network_session_interface_get_data_4db0();
	long a;
	long b;

	if (!function_19a84e(&a, &b))
	{
		a = NONE;
	}
	switch (value)
	{
	case 0:
	case 1:
		if (a != NONE)
		{
			return value ? 2 : 0;
		}
		if (data)
		{
			long state = *(long *)(data + 0x44);
			if (state > 0 && (state <= 4 || state > 6 && state <= 9))
			{
				return (value != 0) * 2 + 1;
			}
		}
		break;
	case 2:
		switch (network_session_interface_get_value_49a4())
		{
		case 1:
			if (a != NONE)
			{
				return 4;
			}
			if (data)
			{
				long state = *(long *)(data + 0x44);
				if (state > 0 && (state <= 4 || state > 6 && state <= 9))
				{
					return 5;
				}
			}
			break;
		case 2:
			return 6;
		}
		break;
	}
	return result;
}

// @retail 0x199951
bool function_199951(long state)
{
	return state == 1 || state == 3 || state == 5;
}

// @retail 0x199967
bool function_199967(void)
{
	return function_199951(function_19989d());
}

// @retail 0x199971
bool function_199971(void)
{
	return function_19989d() == 6;
}

// @retail 0x19997f
bool function_19997f(long state)
{
	return state == 0 || state == 2 || state == 4;
}

// @retail 0x199994
bool function_199994(void)
{
	return function_19997f(function_19989d());
}

// @retail 0x19999e
bool function_19999e(long state)
{
	return state == 0 || state == 1;
}

// @retail 0x1999b3
bool function_1999b3(void)
{
	return function_19999e(function_19989d());
}

// @retail 0x1999bf
bool function_1999bf(long state)
{
	return state == 2 || state == 3;
}

// @retail 0x1999d7
bool function_1999d7(void)
{
	return function_1999bf(function_19989d());
}

// @retail 0x1999e3
bool function_1999e3(long state)
{
	return state == 4 || state == 5 || state == 6;
}

// @retail 0x1999f9
bool function_1999f9(void)
{
	return function_1999e3(function_19989d());
}

// @retail 0x199eaa
byte function_199eaa(void)
{
	byte result = 0;
	byte *data = network_session_interface_get_data_49a1();
	if (data)
	{
		result = *data;
	}
	return result;
}

// @retail 0x199ebc
long function_199ebc(void)
{
	long result = 0;
	c_network_session *session = NULL;

	if (function_59670(&session) && function_058d70(session))
	{
		long current_member;
		long member_index;
		result = function_5a680(session, &current_member, &member_index)->player_count;
	}
	return result;
}

// @retail 0x199ef8
long function_199ef8(void)
{
	long result = 0;
	c_network_session *session = NULL;

	if (function_596a0(&session) && function_058d70(session))
	{
		long current_member;
		long member_index;
		result = function_5a680(session, &current_member, &member_index)->player_count;
	}
	return result;
}

// @retail 0x199f34
long function_199f34(void)
{
	long result = 0;
	c_network_session *session = NULL;

	if (function_59670(&session) && function_058d70(session))
	{
		long current_member;
		long member_index;
		result = function_5a680(session, &current_member, &member_index)->member_count;
	}
	return result;
}

// @retail 0x199fd6
long function_199fd6(void)
{
	long count = 0;
	c_network_session *session = NULL;

	if (function_59670(&session) && function_058d70(session))
	{
		long current_member;
		long member_index;
		count = function_5a680(session, &current_member, &member_index)->player_count;
	}
	return 16 - count;
}

// @retail 0x19a015
bool function_19a015(void)
{
	if (network_session_interface_can_add_player() && function_199fd6() > 0)
	{
		return true;
	}
	return false;
}

// @retail 0x19a161
long function_19a161(void)
{
	switch (network_session_interface_get_value_498c())
	{
	case 0:
		return 0;
	case 1:
		return 1;
	}
	return 2;
}

// @retail 0x19a250
bool function_19a250(void)
{
	bool result = false;

	if (g_4d8ba0)
	{
		long mode = function_0592d0();
		if (mode <= 1 || mode != 2 && (mode <= 4 || mode > 8))
		{
			result = true;
		}
	}
	return result;
}

// @retail 0x19a279
long function_19a279(void)
{
	long result = 0;

	if (g_4d8ba0)
	{
		switch (function_0592d0())
		{
		case 0:
			return 0;
		case 1:
			return 3;
		case 2:
			return 3;
		case 3:
			return 4;
		case 4:
			return 6;
		case 5:
			return 7;
		case 6:
			return 2;
		case 7:
			return 2;
		case 8:
			return 4;
		case 9:
			return 6;
		default:
			__assume(0);
		}
	}
	return result;
}

// @retail 0x19a8ef
bool function_19a8ef(void)
{
	bool result = false;
	if (network_session_interface_get_value_49b0() == 2)
	{
		result = true;
	}
	return result;
}

// @retail 0x19a935
bool function_19a935(void)
{
	return network_session_interface_get_value_18() == 2;
}

// @retail 0x19b3e3
long function_19b3e3(void)
{
	long count = 0;
	long index;

	for (index = 0; index != NONE; index = function_190262(index))
	{
		if (TEST_FIELD_BIT(g_54e8e0[index].flag4))
		{
			count++;
		}
	}
	return count;
}

// @retail 0x199f6d
long function_199f6d(void)
{
	long count = 0;
	c_network_session *session = NULL;

	if (function_59670(&session))
	{
		long host_member_index;
		dword player_mask;
		s_network_session_player *players;

		if (network_session_get_membership(session, NULL, &host_member_index, NULL, NULL, NULL, NULL, NULL, &player_mask, &players))
		{
			long index;

			for (index = 0; index < 16; index++)
			{
				if ((player_mask & (1 << index)) && players[index].member_index == host_member_index)
				{
					count++;
				}
			}
		}
	}
	return count;
}

// @retail 0x19a951
bool function_19a951(long player_index)
{
	bool result = false;

	if (player_index != NONE)
	{
		c_network_session *session = NULL;
		long index = player_index & 0xffff;

		if (function_59670(&session) && function_058d70(session))
		{
			long current_member;
			long member_index;
			dword player_mask = function_5a680(session, &current_member, &member_index)->player_mask;

			if (index >= 0 && index < 16 && (player_mask & (1 << index)))
			{
				result = true;
			}
			else
			{
				result = false;
			}
		}
	}
	return result;
}

// @retail 0x19a9b4
bool function_19a9b4(long player_index)
{
	bool result = false;

	if (player_index != NONE)
	{
		c_network_session *session = NULL;
		long index = player_index & 0xffff;

		if (function_596a0(&session) && function_058d70(session))
		{
			long current_member;
			long member_index;
			dword player_mask = function_5a680(session, &current_member, &member_index)->player_mask;

			if (index >= 0 && index < 16 && (player_mask & (1 << index)))
			{
				result = true;
			}
			else
			{
				result = false;
			}
		}
	}
	return result;
}

// @retail 0x19aa17
long function_19aa17(long value)
{
	long result = NONE;
	c_network_session *session = NULL;

	if (function_59670(&session))
	{
		long host_member_index;
		long member_count;
		s_session_member *members;
		long player_count;
		dword player_mask;
		s_network_session_player *players;

		if (network_session_get_membership(session, NULL, &host_member_index, NULL, NULL, &member_count, &members, &player_count, &player_mask, &players))
		{
			long index;

			for (index = 0; index < 4; index++)
			{
				long player_index = members[host_member_index].player_indices[index];
				if (player_index != NONE && players[player_index].unknown14 == value)
				{
					result = player_index;
					break;
				}
			}
		}
	}
	return result;
}

// @retail 0x19aaa5
byte *function_19aaa5(long player_index)
{
	long index = player_index & 0xffff;
	byte *result = NULL;
	c_network_session *session = NULL;

	if (function_59670(&session))
	{
		dword player_mask;
		s_network_session_player *players;

		if (network_session_get_membership(session, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &player_mask, &players) && (player_mask & (1 << index)))
		{
			result = players[index].propertiesa8;
		}
	}
	return result;
}

// @retail 0x19ab0e
byte *function_19ab0e(long player_index)
{
	long index = player_index & 0xffff;
	byte *result = NULL;
	c_network_session *session = NULL;

	if (function_596a0(&session))
	{
		dword player_mask;
		s_network_session_player *players;

		if (network_session_get_membership(session, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &player_mask, &players) && (player_mask & (1 << index)))
		{
			result = players[index].propertiesa8;
		}
	}
	return result;
}

// @retail 0x19ab77
bool function_19ab77(long player_index)
{
	bool result = false;
	long index = player_index & 0xffff;
	c_network_session *session = NULL;

	if (function_59670(&session))
	{
		long host_member_index;
		dword player_mask;
		s_network_session_player *players;

		if (network_session_get_membership(session, NULL, &host_member_index, NULL, NULL, NULL, NULL, NULL, &player_mask, &players) &&
			(player_mask & (1 << index)) && players[index].member_index == host_member_index)
		{
			result = true;
		}
	}
	return result;
}

// @retail 0x19abe4
bool function_19abe4(long player_index)
{
	bool result = false;
	long index = player_index & 0xffff;
	c_network_session *session = NULL;

	if (function_59670(&session))
	{
		long value50;
		dword player_mask;
		s_network_session_player *players;

		if (network_session_get_membership(session, NULL, NULL, NULL, &value50, NULL, NULL, NULL, &player_mask, &players) &&
			(player_mask & (1 << index)) && players[index].member_index == value50)
		{
			result = true;
		}
	}
	return result;
}

// @retail 0x19ac53
short function_19ac53(void)
{
	long result = NONE;
	c_network_session *session = NULL;

	if (function_59670(&session))
	{
		long value50;
		dword player_mask;
		s_network_session_player *players;

		if (network_session_get_membership(session, NULL, NULL, NULL, &value50, NULL, NULL, NULL, &player_mask, &players))
		{
			short index;

			for (index = 0; index < 16; index++)
			{
				if ((player_mask & (1 << index)) && players[index].member_index == value50)
				{
					result = index;
					break;
				}
			}
		}
	}
	return (short)result;
}

// @retail 0x19b4e4
long function_19b4e4(void)
{
	long result = NONE;
	long index;

	for (index = 0; index < 16; index++)
	{
		if (function_19a951(index) && !function_19ab77(index))
		{
			result = index;
			break;
		}
	}
	return result;
}

/* the game variants (unknown_1932c0.cpp) */
struct s_surface_description;
long network_session_interface_get_value_49c8(void);
s_surface_description *function_192e60(long index);
bool function_193470(s_surface_description *p);
bool function_193490(s_surface_description *p);
bool function_1934b0(s_surface_description *p);
bool network_session_interface_kick_player(long player_index);
bool network_session_interface_ban_player(long player_index);

// @retail 0x19a0c5
bool function_19a0c5(void)
{
	bool result = false;
	long index = network_session_interface_get_value_49c8();

	if (index != NONE)
	{
		s_surface_description *variant = function_192e60(index);
		if (variant)
		{
			result = function_193470(variant);
		}
	}
	return result;
}

// @retail 0x19a0e6
bool function_19a0e6(void)
{
	bool result = false;
	long index = network_session_interface_get_value_49c8();

	if (index != NONE)
	{
		s_surface_description *variant = function_192e60(index);
		if (variant)
		{
			result = function_193490(variant);
		}
	}
	return result;
}

// @retail 0x19a107
bool function_19a107(void)
{
	bool result = false;
	long index = network_session_interface_get_value_49c8();

	if (index != NONE)
	{
		s_surface_description *variant = function_192e60(index);
		if (variant)
		{
			result = *(long *)variant == 5;
		}
	}
	return result;
}

// @retail 0x19a127
bool function_19a127(void)
{
	bool result = false;
	long index = network_session_interface_get_value_49c8();

	if (index != NONE)
	{
		s_surface_description *variant = function_192e60(index);
		if (variant)
		{
			result = function_1934b0(variant);
		}
	}
	return result;
}

// @retail 0x19a203
bool function_19a203(void)
{
	bool result = false;

	if (g_4d8ba0 && function_592f0())
	{
		switch (function_0592d0())
		{
		case 0:
		case 2:
		case 5:
		case 6:
		case 7:
			result = false;
			break;
		case 8:
			result = !function_19a0c5();
			break;
		default:
			result = true;
			break;
		}
	}
	return result;
}

// @retail 0x19a1d4
bool function_19a1d4(long player_index)
{
	bool result = false;

	if (function_19a203() && function_19a951(player_index) && !function_19ab77(player_index))
	{
		result = network_session_interface_ban_player(player_index);
	}
	return result;
}

// @retail 0x19a179
bool function_19a179(long player_index)
{
	bool result = false;

	if (function_592f0())
	{
		if (player_index == NONE)
		{
			player_index = function_19b4e4();
			if (player_index == NONE)
			{
				return result;
			}
		}
		if (function_19a951(player_index) && !function_19ab77(player_index))
		{
			result = network_session_interface_kick_player(player_index);
		}
	}
	return result;
}

/* the voice/peer list (unknown_0b35e0.cpp) and its state */
struct s_0b35e0_entry;
s_0b35e0_entry *function_b35e0(long index);
extern long g_4d8f14;

s_peer_list_globals g_4ee4c4;

/* the user interface's allocator (unknown_1a4742.cpp) */
class c_user_interface_allocator : public c_data_allocator
{
public:
	virtual void *allocate(long size);
	virtual void deallocate(void *block);
};

extern c_user_interface_allocator g_47d92c;

/* unknown_0b3570.cpp */
bool function_b3570(long count, c_data_allocator *allocator, bool flag);
void function_b35a0(void);
void function_b35d0(bool start);

// @retail 0x199b33
void function_199b33(bool flag)
{
	g_4ee4c4.active = function_b3570(0x20, &g_47d92c, flag);
}

// @retail 0x199b45
void function_199b45(void)
{
	if (g_4ee4c4.active)
	{
		function_b35a0();
		g_4ee4c4.active = false;
	}
}

// @retail 0x199b5b
void function_199b5b(bool start)
{
	if (g_4ee4c4.active)
		function_b35d0(start);
}

// @retail 0x199b6a
long function_199b6a(long start)
{
	long result = NONE;

	if (g_4ee4c4.active)
	{
		long index;

		for (index = start != NONE ? start + 1 : 0; index < g_4d8f14; index++)
		{
			if (function_b35e0(index))
			{
				result = index;
				break;
			}
		}
	}
	return result;
}

// @retail 0x199ba5
bool function_199ba5(long index)
{
	bool result = false;

	if (g_4ee4c4.active && function_b35e0(index))
	{
		result = true;
	}
	return result;
}

// @retail 0x199bbf
byte *function_199bbf(long index)
{
	byte *result = NULL;

	if (g_4ee4c4.active)
	{
		byte *entry = (byte *)function_b35e0(index);
		if (entry)
		{
			short count = *(short *)(entry + 0x12c);
			byte *data = entry + 0x70;
			if (count >= 0 && count <= 16)
			{
				result = data;
			}
		}
	}
	return result;
}

void unicode_string_copy(word *destination, const word *source, long maximum_count);
void unicode_string_snprintf(word *buffer, long maximum_count, const word *format, ...);
void network_session_interface_set_local_name(const wchar_t *machine_name, const wchar_t *session_name);

// @retail 0x199bef
bool function_199bef(const word *machine_name, const word *session_name)
{
	bool result = false;

	if (g_4d8ba0)
	{
		word machine[16];
		word session[32];

		if (machine_name)
		{
			unicode_string_copy(machine, machine_name, 16);
		}
		else
		{
			unicode_string_snprintf(machine, 16, (const word *)L"%S", "");
		}
		unicode_string_copy(session, session_name, 32);
		network_session_interface_set_local_name((const wchar_t *)machine, (const wchar_t *)session);
		result = true;
	}
	return result;
}

bool network_session_interface_get_user_xuid(long index, XUID *xuid);
void network_session_manager_join(const void *target, long count, const void *entries, bool flag);
void network_session_manager_join_description(const s_session_description *description, long count, const void *entries);
bool network_session_manager_host_session(long mode, const XNKID *kid, const XNKEY *key);
bool network_session_manager_host_offline(void);
bool network_session_manager_host_online(void);
void function_24f9d4();
void function_199e2e(bool close);
void network_session_manager_check_joining_leader(void);

typedef bool (__stdcall *dialog_choice_callback)(long controller_index);
class c_screen_widget;
typedef bool (__stdcall *dialog_closed_callback)(c_screen_widget *screen, long dialog_id);
void dialog_ok_show(long a, long dialog_id, long b, word user_flags, dialog_choice_callback chosen, dialog_closed_callback closed);

/* joins the session the search found at the index, with the local users */
// @retail 0x199c47
void function_199c47(long index)
{
	byte *description;
	XUID users[4];
	long count;
	long i;

	network_session_manager_check_joining_leader();
	description = function_199bbf(index);
	count = 0;
	for (i = 0; i < 4; i++)
	{
		if (network_session_interface_get_user_xuid(i, &users[count]))
			count++;
	}
	network_session_manager_join_description((s_session_description *)description, count, users);
	function_24f9d4();
}

/* joins the target with the local users, or says there are none */
// @retail 0x199c94
void function_199c94(const void *target, long controller, bool flag)
{
	if (function_19b3e3() > 0)
	{
		XUID users[4];
		long count = 0;
		long i;

		network_session_manager_check_joining_leader();
		for (i = 0; i < 4; i++)
		{
			if (network_session_interface_get_user_xuid(i, &users[count]))
				count++;
		}
		network_session_manager_join(target, count, users, flag);
		function_24f9d4();
	}
	else
	{
		dialog_ok_show(1, 0x44, 4, 1 << controller, 0, 0);
	}
}

/* leaves the sessions and hosts a new one */
// @retail 0x199df9
void function_199df9(bool offline, bool system_link)
{
	function_199e2e(true);
	if (offline)
	{
		if (!system_link)
			network_session_manager_host_offline();
		else
			network_session_manager_host_session(2, NULL, NULL);
	}
	else
	{
		if (system_link)
			network_session_manager_host_session(2, NULL, NULL);
		else
			network_session_manager_host_online();
	}
}

long network_session_manager_get_match_mode(void);
long function_59570(void);
bool __stdcall function_594a0(long a, long b, long c);

/* the session state as the user interface shows it */
// @retail 0x199d7c
long function_199d7c(void)
{
	long result;

	switch (network_session_manager_get_match_mode())
	{
	case 1:
		return 1;
	case 2:
		return 8;
	case 3:
		switch (function_59570())
		{
		case 0:
			return 0;
		case 1:
			result = 3;
			break;
		case 2:
			result = 4;
			break;
		case 3:
			result = 5;
			break;
		case 4:
			result = 6;
			break;
		case 5:
			result = 7;
			break;
		default:
			return 0;
		}
		break;
	case 4:
		result = 2;
		break;
	default:
		return 0;
	}
	return result;
}

// @retail 0x199dc9
bool function_199dc9(long a, long b, long c)
{
	bool result = false;

	if (function_199d7c() == 4 || function_199d7c() == 6)
	{
		result = function_594a0(a, b, c);
	}
	return result;
}

void function_1487c3(long a, long b, long load, long c, long d);

/* leaves the sessions and goes back to the main menu */
// @retail 0x199e3c
void function_199e3c(long controller)
{
	bool close = function_199f34() <= 1;

	function_199e2e(close);
	function_1487c3(controller, NONE, 0x2523bc, 0, 0);
}

long __stdcall function_63e90(long index);

// @retail 0x199e6d
bool function_199e6d(long index)
{
	return function_63e90(index) == 0;
}

// @retail 0x199cfc
long function_199cfc(void)
{
	switch (g_527330.state_joining.unknown104)
	{
	case 0:
		return 0;
	case 1:
		return 2;
	case 2:
		return 3;
	case 3:
		return 4;
	case 4:
		return 5;
	case 5:
		return 5;
	case 6:
		return 6;
	case 7:
		return 7;
	case 8:
		return 8;
	case 9:
		return 9;
	case 11:
		return 9;
	case 12:
		return 10;
	case 13:
		return 10;
	case 16:
		return 11;
	default:
		return 11;
	}
}

/* the string id that names each network session state */
// @retail 0x19a50f
long function_19a50f(long state)
{
	long result = 0;

	switch (state)
	{
	case 0:
		result = 0x0a0001e3;
		break;
	case 1:
		result = 0x0b0001e4;
		break;
	case 2:
		result = 0x0f000205;
		break;
	case 5:
		result = 0x11000209;
		break;
	case 6:
		result = 0x190007bb;
		break;
	case 4:
		result = 0x0a000208;
		break;
	case 9:
		result = 0x240001ef;
		break;
	case 11:
		result = 0x0c0001f2;
		break;
	case 12:
		result = 0x0e0001f3;
		break;
	case 13:
	case 14:
		result = 0x210001f4;
		break;
	case 15:
		result = 0x230001f5;
		break;
	case 16:
		result = 0x0f0001f7;
		break;
	case 17:
		result = 0x0f0001f8;
		break;
	case 18:
		result = 0x140001f9;
		break;
	case 19:
		result = 0x190001fa;
		break;
	case 20:
		result = 0x160001fb;
		break;
	case 21:
		result = 0x0f0001fc;
		break;
	case 22:
		result = 0x1c0001f6;
		break;
	case 3:
	case 24:
		result = 0x0f000206;
		break;
	case 25:
		result = 0x1a000207;
		break;
	}

	return result;
}

/* the interface state for each network session state */
// @retail 0x19a5fd
long function_19a5fd(long state)
{
	struct
	{
		struct
		{
			long state;
			long value;
		} entries[16];
		long default_value;
	} table =
	{
		{
			{ 0, 0x9 },
			{ 1, 0xb },
			{ 2, 0xc },
			{ 3, 0xd },
			{ 4, 0xe },
			{ 5, 0xf },
			{ 6, 0x10 },
			{ 7, 0x11 },
			{ 8, 0x12 },
			{ 9, 0x13 },
			{ 0xa, 0x14 },
			{ 0xb, 0x15 },
			{ 0xc, 0x16 },
			{ 0xd, 0x17 },
			{ 0xe, 0x18 },
			{ 0xf, 0x19 },
		},
		0
	};

	for (dword i = 0; i < 16; i++)
	{
		if (table.entries[i].state == state)
			return table.entries[i].value;
	}

	return table.default_value;
}

// @retail 0x199e7e
bool function_199e7e(byte value)
{
	bool result = false;

	if (network_session_interface_get_data_49a1())
	{
		byte copy = value;

		if (network_session_interface_set_value49a1(&copy))
			result = true;
	}
	return result;
}

// @retail 0x19a148
bool function_19a148(long privacy)
{
	long value;

	switch (privacy)
	{
	case 0:
		value = 0;
		break;
	case 1:
		value = 1;
		break;
	default:
		value = 2;
		break;
	}
	return network_session_interface_set_value498c(value);
}

// @retail 0x19a1bd
bool function_19a1bd(void)
{
	bool result = false;

	if (function_592f0())
		result = network_session_interface_set_value49c4();
	return result;
}

// @retail 0x19a6f2
bool function_19a6f2(long campaign_id, long map_id)
{
	bool result = false;
	const char *path = levels_get_path(campaign_id, map_id);

	if (path)
	{
		result = network_session_interface_set_value4d08_and_stop_countdown(campaign_id, map_id, path);
		if (result)
		{
			global_preferences_globals.current.unknown16c = map_id;
			global_preferences_globals.dirty = true;
		}
	}
	return result;
}

// @retail 0x19acc6
bool function_19acc6(XUID const *xuid)
{
	bool found = false;
	c_network_session *session = NULL;

	if (function_59670(&session))
	{
		dword player_mask;
		s_network_session_player *players;

		if (network_session_get_membership(session, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &player_mask, &players))
		{
			long index;

			for (index = 0; index < 16; index++)
			{
				if (found)
					break;
				if (player_mask & (1 << index))
					found = xuid_equal((XUID const *)&players[index].user_id, xuid, true);
			}
		}
	}
	return found;
}

// @retail 0x19ad39
long function_19ad39(c_network_session *session, XUID const *xuid)
{
	long result = NONE;

	if (session && xuid && function_058d70(session))
	{
		long index;

		for (index = 0; index < 16; index++)
		{
			if ((session->player_mask & (1 << index)) && xuid_equal((XUID const *)&session->players[index].user_id, xuid, true))
			{
				result = index;
				break;
			}
		}
	}
	return result;
}
/* unknown_01cf50.cpp's view of the session */
struct s_597d0_object;
bool function_597d0(s_597d0_object **out);

// @retail 0x19ad9c
long function_19ad9c(XUID const *xuid)
{
	c_network_session *session = NULL;
	long result = NONE;

	if (function_597d0((s_597d0_object **)&session))
		result = function_19ad39(session, xuid);
	return result;
}

// @retail 0x19adca
long function_19adca(XUID const *xuid)
{
	c_network_session *session = NULL;
	long result = NONE;

	if (function_59670(&session))
		result = function_19ad39(session, xuid);
	return result;
}
/* the session manager (network_session_manager.cpp) */
void network_session_manager_check_joining_leader(void);
void network_session_manager_leave_session_a(bool close);
void network_session_manager_leave_session_b(bool close);
bool network_session_manager_set_mode(void);

/* resets the peer list's state */
// @retail 0x19987f
void function_19987f(void)
{
	network_session_manager_check_joining_leader();
	memset(&g_4ee4c4, 0, sizeof(g_4ee4c4));
}


// @retail 0x19a942
void function_19a942(void)
{
	if (function_592f0())
		network_session_manager_set_mode();
}