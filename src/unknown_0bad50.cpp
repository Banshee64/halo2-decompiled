#include "cseries.h"
#include "real_math.h"
#include "globals.h"
#include "object_iterator.h"

// @flags /O2 /Gr

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
