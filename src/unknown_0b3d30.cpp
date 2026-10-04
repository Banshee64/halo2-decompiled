// @flags /O2 /Gr
#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_053310.h"

dword g_547610;
long g_547614;
long g_547618;
byte g_54761c;
byte g_546a8c[1];

void __stdcall function_b5e40(void *block);

// @retail 0xb3d30
void __stdcall function_b3d30(long stage)
{
	long size;

	if (stage > 1)
	{
		if (stage > 3)
		{
			return;
		}
		size = 0x400;
	}
	else
	{
		size = 0xa000;
	}

	g_547610 = (dword)physical_memory_malloc_fixed(size, PAGE_READWRITE);
	g_547614 = size;
}

// @retail 0xb3db0
void function_b3db0(void)
{
	if (g_547610)
	{
		function_b5e40(g_546a8c);
		g_547610 = 0;
		g_547614 = 0;
		g_547618 = 0;
		g_54761c = 0;
	}
}