/* UNKNOWN_16B570.H: the engine's data array (handle-addressed element pool)

   A data array is a 0x4c byte header, followed (when built in place) by the
   element storage and an occupancy bitmap. A datum index is a 16-bit salt in
   the high half and the absolute slot index in the low half; every element
   starts with its own 16-bit salt (0 = free). */

#ifndef UNKNOWN_16B570_H
#define UNKNOWN_16B570_H

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

#endif
