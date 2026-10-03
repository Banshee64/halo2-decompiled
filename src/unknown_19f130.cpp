#include "cseries.h"
#include "globals.h"
#include "real_math.h"

// @flags /O2 /arch:SSE /Gr

/* UNKNOWN_19F130.CPP: weighted random choices from a tag's list */

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
