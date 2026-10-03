// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1689B0.CPP: collision queries (0x1689b0..0x16b569): the flag
   conversions between the query flags and the collision tests' own, and the
   test of a surface against the query */

#include "cseries.h"
#include "globals.h"

#define FLAG(bit) (1 << (bit))
#define TEST_FLAG(flags, bit) (((flags) & FLAG(bit)) != 0)
#define SET_FLAG(flags, bit, value) ((value) ? ((flags) |= FLAG(bit)) : ((flags) &= ~FLAG(bit)))

/* a collision result, as read here */
struct s_collision_surface_entry
{
	byte unknown0[4];
	dword flags;
};

struct s_collision_result_view
{
	byte unknown00[0x98];
	long surface_count;
	s_collision_surface_entry *surfaces;
};

struct s_collision_index_block
{
	byte unknown00[0x40];
	long count;
	short *indices;
};

struct s_match_globals_collision_view
{
	byte unknown00[0xc4];
	long block_count;
	s_collision_index_block *blocks;
};

// @retail 0x1689b0
bool collision_surface_test(s_collision_result_view const *result, long surface_index, dword flags)
{
	bool valid = true;

	if (result->surface_count <= 0 || (result->surfaces[result->surface_count - 1].flags & 0x10))
	{
		valid = false;
	}
	else if (flags & 0x40000)
	{
		s_match_globals_collision_view *globals = (s_match_globals_collision_view *)g_4e0348;

		if (globals->block_count > 0 && globals->blocks)
		{
			s_collision_index_block *block = globals->blocks;

			if (surface_index < 0 || surface_index >= block->count || block->indices[surface_index * 2] == NONE)
			{
				valid = false;
			}
		}
	}
	return valid;
}

// @retail 0x168ae0
dword collision_flags_to_test_flags(dword flags)
{
	dword result = 0;

	SET_FLAG(result, 0, TEST_FLAG(flags, 23));
	SET_FLAG(result, 1, TEST_FLAG(flags, 24));
	SET_FLAG(result, 2, TEST_FLAG(flags, 25));
	SET_FLAG(result, 3, TEST_FLAG(flags, 26));
	SET_FLAG(result, 4, TEST_FLAG(flags, 27));
	SET_FLAG(result, 5, TEST_FLAG(flags, 28));
	return result;
}

// @retail 0x16b500
word collision_flags_to_object_flags(dword flags)
{
	word result = 0;

	SET_FLAG(result, 0, true);
	SET_FLAG(result, 1, TEST_FLAG(flags, 19));
	SET_FLAG(result, 2, TEST_FLAG(flags, 18));
	SET_FLAG(result, 3, TEST_FLAG(flags, 30));
	SET_FLAG(result, 4, TEST_FLAG(flags, 20));
	SET_FLAG(result, 5, TEST_FLAG(flags, 22));
	SET_FLAG(result, 6, TEST_FLAG(flags, 21));
	return result;
}
