// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_2963F0.CPP: the search of a graph's weapon types with the "any"
   names as fallbacks */

#include "unknown_11c920.h"
#include "unknown_1dacb0.h"
#include "data_array.h"
#include "globals.h"
#include "slot_handler.h"
#include <string.h>


#define ANY_NAME 0x30000d9

// @retail 0x2963f0
s_graph_weapon_type *graph_weapon_type_iterate(s_graph_weapon_type_iterator *iterator, long *found_mode,
	long *found_weapon_class, long *found_weapon_type)
{
	s_graph_weapon_type *result = NULL;

	do
	{
		if (iterator->step >= 8)
		{
			break;
		}
		if (!iterator->mode_entry)
		{
			if (iterator->step & 4)
			{
				if (iterator->mode == ANY_NAME)
				{
					goto mode_found;
				}
				*found_mode = ANY_NAME;
			}
			else
			{
				*found_mode = iterator->mode;
			}
			iterator->mode_entry = (s_graph_mode_entry *)function_1dd560((s_sorted_array *)&iterator->graph->mode_count,
				*found_mode, 0x14);
			iterator->class_entry = NULL;
		}
	mode_found:
		if (iterator->mode_entry)
		{
			if (!iterator->class_entry)
			{
				if (iterator->step & 2)
				{
					if (iterator->weapon_class == ANY_NAME)
					{
						goto class_found;
					}
					*found_weapon_class = ANY_NAME;
				}
				else
				{
					*found_weapon_class = iterator->weapon_class;
				}
				iterator->class_entry = (s_graph_mode_entry *)function_1dd560(
					(s_sorted_array *)&iterator->mode_entry->child_count, *found_weapon_class, 0x14);
			}
		class_found:
			if (iterator->class_entry)
			{
				if (iterator->step & 1)
				{
					if (iterator->weapon_type == ANY_NAME)
					{
						goto type_found;
					}
					*found_weapon_type = ANY_NAME;
				}
				else
				{
					*found_weapon_type = iterator->weapon_type;
				}
				result = (s_graph_weapon_type *)function_1dd560((s_sorted_array *)&iterator->class_entry->child_count,
					*found_weapon_type, 0x34);
			type_found:
				iterator->step++;
				if (!(iterator->step & 1))
				{
					iterator->class_entry = NULL;
					if (!(iterator->step & 2))
					{
						iterator->mode_entry = NULL;
					}
				}
			}
			else
			{
				iterator->step = (iterator->step + 2) & ~1;
				if (!(iterator->step & 2))
				{
					iterator->mode_entry = NULL;
				}
			}
		}
		else
		{
			iterator->step = (iterator->step + 4) & ~3;
			iterator->class_entry = NULL;
		}
	}
	while (!result);
	return result;
}

/* Object, pool, and tree utilities. */

extern void *g_5047f4;

struct s_295970_state
{
	short field00;
	short field02;
	short field04;
	short field06;
	short field08;
	short field0a;
	long field0c;
	short field10;
	short counts[3];
	long indices[3];
};

// @retail 0x295970
void function_295970(void)
{
	s_295970_state *state = (s_295970_state *)g_5047f4;
	state->field00 = 0;
	state->field02 = 0;
	state->field08 = 0;
	state->field06 = 0;
	state->field0c = NONE;
	long i = 0;
	do
	{
		state->counts[i] = 0;
		state->indices[i] = NONE;
		i++;
	}
	while (i < 3);
}

char *function_11c9c0(char *buffer, long size, char const *format, ...);

// @retail 0x296270
s_record_pool *function_296270(char const *name, long size, long count)
{
	s_record_pool *result = NULL;
	char label[256];
	function_11c9c0(label, sizeof(label), "%s reference", name);
	size += 12;
	if (size == 32)
		result = data_new(label, count, size, 5, g_510c2c);
	else
		result = data_new(label, count, size, 0, g_510c2c);
	return result;
}

struct s_296230_object
{
	byte unknown00[0xaa];
	byte type;
	byte unknownab[0x130 - 0xab];
	short field130;
	byte unknown132[0x1d4 - 0x132];
	short field1d4;
};

// @retail 0x296230
short function_296230(long object_index)
{
	short result = NONE;
	s_296230_object *object = (s_296230_object *)object_get(object_index);
	if (object->type == 6)
		result = object->field130;
	else if (object->type == 7)
		result = object->field1d4;
	return result;
}

struct s_296330_entry
{
	long unknown00;
	long key;
	long next;
};

// @retail 0x2962e0
void function_2962e0(s_record_pool *pool, long *head, long key, long size, void const *value)
{
	long index = *head;
	while (index != NONE)
	{
		s_296330_entry *entry = (s_296330_entry *)(pool->data + (index & 0xffff) * pool->size);
		if (entry->key == key)
		{
			memcpy(entry + 1, value, size);
			break;
		}
		index = entry->next;
	}
}

// @retail 0x296330
void function_296330(s_record_pool *pool, long *head, long key)
{
	while (*head != NONE)
	{
		s_296330_entry *entry = (s_296330_entry *)(pool->data + (*head & 0xffff) * pool->size);
		if (entry->key == key)
		{
			long next = entry->next;
			record_pool_release(pool, *head);
			*head = next;
			break;
		}
		head = &entry->next;
	}
}

// @retail 0x296370
bool function_296370(long *head, s_record_pool *pool, long key)
{
	bool result = false;
	long index = *head;
	while (index != NONE)
	{
		s_296330_entry *entry = (s_296330_entry *)(pool->data + (index & 0xffff) * pool->size);
		if (entry->key == key)
		{
			result = true;
			break;
		}
		index = entry->next;
	}
	return result;
}

// @retail 0x2963b0
void function_2963b0(s_record_pool *pool, long index)
{
	while (index != NONE)
	{
		s_296330_entry *entry = (s_296330_entry *)(pool->data + (index & 0xffff) * pool->size);
		long next = entry->next;
		record_pool_release(pool, index);
		index = next;
	}
}

struct s_296520_node
{
	point2f normal;
	real distance;
	short children[2];
};

struct s_296520_tree
{
	long count;
	s_296520_node *nodes;
};

// @retail 0x296520
long function_296520(s_296520_tree *tree, long index, point2f const *point)
{
	while (!(index & 0x8000))
	{
		s_296520_node *node = &tree->nodes[index & 0x7fff];
		real distance = node->normal.y * point->y + node->normal.x * point->x - node->distance;
		index = node->children[distance >= 0.0f ? 1 : 0];
	}
	if (index != NONE)
		return index & 0x7fff;
	return NONE;
}
