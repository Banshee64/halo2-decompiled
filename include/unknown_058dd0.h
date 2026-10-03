/* UNKNOWN_058DD0.H: the game-session state machine states (the vtables at
   0x450990) and the session structure they drive */

#ifndef UNKNOWN_058DD0_H
#define UNKNOWN_058DD0_H

#include "cseries.h"
#include "network_session.h"

/* the object the states act on (state +8): sessions and a message buffer */
struct s_session_owner
{
	byte unknown00[0x30];
	c_network_session *session_a;
	c_network_session *session_c;
	c_network_session *session_b;
	byte unknown3c[0x4a - 0x3c];
	byte failed;
	byte unknown4b;
	long error_code;
	long data_size;
	long data[1];
};

/* the remote machine a session client talks to (only the fields it reads) */
struct s_session_remote
{
	long id;
	byte address04[0x140];
	byte address144[8];
	byte has_address;
	byte unknown14d[0x188 - 0x14d];
	dword key188[9];
};

/* a request the client queues (0x1c8 bytes; lane D, network_session_client.cpp) */
struct s_session_request
{
	s_session_request *next;
	dword key04[5];
	s_session_remote remote;
	byte unknown1c4[4];
};

/* the client of a session: slot 0 connects, slot 1 checks an id, slot 3 asks
   whether the address is known */
class c_session_client
{
public:
	virtual void function_06dae0(long a, s_session_remote *remote);
	virtual void function_06dbd0(const s_session_id *id);
	virtual void function_06dbc0();
	virtual bool function_06daa0(long a);

	bool function_06dcc0(s_session_remote *remote);
	bool function_06de10(s_session_remote *remote);
	bool function_06dd00(long a, s_session_remote *remote);

	c_network_session *session;
	long mode;
	s_session_request *requests;
	long request_count;
};

/* a snapshot of a session member, built by 05a620 and compared with the live one */
struct s_session_snapshot
{
	long unknown00;
	byte unknown04[8];
	byte unknown0c[0x10];
	dword words[9];
	long unknown40;
};

/* the common base of the states: slot 0 updates the state, slot 1 enters it,
   slot 2 leaves it and slot 3 names it */
class c_session_state
{
public:
	virtual bool update() { return false; }
	virtual void enter(long a, long b, long c);
	virtual void leave(long a) {}
	virtual const char *get_name() { return 0; }

	bool function_06dfa0();
	void function_06ec10(c_network_session *s);

	byte unknown04[4];
	s_session_owner *owner;
	bool skip_cleanup;
};

/* in-game */
class c_session_state_in_game : public c_session_state
{
public:
	virtual bool update();
	virtual void enter(long a, long b, long c);
	virtual void leave(long a);
	virtual const char *get_name();

	long id_a;
	long id_b;
};

/* in-match */
class c_session_state_in_match : public c_session_state
{
public:
	virtual bool update();
	virtual void enter(long a, long b, long c);
	virtual void leave(long a);
	virtual const char *get_name();

	long time;
	long unknown14;
	long id_a;
	long id_b;
};

/* start-match */
class c_session_state_start_match : public c_session_state
{
public:
	virtual bool update() { return false; }
	virtual void enter(long a, long b, long c);
	virtual void leave(long a);
	virtual const char *get_name();

	void function_072950();

	long mode;
	byte unknown14;
	byte unknown15[3];
	long unknown18;
};

/* pre-game */
class c_session_state_pre_game : public c_session_state
{
public:
	virtual bool update();
	virtual void enter(long a, long b, long c);
	virtual const char *get_name();

	bool function_06e410();

	byte unknown0d[4];
	long unknown14;
	long unknown18;
	long unknown1c;
};

/* matchmaking */
class c_session_state_matchmaking : public c_session_state
{
public:
	virtual bool update() { return false; }
	virtual void enter(long a, long b, long c);
	virtual void leave(long a);
	virtual const char *get_name();

	void function_070c50(bool flag);
	void function_070d20(bool flag);

	byte flag10;
	byte unknown11[3];
	long unknown14[0x254];
	long time;
	long unknown968;
	byte unknown96c[4];
	long unknown970;
	byte unknown974[4];
	long mode;
	byte flag97c;
	byte unknown97d[0xa64 - 0x97d];
	byte flaga64;
	byte unknowna65[3];
	long unknowna68;
	byte unknowna6c[0xa78 - 0xa6c];
	byte flaga78;
	byte unknowna79[3];
	long unknowna7c;
};

/* post-match */
class c_session_state_post_match : public c_session_state
{
public:
	virtual bool update();
	virtual const char *get_name();
};

/* none */
class c_session_state_none : public c_session_state
{
public:
	virtual bool update();
	virtual const char *get_name();
};

/* start-game */
class c_session_state_start_game : public c_session_state
{
public:
	virtual bool update();
	virtual void enter(long a, long b, long c);
	virtual const char *get_name();

	byte flag10;
};

/* joining */
class c_session_state_joining : public c_session_state
{
public:
	virtual bool update();
	virtual void enter(long a, long b, long c);
	virtual void leave(long a);
	virtual const char *get_name();

	void function_06f0f0();

	byte flag10;
	byte unknown11[0xe9 - 0x11];
	bool flage9;
	byte unknownea[0xf8 - 0xea];
	byte flagf8;
	byte unknownf9[3];
	long unknownfc;
	long unknown100;
	long unknown104;
};

/* post-game */
class c_session_state_post_game : public c_session_state
{
public:
	virtual bool update();
	virtual const char *get_name();
};

/* callees that are not decompiled yet */
long __stdcall function_063190(void *p, long a);
long __stdcall function_0632e0(void *p, void *q);
bool function_063510(void *a, void *p, long x);
void __stdcall function_06d380(c_session_client *client, const s_session_id *id);
void __stdcall function_06dc60(c_session_client *client, long n);
void __stdcall function_07b140(void *x, long a, long ten, long twelve, void *local);
bool function_058d90(c_network_session *s);
bool function_06ec80(c_network_session *s, bool flag);
bool function_06e6b0(c_network_session *s, byte *p);
bool function_06e720(c_network_session *s);
void function_06df60(s_session_owner *o, long a, long b, long c);
bool function_0682c0();
bool function_058d50(c_network_session *s);
void function_090c80(byte *p);
void function_06f4b0(c_session_state_joining *self);
void function_06f700(c_session_state_joining *self);
void function_06fcc0(c_session_state_joining *self);
bool function_058d70(c_network_session *s);
bool function_06e360();
void function_06e620(c_network_session *s);
bool function_138800();
bool function_138a10();
void function_1388e0();
void function_068750();

#endif
