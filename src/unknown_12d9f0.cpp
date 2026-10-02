// @flags /O2 /Gr
#include "cseries.h"
#include "unknown_053310.h"

struct s_physical_object
{
	byte unknown00[0x2c];
	long state;

	void method_13d8b0(long pages);
};

s_physical_object *g_4e649c;
dword g_4e6494;
long g_4e6498;

// @retail 0x12d9f0
void __stdcall function_12d9f0(long stage)
{
	long size;
	long pages;
	long bytes;
	long aligned_size;
	dword memory;

	if (g_4e649c)
	{
		switch (stage)
		{
		case 0:
		case 1:
		case 2:
			size = (stage > 1) ? 0x680000 : 0x700000;
			break;
		case 3:
			size = 0x700000;
			break;
		default:
			__assume(0);
		}

		pages = size / 4096;
		bytes = pages * 4096;
		aligned_size = (bytes + 0xfff) & 0xfffff000;
		PHYSICAL_MEMORY_ALLOCATE(memory, aligned_size);
		g_4e6494 = memory;
		g_4e6498 = bytes;
		g_4e649c->method_13d8b0(pages);
		g_4e649c->state = 2;
	}
}

// @retail 0x12dad0
void function_12dad0(void)
{
	if (g_4e649c)
	{
		g_4e649c->method_13d8b0(0);
		g_4e6494 = 0;
	}
}