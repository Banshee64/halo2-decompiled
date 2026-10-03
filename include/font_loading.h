#ifndef FONT_LOADING_H
#define FONT_LOADING_H

#include "cseries.h"

/* the font headers (src/font_loading.cpp) */

struct s_kerning_pair
{
	byte first_character;
	byte second_character;
	short offset;
};

struct s_font_header
{
	long version;
	short ascending_height;
	short descending_height;
	short leading_height;
	byte unknown0a[0x16];
	long kerning_pair_count;
	s_kerning_pair kerning_pairs[96];
	dword kerning_characters[8];
};

/* the font indices of the font table */
extern long g_4e28f4[11];

s_font_header *font_get(long font_index);

#endif
