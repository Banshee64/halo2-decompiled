// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "unknown_2accd0.h"
#include <string.h>

struct s_profile_file_location
{
	char name[0x14];
	wchar_t display_name[0x12];
	long type;
	byte unknown3c;
	char language;
	byte unknown3e[2];
};

struct s_cached_profile_files
{
	long count;
	struct
	{
		long unknown0;
		byte profile[0x1e0];
	} entries[1];
};

extern void *g_51ea14;
extern long g_55c154;

void function_125d60(void);
bool function_216800(void *location, long file_index);
const char *function_216b60(long type);
char *function_122810(char *path, const char *suffix);
bool saved_game_file_read_begin(void *buffer, dword size, bool non_roamable, s_saved_game_file_task *task, const char *path);
bool function_2162b0(long file_index, void *buffer, long size, s_saved_game_file_task *task);

// @retail 0x2161d0
bool function_2161d0(long file_index, void *buffer, long size)
{
	bool result = false;
	s_saved_game_file_task task;

	if (function_2162b0(file_index, buffer, size, &task))
	{
		if (!*(volatile bool *)&task.done)
		{
			while (!*(volatile bool *)&task.done)
			{
				SwitchToThread();
				function_125d60();
			}
		}
		result = task.succeeded;
		if (!result)
			g_55c154 = task.state;
	}
	return result;
}

// @retail 0x2162b0
bool function_2162b0(long file_index, void *buffer, long size, s_saved_game_file_task *task)
{
	bool result = false;

	if ((bool)(((dword)file_index >> 21) & 1) && g_51ea14)
	{
		s_cached_profile_files *files = (s_cached_profile_files *)g_51ea14;
		long index = (file_index >> 8) & 0x1fff;
		long bounded_index = index < 0 ? 0 : index > files->count - 1 ? files->count - 1 : index;
		if (bounded_index == index && (dword)size <= 0x1e0)
		{
			memcpy(buffer, files->entries[index].profile, size);
			task->done = true;
			task->succeeded = true;
			task->state = 0;
			task->progress = 1.0f;
			result = true;
		}
	}
	else
	{
		s_profile_file_location location;
		char path[0x100];
		location.name[0] = 0;
		location.display_name[0] = 0;
		path[0] = 0;
		if (function_216800(&location, file_index) && location.language != -1)
		{
			long type = location.type;
			strncpy(path, location.name, sizeof(path));
			path[sizeof(path) - 1] = 0;
			function_122810(path, function_216b60(type));
			result = saved_game_file_read_begin(buffer, size, false, task, path);
		}
	}
	return result;
}
