// @flags /O2 /Ob1 /Gr
/* UNKNOWN_249E20.CPP: lookups in the structure bsp's audibility data.
   Decompiled by lane F: 0x18c3b0 calls them with register arguments. */

#include "cseries.h"
#include "unknown_249e20.h"

// @retail 0x249e20
long function_249e20(s_structure_bsp_view *bsp, long index)
{
	long result = NONE;

	if (index != NONE && bsp->audibility_count > 0)
	{
		s_structure_audibility *audibility = bsp->audibility;
		for (long i = 0; i < audibility->cluster_count; i++)
		{
			if (audibility->clusters[i] == index)
			{
				result = i;
				break;
			}
		}
	}
	return result;
}

// @retail 0x249e60
long function_249e60(long cluster_index, s_structure_bsp_view *bsp, long index)
{
	long result = NONE;

	if (bsp->audibility_count > 0)
	{
		dword *bits = bsp->audibility->bits;
		long stride = (bsp->cluster_count + 31) >> 5;
		long row = 2 * index;

		if (bits[stride * row + (cluster_index >> 5)] & (1 << (cluster_index & 0x1f)))
		{
			result = 0;
		}
		else if (bits[stride * (row + 1) + (cluster_index >> 5)] & (1 << (cluster_index & 0x1f)))
		{
			result = 1;
		}
	}
	return result;
}
