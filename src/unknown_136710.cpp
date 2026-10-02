// @flags /O2 /Gr
/* UNKNOWN_136710.CPP: file reference construction (files_windows.obj) */

#include "cseries.h"
#include <xtl.h>

#define MAXIMUM_PATH_SIZE 256

struct file_reference
{
	dword signature;
	byte flags;
	byte unknown05[3];
	char path[MAXIMUM_PATH_SIZE];
	HANDLE handle;
	dword position;
};

void file_path_add_name(char *path, const char *name);
void file_path_remove_name(char *path);

// @retail 0x136710
file_reference *function_136710(file_reference *file, bool replace, const char *name)
{
	memset(file, 0, sizeof(file_reference));
	file->signature = 0x66696c6f;
	((word *)file)[3] = 0xffff;
	if (replace)
	{
		file_path_add_name(file->path, name);
		return file;
	}

	if (file->flags & 1)
	{
		file_path_remove_name(file->path);
	}
	file_path_add_name(file->path, name);
	file->flags |= 1;
	return file;
}
