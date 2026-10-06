// @flags /O2 /Gr
#include "unknown_11c920.h"
#include "unknown_1efac0.h"
#include "havok_reference.h"

class c_1c5910 : public c_a
{
public:
	long field_8;
	c_havok_reference_counted *field_c;
	virtual ~c_1c5910();
	static void operator delete(void *arg_0)
	{
		g_480118->allocate((long)arg_0, ((c_1c5910 *)arg_0)->flags, 0x22);
	}
};

// @retail 0x1c5910
c_1c5910::~c_1c5910()
{
	havok_reference_remove(field_c);
}

// @retail 0x1c5970 deleting c_1c5910
