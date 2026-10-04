// @flags /O2 /Gr
#include "unknown_11c920.h"

struct s_47f0d0
{
	byte unknown0[0x14];
	void *field14;
};

s_47f0d0 g_47f0d0;

void function_2ae1d0(s_47f0d0 *p);

// @retail 0x21ebc0
void __stdcall function_21ebc0(long stage)
{
	if (stage == 1)
	{
		function_2ae1d0(&g_47f0d0);
	}
}

// @retail 0x21ebe0
void function_21ebe0(void)
{
	if (g_47f0d0.field14)
	{
		g_47f0d0.field14 = 0;
	}
}