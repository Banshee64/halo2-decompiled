// @flags /O2 /Gr
/* UNKNOWN_136D50.CPP: file reads and writes at a position (the same object as
   unknown_1367d0.cpp in retail) */

#include "cseries.h"
#include <xtl.h>
#include "files.h"

// @retail 0x136d50
bool file_read_from_position(file_reference *file, dword position, dword size, bool silent, void *buffer)
{
	return function_136bf0(file, position, silent) && function_136ca0(file, buffer, size, silent);
}

// @retail 0x136d90
bool file_write_to_position(file_reference *file, dword position, dword size, const void *buffer)
{
	bool success = true;
	if (file->position != position)
	{
		file->position = SetFilePointer(file->handle, position, NULL, FILE_BEGIN);
		success = file->position != INVALID_SET_FILE_POINTER;
		if (!success)
		{
			GetLastError();
			SetLastError(0);
		}
	}
	return success && function_136d00(file, buffer, size);
}
