// @flags /O2 /Ob1 /Gr
#include "cseries.h"
#include "unknown_16b570.h"
#include <string.h>

long data_next_absolute_index(s_data_array *data, long index);
void datum_initialize(s_data_array *data, byte *datum);
void data_delete_all(s_data_array *data);
void data_initialize(s_data_array *data, const char *name, long maximum_count, long size, long alignment_bits, c_data_allocator *allocator);
void data_array_construct(s_data_array *data, const char *name, long maximum_count, long size, long alignment_bits, c_data_allocator *allocator, dword *bitmap);

// @retail 0x16b570
s_data_array *data_new(const char *name, long maximum_count, long size, long alignment_bits, c_data_allocator *allocator)
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

// @retail 0x16b5d0
void data_dispose(s_data_array *data)
{
	c_data_allocator *allocator = data->allocator;

	memset(data, 0, sizeof(s_data_array));
	if (allocator)
	{
		allocator->deallocate(data);
	}
}

// @retail 0x16b5f0
void data_initialize(s_data_array *data, const char *name, long maximum_count, long size, long alignment_bits, c_data_allocator *allocator)
{
	long alignment_mask = (1 << alignment_bits) - 1;
	byte *elements = (byte *)(((dword)data + sizeof(s_data_array) + alignment_mask) & ~alignment_mask);

	data_array_construct(data, name, maximum_count, size, alignment_bits, allocator, (dword *)(elements + maximum_count * size));
	data->flag0 = 0;
	data->flag1 = 0;
	data->data = elements;
	memset(data->bitmap, 0, ((maximum_count + 31) >> 5) * 4);
}

// @retail 0x16b650
void data_array_construct(s_data_array *data, const char *name, long maximum_count, long size, long alignment_bits, c_data_allocator *allocator, dword *bitmap)
{
	memset(data, 0, sizeof(s_data_array));
	strncpy(data->name, name, 0x20);
	data->flag0 = 1;
	data->flag1 = 1;
	data->name[0x1f] = 0;
	data->maximum_count = maximum_count;
	data->size = size;
	data->alignment_bits = (byte)alignment_bits;
	data->data = 0;
	data->valid = 0;
	data->signature = DATA_ARRAY_SIGNATURE;
	data->bitmap = bitmap;
	data->allocator = allocator;
}

// @retail 0x16b6b0
void data_connect(s_data_array *data, long maximum_count, byte *elements)
{
	long index = 0;

	data->flag1 = 0;
	data->maximum_count = maximum_count;
	data->data = elements;
	data->valid = 1;
	data->high_water_index = 0;
	data->actual_count = 0;
	data->first_free_index = maximum_count;
	strncpy((char *)&data->next_salt, data->name, 2);
	data->next_salt |= 0x8000;

	for (; index < data->maximum_count; index++)
	{
		byte *datum = data->data + data->size * index;
		bool used = *(short *)datum != 0;

		if (TEST_FIELD_BIT(data->initialize_to_bad) && !used)
		{
			memset(datum, 0xba, data->size);
			*(short *)datum = 0;
		}

		if (used)
		{
			data->bitmap[index >> 5] |= 1 << (index & 0x1f);
			data->high_water_index = index + 1;
			data->actual_count++;
		}
		else
		{
			data->bitmap[index >> 5] &= ~(1 << (index & 0x1f));
			if (index < data->first_free_index)
			{
				data->first_free_index = index;
			}
		}
	}
}

// @retail 0x16b790
void data_make_valid(s_data_array *data)
{
	data->valid = 1;
	data_delete_all(data);
}

// @retail 0x16b7a0
void data_delete_all(s_data_array *data)
{
	long index;

	data->high_water_index = 0;
	data->actual_count = 0;
	data->first_free_index = 0;
	strncpy((char *)&data->next_salt, data->name, 2);
	data->next_salt |= 0x8000;

	if (TEST_FIELD_BIT(data->initialize_to_bad))
	{
		memset(data->data, 0xba, data->size * data->maximum_count);
	}

	for (index = 0; index < data->maximum_count; index++)
	{
		*(short *)(data->data + data->size * index) = 0;
	}

	memset(data->bitmap, 0, ((data->maximum_count + 31) >> 5) * 4);
}

// @retail 0x16b840
long datum_new(s_data_array *data)
{
	long result = NONE;
	long index = data->first_free_index;
	long high_water = data->high_water_index;
	long new_index = NONE;

	if (index < high_water)
	{
		do
		{
			if (!(data->bitmap[index >> 5] & (1 << (index & 0x1f))))
			{
				new_index = index;
				break;
			}

			index++;
		}
		while (index < data->high_water_index);
	}

	if (new_index == NONE)
	{
		if (high_water < data->maximum_count)
		{
			new_index = high_water;
		}
	}

	if (new_index != NONE)
	{
		byte *datum = data->data + data->size * new_index;

		data->bitmap[new_index >> 5] |= 1 << (new_index & 0x1f);
		data->actual_count++;
		data->first_free_index = new_index + 1;
		if (data->high_water_index <= new_index)
		{
			data->high_water_index = new_index + 1;
		}

		memset(datum, 0, data->size);
		*(word *)datum = data->next_salt;
		data->next_salt++;
		if (data->next_salt == -1)
		{
			data->next_salt = 0x8000;
		}

		result = (*(short *)datum << 16) | new_index;
	}

	return result;
}

// @retail 0x16b910
long datum_new_at_index_with_salt(s_data_array *data, long datum_index)
{
	long index = datum_index & 0xffff;
	short salt = (short)(datum_index >> 16);
	byte *datum;

	if (index < 0 || index >= data->maximum_count)
	{
		return NONE;
	}

	datum = data->data + data->size * index;
	if (*(short *)datum == 0)
	{
		data->bitmap[index >> 5] |= 1 << (index & 0x1f);
		data->actual_count++;
		if (index >= data->high_water_index)
		{
			data->high_water_index = index + 1;
		}

		datum_initialize(data, datum);
		*(short *)datum = salt;
		return (salt << 16) | index;
	}

	return NONE;
}

// @retail 0x16b990
long datum_new_at_index(s_data_array *data, long index)
{
	byte *datum;

	if (index < 0 || index >= data->maximum_count)
	{
		return NONE;
	}

	datum = data->data + data->size * index;
	if (*(short *)datum == 0)
	{
		data->bitmap[index >> 5] |= 1 << (index & 0x1f);
		data->actual_count++;
		if (index >= data->high_water_index)
		{
			data->high_water_index = index + 1;
		}

		datum_initialize(data, datum);
		return (*(short *)datum << 16) | index;
	}

	return NONE;
}

// @retail 0x16ba00
void datum_initialize(s_data_array *data, byte *datum)
{
	memset(datum, 0, data->size);
	*(word *)datum = data->next_salt;
	data->next_salt++;
	if (data->next_salt == -1)
	{
		data->next_salt = 0x8000;
	}
}

// @retail 0x16ba40
void datum_delete(s_data_array *data, long datum_index)
{
	long index = datum_index & 0xffff;
	byte *datum = data->data + data->size * index;

	if (TEST_FIELD_BIT(data->initialize_to_bad))
	{
		memset(datum, 0xba, data->size);
	}

	data->bitmap[index >> 5] &= ~(1 << (index & 0x1f));
	*(short *)datum = 0;
	if (index < data->first_free_index)
	{
		data->first_free_index = index;
	}

	if (index + 1 == data->high_water_index)
	{
		do
		{
			datum -= data->size;
			data->high_water_index--;
		}
		while (data->high_water_index > 0 && *(short *)datum == 0);
	}

	data->actual_count--;
}

// @retail 0x16bae0
byte *datum_get(s_data_array *data, long datum_index)
{
	byte *result = 0;

	if (datum_index != NONE)
	{
		long index = datum_index & 0xffff;

		if (index < data->high_water_index)
		{
			byte *datum = data->data + data->size * index;
			short salt = *(short *)datum;

			if (salt != 0 && salt == (datum_index >> 16))
			{
				result = datum;
			}
		}
	}

	return result;
}

// @retail 0x16bb20
byte *datum_get_absolute(s_data_array *data, long index)
{
	byte *result = 0;

	if (index != NONE && index >= 0 && index < data->high_water_index)
	{
		byte *datum = data->data + data->size * index;

		if (*(short *)datum != 0)
		{
			result = datum;
		}
	}

	return result;
}

// @retail 0x16bb50
long index_to_datum_index(s_data_array *data, long index)
{
	long result = NONE;

	if (index != NONE)
	{
		result = (*(short *)(data->data + data->size * index) << 16) | index;
	}

	return result;
}

// @retail 0x16bb70
byte *data_iterator_next(s_data_iterator *iterator)
{
	s_data_array *data = iterator->data;
	long index = data_next_absolute_index(data, iterator->index + 1);
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

// @retail 0x16bbc0
long data_next_index(s_data_array *data, long datum_index)
{
	long start;
	long index;
	long result;

	if (datum_index == NONE)
	{
		start = 0;
	}
	else
	{
		start = (datum_index & 0xffff) + 1;
	}

	index = data_next_absolute_index(data, start);
	result = NONE;
	if (index != NONE)
	{
		result = (*(short *)(data->data + data->size * index) << 16) | index;
	}

	return result;
}

// @retail 0x16bc00
long data_next_absolute_index(s_data_array *data, long index)
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
