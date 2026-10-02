/* UNKNOWN_1EFAC0.H: a small class hierarchy (c_b <- c_a <- c_d); c_a's
   enable/disable virtual lives in code not decompiled here (stubbed) */

#ifndef UNKNOWN_1EFAC0_H
#define UNKNOWN_1EFAC0_H

#include "cseries.h"

/* the allocator at 0x480118; slot 5 frees a block */
struct c_allocator
{
	virtual void v0() {}
	virtual void v1() {}
	virtual void v2() {}
	virtual void v3() {}
	virtual void v4() {}
	virtual void free(void *block, long size, long tag) {}
};

extern c_allocator *g_480118;

struct c_b
{
	virtual ~c_b() {}

	void operator delete(void *block)
	{
		g_480118->free(block, 0xc, 0x22);
	}
};

struct c_a : c_b
{
	word flags;

	virtual ~c_a()
	{
		if (flags & 0x8000)
		{
			flags &= 0x7fff;
			v2(false);
		}
	}

	virtual long v1(long, long) { return 0; }
	virtual void v2(bool enable);
	virtual void *v3() { return 0; }
	virtual void v4() {}
};

#endif
