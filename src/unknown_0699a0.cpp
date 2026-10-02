#include "cseries.h"
#include "unknown_08b110.h"

// @flags /O2 /Ob1 /Gr

// @retail 0x699a0
long function_699a0(long index, void *table)
{
	long handle = NONE;
	if (index < 16)
	{
		if (index != NONE && index >= 0 && index < g_4e8c24->count)
		{
			byte *record = (byte *)g_4e8c24->players + g_4e8c24->size * index;
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