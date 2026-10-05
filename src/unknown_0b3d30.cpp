// @flags /O2 /Gr
#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_053310.h"

dword g_547610;
long g_547614;
long g_547618;
byte g_54761c;
long g_547620;
byte g_546a8c[1];

void __stdcall function_b5e40(void *block);
void online_result_registration_clear(void);
void function_6b640(long task_index);
extern bool g_510580;
extern long g_510584;
extern dword g_5107d8;

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

// @retail 0xb4020
void function_b4020(void)
{
	g_54761c = 0;
	online_result_registration_clear();
	long task_index = g_510584;
	if (task_index != NONE)
	{
		function_6b640(task_index);
		g_510584 = NONE;
	}
	g_510580 = false;
}

// @retail 0xb4050
void function_b4050(void)
{
	g_547620++;
	long count = g_510580 ? g_5107d8 : 0;
	if (g_547620 >= 3 * count)
	{
		online_result_registration_clear();
		long task_index = g_510584;
		if (task_index != NONE)
		{
			function_6b640(task_index);
			g_510584 = NONE;
		}
		g_510580 = false;
		g_54761c = 0;
	}
}
