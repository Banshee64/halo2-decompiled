// @flags /O2 /Gr
/* UNKNOWN_136710.CPP: file reference construction (files_windows.obj) */

#include "cseries.h"
#include <xtl.h>
#include "files.h"

// @retail 0x136710
file_reference *function_136710(file_reference *file, bool replace, const char *name)
{
	memset(file, 0, sizeof(file_reference));
	file->signature = FILE_REFERENCE_SIGNATURE;
	file->location = NONE;
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
