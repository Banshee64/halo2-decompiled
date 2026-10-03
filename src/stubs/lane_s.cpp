#include "cseries.h"
#include "real_math.h"

/* callees of lane S's region (0x100000-0x10ffff) that are not decompiled yet */

// @stub 0x176780
void function_176780(void *location, long tag_index, real position, long a, long b, real power)
{
}

// @stub 0x159dd0
bool __stdcall function_159dd0(long player_index)
{
	return false;
}

// @stub 0x159d40
bool function_159d40(void)
{
	return false;
}

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
