#include "cseries.h"
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

s_data_array *g_4f55d4;

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
