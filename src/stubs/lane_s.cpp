#include "unknown_11c920.h"
#include "unknown_0259d0.h"

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
	long const *node_remapping, transform4x3f const *field_50, bool mirrored, s_object_marker *markers, short count)
{
	return 0;
}

/* object callees of 0xb7680 */

// @stub 0xbd020
void __stdcall function_bd020(long object_index)
{
}

/* the unit's weapon state change (lane S's 0x1058b0 calls it) */

// @stub 0xc9d00
void __stdcall function_c9d00(long unit_index, long weapon_index, long state)
{
}

/* lane S's 0x102b90 calls it (it takes the weapon in eax in retail) */

// @stub 0xa7cd0
void function_a7cd0(long weapon_index)
{
}
