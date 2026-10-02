/* UNKNOWN_2420A0.H: the table at g_51ec80 and the types around it, shared by
   the functions of src/unknown_2420a0.cpp */

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

/* g_51ec80: nine slots, each described by several parallel arrays */
struct s_slot_table
{
	byte unknown00[2];
	short a[9];
	short b[9];
	short c[9];
	byte unknown38[4];
	real_point3d bounds[2][9];
	short d[9];
	byte unknown126[0x170 - 0x126];
	byte flags[9];
	byte unknown179[3];
	s_long_triple triples[3];
	short e[9];
	byte unknown1b2[0x1f4 - 0x1b2];
	long l1f4;
};

extern s_slot_table *g_51ec80;

#endif
