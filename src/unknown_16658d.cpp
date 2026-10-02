// @flags /O1 /Gr
/* UNKNOWN_16658D.CPP */

#include "cseries.h"

struct s_16658d_entry
{
	long key;
	byte unknown04[0x1010 - 4];
};

struct s_16658d_group
{
	byte unknown00[0x10];
	s_16658d_entry entries[2];
	byte pad[0x20cc - 0x10 - 2 * 0x1010];
};

s_16658d_group *g_4e9bc8;

// @retail 0x16658d
long function_16658d(long group_index, long key)
{
	long result = NONE;

	if (group_index != NONE)
	{
		s_16658d_group *group = &g_4e9bc8[group_index];
		long i = 0;

		do
		{
			if (group->entries[i].key == key)
			{
				result = i;
				break;
			}
			i++;
		}
		while (i < 2);
	}

	return result;
}