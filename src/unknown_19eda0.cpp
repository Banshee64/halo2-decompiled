#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_0259d0.h"

// @flags /O2 /arch:SSE /Gr

/* UNKNOWN_19EDA0.CPP: the multiplayer weapon choices (a weapon type names
   one of the multiplayer globals' weapon references, or a random one) and
   weighted random choices from a tag's list */

struct s_tag_reference_view
{
	dword group_tag;
	long index;
};

/* the multiplayer globals' weapon references */
struct s_multiplayer_weapons
{
	s_tag_reference_view references[18];
};

struct s_multiplayer_runtime_globals
{
	byte unknown00[0x6c];
	s_multiplayer_weapons *weapons;
};

struct s_multiplayer_globals_definition
{
	byte unknown00[0xc];
	s_multiplayer_runtime_globals *runtime;
};

/* the weapon of a weapon type: none, a random one other than the excluded
   one, one of the globals' weapons, or the default */
// @retail 0x19eda0
long function_19eda0(long default_weapon, char type, long excluded)
{
	s_multiplayer_runtime_globals *runtime = ((s_multiplayer_globals_definition *)g_4e3b44[g_4e034c->index & 0xffff].bytes)->runtime;

	switch (type)
	{
	case 1:
		return NONE;
	case 2:
	{
		long result;
		do
		{
			result = function_19eda0(default_weapon, (char)(random_index(&g_4e7408->unknown0, 16) + 3), excluded);
		} while (result == excluded && excluded != NONE || result == NONE);
		return result;
	}
	case 3:
		return runtime->weapons->references[3].index;
	case 4:
		return runtime->weapons->references[0].index;
	case 5:
		return runtime->weapons->references[1].index;
	case 6:
		return runtime->weapons->references[6].index;
	case 7:
		return runtime->weapons->references[7].index;
	case 8:
		return runtime->weapons->references[5].index;
	case 9:
		return runtime->weapons->references[2].index;
	case 10:
		return runtime->weapons->references[4].index;
	case 11:
		return runtime->weapons->references[12].index;
	case 12:
		return runtime->weapons->references[9].index;
	case 13:
		return runtime->weapons->references[10].index;
	case 14:
		return runtime->weapons->references[13].index;
	case 15:
		return runtime->weapons->references[11].index;
	case 16:
		return runtime->weapons->references[15].index;
	case 17:
		return runtime->weapons->references[14].index;
	case 18:
		return runtime->weapons->references[8].index;
	case 19:
		return runtime->weapons->references[16].index;
	case 20:
		return runtime->weapons->references[17].index;
	default:
		return default_weapon;
	}
}

struct s_weighted_choice
{
	real weight;
	byte unknown04[4];
	long value;
	long extra;
};

struct s_weighted_choice_block
{
	long count;
	s_weighted_choice *choices;
};

// @retail 0x19f130
real weighted_choices_total(s_weighted_choice_block *block)
{
	long count = block->count;
	s_weighted_choice *choices = block->choices;
	real total = 0.0f;

	for (long i = 0; i < count; i++)
		total = choices[i].weight + total;

	return total;
}

// @retail 0x19f1a0
long weighted_choice_random(long tag_index, long *extra)
{
	s_weighted_choice_block *block = (s_weighted_choice_block *)g_4e3b44[tag_index & 0xffff].bytes;
	long count = block->count;
	s_weighted_choice *choices = block->choices;
	long result = NONE;
	real total = weighted_choices_total(block);

	if (count > 1)
	{
		real value = (real)random_index(&g_4e7408->unknown0, (short)(long)total);

		for (long i = 0; i < count; i++)
		{
			value -= choices[i].weight;
			if (value <= 0.0f)
			{
				result = choices[i].value;
				if (extra)
					*extra = choices[i].extra;
				break;
			}
		}
	}
	else if (count > 0)
	{
		result = choices[0].value;
		if (extra)
			*extra = choices[0].extra;
	}

	return result;
}
