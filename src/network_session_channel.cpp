// @flags /O2 /Ob1 /Gr
/* NETWORK_SESSION_CHANNEL.CPP: the member of a session that a channel belongs to.
   Retail never inlines it, so it has its own /Ob1 file (lane D) */

#include "unknown_11c920.h"
#include "unknown_059ad0.h"

// @retail 0x5f670
long network_session_find_member_by_channel(c_class_58d20 *session, long channel_index)
{
	long result = NONE;

	if (channel_index != NONE)
	{
		for (long i = 0; i < MAXIMUM_PLAYERS_PER_SESSION; i++)
		{
			if (session->member_states[i].unknown00 && session->member_states[i].unknown04 == channel_index)
			{
				result = i;
				break;
			}
		}
	}
	return result;
}
