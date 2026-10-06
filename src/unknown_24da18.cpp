#include "unknown_11c920.h"
#include "globals.h"

// @flags /O1 /Gr

struct s_collision_result_1697c0;
bool function_14ddc0(long local_player_index);
long function_14de70(long local_player_index);
long function_baf80(long object_index);
bool function_16a040(long flags, point3f const *point0, point3f const *point1,
	long ignore_object_index, long ignore_unit_index, s_collision_result_1697c0 *result);

struct s_result_24da18
{
	byte field_00[0x24];
	short field_24;
	byte field_26[0x5c - 0x26];
};

// @retail 0x24da18
long function_24da18(point3f const *point0, point3f const *point1,
	long ignore_unit_index, long local_player_index)
{
	s_result_24da18 result;
	result.field_24 = NONE;
	long object_index;
	if (function_14ddc0(local_player_index))
	{
		long player_index = function_14de70(local_player_index);
		object_index = *(long *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c + 0x2c);
	}
	else
		object_index = NONE;
	if (object_index != NONE)
		object_index = function_baf80(object_index);
	return function_16a040(0x16808c2d, point0, point1, object_index, ignore_unit_index,
		(s_collision_result_1697c0 *)&result) ? 2 : 0;
}
