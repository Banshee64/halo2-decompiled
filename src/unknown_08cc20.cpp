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
