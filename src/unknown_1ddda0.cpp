// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "unknown_0259d0.h"

struct s_bsp3d;

struct s_1ddda1
{
	word field_0;
	word field_2;
	long field_4;
};

struct s_1ddda2
{
	word field_0[2];
	word field_4[2];
	short field_8;
	short field_a;
};

struct s_1ddda3
{
	point3f field_0;
	long field_c;
};

struct s_1ddda0
{
	byte field_0[0x2c];
	s_1ddda1 *field_2c;
	long field_30;
	s_1ddda2 *field_34;
	long field_38;
	s_1ddda3 *field_3c;
};

// @retail 0x1ddda0
short function_1ddda0(s_bsp3d const *arg_0, long arg_1, point3f *arg_2)
{
	(void)&arg_1;
	(void)&arg_2;
	short local_3 = 0;
	s_1ddda0 const *local_0 = (s_1ddda0 const *)arg_0;
	long local_1 = local_0->field_2c[arg_1].field_2;
	long local_2 = local_1;
	do
	{
		s_1ddda2 const *local_4 = &local_0->field_34[local_2];
		bool local_5 = local_4->field_a == arg_1;
		arg_2[local_3++] = local_0->field_3c[local_4->field_0[local_5]].field_0;
		local_2 = local_4->field_4[local_5];
	} while (local_2 != local_1);
	return local_3;
}
