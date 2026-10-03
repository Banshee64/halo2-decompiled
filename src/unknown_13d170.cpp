#include "cseries.h"
#include <string.h>
#include "globals.h"
#include "data_array.h"
#include "physical_memory.h"

// @flags /O2 /Gr

/* the physical memory allocator (physical_memory.h): a named, doubly linked
   list of blocks kept in a data array that follows the object */

// @retail 0x13d170
void function_13d170(s_physical_object *manager, const char *name, long a3, long page_shift, long maximum_count, physical_block_delete_proc delete_proc, physical_block_busy_proc busy_proc, long a8, c_data_allocator *allocator)
{
	s_data_array *data = (s_data_array *)(manager + 1);

	data_initialize(data, name, maximum_count, sizeof(s_physical_block), 0, g_46875c);
	data->valid = 1;
	data_delete_all(data);

	memset(manager, 0, sizeof(s_physical_object));
	strncpy(manager->name, name, 0x20);
	manager->delete_proc = delete_proc;
	manager->unknown28 = a8;
	manager->page_shift = page_shift;
	manager->busy_proc = busy_proc;
	manager->blocks = data;
	manager->time = 1;
	manager->unknown30 = a3;
	manager->signature = 0x77656565;
	manager->name[0x1f] = 0;
	manager->state = 0;
	manager->allocator = allocator;
	manager->first = NONE;
	manager->last = NONE;
	manager->limits[0] = 0x7fffffff;
	manager->limits[1] = 0x7fffffff;
	manager->limits[2] = 0x7fffffff;
	manager->limits[3] = 0x7fffffff;
	manager->limits[4] = 0x7fffffff;
	manager->limits[5] = 0x7fffffff;
	manager->limits[6] = 0x7fffffff;
	manager->limits[7] = 0x7fffffff;
}

// @retail 0x13d230
void physical_memory_reset_time(s_physical_object *physical)
{
	s_data_iterator iterator;
	s_physical_block *block;

	iterator.data = physical->blocks;
	physical->time = 0x101;
	iterator.index = NONE;
	iterator.datum_index = NONE;
	while ((block = (s_physical_block *)data_iterator_next_inlined(&iterator)) != NULL)
	{
		dword age = 0x7fffffff - block->time;

		if (age > 0x100)
		{
			block->time = 0;
		}
		else
		{
			block->time = 0x100 - age;
		}
	}
}

// @retail 0x13d2b0
void physical_memory_flush(s_physical_object *physical)
{
	s_data_iterator iterator;

	iterator.data = physical->blocks;
	iterator.index = NONE;
	iterator.datum_index = NONE;
	while (data_iterator_next_inlined(&iterator))
	{
		function_13d830(physical, iterator.datum_index);
	}
}

// @retail 0x13d830
void function_13d830(s_physical_object *manager, long handle)
{
	s_physical_block *entry = &((s_physical_block *)manager->blocks->data)[handle & 0xffff];

	if (manager->delete_proc)
		manager->delete_proc(handle);

	if (entry->previous != NONE)
		((s_physical_block *)manager->blocks->data)[entry->previous & 0xffff].next = entry->next;
	else
		manager->first = entry->next;

	if (entry->next != NONE)
	{
		((s_physical_block *)manager->blocks->data)[entry->next & 0xffff].previous = entry->previous;
		datum_delete(manager->blocks, handle);
	}
	else
	{
		manager->last = entry->previous;
		datum_delete(manager->blocks, handle);
	}
}
