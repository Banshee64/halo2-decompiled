// @flags /O2 /Gr
/* UNKNOWN_082B70.CPP: object queries of the simulation's entity code
   (lane D) */

#include "unknown_11c920.h"
#include "globals.h"

/* the object fields read here */
struct s_082b70_object
{
	byte unknown000[0x14];
	long parent;
	byte unknown018[0xaa - 0x18];
	char type;
	byte unknown0ab[0x212 - 0xab];
	char weapon_slots[2];
	byte unknown214[0x24c - 0x214];
	long driver;
};

struct s_082b70_object_header
{
	byte unknown00[8];
	s_082b70_object *object;
};

static inline s_082b70_object *object_get_082b70(long object_index)
{
	return ((s_082b70_object_header *)g_4e0300->data)[object_index & 0xffff].object;
}

/* src/unknown_0cbd50.cpp */
long function_cbd50(long unit_index, short weapon_index);

/* the weapons of the unit an object drives, when the object drives the unit
   it is ultimately attached to */
// @retail 0x82b70
void function_82b70(long object_index, long *weapons)
{
	if (object_index != NONE)
	{
		long root;
		long index = object_index;
		do
		{
			root = index;
			index = object_get_082b70(root)->parent;
		} while (index != NONE);
		if (root != NONE)
		{
			s_082b70_object *unit = object_get_082b70(root);
			if (((1 << unit->type) & 3) && unit->driver == object_index)
			{
				weapons[0] = function_cbd50(root, unit->weapon_slots[0]);
				weapons[1] = function_cbd50(root, object_get_082b70(root)->weapon_slots[1]);
			}
		}
	}
}
