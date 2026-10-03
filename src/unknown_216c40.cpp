// @flags /O2 /Ob1 /Gr
/* UNKNOWN_216C40.CPP: the sizes of saved game files in blocks (the user
   interface's block counts call these) */

#include "cseries.h"
#include <xtl.h>

char *csnprintf(char *buffer, long maximum_count, const char *format, ...);

/* the saved game drive's flags: bit 0 set when saved games go to the
   utility drive */
byte g_4e61c0;

/* the blocks a file of this size takes on the saved game drive (a block is
   16 KB; a file takes six clusters more for its directory entries) */
// @retail 0x216c70
long saved_game_file_size_in_blocks(long size)
{
	char root[8];
	char letter = (g_4e61c0 & 1) ? 'u' : 0;

	if (letter)
	{
		csnprintf(root, sizeof(root), "%c:\\", letter);
	}
	else
	{
		root[0] = 0;
	}
	return (size + (long)XGetDiskClusterSize(root) * 6 + 0x3fff) / 0x4000;
}

/* the blocks a saved game file of this type takes: type 0 is a player
   profile, types 1..9 game variants, the rest playlists */
// @retail 0x216c40
long saved_game_file_type_size_in_blocks(long type)
{
	long size;

	if (type)
	{
		size = type > 9 ? 0x15cb8 : 0x130;
	}
	else
	{
		size = 0x1e0;
	}
	return saved_game_file_size_in_blocks(size);
}
