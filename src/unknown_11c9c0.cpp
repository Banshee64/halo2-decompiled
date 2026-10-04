// @flags /O2 /Gr
/* UNKNOWN_11C9C0.CPP: bounded text formatting, virtual memory wrappers */

#include "unknown_11c920.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <xtl.h>

// @retail 0x11c9c0
char *function_11c9c0(char *buffer, long maximum_count, const char *format, ...)
{
	va_list arguments;
	va_start(arguments, format);
	_vsnprintf(buffer, maximum_count - 1, format, arguments);
	buffer[maximum_count - 1] = 0;
	return buffer;
}

// @retail 0x11c9e0
char *function_11c9e0(char *buffer, long maximum_count, const char *format, ...)
{
	va_list arguments;
	va_start(arguments, format);
	long length = (long)strlen(buffer);
	long remaining = maximum_count - length;
	char *end = buffer + length;
	_vsnprintf(end, remaining - 1, format, arguments);
	end[remaining - 1] = 0;
	return buffer;
}

// @retail 0x11ca20
void * __stdcall function_11ca20(void *address)
{
	void *memory = VirtualAlloc(NULL, (SIZE_T)address, 0x101000, PAGE_READWRITE);
	if (!memory)
	{
		GetLastError();
	}
	return memory;
}

// @retail 0x11ca50
void __stdcall function_11ca50(void *address)
{
	if (!VirtualFree(address, 0, MEM_RELEASE))
	{
		GetLastError();
	}
}

// @retail 0x11ca70
long __stdcall function_11ca70(long unused)
{
	return 0;
}

void __stdcall function_72c70(dword value);

struct s_memory_callbacks
{
	long (__stdcall *function0)(long);
	void (__stdcall *function1)(dword);
	void *(__stdcall *allocate)(void *);
	void (__stdcall *free)(void *);
};

// retail .rdata 0x453300
const s_memory_callbacks g_453300 =
{
	function_11ca70,
	function_72c70,
	function_11ca20,
	function_11ca50,
};

// @retail 0x11ca80
long function_11ca80(long value)
{
	switch (value)
	{
		case 1: return 0;
		case 2: return 1;
		case 3: return 2;
		case 4: return 3;
		case 5: return 4;
		case 6: return 5;
		case 7: return 6;
		case 8: return 7;
		case 9: return 0;
		default: return 0;
	}
}
