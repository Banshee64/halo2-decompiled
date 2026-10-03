#include "cseries.h"
#include "real_math.h"

/* callees of lane S's region (0x100000-0x10ffff) that are not decompiled yet */

/* lane C's marker lookups (0x1d8f00 takes ecx and edi in retail, 0x1d8f50 eax and ecx) */
struct s_object_marker;

// @stub 0x1d8f00
long function_1d8f00(long render_model_index, long marker_name)
{
	return NONE;
}

// @stub 0x1d8f50
short function_1d8f50(long marker_group_index, long render_model_index, byte const *region_permutations,
	long const *node_remapping, real_matrix4x3 const *node_matrices, bool mirrored, s_object_marker *markers, short count)
{
	return 0;
}

/* object callees of 0xb7680 (0xba350 takes the object index in esi in retail) */

// @stub 0xba350
void function_ba350(long object_index, long a)
{
}

// @stub 0xbd020
void __stdcall function_bd020(long object_index)
{
}
