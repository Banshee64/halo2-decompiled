// @flags /O2 /Gr
/* UNKNOWN_136710.CPP: file reference construction (unknown_136770.obj) */

#include "unknown_11c920.h"
#include <xtl.h>
#include "files.h"

// @retail 0x136710
s_type_acf665 *function_136710(s_type_acf665 *file, bool replace, const char *name)
{
	memset(file, 0, sizeof(s_type_acf665));
	file->signature = FILE_REFERENCE_SIGNATURE;
	file->location = NONE;
	if (replace)
	{
		function_137320(file->path, name);
		return file;
	}

	if (file->flags & 1)
	{
		function_1373c0(file->path);
	}
	function_137320(file->path, name);
	file->flags |= 1;
	return file;
}
