// @flags /O2 /Gr
/* UNKNOWN_100880.CPP: object queries */

#include "cseries.h"
#include "globals.h"

struct s_object_slot
{
	short value;
	byte unknown02[14];
};

struct s_object_100880
{
	dword tag_index;
	byte unknown04[0x220];
	s_object_slot slots[1];
};

struct s_object_definition_100880
{
	byte unknown00[0x2c0];
	long slot_count;
};

// @retail 0x100880
bool function_100880(long object_index, long slot_index)
{
	s_object_100880 *object = *(s_object_100880 **)((byte *)g_4e0300->data + 8 + (object_index & 0xffff) * 12);
	s_object_definition_100880 *definition = (s_object_definition_100880 *)g_4e3b44[object->tag_index & 0xffff].bytes;
	bool result = false;
	for (long i = 0; i < definition->slot_count; i++)
	{
		if (slot_index == NONE || slot_index == i)
		{
			long value = object->slots[(short)i].value;
			if (value > 0 && value <= 6)
			{
				result = true;
				break;
			}
		}
	}
	return result;
}
