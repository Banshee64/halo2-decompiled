#include "unknown_11c920.h"
#include <xmmintrin.h>
#include "globals.h"

// @flags /O2 /Ob1 /arch:SSE /Gr

/* a datum of the linked list array g_4f55d4: the next link at +8 and the
   value returned at +4 */
struct s_linked_datum
{
	byte unknown00[4];
	long value;
	long next;
};

s_record_pool *g_4f55d4;

PRIVATE __forceinline long object_list_next_inlined(long *index)
{
	long result;
	if (*index != NONE)
	{
		long size = g_4f55d4->size;
		byte *data = g_4f55d4->data;
		s_linked_datum *datum = (s_linked_datum *)(data + (*index & 0xffff) * size);
		long next = datum->next;

		if (next != NONE)
			_mm_prefetch((const char *)(data + (next & 0xffff) * size), _MM_HINT_T0);

		*index = next;
		result = datum->value;
	}
	else
		result = NONE;
	return result;
}

// @retail 0x1dee80
long function_1dee80(long *index)
{
	if (*index != NONE)
	{
		long size = g_4f55d4->size;
		byte *data = g_4f55d4->data;
		s_linked_datum *datum = (s_linked_datum *)(data + (*index & 0xffff) * size);
		long next = datum->next;
		if (next != NONE)
			_mm_prefetch((const char *)(data + (next & 0xffff) * size), _MM_HINT_T0);
		*index = next;
		return datum->value;
	}
	return NONE;
}

struct s_counted_object_list
{
	byte unknown00[8];
	long first;
};

struct s_counted_object
{
	byte unknown000[0x10a];
	word unknown10a_0 : 2;
	word excluded : 1;
	word unknown10a_3 : 13;
};

struct s_counted_object_header
{
	byte unknown00[8];
	s_counted_object *object;
};

extern s_record_pool *g_4f55d8;

// @retail 0x1defb0
short function_1defb0(long list_index)
{
	struct { long reference; } state;
	long count = 0;
	if (list_index != NONE)
	{
		state.reference = ((s_counted_object_list *)g_4f55d8->data)[list_index & 0xffff].first;
		long object_index = function_1dee80(&state.reference);
		while (object_index != NONE)
		{
			s_counted_object *object = ((s_counted_object_header *)g_4e0300->data)[object_index & 0xffff].object;
			if (!TEST_FIELD_BIT(object->excluded))
				count++;
			object_index = object_list_next_inlined(&state.reference);
		}
	}
	return (short)count;
}
