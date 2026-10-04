// @flags /O2 /Gr
/* UNKNOWN_08CC20.CPP: a name string setter */

#include "cseries.h"
#include <wchar.h>

struct s_name_buffer
{
	wchar_t name[256];
};

// @retail 0x8cc20
void function_08cc20(s_name_buffer *buffer, const wchar_t *name)
{
	wcsncpy(buffer->name, name, 255);
	buffer->name[255] = 0;
}

/* the 32 names of a list, each a s_name_buffer, after a count */
struct s_name_list
{
	long count;
	s_name_buffer names[32];
};

// @retail 0x8cc00
void name_list_clear(s_name_list *list)
{
	for (long i = 0; i < 32; i++)
		list->names[i].name[0] = 0;
}