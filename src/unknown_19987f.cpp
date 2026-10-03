// @flags /O1 /arch:SSE /Gr
/* UNKNOWN_19987F.CPP: the menus' view of the network session: the network
   state the interface shows, the session members and the session queries
   (lane H) */

#include "cseries.h"
#include <xtl.h>
#include <xonline.h>
#include "globals.h"
#include "network_session.h"

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

bool g_4d8ba0;

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

struct s_peer_list_globals
{
	bool active;
	byte unknown01[0x20 - 1];
};

s_peer_list_globals g_4ee4c4;

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

long g_5274fc;

// @retail 0x199cfc
long function_199cfc(void)
{
	switch (g_5274fc)
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
