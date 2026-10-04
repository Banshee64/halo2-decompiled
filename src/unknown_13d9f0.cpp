// @flags /O2 /Gr
/* UNKNOWN_13D9F0.CPP */

#include "unknown_11c920.h"
#include "globals.h"

static inline bool function_13d9f0_inline(long type)
{
	switch (type)
	{
	case 1:
	case 2:
	case 3:
		return true;
	case 4:
	case 5:
		return false;
	}
	return true;
}

// @retail 0x13d9f0
bool function_13d9f0(long type)
{
	return !function_13d9f0_inline(type);
}

struct s_13da30_block
{
	byte unknown00[8];
	long *elements;
};

struct s_13da30_tag
{
	byte unknown00[0x24];
	s_13da30_block *block;
};

struct s_13da30_element
{
	long value;
	byte unknown04[8];
};

struct s_13da30_block_header_view
{
	byte unknown00[8];
	s_13da30_element *elements;
	byte unknown0c[0x60 - 0xc];
	short *indices;
};

struct s_13da30_owner
{
	byte unknown00[0xc];
	long tag_index;
};

// @retail 0x13da30
long function_13da30(s_13da30_owner const *owner)
{
	long result = NONE;
	if (owner && owner->tag_index != NONE)
	{
		s_13da30_block_header_view *block = (s_13da30_block_header_view *)((s_13da30_tag *)g_4e3b44[owner->tag_index & 0xffff].bytes)->block;
		long index = *block->indices;
		if (index != NONE)
		{
			result = block->elements[index].value;
		}
	}
	return result;
}