// @flags /O2 /Gr
/* UNKNOWN_08AD30.CPP: a 1024-entry table of handler-owned records */

#include "cseries.h"
#include <string.h>

class c_entry_handler
{
public:
	virtual void slot0() {}
	virtual void slot1() {}
	virtual long get_data_size() { return 0; }
	virtual long get_state_size() { return 0; }
	virtual long get_bit_count() { return 0; }
	virtual void slot5() {}
	virtual void slot6() {}
	virtual void slot7() {}
	virtual void slot8() {}
	virtual void slot9() {}
	virtual void slot10() {}
	virtual void slot11() {}
	virtual void slot12() {}
	virtual void slot13() {}
	virtual void slot14() {}
	virtual void slot15() {}
	virtual void slot16() {}
	virtual void slot17() {}
	virtual void slot18() {}
	virtual void save(long, long, long, void *) {}
	virtual bool load(void *entry, long *out, long size, void *buffer) { return false; }
	virtual void advance(void *entry) {}
	virtual void slot22() {}
	virtual void slot23() {}
	virtual void slot24() {}
	virtual bool is_valid(void *entry) { return false; }
};

class c_allocator
{
public:
	virtual void release(void *, long) {}
	virtual void get_info(void *, void *) {}
	virtual void slot2() {}
	virtual void slot3() {}
	virtual void slot4() {}
	virtual void *allocate(long, long, long) { return 0; }
	virtual void slot6() {}
	virtual void slot7() {}
	virtual void slot8() {}
	virtual void slot9() {}
	virtual void compact(long) {}
};

struct s_allocator_globals
{
	c_allocator *allocator;
	long count;
};

s_allocator_globals *g_4d87f8;

struct s_entry
{
	dword identifier;
	short handler_index;
	byte unknown06;
	byte unknown07;
	long unknown08;
	long unknown0c;
	long data_size;
	void *data;
	long state_size;
	void *state;
};

struct c_entry_table
{
	virtual bool function_08ad30(dword identifier);
	virtual void function_08ad70();
	virtual dword function_08add0(dword identifier);
	byte unknown04[0xc];
	struct { long count; c_entry_handler *handlers[1]; } *handlers;
	s_entry entries[1024];

	void function_08ae80(dword identifier, short handler_index, long a, long b, long c, long d);
	void function_08aed0(dword identifier);
	bool function_08af70(long handler_index, long *size, void **data);
	bool function_08b010(long handler_index, long *size, void **data);
};

#define ENTRY_INDEX(identifier) ((identifier) & 0x3ff)

// @retail 0x8ad30
bool c_entry_table::function_08ad30(dword identifier)
{
	s_entry *entry = &entries[ENTRY_INDEX(identifier)];
	c_entry_handler *handler = handlers->handlers[entry->handler_index];
	bool result = false;
	if (handler->is_valid(entry))
	{
		result = true;
	}
	return result;
}

// @retail 0x8ad70
void c_entry_table::function_08ad70()
{
	for (long i = 0; i < 1024; i++)
	{
		s_entry *entry = &entries[i];
		if (entry->identifier != NONE)
		{
			c_entry_handler *handler = handlers->handlers[entry->handler_index];
			byte salt = (byte)(((long)(entry->identifier >> 28) + 1) % 16);
			entry->identifier = (entry->identifier & 0x3ff) | ((dword)salt << 28);
			handler->advance(entry);
		}
	}
}

// @retail 0x8add0
dword c_entry_table::function_08add0(dword identifier)
{
	s_entry *entry = &entries[ENTRY_INDEX(identifier)];
	c_entry_handler *handler = handlers->handlers[entry->handler_index];
	dword result;
	long size = handler->get_data_size();
	byte buffer[1024];
	memset(buffer, 0, size);
	handler->save(entry->data_size, (long)entry->data, size, buffer);
	long bits = handler->get_bit_count();
	result = (1 << bits) - 1;
	if (!handler->load(entry, (long *)&result, size, buffer))
	{
		result = 0;
	}
	return result;
}

PRIVATE void free_block(void *block)
{
	long dummy;
	g_4d87f8->allocator->get_info(block, &dummy);
	s_allocator_globals *globals = g_4d87f8;
	globals->allocator->release(block, -1);
	if (block)
	{
		globals->count--;
	}
}

// @retail 0x8aed0
void c_entry_table::function_08aed0(dword identifier)
{
	s_entry *entry = &entries[ENTRY_INDEX(identifier)];
	if (entry->data)
	{
		free_block(entry->data);
		entry->data = 0;
		entry->data_size = 0;
	}
	if (entry->state)
	{
		free_block(entry->state);
		entry->state = 0;
		entry->state_size = 0;
	}
	entry->identifier = NONE;
	entry->handler_index = -1;
}

// @retail 0x8ae80
void c_entry_table::function_08ae80(dword identifier, short handler_index, long a, long b, long c, long d)
{
	s_entry *entry = &entries[ENTRY_INDEX(identifier)];
	entry->identifier = identifier;
	entry->handler_index = handler_index;
	entry->unknown06 = 0;
	entry->unknown0c = 0;
	entry->unknown07 = 0;
	entry->data_size = a;
	entry->data = (void *)b;
	entry->unknown08 = NONE;
	entry->state_size = c;
	entry->state = (void *)d;
}

PRIVATE void *allocate_block(long block_size)
{
	s_allocator_globals *globals = g_4d87f8;
	void *block = globals->allocator->allocate(block_size, 0, 0);
	if (!block)
	{
		globals->allocator->compact(0);
		block = globals->allocator->allocate(block_size, 0, 0);
	}
	if (block)
	{
		globals->count++;
	}
	return block;
}

// @retail 0x8af70
bool c_entry_table::function_08af70(long handler_index, long *size, void **data)
{
	bool result = true;
	void *block = 0;
	long block_size = handlers->handlers[handler_index]->get_state_size();
	if (block_size > 0)
	{
		s_allocator_globals *globals = g_4d87f8;
		block = globals->allocator->allocate(block_size, 0, 0);
		if (!block)
		{
			globals->allocator->compact(0);
			block = globals->allocator->allocate(block_size, 0, 0);
		}
		if (block)
		{
			globals->count++;
		}
		if (block)
		{
			memset(block, 0, block_size);
		}
		else
		{
			result = false;
		}
	}
	*size = block_size;
	*data = block;
	return result;
}

// @retail 0x8b010
bool c_entry_table::function_08b010(long handler_index, long *size, void **data)
{
	bool result = true;
	long block_size = handlers->handlers[handler_index]->get_data_size();
	void *block = allocate_block(block_size);
	if (block)
	{
		memset(block, 0, block_size);
	}
	else
	{
		result = false;
	}
	*size = block_size;
	*data = block;
	return result;
}
