#include "cseries.h"
#include <string.h>
#include "globals.h"
#include "data_array.h"
#include "physical_memory.h"

// @flags /O2 /Gr

/* the physical memory allocator (physical_memory.h): a named, doubly linked
   list of blocks kept in a data array that follows the object */

// @retail 0x13d170
void function_13d170(s_physical_object *manager, const char *name, long page_count, long page_shift, long maximum_count, physical_block_delete_proc delete_proc, physical_block_busy_proc busy_proc, physical_block_state_proc state_proc, c_data_allocator *allocator)
{
	s_data_array *data = (s_data_array *)(manager + 1);

	data_initialize(data, name, maximum_count, sizeof(s_physical_block), 0, g_46875c);
	data->valid = 1;
	data_delete_all(data);

	memset(manager, 0, sizeof(s_physical_object));
	strncpy(manager->name, name, 0x20);
	manager->delete_proc = delete_proc;
	manager->state_proc = state_proc;
	manager->page_shift = page_shift;
	manager->busy_proc = busy_proc;
	manager->blocks = data;
	manager->time = 1;
	manager->page_count = page_count;
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
		physical->block_delete(iterator.datum_index);
	}
}

/* frees a block: the owner's delete callback, then unlinks it */
// @retail 0x13d830
void s_physical_object::block_delete(long handle)
{
	s_physical_block *entry = &((s_physical_block *)blocks->data)[handle & 0xffff];

	if (delete_proc)
		delete_proc(handle);

	if (entry->previous != NONE)
		((s_physical_block *)blocks->data)[entry->previous & 0xffff].next = entry->next;
	else
		first = entry->next;

	if (entry->next != NONE)
	{
		((s_physical_block *)blocks->data)[entry->next & 0xffff].previous = entry->previous;
		datum_delete(blocks, handle);
	}
	else
	{
		last = entry->previous;
		datum_delete(blocks, handle);
	}
}

/* resizes the allocator to a number of pages, freeing the blocks past its end */
// @retail 0x13d8b0
void s_physical_object::method_13d8b0(long pages)
{
	s_data_iterator iterator;
	s_physical_block *block;

	iterator.data = blocks;
	iterator.index = NONE;
	iterator.datum_index = NONE;
	while ((block = (s_physical_block *)data_iterator_next_inlined(&iterator)) != NULL)
	{
		if (block->offset + block->pages > pages)
		{
			block_delete(iterator.datum_index);
		}
	}
	page_count = pages;
}

// @retail 0x13d950
long physical_memory_used_pages(s_physical_object *physical, long age)
{
	long result = 0;
	s_data_iterator iterator;
	s_physical_block *block;

	iterator.data = physical->blocks;
	iterator.index = NONE;
	iterator.datum_index = NONE;
	while ((block = (s_physical_block *)data_iterator_next_inlined(&iterator)) != NULL)
	{
		if ((dword)(block->time + age) >= (dword)physical->time || physical->busy_proc && physical->busy_proc(iterator.datum_index))
		{
			result += block->pages;
		}
	}
	return result;
}
