// @flags /O2 /Ob1 /Gr
/* NETWORK_SESSION_CLIENT.CPP: the session client's request queue, the
   session owner and the joining state's helpers (lane D) */

#include "unknown_11c920.h"
#include <xtl.h>
#include <xonline.h>
#include <string.h>
#include "globals.h"
#include "unknown_058dd0.h"
#include "unknown_058ee0.h"
#include "unknown_0662e0.h"
#include "online_tasks.h"
#include "unknown_234c64.h"
#include "unknown_19b510.h"

#define SESSION_STATE_IS_LIVE(state) ((state) > 2 && (state) <= 8)

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
long __stdcall function_063190(void *summary, long user);
long session_summary_get_player_value(s_session_summary *summary, long player_index, long variant_index);
long network_session_find_member(c_class_58d20 *session, const s_session_member_identity *identity);
void network_session_disband_member(c_class_58d20 *session, long member_index);
long network_session_cancel_reservations(c_class_58d20 *session, const s_session_id *id);
bool session_summary_remove_machine(s_session_summary *summary, s_session_id *id);
bool network_session_host_set_summary(c_class_58d20 *session, const s_session_summary *summary);

// @retail 0x6d380
void __stdcall function_06d380(c_session_client *client, const s_session_id *id)
{
	{
		c_class_58d20 *session = client->session;
		long current = session->current_member;
		long count = session->member_count;
		long found = 0;
		s_session_member_identity members[16];
		for (long i = 0; i < count; i++)
		{
			if (i != current && !memcmp(&session->members[i].id, id, sizeof(*id)))
			{
				memcpy(&members[found], session->members[i].words, sizeof(members[found]));
				found++;
			}
		}
		for (long j = 0; j < found; j++)
		{
			session = client->session;
			long index = network_session_find_member(session, &members[j]);
			if (index != NONE && index != session->current_member &&
				session->state != 7 && session->state != 6 && session->state != 8)
				network_session_disband_member(session, index);
		}
	}
	network_session_cancel_reservations(client->session, id);
	c_class_58d20 *session = client->session;
	s_session_summary *source = 0;
	if (session_state_is_live(session) && session->flag49fd)
		source = (s_session_summary *)session->data4a00;
	if (source)
	{
		s_session_summary summary = *source;
		if (session_summary_remove_machine(&summary, (s_session_id *)id))
			network_session_host_set_summary(client->session, &summary);
	}
}

// @retail 0x6e910
void function_6e910(const s_network_session_player *player, const s_session_machine *machines,
	long variant_index, const byte *variant, s_session_summary *summary, char default_team, s_session_player *output)
{
	output->active = true;
	output->flag1 = false;
	output->machine = machines[player->member_index];
	output->index = (short)player->slot;
	output->controller = player->unknown14;
	memcpy(&output->id, player, sizeof(output->id));
	memcpy(output->name, player->propertiesa8, sizeof(player->propertiesa8));
	if (variant_index != NONE)
	{
		long index = function_063190(summary, (long)player);
		if (index != NONE)
		{
			*(long *)((byte *)output + 0xa0) = variant_index;
			*(long *)((byte *)output + 0xa8) = *(long *)((byte *)summary + 0x288 + index * 4);
			*(short *)((byte *)output + 0xa6) = *(short *)((byte *)summary + 0x248 + index * 4);
			*(short *)((byte *)output + 0xa4) = (short)session_summary_get_player_value(summary, index, variant_index);
		}
	}
	if (variant && output->flag98 != 0xff)
	{
		if (variant[0x48] & 1)
		{
			long team;
			if ((char)output->flag98 < 0)
				team = 0;
			else if ((char)output->flag98 > 7)
				team = 7;
			else
				team = (char)output->flag98;
			output->flag98 = (byte)team;
		}
		else
			output->flag98 = default_team;
	}
}

// @retail 0x6dd00
bool c_session_client::function_06dd00(long a, s_session_remote *remote)
{
	bool result = false;
	if (request_count < 30)
	{
		s_allocator_globals *globals = g_4d87f8;
		s_session_request *request = (s_session_request *)globals->allocator->allocate(sizeof(s_session_request), 0, 0);
		if (!request)
		{
			globals->allocator->compact(0);
			request = (s_session_request *)globals->allocator->allocate(sizeof(s_session_request), 0, 0);
		}
		if (request)
			globals->count++;
		if (request)
		{
			request->remote = *remote;
			memcpy(request->key04, (const void *)a, sizeof(request->key04));
			request->next = requests;
			requests = request;
			request_count++;
			result = true;
		}
	}
	return result;
}

// @retail 0x6ddb0
void session_client_remove_request(c_session_client *client, s_session_request *request)
{
	s_session_request **link = &client->requests;
	while (*link && *link != request)
		link = &(*link)->next;
	if (*link == request)
	{
		*link = (*link)->next;
		long info;
		g_4d87f8->allocator->get_info(request, &info);
		s_allocator_globals *globals = g_4d87f8;
		globals->allocator->release(request, NONE);
		if (request)
			globals->count--;
		client->request_count--;
	}
}

// @retail 0x6de10
bool c_session_client::function_06de10(s_session_remote *remote)
{
	s_session_request *request = requests;
	bool found = false;
	for (; request; request = request->next)
	{
		if (found)
			break;
		if (!memcmp(request->remote.key188, remote->key188, sizeof(remote->key188)))
			found = true;
	}
	return found;
}

// @retail 0x6de50
void session_owner_initialize(s_session_owner *owner_, long unknown40, long unknown44, void *unknown2c, c_class_58d20 *session_a, c_class_58d20 *session_c, c_class_58d20 *session_b, void *unknown3c)
{
	s_session_owner_view *owner = (s_session_owner_view *)owner_;
	memset(owner->states, 0, sizeof(owner->states));
	owner->unknown40 = unknown40;
	owner->unknown44 = unknown44;
	owner->unknown2c = unknown2c;
	owner->session_a = session_a;
	owner->session_c = session_c;
	owner->session_b = session_b;
	owner->unknown3c = unknown3c;
	owner->mode = 0;
	owner->unknown49 = false;
	owner->unknown48 = false;
	owner->failed = false;
}

// @retail 0x6df60
inline void function_06df60(s_session_owner *o, long a, long b, long c)
{
	o->failed = true;
	o->error_code = a;
	o->data_size = b;
	long *data = o->data;
	*data = 0;
	if (o->data_size > 0)
		memcpy(data, (const void *)c, o->data_size);
}

// @retail 0x6e0f0
bool network_session_members_ready(c_class_58d20 *session, dword *unready_mask)
{
	dword mask = 0;
	bool ready = true;
	for (long i = 0; i < session->member_count; i++)
	{
		if (session->members[i].unknown88 <= 2)
		{
			mask |= 1 << i;
			ready = false;
		}
	}
	if (unready_mask)
		*unready_mask = mask;
	return ready;
}

// @retail 0x6f090
void session_state_joining_initialize(c_session_state_joining *state_, s_session_owner *owner)
{
	s_session_state_joining_view *state = (s_session_state_joining_view *)state_;
	state->owner = (s_session_owner_view *)owner;
	state->index = 5;
	state->unknown0d = true;
	state->skip_cleanup = false;
	state->owner->states[5] = (c_session_state *)state_;
	state->unknown104 = 16;
	state->unknownf0 = NONE;
	state->unknownf4 = NONE;
	state->unknown68 = false;
	state->unknown10 = false;
	state->unknown11 = false;
	state->unknownf8 = false;
	state->unknowne4 = 0;
	state->unknowne8 = false;
	state->unknownf9 = false;
	state->unknowne9 = false;
	state->unknownec = 0;
}

// @retail 0x6fc80
void session_state_joining_check_target(c_session_state_joining *state_)
{
	s_session_state_joining_view *state = (s_session_state_joining_view *)state_;
	long session_state = state->owner->session_c->state;
	if (session_state != 1)
	{
		if (session_state != 0)
		{
			if (SESSION_STATE_IS_LIVE(session_state))
				state->unknownf8 = true;
		}
		else
		{
			state->unknown104 = 16;
		}
	}
}

void function_6b640(long task_index);
void qos_release(long handle);

/* clears the joining state's progress */
static inline void session_state_joining_reset(s_session_state_joining_view *state)
{
	state->unknown68 = false;
	state->unknown10 = false;
	state->unknown11 = false;
	state->unknownf8 = false;
	state->unknowne4 = 0;
	state->unknowne8 = false;
	state->unknownf9 = false;
	state->unknowne9 = false;
	state->unknownec = 0;
}

// @retail 0x6f0f0
void c_session_state_joining::function_06f0f0()
{
	s_session_state_joining_view *state = (s_session_state_joining_view *)this;
	c_class_58d20 *session = state->owner->session_c;
	if (!state->unknown104)
		state->unknown104 = 16;
	if (state->unknownf0 != NONE)
	{
		function_6b640(state->unknownf0);
		state->unknownf0 = NONE;
	}
	if (state->unknownf4 != NONE)
	{
		qos_release(state->unknownf4);
		state->unknownf4 = NONE;
	}
	if (session->state && !function_058d90(session))
		network_session_leave(session, false);
	session_state_joining_reset(state);
}

/* a random value in [lower, upper) from the second seed */
static inline short session_random_range(short lower, short upper)
{
	dword *seed = &g_4e7408->seed;
	*seed = *seed * 0x19660d + 0x3c6ef35f;
	return lower + (short)(((upper - lower) * (*seed >> 16)) >> 16);
}

// @retail 0x70190
void session_state_matchmaking_initialize(c_session_state_matchmaking *state_, s_session_owner *owner)
{
	s_session_state_matchmaking_view *state = (s_session_state_matchmaking_view *)state_;
	session_state_initialize((s_session_state_view *)state, (s_session_owner_view *)owner, 6, true, false);
	state->unknown97c = false;
	state->unknowna08 = 0;
	state->unknowna0c = 0;
	state->unknowna1c = 0;
	state->unknown9ec = NONE;
	state->unknown9f0 = NONE;
	state->unknown9f4 = NONE;
	state->mode = 1;
	state->unknown96c = session_random_range(0, (short)g_network_configuration.value19c);
	state->unknown974 = session_random_range(0, (short)g_network_configuration.value194);
	state->unknowna64 = false;
	state->unknowna80 = 0;
	state->unknowna84 = 0;
	state->unknowna88 = 0;
	state->unknowna78 = false;
	state->unknowna8c = 0;
	state->unknowna90 = 0;
	state->unknowna94 = 0;
}

// @retail 0x6f1a0
void c_session_state_joining::function_06f1a0()
{
	s_session_state_joining_view *state = (s_session_state_joining_view *)this;
	s_session_owner_view *owner = state->owner;
	long mode = owner->mode;
	c_class_58d20 *session = owner->session_a;
	if (mode != 1 && mode != 0 && mode != 4 && mode != 9)
		state->unknown104 = 15;
	if (!state->unknown104)
	{
		if (session->state && !function_058d50(session))
			state->unknown104 = 14;
	}
}

// @retail 0x6f200
void c_session_state_joining::function_06f200(bool flag, const void *target, long count, const void *entries)
{
	s_session_state_joining_view *state = (s_session_state_joining_view *)this;
	state->unknown104 = 0;
	state->unknown11 = flag;
	function_06f1a0();
	if (!state->unknown104)
	{
		s_session_owner *owner = (s_session_owner *)state->owner;
		memcpy(state->target, target, sizeof(state->target));
		state->entry_count = count;
		memset(state->entries, 0, sizeof(state->entries));
		memcpy(state->entries, entries, count * 12);
		state->unknown10 = true;
		function_06df60(owner, 5, 0, 0);
	}
}

// @retail 0x6f2b0
void c_session_state_joining::function_06f2b0(const s_session_description *description, long count)
{
	s_session_state_joining_view *state = (s_session_state_joining_view *)this;
	long minimum_version = description->unknown0c;
	long version = description->unknown08;
	if (description->unknown04 == 4 && version >= 0x2651 && minimum_version <= 0x2651)
	{
		if (description->unknown10 == (online_logon_connected() ? 2 : 1))
		{
			short kind = description->unknown14;
			if (kind == 0)
			{
				if (count > description->unknown94)
					state->unknown104 = 7;
			}
			else if (kind == 1 && state->unknown11)
			{
				if (count > description->unknown96)
					state->unknown104 = 7;
			}
			else
			{
				state->unknown104 = 8;
			}
		}
		else
		{
			state->unknown104 = 10;
		}
	}
	else
	{
		state->unknown104 = 9;
	}
	short status = description->unknown9e;
	if (status == 7 || status == 8 || status == 6)
		state->unknown104 = 12;
	if (description->unknown9e == 5)
		state->unknown104 = 13;
}

// @retail 0x6f3a0
void c_session_state_joining::function_06f3a0(const s_session_description *description, long count, const void *entries)
{
	s_session_state_joining_view *state = (s_session_state_joining_view *)this;
	state->unknown104 = 0;
	function_06f1a0();
	if (!state->unknown104)
	{
		function_06f2b0(description, count);
		if (!state->unknown104)
		{
			s_session_owner *owner = (s_session_owner *)state->owner;
			state->entry_count = count;
			memset(state->entries, 0, sizeof(state->entries));
			memcpy(state->entries, entries, count * 12);
			state->part.unknown00 = description->unknown02;
			*(XNKID *)state->part.unknown04 = description->kid;
			*(XNKEY *)state->part.unknown0c = description->key;
			*(XNADDR *)state->part.unknown1c = description->address;
			state->part.unknown40 = description->unknown10;
			state->unknown68 = true;
			function_06df60(owner, 5, 0, 0);
		}
	}
}

static inline long session_time_get(void)
{
	long time;
	if (g_510548)
		time = g_51054c;
	else
		time = GetTickCount();
	return time;
}

void network_session_set_mode(c_class_58d20 *session, long mode);
bool network_session_parameters_set_mode(c_class_58d20 *session, long mode);
bool network_session_parameters_set_data5ddc(c_class_58d20 *session, const s_parameters_part *data);
bool network_session_id_differs(c_class_58d20 *session, const s_parameters_part *part);
long function_75890(long time);

// @retail 0x6f9d0
void session_state_joining_request_host_mode(c_session_state_joining *state_)
{
	s_session_state_joining_view *state = (s_session_state_joining_view *)state_;
	c_class_58d20 *session = state->owner->session_a;
	c_class_58d20 *target = state->owner->session_c;
	state->unknowne8 = true;
	if (session->function_058d20())
	{
		long target_state = target->state;
		if (!target_state)
		{
			state->unknown104 = 16;
		}
		else if (target_state > 2 && target_state <= 8)
		{
			network_session_set_mode(session, 17);
			state->unknownfc = session_time_get();
		}
	}
}

// @retail 0x6fbe0
void session_state_joining_check_ready(c_session_state_joining *state_)
{
	s_session_state_joining_view *state = (s_session_state_joining_view *)state_;
	c_class_58d20 *session = state->owner->session_a;
	c_class_58d20 *target = state->owner->session_c;
	state->unknowne8 = true;
	if (session_state_is_live(target))
	{
		if (session->function_058d20())
		{
			long start = state->unknown100;
			long now = session_time_get();
			if (session->member_count == 1 || now - start > g_network_configuration.value17c)
				state->unknownf8 = true;
		}
		else
		{
			state->unknownf8 = true;
		}
	}
	else
	{
		state->unknown104 = 16;
	}
}

// @retail 0x6f800
void __stdcall session_state_joining_set_mode(c_session_state_joining *state_)
{
	s_session_state_joining_view *state = (s_session_state_joining_view *)state_;
	c_class_58d20 *session = state->owner->session_a;
	if (session->type != 1)
	{
		long last = state->unknownec;
		if (!last || session_time_get() - last > g_network_configuration.value188)
		{
			network_session_parameters_set_mode(session, 1);
			state->unknownec = session_time_get();
		}
	}
}

// @retail 0x6f880
void __stdcall session_state_joining_send_target(c_session_state_joining *state_)
{
	s_session_state_joining_view *state = (s_session_state_joining_view *)state_;
	c_class_58d20 *session = state->owner->session_a;
	if (state->unknown68 && network_session_id_differs(session, &state->part))
	{
		if (session->type != 15)
		{
			long last = state->unknowne4;
			if (!last || function_75890(last) > g_network_configuration.value184)
			{
				network_session_parameters_set_data5ddc(session, &state->part);
				network_session_parameters_set_mode(session, 15);
				state->unknowne4 = session_time_get();
			}
		}
	}
	else
	{
		state->unknown104 = 16;
	}
}

// @retail 0x6e720
bool function_06e720(c_class_58d20 *session)
{
	bool result = false;
	if (session->state == 5)
	{
		if (!session->flag4f20 || !&session->data4f24 || !&session->data4f90)
		{
			s_session_summary *summary = session->flag49fd ? (s_session_summary *)session->data4a00 : NULL;
			s_unknown_108 machines;
			__declspec(align(8)) s_session_player players[16];
			memset(&machines, 0, sizeof(machines));
			memset(players, 0, sizeof(players));
			machines.data[0] = (1 << session->member_count) - 1;
			s_session_machine *addresses = (s_session_machine *)&machines.data[1];
			for (long i = 0; i < session->member_count; i++)
				addresses[i] = *(s_session_machine *)((byte *)session->members[i].words + 0xa);
			for (long i = 0; i < 16; i++)
			{
				if ((session->player_mask & (1 << i)) && session->players[i].unknown14 != NONE)
					function_6e910(&session->players[i], addresses, session->value49c8,
						session->data4db0, summary, (char)i, &players[i]);
			}
			session->set_data_4f24(&machines, (s_unknown_3648 *)players);
			if (SESSION_STATE_IS_LIVE(session->state) && session->value5dd0 != NONE)
			{
				if (SESSION_STATE_IS_LIVE(session->state))
				{
					if (session->state == 5 || session->state == 6 || session->state == 7 || session->state == 8)
					{
						session->value4994 = 2;
						session->update_count++;
					}
					else { volatile long unused = session->state; }
				}
				if (SESSION_STATE_IS_LIVE(session->state))
				{
					if (session->state == 5 || session->state == 6 || session->state == 7 || session->state == 8)
					{
						session->value4990 = 1;
						session->update_count++;
					}
					else { volatile long unused = session->state; }
				}
			}
		}
		result = true;
	}
	return result;
}

void function_148823();
void function_14a152();
bool __stdcall function_236917(long controller);
bool __stdcall function_236953(long controller);
bool __stdcall function_2323ab(long controller);
void network_session_check_tracking(c_class_58d20 *session);
void network_session_close(c_class_58d20 *session);

static inline bool session_dialog_is(c_class_1473c9 *screen, long id)
{
 return screen && ((screen->screen_id >= 7 && screen->screen_id <= 8) ||
  screen->screen_id == 0xf0) && ((c_dialog_screen *)screen)->dialog_id == id;
}

// @retail 0x6d270
void function_6d270(void)
{
 function_148823();
 function_14a152();
 bool choice = true;
 long id = 0x38;
 dialog_choice_callback accept = function_236917;
 if (g_4e6948 && g_4e6948->flag1120 && g_4e6948->state == 1 && !g_4e6948->flag134)
 {
  choice = false;
  id = 0xb8;
  accept = function_2323ab;
 }
 c_window_channel *channel = &g_54d598.windows_1[4];
 if (!session_dialog_is(channel->current, id) && !session_dialog_is(channel->next, id))
 {
  if (choice)
   dialog_choice_show(1, id, 4, (word)-1, accept, function_236953, 0);
  else
   dialog_ok_show(1, id, 4, (word)-1, accept, 0);
 }
 c_class_58d20 *session = (c_class_58d20 *)g_527330.session_a;
 if (session->state)
 {
  network_session_check_tracking(session);
  network_session_close(session);
 }
 session = (c_class_58d20 *)g_527330.session_b;
 if (session->state)
 {
  network_session_check_tracking(session);
  network_session_close(session);
 }
}


struct s_session_join_request;
long network_session_evaluate_join_request(c_class_58d20 *session, const s_session_join_request *request);
bool function_630f0(c_class_58d20 *session, const s_session_join_request *request, long address, long reason);

// @retail 0x6dc60
void __stdcall function_06dc60(c_session_client *client, long reason)
{
 s_session_request *request = client->requests;
 while (request)
 {
  s_session_request *next = request->next;
  long rejection = reason;
  if (!rejection)
   rejection = network_session_evaluate_join_request(client->session,
    (const s_session_join_request *)&request->remote);
  function_630f0(client->session, (const s_session_join_request *)&request->remote,
   (long)request->key04, rejection);
  session_client_remove_request(client, request);
  request = next;
 }
}

long function_19989d(void);

// @retail 0x6cad0
void __stdcall function_6cad0(long unused)
{
 long const *argument_reference = &unused;
 if (g_467214 != NONE)
  function_6d270();
 else
 {
  long state = function_19989d();
  if ((state == 2 || state == 3) && g_4e6948 && g_4e6948->flag1120 && g_4e6948->state == 2)
  {
   c_class_58d20 *session = (c_class_58d20 *)g_527330.session_a;
   if (session->state)
   {
    network_session_check_tracking(session);
    network_session_close(session);
   }
   session = (c_class_58d20 *)g_527330.session_b;
   if (session->state)
   {
    network_session_check_tracking(session);
    network_session_close(session);
   }
   dialog_ok_show(1, 0x3a, 4, (word)-1, 0, 0);
  }
 }
}


struct s_match_result_data
{
 XNKEY key;
 XNKID id;
 XNADDR address;
 DWORD public_filled, public_open, private_filled, private_open;
 long properties[7];
};
struct s_qos_target
{
 XNKID kid;
 XNKEY key;
 XNADDR xna;
};
#include "network_qos.h"
bool function_90160(long task_index, s_match_result_data *result);
long online_match_session_find(XNKID const *session_id);
long qos_lookup(long kind, long count, long bits_per_second, s_qos_target *targets);
bool qos_is_complete(long handle);
bool function_7c530(const byte *data, long size, void *description);

// @retail 0x6f4b0
void function_06f4b0(c_session_state_joining *self)
{
 s_session_state_joining_view *state = (s_session_state_joining_view *)self;
 if (state->unknowne9)
 {
  state->unknown104 = 1;
  return;
 }
 if (state->unknownf0 != NONE)
 {
  switch (online_task_poll(state->unknownf0))
  {
  case 0:
  case 1:
   return;
  case 2:
   {
    s_match_result_data result;
    if (function_90160(state->unknownf0, &result))
    {
     s_qos_target target;
     target.kid = result.id;
     target.key = result.key;
     target.xna = result.address;
     state->unknownf4 = qos_lookup(1, 1, g_network_configuration.value18c, &target);
     if (state->unknownf4 == NONE)
      state->unknown104 = 4;
    }
    else
     state->unknown104 = 3;
   }
   break;
  }
  function_6b640(state->unknownf0);
  state->unknownf0 = NONE;
 }
 else if (state->unknownf4 != NONE)
 {
  if (!qos_is_complete(state->unknownf4))
   return;
  s_qos_result result;
  if (qos_target_result(state->unknownf4, &result, 0))
  {
   struct { s_session_description description; byte remaining[0x714 - sizeof(s_session_description)]; } buffer;
   s_session_description *description = &buffer.description;
   if (function_7c530(result.data, result.data_size, description))
   {
    self->function_06f2b0(description, state->entry_count);
    state->unknown10 = false;
    state->unknown11 = false;
    if (!state->unknown104)
    {
     state->part.unknown40 = description->unknown10;
     state->part.unknown00 = description->unknown02;
     *(XNKID *)state->part.unknown04 = description->kid;
     *(XNKEY *)state->part.unknown0c = description->key;
     *(XNADDR *)state->part.unknown1c = description->address;
     state->unknown68 = true;
    }
   }
   else
    state->unknown104 = 6;
  }
  else
   state->unknown104 = 5;
  qos_release(state->unknownf4);
  state->unknownf4 = NONE;
 }
 else
 {
  state->unknownf0 = online_match_session_find((const XNKID *)(state->target + 0x28));
  if (state->unknownf0 == NONE)
   state->unknown104 = 16;
 }
}
