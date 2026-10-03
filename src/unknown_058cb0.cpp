// @flags /O2 /Ob1 /Gr
/* UNKNOWN_058CB0.CPP: session state queries */

#include "cseries.h"
#include "globals.h"
#include "unknown_058dd0.h"
#include "network_session_manager.h"
#include <string.h>

struct s_session_address_list
{
	long entries[4 * 250];
};

long g_4cd854[16];
s_session_address_list g_4c99d4[16];

struct s_session_address
{
	long a;
	long b;
	long c;
};

// @retail 0x58cb0
bool function_058cb0(long index, const s_session_address *address)
{
	bool result = false;
	long count = g_4cd854[index];
	if (count != NONE && TEST_FIELD_BIT(g_54e8e0[index].flag5))
	{
		for (long i = 0; i < count; i++)
		{
			if (memcmp(address, &g_4c99d4[index].entries[i * 4], sizeof(*address)) == 0)
			{
				result = true;
				break;
			}
		}
	}
	return result;
}

// @retail 0x58d20
inline bool c_network_session::function_058d20()
{
	bool result = false;
	long current = state;
	if (current == 5 || current == 6 || current == 7 || current == 8)
	{
		result = true;
	}
	else
	{
		/* retail keeps a dead stack store of the state on this path,
		   inlined into every caller: a volatile local reproduces it */
		volatile long unused = current;
	}
	return result;
}

// @retail 0x58d50
inline bool function_058d50(c_network_session *s)
{
	bool result = false;
	if (s->state > 2 && s->state <= 8)
	{
		result = s->current_member == s->value50;
	}
	return result;
}

// @retail 0x58d70
inline bool function_058d70(c_network_session *s)
{
	if (s->state > 2 && s->state <= 8)
	{
		return true;
	}
	return false;
}

// @retail 0x58d90
bool function_058d90(c_network_session *s)
{
	bool result = false;
	switch (s->state)
	{
	case 2:
		result = true;
		break;
	case 3:
		break;
	case 4:
		result = true;
		break;
	case 5:
		break;
	case 6:
		result = true;
		break;
	case 7:
		result = s->flag7420;
		break;
	case 8:
		result = s->flag7420;
		break;
	}
	return result;
}

/* the session manager's embedded states and client */
// @retail 0x58e70
s_session_states::s_session_states()
{
}
