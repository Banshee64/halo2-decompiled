#include "unknown_11c920.h"
#include "unknown_08b110.h"
#include "unknown_096ed0.h"
#include <string.h>

// @flags /O2 /Ob1 /Gr

// @retail 0x699a0
long function_699a0(long index, void *table)
{
	long handle = NONE;
	if (index < 16)
	{
		if (index != NONE && index >= 0 && index < g_4e8c24->high_water_index)
		{
			byte *record = g_4e8c24->data + g_4e8c24->size * index;
			if (*(word *)record != 0)
				handle = *(long *)(record + 0x2c);
		}
	}
	else if (index < 32)
	{
		byte *slot = (byte *)table + index * 0x90;
		if (*(long *)(slot - 0x90 + 0x8fc) != NONE)
			handle = *(long *)slot;
	}
	return handle;
}

// @retail 0x89e70
void function_89e70(s_node_450d1c *node, s_owner_450d1c *owner)
{
	s_node_450d1c **link = &owner->head;
	if (*link != 0)
	{
		do
		{
			s_node_450d1c *current = *link;
			if (current == node)
			{
				*link = node->next;
				break;
			}
			link = &current->next;
		}
		while (*link != 0);
	}
	owner->count--;
	if (node != 0)
		function_89eb0(node, 1);
}
/* frees a node and, when asked, the node itself */
// @retail 0x89eb0
s_node_450d1c *function_89eb0(s_node_450d1c *node, long flags)
{
	void *data = node->data;
	if (data)
	{
		long info;
		g_4d87f8->allocator->get_info(data, &info);
		s_allocator_globals *globals = g_4d87f8;
		globals->allocator->release(data, NONE);
		if (data)
			globals->count--;
	}
	if (flags & 1)
	{
		long info;
		g_4d87f8->allocator->get_info(node, &info);
		s_allocator_globals *globals = g_4d87f8;
		globals->allocator->release(node, NONE);
		globals->count--;
	}
	return node;
}

/* adds a new node at the end of the owner's list */
// @retail 0x89df0
s_node_450d1c *function_89df0(s_owner_450d1c *owner)
{
	s_node_450d1c *node = (s_node_450d1c *)handle_allocate(sizeof(s_node_450d1c));
	if (node)
	{
		node->unknown04 = NONE;
		node->timeout = NONE;
		node->unknown00 = 0;
		node->unknown10 = NONE;
		node->unknown14 = NONE;
		node->data = 0;
		node->size = 0;
		node->active_mask = 0;
		node->done_mask = 0;
		s_node_450d1c **link = &owner->head;
		while (*link)
			link = &(*link)->next;
		*link = node;
		node->next = 0;
		owner->count++;
		return node;
	}
	return 0;
}

/* gives a node a copy of some data */
// @retail 0x8b0a0
bool function_8b0a0(s_node_450d1c *node, long size, const void *source)
{
	bool result = false;
	void *block = handle_allocate(size);
	if (block)
	{
		node->size = size;
		node->data = block;
		memcpy(block, source, size);
		return true;
	}
	return result;
}
