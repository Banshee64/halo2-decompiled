// @flags /O2 /Ob1 /Gr
/* UNKNOWN_16BCC0.CPP: walking a data array backwards (record_pool_next_used's
   counterpart, used by the user interface lists) */

#include "unknown_11c920.h"
#include "data_array.h"

// @retail 0x16bcc0
long function_16bcc0(s_record_pool *data, long datum_index)
{
	long result = NONE;
	long index;

	if (datum_index == NONE)
	{
		index = data->high_water_index - 1;
	}
	else
	{
		index = (datum_index & 0xffff) - 1;
	}

	if (index >= 0 && index < data->high_water_index)
	{
		byte *datum = data->data + data->size * index;
		do
		{
			if (*(short *)datum)
			{
				result = (*(short *)datum << 16) | index;
				break;
			}
			datum -= data->size;
		}
		while (index-- >= 0);
	}
	return result;
}
