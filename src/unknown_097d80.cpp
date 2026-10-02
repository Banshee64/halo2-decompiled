#include "cseries.h"
#include "globals.h"

// @flags /O2 /Gr

/* ---- class A: an aggregate of three child objects ---- */
struct c_child;

struct s_block_item
{
	c_child *object;
	long unknown04;
};

struct s_block
{
	byte unknown00[0x3c];
	long count;
	long unknown40;
	s_block_item items[7];
	long unknown7c;
};

struct c_child
{
	virtual void v0(long, long) {}
	virtual bool v1(c_child *, long *) { return false; }
	virtual void v2() {}
	virtual void v3() {}
	virtual void v4() {}
	virtual long v5(dword, void *, long, s_block *, long *) { return 0; }
	virtual void v6(s_block *) {}
	virtual void v7(void *) {}
	virtual void v8(long, long) {}
};

struct s_request
{
	byte unknown00[0xc];
	long type;
};

struct c_aggregate : c_child
{
	byte unknown04[8];
	c_child *children[3];

	long function_97d80(dword *source, s_request *request);
	void v7(void *a);
	void v8(long a, long b);

};


// @retail 0x97d80
long c_aggregate::function_97d80(dword *source, s_request *request)
{
	s_block blocks[256];
	long starts[4];
	long error = 0;
	long total = 0;

	c_child **child = children;
	for (long i = 0; i < 3; i++, child++)
	{
		starts[i] = total;
		if (*child != 0 && error == 0)
		{
			long produced = 0;
			error = (*child)->v5(*source, request, 256 - total, &blocks[total], &produced);
			total += produced;
		}
	}
	starts[3] = total;

	if (error == 0 && request->type != 4)
	{
		child = children;
		for (long i = 0; i < 3; i++, child++)
		{
			c_child *object = *child;
			for (long j = starts[i]; j < starts[i + 1]; j++)
				object->v6(&blocks[j]);
		}
	}

	for (long k = 0; k < total; k++)
	{
		s_block *block = &blocks[k];
		for (long j = 0; j < block->count; j++)
		{
			c_child *object = block->items[j].object;
			if (object != 0)
			{
				long result;
				g_4d87f8->allocator->get_info(object, &result);
				g_4d87f8->allocator->release(object, NONE);
				if (object != 0)
					g_4d87f8->count--;
				block->items[j].object = 0;
			}
		}
	}

	return error;
}

// @retail 0x97f00
void c_aggregate::v7(void *a)
{
	for (long i = 0; i < 3; i++)
	{
		c_child *child = children[i];
		if (child != 0)
			child->v7(a);
	}
}

// @retail 0x97f30
void c_aggregate::v8(long a, long b)
{
	for (long i = 0; i < 3; i++)
	{
		c_child *child = children[i];
		if (child != 0)
			child->v8(a, b);
	}
}

/* ---- class B: a handle table ---- */
struct s_table_item
{
	byte flags;
	byte unknown01;
	word mask;
	dword unknown04;
};

struct c_owner
{
	virtual void v0() = 0;
	virtual void v1() = 0;
	virtual void v2() = 0;
	virtual void v3() = 0;
	virtual void v4() = 0;
	virtual void v5() = 0;
	virtual void v6() = 0;
	virtual void v7() = 0;
	virtual void v8() = 0;
	virtual void v9() = 0;
	virtual void v10() = 0;
	virtual void v11() = 0;
	virtual void v12(long handle) = 0;
};

struct s_table
{
	c_owner *owner;
	byte unknown04[0x40];
	s_table_item items[1024];
};

struct s_entry
{
	long handle;
	dword unknown04;
	word state;
	short unknown0a;
	dword unknown0c;
	word unknown10;
	word unknown12;
};

struct c_handle_table
{
	virtual void v0() = 0;
	virtual void v1() = 0;
	virtual void v2() = 0;
	virtual void v3() = 0;
	virtual void v4() = 0;
	virtual void v5() = 0;
	virtual void v6() = 0;
	virtual void v7() = 0;
	virtual void v8(long handle, long a) = 0;

	byte unknown04[4];
	byte unknown08;
	byte unknown09;
	byte unknown0a;
	long shift;
	long bit;
	s_table *table;
	long *head;
	long unknown1c;
	long unknown20;
	s_entry entries[1024];
	long unknown5024;
	long unknown5028;
	long unknown502c;
	long unknown5030;
	long unknown5034;
	long unknown5038;
	long unknown503c;

	void function_97fe0();
	void function_980d0(long handle, dword mask);
	bool function_98120();
};

// @retail 0x97f60
void function_97f60(s_table *t, long a, c_handle_table *self)
{
	self->shift = a;
	self->bit = 1 << a;
	self->table = t;
	self->head = 0;
	self->unknown1c = 0;
	for (long i = 0; i < 1024; i++)
	{
		self->entries[i].handle = NONE;
		self->entries[i].unknown04 = 0;
		self->entries[i].state = 0;
		self->entries[i].unknown0a = NONE;
		self->entries[i].unknown0c = 0;
		self->entries[i].unknown10 = 0;
	}
	self->unknown5024 = NONE;
	self->unknown502c = 0;
	self->unknown5028 = 0;
	self->unknown503c = 0;
	self->unknown5038 = 0;
	self->unknown5034 = 0;
	self->unknown5030 = 0;
	self->unknown09 = 0;
	self->unknown0a = 0;
	self->unknown08 = 1;
}

// @retail 0x97fe0
void c_handle_table::function_97fe0()
{
	while (head != 0)
		v8(*head, 0);

	s_entry *entry = entries;
	for (long i = 0; i < 1024; i++, entry++)
	{
		if (entry->state != 0)
		{
			long handle = entry->handle;
			s_table_item *item = &table->items[handle & 0x3ff];
			word mask = item->mask;
			if (mask & (1 << shift))
			{
				item->mask = mask & ~(1 << shift);
				s_table *t = table;
				if (t->items[handle & 0x3ff].mask == 0)
				{
					t->owner->v12(handle);
					t->items[handle & 0x3ff].flags &= 0xfe;
				}
			}
			entry->state = 0;
			entry->handle = NONE;
			entry->unknown0a = NONE;
			entry->unknown10 = 0;
		}
	}

	unknown502c = 0;
	unknown5028 = 0;
	unknown5034 = 0;
	unknown5030 = 0;
	unknown5024 = NONE;
}

// @retail 0x980d0
void c_handle_table::function_980d0(long handle, dword mask)
{
	s_entry *entry = &entries[handle & 0x3ff];
	if (entry->state == 3)
	{
		word item_mask = table->items[handle & 0x3ff].mask;
		dword value = entry->unknown04;
		if (!((1 << shift) & item_mask))
		{
			if (value == 0)
				unknown5034++;
			value = entry->unknown04;
		}
		entry->unknown04 = value | mask;
	}
	else
		entry->unknown04 |= mask;
}

// @retail 0x98120
bool c_handle_table::function_98120()
{
	bool result = false;
	if (unknown09)
		result = (unknown503c != 0) | (unknown5034 != 0) | (unknown502c != 0);
	return result;
}
