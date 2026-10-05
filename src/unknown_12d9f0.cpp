// @flags /O2 /Gr
#include "unknown_11c920.h"
#include "unknown_053310.h"
#include "physical_memory.h"

s_physical_object *g_4e649c;
dword g_4e6494;
long g_4e6498;

// @retail 0x12d9f0
void __stdcall function_12d9f0(long stage)
{
	long size;
	long pages;
	long bytes;

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
		g_4e6494 = (dword)physical_memory_malloc_fixed(bytes, PAGE_READWRITE);
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