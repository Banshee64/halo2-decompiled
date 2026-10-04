// @flags /O2 /Gr
#include "cseries.h"
#include <xtl.h>
#include <string.h>
#include "globals.h"
#include "language.h"
#include "real_math.h"
#include "files.h"
#include "pending_messages.h"

/* a game variant (0x614 bytes; the 16 of them are at 0x551ae8) */
struct s_surface_description
{
	long type;
	long field_4;
	dword field_8;
	char names[9][0x10];
	char descriptions[9][0x80];
	char field_51c[128];
	byte unknown59c[0x5a4 - 0x59c];
	long points[16];
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

// @retail 0x1931d0
long function_1931d0(s_surface_description *p, long place)
{
	long last = function_193300(p);
	long first = function_1932c0(p);

	if (place < first || place > last)
		return 0x7fffffff;
	if (place == last)
		return 0;
	return p->points[place - 1];
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

bool function_193560(s_surface_description *p);
byte function_193610(s_surface_description *p);
byte function_193630(s_surface_description *p);
byte function_1936a0(s_surface_description *p);
byte function_1936a0_type5(s_surface_description *p);

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
		return function_193610(p);
	case 2:
		return function_193610(p);
	case 4:
		return function_193630(p);
	case 3:
		return function_1936a0(p);
	case 5:
		return function_1936a0_type5(p);
	default:
		__assume(0);
	}
}

static inline bool game_variants_available(void)
{
	return g_47d8f4.count && (g_47d8f4.flags & 2);
}

static inline bool game_variant_valid(long index)
{
	bool result = false;

	if (index >= 0 && index < 16 && game_variants_available())
	{
		result = function_1934f0(&g_551ae8[index]);
	}
	return result;
}

// @retail 0x192e60
s_surface_description *function_192e60(long index)
{
	s_surface_description *result = NULL;

	if (game_variant_valid(index))
	{
		result = &g_551ae8[index];
	}
	return result;
}

// @retail 0x192db0
bool function_192db0(long index)
{
	return game_variant_valid(index);
}

// @retail 0x193f50
bool function_193f50(void)
{
	return game_variants_available();
}

bool pending_message_request_get_progress(s_pending_message_header *header, real *progress);

/* g_47d8f4 starts with a pending message request (unknown_080f30.cpp) */
// @retail 0x193f70
bool function_193f70(real *progress)
{
	bool result = false;

	if (g_47d8f4.count && g_47d8f4.state == 1)
	{
		if (progress)
		{
			pending_message_request_get_progress((s_pending_message_header *)&g_47d8f4, progress);
		}
		result = true;
	}
	return result;
}

// @retail 0x1945c0
long function_1945c0(long index)
{
	long result = 0;

	if (game_variant_valid(index))
	{
		result = g_551ae8[index].field_5e4;
	}
	return result;
}

// @retail 0x194610
bool function_194610(long index)
{
	bool result = false;

	if (game_variant_valid(index))
	{
		result = g_551ae8[index].flag_5e8;
	}
	return result;
}

char const *g_46dd5c;

// @retail 0x193fa0
void function_193fa0(s_type_acf665 *file)
{
	memset(file, 0, sizeof(*file));
	file->signature = FILE_REFERENCE_SIGNATURE;
	file->location = NONE;
	function_137320(file->path, "n:\\");
	if (file->flags & 1)
	{
		function_1373c0(file->path);
	}
	function_137320(file->path, g_46dd5c);
	file->flags |= 1;
}

/* the game variants file: the 16 variants, then one block per variant */
/* one map of a variant's map list (0x37c bytes) */
struct s_game_variant_map
{
	long unknown000;
	long map_id;
	byte unknown008[0x248 - 8];
	long weight;
	byte settings[0x130];
};

struct s_game_variant_block
{
	byte unknown0000[0x44];
	long map_count;
	s_game_variant_map maps[100];
};

struct s_game_variants_file
{
	s_surface_description variants[16];
	s_game_variant_block blocks[16];
};

// @retail 0x193ff0
bool game_variants_file_write(s_game_variants_file *variants_file)
{
	bool result = false;
	s_type_acf665 file;

	function_193fa0(&file);
	if (function_1367d0(&file))
	{
		dword error;

		if (function_136970(&file, 2, &error))
		{
			if (function_136d00(&file, variants_file, sizeof(*variants_file)))
				result = true;
			function_136bb0(&file);
		}

		if (!result)
			function_136860(&file);
		else
			memcpy(g_551ae8, variants_file->variants, sizeof(g_551ae8));
	}

	return result;
}

// @retail 0x1943d0
bool game_variant_block_read(long index, s_game_variant_block *block)
{
	bool result = false;

	if (game_variants_available() && function_1934f0(&g_551ae8[index]))
	{
		s_type_acf665 file;
		dword error;

		function_193fa0(&file);
		if (function_136970(&file, 1, &error))
		{
			if (function_136bf0(&file, sizeof(g_551ae8) + index * sizeof(s_game_variant_block), true) &&
				function_136ca0(&file, block, sizeof(*block), true))
			{
				result = true;
			}
			function_136bb0(&file);
		}
	}

	return result;
}

void utf8_string_to_utf16_string(const char *source, word *destination, long destination_count);

// @retail 0x1944c0
bool game_variant_get_name(long index, word *name)
{
	bool result = false;

	if (game_variant_valid(index))
	{
		utf8_string_to_utf16_string(g_551ae8[index].names[get_current_language()], name, 0x10);
		result = true;
	}
	return result;
}

// @retail 0x194540
bool game_variant_get_description(long index, word *description)
{
	bool result = false;

	if (game_variant_valid(index))
	{
		utf8_string_to_utf16_string(g_551ae8[index].descriptions[get_current_language()], description, 0x80);
		result = true;
	}
	return result;
}

struct s_entry_c;
s_entry_c *function_19c5f0(long map_id);

// @retail 0x194660
long game_variant_check_maps(long index)
{
	s_game_variant_block block;
	long status = 0;

	if (game_variants_available())
	{
		if (game_variant_block_read(index, &block))
		{
			status = 1;
			for (long i = 0; i < block.map_count; i++)
			{
				if (!function_19c5f0(block.maps[i].map_id))
					status = 2;
			}
		}
		else
		{
			status = 2;
		}
	}

	g_551ae8[index].field_4 = status;
	return status;
}

// @retail 0x192df0
long game_variant_get_map_status(long index)
{
	long status = 0;

	if (index != NONE)
	{
		if (game_variant_valid(index))
		{
			s_surface_description *variant = &g_551ae8[index];

			if (function_1934f0(variant) && variant->field_4 == 0)
				game_variant_check_maps(index);
			if (function_1934f0(variant))
				return variant->field_4;
		}
		status = 3;
	}

	return status;
}

static inline short random_range(dword *seed, short lower, short upper)
{
	return (short)(lower + ((random_next(seed) * (upper - lower)) >> 16));
}

// @retail 0x192eb0
bool game_variant_choose_map(long index, long *map_id, byte *settings)
{
	s_game_variant_block block;
	long weights[100];
	bool result = false;

	if (game_variant_block_read(index, &block))
	{
		long selected = NONE;
		long total = 0;
		long i;

		for (i = 0; i < block.map_count; i++)
		{
			long weight = 0;

			if (function_19c5f0(block.maps[i].map_id))
			{
				weight = block.maps[i].weight;
				if (weight < 1)
					weight = 1;
				else if (weight > 1000)
					weight = 1000;
			}
			total += weight;
			weights[i] = weight;
		}

		if (total <= 0)
			return false;

		long choice = random_range(&g_4e7408->seed, 1, (short)(total + 1));
		long sum = 0;

		for (i = 0; i < block.map_count && selected == NONE; i++)
		{
			sum += weights[i];
			if (sum >= choice)
				selected = i;
		}

		if (selected < 0 || selected >= block.map_count)
			return false;

		*map_id = block.maps[selected].map_id;
		memcpy(settings, block.maps[selected].settings, sizeof(block.maps[selected].settings));
		return true;
	}

	return result;
}
