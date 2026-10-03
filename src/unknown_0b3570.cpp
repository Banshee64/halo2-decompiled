// @flags /O2 /Gr
/* UNKNOWN_0B3570.CPP: the reference counted start and stop of the session
   search list of unknown_0b35e0.cpp (its entries are allocated through the
   allocator given at the first start) (lane H) */

#include "cseries.h"
#include "data_array.h"

long g_4d8f04;
bool g_4d8f08;
long g_4d8f0c;
c_data_allocator *g_4d8f10;

bool function_b3610(void);
void function_b3670(void);

// @retail 0xb3570
bool function_b3570(long count, c_data_allocator *allocator, bool flag)
{
	bool result;

	if (g_4d8f04 <= 0)
	{
		g_4d8f0c = count;
		g_4d8f08 = flag;
		g_4d8f10 = allocator;
	}
	result = function_b3610();
	if (result)
		g_4d8f04++;
	return result;
}

// @retail 0xb35a0
void function_b35a0(void)
{
	g_4d8f04--;
	if (g_4d8f04 <= 0)
	{
		function_b3670();
		g_4d8f0c = 0;
		g_4d8f10 = NULL;
	}
}

// @retail 0xb35d0
void function_b35d0(bool start)
{
	if (start)
		function_b3610();
	else
		function_b3670();
}
