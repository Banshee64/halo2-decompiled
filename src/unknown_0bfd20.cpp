// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "globals.h"

struct s_object_hdr
{
	dword tag_index;
	byte unknown04[0xa0];
	dword key;
	short subkey;
	byte byte_aa;
	byte byte_ab;
	byte unknownac[0x24];
	short match_index;
};

/* the entries of the object header array (g_4e0300->headers) */
struct s_object_header
{
	byte unknown00[8];
	s_object_hdr *object;
};

struct s_match_entry
{
	dword key;
	short subkey;
	byte byte_06;
	byte byte_07;
	byte unknown08[0x54];
};

struct s_match_globals
{
	byte unknown00[0x22c];
	long count;
	s_match_entry *entries;
};

/* the tag flags (g_4e3b44's flags pointer); only the bit tested here (108fd0
   has the fuller view) */
struct s_tag_flags
{
	byte unknown00[3];
	unsigned char flag0 : 1;
};

s_match_globals *g_4e0348;
real g_547634;
real g_547638;

// @retail 0xbfd20
void function_0bfd20(word object_index)
{
	s_object_hdr *object = g_4e0300->headers[object_index].object;
	s_match_globals *globals = g_4e0348;
	if (TEST_FIELD_BIT(g_4e3b44[(object->tag_index & 0xffff)].flags->flag0))
	{
		s_match_entry *entry = globals->entries;
		for (long i = 0; i < globals->count; i++, entry++)
		{
			bool match = (entry->byte_06 == object->byte_aa) & (entry->byte_07 == object->byte_ab) & (entry->key == object->key);
			if (match && entry->byte_07 == 0)
				match &= entry->subkey == object->subkey;
			if (match)
			{
				object->match_index = (short)i;
				return;
			}
		}
	}
	object->match_index = NONE;
}

// @retail 0xbfe20
void function_0bfe20(dword *flags, long bit, bool value)
{
	if (value)
		*flags |= 1 << bit;
	else
		*flags &= ~(1 << bit);
}

// @retail 0xbfe40
void function_0bfe40(word *flags, long bit, bool value)
{
	if (value)
		*flags |= (word)(1 << bit);
	else
		*flags &= (word)~(1 << bit);
}

// @retail 0xbfe60
bool function_0bfe60(long bit, const dword *flags)
{
	return (flags[bit >> 5] & (1 << (bit & 31))) != 0;
}

// @retail 0xbfe80
void function_0bfe80(dword *flags)
{
	for (long i = 0; i < 8; i++)
		flags[i] = ~flags[i];
	flags[7] &= 0xff;
}

// @retail 0xbfed0
void function_0bfed0(long count, dword *flags)
{
	long last = ((count + 31) >> 5) - 1;
	for (long i = last + 1; i < 8; i++)
		flags[i] = 0;
	long remainder = count & 31;
	dword mask = 0xffffffff;
	if (remainder > 0)
		mask >>= 32 - remainder;
	flags[last] &= mask;
}

struct s_bit_vector
{
	dword bits[8];

	bool is_empty() const;
};

// @retail 0xbff10
bool s_bit_vector::is_empty() const
{
	bool result = bits[7] == 0;
	result = result & (bits[6] == 0);
	result = result & (bits[5] == 0);
	result = result & (bits[4] == 0);
	result = result & (bits[3] == 0);
	result = result & (bits[2] == 0);
	result = result & (bits[1] == 0);
	result = result & (bits[0] == 0);
	return result;
}

// @retail 0xbff60
real function_0bff60(real a, real b)
{
	real d = a - b;
	if (d >= g_547638)
		d -= g_547634;
	if (-g_547638 >= d)
		d = g_547634 + d;
	return d;
}
