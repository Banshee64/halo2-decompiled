/* UNKNOWN_2420A0.H: the table at g_51ec80 and the types around it, shared by
   the functions of src/unknown_2420a0.cpp (the capture the flag and assault
   engine, whose vtable is at 0x459d18) */

#ifndef UNKNOWN_2420A0_H
#define UNKNOWN_2420A0_H

#include "cseries.h"
#include "real_math.h"

struct s_long_triple
{
	long a;
	long b;
	long c;
};

struct s_slot_flags
{
	byte bit0 : 1;
	byte bit1 : 1;
	byte bit2 : 1;
	byte bit3 : 1;
	byte unused : 4;
};

/* g_51ec80 (0x208 bytes at +0xfc of the multiplayer globals): nine slots
   (one per team, the last one neutral), each described by several parallel
   arrays */
struct s_slot_table
{
	bool initialized;
	byte unknown01;
	short a[9];
	short b[9];
	short c[9];
	long l38;
	point3f bounds[2][9];
	short d[9];
	short times[9];
	byte unknown138[0x14c - 0x138];
	long objects[9];
	union
	{
		byte flags[9];
		s_slot_flags flag_bits[9];
	};
	byte unknown179[3];
	union
	{
		s_long_triple triples[3];
		long carriers[9];
	};
	short e[9];
	byte unknown1b2[2];
	long player_times[16];
	long l1f4;
	byte unknown1f8[4];
	bool b1fc;
	byte unknown1fd[3];
	long l200;
	word w204;
	byte unknown206[2];
};

extern s_slot_table *g_51ec80;

#endif
