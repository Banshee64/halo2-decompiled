/* NETWORK_SESSION.H: the network session object (0x7424 bytes; the state of
   the session is at +0x741c). The state machine (unknown_058dd0.cpp), the
   getters of unknown_05b040.cpp and the setters of unknown_05c490.cpp all
   work on this one type. Only the fields they touch are named. */

#ifndef NETWORK_SESSION_H
#define NETWORK_SESSION_H

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

struct s_long_pair
{
	long a;
	long b;
};

struct s_unknown_108
{
	long data[27];
};

struct s_unknown_3648
{
	long data[0x390];
};

#pragma pack(push, 1)
class c_network_session
{
public:
	byte unknown00[4];
	void *unknown04;
	byte unknown08[0x14];
	long unknown1c;
	long unknown20;
	byte unknown24[0x1c];
	long member_index;
	byte unknown44[0xc];
	long value50;
	byte unknown54[4];
	s_session_member members[16];
	byte unknown1118[0x4978 - 0x1118];
	long update_count;
	byte unknown497c[4];
	long type;
	byte unknown4984[0x4994 - 0x4984];
	long value4994;
	byte flag4998;
	s_long_pair data4999;
	byte unknown49a1[0x49c4 - 0x49a1];
	byte value49c4;
	byte unknown49c5[3];
	long value49c8;
	byte unknown49cc[0x49fd - 0x49cc];
	byte flag49fd;
	byte unknown49fe[2];
	byte data4a00[4];
	byte unknown4a04[0x4d08 - 0x4a04];
	long value4d08;
	long value4d0c;
	byte flag4d10;
	byte unknown4d11[0x4da0 - 0x4d11];
	long value4da0;
	long value4da4;
	long value4da8;
	byte unknown4dac[0x4f20 - 0x4dac];
	byte flag4f20;
	byte unknown4f21[3];
	s_unknown_108 data4f24;
	s_unknown_3648 data4f90;
	byte unknown5dd0[0x5e20 - 0x5dd0];
	long value5e20;
	byte unknown5e24[0x72d8 - 0x5e24];
	long current_member;
	byte unknown72dc[0x741c - 0x72dc];
	long state;
	bool flag7420;

	/* getters (unknown_05b040.cpp) */
	byte get_value_49c4();
	long get_value_49c8();
	byte *get_data_4a00();
	long get_value_5e20();
	bool get_values_4d08(long *a, long *b, byte **c);
	__int64 get_values_4da0();

	/* setters (unknown_05c490.cpp) */
	bool set_value_4994(long value);
	bool set_values_4da0(long a, long b);
	bool clear_value_49c4();
	bool set_value_4da8(long value);
	bool set_data_4f24(const s_unknown_108 *a, const s_unknown_3648 *b);
	bool set_data_4999(const s_long_pair *data);
	bool set_value_5e20(long value);

	/* state query (unknown_058cb0.cpp) */
	bool function_058d20();

	/* not decompiled yet (src/stubs/session.cpp) */
	void function_05a400(long arg);
	void function_05bec0();
};
#pragma pack(pop)

#endif
