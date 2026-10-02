// @flags /O2 /Gr
/* UNKNOWN_137320.CPP: file path helpers (files_windows.obj) and bitmap group lookups */

#include "cseries.h"
#include "globals.h"
#include <string.h>

#define MAXIMUM_PATH_SIZE 256

// @retail 0x137320
void file_path_add_name(char *path, const char *name)
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
		strncpy(end, name, MAXIMUM_PATH_SIZE - length);
		path[MAXIMUM_PATH_SIZE - 1] = 0;
	}
}

// @retail 0x137370
void file_path_add_extension(char *path, const char *extension)
{
	if (*extension)
	{
		size_t length = strlen(path);
		char *end = path + length;
		if (end != path)
		{
			*end++ = '.';
			*end = 0;
			length++;
		}
		strncpy(end, extension, MAXIMUM_PATH_SIZE - length);
		path[MAXIMUM_PATH_SIZE - 1] = 0;
	}
}

// @retail 0x1373c0
void file_path_remove_name(char *path)
{
	short index = (short)strlen(path);
	while (index > 0 && path[index] != '\\')
	{
		index--;
	}
	path[index] = 0;
}

// @retail 0x137400
void function_137400(char *path, char **a, char **b, char **c, char **d, bool flag)
{
	short length = (short)strlen(path);
	char *p = path + length;
	*c = p;
	*d = p;
	*a = p;
	*b = p;
	if (length > 0)
	{
		dword n = (word)length;
		do
		{
			if (*p == '.')
			{
				if (flag && **a == 0 && **b == 0)
				{
					*b = p + 1;
					*p = 0;
				}
			}
			else if (*p == '\\')
			{
				if (flag && **a == 0)
				{
					*a = p + 1;
					*p = 0;
				}
				else if (**d == 0)
				{
					*d = p + 1;
				}
			}
			p--;
			n--;
		} while (n);
	}
	if (flag && **a == 0)
	{
		*a = path;
	}
	else if (*a != path)
	{
		*c = path;
	}
}

char g_453588[256];

// @retail 0x1374c0
void function_1374c0(char *dest, const char *path)
{
	dest[0] = 0;
	char c = path[0];
	if (!(((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) && path[1] == ':' && path[2] == '\\'))
	{
		strncpy(dest, g_453588, MAXIMUM_PATH_SIZE);
		dest[MAXIMUM_PATH_SIZE - 1] = 0;
	}
	const char *end = dest;
	dword length;
	for (length = 0; length < MAXIMUM_PATH_SIZE; length++)
	{
		if (!*end++)
		{
			break;
		}
	}
	strncpy(dest + length, path, MAXIMUM_PATH_SIZE - length);
	dest[MAXIMUM_PATH_SIZE - 1] = 0;
}

struct bitmap_data
{
	byte unknown00[0x74];
};

struct bitmap_sequence
{
	byte unknown00[0x20];
	short first_bitmap_index;
	short bitmap_count;
	byte unknown24[0x10];
	long frame_count;
	byte *frames;
};

struct bitmap_group
{
	byte unknown00[0x3c];
	long sequence_count;
	bitmap_sequence *sequences;
	long bitmap_count;
	bitmap_data *bitmaps;
};

// @retail 0x137550
struct bitmap_data *bitmap_group_try_and_get_bitmap(dword group_index, short bitmap_index)
{
	bitmap_group *group = g_4e3b44[(word)group_index].group;
	bitmap_data *result = 0;
	if (group && bitmap_index >= 0 && bitmap_index < group->bitmap_count)
	{
		result = &group->bitmaps[bitmap_index];
	}
	return result;
}

// @retail 0x137590
long function_137590(dword group_index, short frame_index, short sequence_index)
{
	if (group_index == NONE)
	{
		return NONE;
	}
	bitmap_group *group = g_4e3b44[(word)group_index].group;
	if (!group)
	{
		return NONE;
	}
	if (group->sequence_count <= 0)
	{
		return 0;
	}
	bitmap_sequence *sequence = &group->sequences[sequence_index % group->sequence_count];
	long result;
	if (sequence->bitmap_count > 0)
	{
		result = frame_index % sequence->bitmap_count + sequence->first_bitmap_index;
	}
	else if (sequence->frame_count)
	{
		result = *(short *)(sequence->frames + (frame_index << 5));
	}
	else
	{
		return frame_index;
	}
	if (result == NONE)
	{
		return frame_index;
	}
	return result;
}
