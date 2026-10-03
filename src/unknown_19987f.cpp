// @flags /O1 /Gr
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
	byte unknown00[8];
	long member_count;
	byte unknown0c[0x10cc - 0xc];
	long value10cc;
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
	if (value >= 0)
	{
		if (value <= 1)
		{
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
		}
		else if (value == 2)
		{
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
		}
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
		result = function_5a680(session, &current_member, &member_index)->value10cc;
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
		result = function_5a680(session, &current_member, &member_index)->value10cc;
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
		count = function_5a680(session, &current_member, &member_index)->value10cc;
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
	if (g_4d8ba0)
	{
		switch (function_0592d0())
		{
		case 2:
		case 5:
		case 6:
		case 7:
		case 8:
			return false;
		}
		return true;
	}
	return false;
}

// @retail 0x19a279
long function_19a279(void)
{
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
	return 0;
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
