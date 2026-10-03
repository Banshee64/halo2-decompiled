// @flags /O2 /Gr
/* UNKNOWN_213760.CPP: queue an asynchronous read from one of the cache files.
   The top two bits of a location pick the file. Decompiled by lane F for
   0x12e3a0. */

#include "cseries.h"
#include <xtl.h>

/* the open cache files (0x804 bytes each) and the current one */
struct s_cache_file
{
	HANDLE handle;
	byte unknown04[0x800];
};

s_cache_file g_557c90[3];
long g_55aca8;

#include "async.h"

static inline long cache_location_file_type(dword location)
{
	long type = NONE;

	switch (location >> 30)
	{
	case 0:
		type = NONE;
		break;
	case 1:
		type = 0;
		break;
	case 2:
		type = 1;
		break;
	case 3:
		type = 2;
		break;
	}
	return type;
}

static inline HANDLE cache_file_handle(long type)
{
	HANDLE handle = g_557c90[g_55aca8].handle;

	switch (type)
	{
	case 0:
		handle = g_557c90[2].handle;
		break;
	case 1:
		handle = g_557c90[0].handle;
		break;
	case 2:
		handle = g_557c90[1].handle;
		break;
	}
	return handle;
}

// @retail 0x213760
long function_213760(dword location, long size, void *buffer, long unknown, bool *done, long type, long priority)
{
	HANDLE file = cache_file_handle(cache_location_file_type(location));

	s_file_handle handle;

	handle.handle = file;
	return async_read_position(handle, buffer, size, location & 0x3fffffff, type, priority, (dword *)unknown, done);
}
