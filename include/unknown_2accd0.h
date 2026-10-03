/* UNKNOWN_2ACCD0.H: saved game files on the Xbox hard disk
   (src/unknown_2accd0.cpp), the file references they use (files_windows.cpp,
   unknown_1367d0.cpp) and the asynchronous tasks that run them
   (src/unknown_1a17b0.cpp) */

#ifndef UNKNOWN_2ACCD0_H
#define UNKNOWN_2ACCD0_H

#include "cseries.h"
#include <xtl.h>

#define FILE_REFERENCE_SIGNATURE 0x66696c6f

/* the same layout as unknown_1367d0.cpp's file_reference */
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
bool function_136970(file_reference *file, dword flags, dword *error);
bool function_136bb0(file_reference *file);
bool function_136bf0(file_reference *file, dword position, bool silent);
bool function_136ca0(file_reference *file, void *buffer, dword size, bool silent);
bool function_136d00(file_reference *file, const void *buffer, dword size);
bool file_read_from_position(file_reference *file, dword position, dword size, bool silent, void *buffer);
bool file_write_to_position(file_reference *file, dword position, dword size, const void *buffer);

/* an asynchronous task: a work function and a copy of its parameters */
struct s_async_task;
typedef long (__stdcall *async_work_callback)(s_async_task *task, void *parameters, long parameters_size);

struct s_async_task
{
	async_work_callback callback;
	byte parameters[0x23];
	char parameters_size;
};

long async_task_add_work(async_work_callback callback, long parameters_size, void *parameters, long priority, bool *done);

/* the state of a saved game file operation */
struct s_saved_game_file_task
{
	bool done;
	bool unknown1;
	bool succeeded;
	byte unknown3;
	long state;
	char path[256];
	real progress;
};

#endif
