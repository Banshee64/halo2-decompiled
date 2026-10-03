// @flags /O2 /Ob1 /Gr
/* UNKNOWN_16BCC0.CPP: walking a data array backwards (data_next_index's
   counterpart, used by the user interface lists) */

#include "cseries.h"
#include "data_array.h"

// @retail 0x16bcc0
long data_previous_index(s_data_array *data, long datum_index)
{
	long result = NONE;
	long index;

	if (datum_index == NONE)
	{
		index = data->high_water_index;
	}
	else
	{
		index = datum_index & 0xffff;
	}
	index--;

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
