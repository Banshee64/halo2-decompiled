// @flags /O2 /Ob1 /Gr
/* UNKNOWN_137550.CPP: bitmap group lookups */

#include "cseries.h"
#include "globals.h"

struct bitmap_data
{
	byte unknown00[0x74];
};

struct bitmap_sequence
{
	byte unknown00[0x20];
	short first_bitmap_index;
	short bitmap_count;
	byte unknown24[0x10];
	long frame_count;
	byte *frames;
};

struct bitmap_group
{
	byte unknown00[0x3c];
	long sequence_count;
	bitmap_sequence *sequences;
	long bitmap_count;
	bitmap_data *bitmaps;
};

// @retail 0x137550
struct bitmap_data *bitmap_group_try_and_get_bitmap(dword group_index, short bitmap_index)
{
	bitmap_group *group = g_4e3b44[(word)group_index].group;
	bitmap_data *result = 0;
	if (group && bitmap_index >= 0 && bitmap_index < group->bitmap_count)
	{
		result = &group->bitmaps[bitmap_index];
	}
	return result;
}

// @retail 0x137590
long function_137590(dword group_index, short frame_index, short sequence_index)
{
	if (group_index == NONE)
	{
		return NONE;
	}
	bitmap_group *group = g_4e3b44[(word)group_index].group;
	if (!group)
	{
		return NONE;
	}
	if (group->sequence_count <= 0)
	{
		return 0;
	}
	bitmap_sequence *sequence = &group->sequences[sequence_index % group->sequence_count];
	long result;
	if (sequence->bitmap_count > 0)
	{
		result = frame_index % sequence->bitmap_count + sequence->first_bitmap_index;
	}
	else if (sequence->frame_count)
	{
		result = *(short *)(sequence->frames + (frame_index << 5));
	}
	else
	{
		return frame_index;
	}
	if (result == NONE)
	{
		return frame_index;
	}
	return result;
}
