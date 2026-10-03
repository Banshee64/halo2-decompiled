/* UNKNOWN_249E20.H: the structure bsp's audibility data (g_4e0348) */
#ifndef UNKNOWN_249E20_H
#define UNKNOWN_249E20_H

#include "cseries.h"

struct s_structure_audibility
{
	byte unknown00[0x10];
	dword *bits;
	byte unknown14[0x2c - 0x14];
	long cluster_count;
	char *clusters;
};

struct s_structure_bsp_view
{
	byte unknown00[0x60];
	short *sound_clusters;
	byte unknown64[0x9c - 0x64];
	long cluster_count;
	byte unknowna0[0x220 - 0xa0];
	struct s_structure_cluster_map
	{
		byte unknown00[0xc];
		short *indices;
	} *cluster_map;
	long audibility_count;
	s_structure_audibility *audibility;
};

long function_249e20(s_structure_bsp_view *bsp, long index);
long function_249e60(long cluster_index, s_structure_bsp_view *bsp, long index);

#endif
