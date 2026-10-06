// @flags /O2 /Gr
#include "unknown_11c920.h"
#include "globals.h"
#include "havok_reference.h"
#include "unknown_1cec30.h"
#include "unknown_1efac0.h"

struct s_1d8dd0
{
	long field_0;
	word field_4;
};

// @retail 0x1d8dd0
void __cdecl function_1d8dd0(void *arg_0)
{
	g_480118->allocate((long)arg_0, ((s_1d8dd0 *)arg_0)->field_4, 0x26);
}

struct s_1d8df0
{
	byte field_0[0x54];
	c_havok_reference_counted *field_54;
	void function_1d8df0(c_havok_reference_counted *arg_0);
};

// @retail 0x1d8df0
void s_1d8df0::function_1d8df0(c_havok_reference_counted *arg_0)
{
	if (arg_0)
		++arg_0->reference_count;
	if (field_54)
		havok_reference_remove(field_54);
	field_54 = arg_0;
}

struct s_1d8e40 : hkRigidBody
{
	void function_1d8e40(hkVector4 const &arg_0);
	void function_1d8ec0(hkVector4 const &arg_0, hkVector4 const &arg_1);
};

// @retail 0x1d8e40
void s_1d8e40::function_1d8e40(hkVector4 const &arg_0)
{
	if (!isActive().m_bool && m_simulation_island)
		activate();
	m_motion->setLinearVelocity(arg_0);
}

// @retail 0x1d8ec0
void s_1d8e40::function_1d8ec0(hkVector4 const &arg_0, hkVector4 const &arg_1)
{
	if (!isActive().m_bool && m_simulation_island)
		activate();
	m_motion->applyPointImpulse(arg_0, arg_1);
}

class c_314710 : public c_a
{
public:
	byte field_8[0x58];
	c_314710(hkEntity *arg_0);
};

class c_1d19d0 : public c_314710
{
public:
	long field_60;
	long field_64;
	c_1d19d0(hkEntity *arg_0, long arg_1);
	static void operator delete(void *arg_0)
	{
		g_480118->allocate((long)arg_0, ((c_1d19d0 *)arg_0)->flags, 0x26);
	}
};

// @retail 0x1d19d0 deleting c_1d19d0

// @retail 0x1d1820
c_1d19d0::c_1d19d0(hkEntity *arg_0, long arg_1)
	: c_314710(arg_0), field_60(arg_1)
{
	(void)&arg_1;
	field_64 = havok_entity_property_get(arg_0, 0x2002);
}
