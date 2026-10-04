// @flags /O2 /arch:SSE /Ob1 /Gr
/* UNKNOWN_16E5A0.CPP: the request for one cluster's geometry block of the
   current structure bsp. Retail calls it out of line from the script
   function 0x2ac550, so it sits in its own /Ob1 file apart from
   unknown_16e290.cpp (which needs its own inlining). */

#include "cseries.h"
#include "globals.h"
#include "geometry_cache.h"

bool function_12dcb0(s_geometry_block_info *block);

/* the structure bsp's clusters (a local view, as in unknown_16e290.cpp) */
struct s_16e5a0_cluster
{
	byte unknown00[0xc];
	s_geometry_block_info block;
	byte unknown_end[0x38 - 0xc - sizeof(s_geometry_block_info)];
};

struct s_16e5a0_bsp
{
	byte unknown00[0x40];
	long cluster_count;
	s_16e5a0_cluster *clusters;
};

#define PIN(x, lo, hi) ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))

// @retail 0x16e5a0
void function_16e5a0(short bsp_index, long cluster_index)
{
	if (bsp_index == g_4686c4)
	{
		s_16e5a0_bsp *bsp = (s_16e5a0_bsp *)g_4e0344->bsp;

		if (cluster_index != NONE && PIN(cluster_index, 0, bsp->cluster_count - 1) == cluster_index)
		{
			function_12dcb0(&bsp->clusters[cluster_index].block);
		}
	}
}
