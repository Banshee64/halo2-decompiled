// the Bink library (third-party code)
#include "cseries.h"

// @stub 0x3e2820
int __stdcall BinkSetMemory(void *(__stdcall *allocate)(unsigned long), void (__stdcall *free)(void *))
{
	return 0;
}

/* called by bink_playback.cpp with 0 */
// @stub 0x18f1c0
void __stdcall function_18f1c0(long a) { }
/* Bink library functions called by bink_playback.cpp */
// @stub 0x3e2330
int __stdcall function_3e2330(void *movie) { return 0; }
// @stub 0x3e2870
int __stdcall function_3e2870(void *movie) { return 0; }
// @stub 0x3e2e50
void __stdcall function_3e2e50(void *movie) { }
// @stub 0x3e2830
int __stdcall function_3e2830(void *movie, void *destination, long pitch, long height, long x, long y, unsigned long flags) { return 0; }
