// @flags /O2 /Gr
/* UNKNOWN_082B70.CPP: object queries of the simulation's entity code
   (lane D) */

#include "unknown_11c920.h"
#include "globals.h"
#include <string.h>

/* the object fields read here */
struct s_082b70_object
{
	byte unknown000[0xc];
	long next;
	long child;
	long parent;
	byte unknown018[0xaa - 0x18];
	char type;
	byte unknown0ab[0x212 - 0xab];
	char weapon_slots[2];
	byte unknown214[4];
	long weapons[4];
	byte unknown228[0x248 - 0x228];
	long occupant;
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

long function_c7100(long unit_index);

static __forceinline long object_query_weapon(long unit_index, short slot)
{
	long result = NONE;
	if (slot != NONE)
		result = object_get_082b70(unit_index)->weapons[slot];
	return result;
}

// @retail 0x82c10
void __stdcall function_82c10(long object_index, long *weapons)
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
			for (long child = object_get_082b70(root)->child; child != NONE; )
			{
				s_082b70_object *unit = object_get_082b70(child);
				if (((1 << unit->type) & 3) && unit->driver != NONE && function_c7100(child) == object_index)
				{
					if (weapons[0] == NONE)
						weapons[0] = object_query_weapon(child, object_get_082b70(child)->weapon_slots[0]);
					if (weapons[1] == NONE)
						weapons[1] = object_query_weapon(child, object_get_082b70(child)->weapon_slots[1]);
				}
				child = unit->next;
			}
		}
	}
}

point3f *function_b9dd0(long object_index, point3f *result);
void function_b9fc0(long object_index, vector3f *forward, vector3f *up);
void function_ba1d0(long object_index, vector3f *linear, vector3f *angular);
long function_a5980(long index);

struct s_player_object_motion
{
	long object_index;
	point3f position;
	vector3f forward;
	vector3f up;
	vector3f linear;
	vector3f angular;
};

// @retail 0x828b0
bool function_828b0(long player_index, s_player_object_motion *result)
{
	bool valid = false;
	long unit_index = *(long *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c + 0x2c);
	if (unit_index != NONE)
	{
		long parent = object_get_082b70(unit_index)->parent;
		if (parent != NONE)
		{
			s_082b70_object *object = object_get_082b70(parent);
			if (!((1 << object->type) & 3) || object->occupant != unit_index)
				return false;
			unit_index = parent;
		}
		if (unit_index != NONE)
		{
			long mapped = function_a5980(unit_index);
			if (mapped != NONE)
			{
				memset(result, 0, sizeof(*result));
				result->object_index = mapped;
				function_b9dd0(unit_index, &result->position);
				function_b9fc0(unit_index, &result->forward, &result->up);
				function_ba1d0(unit_index, &result->linear, &result->angular);
				return true;
			}
		}
		return false;
	}
	return valid;
}
