// stubs for the library code called by the page heap at 0x1a1a00
#include "unknown_1a1940.h"

static volatile long stub_state;

// @stub 0x329e10
void __fastcall function_329e10(void *heap)
{
	stub_state = 1;
}

// @stub 0x321379
void __cdecl function_321379(void *memory)
{
	stub_state = 2;
}
