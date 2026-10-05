// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_0DC310.CPP: an object state test (an outside function lane A's
   script evaluator 0x2a2010 needs) */

#include "unknown_11c920.h"
#include "globals.h"

struct s_dc310_object
{
	byte unknown00[0xec];
	real valueec;
	byte unknownf0[0x10a - 0xf0];
	word unknown10a : 14;
	word flag10a_14 : 1;
	word unknown10a_15 : 1;
};

struct s_dc310_object_header
{
	byte unknown00[8];
	s_dc310_object *object;
};

/* what function_d5b60 returns: flags in the first byte */
struct s_dc310_definition
{
	byte unknown0 : 7;
	byte flag7 : 1;
};

long function_d5b60(long object_index);

// @retail 0xdc310
bool function_dc310(long object_index)
{
	bool result = false;

	if (object_index != NONE)
	{
		s_dc310_object *object = ((s_dc310_object_header *)g_4e0300->data)[object_index & 0xffff].object;
		s_dc310_definition *definition = (s_dc310_definition *)function_d5b60(object_index);

		if (TEST_FIELD_BIT(object->flag10a_14) || (definition && TEST_FIELD_BIT(definition->flag7)))
		{
			if (0.0002f >= object->valueec)
				result = true;
		}
	}

	return result;
}
