// @flags /O2 /Gr
/* UNKNOWN_136490.CPP: bitmap sizes */

#include "unknown_11c920.h"
#include <xtl.h>
#include <string.h>

struct s_type_7ba8e9
{
	dword signature;
	short width;
	short height;
	char depth;
	byte more_flags;
	short type;
	short format;
	word flags;
	short registration_point[2];
	short mipmap_count;
	byte unknown16[0x50 - 0x16];
	D3DResource *field_50;
	void *base_address;
	byte unknown58[0x74 - 0x58];
};

enum
{
	_enum_value_9146b9 = 2,
	_enum_value_e13d3d = 24
};

/* the bits per pixel of each format */
short g_453550[_enum_value_e13d3d] = { 8, 8, 8, 16, 0, 0, 16, 0, 16, 16, 32, 32, 0, 0, 4, 8, 8, 8, 8, 128, 96, 48, 16, 16 };

// @retail 0x1358c0
short function_1358c0(short format)
{
	long bits = 0;
	if (format != NONE)
	{
		bits = g_453550[format];
	}
	return bits;
}

static inline short function_x48d32c(short format)
{
	return function_1358c0(format);
}

// @retail 0x1364e0
long function_1364e0(s_type_7ba8e9 const *bitmap, short mipmap_index)
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
	if (bitmap->type == _enum_value_9146b9)
	{
		pixels *= 6;
	}
	return pixels;
}

// @retail 0x136490
long function_136490(s_type_7ba8e9 const *bitmap)
{
	long pixels = 0;
	for (short mipmap_index = 0; mipmap_index <= bitmap->mipmap_count; mipmap_index++)
	{
		pixels += function_1364e0(bitmap, mipmap_index);
	}
	short bits = function_x48d32c(bitmap->format);
	return bits * pixels / 8;
}

// @retail 0x1365a0
long function_1365a0(s_type_7ba8e9 const *bitmap, short mipmap_index)
{
	short width = bitmap->width >> mipmap_index > 1 ? bitmap->width >> mipmap_index : 1;
	if (bitmap->flags & 2)
	{
		width += -width & 3;
	}
	short bits = function_x48d32c(bitmap->format);
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
	short row_size = function_x48d32c(format) * mipmap_width / 8;
	if (alignment > 0)
	{
		row_size = (row_size + alignment - 1) & ~(alignment - 1);
	}
	return row_size * mipmap_depth * mipmap_height;
}

// @retail 0x1366d0
long function_1366d0(short width, short height, short depth, short format, short alignment, short mipmap_count)
{
	long total = 0;
	for (short mipmap_index = 0; mipmap_index <= mipmap_count; mipmap_index++)
	{
		total += function_136600(width, height, mipmap_index, depth, format, alignment);
	}
	return total;
}

// @retail 0x1358e0
s_type_7ba8e9 *function_1358e0(short width, short height, short mipmap_count, short format, word flags)
{
	s_type_7ba8e9 *bitmap = (s_type_7ba8e9 *)VirtualAlloc(NULL, sizeof(s_type_7ba8e9), MEM_COMMIT | MEM_TOP_DOWN, PAGE_READWRITE);
	if (!bitmap)
	{
		GetLastError();
	}
	else
	{
		memset(bitmap, 0, sizeof(*bitmap));
		bitmap->type = 0;
		bitmap->mipmap_count = mipmap_count;
		bitmap->width = width;
		bitmap->signature = 'bitm';
		bitmap->height = height;
		bitmap->depth = 1;
		bitmap->format = format;
		bitmap->flags = flags | 0x100;
		if (!(width & (width - 1)) && !(height & (height - 1)))
		{
			bitmap->flags |= 1;
		}
		if (format >= 14 && format <= 16)
		{
			bitmap->flags |= 2;
		}
		if (format == 17)
		{
			bitmap->flags |= 4;
		}
		bitmap->base_address = NULL;
		if (!(flags & 0x800))
		{
			long size = function_136490(bitmap);
			void *base_address = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_TOP_DOWN, PAGE_READWRITE);
			if (!base_address)
			{
				GetLastError();
			}
			bitmap->base_address = base_address;
		}
	}
	return bitmap;
}

/* the address of a pixel of a 2d texture's mipmap */
// @retail 0x135a30
void *function_135a30(s_type_7ba8e9 const *bitmap, short mipmap_index, short x, short y)
{
	short minimum = (bitmap->flags & 2) ? 4 : 1;
	short height = bitmap->height;
	long offset = 0;
	short width = bitmap->width;
	long bits = function_x48d32c(bitmap->format);

	for (short i = 0; i < mipmap_index; i++)
	{
		offset += width * height;
		width = minimum > width >> 1 ? minimum : width >> 1;
		height = minimum > height >> 1 ? minimum : height >> 1;
	}

	return (byte *)bitmap->base_address + bits * (x + y * width + offset) / 8;
}

/* the address of a pixel of a 3d texture's mipmap */
// @retail 0x135af0
void *function_135af0(s_type_7ba8e9 const *bitmap, short x, short y, short z, short mipmap_index)
{
	short minimum = (bitmap->flags & 2) ? 4 : 1;
	short width = bitmap->width;
	short height = bitmap->height;
	short depth = bitmap->depth;
	long offset = 0;
	long bits = function_x48d32c(bitmap->format);

	for (short i = 0; i < mipmap_index; i++)
	{
		offset += width * height * depth;
		width = minimum > width >> 1 ? minimum : width >> 1;
		height = minimum > height >> 1 ? minimum : height >> 1;
		depth = 1 > depth >> 1 ? 1 : depth >> 1;
	}

	return (byte *)bitmap->base_address + bits * (x + width * (y + height * z) + offset) / 8;
}

/* the address of a pixel of a cube map face's mipmap */
// @retail 0x135c00
void *function_135c00(s_type_7ba8e9 const *bitmap, short x, short y, short face, short mipmap_index)
{
	long offset = 0;
	short minimum = (bitmap->flags & 2) ? 4 : 1;
	short size = bitmap->width;
	long bits = function_x48d32c(bitmap->format);

	for (short i = 0; i < mipmap_index; i++)
	{
		offset += size * size * 6;
		size = minimum > size >> 1 ? minimum : size >> 1;
	}

	return (byte *)bitmap->base_address + bits * (x + size * (y + size * face) + offset) / 8;
}

struct s_bitmap_data;
void texture_cache_bitmap_unload(s_bitmap_data *bitmap);

// @retail 0x1359d0
void function_1359d0(s_type_7ba8e9 *bitmap)
{
	if (bitmap)
	{
		texture_cache_bitmap_unload((s_bitmap_data *)bitmap);
		if (bitmap->field_50)
		{
			D3DResource_Release(bitmap->field_50);
			bitmap->field_50 = NULL;
		}
		if (bitmap->flags & 0x100)
		{
			if (bitmap->base_address && !VirtualFree(bitmap->base_address, 0, MEM_RELEASE))
			{
				GetLastError();
			}
			if (!VirtualFree(bitmap, 0, MEM_RELEASE))
			{
				GetLastError();
			}
		}
	}
}

struct S3TCBlockRGB;
struct S3TCBlockRGBA_explicit;
struct S3TCBlockRGBA_interpolated;
struct S3TC_COLOR;
void function_223ed0(S3TCBlockRGB const *arg_1, S3TC_COLOR *arg_2, short arg_3, short arg_4);
void DecodeBlockRGBA_explicit__single_pixel(S3TCBlockRGBA_explicit const *arg_1, S3TC_COLOR *arg_2, short arg_3, short arg_4);
void DecodeBlockRGBA_interpolated__single_pixel(S3TCBlockRGBA_interpolated const *arg_1, S3TC_COLOR *arg_2, short arg_3, short arg_4);
void function_358d0(short arg_1, short arg_2, short arg_3, short arg_4, dword *arg_5);
dword function_135ca0(long arg_1, short arg_2, void const *arg_3);
bool g_55e72c;

// @retail 0x136140
dword function_136140(byte const *arg_1, long arg_2, byte const *arg_3, short arg_4, short arg_5, short arg_6, word arg_7, short arg_8, short arg_9)
{
	byte const *const *local_5 = &arg_3;
	short *local_4 = &arg_4;
	if (arg_7 & 2)
	{
		short local_1 = function_1358c0(arg_6);
		byte const *local_2 = *local_5 + ((short)(arg_9 / 4) * arg_8 / 4 + (short)(*local_4 / 4)) * (short)(local_1 * 16 / 8);
		*local_4 &= 3;
		arg_9 &= 3;
		if (local_2 < arg_1 || local_2 >= arg_1 + arg_2)
		{
			if (!g_55e72c)
				g_55e72c = true;
			local_2 = arg_1;
			*local_4 = 0;
			arg_9 = 0;
		}
		dword local_3;
		switch (arg_6)
		{
		case 14:
			function_223ed0((S3TCBlockRGB const *)local_2, (S3TC_COLOR *)&local_3, *local_4, arg_9);
			return local_3;
		case 15:
			DecodeBlockRGBA_explicit__single_pixel((S3TCBlockRGBA_explicit const *)local_2, (S3TC_COLOR *)&local_3, *local_4, arg_9);
			return local_3;
		case 16:
			DecodeBlockRGBA_interpolated__single_pixel((S3TCBlockRGBA_interpolated const *)local_2, (S3TC_COLOR *)&local_3, *local_4, arg_9);
			return local_3;
		default:
			return 0;
		}
	}
	if (arg_7 & 8)
	{
		dword local_1[2];
		function_358d0(arg_8, *local_4, arg_9, arg_5, local_1);
		return function_135ca0(local_1[0] | local_1[1], arg_6, *local_5);
	}
	return function_135ca0(arg_9 * arg_8 + *local_4, arg_6, *local_5);
}
