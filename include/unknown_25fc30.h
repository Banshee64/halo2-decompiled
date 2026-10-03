/* UNKNOWN_25FC30.H: firing position evaluation structures (offsets taken from
the retail code; only the fields the evaluators touch are named) */

#ifndef UNKNOWN_25FC30_H
#define UNKNOWN_25FC30_H

#include "cseries.h"
#include "globals.h"
#include "real_math.h"

struct firing_position_definition
{
	byte unknown00[0xc];
	short unknown0c;
	byte flags;
	byte unknown0f;
	short unknown10;
	short unknown12;
	long unknown14;
};

/* a firing position being evaluated (0x78 bytes) */
struct firing_position
{
	firing_position_definition *definition;
	s_reference reference;
	short type;
	byte unknown0a[2];
	real_point3d position;
	real unknown18;
	byte unknown1c[0xc];
	real unknown28;
	real unknown2c;
	real unknown30;
	real_vector3d unknown34;
	real_vector3d unknown40;
	bool unknown4c;
	bool unknown4d;
	byte unknown4e[2];
	real unknown50;
	real score;
	bool unknown58;
	bool unknown59;
	byte unknown5a[2];
	short unknown5c;
	byte unknown5e[2];
	real_point3d unknown60;
	real_vector3d unknown6c;
};

struct firing_position_evaluation_context
{
	byte type;
	byte unknown01[3];
	union
	{
		struct
		{
			bool unknown04;
			byte unknown05[3];
			long unknown08;
			byte unknown0c[4];
			bool unknown10;
			bool unknown11;
			byte unknown12[2];
			bool unknown14;
			byte unknown15[3];
			real unknown18;
		};
		struct
		{
			byte unknown04_[4];
			real range08;
			real range0c;
			real range10;
			byte unknown14_[4];
			real range18;
		};
	};
	byte unknown1c[0x54 - 0x1c];
	bool unknown54;
	bool unknown55;
	bool unknown56;
	byte unknown57;
	bool unknown58;
	byte unknown59;
	bool unknown5a;
	byte unknown5b[0x60c - 0x5b];
	byte unknown60c[0x618 - 0x60c];
	bool unknown618;
	byte unknown619[3];
	real unknown61c;
	byte unknown620[0x24];
	bool unknown644;
	byte unknown645[0x668 - 0x645];
	bool unknown668;
	byte unknown669[3];
	real_vector3d unknown66c;
	real unknown678;
	byte unknown67c[4];
	real unknown680;
	long unknown684;
	long unknown688;
	bool unknown68c;
	byte unknown68d;
	short unknown68e;
	short unknown690;
	byte unknown692[0x6a0 - 0x692];
	long unknown6a0;
};

#endif
