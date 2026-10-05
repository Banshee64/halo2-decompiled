// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_0C40F0.CPP: setting a value on the data (g_4e031c) an object keeps
   for a tag. Decompiled by lane R for the effects (0x1771a0). */

#include "unknown_11c920.h"
#include "globals.h"
#include "data_array.h"

s_record_pool *g_4e031c;

struct s_c40f0_datum
{
	short salt;
	byte unknown02[6];
	long tag_index;
	long object_index;
	real value;
	byte unknown14[0x110 - 0x14];
};

struct s_c40f0_definition
{
	byte unknown00[2];
	short unknown02;
};

// @retail 0xc40f0
void function_c40f0(long tag_index, long object_index, real value)
{
	s_record_pool *array = g_4e031c;

	if (array && array->valid && tag_index != NONE && object_index != NONE)
	{
		s_c40f0_definition *definition = (s_c40f0_definition *)g_4e3b44[tag_index & 0xffff].bytes;
		long datum = data_datum_index(array, function_16bc00(array, 0));

		while (datum != NONE)
		{
			s_c40f0_datum *element = (s_c40f0_datum *)array->data + (datum & 0xffff);

			if (element->object_index == object_index && element->tag_index == tag_index &&
				((s_c40f0_definition *)g_4e3b44[element->tag_index & 0xffff].bytes)->unknown02 == 0 &&
				definition->unknown02 == 0)
			{
				real clamped;

				if (0.0f > value)
					clamped = 0.0f;
				else if (value > 1.0f)
					clamped = 1.0f;
				else
					clamped = value;
				element->value = clamped;
			}
			datum = data_datum_index(array, function_16bc00(array, datum == NONE ? 0 : (datum & 0xffff) + 1));
		}
	}
}
