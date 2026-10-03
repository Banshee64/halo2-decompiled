// @flags /O2 /Gr
/* NETWORK_SESSION_MEMBERSHIP.CPP: the players, members and reservations of a
   network session (lane D) */

#include "cseries.h"
#include <xtl.h>
#include <xonline.h>
#include <string.h>
#include "globals.h"
#include "network_session.h"

/* the reservation search (unknown_062f40.cpp) */
struct s_reservation;
struct s_reservation_session;
bool function_062f40(s_reservation_session *session, const void *identity, s_reservation **reservation_out);

static inline long network_time_get(void)
{
	if (g_510548)
		return g_51054c;
	return GetTickCount();
}

static inline bool session_find_reservation(c_network_session *session, const void *identity, s_network_session_reservation **reservation)
{
	return function_062f40((s_reservation_session *)session, identity, (s_reservation **)reservation);
}

#define SESSION_STATE_IS_HOSTING(state) ((state) == 5 || (state) == 6 || (state) == 7 || (state) == 8)

// @retail 0x600f0
void network_session_add_player(c_network_session *session, long member_index, const XUID *xuid, long player_index, long slot)
{
	s_network_session_player *player = &session->players[player_index];
	*(XUID *)player = *xuid;
	player->slot = slot;
	player->member_index = member_index;
	player->unknown14 = NONE;
	memset(player->properties18, 0, sizeof(player->properties18));
	memset(player->propertiesa8, 0, sizeof(player->propertiesa8));
	session->members[member_index].player_indices[slot] = player_index;
	session->members[member_index].player_count++;
	session->player_mask |= 1 << player_index;
	session->player_count++;

	long state = session->state;
	if (SESSION_STATE_IS_HOSTING(state))
	{
		s_network_session_reservation *reservation = 0;
		if (session_find_reservation(session, player, &reservation))
			reservation->joined = true;
	}
}

// @retail 0x60200
void network_session_remove_player(c_network_session *session, long player_index)
{
	s_network_session_player *player = &session->players[player_index];

	long state = session->state;
	if (SESSION_STATE_IS_HOSTING(state))
	{
		s_network_session_reservation *reservation = 0;
		if (session_find_reservation(session, player, &reservation))
			reservation->joined = false;
	}
	session->members[player->member_index].player_indices[player->slot] = NONE;
	session->members[player->member_index].player_count--;
	session->player_mask &= ~(1 << player_index);
	session->player_count--;
}

// @retail 0x60fa0
void network_session_reset_membership(c_network_session *session, bool reset_limits)
{
	memset(&session->value24e0, 0, 0x2494);
	session->value24e0 = NONE;
	memset(&session->value5e28, 0, 0x14b0);
	session->value5e28 = NONE;
	memset(session->reservations, 0, sizeof(session->reservations));
	session->flag765c = false;
	session->value44 = NONE;
	for (long i = 0; i < session->member_count; i++)
	{
		session->member_states[i].flag2 = false;
		session->member_states[i].unknown08 = NONE;
		session->member_states[i].unknown0c = NONE;
		session->member_states[i].flag3 = false;
	}
	if (reset_limits)
	{
		session->value4990 = 16;
		session->value4994 = 16;
		session->update_count++;
	}
}

// @retail 0x616b0
void network_session_disconnect(c_network_session *session, long reason)
{
	memset(&session->value7420, 0, 0x1f8);
	session->value7420 = reason;
	session->state = 10;
}

// @retail 0x61950
void network_session_clear_peer(c_network_session *session, long peer_index)
{
	session->mask7424 &= ~(1 << peer_index);
	if (session->index742c == peer_index)
	{
		session->flag743c = false;
		session->flag7430 = false;
		session->index742c = NONE;
		session->time7428 = network_time_get();
	}
}
