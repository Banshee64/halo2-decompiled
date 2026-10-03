// @flags /O2 /Ob1 /Gr
/* NETWORK_SESSION_INTERFACE.CPP: the session interface globals (0x4cd868)
   and the queries on the current game session (g_527364) (lane D) */

#include "cseries.h"
#include <xtl.h>
#include <xonline.h>
#include <string.h>
#include <wchar.h>
#include "globals.h"
#include "network_session.h"
#include "online_tasks.h"
#include "network_configuration.h"

/* one local user's state (0xd0 bytes) */
#pragma pack(push, 1)
struct s_session_interface_user
{
	bool valid;
	XUID xuid;
	byte unknown0d[3];
	long unknown10;
	byte properties[0x90];
	long unknowna4;
	long unknowna8[3];
	long unknownb4[3];
	long unknownc0[3];
	byte unknowncc[4];
};
#pragma pack(pop)

struct s_session_interface_globals
{
	bool initialized;
	byte unknown01;
	wchar_t machine_name[16];
	wchar_t session_name[32];
	bool unknown62;
	byte unknown63;
	byte unknown64[32];
	long unknown84;
	long unknown88;
	long unknown8c;
	long unknown90;
	long unknown94;
	long unknown98;
	s_session_interface_user users[4];
	byte unknown3dc[0x3e8 - 0x3dc];
	long unknown3e8[3];
	byte unknown3f4[0x7d4 - 0x3f4];
	void *session_manager;
};

s_session_interface_globals g_4cd868;

#define SESSION_STATE_IS_LIVE(state) ((state) > 2 && (state) <= 8)

static inline c_network_session *network_session_get_live(void)
{
	c_network_session *result = 0;
	if (g_527330)
	{
		c_network_session *session = (c_network_session *)g_527364;
		long state = session->state;
		if (state && SESSION_STATE_IS_LIVE(state))
			result = session;
	}
	return result;
}

bool session_is_host(c_network_session *session)
{
	return session->current_member == session->value50;
}

/* compares two Xbox Live user ids, and their guest numbers if asked to */
// @retail 0x63d00
bool xuid_equal(XUID const *a, XUID const *b, bool compare_guest_number)
{
	bool result = false;

	if (a && b)
	{
		if (compare_guest_number)
			result = XOnlineAreUsersIdentical(a, b);
		else
			result = a->qwUserID == b->qwUserID;
	}
	return result;
}

// @retail 0x63d50
bool network_session_interface_initialize(void *session_manager)
{
	memset(&g_4cd868, 0, sizeof(g_4cd868));
	g_4cd868.session_manager = session_manager;
	g_4cd868.initialized = true;
	g_4cd868.unknown98 = NONE;
	return true;
}

// @retail 0x63dc0
void network_session_interface_clear_user_slot(long index)
{
	g_4cd868.unknown3e8[index] = 0;
	for (short i = 0; i < 4; i++)
	{
		g_4cd868.users[i].unknowna8[index] = 0;
		g_4cd868.users[i].unknownb4[index] = 0;
		g_4cd868.users[i].unknownc0[index] = 0;
		g_4cd868.users[i].unknowncc[index] = 0;
	}
}

// @retail 0x63f00
bool network_session_interface_local_machine_is_host(void)
{
	bool result = false;
	c_network_session *session = network_session_get_live();
	if (session && SESSION_STATE_IS_LIVE(session->state))
	{
		if (session_is_host(session) && session->type == 1)
			result = true;
	}
	return result;
}

// @retail 0x64160
long network_session_interface_get_value_49a4(void)
{
	long result = 0;
	c_network_session *session = network_session_get_live();
	if (session && SESSION_STATE_IS_LIVE(session->state))
		result = session->value49a4;
	return result;
}

// @retail 0x641f0
long network_session_interface_get_value_49c8(void)
{
	long result = NONE;
	c_network_session *session = network_session_get_live();
	if (session)
		result = session->get_value_49c8();
	return result;
}

// @retail 0x64260
long network_session_interface_get_value_498c(void)
{
	long result = 0;
	c_network_session *session = network_session_get_live();
	if (session && SESSION_STATE_IS_LIVE(session->state))
		result = session->value498c;
	return result;
}

// @retail 0x642d0
byte *network_session_interface_get_data_49a1(void)
{
	byte *result = 0;
	c_network_session *session = network_session_get_live();
	if (session && SESSION_STATE_IS_LIVE(session->state))
		result = session->data49a1;
	return result;
}

// @retail 0x645d0
void network_session_interface_set_local_name(const wchar_t *machine_name, const wchar_t *session_name)
{
	wcsncpy(g_4cd868.machine_name, machine_name, 15);
	g_4cd868.machine_name[15] = 0;
	wcsncpy(g_4cd868.session_name, session_name, 31);
	g_4cd868.session_name[31] = 0;
}

// @retail 0x64690
void network_session_interface_clear_user(long index)
{
	memset(&g_4cd868.users[index], 0, sizeof(s_session_interface_user));
}

/* how far a local user has got into the session: 0 none, 1 the session is
   not live, 2/3 no player yet, 4 no slot, 5 player out of date, 6 up to date */
// @retail 0x646b0
long network_session_interface_get_user_state(c_network_session *session, long user_index)
{
	if (g_4cd868.users[user_index].valid &&
		!(online_logon_connected() && g_4cd868.users[user_index].xuid.dwUserFlags == 0xbad00000))
	{
		if (SESSION_STATE_IS_LIVE(session->state))
		{
			long player_index = session->members[session->current_member].player_indices[user_index];
			if (player_index != NONE)
			{
				s_network_session_player *player = &session->players[player_index];
				if (player->unknown14 != NONE)
				{
					if (player->unknown14 == g_4cd868.users[user_index].unknown10 &&
						memcmp(player->properties18, g_4cd868.users[user_index].properties, sizeof(player->properties18)) == 0 &&
						player->unknown138 == g_4cd868.users[user_index].unknowna4)
						return 6;
					return 5;
				}
				return 4;
			}
			return g_4cd868.users[user_index].unknowncc[session->value10] ? 3 : 2;
		}
		return 1;
	}
	return 0;
}

// @retail 0x647a0
bool network_session_interface_get_user_xuid(long index, XUID *xuid)
{
	bool result = false;
	if (g_4cd868.users[index].valid)
	{
		*xuid = g_4cd868.users[index].xuid;
		result = true;
	}
	return result;
}

// @retail 0x647d0
bool network_session_interface_get_user_properties(long index, long *unknown10, byte *properties, long *unknowna4)
{
	bool result = false;
	if (g_4cd868.users[index].valid)
	{
		if (unknown10)
			*unknown10 = g_4cd868.users[index].unknown10;
		if (properties)
			memcpy(properties, g_4cd868.users[index].properties, sizeof(g_4cd868.users[index].properties));
		if (unknowna4)
			*unknowna4 = g_4cd868.users[index].unknowna4;
		result = true;
	}
	return result;
}

// @retail 0x64820
void network_session_interface_set_user_xuid(long index, const XUID *xuid)
{
	g_4cd868.users[index].xuid = *xuid;
}

// @retail 0x64840
void network_session_interface_set_user_properties(long index, long unknown10, const byte *properties, long unknowna4)
{
	g_4cd868.users[index].unknown10 = unknown10;
	memcpy(g_4cd868.users[index].properties, properties, sizeof(g_4cd868.users[index].properties));
	g_4cd868.users[index].unknowna4 = unknowna4;
}

long g_4cf968;
long g_4cf96c;

// @retail 0x64870
void network_session_interface_set_unknown64(const byte *data, long unknown84)
{
	g_4cd868.unknown62 = true;
	memcpy(g_4cd868.unknown64, data, sizeof(g_4cd868.unknown64));
	g_4cd868.unknown84 = unknown84;
	g_4cd868.unknown88 = g_4cf968;
	g_4cd868.unknown8c = g_4cf96c;
}

// @retail 0x648b0
bool network_session_interface_get_unknown64(byte *data, long *unknown84, long *unknown88, long *unknown8c)
{
	bool result = false;
	if (g_4cd868.unknown62)
	{
		memcpy(data, g_4cd868.unknown64, sizeof(g_4cd868.unknown64));
		*unknown84 = g_4cd868.unknown84;
		*unknown88 = g_4cd868.unknown88;
		*unknown8c = g_4cd868.unknown8c;
		result = true;
	}
	return result;
}

/* the session the current mode works on (unknown_01cf50.cpp) */
bool function_597d0(s_597d0_object **out);

static inline bool network_session_get(c_network_session **session)
{
	return function_597d0((s_597d0_object **)session);
}

// @retail 0x64970
bool network_session_interface_get_values_4d08(long *a, long *b, byte **c)
{
	bool result = false;
	c_network_session *session = 0;
	if (network_session_get(&session) && SESSION_STATE_IS_LIVE(session->state))
		result = session->get_values_4d08(a, b, c);
	return result;
}

byte *session_get_data_4db0(c_network_session *session)
{
	byte *result = 0;
	if (SESSION_STATE_IS_LIVE(session->state))
		result = session->data4db0;
	return result;
}

// @retail 0x649c0
byte *network_session_interface_get_data_4db0(void)
{
	byte *result = 0;
	c_network_session *session = 0;
	if (network_session_get(&session) && SESSION_STATE_IS_LIVE(session->state))
		result = session_get_data_4db0(session);
	return result;
}

short session_get_value_5dd0(c_network_session *session)
{
	short result = NONE;
	if (SESSION_STATE_IS_LIVE(session->state))
		result = session->value5dd0;
	return result;
}

// @retail 0x64a10
short network_session_interface_get_value_5dd0(void)
{
	c_network_session *session = 0;
	short result = NONE;
	if (network_session_get(&session) && SESSION_STATE_IS_LIVE(session->state))
		result = session_get_value_5dd0(session);
	return result;
}

// @retail 0x64a60
long network_session_interface_get_value_49ac(void)
{
	long result = NONE;
	c_network_session *session = network_session_get_live();
	if (session)
	{
		result = NONE;
		if (SESSION_STATE_IS_LIVE(session->state) && session->flag49a8)
			result = session->value49ac;
	}
	return result;
}

// @retail 0x64ab0
long network_session_interface_get_value_49b0(void)
{
	long result = 0;
	c_network_session *session = network_session_get_live();
	if (session)
	{
		result = 0;
		if (SESSION_STATE_IS_LIVE(session->state) && session->flag49a8)
			result = session->value49b0;
	}
	return result;
}

static inline byte *session_get_data_49b8(c_network_session *session)
{
	byte *result = 0;
	if (SESSION_STATE_IS_LIVE(session->state) && session->flag49a8 && session->value49b0 == 1)
		result = session->data49b8;
	return result;
}

// @retail 0x64b00
long network_session_interface_find_player_49b8(void)
{
	long result = NONE;
	c_network_session *session = network_session_get_live();
	if (session)
	{
		byte *key = session_get_data_49b8(session);
		if (key)
		{
			for (long i = 0; i < 16; i++)
			{
				if ((session->player_mask & (1 << i)) && !memcmp(&session->players[i], key, 12))
					return i;
			}
		}
	}
	return result;
}

bool session_get_id(c_network_session *session, s_session_id *id, byte *key)
{
	bool result = false;
	if (session->state && session->flag24)
	{
		if (id)
		{
			id->a = session->unknown1c;
			id->b = session->unknown20;
		}
		if (key)
			memcpy(key, session->data25, sizeof(session->data25));
		result = true;
	}
	return result;
}

// @retail 0x64bc0
bool network_session_interface_get_id(s_session_id *id, byte *key)
{
	bool result = false;
	c_network_session *session = 0;
	if (network_session_get(&session) && SESSION_STATE_IS_LIVE(session->state))
	{
		result = false;
		if (session->state && session->flag24)
		{
			if (id)
			{
				id->a = session->unknown1c;
				id->b = session->unknown20;
			}
			if (key)
				memcpy(key, session->data25, sizeof(session->data25));
			result = true;
		}
	}
	return result;
}

// @retail 0x64c40
long network_session_interface_get_value_18(void)
{
	long result = NONE;
	c_network_session *session = network_session_get_live();
	if (session)
		result = session->value18;
	return result;
}

static inline s_network_session_player *session_get_player(c_network_session *session, dword index)
{
	s_network_session_player *result = 0;
	if (session->player_mask & (1 << index))
		result = &session->players[index];
	return result;
}

// @retail 0x64c70
bool network_session_interface_has_user(const XUID *xuid)
{
	c_network_session *session = 0;
	if (network_session_get(&session) && SESSION_STATE_IS_LIVE(session->state))
	{
		for (dword i = 0; i < 16; i++)
		{
			s_network_session_player *player = session_get_player(session, i);
			if (player && xuid && player->user_id == xuid->qwUserID && XOnlineUserGuestNumber(player->user_flags) == XOnlineUserGuestNumber(xuid->dwUserFlags))
				return true;
		}
	}
	return false;
}

static inline s_long_pair *session_get_data_4999(c_network_session *session)
{
	s_long_pair *result = 0;
	if (session->flag4998)
		result = &session->data4999;
	return result;
}

// @retail 0x64cf0
s_long_pair *network_session_interface_get_data_4999(void)
{
	s_long_pair *result = 0;
	c_network_session *session = network_session_get_live();
	if (session)
		result = session_get_data_4999(session);
	return result;
}

/* a 12-byte key compared from its last dword to its first */
struct s_sort_key
{
	long low;
	long middle;
	long high;
};

// @retail 0x64d30
bool __stdcall sort_key_less_than(const s_sort_key *a, const s_sort_key *b, void *context)
{
	if (a->high < b->high)
		return true;
	if (a->high > b->high)
		return false;
	if (a->middle < b->middle)
		return true;
	if (a->middle > b->middle)
		return false;
	if (a->low < b->low)
		return false;
	return a->low > b->low;
}

// @retail 0x65280
long network_session_interface_get_value_90(long *value94)
{
	if (value94)
		*value94 = g_4cd868.unknown94;
	return g_4cd868.unknown90;
}

// @retail 0x652a0
long network_session_interface_get_members_status(long *progress)
{
	long status = 0;
	long lowest = 0;
	c_network_session *session = 0;
	if (network_session_get(&session) && SESSION_STATE_IS_LIVE(session->state))
	{
		long member_count = session->member_count;
		status = 4;
		lowest = 100;
		for (long i = 0; i < member_count; i++)
		{
			long member_status = session->members[i].unknown88;
			if (member_status == 1)
			{
				status = 1;
				lowest = 0;
				break;
			}
			if (member_status != 4)
			{
				if (member_status == 3)
				{
					if (status == 4)
						status = member_status;
				}
				else if (member_status == 2)
				{
					status = member_status;
					if (lowest > session->members[i].unknown8c)
						lowest = session->members[i].unknown8c;
				}
			}
		}
	}
	if (progress)
		*progress = lowest;
	return status;
}

static inline long session_get_value_498c(c_network_session *session)
{
	long result = 0;
	if (SESSION_STATE_IS_LIVE(session->state))
		result = session->value498c;
	return result;
}

static inline long session_get_maximum_player_count(c_network_session *session)
{
	long result = 16;
	if (SESSION_STATE_IS_LIVE(session->state))
		result = session->value4994;
	return result;
}

static inline short session_get_value_5dd0_inline(c_network_session *session)
{
	short result = NONE;
	if (SESSION_STATE_IS_LIVE(session->state))
		result = session->value5dd0;
	return result;
}

// @retail 0x63e00
bool network_session_interface_can_add_player(void)
{
	c_network_session *session = network_session_get_live();
	if (!session)
		return false;
	if (session_get_value_498c(session) != 0)
		return false;
	if (session->player_count >= session_get_maximum_player_count(session))
		return false;
	if (g_527334 == 6 || g_527334 == 7 || g_527334 == 8 || g_527334 == 9)
		return false;
	if (session_get_value_5dd0_inline(session) != NONE)
		return false;
	return true;
}

// @retail 0x656e0
bool network_session_get_membership(c_network_session *session, long *value4c, long *host_member_index, long *local_member_index, long *value50, long *member_count, s_session_member **members, long *player_count, dword *player_mask, s_network_session_player **players)
{
	bool result = false;
	if (SESSION_STATE_IS_LIVE(session->state))
	{
		long host = session->current_member;
		long local = session->member_index;
		if (value4c)
			*value4c = session->value4c;
		if (host_member_index)
			*host_member_index = host;
		if (local_member_index)
			*local_member_index = local;
		if (value50)
			*value50 = session->value50;
		if (member_count)
			*member_count = session->member_count;
		if (members)
			*members = session->members;
		if (player_count)
			*player_count = session->player_count;
		if (player_mask)
			*player_mask = session->player_mask;
		if (players)
			*players = session->players;
		result = true;
	}
	return result;
}


// @retail 0x64310
bool network_session_interface_kick_player(long player_index)
{
	bool result = false;
	c_network_session *session = network_session_get_live();
	if (session)
	{
		long host_member = session->current_member;
		if (host_member == session->value50 && (session->player_mask & (1 << player_index)))
		{
			long member_index = session->players[player_index].member_index;
			if (member_index != host_member && network_session_delegate_leader(session, (const s_session_member_identity *)session->members[member_index].words))
				result = true;
		}
	}
	return result;
}

// @retail 0x643f0
bool network_session_interface_ban_player(long player_index)
{
	bool result = false;
	c_network_session *session = network_session_get_live();
	if (session)
	{
		long host_member = session->current_member;
		if (host_member == session->value50 && (session->player_mask & (1 << player_index)))
		{
			long member_index = session->players[player_index].member_index;
			if (member_index != host_member && network_session_boot_machine(session, (const s_session_member_identity *)session->members[member_index].words))
				result = true;
		}
	}
	return result;
}


static inline c_network_session *network_session_get_current(void)
{
	c_network_session *result = 0;
	if (g_527330)
	{
		c_network_session *session = (c_network_session *)g_527364;
		if (session->state)
			result = session;
	}
	return result;
}

static inline bool session_is_established(c_network_session *session)
{
	if (SESSION_STATE_IS_LIVE(session->state))
	{
		return true;
	}
	return false;
}

static inline bool session_is_leader(c_network_session *session)
{
	bool result = false;
	if (SESSION_STATE_IS_LIVE(session->state))
	{
		result = session->current_member == session->value50;
	}
	return result;
}

// @retail 0x63f50
bool network_session_interface_set_value4d08(long value4d08, long value4d0c, const char *string)
{
	bool result = false;
	c_network_session *session = network_session_get_current();
	if (session && session_is_established(session))
	{
		if (session_is_leader(session))
			return network_session_parameters_set_value4d08(session, string, value4d08, value4d0c);
		result = true;
	}
	return result;
}

// @retail 0x63fb0
bool network_session_interface_set_value49a4(long value)
{
	bool result = false;
	if (network_session_interface_local_machine_is_host())
		result = network_session_parameters_set_value49a4(network_session_get_current(), value);
	return result;
}

// @retail 0x63ff0
bool network_session_interface_set_value4d08_and_stop_countdown(long value4d08, long value4d0c, const char *string)
{
	bool result = false;
	if (network_session_interface_local_machine_is_host())
	{
		c_network_session *session = network_session_get_current();
		if (network_session_parameters_set_value4d08(session, string, value4d08, value4d0c) && network_session_start_countdown(session, 0, false, 0, 0))
			return true;
		return false;
	}
	return result;
}

// @retail 0x64100
bool network_session_interface_set_value5dd0(short value)
{
	bool result = false;
	if (network_session_interface_local_machine_is_host())
	{
		c_network_session *session = network_session_get_current();
		if (network_session_parameters_set_value5dd0(session, value) && network_session_start_countdown(session, 0, false, 0, 0))
			return true;
		return false;
	}
	return result;
}

// @retail 0x64230
bool network_session_interface_set_value498c(long value)
{
	bool result = false;
	c_network_session *session = network_session_get_current();
	if (session && session_is_established(session))
		result = network_session_parameters_set_value498c(session, value);
	return result;
}

// @retail 0x642a0
bool network_session_interface_set_value49a1(const byte *value)
{
	bool result = false;
	c_network_session *session = network_session_get_current();
	if (session && session_is_established(session))
		result = network_session_parameters_set_value49a1(session, value);
	return result;
}

// @retail 0x643a0
bool network_session_interface_set_value49c4(void)
{
	bool result = false;
	if (g_527330 && g_527334 == 3)
	{
		c_network_session *session = (c_network_session *)g_527364;
		if (session->state && session_is_established(session) && session_is_leader(session) && network_session_parameters_set_value49c4(session))
			return true;
	}
	return result;
}

// @retail 0x64900
bool network_session_interface_start_countdown(long user_index, bool start, long countdown, long mode)
{
	bool result = false;
	c_network_session *session = network_session_get_current();
	if (session && session_is_established(session))
	{
		s_session_interface_user *user = &g_4cd868.users[user_index];
		if (user->valid)
		{
			if (network_session_start_countdown(session, countdown, start, mode, (const long *)&user->xuid))
				return true;
			return result;
		}
	}
	return result;
}

/* ---- the local users' players in a session (lane D, round 4) ---- */

bool network_session_player_add(c_network_session *session, const byte *properties, const dword *identity, long slot, long unknown18, long unknownac);
bool network_session_player_set_properties(c_network_session *session, const byte *properties, long slot, long unknown0c, long unknowna0);
bool network_session_player_remove(c_network_session *session, long slot);

static inline long session_interface_time_get(void)
{
	long time;
	if (g_510548)
		time = g_51054c;
	else
		time = GetTickCount();
	return time;
}

/* a reserved place of a local user (session +0x761c, 13 bytes each) */
#pragma pack(push, 1)
struct s_session_user_reservation
{
	bool valid;
	XUID xuid;
};
#pragma pack(pop)

// @retail 0x65a60
void network_session_interface_add_user(long user_index, c_network_session *session)
{
	s_session_interface_user *user = &g_4cd868.users[user_index];
	long owner = session->value10;
	long last = user->unknowna8[owner];
	long elapsed = session_interface_time_get() - last;
	if (SESSION_STATE_IS_LIVE(session->state) && (session->member_count > session->value4990 || session->player_count + 1 > session->value4994))
	{
		user->unknowncc[owner] = true;
	}
	else if (elapsed >= g_network_configuration.valuec84)
	{
		if (network_session_player_add(session, user->properties, (const dword *)&user->xuid, user_index, user->unknown10, user->unknowna4))
			user->unknowna8[owner] = session_interface_time_get();
		else
			user->unknowncc[owner] = true;
	}
	else
	{
		s_session_user_reservation *reservation = &((s_session_user_reservation *)session->data761c)[user_index];
		if (reservation->valid && !memcmp(&user->xuid, &reservation->xuid, sizeof(XUID)))
			user->unknowncc[owner] = true;
	}
}

// @retail 0x65b80
void network_session_interface_remove_user(c_network_session *session, long user_index, long slot)
{
	s_session_interface_user *user = &g_4cd868.users[user_index];
	long owner = session->value10;
	if (slot != NONE)
	{
		long last = user->unknownc0[owner];
		if (session_interface_time_get() - last >= g_network_configuration.valuec88 && network_session_player_remove(session, user_index))
			user->unknownc0[owner] = session_interface_time_get();
	}
}

// @retail 0x65c10
void network_session_interface_update_user(long user_index, c_network_session *session)
{
	s_session_interface_user *user = &g_4cd868.users[user_index];
	long owner = session->value10;
	long last = user->unknownb4[owner];
	if (session_interface_time_get() - last >= g_network_configuration.valuec8c && network_session_player_set_properties(session, user->properties, user_index, user->unknown10, user->unknowna4))
		user->unknownb4[owner] = session_interface_time_get();
}