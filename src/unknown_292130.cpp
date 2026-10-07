#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include <math.h>
// @flags /O2 /arch:SSE /Gr

struct s_292130
{
	real field_0;
	vector3f field_4;
	vector3f field_10;
};

real g_55e5b0;
real g_55e5b4[2];
real const g_44af3c[9] = { 0.0f, 0.5f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f };
real const g_44af60[9] = { 0.0f, 0.3f, 0.3f, 0.3f, 0.3f, 1.0f, 1.0f, 1.0f, 1.0f };
real const g_44af84[9] = { 0.0f, 0.0f, 1.5707963705062866f, 3.1415927410125732f, 4.7123889923095703f, 0.0f, 1.5707963705062866f, 3.1415927410125732f, 4.7123889923095703f };
real const g_44afac[8] = { 0.0f, 0.7853981852531433f, 1.5707963705062866f, 2.3561944961547852f, 3.1415927410125732f, 3.9269909858703613f, 4.7123889923095703f, 5.4977874755859375f };
real const g_44afcc[2] = { 0.7f, 1.0f };
s_292130 g_5046f8[9];
s_292130 g_5044d8[8][2];
vector3f g_504698[8];

__forceinline void function_292220(real arg_0, vector3f const *arg_1, vector3f *arg_2)
{
	arg_2->i = arg_1->i * arg_0;
	arg_2->j = arg_1->j * arg_0;
	arg_2->k = arg_1->k * arg_0;
}

// @retail 0x292130
void function_292130(void)
{
	long local_0 = 0;
	long local_9 = 9;
	do
	{
		real local_1 = (real)sin(g_44af84[local_0]);
		real local_2 = (real)cos(g_44af84[local_0]);
		real local_3 = g_55e5b0 * g_44af60[local_0];
		real local_4 = (real)sin(local_3);
		g_5046f8[local_0].field_0 = 1.0f;
		g_5046f8[local_0].field_4.i = 0.0f;
		g_5046f8[local_0].field_4.j = local_2 * g_44af3c[local_0] * 0.7f;
		g_5046f8[local_0].field_4.k = local_1 * g_44af3c[local_0] * 0.7f;
		g_5046f8[local_0].field_10.i = (real)cos(local_3);
		g_5046f8[local_0].field_10.j = local_4 * local_2;
		g_5046f8[local_0].field_10.k = local_4 * local_1;
		local_0++;
	} while (--local_9);
	long local_5 = 0;
	long local_10 = 2;
	do
	{
		real local_6 = (real)sin(g_55e5b4[local_5]);
		real local_7 = (real)cos(g_55e5b4[local_5]);
		long local_8 = 0;
		long local_11 = 8;
		do
		{
			g_504698[local_8].i = 0.0f;
			g_504698[local_8].j = (real)cos(g_44afac[local_8]);
			g_504698[local_8].k = (real)sin(g_44afac[local_8]);
			g_5044d8[local_8][local_5].field_0 = 0.7f;
			function_292220(g_44afcc[local_5], &g_504698[local_8], &g_5044d8[local_8][local_5].field_4);
			function_292220(local_6, &g_504698[local_8], &g_5044d8[local_8][local_5].field_10);
			g_5044d8[local_8][local_5].field_10.i = local_7;
			local_8++;
		} while (--local_11);
		local_5++;
	} while (--local_10);
}
