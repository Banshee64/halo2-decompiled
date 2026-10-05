// @flags /O1 /Oi /Oy /arch:SSE /Gr
#include "unknown_11c920.h"
#include "globals.h"

struct s_510c4c;
extern s_510c4c *g_510c4c;
struct s_13a720_user
{
	real value00;
	byte field_4[0x50 - 4];
	long unknown50[6];
	long unknown68;
};

struct s_13a720_view
{
	byte field_0[0x1e];
	word field_1e;
	byte field_20[8];
	real field_28;
	real field_2c;
	byte field_30[0xad - 0x30];
	bool field_ad;
	bool field_ae;
};

struct s_interface_sound_block;
bool function_13cb40(void);
long function_14de70(long local_player_index);
bool function_15eb20(long player_index);
void function_22beb5(long local_player_index, dword state, s_interface_sound_block const *block, long *sounds, word *playing);

// @retail 0x13a720
void function_13a720(s_13a720_view const *data, long user_index)
{
	s_13a720_user *user = &((s_13a720_user *)g_510c4c)[user_index];
	dword flags = 0;
	if (!(data->field_1e & 4) && !(0.0f >= data->field_28) && !function_13cb40())
	{
		if (user->value00 != -1.0f && function_15eb20(function_14de70(user_index)))
		{
			flags = ((dword)((s_13a720_view const volatile *)data)->field_1e >> 9) & 1;
			if (user->value00 > data->field_2c)
				flags |= 2;
			else
				flags &= ~2;
			if (0.25f > data->field_2c && data->field_2c > 0.0f)
				flags |= 4;
			else
				flags &= ~4;
			if (data->field_2c == 0.0f)
				flags |= 8;
			else
				flags &= ~8;
			if (data->field_ad)
				flags |= 0x100;
			else
				flags &= ~0x100;
			if (data->field_ae)
				flags |= 0x200;
			else
				flags &= ~0x200;
		}
		if (g_4e6948->state == 2 && data->field_2c >= 1.0f)
			flags &= ~2;
	}
	function_22beb5(user_index, flags, (s_interface_sound_block const *)((byte const *)g_510c94 + 0x418), user->unknown50, (word *)&user->unknown68);
}
