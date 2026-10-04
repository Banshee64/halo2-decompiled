// @flags /O2 /Gr
/* UNKNOWN_155EA0.CPP: a lifecycle callback (entry 11, initialize): registers the Bink memory callbacks */

#include "unknown_11c920.h"
#include "globals.h"
#include <string.h>

void *__stdcall function_156710(unsigned long size);
void __stdcall function_156810(void *block);
int __stdcall BinkSetMemory(void *(__stdcall *allocate)(unsigned long), void (__stdcall *free)(void *));

// @retail 0x155ea0
void function_155ea0(void)
{
	memset(&g_4e9188, 0, sizeof(g_4e9188));
	BinkSetMemory(function_156710, function_156810);
	g_4e9188.initialized = true;
}
