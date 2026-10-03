#include "cseries.h"
#include "globals.h"
#include "slot_owner.h"

// @flags /O2 /Gr

/* an iterator over the players (g_4e8c24) that skips the players whose flag
   at +2 is set (its first parameter is passed as a pointer to longs, as the
   callers declare it) */
struct s_player_iterator
{
	byte *datum;
	s_data_array *data;
	long datum_index;
	long index;
};

/* the players (0x21c bytes each) and their units' item slots */
struct s_player_record
{
	byte unknown00[0x2c];
	long unit_index;
};

struct s_item
{
	long tag_index;
};

struct s_unit_object
{
	byte unknown00[0x218];
	long items[4];
};

struct s_item_object_header
{
	short identifier;
	byte unknown02[6];
	void *object;
};

struct s_item_tag
{
	byte unknown00[0x290];
	short type;
};

struct s_object_header
{
	byte unknown00[8];
	byte *object;
};

/* data_iterator_next, as these iterators inline it: once with its call to
   data_next_absolute_index, then with that inlined too */
static inline byte *player_iterator_first(s_data_iterator *iterator)
{
	s_data_array *data = iterator->data;
	long index = data_next_absolute_index(data, iterator->index + 1);
	byte *result;

	if (index != NONE)
	{
		result = data->data + data->size * index;
		iterator->index = index;
		iterator->datum_index = (*(short *)result << 16) | index;
	}
	else
	{
		iterator->index = data->maximum_count;
		iterator->datum_index = NONE;
		result = 0;
	}
	return result;
}

// @retail 0x19f240
bool function_19f240(long *iterator_)
{
	s_player_iterator *iterator = (s_player_iterator *)iterator_;
	s_data_iterator *data_iterator = (s_data_iterator *)&iterator->data;

	iterator->datum = player_iterator_first(data_iterator);
	while (iterator->datum && (iterator->datum[2] & 2))
		iterator->datum = data_iterator_next_inlined(data_iterator);

	return iterator->datum != 0;
}

// @retail 0x19f300
bool function_19f300(long *iterator_)
{
	s_player_iterator *iterator = (s_player_iterator *)iterator_;
	s_data_iterator *data_iterator = (s_data_iterator *)&iterator->data;

	iterator->datum = player_iterator_first(data_iterator);
	while (iterator->datum && ((s_player_record *)iterator->datum)->unit_index == NONE)
		iterator->datum = data_iterator_next_inlined(data_iterator);

	return iterator->datum != 0;
}

// @retail 0x19f3c0
long function_19f3c0(long player_index, long type)
{
	s_player_record *player = (s_player_record *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c);
	long result = NONE;

	if (player->unit_index != NONE)
	{
		s_object_header *headers = (s_object_header *)g_4e0300->data;
		s_unit_object *unit = (s_unit_object *)headers[player->unit_index & 0xffff].object;
		long i;

		for (i = 0; i < 4; i++)
		{
			long item_index = unit->items[i];

			if (item_index != NONE)
			{
				s_item *item = (s_item *)headers[item_index & 0xffff].object;
				s_item_tag *tag = (s_item_tag *)g_4e3b44[item->tag_index & 0xffff].bytes;

				if (tag->type == type)
				{
					result = item_index;
					break;
				}
			}
		}
	}

	return result;
}

// @retail 0x1a6fe0
short function_1a6fe0(long owner_index, short type)
{
	s_slot_owner_entry *owner = (s_slot_owner_entry *)(g_4f55f0->data + (owner_index & 0xffff) * sizeof(s_slot_owner_entry));
	short count = owner->current;
	short result = NONE;
	short i;

	for (i = 0; i <= count; i++)
	{
		if (owner->slots[i].type == type)
		{
			result = i;
			break;
		}
	}

	return result;
}
