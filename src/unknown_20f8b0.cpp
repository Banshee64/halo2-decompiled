#include "unknown_11c920.h"
#include "globals.h"

// @flags /O2 /Gr

struct s_audio_queue_node
{
	byte unknown00[4];
	long object_index;
	byte unknown08[0x48 - 8];
	bool release;
	byte unknown49[7];
	long next;
};

struct s_audio_queue
{
	byte unknown000[0x7d0];
	long first;
	short count;
	byte unknown7d6[2];
	long unknown7d8;
};

struct s_audio_linked_object
{
	byte unknown000[0x342];
	short link_offset;
};

struct s_audio_linked_header
{
	byte unknown00[8];
	s_audio_linked_object *object;
};

struct s_audio_object_link
{
	byte unknown00[0x1c];
	long node_index;
};

struct s_node_owner;
void function_20fe20(s_node_owner *owner);
extern s_record_pool *g_4f9398;
s_audio_queue *g_4f939c;

PRIVATE __forceinline s_audio_queue_node *audio_queue_node(long index)
{
	return (s_audio_queue_node *)(g_4f9398->data + (index & 0xffff) * sizeof(s_audio_queue_node));
}

// @retail 0x20f8b0
bool function_20f8b0(s_audio_queue *queue, long node_index)
{
	bool result = false;
	long *link = &queue->first;
	while (*link != NONE)
	{
		long current = *link;
		s_audio_queue_node *node = audio_queue_node(current);
		if (current == node_index)
		{
			*link = node->next;
			queue->count--;
			result = true;
			break;
		}
		link = &node->next;
	}
	return result;
}

// @retail 0x20f910
void function_20f910(long node_index)
{
	s_record_pool *pool = g_4f9398;
	s_audio_queue_node *node = (s_audio_queue_node *)(pool->data + (node_index & 0xffff) * sizeof(s_audio_queue_node));
	long object_index = node->object_index;
	if (object_index != NONE)
	{
		s_audio_linked_object *object = ((s_audio_linked_header *)g_4e0300->data)[object_index & 0xffff].object;
		s_audio_object_link *link = (s_audio_object_link *)((byte *)object + object->link_offset);
		if (link->node_index == node_index)
			link->node_index = NONE;
	}
	record_pool_release(pool, node_index);
}

// @retail 0x210130
void function_210130(short queue_index)
{
	s_audio_queue *queue = (s_audio_queue *)((byte *)g_4f939c + queue_index * sizeof(s_audio_queue));
	for (long index = queue->first; index != NONE; )
	{
		s_audio_queue_node *node = audio_queue_node(index);
		node->release = true;
		index = node->next;
	}
	function_20fe20((s_node_owner *)queue);
}
