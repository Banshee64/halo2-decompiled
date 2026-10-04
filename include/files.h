/* FILES.H: file references and file operations
   (unknown_136710.cpp, unknown_1367d0.cpp, unknown_136d50.cpp) */

#ifndef FILES_H
#define FILES_H

#include "cseries.h"
#include <xtl.h>

#define FILE_REFERENCE_SIGNATURE 0x66696c6f

struct file_reference
{
	dword signature;
	word flags;
	short location;
	char path[256];
	HANDLE handle;
	dword position;
};

void file_path_add_name(char *path, const char *name);
void file_path_remove_name(char *path);
bool function_1367d0(file_reference *file);
bool function_136860(file_reference *file);
bool function_136970(file_reference *file, dword flags, dword *error);
bool function_136bb0(file_reference *file);
bool function_136bf0(file_reference *file, dword position, bool silent);
bool function_136ca0(file_reference *file, void *buffer, dword size, bool silent);
bool function_136d00(file_reference *file, const void *buffer, dword size);
bool file_read_from_position(file_reference *file, dword position, dword size, bool silent, void *buffer);
bool file_write_to_position(file_reference *file, dword position, dword size, const void *buffer);

#endif
