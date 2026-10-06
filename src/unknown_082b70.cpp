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

struct s_object_relevance_source;
struct s_object_relevance_result
{
 real first;
 real second;
 long object_index;
 long identifier;
};
void function_82ac0(s_object_relevance_source *source, s_object_relevance_result *result);

struct s_weapon_activity_result
{
 byte unknown00[0x18];
 bool active[2][2];
 bool update_relevance;
 byte unknown1d[3];
 s_object_relevance_result relevance;
 bool consumed[2];
};

struct s_weapon_trigger_view
{
 byte unknown00[8];
 short barrel;
 byte unknown0a[2];
 short mode;
 byte unknown0e[0x40 - 0xe];
};

struct s_weapon_definition_view
{
 byte unknown00[0x2c8];
 long trigger_count;
 s_weapon_trigger_view *triggers;
 long barrel_count;
 byte *barrels;
};

// @retail 0x82d20
void function_82d20(long object_index, s_object_relevance_source *source, s_weapon_activity_result *result)
{
 result->relevance.object_index = NONE;
 result->relevance.identifier = NONE;
 result->relevance.first = 0.0f;
 result->relevance.second = 0.0f;
 result->active[0][0] = false;
 result->active[0][1] = false;
 result->active[1][0] = false;
 result->active[1][1] = false;
 result->consumed[0] = false;
 result->consumed[1] = false;
 result->update_relevance = false;
 if (object_index == NONE)
  return;
 long weapons[2];
 weapons[0] = object_query_weapon(object_index, object_get_082b70(object_index)->weapon_slots[0]);
 weapons[1] = object_query_weapon(object_index, object_get_082b70(object_index)->weapon_slots[1]);
 if (weapons[0] == NONE && weapons[1] == NONE)
 {
  function_82b70(object_index, weapons);
  if (weapons[0] == NONE && weapons[1] == NONE)
   function_82c10(object_index, weapons);
 }
 for (long hand = 0; hand < 2; hand++)
 {
  long weapon_index = weapons[hand];
  if (weapon_index != NONE)
  {
   byte *weapon = (byte *)object_get_082b70(weapon_index);
   s_weapon_definition_view *definition = (s_weapon_definition_view *)g_4e3b44[*(dword *)weapon & 0xffff].bytes;
   for (long trigger_index = 0; trigger_index < definition->trigger_count; trigger_index++)
   {
    s_weapon_trigger_view *trigger = &definition->triggers[trigger_index];
    long barrel = trigger->barrel;
    if (barrel >= 0 && barrel < definition->barrel_count)
    {
     byte *state = (byte *)object_get_082b70(weapon_index) + 0x1a4 + barrel * 0x34;
     word flags = *(word *)(state + 4);
     if (flags & 0x80)
     {
      *(word *)(state + 4) = flags & 0xff7f;
      result->consumed[hand] = true;
     }
    }
    byte *trigger_state = weapon + 0x20c + trigger_index * 0xc;
    switch (trigger->mode)
    {
    case 2:
     if (*trigger_state == 1 || *trigger_state == 2)
      result->active[hand][trigger_index] = true;
     break;
    case 1:
     if ((trigger_state[4] & 0x20) && trigger->barrel != NONE &&
      *(short *)(definition->barrels + trigger->barrel * 0xec + 0x34) == 1)
     {
      result->active[hand][trigger_index] = true;
      result->update_relevance = true;
     }
     break;
    }
   }
  }
 }
 if (result->update_relevance)
  function_82ac0(source, &result->relevance);
}
