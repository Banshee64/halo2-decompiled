// @flags /O2 /Gr
#include "cseries.h"
#include "unknown_053310.h"

struct s_physical_object
{
	void method_13d8b0(long pages);
};

s_physical_object *g_4e3b54;
dword g_4e3b50;
long g_4e3b58;
long g_4e3b5c;

// @retail 0x123230
void __stdcall function_123230(long stage)
{
	long size;
	long aligned_size;
	dword memory;

	if (g_4e3b54)
	{
		if (stage > 1)
		{
			size = 0x300000;
			g_4e3b5c = 0x600;
		}
		else
		{
			size = 0x400000;
			g_4e3b5c = 0x800;
		}

		aligned_size = (size + 0xfff) & 0xfffff000;
		g_4e3b58 = size;
		PHYSICAL_MEMORY_ALLOCATE(memory, aligned_size);
		g_4e3b50 = memory;
		g_4e3b54->method_13d8b0(g_4e3b5c);
	}
}

// @retail 0x1232e0
void function_1232e0(void)
{
	g_4e3b5c = 0;
	g_4e3b58 = 0;
	if (g_4e3b54)
	{
		g_4e3b54->method_13d8b0(0);
		g_4e3b50 = 0;
	}
}