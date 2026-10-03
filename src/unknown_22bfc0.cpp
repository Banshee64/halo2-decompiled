// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_22BFC0.CPP: the first or second entry of two definition blocks
   (0x22bfc0, 0x22bff0; the file continues past 0x22c000) */

#include "cseries.h"

struct s_22bfc0_entry
{
	long unknown00;
	long value;
};

struct s_22bfc0_block
{
	long count;
	s_22bfc0_entry *entries;
};

struct s_22bfc0_definition
{
	byte unknown000[0x1a0];
	s_22bfc0_block block;
};

struct s_22bff0_element
{
	byte unknown00[0x7c];
	s_22bfc0_block block;
	byte unknown84[0xb0 - 0x84];
};

struct s_22bff0_definition
{
	byte unknown000[0x1cc];
	s_22bff0_element *elements;
};

#define MIN(a, b) ((a) > (b) ? (b) : (a))

PRIVATE inline long block_entry_value_get(s_22bfc0_block const *block, bool second)
{
	short index = (short)second;

	index = (short)MIN(index, block->count - 1);
	if (index < 0)
	{
		return NONE;
	}
	return block->entries[index].value;
}

// @retail 0x22bfc0
long function_22bfc0(
	bool second,
	s_22bfc0_definition const *definition)
{
	return block_entry_value_get(&definition->block, second);
}

// @retail 0x22bff0
long function_22bff0(
	short element_index,
	s_22bff0_definition const *definition,
	bool second)
{
	return block_entry_value_get(&definition->elements[element_index].block, second);
}
