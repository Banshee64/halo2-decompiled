#include "unknown_11c920.h"
#include "unknown_0259d0.h"
// @flags /O2 /arch:SSE /Gr

struct s_1362b0
{
	dword field_0;
	short field_4;
	short field_6;
	byte field_8[2];
	short field_a;
	short field_c;
	word field_e;
	byte field_10[4];
	short field_14;
	byte field_16[0x34 - 0x16];
	long field_34;
	byte field_38[0x54 - 0x38];
	byte *field_54;
};

struct s_type_7ba8e9;
void *function_135a30(s_type_7ba8e9 const *arg_1, short arg_2, short arg_3, short arg_4);
void *function_135af0(s_type_7ba8e9 const *arg_1, short arg_2, short arg_3, short arg_4, short arg_5);
void *function_135c00(s_type_7ba8e9 const *arg_1, short arg_2, short arg_3, short arg_4, short arg_5);
dword function_136140(byte const *arg_1, long arg_2, byte const *arg_3, short arg_4, short arg_5, short arg_6, word arg_7, short arg_8, short arg_9);

PRIVATE __forceinline long function_1362b1(real arg_1)
{
	long local_1;
	__asm
	{
		fld arg_1
		fistp local_1
	}
	return local_1;
}

// @retail 0x1362b0
dword function_1362b0(s_1362b0 const *arg_1, point2f const *arg_2, real arg_3)
{
	if (arg_1->field_54)
	{
		short local_1;
		if (arg_3 < 1.0f && arg_1->field_14 > 0)
			local_1 = (short)function_1362b1((1.0f - arg_3) * arg_1->field_14);
		else
			local_1 = 0;
		short local_2 = (short)(arg_1->field_4 >> local_1) > 1 ? arg_1->field_4 >> local_1 : 1;
		if (arg_1->field_e & 2)
			local_2 += (long)(byte)-local_2 & 3;
		short local_3 = (short)(arg_1->field_6 >> local_1) > 1 ? arg_1->field_6 >> local_1 : 1;
		if (arg_1->field_e & 2)
			local_3 += (long)(byte)-local_3 & 3;
		long local_4;
		if (!(local_2 & (local_2 - 1)))
			local_4 = function_1362b1(local_2 * arg_2->x - 0.5f) & (local_2 - 1);
		else
			local_4 = (function_1362b1(local_2 * arg_2->x - 0.5f) % local_2 + local_2) % local_2;
		long local_5;
		if (!(local_3 & (local_3 - 1)))
			local_5 = function_1362b1(local_3 * arg_2->y - 0.5f) & (local_3 - 1);
		else
			local_5 = (function_1362b1(local_3 * arg_2->y - 0.5f) % local_3 + local_3) % local_3;
		void *local_6;
		switch (arg_1->field_a)
		{
		case 0:
			local_6 = function_135a30((s_type_7ba8e9 const *)arg_1, local_1, 0, 0);
			break;
		case 1:
			local_6 = function_135af0((s_type_7ba8e9 const *)arg_1, 0, 0, 0, local_1);
			break;
		default:
			local_6 = function_135c00((s_type_7ba8e9 const *)arg_1, 0, 0, 0, local_1);
			break;
		}
		return function_136140(arg_1->field_54, arg_1->field_34, (byte const *)local_6,
			(short)local_4, local_3, arg_1->field_c, arg_1->field_e, local_2, (short)local_5);
	}
	return 0xffffffff;
}
