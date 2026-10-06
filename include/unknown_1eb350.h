#ifndef UNKNOWN_1EB350_H
#define UNKNOWN_1EB350_H

#include "unknown_11c920.h"
#include "globals.h"

class c_havok_reference_counted;

struct c_shape_library_base_a
{
	c_shape_library_base_a() {}
	c_shape_library_base_a(c_havok_reference_counted *arg_0, long arg_1);
	virtual ~c_shape_library_base_a();
	word allocation_size;
	word references;

	void operator delete(void *block)
	{
		g_480118->allocate((long)block, ((c_shape_library_base_a *)block)->allocation_size, 0x22);
	}
};

struct c_shape_library_base_b
{
	virtual ~c_shape_library_base_b();
	word allocation_size;
	word references;

	void operator delete(void *block)
	{
		g_480118->allocate((long)block, ((c_shape_library_base_b *)block)->allocation_size, 0x22);
	}
};

#endif
