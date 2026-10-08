// @flags /O2 /Gr /arch:SSE
#include "unknown_1cec30.h"

struct s_2789f0
{
	byte field_0[0x18];
	long field_18;
	long field_1c;
	hkEntity *field_20;
};

struct s_2789f1
{
	long field_0;
	s_2789f0 *field_4;
};

class c_2789f0
{
public:
	virtual void function_2789f0(s_2789f1 *arg_0);
};

bool function_a7670(long arg_0);
void __stdcall function_b8540(long arg_0);

// @retail 0x2789f0
void c_2789f0::function_2789f0(s_2789f1 *arg_0)
{
	if (arg_0->field_4->field_18 == 1)
	{
		hkEntity *local_0 = arg_0->field_4->field_20;
		if (local_0)
		{
			long local_1 = havok_entity_property_get(local_0, 0x2001);
			if (local_1 != NONE)
			{
				s_havok_component *local_2 = havok_component_get(local_1);
				local_2->flags12 |= 1;
				long local_3 = local_2->object_index;
				if (!function_a7670(local_3))
					function_b8540(local_3);
			}
		}
	}
}
