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

struct s_1debd0
{
 word field_0;
 word field_2;
 byte field_4;
 byte field_5;
 short field_6;
};

// @retail 0x1debd0
bool function_1debd0(dword const *arg_1, s_bsp3d const *arg_0, short arg_2, long arg_3,
 short arg_4, bool arg_5, point2f const *arg_6)
{
 (void)&arg_2; (void)&arg_3; (void)&arg_4; (void)&arg_5; (void)&arg_6;
 point2f const *const *local_18 = &arg_6;
 s_1ddda0 const *local_0 = (s_1ddda0 const *)arg_0;
 s_1debd0 const *local_1 = &((s_1debd0 const *)local_0->field_2c)[arg_3];
 if (arg_1 && (local_1->field_4 & 8) && (short)local_1->field_5 < arg_2 &&
  !(arg_1[local_1->field_5 >> 5] & (1 << (local_1->field_5 & 31))))
  return false;
 s_1ddda2 const *local_2 = local_0->field_34;
 s_1ddda3 const *local_3 = local_0->field_3c;
 long local_4 = local_1->field_2;
 long local_5 = local_4;
 short const *local_6 = g_440b94[arg_4 * 2 + arg_5];
 long local_7 = local_6[0] * sizeof(real), local_8 = local_6[1] * sizeof(real);
 do
 {
  s_1ddda2 const *local_9 = &local_2[local_5];
  bool local_10 = local_9->field_a == arg_3;
  real const *local_11 = (real const *)&local_3[local_9->field_0[local_10]].field_0;
  point2f local_12 = { *(real const *)((byte const *)local_11 + local_7), *(real const *)((byte const *)local_11 + local_8) };
  point2f local_14 = { (*local_18)->x - local_12.x, (*local_18)->y - local_12.y };
  real const *local_16 = (real const *)&local_3[local_9->field_0[!local_10]].field_0;
  point2f local_17 = { *(real const *)((byte const *)local_16 + local_7) - local_12.x, *(real const *)((byte const *)local_16 + local_8) - local_12.y };
  if (local_17.y * local_14.x - local_14.y * local_17.x > 0.0f) return false;
  local_5 = local_9->field_4[local_10];
 } while (local_5 != local_4);
 return true;
}
