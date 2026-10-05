#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include "globals.h"
#include "object_iterator.h"
#include "unknown_16d180.h"

// @flags /O2 /Gr

struct s_object_blocks_ab
{
	long tag_index;
	byte unknown04[0xb1 - 4];
	char variant;
	byte unknownb2[0x118 - 0xb2];
	short regions_size;
	short regions_offset;
	byte unknown11c[0x12a - 0x11c];
	short animation_offset;
};

struct s_object_blocks_header_ab
{
	byte unknown00[8];
	s_object_blocks_ab *object;
};

struct s_object_variant_definition_ab
{
	byte unknown00[0x30];
	string_handle variant;
	byte unknown34[4];
	long model_index;
};

void function_b7360(long object_index);

// @retail 0xba3d0
void function_ba3d0(long object_index)
{
	s_object_blocks_ab *object = ((s_object_blocks_header_ab *)g_4e0300->data)[object_index & 0xffff].object;
	byte *state = (byte *)object + object->animation_offset + 0x64;
	state[1] = 0;
	state[0] = 0;
	state[3] = 0;
	function_b7360(object_index);
}

static __forceinline s_object_blocks_ab *object_blocks_get_ab(long object_index)
{
    return ((s_object_blocks_header_ab *)g_4e0300->data)[object_index & 0xffff].object;
}

// @retail 0xba540
void function_ba540(long object_index, string_handle name)
{
	s_object_blocks_ab *object = object_blocks_get_ab(object_index);
	s_object_variant_definition_ab *definition = (s_object_variant_definition_ab *)g_4e3b44[object->tag_index & 0xffff].bytes;
	string_handle resolved = name;
	if (!resolved)
		resolved = definition->variant;
	object->variant = (char)function_16d180(definition->model_index, resolved);
}

// @retail 0xba690
void function_ba690(long object_index, byte **states, long *state_count, long *a, long *b)
{
	s_object_blocks_ab *object = ((s_object_blocks_header_ab *)g_4e0300->data)[object_index & 0xffff].object;
	*state_count = object->regions_size;
	byte *regions = (byte *)object + object->regions_offset;
	*state_count /= 10;
	if (states)
		*states = regions;
	if (a)
		*a = (long)(regions + *state_count);
	if (b)
		*b = (long)(regions + 2 * *state_count);
}

struct s_object
{
	byte unknown00[4];
	dword : 26;
	dword flag26 : 1;
	dword : 5;
	byte unknown08[0xc];
	long parent_index;
	byte unknown18[0x10c];
	short markers_size;
	short markers_offset;
};

struct s_object_header
{
	short identifier;
	byte flags;
	byte type;
	byte unknown04[4];
	s_object *object;
};

#define OBJECT_HEADER(index) ((s_object_header *)(g_4e0300->data + g_4e0300->size * (index)))

// @retail 0xbad50
bool function_bad50(long object_index, long index, point3f *out)
{
	s_object *object = ((s_object_header *)g_4e0300->data)[object_index & 0xffff].object;
	byte *base = object->markers_offset + (byte *)object;
	bool result = false;
	long count = (long)((dword)(long)object->markers_size / 12) / 2;

	if (index >= 0 && index < count)
	{
		*out = *(point3f *)(base + (count + index) * 12);
		result = true;
	}
	return result;
}

// @retail 0xbadc0
s_object *function_badc0(long object_index, dword type_mask)
{
	s_object_header *header = 0;

	if (object_index != NONE)
	{
		long index = object_index & 0xffff;
		if (index < g_4e0300->high_water_index)
		{
			s_object_header *h = OBJECT_HEADER(index);
			if (h->identifier && h->identifier == (object_index >> 16))
				header = h;
		}
	}

	s_object *result = 0;
	if (header && (type_mask & (1 << header->type)))
		result = header->object;
	return result;
}

// @retail 0xbae20
s_object *function_bae20(long object_index, dword type_mask)
{
	s_object_header *header = 0;

	if (object_index != NONE)
	{
		long index = object_index & 0xffff;
		if (index >= 0 && index < g_4e0300->high_water_index)
		{
			s_object_header *h = OBJECT_HEADER(index);
			if (h->identifier && h->identifier == (object_index >> 16))
				header = h;
		}
	}

	s_object *result = 0;
	if (header && (type_mask & (1 << header->type)))
		result = header->object;
	return result;
}

// @retail 0xbae80
void function_bae80(s_type_f1af8e *iterator, dword type_mask, byte flags)
{
	iterator->signature = 0x86868686;
	if (!type_mask)
		type_mask = NONE;
	iterator->type_mask = type_mask;
	iterator->flags = flags;
	iterator->index = 0;
	iterator->object_index = NONE;
}

// @retail 0xbaeb0
s_object *function_baeb0(s_type_f1af8e *iterator)
{
	short index = iterator->index;
	s_object_header *header = (s_object_header *)(g_4e0300->data + index * 12);
	s_object *result = 0;

	while (index < g_4e0300->high_water_index)
	{
		short identifier = header->identifier;
		long object_index = (identifier << 16) | index;
		index++;
		if (identifier && (header->flags & iterator->flags) == iterator->flags && (iterator->type_mask & (1 << header->type)))
		{
			iterator->object_index = object_index;
			result = header->object;
			break;
		}
		header++;
	}

	iterator->index = index;
	return result;
}

// @retail 0xbaf40
long function_baf40(long object_index)
{
	if (object_index != NONE)
	{
		do
		{
			s_object *object = ((s_object_header *)g_4e0300->data)[object_index & 0xffff].object;
			if (!TEST_FIELD_BIT(object->flag26))
				break;
			object_index = object->parent_index;
		}
		while (object_index != NONE);
	}
	return object_index;
}

// @retail 0xbaf80
long function_baf80(long object_index)
{
	long result = NONE;

	while (object_index != NONE)
	{
		result = object_index;
		object_index = ((s_object_header *)g_4e0300->data)[object_index & 0xffff].object->parent_index;
	}
	return result;
}

// @retail 0xbafb0
bool function_bafb0(long object_index, long ancestor_index)
{
	long current = NONE;

	while (object_index != NONE && current != ancestor_index)
	{
		current = object_index;
		object_index = ((s_object_header *)g_4e0300->data)[object_index & 0xffff].object->parent_index;
	}
	return ancestor_index != NONE && current == ancestor_index;
}
