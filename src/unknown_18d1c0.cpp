// @flags /O2 /Gr
/* UNKNOWN_18D1C0.CPP: lookup in the tag-reference data array (g_4ed28c) */

#include "unknown_11c920.h"
#include "data_array.h"
#include "globals.h"
#include "unknown_03d380.h"

struct s_18d1c0_flags
{
	word flag0 : 1;
	word flag1 : 1;
	word flag2 : 1;
	word flag3 : 1;
	word flag4 : 1;
	word flag5 : 1;
	word flag6 : 1;
	word flag7 : 1;
	word flag8 : 8;
};

struct s_18d1c0_element
{
	byte unknown00[3];
	byte state;
	s_18d1c0_flags flags;
	byte unknown06[6];
	long value;
	byte unknown10[8];
};

// @retail 0x18d1c0
long __stdcall function_18d1c0(long value)
{
	s_record_pool *array = g_4ed28c;
	long datum = data_datum_index(array, function_16bc00(array, 0));

	while (datum != NONE)
	{
		s_18d1c0_element *element = (s_18d1c0_element *)(array->data + sizeof(s_18d1c0_element) * (datum & 0xffff));

		if (element->value == value && TEST_FIELD_BIT(element->flags.flag5))
			break;

		datum = data_datum_index(array, data_find_index(array, datum == NONE ? 0 : (datum & 0xffff) + 1));
	}

	return datum;
}
