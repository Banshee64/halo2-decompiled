// @flags /O2 /Gr
#include "unknown_11c920.h"
#include "globals.h"

struct s_engine_label_list
{
	byte unknown00[0x1c];
	long string_list_index;
};

struct s_engine_label_globals
{
	long count;
	s_engine_label_list *data;
};

void function_1a0180(long tag_index, long string_handle, word *buffer);

// @retail 0x159050
void function_159050(long index, word *buffer)
{
	long string_handle = 0;
	word *const *buffer_reference = &buffer;
	long const handles[16] =
	{
		0x0c0000ec, 0x0c0000ed, 0x0c0000ee, 0x0c0000ef,
		0x0c0000f0, 0x0c0000f1, 0x0c0000f2, 0x0c0000f3,
		0x0c0000f4, 0x0d0000f5, 0x0d0000f6, 0x0d0000f7,
		0x0d0000f8, 0x0d0000f9, 0x0d0000fa, 0x0d0000fb
	};

	if (index >= 0 && index < 16)
	{
		string_handle = handles[index];
	}
	if (g_4e034c && g_4e034c->index != NONE)
	{
		s_engine_label_globals *labels = (s_engine_label_globals *)g_4e3b44[g_4e034c->index & 0xffff].bytes;
		long string_list_index = labels->data->string_list_index;

		if (string_list_index != NONE)
		{
			function_1a0180(string_list_index, string_handle, *buffer_reference);
		}
	}
}
