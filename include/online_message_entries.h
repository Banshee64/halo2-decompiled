/* ONLINE_MESSAGE_ENTRIES.H: the views of a message summary, of the game's
   copy of it and of a message being read, shared by src/unknown_08ebc0.cpp
   (0x8ebd0, 0x8eda0) and src/online_messages.cpp */

#ifndef ONLINE_MESSAGE_ENTRIES_H
#define ONLINE_MESSAGE_ENTRIES_H

#include "unknown_11c920.h"

struct s_entry_source
{
	long unknown0;
	long unknown4;
	long unknown8;
	byte type;
	byte unknownd[3];
	long unknown10;
	long unknown14;
	long unknown18;
	long unknown1c;
	long unknown20;
	union
	{
		byte low;
		dword all;
	};
	long unknown28;
	short unknown2c;
	short unknown2e;
	char name[0x10];
};

struct s_entry
{
	long unknown0;
	long unknown4;
	long unknown8;
	char name[0x10];
	union
	{
		dword flags;
		struct
		{
			dword unknown_bits0 : 10;
			dword flag10 : 1;
			dword flag11 : 1;
			dword flag12 : 1;
		} flag_bits;
	};
	long unknown20;
	long unknown24;
	long unknown28;
	long unknown2c;
	long unknown30;
	long unknown34;
	short unknown38;
	short unknown3a;
	byte unknown3c[4];
};

struct s_state_block
{
	byte active;
	byte unknown1[3];
	long unknown4;
	long unknown8;
	short unknownc;
	byte unknownE[0x1fe];
	long unknown20c;
	long unknown210;
	byte unknown214[4];
	long unknown218;
	long unknown21c;
};

#endif
