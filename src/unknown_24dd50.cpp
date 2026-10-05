#include "unknown_11c920.h"
#include "unknown_1523c0.h"
#include "globals.h"

// @flags /O2 /Gr

// @retail 0x24dd50
real c_game_engine::v41(long)
{
	return 1.0f;
}

struct s_string_block_24ddf0
{
	byte field_00[0x1c];
	long field_1c;
};

struct s_tag_view_24ddf0
{
	long field_00;
	s_string_block_24ddf0 *field_04;
};

void function_1a0180(long tag_index, long string_handle, word *buffer);

// @retail 0x24ddf0
void c_game_engine::v50(long buffer)
{
	s_tag_header_globals *view = g_4e034c;
	if (view && view->index != NONE)
	{
		s_tag_view_24ddf0 *tag = (s_tag_view_24ddf0 *)g_4e3b44[view->index & 0xffff].data;
		long value = tag->field_04->field_1c;
		if (value != NONE)
			function_1a0180(value, 0x8000102, (word *)buffer);
	}
}
