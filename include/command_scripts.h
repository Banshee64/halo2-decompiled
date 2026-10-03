/* COMMAND_SCRIPTS.H: the command scripts (g_502408) and the one being run */

#ifndef COMMAND_SCRIPTS_H
#define COMMAND_SCRIPTS_H

#include "cseries.h"
#include "globals.h"

/* the command scripts (0xd4 bytes each) and the one being run */
struct s_command_script
{
	byte unknown00[4];
	short type;
	short value_short;
	real value8;
	real valuec;
	byte unknown10[0x28 - 0x10];
	long index_a;
	long index_b;
	byte unknown30[4];
	long name_index;
	byte unknown38[4];
	long next_index;
	byte unknown40[0x45 - 0x40];
	bool flag45;
	bool flag46;
	byte unknown47;
	short type48;
	byte unknown4a[2];
	long index4c;
	bool flag50;
	bool flag51;
	bool flag52;
	byte unknown53;
	short type54;
	byte unknown56[2];
	long index58;
	bool flag5c;
	byte unknown5d[3];
	real value60;
	bool flag64;
	byte unknown65[3];
	real value68;
	bool flag6c;
	byte unknown6d[3];
	real value70;
	bool flag74;
	bool flag75;
	bool flag76;
	byte unknown77[2];
	bool flag79;
	bool flag7a;
	byte unknown7b;
	short value7c;
	byte unknown7e;
	bool flag7f;
	bool flag80;
	bool flag81;
	bool flag82;
	bool flag83;
	short value84;
	bool flag86;
	byte unknown87;
	long style88;
	byte unknown8c[0x94 - 0x8c];
	long name94;
	byte unknown98[0xac - 0x98];
	bool flagac;
	byte unknownad[3];
	long indexb0;
	real valueb4;
	real valueb8;
	real valuebc;
	byte unknownc0[0xd0 - 0xc0];
	bool flagd0;
	byte unknownd1[3];
};

extern long g_502410;

inline s_command_script *command_script_get(long index)
{
	return &((s_command_script *)g_502408->data)[index & 0xffff];
}

#endif
