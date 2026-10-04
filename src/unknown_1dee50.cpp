// @flags /O2 /Gr
/* UNKNOWN_1DEE50.CPP: the first object of an object list */

#include "cseries.h"
#include "globals.h"
#include "unknown_1dee50.h"

/* the object lists (g_4f55d8, hs_library_external.cpp), 12 bytes each */
struct s_object_list_datum_1dee50
{
	byte unknown00[8];
	long first_reference_index;
};

extern s_record_pool *g_4f55d8;

long function_1dee80(long *reference_index);

// @retail 0x1dee50
long function_1dee50(long list_index, long *reference_index)
{
	long object_index = NONE;
	if (list_index != NONE)
	{
		*reference_index = ((s_object_list_datum_1dee50 *)g_4f55d8->data)[list_index & 0xffff].first_reference_index;
		object_index = function_1dee80(reference_index);
	}
	return object_index;
}