// @flags /O2 /Gr
/* UNKNOWN_122810.CPP: appending to a 256-character string */

#include "cseries.h"
#include <string.h>

// @retail 0x122810
char *function_122810(char *string, const char *suffix)
{
	const char *s = string;
	dword length;

	for (length = 0; length < 0x100; length++)
	{
		if (!*s++)
		{
			break;
		}
	}
	strncpy(string + length, suffix, 0x100 - length);
	string[0xff] = 0;
	return string;
}
