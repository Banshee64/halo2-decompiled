// @flags /O2 /Gr
/* UNKNOWN_136FC0.CPP: file enumeration (files_windows.obj) */

#include "cseries.h"
#include <string.h>
#include <xtl.h>

struct find_file_data
{
	dword flags;
	short depth;
	short unknown06;
	char path[256];
	char root[256];
	HANDLE handles[16];
	WIN32_FIND_DATA data;
};

struct file_reference_data
{
	dword signature;
	byte flags;
	byte unknown05;
	word unknown06;
	char path[256];
	byte unknown108[8];
};

struct s_file_time
{
	dword low;
	dword high;
};

void function_1374c0(char *dest, const char *path);
void file_path_add_name(char *path, const char *name);
void file_path_remove_name(char *path);

inline void find_files_path_append(char *path, const char *name)
{
	if (*name)
	{
		size_t length = strlen(path);
		char *end = path + length;
		if (end != path && end[-1] != '\\')
		{
			*end++ = '\\';
			*end = 0;
			length++;
		}
		strncpy(end, name, 256 - length);
		path[255] = 0;
	}
}

inline void find_files_path_truncate(char *path)
{
	short index = (short)strlen(path);
	while (index > 0 && path[index] != '\\')
	{
		index--;
	}
	path[index] = 0;
}

// @retail 0x136fc0
void find_files_end(find_file_data *find)
{
	short depth = find->depth;
	while (depth >= 0)
	{
		if (find->handles[depth] != INVALID_HANDLE_VALUE)
		{
			CloseHandle(find->handles[depth]);
			find->handles[depth] = INVALID_HANDLE_VALUE;
		}
		depth--;
	}
}

// @retail 0x137000
bool function_137000(find_file_data *find, file_reference_data *file, s_file_time *time)
{
	char buffer[256] = {0};
	short depth = find->depth;
	char *path = find->path;

	while (depth >= 0)
	{
		HANDLE *handle = &find->handles[depth];
		if (*handle == INVALID_HANDLE_VALUE)
		{
			function_1374c0(buffer, path);
			if (find->root[0])
			{
				find_files_path_append(buffer, find->root);
			}
			*handle = FindFirstFileA(buffer, &find->data);
			if (*handle == INVALID_HANDLE_VALUE)
			{
				find_files_path_truncate(path);
				depth--;
				continue;
			}
		}
		else if (!FindNextFileA(*handle, &find->data))
		{
			CloseHandle(*handle);
			*handle = INVALID_HANDLE_VALUE;
			find_files_path_truncate(path);
			depth--;
			continue;
		}

		if (find->data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (!strcmp(find->data.cFileName, ".") || !strcmp(find->data.cFileName, ".."))
			{
				continue;
			}
			bool descended = false;
			if (find->flags & 1)
			{
				file_path_add_name(path, find->data.cFileName);
				depth++;
				descended = true;
			}
			if (find->flags & 6)
			{
				word unknown06 = find->unknown06;
				memset(file, 0, sizeof(*file));
				file->signature = 'filo';
				file->unknown06 = unknown06;
				file_path_add_name(file->path, path);
				if (!descended)
				{
					file_path_add_name(file->path, find->data.cFileName);
				}
				if (time)
				{
					time->low = find->data.ftLastWriteTime.dwLowDateTime;
					time->high = find->data.ftLastWriteTime.dwHighDateTime;
				}
				find->depth = depth;
				return true;
			}
		}
		else if ((find->flags & 4) || !(find->flags & 2))
		{
			word unknown06 = find->unknown06;
			memset(file, 0, sizeof(*file));
			file->signature = 'filo';
			file->unknown06 = unknown06;
			file_path_add_name(file->path, path);
			if (file->flags & 1)
			{
				file_path_remove_name(file->path);
			}
			file_path_add_name(file->path, find->data.cFileName);
			file->flags |= 1;
			if (time)
			{
				time->low = find->data.ftLastWriteTime.dwLowDateTime;
				time->high = find->data.ftLastWriteTime.dwHighDateTime;
			}
			find->depth = depth;
			return true;
		}
	}
	find->depth = depth;
	return false;
}
