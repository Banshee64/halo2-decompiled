/* UNKNOWN_1CEC30.H: the parts of Havok's rigid body (hkEntity) that the
   game's havok components (src/unknown_1cec30.cpp) read: the property list
   at +0x30, whose keys 0x2001..0x2003 the game uses for its own indices. */

#ifndef UNKNOWN_1CEC30_H
#define UNKNOWN_1CEC30_H

#include "cseries.h"

class hkPropertyValue
{
public:
	hkPropertyValue() {}
	hkPropertyValue(long value) : m_data(value) {}

	long m_data;
};

struct hkProperty
{
	dword m_key;
	hkPropertyValue m_value;
};

/* an hkArray: data, size, capacity (the top bit set when the array does
   not own its storage) */
struct s_havok_array
{
	void *data;
	long size;
	dword capacity_and_flags;
};

class hkEntity
{
public:
	hkPropertyValue removeProperty(dword key);
	void addProperty(dword key, hkPropertyValue value);

	byte unknown00[0x30];
	hkProperty *m_properties;
	long m_property_count;
	dword m_property_capacity;
};

#define HAVOK_PROPERTY_COMPONENT_INDEX 0x2001
#define HAVOK_PROPERTY_2002 0x2002
#define HAVOK_PROPERTY_2003 0x2003

#endif
