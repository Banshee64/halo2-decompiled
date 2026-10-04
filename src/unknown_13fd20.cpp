// @flags /O2 /Gr
/* UNKNOWN_13FD20.CPP: unicode line breaking */

#include "unknown_11c920.h"
#include "unknown_13fd90.h"

struct unicode_range
{
	word first;
	word last;
};

bool unicode_ranges_contain(long range_count, word character, const unicode_range *ranges);
bool function_13fd90(utf32 character);
bool function_13fde0(utf32 character);
bool function_140260(utf32 character);

/* the white space characters */
unicode_range const g_4536cc[] =
{
	{ 0x9, 0xd },
	{ 0x20, 0x20 },
	{ 0x85, 0x85 },
	{ 0xa0, 0xa0 },
	{ 0x1680, 0x1680 },
	{ 0x180e, 0x180e },
	{ 0x2000, 0x200a },
	{ 0x2028, 0x2028 },
	{ 0x2029, 0x2029 },
	{ 0x202f, 0x202f },
	{ 0x205f, 0x205f },
	{ 0x3000, 0x3000 }
};

/* whether a line may break between two characters: after white space or
   around east asian characters, unless the punctuation forbids it */
// @retail 0x13fd20
bool function_13fd20(utf32 previous, utf32 character)
{
	bool result = false;

	if (previous.value)
	{
		bool non_beginning = function_13fde0(character);
		bool non_ending = function_140260(previous);
		if ((unicode_ranges_contain(sizeof(g_4536cc) / sizeof(g_4536cc[0]), (word)previous.value, g_4536cc) ||
			function_13fd90(previous) || function_13fd90(character)) &&
			!non_beginning && !non_ending)
		{
			result = true;
		}
	}

	return result;
}
