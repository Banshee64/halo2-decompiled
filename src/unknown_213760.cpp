// @flags /O2 /Gr
/* UNKNOWN_213760.CPP: queue an asynchronous read from one of the cache files.
   The top two bits of a location pick the file. Decompiled by lane F for
   0x12e3a0. */

#include "unknown_11c920.h"
#include <xtl.h>
#include "unknown_122870.h"

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
long function_213760(dword location, long size, void *buffer, dword *bytes_read, bool *done, long type, long priority)
{
	HANDLE file = cache_file_handle(cache_location_file_type(location));

	s_file_handle handle;

	handle.handle = file;
	return function_1a0f10(handle, buffer, size, location & 0x3fffffff, type, priority, bytes_read, done);
}

void map_file_path_get(char const *map_name, char *path);
bool cache_header_verify(s_cache_header const *header);

// @retail 0x213800
bool function_213800(char const *map_name, s_cache_header *header)
{
	char path[256];
	path[0] = 0;
	bool result = false;
	(void)&result;
	map_file_path_get(map_name, path);
	HANDLE file = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (file != INVALID_HANDLE_VALUE)
	{
		DWORD bytes_read;
		if (ReadFile(file, header, 0x800, &bytes_read, NULL) && bytes_read == 0x800 && cache_header_verify(header))
			result = true;
		CloseHandle(file);
	}
	return result;
}

struct s_cache_initial_file
{
	byte unknown00[0x24];
	char name[0x100];
	char build[0x78];
	char description[0x24];
	char path[0x804 - 0x1c0];
	s_cache_initial_file()
	{
		name[0] = 0;
		build[0] = 0;
		description[0] = 0;
		path[0] = 0;
	}
};

struct s_cache_initial_path
{
	char path[0x108];
	s_cache_initial_path() { path[0] = 0; }
};

struct s_cache_initial_state
{
	s_cache_initial_file files[6];
	long current;
	s_cache_initial_path paths[8];
	s_cache_initial_state();
};

// @retail 0x2135c0
s_cache_initial_state::s_cache_initial_state()
{
}
