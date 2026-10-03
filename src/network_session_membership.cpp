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

static inline long network_time_since(long time)
{
	return network_time_get() - time;
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

// @retail 0x62b40
void network_session_reset_7620(c_network_session *session)
{
	session->update7650++;
	memset(session->data761c, 0, sizeof(session->data761c));
	session->value7654 = NONE;
	session->value7658 = NONE;
}

/* the first 0x24 bytes of a member record: its machine's address at +0xa */
struct s_session_member_header
{
	byte unknown00[0xa];
	s_session_machine address;
	byte unknown10[0x24 - 0x10];
};

/* the machines a member can reach (0xc0 bytes) */
struct s_session_peer_map
{
	s_session_member_header local_member;
	s_session_member_header host_member;
	long unknown48;
	long host_member_index;
	long unknown50;
	long machine_count;
	s_session_machine machines[16];
	dword reachable_mask;
	dword connected_mask;
};

static inline s_session_member_header *session_get_member_header(c_network_session *session, long member_index)
{
	return (s_session_member_header *)session->members[member_index].words;
}

// @retail 0x62b70
long network_session_build_peer_map(c_network_session *session, s_session_peer_map *map)
{
	memset(map, 0, sizeof(s_session_peer_map));
	map->local_member = *(s_session_member_header *)session->members[session->member_index].words;
	map->host_member = *(s_session_member_header *)session->members[session->current_member].words;
	map->unknown48 = session->value4c;
	map->host_member_index = session->current_member;
	map->unknown50 = session->members[session->current_member].unknown94;
	map->machines[0] = session_get_member_header(session, session->current_member)->address;
	map->machine_count = 1;
	map->reachable_mask = 1;
	map->connected_mask = 1;
	return session->current_member;
}

// @retail 0x62de0
void network_session_expire_reservations(c_network_session *session)
{
	s_network_session_reservation *reservations = session->reservations;
	for (s_network_session_reservation *reservation = reservations; reservation < reservations + 16; reservation++)
	{
		if (reservation->active && reservation->timeout != NONE)
		{
			if ((dword)network_time_since(reservation->time) > (dword)reservation->timeout)
				reservation->active = false;
		}
	}
}

// @retail 0x62e30
long network_session_get_open_slot_count(c_network_session *session)
{
	long pending = 0;
	s_network_session_reservation *reservations = session->reservations;
	for (s_network_session_reservation *reservation = reservations; reservation < reservations + 16; reservation++)
	{
		if (reservation->active && !reservation->joined)
			pending++;
	}
	return session->value4994 - session->player_count - pending;
}

// @retail 0x62fa0
long network_session_cancel_reservations(c_network_session *session, const s_session_id *id)
{
	long count = 0;
	s_network_session_reservation *reservations = session->reservations;
	for (s_network_session_reservation *reservation = reservations; reservation < reservations + 16; reservation++)
	{
		if (reservation->active && !memcmp(id, reservation->id, sizeof(s_session_id)))
		{
			reservation->active = false;
			count++;
		}
	}
	return count;
}

// @retail 0x63ba0
long session_peer_map_find_machine(s_session_peer_map *map, const s_session_member_header *member)
{
	long result = NONE;
	s_session_machine address = member->address;
	for (long i = 0; i < map->machine_count; i++)
	{
		if (!memcmp(&address, &map->machines[i], sizeof(s_session_machine)))
			result = i;
	}
	return result;
}

static inline long session_peer_map_add_machine(s_session_peer_map *map, const s_session_member_header *member)
{
	long index = NONE;
	if ((dword)map->machine_count < 16)
	{
		index = map->machine_count++;
		map->machines[index] = member->address;
	}
	return index;
}

// @retail 0x63c00
long session_peer_map_add_reachable(s_session_peer_map *map, const s_session_member_header *member)
{
	long index = session_peer_map_find_machine(map, member);
	if (index == NONE)
		index = session_peer_map_add_machine(map, member);
	if (index != NONE)
		map->reachable_mask |= (dword)1 << index;
	return index;
}

// @retail 0x63c50
long session_peer_map_add_connected(s_session_peer_map *map, const s_session_member_header *member)
{
	long index = session_peer_map_find_machine(map, member);
	if (index == NONE)
		index = session_peer_map_add_machine(map, member);
	if (index != NONE)
		map->connected_mask |= (dword)1 << index;
	return index;
}

// @retail 0x63ca0
long count_bits(dword value)
{
	value = ((value >> 1) & 0x55555555) + (value & 0x55555555);
	value = ((value >> 2) & 0x33333333) + (value & 0x33333333);
	value = ((value >> 4) & 0x0f0f0f0f) + (value & 0x0f0f0f0f);
	value = ((value >> 8) & 0x00ff00ff) + (value & 0x00ff00ff);
	return (value >> 16) + (value & 0xffff);
}

/* a summary of a session's machines and players (unknown layout past +0x308) */
struct s_session_summary
{
	long machine_count;
	s_session_id machine_ids[16];
	XUID machine_users[16];
	long machine_times[16];
	long player_count;
	XUID player_users[16];
	long player_values248[16];
	long player_values288[16];
	long player_machines[16];
};

// @retail 0x63190
long __stdcall function_063190(void *p, long a)
{
	s_session_summary *summary = (s_session_summary *)p;
	const XUID *user = (const XUID *)a;
	long result = NONE;
	for (long i = 0; i < summary->player_count && result == NONE; i++)
	{
		if (!memcmp(&summary->player_users[i], user, sizeof(XUID)))
			result = i;
	}
	return result;
}

// @retail 0x631f0
long session_summary_get_machine_highest_value(s_session_summary *summary, long machine_index)
{
	long result = NONE;
	for (long i = 0; i < summary->player_count; i++)
	{
		if (summary->player_machines[i] == machine_index && result <= summary->player_values248[i])
			result = summary->player_values248[i];
	}
	return result;
}

// @retail 0x63280
long session_summary_find_machine_user(s_session_summary *summary, const XUID *user)
{
	long result = NONE;
	for (long i = 0; i < summary->machine_count && result == NONE; i++)
	{
		if (!memcmp(&summary->machine_users[i], user, sizeof(XUID)))
			result = i;
	}
	return result;
}

// @retail 0x632e0
long __stdcall function_0632e0(void *p, void *q)
{
	s_session_summary *summary = (s_session_summary *)p;
	const s_session_id *id = (const s_session_id *)q;
	long result = NONE;
	for (long i = 0; i < summary->machine_count && result == NONE; i++)
	{
		if (!memcmp(&summary->machine_ids[i], id, sizeof(s_session_id)))
			result = i;
	}
	return result;
}

// @retail 0x63340
bool session_summary_valid(const s_session_summary *summary)
{
	long machine_count = summary->machine_count;
	bool valid = machine_count >= 0 && machine_count <= 16 && summary->player_count >= 0 && summary->player_count <= 16;
	for (long i = 0; i < summary->player_count && valid; i++)
	{
		valid = summary->player_machines[i] >= 0 && summary->player_machines[i] < machine_count &&
			(summary->player_values248[i] == NONE || summary->player_values248[i] >= 0 && summary->player_values248[i] <= 0x7f) &&
			(summary->player_values288[i] == NONE || summary->player_values288[i] >= 0 && summary->player_values288[i] <= 0x3fffffff);
	}
	return valid;
}

// @retail 0x63510
bool function_063510(void *a, void *p, long x)
{
	XUID *users = (XUID *)a;
	bool found = false;
	for (long i = 0; i < x && !found; i++)
	{
		if (function_063190(p, (long)&users[i]) != NONE)
			found = true;
	}
	return found;
}

// @retail 0x601e0
void network_session_remove_player_and_update(c_network_session *session, long player_index)
{
	network_session_remove_player(session, player_index);
	session->value4c++;
	session->update7618++;
}

// @retail 0x61390
void network_session_enter_state_5(c_network_session *session)
{
	long state = session->state;
	if (!SESSION_STATE_IS_HOSTING(state))
	{
		bool any = false;
		network_session_reset_membership(session, true);
		for (long i = 0; i < session->member_count; i++)
		{
			bool flag = session->member_states[i].flag1;
			session->member_states[i].flag2 = flag;
			if (flag)
				any = true;
		}
		if (any)
		{
			session->value7660 = session->type;
			session->flag765c = true;
			session->time7664 = network_time_get();
		}
		session->member_index = session->current_member;
	}
	memset(&session->value7420, 0, 0x1f8);
	session->state = 5;
}

/* the data of state 9 (0x118 bytes at +0x7420) */
struct s_session_state_9_data
{
	long time;
	long unknown04;
	long time2;
	long host_member_index;
	s_session_peer_map map;
	dword host_mask;
	dword local_mask;
	__int64 unknownd8;
	byte unknowne0[0x118 - 0xe0];
};

// @retail 0x616e0
void network_session_enter_state_9(c_network_session *session)
{
	s_session_state_9_data data;
	memset(&data, 0, sizeof(data));
	data.time = network_time_get();
	data.host_member_index = network_session_build_peer_map(session, &data.map);
	data.time2 = network_time_get();
	data.host_mask = 1 << session->current_member;
	data.local_mask = 1 << session->member_index;
	network_session_reset_7620(session);
	memset(&session->value7420, 0, 0x1f8);
	memcpy(&session->value7420, &data, sizeof(data));
	session->state = 9;
}

// @retail 0x633c0
bool session_summary_remove_machine(s_session_summary *summary, s_session_id *id)
{
	long machine_index = function_0632e0(summary, id);
	if (machine_index != NONE)
	{
	s_session_summary old = *summary;
	memset(summary, 0, sizeof(s_session_summary));
	summary->machine_count = 0;
	for (long i = 0; i < old.machine_count; i++)
	{
		if (i != machine_index)
		{
			summary->machine_ids[summary->machine_count] = old.machine_ids[i];
			summary->machine_times[summary->machine_count] = old.machine_times[i];
			summary->machine_count++;
		}
	}
	summary->player_count = 0;
	for (long j = 0; j < old.player_count; j++)
	{
		long machine = old.player_machines[j];
		if (machine != machine_index)
		{
			long new_machine = machine > machine_index ? machine - 1 : machine;
			summary->player_users[summary->player_count] = old.player_users[j];
			summary->player_values248[summary->player_count] = old.player_values248[j];
			summary->player_values288[summary->player_count] = old.player_values288[j];
			summary->player_machines[summary->player_count] = new_machine;
			summary->player_count++;
		}
	}
	return true;
	}
	return false;
}

// @retail 0x63550
bool session_summary_add_machine(s_session_summary *summary, s_session_id *id, const XUID *machine_user, const long *values288, long player_count, XUID *players, const long *values248)
{
	if (summary->player_count + player_count > 16 || summary->machine_count >= 16)
		return false;
	if (function_0632e0(summary, id) != NONE || function_063510(players, summary, player_count))
		return false;

	for (long i = 0; i < player_count; i++)
	{
		summary->player_users[summary->player_count] = players[i];
		summary->player_values248[summary->player_count] = values248[i];
		summary->player_values288[summary->player_count] = values288[i];
		summary->player_machines[summary->player_count] = summary->machine_count;
		summary->player_count++;
	}
	summary->machine_ids[summary->machine_count] = *id;
	summary->machine_users[summary->machine_count] = *machine_user;
	summary->machine_times[summary->machine_count] = network_time_get();
	summary->machine_count++;
	return true;
}

static inline bool session_peer_map_is_reachable(s_session_peer_map *map, const s_session_member_header *member)
{
	bool result = false;
	long index = session_peer_map_find_machine(map, member);
	if (index != NONE)
		result = TEST_FIELD_BIT(map->reachable_mask & (1 << index));
	return result;
}

// @retail 0x62c20
bool session_peer_map_update_reachable(s_session_member_header *a, s_session_member_header *b, s_session_peer_map *map)
{
	s_session_member_header *other;
	if (!memcmp(&map->host_member, a, sizeof(s_session_member_header)))
		other = b;
	else if (!memcmp(b, &map->host_member, sizeof(s_session_member_header)))
		other = a;
	else
		return false;

	if (!other)
		return false;
	if (session_peer_map_is_reachable(map, other))
		return false;
	if (session_peer_map_add_reachable(map, other) == NONE)
		return false;
	return true;
}

static inline bool session_peer_map_is_connected(s_session_peer_map *map, const s_session_member_header *member)
{
	bool result = false;
	long index = session_peer_map_find_machine(map, member);
	if (index != NONE)
		result = TEST_FIELD_BIT(map->connected_mask & (1 << index));
	return result;
}

// @retail 0x62ca0
bool session_peer_map_set_connected(s_session_peer_map *map, const s_session_member_header *member, bool connected)
{
	bool is_connected = session_peer_map_is_connected(map, member);
	long index;
	if (connected)
	{
		if (is_connected)
			return false;
		session_peer_map_add_connected(map, member);
		return true;
	}
	if (is_connected)
	{
		index = session_peer_map_find_machine(map, member);
		if (index != NONE)
			map->connected_mask &= ~(1 << index);
		return true;
	}
	return false;
}

/* a session's parameters, and the update that carries the ones that changed */
struct s_session_parameters
{
	wchar_t name[16];
	wchar_t description[32];
	long unknown60;
	long unknown64;
	long unknown68;
	long unknown6c;
	long unknown70;
	byte unknown74[16];
	byte unknown84[0x40];
	long unknownc4;
};

struct s_session_parameters_update
{
	bool name_changed;
	byte unknown01;
	wchar_t name[16];
	wchar_t description[32];
	bool unknown60_changed;
	byte unknown63;
	long unknown60;
	long unknown64;
	bool unknown68_changed;
	byte unknown6d[3];
	long unknown68;
	long unknown6c;
	long unknown70;
	byte unknown74[16];
	bool unknown84_changed;
	byte unknown8d[3];
	byte unknown84[0x40];
	bool unknownc4_changed;
	byte unknownd1[3];
	long unknownc4;
};

static inline void ustrnzcpy(wchar_t *dest, const wchar_t *source, long count)
{
	wcsncpy(dest, source, count - 1);
	dest[count - 1] = 0;
}

// @retail 0x602b0
void session_parameters_build_update(s_session_parameters_update *update, const s_session_parameters *parameters, const s_session_parameters *old_parameters)
{
	if (!old_parameters || wcsncmp(parameters->name, old_parameters->name, 16) || wcsncmp(parameters->description, old_parameters->description, 32))
	{
		update->name_changed = true;
		ustrnzcpy(update->name, parameters->name, 16);
		ustrnzcpy(update->description, parameters->description, 32);
	}
	if (!old_parameters || parameters->unknown60 != old_parameters->unknown60 || parameters->unknown64 != old_parameters->unknown64)
	{
		update->unknown60_changed = true;
		update->unknown60 = parameters->unknown60;
		update->unknown64 = parameters->unknown64;
	}
	if (!old_parameters || parameters->unknown68 != old_parameters->unknown68 || parameters->unknown6c != old_parameters->unknown6c || parameters->unknown70 != old_parameters->unknown70 || memcmp(parameters->unknown74, old_parameters->unknown74, sizeof(parameters->unknown74)))
	{
		update->unknown68_changed = true;
		update->unknown68 = parameters->unknown68;
		update->unknown6c = parameters->unknown6c;
		update->unknown70 = parameters->unknown70;
		memcpy(update->unknown74, parameters->unknown74, sizeof(update->unknown74));
	}
	if (!old_parameters || memcmp(parameters->unknown84, old_parameters->unknown84, sizeof(parameters->unknown84)))
	{
		update->unknown84_changed = true;
		memcpy(update->unknown84, parameters->unknown84, sizeof(update->unknown84));
	}
	if (!old_parameters || parameters->unknownc4 != old_parameters->unknownc4)
	{
		update->unknownc4_changed = true;
		update->unknownc4 = parameters->unknownc4;
	}
}
