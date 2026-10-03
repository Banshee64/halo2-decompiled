// @flags /O2 /Gr
#include "cseries.h"
#include <string.h>

/* a game variant (0x614 bytes; the 16 of them are at 0x551ae8) */
struct s_surface_description
{
	long type;
	long field_4;
	dword field_8;
	byte unknown00c[0x51c - 0xc];
	char field_51c[128];
	byte unknown59c[0x5e4 - 0x59c];
	long field_5e4;
	bool flag_5e8;
	byte unknown5e9[3];
	long width;       // 0x5ec
	long height;      // 0x5f0
	long depth;       // 0x5f4
	long field_5f8;   // 0x5f8
	long field_5fc;
	long field_600;
	bool flag_604;    // 0x604
	byte unknown605[3];
	long field_608;   // 0x608
	long field_60c;   // 0x60c
	byte unknown610[4];
};

/* the game variant globals at 0x47d8f4 */
struct s_game_variant_globals
{
	dword unknown0;
	dword flags;
	word count;
	word state;
};

s_game_variant_globals g_47d8f4;
s_surface_description g_551ae8[16];
// @retail 0x001932c0
long function_1932c0(s_surface_description* p)
{
	switch (p->type)
	{
	case 1:
		return p->width;
	case 2:
		return p->width;
	case 3:
		return p->height * p->width;
	case 4:
		return p->height * p->width;
	case 5:
		return p->height * p->width;
	default:
		__assume(0);
	}
}

// @retail 0x00193300
long function_193300(s_surface_description* p)
{
	switch (p->type)
	{
	case 1:
		return p->height;
	case 2:
		return p->height;
	case 3:
		return p->depth * p->width;
	case 4:
		return p->depth * p->width;
	case 5:
		return p->depth * p->width;
	default:
		__assume(0);
	}
}

// @retail 0x00193340
long function_193340(s_surface_description* p)
{
	switch (p->type)
	{
	case 1:
		return 1;
	case 2:
		return 1;
	case 3:
		return p->height;
	case 4:
		return p->height;
	case 5:
		return p->height;
	default:
		__assume(0);
	}
}

// @retail 0x00193370
long function_193370(s_surface_description* p)
{
	switch (p->type)
	{
	case 1:
		return 1;
	case 2:
		return 1;
	case 3:
		return p->depth;
	case 4:
		return p->depth;
	case 5:
		return p->depth;
	default:
		__assume(0);
	}
}

// @retail 0x001933a0
long function_1933a0(s_surface_description* p)
{
	long result = 0;
	if (p->type == 5)
	{
		result = p->depth;
	}
	else if (p->type == 4 && p->flag_604)
	{
		result = p->field_60c;
	}
	return result;
}

// @retail 0x001933d0
long function_1933d0(s_surface_description* p)
{
	long result = 0;
	if (p->type == 5)
	{
		result = p->height;
	}
	else if (p->type == 4 && p->flag_604)
	{
		result = p->field_608;
	}
	return result;
}

// @retail 0x00193400
long function_193400(s_surface_description* p)
{
	long result = 0;
	switch (p->type)
	{
	case 1:
		break;
	case 2:
		break;
	case 3:
		if (*(byte*)&p->field_5f8)
		{
			result = 1;
		}
		break;
	case 4:
		result = p->field_5f8;
		break;
	case 5:
		result = p->field_5f8;
		break;
	default:
		__assume(0);
	}
	return result;
}

// @retail 0x00193440
long function_193440(s_surface_description* p)
{
	long result = NONE;
	switch (p->type)
	{
	case 1:
		break;
	case 2:
		break;
	case 3:
		result = p->width;
		break;
	case 4:
		result = p->width;
		break;
	case 5:
		result = p->width;
		break;
	default:
		__assume(0);
	}
	return result;
}
// @retail 0x00193470
bool function_193470(s_surface_description* p)
{
	return p->type == 5 || p->type == 2 || p->type == 4;
}

// @retail 0x00193490
bool function_193490(s_surface_description* p)
{
	return p->type == 1 || p->type == 3;
}

// @retail 0x001934b0
bool function_1934b0(s_surface_description* p)
{
	return p->type == 5 || p->type == 3 || p->type == 4;
}

// @retail 0x001934d0
bool function_1934d0(s_surface_description* p)
{
	return (p->type == 4 && p->flag_604) || p->type == 5;
}

// @retail 0x1931a0
long function_1931a0(long count, s_surface_description *p)
{
	long index = count;

	if (count > 0)
	{
		do
		{
			if (p->field_51c[index - 1] < count)
			{
				break;
			}
			index--;
		}
		while (index > 0);
	}
	return index;
}

// @retail 0x193250
long function_193250(s_surface_description *p)
{
	switch (p->type)
	{
	case 1:
		return function_193300(p);
	case 2:
		return 1;
	case 3:
		return function_193300(p);
	case 4:
		return p->field_600;
	case 5:
		return p->depth;
	default:
		__assume(0);
	}
}

// @retail 0x193560
bool function_193560(s_surface_description *p)
{
	bool valid;
	long index;

	switch (p->type)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		valid = p->field_4 >= 0 && p->field_4 < 4 && p->field_8 > 0 && p->field_8 <= 100;
		break;
	default:
		valid = false;
		break;
	}
	for (index = 0; index < 128; index++)
	{
		valid = valid && p->field_51c[index] >= index && p->field_51c[index] <= 127;
	}
	return valid;
}

// @retail 0x193610
byte function_193610(s_surface_description *p)
{
	long width = p->width;
	if (width >= 1)
	{
		long height = p->height;
		if (height <= 16 && width <= height)
		{
			return true;
		}
	}
	return false;
}

// @retail 0x193630
byte function_193630(s_surface_description *p)
{
	long width = p->width;
	if (width > 1 && width <= 16)
	{
		long height = p->height;
		if (height > 1)
		{
			long depth = p->depth;
			if (depth <= 16 / width && height <= depth)
			{
				long a = p->field_5fc;
				if (a > 0)
				{
					long b = p->field_600;
					if (b <= depth && a <= b)
					{
						long c = p->field_5f8;
						if (c >= 0 && c <= depth - height)
						{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

// @retail 0x1936a0
byte function_1936a0(s_surface_description *p)
{
	long width = p->width;
	if (width > 1 && width <= 16)
	{
		long height = p->height;
		if (height >= 1)
		{
			long depth = p->depth;
			if (depth <= 16 / width && height <= depth)
			{
				return true;
			}
		}
	}
	return false;
}

// @retail 0x1934f0
bool function_1934f0(s_surface_description *p)
{
	if (!function_193560(p))
	{
		return false;
	}
	switch (p->type)
	{
	case 1:
		return function_193610(p) != 0;
	case 2:
		return function_193630(p) != 0;
	case 3:
		return function_1936a0(p) != 0;
	case 4:
		return function_1936a0(p) != 0;
	default:
		__assume(0);
	}
}

static inline bool game_variants_available(void)
{
	return g_47d8f4.count && (g_47d8f4.flags & 2);
}

static inline s_surface_description *game_variant_get(long index)
{
	if (index >= 0 && index < 16 && game_variants_available())
	{
		s_surface_description *variant = &g_551ae8[index];
		if (function_1934f0(variant))
		{
			return variant;
		}
	}
	return NULL;
}

// @retail 0x192e60
s_surface_description *function_192e60(long index)
{
	return game_variant_get(index);
}

// @retail 0x192db0
bool function_192db0(long index)
{
	if (index >= 0 && index < 16 && game_variants_available())
	{
		return function_1934f0(&g_551ae8[index]);
	}
	return false;
}

// @retail 0x193f50
long function_193f50(void)
{
	if (game_variants_available())
	{
		return 1;
	}
	return 0;
}

// @retail 0x1945c0
long function_1945c0(long index)
{
	s_surface_description *variant = game_variant_get(index);
	if (variant)
	{
		return variant->field_5e4;
	}
	return 0;
}

// @retail 0x194610
bool function_194610(long index)
{
	bool result = false;
	s_surface_description *variant = game_variant_get(index);
	if (variant)
	{
		return variant->flag_5e8;
	}
	return result;
}

struct file_reference_data
{
	dword signature;
	byte flags;
	byte unknown05;
	word unknown06;
	char path[256];
	byte unknown108[8];
};

void file_path_add_name(char *path, const char *name);
void file_path_remove_name(char *path);

char const *g_46dd5c;

// @retail 0x193fa0
void function_193fa0(file_reference_data *file)
{
	memset(file, 0, sizeof(*file));
	file->signature = 'filo';
	file->unknown06 = 0xffff;
	file_path_add_name(file->path, "n:\\");
	if (file->flags & 1)
	{
		file_path_remove_name(file->path);
	}
	file_path_add_name(file->path, g_46dd5c);
	file->flags |= 1;
}
