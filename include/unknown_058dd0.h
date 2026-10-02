/* UNKNOWN_058DD0.H: the game-session state machine states (the vtables at
   0x450990) and the session structure they drive */

#ifndef UNKNOWN_058DD0_H
#define UNKNOWN_058DD0_H

#include "cseries.h"

struct s_session_id
{
	long a;
	long b;
};

/* one of the session's 0x10c-byte member records */
struct s_session_member
{
	dword words[9];
	byte unknown24[0x88 - 0x24];
	long unknown88;
	byte unknown8c[0xf0 - 0x8c];
	s_session_id id;
	byte unknownf8[0x10c - 0xf8];
};

/* the session structure; only the fields the states touch are named */
struct s_session
{
	byte unknown00[4];
	void *unknown04;
	byte unknown08[0x14];
	long unknown1c;
	long unknown20;
	byte unknown24[0x1c];
	long member_index;
	byte unknown44[0x14];
	s_session_member members[16];
	byte unknown1118[0x4980 - 0x1118];
	long type;
	byte unknown4984[0x49fd - 0x4984];
	byte flag49fd;
	byte unknown49fe[2];
	byte unknown4a00[0x72d8 - 0x4a00];
	long current_member;
	byte unknown72dc[0x741c - 0x72dc];
	long state;
	byte flag7420;

	bool function_058d20();
	void function_05a400(long arg);
	void function_05bec0();
	bool function_05b0d0(long *a, long *b, long *c);
};

/* the object the states act on (state +8): sessions and a message buffer */
struct s_session_owner
{
	byte unknown00[0x30];
	s_session *session_a;
	s_session *session_c;
	s_session *session_b;
	byte unknown3c[0x4a - 0x3c];
	byte failed;
	byte unknown4b;
	long error_code;
	long data_size;
	long data[1];
};

/* the game options (g_4e6948), seen with the fields the states read */
struct s_session_options
{
	byte unknown00[0x10];
	long id_a;
	long id_b;
	byte unknown18[4];
	long position_a;
	long position_b;
	byte unknown24[0x1120 - 0x24];
	byte flag1120;
};

/* the remote machine a session client talks to (only the fields it reads) */
struct s_session_remote
{
	long id;
	byte address04[0x140];
	byte address144[8];
	byte has_address;
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

	s_session *session;
	long mode;
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
	void function_06ec10(s_session *s);

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
	byte unknown11[0xf8 - 0x11];
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
bool function_058d90(s_session *s);
bool function_05b040(s_session *s);
void function_05a220(s_session *s, long what);
bool function_06ec80(s_session *s, bool flag);
bool function_06e6b0(s_session *s, byte *p);
bool function_06e720(s_session *s);
void function_06df60(s_session_owner *o, long a, long b, long c);
bool function_0682c0();
bool function_058d50(s_session *s);
void function_05c290(s_session *s, long mode);
void function_090c80(byte *p);
void function_05a620(s_session *s, s_session_snapshot *snapshot);
bool function_05b1a0(s_session *a, s_session_snapshot *out);
void function_05c3f0(s_session_snapshot *snapshot, s_session *a);
void function_06f4b0(c_session_state_joining *self);
void function_06f700(c_session_state_joining *self);
void function_06fcc0(c_session_state_joining *self);
bool function_058d70(s_session *s);
bool function_06e360();
void function_06e620(s_session *s);
bool function_138800();
bool function_138a10();
void function_1388e0();
void function_068750();
long function_3314b0();

#endif
