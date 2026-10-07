// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "globals.h"

struct s_18f0a0
{
	long field_0;
	byte field_4[0x18 - 4];
	long field_18;
	byte field_1c[0x12c - 0x1c];
	bool field_12c;
};

struct s_18f0a1
{
	dword field_0;
	byte field_4[0x1f8 - 4];
	long field_1f8;
	byte field_1fc[0xc70 - 0x1fc];
};

extern bool g_54e7cc;
void function_19040d(long arg_0);
long player_slot_get_single_profile(void);
void function_1240b0(long arg_0);

// @retail 0x18f0a0
void __stdcall function_18f0a0(s_18f0a0 const *arg_0)
{
	s_18f0a0 const *const *local_3 = &arg_0;
	arg_0 = *local_3;
	if (arg_0->field_0 == 1 && arg_0->field_18 != NONE)
		function_19040d(arg_0->field_18);
	if (!arg_0->field_12c && g_54e7cc)
	{
		long local_0 = NONE;
		long local_1 = player_slot_get_single_profile();
		if (local_1 != NONE)
		{
			s_18f0a1 *local_2 = (s_18f0a1 *)g_54e8e0 + local_1;
			if (local_2->field_0 & 0x10)
				local_0 = local_2->field_1f8;
		}
		function_1240b0(local_0);
	}
}
