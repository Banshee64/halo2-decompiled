// @flags /O2 /Gr
/* UNKNOWN_059670.CPP: the current and the other session (g_527364,
   g_52736c) as the menus query them (outside lane H's region, decompiled
   by lane H because its menus call these with register arguments) */

#include "cseries.h"
#include <xtl.h>
#include <xonline.h>
#include "globals.h"
#include "network_session.h"

struct s_network_session_membership;

extern s_597d0_object *g_52736c;

#define SESSION_STATE_IS_LIVE(state) ((state) > 2 && (state) <= 8)

// @retail 0x592f0
bool function_592f0(void)
{
	bool result = false;
	if (g_527330)
	{
		c_network_session *session = (c_network_session *)g_527364;
		long state = session->state;
		if (state && SESSION_STATE_IS_LIVE(state))
		{
			result = session->current_member == session->value50;
		}
	}
	return result;
}

// @retail 0x59670
bool function_59670(c_network_session **session)
{
	bool result = false;
	if (g_527330)
	{
		c_network_session *current = (c_network_session *)g_527364;
		if (current->state)
		{
			if (session)
			{
				*session = current;
			}
			result = true;
		}
	}
	return result;
}

// @retail 0x596a0
bool function_596a0(c_network_session **session)
{
	bool result = false;
	if (g_527330)
	{
		c_network_session *other = (c_network_session *)g_52736c;
		if (other->state)
		{
			if (session)
			{
				*session = other;
			}
			result = true;
		}
	}
	return result;
}

// @retail 0x5a680
s_network_session_membership *function_5a680(c_network_session *session, long *current_member, long *member_index)
{
	if (current_member)
	{
		*current_member = session->current_member;
	}
	if (member_index)
	{
		*member_index = session->member_index;
	}
	return (s_network_session_membership *)&session->value4c;
}
