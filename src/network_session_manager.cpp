// @flags /O2 /Ob1 /Gr
/* NETWORK_SESSION_MANAGER.CPP: the session manager (0x527330): its state
   machine (the states at 0x52738c..0x527fd8, the owner at 0x527334), the
   game session (g_527364) and the other session (g_52736c) (lane D).
   The manager's globals are separate globals by address, as elsewhere:
   g_527330 (initialized), g_527334 (the state), g_527338 (the states),
   g_527364/g_52736c (the sessions) and the state objects. */

#include "cseries.h"
#include <xtl.h>
#include <string.h>
#include "globals.h"
#include "network_session.h"
#include "unknown_058dd0.h"

extern s_597d0_object *g_52736c;

/* the owner's state table and flags */
c_session_state *g_527338[10];
void *g_527360;
bool g_52737c;
bool g_52737d;

/* the states */
c_session_state_none g_52738c;
c_session_state_pre_game g_52739c;
c_session_state_start_game g_5273bc;
c_session_state_in_game g_5273d0;
c_session_state_post_game g_5273e8;
c_session_state_joining g_5273f8;
c_session_state_matchmaking g_527500;
c_session_state_start_match g_527f98;
c_session_state_in_match g_527fb8;
c_session_state_post_match g_527fd8;

/* the session being tracked for debugging: when it changes, the flags are set */
long g_510518;
bool g_51051c;
bool g_51051d;
s_session_id g_510528;

/* src/unknown_058cb0.cpp, src/unknown_059670.cpp, src/unknown_0592d0.cpp */
bool function_59670(c_network_session **session);
bool function_596a0(c_network_session **session);
dword function_0592d0(void);

/* src/network_session_interface.cpp, src/unknown_1932c0.cpp */
long network_session_interface_get_value_49c8(void);
struct s_surface_description;
s_surface_description *function_192e60(long index);
bool function_193470(s_surface_description *variant);

/* src/network_session.cpp */
void network_session_close(c_network_session *session);
s_session_id *network_session_get_id(c_network_session *session);
bool network_session_parameters_set_value49f8(c_network_session *session, long value);
bool network_session_parameters_set_mode(c_network_session *session, long mode);
bool network_session_request_mode_acknowledge(c_network_session *session);

#define PIN(value, lower, upper) ((value) < (lower) ? (lower) : ((value) > (upper) ? (upper) : (value)))

static inline void session_tracking_changed(void)
{
	if (g_510518)
	{
		g_51051c = true;
		g_51051d = true;
	}
}

static inline c_network_session *session_manager_session_a(void)
{
	return (c_network_session *)g_527364;
}

static inline c_network_session *session_manager_session_b(void)
{
	return (c_network_session *)g_52736c;
}

// @retail 0x59260
bool network_session_manager_is_joining(void)
{
	bool result = false;
	if (g_527330 && g_527334 == 5)
		result = true;
	return result;
}

// @retail 0x59280
void network_session_manager_check_joining_leader(void)
{
	if (g_527330 && g_527334 == 5 && !g_5273f8.unknown104)
	{
		c_network_session *session = g_5273f8.owner->session_a;
		if (!session->state || function_058d50(session))
			g_5273f8.flage9 = true;
	}
}

// @retail 0x59330
bool network_session_manager_session_unready(void)
{
	bool result = false;
	c_network_session *session;
	if (function_59670(&session))
	{
		if (!function_058d90(session))
			result = true;
	}
	return result;
}

// @retail 0x598f0
void network_session_check_tracking(c_network_session *session)
{
	if (g_510518)
	{
		if (!memcmp(&g_510528, network_session_get_id(session), sizeof(s_session_id)))
			session_tracking_changed();
	}
}

// @retail 0x59360
void network_session_manager_leave_session_a(bool close)
{
	c_network_session *session = session_manager_session_a();
	if (session->state)
	{
		network_session_check_tracking(session);
		if (close)
			network_session_close(session);
		else
			network_session_leave(session, false);
	}
}

// @retail 0x593a0
void network_session_manager_leave_session_b(bool close)
{
	c_network_session *session = session_manager_session_b();
	if (session->state)
	{
		network_session_check_tracking(session);
		if (close)
			network_session_close(session);
		else
			network_session_leave(session, false);
	}
}

/* called by unknown_03d380.cpp under its old name */
// @retail 0x593e0
void function_593e0(void)
{
	c_network_session *session_b = session_manager_session_b();
	if (session_b->state)
	{
		network_session_check_tracking(session_b);
		network_session_close(session_b);
	}
	c_network_session *session_a = session_manager_session_a();
	if (session_a->state)
	{
		network_session_check_tracking(session_a);
		network_session_close(session_a);
	}
	long state = g_527334;
	g_52737d = false;
	g_52737c = true;
	if (state)
	{
		c_session_state *previous = g_527338[state];
		c_session_state *next = g_527338[0];
		previous->leave((long)next);
		g_527334 = 0;
		next->enter((long)previous, 0, 0);
	}
	g_52737c = false;
}

// @retail 0x59470
bool network_session_manager_session_a_established(void)
{
	c_network_session *session = session_manager_session_a();
	if (function_058d70(session) && !session->value18)
		return true;
	return false;
}

// @retail 0x594f0
long network_session_manager_get_match_mode(void)
{
	long mode = g_527500.mode;
	if (mode == 2)
		mode = g_527f98.mode;
	return mode;
}

static inline long session_get_value49f8(c_network_session *session)
{
	long result = 0;
	if (function_058d70(session))
		result = session->value49f8;
	return result;
}

// @retail 0x59500
long network_session_manager_get_value49f8(void)
{
	c_network_session *session = session_manager_session_a();
	if (!function_058d70(session))
		return 1;
	return session_get_value49f8(session);
}

// @retail 0x59530
void network_session_manager_set_value49f8(long value)
{
	c_network_session *session = session_manager_session_a();
	if (function_058d70(session) && function_058d50(session))
		network_session_parameters_set_value49f8(session, value);
}

static inline long session_get_value49ac(c_network_session *session)
{
	long result = NONE;
	if (function_058d70(session) && session->flag49a8)
		result = session->value49ac;
	return result;
}

static inline long session_get_established_value49ac(c_network_session *session)
{
	long result = NONE;
	if (function_058d70(session))
		result = session_get_value49ac(session);
	return result;
}

// @retail 0x59590
long network_session_manager_get_value49ac(void)
{
	long result = NONE;
	if (g_527334 == 7)
		result = session_get_established_value49ac(g_527f98.owner->session_b);
	return result;
}

// @retail 0x595e0
void network_session_manager_request_mode_acknowledge(void)
{
	if (g_527334 == 6)
	{
		session_tracking_changed();
		network_session_request_mode_acknowledge(g_527500.owner->session_a);
	}
}

// @retail 0x59610
bool network_session_manager_set_mode(void)
{
	bool result = false;
	if (g_527334)
	{
		if (g_527334 == 1)
			return true;
		c_network_session *session = session_manager_session_a();
		if (function_058d70(session) && function_058d50(session))
		{
			if (network_session_parameters_set_mode(session, 1))
				return true;
		}
	}
	return result;
}

// @retail 0x596d0
bool network_session_manager_get_session(c_network_session **session)
{
	bool result = false;
	switch (function_0592d0())
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		result = function_59670(session);
		break;
	case 6:
	{
		long index = network_session_interface_get_value_49c8();
		result = function_59670(session);
		if (index != NONE)
		{
			s_surface_description *variant = function_192e60(index);
			if (variant && !function_193470(variant))
			{
				c_network_session *other;
				if (function_596a0(&other))
				{
					result = true;
					if (session)
						*session = other;
				}
			}
		}
		break;
	}
	case 7:
	case 8:
	case 9:
		result = function_596a0(session);
		break;
	}
	return result;
}

// @retail 0x59780
bool network_session_manager_get_any_session(c_network_session **session)
{
	bool result = function_596a0(session);
	if (!result)
		result = function_59670(session);
	return result;
}

// @retail 0x59840
bool network_session_manager_get_hosted_session(c_network_session **session)
{
	bool result = false;
	if (g_527330)
	{
		c_network_session *current = session_manager_session_a();
		if (current->function_058d20() && current->value18 == 1)
		{
			if (session)
				*session = current;
			result = true;
		}
	}
	return result;
}

/* a value of the session's properties: a kind and the values it uses, each
   pinned to its range */
struct s_session_property_value
{
	long kind;
	long value04;
	long value08;
	long value0c;
	long value10;
	long value14;
	long value18;
};

// @retail 0x599b0
void session_property_value_set_kind2(s_session_property_value *value, long value14)
{
	value->value04 = 0;
	value->value08 = 0;
	value->value0c = 0;
	value->value10 = 0;
	value->value14 = 0;
	value->value18 = 0;
	value->kind = 2;
	value->value14 = PIN(value14, 0, 1023);
}

// @retail 0x599f0
void session_property_value_set_kind3(s_session_property_value *value, long value18)
{
	value->value04 = 0;
	value->value08 = 0;
	value->value0c = 0;
	value->value10 = 0;
	value->value14 = 0;
	value->value18 = 0;
	value->kind = 3;
	value->value18 = PIN(value18, 0, 1023);
}

// @retail 0x59a30
void session_property_value_set_kind1(s_session_property_value *value, long value04, long value08, long value0c, long value10)
{
	value->value04 = 0;
	value->value08 = 0;
	value->value0c = 0;
	value->value10 = 0;
	value->value14 = 0;
	value->value18 = 0;
	value->kind = 1;
	value->value04 = PIN(value04, 0, 15);
	value->value08 = PIN(value08, 0, 15);
	value->value0c = PIN(value0c, 0, 63);
	value->value10 = PIN(value10, 0, 1023);
}

// @retail 0x59ab0
void session_property_value_set_kind4(s_session_property_value *value)
{
	value->value04 = 0;
	value->value08 = 0;
	value->value0c = 0;
	value->value10 = 0;
	value->value14 = 0;
	value->value18 = 0;
	value->kind = 4;
}
