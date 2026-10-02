/* UNKNOWN_26B230.H: clump structures (guessed layout) */

#ifndef UNKNOWN_26B230_H
#define UNKNOWN_26B230_H

#include "cseries.h"

struct s_data_array
{
	byte unknown0[0x44];
	byte *data;
};

struct s_clump
{
	byte unknown0[0x10];
	short divisor;
	byte unknown12[2];
	long first_prop;
	long first_object;
	byte unknown1c[0x0a];
	short state;
	long state_time;
	byte unknown2c[4];
	byte unknown30;
	byte unknown31[0x0d];
	word unknown3e;
	byte unknown40[0x10];
};

struct s_clump_prop
{
	byte unknown0[8];
	long type;
	byte unknownc[8];
	long next;
	byte unknown18[0xac];
};

struct s_clump_object
{
	byte unknown0[0x80];
	long next;
	byte unknown84[0x2a4];
	short count;
	byte unknown32a[0x55e];
};


#endif
