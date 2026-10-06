#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_096ed0.h"

// @flags /O2 /arch:SSE /Gr

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
// @retail 0x97f60
void function_97f60(s_handle_peers *t, long a, c_handle_table_450cd0 *self)
{
	self->shift = a;
	self->bit = 1 << a;
	self->table = t;
	self->head = 0;
	self->node = 0;
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
void c_handle_table_450cd0::function_97fe0()
{
	while (head != 0)
		v8(head->handle, false);

	s_handle_entry *entry = entries;
	for (long i = 0; i < 1024; i++, entry++)
	{
		if (entry->state != 0)
		{
			long handle = entry->handle;
			s_handle_peer *item = &table->peers[handle & 0x3ff];
			word mask = item->mask;
			if (mask & (1 << shift))
			{
				item->mask = mask & ~(1 << shift);
				s_handle_peers *t = table;
				if (t->peers[handle & 0x3ff].mask == 0)
				{
					t->owner->v12(handle);
					t->peers[handle & 0x3ff].flags &= 0xfe;
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
void c_handle_table_450cd0::function_980d0(long handle, dword mask)
{
	s_handle_entry *entry = &entries[handle & 0x3ff];
	if (entry->state == 3)
	{
		word item_mask = table->peers[handle & 0x3ff].mask;
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
bool c_handle_table_450cd0::function_98120()
{
	bool result = false;
	if (unknown09)
		result = (unknown503c != 0) | (unknown5034 != 0) | (unknown502c != 0);
	return result;
}


struct s_98150
{
	long field_0;
	real field_4;
	long field_8;
	long field_c;
	long field_10;
};

long replication_table_get_chain(s_handle_peers *arg_0, long arg_1, long *arg_2);



// @retail 0x98150
long c_handle_table_450cd0::v1(long arg_0, long arg_1, void *arg_2)
{
	long local_0 = 0;
	if (unknown09)
	{
		long local_1 = unknown5024;
		if (local_1 != NONE)
		{
			s_98150 *local_2 = (s_98150 *)arg_2;
			while (local_1 != NONE && local_0 < arg_1)
			{
				real local_4;
				long local_5;
				long local_6;
				long local_7;
				if (!entries[local_1].state)
					goto local_20;
				{
					long local_8 = entries[local_1].handle;
					s_handle_peer *local_9 = &table->peers[local_8 & 0x3ff];
					if (entries[local_1].state == 1)
					{
						if (local_9->flags & 8)
						{
							long local_10[4];
							bool local_11 = false;
							local_6 = 3;
							local_7 = replication_table_get_chain(table, local_8, local_10);
							local_4 = 0.0f;
							local_5 = 0;
							for (long local_12 = 0; local_12 < local_7; local_12++)
							{
								real local_13;
								long local_14;
								table->owner->v3(local_10[local_12], entries[local_10[local_12] & 0x3ff].unknown04, arg_0, &local_13, &local_14);
								if (local_13 > 0.0f)
									local_4 = local_4 > local_13 ? local_4 : local_13;
								else
									local_11 = true;
								local_5 += local_14;
							}
							if (local_11)
								local_4 = 0.0f;
						}
						else if (!(local_9->flags & 0x10))
						{
							local_6 = 1;
							local_7 = 1;
							table->owner->v3(local_8, entries[local_1].unknown04, arg_0, &local_4, &local_5);
						}
						else
							goto local_20;
					}
					else if (entries[local_1].state == 3)
					{
						if ((1 << shift) & local_9->mask)
						{
							if (local_9->flags & 8)
							{
								long local_15[4];
								bool local_16 = false;
								local_6 = 4;
								local_7 = replication_table_get_chain(table, local_8, local_15);
								local_4 = 0.0f;
								local_5 = 0;
								for (long local_17 = 0; local_17 < local_7; local_17++)
								{
									real local_18;
									table->owner->v9(local_15[local_17], arg_0, &local_18);
									if (local_18 > 0.0f)
										local_4 = local_4 > local_18 ? local_4 : local_18;
									else
										local_16 = true;
								}
								if (local_16)
									local_4 = 0.0f;
							}
							else if (!(local_9->flags & 0x10))
							{
								local_6 = 2;
								local_7 = 1;
								table->owner->v9(local_8, arg_0, &local_4);
								local_5 = 0;
							}
							else
								goto local_20;
						}
						else if (entries[local_1].unknown04)
						{
							local_6 = 5;
							local_7 = 1;
							table->owner->v8(local_8, entries[local_1].unknown04, entries[local_1].unknown0c, arg_0, &local_4, &local_5);
						}
						else
							goto local_20;
					}
					else
						goto local_20;
				}
				if (local_4 > 0.0f)
				{
					s_98150 *local_19 = local_2++;
					local_0++;
					local_19->field_0 = *(long *)unknown04;
					local_19->field_10 = local_1;
					local_19->field_4 = local_4;
					local_19->field_c = local_6;
					local_19->field_8 = local_7 * 14 + local_5 + (local_7 > 1 ? 2 : 0) + 3;
				}
			local_20:
				local_1 = entries[local_1].unknown0a;
			}
		}
	}
	return local_0;
}
