#include "unknown_11c920.h"
#include "unknown_1c3b20.h"
#include <new>
// @flags /O2 /Gr

class c_1d7390
{
public:
	virtual void slot0() = 0;
	virtual void slot1() = 0;
	virtual void slot2() = 0;
	virtual void slot3() = 0;
	virtual void *function_1d7390(long arg_0, long arg_1) = 0;
};

extern c_shape_owner *g_51e9d4;

inline c_shape_owner::c_shape_owner(c_havok_reference_counted *arg_0, long arg_1)
 : c_shape_library_base_a(arg_0, arg_1), object(arg_0)
{
 ++arg_0->reference_count;
}

// @retail 0x1c3b20
c_shape_owner *function_1c3b20(c_havok_reference_counted *arg_0)
{
 void *local_0 = ((c_1d7390 *)g_480118)->function_1d7390(0x18, 0x22);
 ((c_shape_library_base_a *)local_0)->allocation_size = 0x18;
 c_shape_owner *local_1 = new (local_0) c_shape_owner(arg_0, *(long *)((byte *)g_4e0348 + 0x1e4));
 g_51e9d4 = local_1;
 return local_1;
}
