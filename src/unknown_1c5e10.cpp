// @flags /O2 /Ob1 /Gr
/* UNKNOWN_1C5E10.CPP: packing settings into tagged records (0x1c59a0..0x1c62e0):
   each record starts with a 'b...' tag and ends with the matching 'e...'
   tag, and the records are byte packed */

#include "cseries.h"
#include <string.h>

struct s_block40
{
	dword unknown[0x10];
};

struct s_vr_source
{
	long unknown00;
	s_block40 unknown04;
	long unknown44;
	byte unknown48[4];
	long unknown4c;
	long unknown50;
	long unknown54;
	byte unknown58[0x74 - 0x58];
	long unknown74;
	byte unknown78[0x80 - 0x78];
	long unknown80;
	byte unknown84[0xd4 - 0x84];
	byte unknownd4;
	byte unknownd5;
	byte unknownd6;
	byte unknownd7;
};

struct s_pap_source
{
	long unknown0;
	byte unknown4;
	byte unknown5;
	byte unknown6;
	byte unknown7;
};

struct s_con_source
{
	long unknown0;
	byte unknown4;
	byte unknown5;
	byte unknown6;
};

struct s_block10
{
	dword unknown[4];
};

struct s_block0c
{
	dword unknown[3];
};

struct s_block20
{
	dword unknown[8];
};

/* six shorts, at an odd place in s_pgd_source */
struct s_shorts6
{
	short unknown[6];
};

struct s_pcn_source
{
	s_block40 unknown00;
	s_pap_source unknown40;
	byte unknown48[8];
	s_block20 unknown50;
	s_block0c unknown70;
	byte unknown7c;
	byte unknown7d;
	byte unknown7e;
	byte unknown7f;
	byte unknown80;
	byte unknown81;
	byte unknown82[2];
	long unknown84;
	short unknown88;
	short unknown8a;
	long unknown8c;
};

struct s_pgd_source
{
	byte unknown00;
	byte unknown01;
	s_shorts6 unknown02;
	byte unknown0e[2];
	s_pcn_source unknown10;
	byte unknowna0;
	byte unknowna1;
	short unknowna2;
};

struct s_ppr_source
{
	byte unknown00[8];
	s_block40 unknown08;
	s_block10 unknown48;
	byte unknown58[0xec - 0x58];
	s_block10 unknownec;
	s_con_source unknownfc;
	byte unknown104[0x118 - 0x104];
	s_pap_source unknown118;
};

#pragma pack(push, 1)

struct s_packed_vr
{
	dword begin;
	s_block40 unknown04;
	long unknown44;
	dword inner_begin;
	long unknown4c;
	long unknown50;
	long unknown54;
	long unknown58;
	long unknown5c;
	byte unknown60;
	byte unknown61;
	byte unknown62;
	dword inner_end;
	dword end;
};

struct s_packed_pap
{
	dword begin;
	long unknown4;
	byte unknown8;
	byte unknown9;
	byte unknowna;
	byte unknownb;
	dword end;
};

struct s_packed_con
{
	dword begin;
	long unknown4;
	byte unknown8;
	byte unknown9;
	byte unknowna;
	dword end;
};

struct s_packed_pcn
{
	dword begin;
	s_block40 unknown04;
	s_packed_pap unknown44;
	s_block20 unknown54;
	s_block0c unknown74;
	byte unknown80;
	byte unknown81;
	byte unknown82;
	byte unknown83;
	byte unknown84;
	byte unknown85;
	long unknown86;
	short unknown8a;
	short unknown8c;
	long unknown8e;
	dword end;
};

struct s_packed_pgd
{
	dword begin;
	byte unknown04;
	byte unknown05;
	s_shorts6 unknown06;
	s_packed_pcn unknown12;
	byte unknowna8;
	byte unknowna9;
	short unknownaa;
	dword end;
};

struct s_packed_ppr
{
	dword begin;
	s_block40 unknown04;
	s_block10 unknown44;
	s_block10 unknown54;
	s_packed_con unknown64;
	s_packed_pap unknown73;
	dword end;
};

#pragma pack(pop)

/* the empty records (the writers below with no source), which the record
   writers inline */
static inline void packed_con_clear(s_packed_con *packed)
{
	memset(packed, 0, sizeof(s_packed_con));
	packed->begin = 'bcon';
	packed->end = 'econ';
}

static inline void packed_pap_clear(s_packed_pap *packed)
{
	memset(packed, 0, sizeof(s_packed_pap));
	packed->begin = 'bpap';
	packed->end = 'epap';
}

// @retail 0x1c5e10
void packed_vr_write(s_vr_source const *source, s_packed_vr *packed)
{
	packed->begin = 'bgvr';
	packed->unknown04 = source->unknown04;
	packed->unknown44 = source->unknown44;
	packed->inner_begin = 'buvr';
	packed->unknown4c = source->unknown4c;
	packed->unknown50 = source->unknown50;
	packed->unknown54 = source->unknown54;
	packed->unknown58 = source->unknown74;
	packed->unknown5c = source->unknown80;
	packed->unknown60 = source->unknownd4;
	packed->unknown61 = source->unknownd6;
	packed->unknown62 = source->unknownd7;
	packed->inner_end = 'euvr';
	packed->end = 'egvr';
}

// @retail 0x1c5e80
void packed_pap_write(s_pap_source const *source, s_packed_pap *packed)
{
	if (!source)
	{
		memset(packed, 0, sizeof(s_packed_pap));
	}
	else
	{
		packed->unknown4 = source->unknown0;
		packed->unknown8 = source->unknown4;
		packed->unknown9 = source->unknown5;
		packed->unknowna = source->unknown6;
		packed->unknownb = source->unknown7;
	}
	packed->begin = 'bpap';
	packed->end = 'epap';
}

// @retail 0x1c61c0
void packed_con_write(s_con_source const *source, s_packed_con *packed)
{
	if (!source)
	{
		memset(packed, 0, sizeof(s_packed_con));
	}
	else
	{
		packed->unknown4 = source->unknown0;
		packed->unknown8 = source->unknown4;
		packed->unknown9 = source->unknown5;
		packed->unknowna = source->unknown6;
	}
	packed->begin = 'bcon';
	packed->end = 'econ';
}
// @retail 0x1c5ed0
void packed_pcn_write(s_pcn_source const *source, s_packed_pcn *packed)
{
	packed->begin = 'bpcn';
	packed->unknown04 = source->unknown00;
	packed_pap_write(&source->unknown40, &packed->unknown44);
	packed->unknown54 = source->unknown50;
	packed->unknown74 = source->unknown70;
	packed->unknown80 = source->unknown7c;
	packed->unknown81 = source->unknown7d;
	packed->unknown82 = source->unknown7e;
	packed->unknown83 = source->unknown7f;
	packed->unknown84 = source->unknown80;
	packed->unknown85 = source->unknown81;
	packed->unknown86 = source->unknown84;
	packed->unknown8a = source->unknown88;
	packed->unknown8c = source->unknown8a;
	packed->unknown8e = source->unknown8c;
	packed->end = 'epcn';
}

// @retail 0x1c5fa0
void packed_pgd_write(s_pgd_source const *source, s_packed_pgd *packed)
{
	packed->begin = 'bgpd';
	packed->unknown04 = source->unknown00;
	packed->unknown05 = source->unknown01;
	packed->unknown06 = source->unknown02;
	packed_pcn_write(&source->unknown10, &packed->unknown12);
	packed->unknowna8 = source->unknowna0;
	packed->unknowna9 = source->unknowna1;
	packed->unknownaa = source->unknowna2;
	packed->end = 'egpd';
}

// @retail 0x1c6210
void packed_ppr_write(s_ppr_source const *source, s_packed_ppr *packed)
{
	if (!source)
	{
		memset(packed, 0, sizeof(s_packed_ppr));
		packed_con_clear(&packed->unknown64);
		packed_pap_clear(&packed->unknown73);
	}
	else
	{
		packed->unknown04 = source->unknown08;
		packed->unknown44 = source->unknown48;
		packed->unknown54 = source->unknownec;
		packed_con_write(&source->unknownfc, &packed->unknown64);
		packed_pap_write(&source->unknown118, &packed->unknown73);
	}
	packed->begin = 'bppr';
	packed->end = 'eppr';
}