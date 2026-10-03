// @flags /O2 /Ob1 /arch:SSE /Gr
#include "cseries.h"
#include "real_math.h"
#include "globals.h"
#include "engine_peer.h"
#include <math.h>

struct s_e4050_object
{
	byte unknown00[0x14];
	long parent_index;
	byte unknown18[0xa9];
	byte flag_c1_0 : 1;
	byte unknownc1 : 7;
	byte unknownc2[0x286];
	byte flag_348_0 : 1;
	byte unknown348 : 7;
	byte unknown349[0x50];
	char value_399;
	byte unknown39a[0x42];
	byte state_3dc;
};

struct s_e4050_object_header
{
	byte unknown00[8];
	s_e4050_object *object;
};

// @retail 0xe4050
bool function_e4050(long object_index)
{
	s_e4050_object *object = ((s_e4050_object_header *)g_4e0300->data)[object_index & 0xffff].object;

	if (!TEST_FIELD_BIT(object->flag_c1_0) && TEST_FIELD_BIT(object->flag_348_0))
	{
		real scaled = g_510c54->ticks_per_second * 0.18f;
		long ticks;

		__asm
		{
			fld scaled
			fistp ticks
		}

		if (object->value_399 >= ticks)
		{
			if (object->state_3dc == 1 || object->state_3dc == 3)
			{
				if (object->parent_index == NONE)
					return true;
			}
		}
	}
	return false;
}

// @retail 0x12aff0
real function_12aff0(real a, real b, real c, bool flag)
{
	if (!(0.0001f > fabs(a - b)))
	{
		real t = (c - a) / (b - a);
		if (t < 0.0f)
			t = 0.0f;
		else if (t > 1.0f)
			t = 1.0f;
		return t;
	}

	return (a > c) == flag ? 0.0f : 1.0f;
}

/* the particle system objects at 0x479868 and 0x479874, picked by the group
   of a tag */
class c_particle_system
{
public:
	virtual void v0() {}
	virtual void v1() {}
	virtual void v2() {}
	virtual void v3() {}
	virtual void v4() {}
	virtual void v5() {}
	virtual void v6() {}
	virtual void v7() {}
	virtual void v8() {}
	virtual void v9() {}
	virtual void v10() {}
	virtual void v11() {}
	virtual void v12() {}
	virtual void v13() {}
	virtual void v14() {}
	virtual void v15() {}
	virtual void v16() {}
	virtual void v17() {}
	virtual void v18() {}
	virtual void v19() {}
	virtual void initialize(long tag_index) {}
};

c_particle_system g_479868;
c_particle_system g_479874;

struct s_137bd0_tag_instance
{
	dword group_tag;
	byte unknown04[12];
};

// @retail 0x137bd0
c_particle_system *function_137bd0(long tag_index)
{
	c_particle_system *result = 0;

	switch (((s_137bd0_tag_instance *)g_4e3b44)[(short)tag_index].group_tag)
	{
	case 'prt3':
		result = &g_479868;
		result->initialize(tag_index);
		break;
	case 'PRTM':
		result = &g_479874;
		result->initialize(tag_index);
		break;
	}
	return result;
}

// @retail 0x15e020
bool function_15e020(short a, short b)
{
	bool result = false;
	c_engine_peer *engine = g_55e4d0[g_4e9ae8->engine_index];

	if (engine)
		result = engine->p27(a, b);
	return result;
}
