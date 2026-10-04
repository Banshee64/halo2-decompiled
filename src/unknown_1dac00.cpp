// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1DAC00.CPP: whether objects are in a state that animation may
   drive (0x1dac00, 0x1dac40) */

#include "unknown_11c920.h"
#include "globals.h"
#include "data_array.h"

struct s_animated_object
{
	byte unknown000[0x3dc];
	char state;
};

struct s_animated_object_header
{
	short identifier;
	byte flags;
	byte type;
	byte unknown04[4];
	s_animated_object *object;
};

static inline s_animated_object_header *animated_object_header_get(long object_index)
{
	return &((s_animated_object_header *)g_4e0300->data)[object_index & 0xffff];
}

/* a biped in state 6 */
static inline bool object_biped_in_state6(long object_index)
{
	s_animated_object_header *header = animated_object_header_get(object_index);
	bool result = false;

	if (((1 << header->type) & 1) && header->object->state == 6)
	{
		result = true;
	}
	return result;
}

// @retail 0x1dac00
long function_1dac00(long object_index)
{
	return !object_biped_in_state6(object_index);
}

// @retail 0x1dac40
long function_1dac40(long object_index, long other_object_index)
{
	if (object_biped_in_state6(object_index) || object_biped_in_state6(other_object_index))
	{
		return false;
	}
	return true;
}
