// @flags /O2 /Gr
/* UNKNOWN_19BD50.CPP: the level handle tables (entry 13 of the game
   module table) and the multiplayer maps loaded from files */

#include "unknown_11c920.h"
#include "globals.h"
#include "data_array.h"
#include "crc.h"

// @retail 0x19bcd0
void level_handle_tables_initialize(void)
{
	s_record_pool *data = data_new_inlined("level handles", 20, 8, 0, g_468758);
	g_4ee4e8 = data;
	data = data_new_inlined("multiplayer level handles", 50, 8, 0, g_468758);
	g_4ee4e4 = data;
}

// @retail 0x19bd50
void level_handle_tables_dispose(void)
{
	if (g_4ee4e4)
	{
		data_dispose(g_4ee4e4);
		g_4ee4e4 = 0;
	}
	if (g_4ee4e8)
	{
		data_dispose(g_4ee4e8);
		g_4ee4e8 = 0;
	}
}

/* a multiplayer map file's header: a checksum of the whole file (0x2d3fc
   bytes) with its own field cleared */
struct s_map_file_header
{
	long type;
	dword checksum;
};

// @retail 0x19bdc0
bool map_file_checksum_valid(s_map_file_header *header)
{
	if (header->type != 2)
		return false;

	dword checksum = header->checksum;
	dword crc;

	header->checksum = 0;
	crc = 0xffffffff;
	function_163ba0(&crc, header, 0x2d3fc);
	bool valid = checksum == crc;
	header->checksum = checksum;

	return valid;
}
