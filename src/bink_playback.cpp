// @flags /O2 /Ob1 /Gr
/* BINK_PLAYBACK.CPP: Bink movie playback

The functions follow bink_playback.obj in Bungie's May 2003 debug builds
(halo-symbol-atlas). Bink allocates through bink_memory_allocate and
bink_memory_free, which bink_playback_initialize (unknown_155ea0.cpp)
registers: the allocations come from a permanent block of physical memory
and are tracked in g_4e9148. */

#include "cseries.h"
#include "globals.h"
#include "unknown_03d380.h"
#include <xtl.h>
#include <string.h>

enum
{
	k_maximum_bink_allocations = 16,
	k_bink_allocation_slack = 0x3000
};

/* the outstanding Bink allocations and their count */
void *g_4e9148[k_maximum_bink_allocations];
long g_510c64;
/* the available physical memory, in kilobytes */
long g_47ff80 = NONE;
dword g_51ebec;
extern dword g_54d5b8;
extern byte g_4e6388;

void __stdcall function_18f1c0(long a);
void function_12d520(long a);

/* the shared body of bink_get_memory_available, which retail inlines into
   the memory callbacks */
PRIVATE inline void bink_update_memory_available(void)
{
	MEMORYSTATUS status;

	memset(&status, 0, sizeof(status));
	status.dwLength = sizeof(status);
	GlobalMemoryStatus(&status);
	g_47ff80 = status.dwAvailPhys >> 10;
}

// @retail 0x155e50
void bink_get_memory_available(void)
{
	bink_update_memory_available();
}

// @retail 0x155f60
bool bink_playback_active(void)
{
	return g_4e9188.movie && g_4e9188.initialized;
}

// @retail 0x1565e0
void function_1565e0(void)
{
	if (g_4e9188.initialized && g_4e9188.permanent_memory)
	{
		function_12d520((long)g_4e9188.permanent_memory);
		g_4e9188.permanent_memory = NULL;
		g_4e9188.unknown18 = 0;
		g_4e9188.permanent_memory_size = 0;
	}
}

// @retail 0x156560
void bink_playback_stop(void)
{
	if (g_4e9188.initialized)
	{
		if (g_4e9188.movie)
		{
			function_3e2ff0(g_4e9188.movie);
			g_4e9188.movie = NULL;
		}
		function_1565e0();
		if (g_4e9188.flag1)
		{
			g_4e9188.flag1 = false;
			g_4e6388 = 0;
		}
	}
}

/* a lifecycle callback with the body of bink_playback_stop */
// @retail 0x1566c0
void __stdcall function_1566c0(dword a, dword b)
{
	if (g_4e9188.initialized)
	{
		if (g_4e9188.movie)
		{
			function_3e2ff0(g_4e9188.movie);
			g_4e9188.movie = NULL;
		}
		function_1565e0();
		if (g_4e9188.flag1)
		{
			g_4e9188.flag1 = false;
			g_4e6388 = 0;
		}
	}
}

// @retail 0x1565a0
void bink_playback_end(void)
{
	if (g_4e9188.initialized)
	{
		bink_playback_stop();
		if (g_4e9188.flags & 0x20)
			function_18f1c0(0);
		g_4e9188.flags = 0;
		g_51ebec = g_54d5b8;
	}
}

// @retail 0x155ed0
void bink_playback_dispose(void)
{
	if (g_4e9188.initialized)
	{
		bink_playback_stop();
		if (g_4e9188.flags & 0x20)
			function_18f1c0(0);
		g_51ebec = g_54d5b8;
		memset(&g_4e9188, 0, sizeof(g_4e9188));
	}
}

// @retail 0x156620
void *bink_alloc_permanent(long size, long alignment)
{
	byte *result = g_4e9188.permanent_memory + g_4e9188.permanent_memory_size - size;

	if (alignment)
	{
		long mask = alignment - 1;

		if ((long)result & mask)
		{
			size += (alignment - (long)result) & mask;
			result -= (alignment - (long)result) & (alignment - 1);
		}
	}
	g_4e9188.permanent_memory_size -= size;
	return result;
}

// @retail 0x156690
bool is_all_bink_memory_free(void)
{
	bool result = true;

	for (short i = 0; i < g_510c64; i++)
	{
		if (g_4e9148[i])
			result = false;
	}
	return result;
}

// @retail 0x156710
void *__stdcall bink_memory_allocate(unsigned long size)
{
	void *result = NULL;

	bink_update_memory_available();

	if (g_510c64 > 0 && !g_4e9148[0] && is_all_bink_memory_free())
	{
		g_510c64 = 0;
		g_4e9188.permanent_memory_used = 0;
	}

	if (g_4e9188.permanent_memory_used + size <= (unsigned long)g_4e9188.permanent_memory_size &&
		g_510c64 < k_maximum_bink_allocations && g_4e9188.permanent_memory)
	{
		result = g_4e9188.permanent_memory + g_4e9188.permanent_memory_used;
		g_4e9188.permanent_memory_used += size;
		XPhysicalProtect(result, size, PAGE_READWRITE);
		g_4e9148[g_510c64++] = result;
		bink_get_memory_available();
		g_4e9188.permanent_memory_used += k_bink_allocation_slack;
		if (g_4e9188.permanent_memory_used > g_4e9188.permanent_memory_size)
			g_4e9188.permanent_memory_used = g_4e9188.permanent_memory_size;
	}
	return result;
}

// @retail 0x156810
void __stdcall bink_memory_free(void *block)
{
	bink_update_memory_available();
	for (long i = 0; i < g_510c64; i++)
	{
		if (g_4e9148[i] == block)
		{
			g_4e9148[i] = NULL;
			break;
		}
	}
	bink_update_memory_available();
}
