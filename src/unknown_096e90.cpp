#include "cseries.h"
#include <string.h>
#include "globals.h"
#include "unknown_08b110.h"

// @flags /O2 /Ob1 /arch:SSE /Gr

// @retail 0x96e90
void *function_96e90(long size)
{
	s_allocator_globals *globals = g_4d87f8;
	void *block = globals->allocator->allocate(size, 0, 0);
	if (block == 0)
	{
		globals->allocator->compact(0);
		block = globals->allocator->allocate(size, 0, 0);
	}
	if (block != 0)
		globals->count++;
	return block;
}

// @retail 0x970a0
bool c_vtable_450d1c::v0()
{
	return pending > 0;
}

// @retail 0x970b0
long c_vtable_450d1c::v1(long a1, long max_count, void *entries)
{
	s_entry_450d1c *entry_pointer = (s_entry_450d1c *)entries;
	long count = 0;
	s_node_450d1c *node = owner->head;
	s_node_450d1c *next;
	while (node != 0 && count < max_count)
	{
		next = node->next;
		dword mask = 1 << player;
		if ((node->active_mask & mask) != 0 && (node->done_mask & mask) == 0)
		{
			long start = node->time;
			if (node->timeout != NONE && time_now() - start >= node->timeout)
			{
				pending--;
				s_owner_450d1c *current_owner = owner;
				node->active_mask &= ~(1 << player);
				if (node->active_mask == 0)
				{
					current_owner->manager->v3(node);
					function_89e70(node, current_owner);
				}
			}
			else
			{
				real fraction;
				long size;
				owner->manager->v4(node, a1, &fraction, &size);
				if (fraction > 0.0f)
				{
					s_entry_450d1c *entry = entry_pointer;
					count++;
					entry_pointer++;
					size += 7;
					if (node->unknown10 != NONE)
						size += 14;
					size++;
					if (node->unknown14 != NONE)
						size += 14;
					entry->unknown00 = unknown04;
					entry->unknown04 = fraction;
					entry->node = node;
					entry->unknown0c = NONE;
					entry->size = size;
				}
				else
				{
					start = node->time;
					if (time_now() - start >= g_4cf474 && node->timeout == NONE)
						node->timeout = 0;
				}
			}
		}
		node = next;
	}
	return count;
}

// @retail 0x974c0
long c_vtable_450d1c::v5(dword a1, s_bitstream *stream, long max_blocks, s_block_450c94 *blocks, long *count_out)
{
	long count = 0;
	long error = 0;
	if (unknown09)
		error = 2;
	else
	{
		for (;;)
		{
			if (!stream_read_bit(stream))
				break;
			if (count >= max_blocks)
			{
				error = 3;
				break;
			}

			long type = function_1959c0(stream, 5);
			if (type < 0 || type >= 32)
			{
				error = 3;
				break;
			}

			long produced = 0;
			dword items[2];
			dword data[2];
			for (long i = 0; i < 2; i++)
			{
				if (stream_read_bit(stream))
				{
					dword low = function_1959c0(stream, 10);
					items[i] = low | ((byte)function_1959c0(stream, 4) << 28);
				}
				else
					items[i] = NONE;
			}

			error = owner->manager->v0(type, items, 1, &produced, data, stream);
			if (error != 0)
				break;

			s_block_450c94 *block = blocks;
			blocks++;
			count++;
			block->index = NONE;
			block->unknown00 = type;
			block->count = produced;
			if (produced > 0)
				memcpy(block->data, data, produced * 8);
			*(dword *)&block->unknown08[0] = items[0];
			*(dword *)&block->unknown08[4] = items[1];
		}
	}
	*count_out = count;
	return error;
}
// @retail 0x97690
void c_vtable_450d1c::v6(s_block_450c94 *block)
{
	owner->manager->v1(block->unknown00, (dword *)&block->unknown08[0], block->count, (dword *)block->data);
}

// @retail 0x976f0
void c_vtable_450d1c::function_976f0(long key, bool flag)
{
	s_request_450d1c **link = &requests;
	s_request_450d1c *request = *link;
	if (request == 0)
		return;
	while (request->key != key)
	{
		link = &request->next;
		request = *link;
		if (request == 0)
			return;
	}

	s_link_450d1c *current = request->links;
	while (current != 0)
	{
		s_node_450d1c *node = current->node;
		s_link_450d1c *next = current->next;
		node->done_mask &= ~(1 << player);
		unknown24--;
		if (!flag)
			pending++;
		else
		{
			unknown1c++;
			s_owner_450d1c *current_owner = owner;
			node->active_mask &= ~(1 << player);
			if (node->active_mask == 0)
			{
				current_owner->manager->v3(node);
				function_89e70(node, current_owner);
			}
		}

		long info;
		if (!g_4d87f8->allocator->get_info(current, &info))
			info = NONE;
		s_allocator_globals *globals = g_4d87f8;
		globals->allocator->release(current, NONE);
		globals->count--;
		current = next;
	}

	*link = request->next;
	request_count--;
	long info;
	if (!g_4d87f8->allocator->get_info(request, &info))
		info = NONE;
	s_allocator_globals *globals = g_4d87f8;
	globals->allocator->release(request, NONE);
	globals->count--;
}
// @retail 0x976c0
void c_vtable_450d1c::v7(void *a1)
{
	function_976f0((long)a1, true);
}

// @retail 0x976d0
void c_vtable_450d1c::v8(long a1, bool a2)
{
	function_976f0(a1, a2);
}

// @retail 0x97830
s_sub_450d14 *c_vtable_450d14::v0()
{
	s_world_450d14 *state = world;
	s_key_450d14 key = state->key;
	void *table = state->unknown0c;
	sub.owner = this;
	sub.count = 0;
	if (((1 << state->kind) & 0x14) != 0)
	{
		sub.flag = state->unknown78 == 0;
		sub.value = state->unknown7c;
		s_game_options_view *options = g_4e6948;
		if (options != 0 && options->flag1120 != 0)
		{
			s_header_450d14 *headers = (s_header_450d14 *)g_4e0300->data;
			for (long i = 0; i < 4; i++)
			{
				s_match_450d14 *match = function_6a3b0(table, &key, i);
				if (match != 0)
				{
					long player_index = match->unknown04 & 0xffff;
					s_player_450d14 *player = &((s_player_450d14 *)g_4e8c24->data)[player_index];
					long handle = player->unknown2c;
					if (handle == NONE)
					{
						handle = player->unknown30;
						if (handle == NONE)
							continue;
					}
					s_object_450d14 *object = headers[handle & 0xffff].object;
					s_entry_450d14 *entry = &sub.entries[sub.count];
					entry->unknown04 = object->unknownd4;
					entry->unknown08 = object->unknown64;
					entry->unknown14 = object->unknown15c;
					entry->unknown20 = object->unknown241;
					entry->unknown00 = player_index;
					sub.count++;
				}
			}
		}
	}
	else
	{
		sub.flag = 0;
		sub.value = NONE;
	}
	return &sub;
}

// @retail 0x979b0
void c_vtable_450d14::v1(long a1, long a2, real *a3, long *a4)
{
	*a3 = 0.0f;
	*a4 = 0;
	long handle = function_699a0(a1, world->unknown0c);
	if (handle != NONE)
		function_82a40(handle, a2, a3);
}
// @retail 0x979f0
bool function_979f0(s_world_450cb8 *world, long handle)
{
	s_table_450cb8 *table = &world->a->block->table;
	long index = handle & 0x3ff;
	bool result = false;
	if (index >= 0 && (dword)index < 0x400 && table->slots[index].handle == handle)
	{
		byte flags = table->flags->entries[index].flags;
		if (flags & 4)
			result = (flags >> 1) & 1;
	}
	return result;
}
// @retail 0x97a30
bool c_vtable_450cf4::v2(bool *a1)
{
	bool result = false;
	for (long i = 0; i < 3; i++)
	{
		c_interface_450c94 *child = children[i];
		if (child != 0 && child->v0())
		{
			result = true;
			*a1 = result;
		}
	}
	return result;
}

// @retail 0x97a70
long c_vtable_450cf4::v3(long a1, long a2)
{
	return unknown24;
}