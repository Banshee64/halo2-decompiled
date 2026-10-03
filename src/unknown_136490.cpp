// @flags /O2 /Gr
/* UNKNOWN_136490.CPP: bitmap sizes (bitmaps.obj) */

#include "cseries.h"

struct bitmap_data
{
	byte unknown00[4];
	short width;
	short height;
	char depth;
	byte more_flags;
	short type;
	short format;
	word flags;
	short registration_point[2];
	short mipmap_count;
};

enum
{
	_bitmap_type_cube_map = 2,
	k_bitmap_format_count = 24
};

/* the bits per pixel of each format */
short g_453550[k_bitmap_format_count] = { 8, 8, 8, 16, 0, 0, 16, 0, 16, 16, 32, 32, 0, 0, 4, 8, 8, 8, 8, 128, 96, 48, 16, 16 };

static inline short bitmap_format_get_bits_per_pixel(short format)
{
	long bits = 0;
	if (format != NONE)
	{
		bits = g_453550[format];
	}
	return bits;
}

// @retail 0x1364e0
long function_1364e0(bitmap_data const *bitmap, short mipmap_index)
{
	short width = bitmap->width >> mipmap_index > 1 ? bitmap->width >> mipmap_index : 1;
	if (bitmap->flags & 2)
	{
		width += -width & 3;
	}
	short height = bitmap->height >> mipmap_index > 1 ? bitmap->height >> mipmap_index : 1;
	if (bitmap->flags & 2)
	{
		height += -height & 3;
	}
	short depth = bitmap->depth >> mipmap_index > 1 ? bitmap->depth >> mipmap_index : 1;
	long pixels = depth * (width * height);
	if (bitmap->type == _bitmap_type_cube_map)
	{
		pixels *= 6;
	}
	return pixels;
}

// @retail 0x136490
long function_136490(bitmap_data const *bitmap)
{
	long pixels = 0;
	for (short mipmap_index = 0; mipmap_index <= bitmap->mipmap_count; mipmap_index++)
	{
		pixels += function_1364e0(bitmap, mipmap_index);
	}
	short bits = bitmap_format_get_bits_per_pixel(bitmap->format);
	return bits * pixels / 8;
}

// @retail 0x1365a0
long function_1365a0(bitmap_data const *bitmap, short mipmap_index)
{
	short width = bitmap->width >> mipmap_index > 1 ? bitmap->width >> mipmap_index : 1;
	if (bitmap->flags & 2)
	{
		width += -width & 3;
	}
	short bits = bitmap_format_get_bits_per_pixel(bitmap->format);
	return bits * width / 8;
}

// @retail 0x136600
long function_136600(short width, short height, short mipmap_index, short depth, short format, short alignment)
{
	short mipmap_width = width >> mipmap_index > 1 ? width >> mipmap_index : 1;
	short mipmap_height = height >> mipmap_index > 1 ? height >> mipmap_index : 1;
	short mipmap_depth = depth >> mipmap_index > 1 ? depth >> mipmap_index : 1;
	if (format >= 14 && format <= 16)
	{
		mipmap_width += -mipmap_width & 3;
		mipmap_height += -mipmap_height & 3;
	}
	short row_size = bitmap_format_get_bits_per_pixel(format) * mipmap_width / 8;
	if (alignment > 0)
	{
		row_size = (row_size + alignment - 1) & ~(alignment - 1);
	}
	return row_size * mipmap_depth * mipmap_height;
}

// @retail 0x1366d0
long bitmap_size_get_total_pixel_size(short width, short height, short depth, short format, short alignment, short mipmap_count)
{
	long total = 0;
	for (short mipmap_index = 0; mipmap_index <= mipmap_count; mipmap_index++)
	{
		total += function_136600(width, height, mipmap_index, depth, format, alignment);
	}
	return total;
}
