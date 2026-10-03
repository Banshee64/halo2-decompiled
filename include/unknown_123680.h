/* UNKNOWN_123680.H: the cache of streamed tag resources (src/unknown_123680.cpp,
   0x123310..0x123b00). A resource (0x14 bytes, the animation graph's
   resources are some) is loaded into a block of the physical memory cache
   g_4e3b54; a request to load it waits in the data array g_4e3b4c. */

#ifndef UNKNOWN_123680_H
#define UNKNOWN_123680_H

#include "cseries.h"

struct s_cache_resource
{
	long unknown00;
	long size;
	long unknown08;
	long block_index;
	bool streamed;
	byte unknown11[3];
};

/* whether resources are streamed (g_510c21) and whether the animation code
   requests them (g_510c20, only read together with g_510c21) */
extern bool g_510c20;
extern bool g_510c21;

long function_123680(s_cache_resource *resource);
void function_1236f0(s_cache_resource *resource, bool urgent);
void *function_1237e0(s_cache_resource *resource, long name);

#endif
