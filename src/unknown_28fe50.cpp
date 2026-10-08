// @flags /O2 /Gr /arch:SSE
#include "unknown_2551c0.h"
#include "globals.h"

struct s_28fe50
{
	byte field_0[0xc];
	bool field_c;
	byte field_d[0x30 - 0xd];
	dword field_30 : 16;
	dword : 16;
	byte field_34[0x7c - 0x34];
	long field_7c;
	byte field_80[0x338 - 0x80];
	long field_338;
	byte field_33c[0x888 - 0x33c];
};

struct s_28fe51
{
	byte field_0[3];
	byte field_3;
	byte field_4[4];
	s_handler_object_view *field_8;
};

struct s_28fe52
{
	byte field_0[4];
	long field_4;
	long field_8;
	byte field_c[0x48];
	short field_54;
};

struct s_28fe53
{
	byte field_0[0x10a];
	word : 2;
	word field_10a : 1;
	word : 13;
};

__forceinline s_handler_object_view *function_28fe51(long arg_0)
{
	return handler_object_get(arg_0);
}

__forceinline s_28fe52 *function_28fe52(s_handler_object_view *arg_0)
{
	s_28fe52 *local_0;
	if (!arg_0->flags134)
		local_0 = (s_28fe52 *)((byte *)arg_0 + arg_0->ai_offset);
	else
		local_0 = NULL;
	return local_0;
}

long function_1e0dc0(short arg_0, long arg_1, short arg_2, short arg_3);
void function_269250(long arg_0, long arg_1);
bool __stdcall function_25d020(long arg_0, long arg_1, long arg_2, long arg_3, long arg_4, long arg_5);
bool __stdcall function_28e390(long arg_0, long arg_1, bool arg_2, bool arg_3);

// @retail 0x28fe50
void __stdcall function_28fe50(long arg_0)
{
	s_handler_object_view *local_0 = function_28fe51(arg_0);
	s_28fe52 *local_1 = function_28fe52(local_0);
	if (local_1)
	{
		long local_2 = *(long *)((byte *)local_0 + 0x14);
		if (local_2 != NONE)
		{
			s_28fe51 *local_3 = &((s_28fe51 *)g_4e0300->data)[local_2 & 0xffff];
			if (local_3->field_3 == 0)
			{
				short local_5 = NONE;
				s_handler_object_view *local_8 = local_3->field_8;
				s_28fe50 *local_4 = NULL;
				if (local_1->field_4 != NONE)
				{
					local_4 = &((s_28fe50 *)g_4f55f0->data)[local_1->field_4 & 0xffff];
					local_5 = (short)local_4->field_30;
				}
				if (TEST_FIELD_BIT(((s_28fe53 *)local_8)->field_10a))
				{
					long local_6 = function_1e0dc0(*(short *)((byte *)local_0 + 0x12e), local_2, local_1->field_54, (short)local_5);
					if (local_4)
					{
						if (local_4->field_7c != NONE && local_6 != NONE)
						{
							function_269250(local_4->field_7c, local_6);
							if (local_4->field_338 != NONE)
							{
								long local_7 = *(long *)(g_502418->data + (local_4->field_338 & 0xffff) * 0x3c + 0x20);
								function_25d020(local_6, local_7, NONE, NONE, local_1->field_4, local_4->field_338);
							}
						}
						if (local_4->field_c && local_6 != NONE)
							((s_28fe50 *)g_4f55f0->data)[local_6 & 0xffff].field_c = true;
					}
					if (local_1->field_8 != NONE)
						function_28e390(local_1->field_8, arg_0, true, true);
				}
			}
		}
	}
}
