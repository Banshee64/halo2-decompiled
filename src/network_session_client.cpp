// @flags /O2 /Gr
/* NETWORK_SESSION_CLIENT.CPP: the session client's request queue, the
   session owner and the joining state's helpers (lane D) */

#include "cseries.h"
#include <xtl.h>
#include <string.h>
#include "globals.h"
#include "unknown_058dd0.h"

/* the session owner (0x527334) as these functions see it */
struct s_session_owner_view
{
	long mode;
	c_session_state *states[10];
	void *unknown2c;
	c_network_session *session_a;
	c_network_session *session_c;
	c_network_session *session_b;
	void *unknown3c;
	long unknown40;
	long unknown44;
	bool unknown48;
	bool unknown49;
	bool failed;
	byte unknown4b;
	long error_code;
	long data_size;
	byte data[1];
};

/* the joining state's fields */
struct s_session_state_joining_view
{
	void *vtable;
	long index;
	s_session_owner_view *owner;
	bool skip_cleanup;
	bool unknown0d;
	byte unknown0e[2];
	bool unknown10;
	bool unknown11;
	byte unknown12[0x68 - 0x12];
	bool unknown68;
	byte unknown69[0xe4 - 0x69];
	long unknowne4;
	bool unknowne8;
	bool unknowne9;
	byte unknownea[2];
	long unknownec;
	long unknownf0;
	long unknownf4;
	bool unknownf8;
	bool unknownf9;
	byte unknownfa[2];
	long unknownfc;
	long unknown100;
	long unknown104;
};

#define SESSION_STATE_IS_LIVE(state) ((state) > 2 && (state) <= 8)

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
	bool found = false;
	for (s_session_request *request = requests; request; request = request->next)
	{
		if (found)
			break;
		if (!memcmp(request->remote.key188, remote->key188, sizeof(remote->key188)))
			found = true;
	}
	return found;
}

// @retail 0x6de50
void session_owner_initialize(s_session_owner *owner_, long unknown40, long unknown44, void *unknown2c, c_network_session *session_a, c_network_session *session_c, c_network_session *session_b, void *unknown3c)
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
void function_06df60(s_session_owner *o, long a, long b, long c)
{
	o->failed = true;
	o->error_code = a;
	o->data_size = b;
	o->data[0] = 0;
	if (o->data_size > 0)
		memcpy(o->data, (const void *)c, o->data_size);
}

// @retail 0x6e0f0
bool network_session_members_ready(c_network_session *session, dword *unready_mask)
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
