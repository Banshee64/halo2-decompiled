// @flags /O2 /Gr
#include "cseries.h"
#include "globals.h"
#include "slot_handler.h"

/* the entries (0x3c bytes) of the actor's character tag block at +0xdc,
   keyed by a short at +4, and the actor's entry for its unit's variant */

struct s_character_variant_entry
{
	byte unknown00[4];
	short key;
	byte unknown06[0x3c - 0x6];
};

struct s_character_variants_view
{
	byte unknown00[8];
	long parent_index;
	byte unknown0c[0xdc - 0xc];
	long entry_count;
	s_character_variant_entry *entries;
};

/* the unit's byte at +0x23c */
struct s_unit_variant_view
{
	byte unknown000[0x23c];
	char variant;
};

void *function_1e53e0(long character_index, short key);

// @retail 0x1e5380
void *function_1e5380(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	void *result = 0;

	if (actor->unknown018 != NONE)
	{
		short variant = ((s_unit_variant_view *)object_get(actor->unknown018))->variant;

		if (variant != NONE)
			result = function_1e53e0(actor->unknown054, variant);
	}
	return result;
}

// @retail 0x1e53e0
void *function_1e53e0(long character_index, short key)
{
	while (character_index != NONE)
	{
		s_character_variants_view *character = (s_character_variants_view *)g_4e3b44[character_index & 0xffff].bytes;
		short i = 0;

		while (i < character->entry_count)
		{
			s_character_variant_entry *entry = &character->entries[i];

			if (entry->key == key)
				return entry;
			i++;
		}
		character_index = character->parent_index;
	}
	return 0;
}
