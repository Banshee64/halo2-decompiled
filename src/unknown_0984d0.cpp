#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_08b110.h"
#include "unknown_096ed0.h"

// @flags /O2 /Ob1 /Gr

/* src/unknown_096ed0.cpp, src/unknown_0b5650.cpp, src/unknown_195720.cpp */
void function_98ac0(c_handle_table_450cd0 *self, long handle);
void function_98b60(c_handle_table_450cd0 *self, long handle);
void function_98bf0(long handle, c_handle_table_450cd0 *self, dword mask);
void function_b5650(long identifier, s_bitstream *stream);
void function_194710(s_bitstream *stream, bool discard);

/* the handle-table messages each start with a 3-bit kind; the stream keeps
   the position before it, so that a message without room can be taken back */
static inline void stream_push_position(s_bitstream *stream)
{
	stream->checkpoints[stream->checkpoint_count] = stream->bit_position;
	stream->checkpoint_count++;
}

static inline bool stream_has_room(s_bitstream *stream, long bits)
{
	return (stream->size_in_bytes << 3) - stream->bit_position >= bits;
}

/* kind 1: an entity's creation */
// @retail 0x98620
bool function_98620(c_handle_table_450cd0 *self, s_bitstream *stream, long index, long a3, long reserved_bits)
{
	long handle = self->entries[index].handle;
	long released = 0;
	stream_push_position(stream);
	function_195720(stream, 1, 3);
	function_b5650(handle, stream);
	if (self->table->owner->v0(handle, self->entries[index].unknown04, a3, stream, reserved_bits, &released) &&
		stream_has_room(stream, reserved_bits))
	{
		stream->checkpoint_count--;
		function_98ac0(self, handle);
		if (released)
			function_98bf0(handle, self, released);
		return true;
	}
	function_194710(stream, true);
	return false;
}

/* kind 2: an entity's deletion */
// @retail 0x986d0
bool function_986d0(c_handle_table_450cd0 *self, long index, s_bitstream *stream, long reserved_bits)
{
	bool result = false;
	long handle = self->entries[index].handle;
	stream->checkpoints[stream->checkpoint_count] = stream->bit_position;
	stream->checkpoint_count++;
	function_195720(stream, 2, 3);
	function_b5650(handle, stream);
	if (stream_has_room(stream, reserved_bits))
	{
		stream->checkpoint_count--;
		function_98b60(self, handle);
		result = true;
	}
	else
		function_194710(stream, true);
	return result;
}

/* kind 4: the deletion of an entity and the entities linked to it */
// @retail 0x988f0
bool function_988f0(c_handle_table_450cd0 *self, long index, s_bitstream *stream, long reserved_bits)
{
	long handles[4];
	long count = 0;
	long handle = self->entries[index].handle;
	s_handle_peers *peers = self->table;
	while (handle != NONE)
	{
		handles[count] = handle;
		handle = peers->peers[handle & 0x3ff].unknown04;
		count++;
	}
	stream->checkpoints[stream->checkpoint_count] = stream->bit_position;
	stream->checkpoint_count++;
	function_195720(stream, 4, 3);
	stream_write_checked(stream, count - 2, 2);
	long i;
	for (i = 0; i < count; i++)
		function_b5650(handles[i], stream);
	if (stream_has_room(stream, reserved_bits))
	{
		stream->checkpoint_count--;
		for (i = 0; i < count; i++)
			function_98b60(self, handles[i]);
		return true;
	}
	function_194710(stream, true);
	return false;
}

/* kind 5: an entity's update */
// @retail 0x989f0
bool function_989f0(c_handle_table_450cd0 *self, s_bitstream *stream, long index, long a3, long reserved_bits)
{
	long handle = self->entries[index].handle;
	long released = 0;
	stream_push_position(stream);
	function_195720(stream, 5, 3);
	function_b5650(handle, stream);
	if (self->table->owner->v5(handle, self->entries[index].unknown04, a3, stream, reserved_bits, &released) &&
		stream_has_room(stream, reserved_bits))
	{
		stream->checkpoint_count--;
		function_98bf0(handle, self, released);
		return true;
	}
	function_194710(stream, true);
	return false;
}

// @retail 0x984d0
void c_handle_table_450cd0::v3(long a1, long a2, long a3, long a4, long a5, long a6)
{
	if (node == 0)
	{
		s_allocator_globals *globals = g_4d87f8;
		s_handle_node *block = (s_handle_node *)globals->allocator->allocate(0x10, 0, 0);
		if (block == 0)
		{
			globals->allocator->compact(0);
			block = (s_handle_node *)globals->allocator->allocate(0x10, 0, 0);
		}
		if (block != 0)
		{
			globals->count++;
			block->handle = NONE;
			block->items = 0;
			block->records = 0;
			block->next = 0;
		}
		node = block;
		if (node != 0)
		{
			node->handle = a4;
			node->items = 0;
			node->records = 0;
			node->next = 0;
			s_handle_node **link = &head;
			while (*link != 0)
				link = &(*link)->next;
			*link = node;
		}
		else
			unknown0a = 1;
	}

	if (!unknown0a)
	{
		switch (a2)
		{
		case 1:
			function_98620(this, (s_bitstream *)a5, a1, a3, a6);
			break;
		case 2:
			function_986d0(this, a1, (s_bitstream *)a5, a6);
			break;
		case 3:
			function_98750(this, a1, a5, a3, a6);
			break;
		case 4:
			function_988f0(this, a1, (s_bitstream *)a5, a6);
			break;
		case 5:
			function_989f0(this, (s_bitstream *)a5, a1, a3, a6);
			break;
		}
	}
}

// @retail 0x98aa0
void c_handle_table_450cd0::v4(long a1, s_bitstream *stream)
{
	function_195720(stream, 0, 3);
	node = 0;
}

// @retail 0x98fb0
void c_handle_table_450cd0::v6(s_request_450cd0 *a1)
{
	switch (a1->kind)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		break;
	}
}