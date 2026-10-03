// @flags /O2 /Gr
/* UNKNOWN_1E3480.CPP: the actor of an object (lane I's outside function,
   called by the props) */

#include "cseries.h"
#include "globals.h"

/* the object as 0x1e3480 reads it: a unit keeps its actor at +0x12c, a
   creature (type 12) in the block at its offset +0x13a */
struct s_actor_object
{
	byte unknown000[0xaa];
	char type;
	byte unknown0ab[0x12c - 0xab];
	long actor_index;
	byte unknown130[0x134 - 0x130];
	long unknown134;
	byte unknown138[0x13a - 0x138];
	short creature_offset;
};

struct s_actor_object_header
{
	byte unknown0[8];
	s_actor_object *object;
};

struct s_actor_creature
{
	byte unknown0[4];
	long actor_index;
};

// @retail 0x1e3480
long function_1e3480(long object_index)
{
	s_actor_object *object = ((s_actor_object_header *)g_4e0300->data)[object_index & 0xffff].object;
	long result = NONE;

	if ((1 << object->type) & 3)
	{
		result = object->actor_index;
	}
	else if (object->type == 12 && !object->unknown134)
	{
		s_actor_creature *creature = (s_actor_creature *)((byte *)object + object->creature_offset);

		if (creature)
		{
			result = creature->actor_index;
		}
	}
	return result;
}
