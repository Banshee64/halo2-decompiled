// @flags /O2 /Gr
/* UNKNOWN_216B60.CPP: saved game file types */

#include "cseries.h"

enum
{
	k_saved_game_file_type_count = 11
};

/* the folder name of a saved game file type */
// @retail 0x216b60
const char *function_216b60(long type)
{
	const char *name = "unknown";
	const char *names[k_saved_game_file_type_count] =
	{
		"profile",
		"slayer",
		"koth",
		"race",
		"oddball",
		"juggernaut",
		"headhunter",
		"ctf",
		"assault",
		"territories",
		"playlist"
	};

	if (type >= 0 && type < sizeof(names) / sizeof(names[0]))
	{
		name = names[type];
	}
	return name;
}

/* the size of a saved game file of a type */
// @retail 0x217480
long function_217480(long type)
{
	if (type != 0)
	{
		return type > 9 ? 0x15cb8 : 0x130;
	}
	return 0x1e0;
}
