// @flags /O2 /Gr
#include "cseries.h"
#include "unknown_053310.h"

long g_4e6420;
long g_4e642c[2];
long g_4e6440[2];

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
	long aligned_size;
	dword memory;

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

	aligned_size = (size + 0xfff) & 0xfffff000;
	PHYSICAL_MEMORY_ALLOCATE(memory, aligned_size);
	g_547610 = memory;
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