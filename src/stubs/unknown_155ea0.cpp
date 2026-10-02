// stubs for the Bink memory callbacks that 155ea0 registers (156710 and
// 156810 are game functions that are not decompiled yet) and for BinkSetMemory
#include "cseries.h"

// @stub 0x156710
void *__stdcall bink_memory_allocate(unsigned long size)
{
	return 0;
}

// @stub 0x156810
void __stdcall bink_memory_free(void *block)
{
}

// @stub 0x3e2820
int __stdcall BinkSetMemory(void *(__stdcall *allocate)(unsigned long), void (__stdcall *free)(void *))
{
	return 0;
}
