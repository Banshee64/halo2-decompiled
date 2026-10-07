#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_2551c0.h"
// @flags /O2 /Ob0 /Gr

struct s_28e320
{
	byte field_0[4];
	long field_4;
	byte field_8[0xc];
	short field_14;
	byte field_16[2];
	long field_18;
	byte field_1c[0x18];
};

struct s_28e324
{
	long field_0;
	long field_4;
	long field_8;
	long field_c;
};

void function_118fe0(long arg_0, bool arg_1);

// @retail 0x28e320
bool function_28e320(long arg_0, long arg_1)
{
	byte *local_4 = g_5044c8->data;
	long local_5 = (arg_0 & 0xffff) * sizeof(s_28e320);
	s_28e320 *local_0 = (s_28e320 *)(local_4 + local_5);
	long local_6 = arg_1 & 0xffff;
	s_object_header_view *local_7 = (s_object_header_view *)g_4e0300->data;
	s_handler_object_view *local_1 = (s_handler_object_view *)local_7[local_6].object;
	bool local_2 = false;
	s_28e324 *local_3;
	if (!local_1->flags134 && (local_3 = (s_28e324 *)((byte *)local_1 + local_1->ai_offset)) != NULL && local_3->field_8 == NONE)
	{
		local_3->field_c = local_0->field_18;
		local_0->field_18 = arg_1;
		local_3->field_8 = arg_0;
		local_3->field_4 = local_0->field_4;
		function_118fe0(arg_1, true);
		local_0->field_14++;
		local_2 = true;
	}
	return local_2;
}
