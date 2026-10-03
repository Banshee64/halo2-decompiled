/* UNKNOWN_1CEC30.H: the parts of Havok's rigid body (hkEntity) that the
   game's havok components (src/unknown_1cec30.cpp) read: the property list
   at +0x30, whose keys 0x2001..0x2003 the game uses for its own indices. */

#ifndef UNKNOWN_1CEC30_H
#define UNKNOWN_1CEC30_H

#include "cseries.h"
#include "globals.h"

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

/* the objects as the physics code sees them: the flag at +0xc0 marks an
   object counted in g_51e9a0 */
struct s_havok_object
{
	byte unknown000[0xc0];
	word havok_flag : 1;
	word unknownc0 : 15;
};

struct s_havok_object_header
{
	byte unknown00[8];
	s_havok_object *object;
};

extern long *g_51e9a0;

inline s_havok_object *havok_object_get(long object_index)
{
	return ((s_havok_object_header *)g_4e0300->data)[object_index & 0xffff].object;
}

/* counts the object in g_51e9a0, once (0x1c3980) */
void havok_object_count(long object_index);

#define HAVOK_PROPERTY_COMPONENT_INDEX 0x2001
#define HAVOK_PROPERTY_2002 0x2002
#define HAVOK_PROPERTY_2003 0x2003

#endif
