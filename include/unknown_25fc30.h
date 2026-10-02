/* UNKNOWN_25FC30.H: firing position evaluation structures (offsets taken from
the retail code; only the fields the evaluators touch are named) */

#ifndef UNKNOWN_25FC30_H
#define UNKNOWN_25FC30_H

#include "cseries.h"

struct firing_position_definition
{
	byte unknown00[0xe];
	byte flags;
};

struct firing_position
{
	firing_position_definition *definition;
	byte unknown04[4];
	short type;
	byte unknown0a[0xe];
	real unknown18;
	byte unknown1c[0x14];
	real unknown30;
	byte unknown34[0x18];
	bool unknown4c;
	bool unknown4d;
	byte unknown4e[6];
	real score;
	bool unknown58;
	byte unknown59[0x1f];
};

struct firing_position_evaluation_context
{
	byte type;
	byte unknown01[0xf];
	bool unknown10;
	bool unknown11;
	byte unknown12[2];
	bool unknown14;
	byte unknown15[0x603];
	bool unknown618;
	byte unknown619[3];
	real unknown61c;
	byte unknown620[0x24];
	bool unknown644;
	byte unknown645[0x3b];
	real unknown680;
};

#endif
