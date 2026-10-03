// @flags /O2 /Gr
/* UNKNOWN_1367D0.CPP: file operations (files_windows.obj) */

#include "cseries.h"
#include <xtl.h>
#include "files.h"

#define MAXIMUM_PATH_SIZE 256

void function_1374c0(char *dest, const char *path);

// @retail 0x1367d0
bool function_1367d0(file_reference *file)
{
	char path[MAXIMUM_PATH_SIZE] = { 0 };
	function_1374c0(path, file->path);

	if (file->flags & 1)
	{
		HANDLE handle = CreateFileA(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (handle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(handle);
			return true;
		}
	}
	else if (CreateDirectoryA(path, NULL))
	{
		return true;
	}

	GetLastError();
	SetLastError(0);
	return false;
}

// @retail 0x136860
bool function_136860(file_reference *file)
{
	char path[MAXIMUM_PATH_SIZE] = { 0 };
	function_1374c0(path, file->path);

	if (file->flags & 1)
	{
		if (SetFileAttributesA(path, FILE_ATTRIBUTE_NORMAL) && DeleteFileA(path))
		{
			return true;
		}
	}
	else if (RemoveDirectoryA(path))
	{
		return true;
	}

	GetLastError();
	SetLastError(0);
	return false;
}

// @retail 0x1368f0
bool function_1368f0(file_reference *file)
{
	char path[MAXIMUM_PATH_SIZE] = { 0 };
	bool success = false;
	function_1374c0(path, file->path);

	if (GetFileAttributesA(path) != 0xFFFFFFFF)
	{
		success = true;
	}
	else if (GetLastError() != ERROR_FILE_NOT_FOUND && GetLastError() != ERROR_PATH_NOT_FOUND)
	{
		GetLastError();
		SetLastError(0);
	}
	return success;
}

// @retail 0x136970
bool function_136970(file_reference *file, dword flags, dword *error)
{
	char path[MAXIMUM_PATH_SIZE] = { 0 };
	dword access = 0;
	dword share = 0;
	dword attributes = FILE_ATTRIBUTE_NORMAL;

	*error = 0;
	function_1374c0(path, file->path);

	if (flags & 1)
	{
		access = GENERIC_READ;
	}
	if (flags & 2)
	{
		access |= GENERIC_WRITE;
	}
	if (!(flags & 2) || (flags & 8))
	{
		share = FILE_SHARE_READ;
	}
	if (flags & 0x20)
	{
		attributes = FILE_ATTRIBUTE_TEMPORARY;
	}
	if (flags & 0x40)
	{
		attributes = FILE_FLAG_DELETE_ON_CLOSE;
	}
	if (flags & 0x80)
	{
		attributes = FILE_FLAG_RANDOM_ACCESS;
	}
	if (flags & 0x100)
	{
		attributes = FILE_FLAG_SEQUENTIAL_SCAN;
	}

	HANDLE handle = CreateFileA(path, access, share, NULL, OPEN_EXISTING, attributes, NULL);
	if (handle == INVALID_HANDLE_VALUE)
	{
		switch (GetLastError())
		{
		case ERROR_FILE_NOT_FOUND: *error = 1; break;
		case ERROR_PATH_NOT_FOUND: *error = 3; break;
		case ERROR_TOO_MANY_OPEN_FILES: *error = 4; break;
		case ERROR_ACCESS_DENIED: *error = 2; break;
		case ERROR_SHARING_VIOLATION: *error = 5; break;
		case ERROR_INVALID_NAME: *error = 6; break;
		default: *error = 6; break;
		}
	}
	else
	{
		bool opened = true;
		file->handle = handle;
		file->position = 0;
		if (flags & 4)
		{
			file->position = SetFilePointer(handle, 0, NULL, FILE_END);
			if (file->position != INVALID_SET_FILE_POINTER)
			{
				return true;
			}
			CloseHandle(file->handle);
			file->handle = 0;
			file->position = 0;
		}
		else
		{
			return opened;
		}
	}

	if (!(flags & 0x10))
	{
		GetLastError();
		SetLastError(0);
	}
	return false;
}

// @retail 0x136bb0
bool function_136bb0(file_reference *file)
{
	bool success = false;
	if (CloseHandle(file->handle))
	{
		file->handle = 0;
		file->position = 0;
		success = true;
	}
	else
	{
		GetLastError();
		SetLastError(0);
	}
	return success;
}

// @retail 0x136bf0
bool function_136bf0(file_reference *file, dword position, bool silent)
{
	if (file->position == position)
	{
		return true;
	}
	dword result = SetFilePointer(file->handle, position, NULL, FILE_BEGIN);
	bool success = result != INVALID_SET_FILE_POINTER;
	file->position = result;
	if (!success && !silent)
	{
		GetLastError();
		SetLastError(0);
	}
	return success;
}

// @retail 0x136c40
bool function_136c40(file_reference *file, dword position)
{
	bool result = false;
	bool success = true;
	if (file->position != position)
	{
		dword result = SetFilePointer(file->handle, position, NULL, FILE_BEGIN);
		file->position = result;
		success = result != INVALID_SET_FILE_POINTER;
		if (!success)
		{
			GetLastError();
			SetLastError(0);
		}
	}
	if (success)
	{
		if (SetEndOfFile(file->handle))
		{
			return true;
		}
	}
	GetLastError();
	SetLastError(0);
	return result;
}

// @retail 0x136ca0
bool function_136ca0(file_reference *file, void *buffer, dword size, bool silent)
{
	dword bytes_read;
	bool success = false;
	if (ReadFile(file->handle, buffer, size, &bytes_read, NULL))
	{
		if (bytes_read == size)
		{
			success = true;
		}
		else
		{
			SetLastError(ERROR_HANDLE_EOF);
		}
	}
	file->position += bytes_read;
	if (!success && !silent)
	{
		GetLastError();
		SetLastError(0);
	}
	return success;
}

// @retail 0x136d00
bool function_136d00(file_reference *file, const void *buffer, dword size)
{
	dword bytes_written;
	bool success = false;
	BOOL result = WriteFile(file->handle, buffer, size, &bytes_written, NULL);
	if (result && bytes_written == size)
	{
		success = true;
	}
	file->position += bytes_written;
	if (!success)
	{
		GetLastError();
		SetLastError(0);
	}
	return success;
}
