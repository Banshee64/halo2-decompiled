/* UNKNOWN_2C0BF0.CPP: finding a named entry of the scenario's block at
   +0x110 */

#include "unknown_11c920.h"

// @flags /O2 /Gr

/* an entry of the block (0x34 bytes), named by its first bytes */
struct s_named_entry
{
	char name[0x20];
	byte unknown20[0x34 - 0x20];
};

/* the scenario, as this file reads it */
struct s_named_entry_owner
{
	byte unknown000[0x110];
	long count;
	s_named_entry *entries;
};

int function_11c920(char const *s1, char const *s2);

/* the index of the entry with the name (case aside), or NONE */
// @retail 0x2c0bf0
short named_entry_find(s_named_entry_owner const *owner, char const *name)
{
	short result = NONE;

	for (short i = 0; i < owner->count; i++)
	{
		s_named_entry const *entry = &owner->entries[i];

		if (!function_11c920(entry->name, name))
		{
			result = i;
			break;
		}
	}
	return result;
}
