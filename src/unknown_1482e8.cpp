// @flags /O1 /Gr
/* UNKNOWN_1482E8.CPP: tag lookup through the tag header globals */

#include "cseries.h"
#include "globals.h"

struct s_tag_indices_1482e8
{
	byte unknown00[0x84];
	long index84;
	byte unknown88[4];
	long index8c;
	byte unknown90[4];
	long index94;
};

struct s_header_globals_1482e8
{
	byte unknown00[0x110];
	long valid;
	s_tag_indices_1482e8 *indices;
};

struct s_selector_globals_1482e8
{
	byte unknown00[0x10];
	short selector;
};

// @retail 0x1482e8
void *function_1482e8(void)
{
	void *result = 0;

	if (g_4e0350 && g_4e034c)
	{
		s_selector_globals_1482e8 *selector_globals = (s_selector_globals_1482e8 *)g_4e0350;
		s_header_globals_1482e8 *header_globals = (s_header_globals_1482e8 *)g_4e034c;
		s_tag_indices_1482e8 *indices = header_globals->valid ? header_globals->indices : 0;

		if (indices)
		{
			long tag_index;
			switch (selector_globals->selector)
			{
			case 0:
				tag_index = indices->index8c;
				break;
			case 1:
				tag_index = indices->index94;
				break;
			case 2:
				tag_index = indices->index84;
				break;
			default:
				tag_index = NONE;
				break;
			}

			if (tag_index != NONE)
			{
				result = g_4e3b44[tag_index & 0xffff].bytes;
			}
		}
	}
	return result;
}
