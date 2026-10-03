/* NETWORK_SESSION_MANAGER.H: the session manager's owner (0x527334) and its
   states as the manager's functions see them (lane D) */

#ifndef NETWORK_SESSION_MANAGER_H
#define NETWORK_SESSION_MANAGER_H

#include "cseries.h"
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

/* the fields every state has */
struct s_session_state_view
{
	void *vtable;
	long index;
	s_session_owner_view *owner;
	bool skip_cleanup;
	bool unknown0d;
	byte unknown0e[2];
	void *unknown10;
	long unknown14;
};

/* the matchmaking state's fields */
struct s_session_state_matchmaking_view
{
	void *vtable;
	long index;
	s_session_owner_view *owner;
	bool skip_cleanup;
	bool unknown0d;
	byte unknown0e[2];
	byte unknown10[0x96c - 0x10];
	long unknown96c;
	long unknown970;
	long unknown974;
	long mode;
	bool unknown97c;
	byte unknown97d[0x9ec - 0x97d];
	long unknown9ec;
	long unknown9f0;
	long unknown9f4;
	byte unknown9f8[0xa08 - 0x9f8];
	long unknowna08;
	long unknowna0c;
	byte unknowna10[0xa1c - 0xa10];
	long unknowna1c;
	byte unknowna20[0xa64 - 0xa20];
	bool unknowna64;
	byte unknowna65[0xa78 - 0xa65];
	bool unknowna78;
	byte unknowna79[0xa80 - 0xa79];
	long unknowna80;
	long unknowna84;
	long unknowna88;
	long unknowna8c;
	long unknowna90;
	long unknowna94;
};

/* sets up a state of the owner */
inline void session_state_initialize(s_session_state_view *state, s_session_owner_view *owner, long index, bool unknown0d, bool skip_cleanup)
{
	state->owner = owner;
	state->index = index;
	state->unknown0d = unknown0d;
	state->skip_cleanup = skip_cleanup;
	state->owner->states[index] = (c_session_state *)state;
}

/* takes a state out of its owner */
inline void session_state_dispose(s_session_state_view *state)
{
	state->owner->states[state->index] = NULL;
	state->owner = NULL;
}

/* src/network_session_client.cpp */
void session_owner_initialize(s_session_owner *owner_, long unknown40, long unknown44, void *unknown2c, c_network_session *session_a, c_network_session *session_c, c_network_session *session_b, void *unknown3c);
void session_state_joining_initialize(c_session_state_joining *state_, s_session_owner *owner);
void session_state_matchmaking_initialize(c_session_state_matchmaking *state_, s_session_owner *owner);

#endif