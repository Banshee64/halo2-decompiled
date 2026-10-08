#include "unknown_11c920.h"
#include "globals.h"

// @flags /O2 /arch:SSE /Gr

struct s_object;
s_object *function_badc0(long object_index, dword type_mask);
long function_baf80(long object_index);
bool function_e68c0(long type, long unit_index);
bool function_f12e0(long unit_index);
bool __stdcall function_14e200(long player_index, long target_index, point3f const *position, bool detach);

// @retail 0x14e970
bool function_14e970(long player_index, point3f const *position, long target_index)
{
	long unit_index = *(long *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c + 0x2c);
	s_object *unit = function_badc0(unit_index, 1);
	bool result = false;
	if (unit)
	{
		if (*(long *)((byte *)unit + 0x14) != NONE)
			function_e68c0(0x1e, unit_index);
		point3f adjusted;
		real offset = 0.03280000016093254f;
		adjusted.x = offset * g_4687b0->i + position->x;
		adjusted.y = offset * g_4687b0->j + position->y;
		adjusted.z = offset * g_4687b0->k + position->z;
		result = function_14e200(player_index, target_index, &adjusted, false);
	}
	return result;
}

// @retail 0x14e0f0
void __stdcall function_14e0f0(long player_index, long target_index, point3f const *position,
	point3f const *alternate)
{
	byte *player = g_4e8c24->data + (player_index & 0xffff) * 0x21c;
	long unit_index = *(long *)(player + 0x2c);
	if (unit_index != NONE)
	{
		byte *headers = g_4e0300->data;
		byte *unit = *(byte **)(headers + (unit_index & 0xffff) * 12 + 8);
		long parent = *(long *)(unit + 0x14);
		if (parent != NONE && function_baf80(parent) != function_baf80(target_index))
		{
			long root = function_baf80(unit_index);
			bool moved = false;
			if (alternate && (1 << headers[(root & 0xffff) * 12 + 3]) & 2)
				if (function_f12e0(root))
					moved = function_14e200(player_index, NONE, alternate, true);
			if (moved)
				return;
			function_e68c0(0x1e, *(long *)(player + 0x2c));
		}
		if (*(long *)(unit + 0x14) == NONE)
		{
			if (!function_14e200(player_index, target_index, position, true))
			{
				byte *state = (byte *)g_4e8c20;
				state[0x9e] = true;
				state[0x9f] = true;
				*(long *)(state + 0xa0) = 0;
			}
		}
	}
}
