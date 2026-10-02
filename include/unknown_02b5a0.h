/* UNKNOWN_02B5A0.H: the data array header shared by the subsystem
   lifecycle callbacks of batch 24-1 (guessed layout). A data array is 0x4c
   bytes: the allocator that owns its block is at +0x30 and the valid flag is
   at +0x29. data_dispose clears the header, then hands it back to the
   allocator. */

#ifndef UNKNOWN_02B5A0_H
#define UNKNOWN_02B5A0_H

#include "cseries.h"
#include <string.h>

class c_data_allocator
{
public:
	virtual void slot0() = 0;
	virtual void deallocate(void *block) = 0;
};

struct s_data_header
{
	byte unknown00[0x29];
	bool valid;
	byte unknown2a[6];
	c_data_allocator *allocator;
	byte unknown34[0x18];
};

inline void data_dispose(s_data_header *data)
{
	c_data_allocator *allocator = data->allocator;

	memset(data, 0, sizeof(*data));
	if (allocator)
	{
		allocator->deallocate(data);
	}
}

#endif
