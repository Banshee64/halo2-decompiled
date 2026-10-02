// @flags /O2 /Gr
/* UNKNOWN_155EA0.CPP: a lifecycle callback (entry 11, initialize): registers the Bink memory callbacks */

#include "cseries.h"
#include "globals.h"
#include <string.h>

void *__stdcall bink_memory_allocate(unsigned long size);
void __stdcall bink_memory_free(void *block);
int __stdcall BinkSetMemory(void *(__stdcall *allocate)(unsigned long), void (__stdcall *free)(void *));

// @retail 0x155ea0
void function_155ea0(void)
{
	memset(&g_4e9188, 0, sizeof(g_4e9188));
	BinkSetMemory(bink_memory_allocate, bink_memory_free);
	g_4e9188.initialized = true;
}
