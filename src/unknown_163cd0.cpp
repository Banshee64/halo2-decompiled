// @flags /O2 /Gr
/* UNKNOWN_163CD0.CPP: calls a procedure for the entries of a list whose
   masks match */

#include "unknown_11c920.h"

typedef void (__stdcall *masked_list_proc)(long value, short mask);

struct s_masked_list
{
	byte unknown00[4];
	word count;
	byte unknown06[2];
	word *masks;
	long *values;
};

// @retail 0x163cd0
void masked_list_iterate(s_masked_list const *list, long mask, masked_list_proc proc)
{
	long i;

	for (i = 0; i < list->count; i++)
	{
		if (((short)mask & list->masks[i]) || (mask & 0xffff) == 0xffff)
		{
			proc(list->values[i], mask);
		}
	}
}
