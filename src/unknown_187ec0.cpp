// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_187EC0.CPP: queries on the first local player, and the
   lookups of the globals' block at +0x150 of g_4e034c (0xb4 byte elements) */

#include "cseries.h"
#include "globals.h"
#include <math.h>

#define k_maximum_local_players 4

struct s_player_datum
{
	byte unknown00[0x24];
	long index24;
	byte unknown28[4];
	long unit_index;
	byte unknown30[0x21c - 0x30];
};

struct s_player_record
{
	byte unknown00[0x1a];
	bool flag1a;
	byte unknown1b;
};

extern byte g_51ea18[];

struct s_pitch_object
{
	byte unknown00[0x18c];
	real_vector3d vector;
};

struct s_object_header
{
	byte unknown00[8];
	s_pitch_object *object;
};

static inline long local_player_first_index(void)
{
	for (long i = 0; i < k_maximum_local_players; i++)
	{
		if (g_4e8c20->entries[i] != NONE)
		{
			return i;
		}
	}
	return NONE;
}

static inline s_player_datum *player_get(long player_index)
{
	return (s_player_datum *)g_4e8c24->data + (player_index & 0xffff);
}

// @retail 0x187ec0
bool function_187ec0(void)
{
	bool result = false;
	long index = local_player_first_index();

	if (index != NONE)
	{
		long record_index = player_get(g_4e8c20->entries[index])->index24;
		if (record_index != NONE)
		{
			s_player_record record = ((s_player_record *)g_51ea18)[record_index];
			result = record.flag1a;
		}
	}
	return result;
}

// @retail 0x187f30
real function_187f30(void)
{
	long index = local_player_first_index();

	if (index != NONE)
	{
		long player_index = g_4e8c20->entries[index];
		bool valid = player_index != NONE;
		if (valid)
		{
			long unit_index = player_get(player_index)->unit_index;
			if (unit_index != NONE)
			{
				real_vector3d vector = ((s_object_header *)g_4e0300->data)[unit_index & 0xffff].object->vector;
				return (real)atan2(vector.k, sqrt(vector.i * vector.i + vector.j * vector.j));
			}
		}
	}
	return 0.0f;
}

/* the 0xb4 byte elements of the block at +0x150 of g_4e034c */
struct s_globals_element
{
	long key;
	byte unknown04[0xb0];
};

struct s_globals_element_block_view
{
	byte unknown00[0x150];
	long count;
	s_globals_element *elements;
};

/* an index into a tag block, returned through memory */
class c_block_index
{
public:
	c_block_index(short index) : m_index(index) {}
	short m_index;
};

// @retail 0x1885f0
c_block_index function_1885f0(s_globals_element_block_view const *globals, long key)
{
	for (long i = 0; i < globals->count; i++)
	{
		if (key == globals->elements[i].key)
		{
			return c_block_index((short)i);
		}
	}
	return c_block_index(NONE);
}

static inline s_globals_element *globals_element_get(s_globals_element_block_view const *globals, short index)
{
	s_globals_element *result = NULL;

	if (index != NONE && index >= 0 && index < globals->count)
	{
		result = &globals->elements[index];
	}
	return result;
}

// @retail 0x188640
s_globals_element *function_188640(long key)
{
	s_globals_element_block_view *globals = (s_globals_element_block_view *)g_4e034c;

	return globals_element_get(globals, function_1885f0(globals, key).m_index);
}

// @retail 0x188690
s_globals_element *function_188690(short index)
{
	return globals_element_get((s_globals_element_block_view *)g_4e034c, index);
}