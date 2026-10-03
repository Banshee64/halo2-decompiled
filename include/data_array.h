/* DATA_ARRAY.H: the engine's data array (handle-addressed element pool;
   src/unknown_16b570.cpp). This is the one data-array type: g_4cf78c,
   g_4e8c24, g_4f55f0, g_502418, g_50241c, g_502420 and the object header data
   g_4e0300 are all views of it.

   A data array is a 0x4c byte header, followed (when built in place) by the
   element storage and an occupancy bitmap. A datum index is a 16-bit salt in
   the high half and the absolute slot index in the low half; every element
   starts with its own 16-bit salt (0 = free). */

#ifndef DATA_ARRAY_H
#define DATA_ARRAY_H

#include "cseries.h"

#define DATA_ARRAY_SIGNATURE 0x64407440

/* the memory allocator a data array was built through (vtable at [0]) */
class c_data_allocator
{
public:
	virtual void *allocate(long size) { return 0; }
	virtual void deallocate(void *block) { }
};

struct s_data_array
{
	char name[0x20];
	long maximum_count;
	long size;
	byte alignment_bits;
	byte valid;
	word flag0 : 1;
	word flag1 : 1;
	word allocated : 1;
	word initialize_to_bad : 1;
	word unknown2a : 12;
	dword signature;
	c_data_allocator *allocator;
	long first_free_index;
	long high_water_index;
	long actual_count;
	short next_salt;
	word unknown42;
	byte *data;
	dword *bitmap;
};

struct s_data_iterator
{
	s_data_array *data;
	long datum_index;
	long index;
};

/* a datum index is a salt in the high half and the absolute index in the low half */
s_data_array *data_new(const char *name, long maximum_count, long size, long alignment_bits, c_data_allocator *allocator);
void data_dispose(s_data_array *data);
void data_initialize(s_data_array *data, const char *name, long maximum_count, long size, long alignment_bits, c_data_allocator *allocator);
void data_array_construct(s_data_array *data, const char *name, long maximum_count, long size, long alignment_bits, c_data_allocator *allocator, dword *bitmap);
void data_connect(s_data_array *data, long maximum_count, byte *elements);
void data_make_valid(s_data_array *data);
void data_delete_all(s_data_array *data);
long datum_new(s_data_array *data);
long datum_new_at_index_with_salt(s_data_array *data, long datum_index);
long datum_new_at_index(s_data_array *data, long index);
void datum_initialize(s_data_array *data, byte *datum);
void datum_delete(s_data_array *data, long datum_index);
byte *datum_get(s_data_array *data, long datum_index);
byte *datum_get_absolute(s_data_array *data, long index);
long index_to_datum_index(s_data_array *data, long index);
byte *data_iterator_next(s_data_iterator *iterator);
long data_next_index(s_data_array *data, long datum_index);
long data_next_absolute_index(s_data_array *data, long index);

/* retail inlines data_new into its callers via LTCG; the out-of-line original
   in unknown_16b570.cpp needs /Ob1, which stops LTCG inlining it, so callers
   that retail inlines it into use this copy. (data_dispose is already
   inlinable: it lives in its own file, unknown_16b5d0.cpp.) */
static inline s_data_array *data_new_inlined(const char *name, long maximum_count, long size, long alignment_bits, c_data_allocator *allocator)
{
	long bitmap_size = ((maximum_count + 31) >> 5) * 4;
	s_data_array *data = (s_data_array *)allocator->allocate(sizeof(s_data_array) + maximum_count * size + bitmap_size + (1 << alignment_bits) - 1);

	if (data)
	{
		data_initialize(data, name, maximum_count, size, alignment_bits, allocator);
		data->allocated = 1;
	}
	return data;
}

/* likewise data_next_absolute_index and data_iterator_next
   (unknown_16b570.cpp), which retail inlines into callers built /Ob1 (ai.cpp,
   unknown_1cec30.cpp) */
static inline long data_next_absolute_index_inlined(s_data_array *data, long index)
{
	long result = NONE;

	if (index >= 0)
	{
		for (; index < data->high_water_index; index++)
		{
			if (data->bitmap[index >> 5] & (1 << (index & 0x1f)))
			{
				result = index;
				break;
			}
		}
	}
	return result;
}

static inline byte *data_iterator_next_inlined(s_data_iterator *iterator)
{
	s_data_array *data = iterator->data;
	long index = data_next_absolute_index_inlined(data, iterator->index + 1);
	byte *result;

	if (index != NONE)
	{
		result = data->data + data->size * index;
		iterator->index = index;
		iterator->datum_index = (*(short *)result << 16) | index;
	}
	else
	{
		iterator->index = data->maximum_count;
		iterator->datum_index = NONE;
		result = 0;
	}
	return result;
}
#endif