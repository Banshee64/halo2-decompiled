// @flags /O2 /Gr
/* UNKNOWN_1A1870.CPP: the path of a file reference, as the asynchronous file
   helpers (async_helpers.cpp) open it */

#include "cseries.h"

/* files_windows.cpp's file reference */
struct file_reference_data
{
	dword signature;
	byte flags;
	byte unknown05;
	word unknown06;
	char path[256];
	byte unknown108[8];
};

void function_1374c0(char *dest, const char *path);
void function_137400(char *path, char **a, char **b, char **c, char **d, bool flag);
void file_path_add_name(char *path, const char *name);
void file_path_add_extension(char *path, const char *extension);

/* the file's path: its directory, name and extension */
// @retail 0x1a1870
char *file_reference_get_path(file_reference_data const *file, char *path)
{
	char full_path[256] = "";
	char *name;
	char *extension;
	char *directory;
	char *parent;

	function_1374c0(full_path, file->path);
	function_137400(full_path, &name, &extension, &directory, &parent, (bool)(file->flags & 1));
	path[0] = 0;
	file_path_add_name(path, directory);
	file_path_add_name(path, name);
	file_path_add_extension(path, extension);
	return path;
}
