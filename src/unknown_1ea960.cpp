#include "unknown_11c920.h"
#include "unknown_1efac0.h"
#include "unknown_1eb350.h"
#include "havok_reference.h"

// @flags /O2 /Gr

struct c_1ea960 : c_a
{
	virtual ~c_1ea960();
	virtual void *v3() { return 0; }
};

// @retail 0x1ea960
c_1ea960::~c_1ea960()
{
	__asm int 3
	__assume(0);
}

struct c_1ea970 : c_a
{
	virtual ~c_1ea970();
	virtual void *v3() { return 0; }
};

// @retail 0x1ea970
c_1ea970::~c_1ea970()
{
	__asm int 3
	__assume(0);
}

struct c_1ea980 : c_a
{
	virtual ~c_1ea980();
	virtual void *v3() { return 0; }
};

// @retail 0x1ea980
c_1ea980::~c_1ea980()
{
	__asm int 3
	__assume(0);
}

struct c_1ea990 : c_a
{
	virtual ~c_1ea990();
	virtual void *v3() { return 0; }
};

// @retail 0x1ea990
c_1ea990::~c_1ea990()
{
	__asm int 3
	__assume(0);
}

struct c_1ea9a0 : c_a
{
	virtual ~c_1ea9a0();
	virtual void *v3() { return 0; }
};

// @retail 0x1ea9a0
c_1ea9a0::~c_1ea9a0()
{
	__asm int 3
	__assume(0);
}

struct c_1ea9b0 : c_a
{
	virtual ~c_1ea9b0();
	virtual void *v3() { return 0; }
};

// @retail 0x1ea9b0
c_1ea9b0::~c_1ea9b0()
{
	__asm int 3
	__assume(0);
}

struct c_shape_adapter_a : c_shape_library_base_a {};
struct c_shape_adapter_b : c_shape_library_base_b {};

// @retail 0x1eb3b0 deleting c_shape_adapter_a
// @retail 0x1eb3e0 deleting c_shape_adapter_b

struct c_shape_reference : c_a
{
	long field_8;
	c_havok_reference_counted *object;
	virtual ~c_shape_reference();
	void operator delete(void *block)
	{
		g_480118->allocate((long)block, ((c_a *)block)->flags, 0x22);
	}
};

// @retail 0x1eb320 deleting c_shape_reference

// @retail 0x1eb350
c_shape_reference::~c_shape_reference()
{
	havok_reference_remove(object);
}

struct c_shape_pair : c_a
{
	long field_8;
	c_havok_reference_counted *first;
	c_havok_reference_counted *second;
	virtual ~c_shape_pair();
	void operator delete(void *block)
	{
		g_480118->allocate((long)block, ((c_a *)block)->flags, 0x22);
	}
};

// @retail 0x1ea930 deleting c_shape_pair

// @retail 0x1eb4c0
c_shape_pair::~c_shape_pair()
{
	havok_reference_remove(second);
	havok_reference_remove(first);
}

struct s_shape_array16
{
	void *data;
	long count;
	long capacity;
	__forceinline ~s_shape_array16()
	{
		if (!(capacity & 0x80000000))
			g_480118->allocate((long)data, (capacity & 0x7fffffff) * 0x10, 0x12);
	}
};

struct s_shape_array48
{
	void *data;
	long count;
	long capacity;
	__forceinline ~s_shape_array48()
	{
		if (!(capacity & 0x80000000))
			g_480118->allocate((long)data, (capacity & 0x7fffffff) * 0x30, 0x12);
	}
};

struct c_shape_arrays : c_a
{
	byte field_8[0x30 - 8];
	s_shape_array48 first;
	byte field_3c[0xd4 - 0x3c];
	s_shape_array16 second;
	void operator delete(void *block)
	{
		g_480118->allocate((long)block, ((c_a *)block)->flags, 0x22);
	}
};

// @retail 0x1eb410 deleting c_shape_arrays

// @retail 0x1eb440 destructor c_shape_arrays
