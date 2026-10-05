// @flags /O2 /Ob1 /Gr
/* UNKNOWN_0BBF40.CPP: object flag setters of the script functions */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_0bbf40.h"
#include "object_iterator.h"
#include "loop_allocator.h"
#include <string.h>

#define FLAG(bit) (1 << (bit))
#define SET_FLAG(flags, bit, value) ((value) ? ((flags) |= FLAG(bit)) : ((flags) &= ~FLAG(bit)))

/* the objects (a local view) */
struct s_object_0bbf40
{
	byte unknown00[4];
	dword flags;
};

struct s_object_header_0bbf40
{
	byte unknown00[8];
	s_object_0bbf40 *object;
};

inline s_object_0bbf40 *object_get_0bbf40(long object_index)
{
	return ((s_object_header_0bbf40 *)g_4e0300->data)[object_index & 0xffff].object;
}

// @retail 0xbbf40
void function_bbf40(long object_index, bool flag)
{
	if (object_index != NONE)
	{
		dword *flags = &object_get_0bbf40(object_index)->flags;
		SET_FLAG(*flags, 19, flag);
	}
}

// @retail 0xbbf80
void function_bbf80(long object_index, bool flag)
{
	if (object_index != NONE)
	{
		dword *flags = &object_get_0bbf40(object_index)->flags;
		SET_FLAG(*flags, 24, flag);
	}
}

// @retail 0xbc150
void function_bc150(long object_index, bool flag)
{
	if (object_index != NONE)
	{
		dword *flags = &object_get_0bbf40(object_index)->flags;
		SET_FLAG(*flags, 21, flag);
	}
}
void function_b8b70(long object_index);

// @retail 0xbc100
void function_bc100(long object_index, bool flag)
{
	if (object_index != NONE)
	{
		dword *flags = &object_get_0bbf40(object_index)->flags;
		SET_FLAG(*flags, 20, flag);
		function_b8b70(object_index);
	}
}

struct s_predicted_resource_block;
bool function_16e5e0(s_predicted_resource_block const *block, short mode);

/* the objects' definitions (a local view) */
struct s_object_definition_0bbf40
{
	byte unknown00[0xb4];
	byte field_84[0xc];
};

/* the objects (another local view) */
struct s_object_tree_0bbf40
{
	long definition_index;
	byte unknown04[0xc - 4];
	long next_object_index;
	long first_child_index;
};

struct s_object_tree_header_0bbf40
{
	byte unknown00[8];
	s_object_tree_0bbf40 *object;
};

/* requests (or releases) the predicted resources of an object definition */
// @retail 0xbbe00
bool function_bbe00(long definition_index, bool load)
{
	bool result = true;
	if (definition_index != NONE)
	{
		if (load)
		{
			s_predicted_resource_block const *block = (s_predicted_resource_block const *)((s_object_definition_0bbf40 *)g_4e3b44[definition_index & 0xffff].bytes)->field_84;
			result = function_16e5e0(block, 1);
		}
		else
		{
			s_predicted_resource_block const *block = (s_predicted_resource_block const *)((s_object_definition_0bbf40 *)g_4e3b44[definition_index & 0xffff].bytes)->field_84;
			result = function_16e5e0(block, 2);
		}
	}
	return result;
}

struct s_object_list;
extern s_object_list *g_4de2f4;
struct s_data_header_40;
extern s_data_header_40 *g_4de2ec;

struct s_object_gc_globals_ab
{
	long unknown00;
	short count;
	short unknown06;
	long first_index;
};

struct s_object_gc_buffer_ab
{
	dword count;
	long indices[0x800];
};

struct s_object_gc_view_ab
{
	byte unknown00[0x14];
	long parent_index;
	long unknown18;
	long next_index;
	byte unknown20[0xb4];
	long simulation_index;
};

// @retail 0xbf1c0
bool function_bf1c0(long level, bool *critical_out)
{
	bool critical = false;
	bool result;
	if (level > 1)
	{
		if (level != 2)
		{
			s_loop_allocator *loop = (s_loop_allocator *)g_4de2ec;
			long used;
			if (!loop->last)
				used = 0;
			else
				used = (byte *)loop->last + loop->last->size - loop->base;
			long remaining = loop->size - used;
			long free_headers = g_4e0300->maximum_count - g_4e0300->high_water_index;
			result = remaining < 0x19999 || free_headers < 0xcc;
			critical = remaining < 0x6666 || free_headers < 0x33;
		}
		else
			result = ((s_object_gc_globals_ab *)g_4de2f4)->count > 30;
	}
	else
		result = true;
	*critical_out = critical;
	return result;
}

// @retail 0xbf240
void __stdcall function_bf240(long level, s_object_gc_buffer_ab *buffer, long size)
{
	buffer->count = 0;
	long index = ((s_object_gc_globals_ab *)g_4de2f4)->first_index;
	while (index != NONE && buffer->count < 0x800)
	{
		s_object_gc_view_ab *object = (s_object_gc_view_ab *)((s_object_header_0bbf40 *)g_4e0300->data)[index & 0xffff].object;
		bool simulation_attached = false;
		if (g_4e6948->mode == 4)
			simulation_attached = object->simulation_index != NONE;
		if (!simulation_attached && object->parent_index == NONE)
			buffer->indices[buffer->count++] = index;
		index = object->next_index;
	}
}

void loop_compact(s_loop_allocator *loop);

// @retail 0xbf360
long __stdcall function_bf360(long level, void *buffer, long size, bool *again, void *unused, long maximum)
{
	loop_compact((s_loop_allocator *)g_4de2ec);
	return 0;
}

/* Slots of the retail garbage-collection table at 0x440570. */
long (__stdcall *g_44057c)(long, void *, long, bool *, void *, long) = function_bf360;
void (__stdcall *g_440588)(long, s_object_gc_buffer_ab *, long) = function_bf240;
long (__stdcall *g_44059c)(long, void *, long, bool *, void *, long) = function_bf360;

struct s_model_reference_ab
{
	byte unknown00[0x38];
	long model_index;
};

struct s_model_render_ab
{
	long unknown00;
	long render_model;
	byte unknown08[0xc];
	long field_14_4;
};

transform4x3f *function_b8c00(long object_index, long *node_count);
void function_109140(long object_index, long count, long nodes);

// @retail 0xbe1d0
void function_be1d0(long object_index)
{
	s_object_tree_0bbf40 *object = ((s_object_tree_header_0bbf40 *)g_4e0300->data)[object_index & 0xffff].object;
	s_model_reference_ab *definition = (s_model_reference_ab *)g_4e3b44[object->definition_index & 0xffff].bytes;
	if (definition->model_index != NONE)
	{
		s_model_render_ab *model = (s_model_render_ab *)g_4e3b44[definition->model_index & 0xffff].bytes;
		if (model->render_model != NONE && model->field_14_4 != NONE)
		{
			long count;
			transform4x3f *nodes = function_b8c00(object_index, &count);
			function_109140(object_index, count, (long)nodes);
		}
	}
}

/* the same for an object and every object attached to it.
   The standard convention (ret 8):
   1. The body matches retail only with the marker; without it LTCG passes
      the object index in ecx and the flag in dl.
   2. Retail holds no reference to 0xbbec0's address; its callers, 0x2a03d0
      and itself (the recursion over attached objects), push both arguments.
   3. Tried: the plain definition (the register convention above). */
// @retail 0xbbec0 standard
bool __stdcall function_bbec0(long object_index, bool load)
{
	bool result = true;
	if (object_index != NONE)
	{
		s_object_tree_0bbf40 *object = ((s_object_tree_header_0bbf40 *)g_4e0300->data)[object_index & 0xffff].object;
		long child_index;

		result = function_bbe00(object->definition_index, load) & 1;
		for (child_index = object->first_child_index; child_index != NONE; child_index = object->next_object_index)
		{
			object = ((s_object_tree_header_0bbf40 *)g_4e0300->data)[child_index & 0xffff].object;
			result &= function_bbec0(child_index, load);
		}
	}
	return result;
}

// @retail 0xbbe60
bool function_bbe60(long definition_index)
{
	bool result = true;
	if (definition_index != NONE)
	{
		s_predicted_resource_block const *block = (s_predicted_resource_block const *)((s_object_definition_0bbf40 *)g_4e3b44[definition_index & 0xffff].bytes)->field_84;
		result = function_16e5e0(block, 2);
	}
	return result;
}

// @retail 0xbbe90
bool function_bbe90(long definition_index)
{
	bool result = true;
	if (definition_index != NONE)
	{
		s_predicted_resource_block const *block = (s_predicted_resource_block const *)((s_object_definition_0bbf40 *)g_4e3b44[definition_index & 0xffff].bytes)->field_84;
		result = function_16e5e0(block, 1);
	}
	return result;
}

struct s_object_query_bbf40
{
	byte unknown00[0xa4];
	long unique_id;
	short origin_bsp;
	char type;
	char source;
	short name_index;
	byte unknownae[0xc2 - 0xae];
	short team;
	long player_index;
	long owner_index;
	byte unknowncc[0x112 - 0xcc];
	short orientations_offset;
	byte unknown114[0x12a - 0x114];
	short animation_offset;
};

struct s_object_query_header_bbf40
{
	byte unknown00[8];
	s_object_query_bbf40 *object;
};

struct s_damage_owner
{
	long player_index;
	long object_index;
	short team;
};

// @retail 0xbc190
void function_bc190(long object_index, s_damage_owner *owner)
{
	s_object_query_bbf40 *object = ((s_object_query_header_bbf40 *)g_4e0300->data)[object_index & 0xffff].object;
	owner->object_index = object->owner_index;
	owner->player_index = object->player_index;
	owner->team = object->team;
}

// @retail 0xbf5a0
bool function_bf5a0(long object_index)
{
	s_object_query_bbf40 *object = ((s_object_query_header_bbf40 *)g_4e0300->data)[object_index & 0xffff].object;
	long result = object->animation_offset != NONE;
	return result != 0;
}

// @retail 0xbf5d0
bool function_bf5d0(long object_index)
{
	s_object_query_bbf40 *object = ((s_object_query_header_bbf40 *)g_4e0300->data)[object_index & 0xffff].object;
	long result = object->orientations_offset != NONE;
	return result != 0;
}

extern long g_4de2fc;
extern long g_4de300[0x800];

// @retail 0xbec70
bool function_bec70(long object_index)
{
	long stamp = g_4de2fc;
	object_index &= 0xffff;
	bool result = false;
	if (g_4de300[object_index] != stamp)
	{
		g_4de300[object_index] = stamp;
		result = true;
	}
	return result;
}

extern long *g_4de2d0;

// @retail 0xbf050
void function_bf050(long object_index, short name_index)
{
	s_object_query_bbf40 *object = ((s_object_query_header_bbf40 *)g_4e0300->data)[object_index & 0xffff].object;
	if (g_4de2d0[name_index] == NONE)
	{
		g_4de2d0[name_index] = object_index;
		object->name_index = name_index;
	}
}

// @retail 0xbe650
void function_be650(long *list, long object_index)
{
	while (*list != NONE)
	{
		s_object_tree_0bbf40 *object = ((s_object_tree_header_0bbf40 *)g_4e0300->data)[*list & 0xffff].object;
		if (*list == object_index)
		{
			*list = object->next_object_index;
			object->next_object_index = NONE;
			break;
		}
		list = &object->next_object_index;
	}
}

struct s_name_scenario_bbf40
{
	byte unknown00[0x48];
	long count;
};

// @retail 0xbf090
void function_bf090(long object_index)
{
	s_object_query_bbf40 *object = ((s_object_query_header_bbf40 *)g_4e0300->data)[object_index & 0xffff].object;
	if (object->name_index != NONE)
	{
		s_name_scenario_bbf40 *scenario = (s_name_scenario_bbf40 *)g_4e0350;
		object->name_index = NONE;
		for (short i = 0; i < scenario->count; i++)
		{
			if (g_4de2d0[i] == object_index)
				g_4de2d0[i] = NONE;
		}
	}
}

struct s_wake_header_bbf40
{
	byte unknown00[3];
	byte type;
	byte unknown04[4];
	s_object_tree_0bbf40 *object;
};

void function_b7360(long object_index);

// @retail 0xbc7b0
void __stdcall function_bc7b0(long object_index)
{
	s_wake_header_bbf40 *header = (s_wake_header_bbf40 *)g_4e0300->data + (object_index & 0xffff);
	s_object_tree_0bbf40 *object = header->object;
	if ((1 << header->type) & 0x1003)
		function_b7360(object_index);
	long child_index = object->first_child_index;
	while (child_index != NONE)
	{
		header = (s_wake_header_bbf40 *)g_4e0300->data + (child_index & 0xffff);
		function_bc7b0(child_index);
		child_index = header->object->next_object_index;
	}
}

struct s_object_identifier_bbf40
{
	long unique_id;
	short origin_bsp;
	char type;
	char source;
};

// @retail 0xbf760
long function_bf760(long const *unique_id)
{
	s_object_identifier_bbf40 const *identifier = (s_object_identifier_bbf40 const *)unique_id;
	long result = NONE;
	char source = identifier->source;
	if (source != NONE)
	{
		char type = identifier->type;
		struct
		{
			s_object_query_bbf40 *object;
			s_type_f1af8e iterator;
		} state;
		function_bae80(&state.iterator, 1 << type, 0);
		while ((state.object = (s_object_query_bbf40 *)function_baeb0(&state.iterator)) != 0)
		{
			bool match = (type == state.object->type) & (source == state.object->source) &
				(identifier->unique_id == state.object->unique_id);
			if (match && source == 0)
				match &= identifier->origin_bsp == state.object->origin_bsp;
			if (match)
				result = state.iterator.object_index;
		}
	}
	return result;
}

bool loop_allocate(s_loop_allocator *loop, void **pointer, long size, char const *file, long line);
bool loop_reallocate(s_loop_allocator *loop, void **pointer, long size, char const *file, long line);

struct s_object_memory_header_ab
{
	short salt;
	byte flags;
	byte type;
	short cluster;
	short size;
	void *object;
};

struct s_object_block_ab
{
	short size;
	short offset;
};

// @retail 0xbc280
long __stdcall function_bc280(short size)
{
	long index = record_pool_allocate(g_4e0300);
	if (index != NONE)
	{
		s_object_memory_header_ab *header = (s_object_memory_header_ab *)g_4e0300->data + (index & 0xffff);
		if (loop_allocate((s_loop_allocator *)g_4de2ec, &header->object, size, 0, 0))
		{
			header->size = size;
			memset(header->object, 0, size);
		}
		else
		{
			record_pool_release(g_4e0300, index);
			index = NONE;
		}
	}
	return index;
}

// @retail 0xbc300
void __stdcall function_bc300(long object_index)
{
	s_object_memory_header_ab *header = (s_object_memory_header_ab *)g_4e0300->data + (object_index & 0xffff);
	header->flags = 0;
	if (header->object)
	{
		s_loop_allocator *loop = (s_loop_allocator *)g_4de2ec;
		s_loop_block *block = (s_loop_block *)header->object - 1;
		loop->free += block->size;
		if (block->previous)
			block->previous->next = block->next;
		else
			loop->first = block->next;
		if (block->next)
			block->next->previous = block->previous;
		else
			loop->last = block->previous;
		header->object = 0;
	}
	record_pool_release(g_4e0300, object_index);
}

// @retail 0xbc380
bool function_bc380(long object_index, long block_offset, long size, long alignment_bits)
{
	volatile bool result = false;
	s_object_memory_header_ab *header = (s_object_memory_header_ab *)g_4e0300->data + (object_index & 0xffff);
	if ((short)size == 0)
	{
		s_object_block_ab *block = (s_object_block_ab *)((byte *)header->object + (short)block_offset);
		block->offset = NONE;
		block->size = 0;
		return true;
	}
	long mask = (1 << (byte)alignment_bits) - 1;
	long extra = (short)size + mask;
	if (loop_reallocate((s_loop_allocator *)g_4de2ec, &header->object, header->size + extra, 0, 0))
	{
		short old_size = header->size;
		header->size = (short)(old_size + extra);
		s_object_memory_header_ab *current = (s_object_memory_header_ab *)g_4e0300->data + (object_index & 0xffff);
		s_object_block_ab *block = (s_object_block_ab *)((byte *)current->object + (short)block_offset);
		block->offset = (short)((old_size + mask) & ~mask);
		block->size = (short)size;
		memset((byte *)header->object + old_size, 0, extra);
		return true;
	}
	return result;
}
