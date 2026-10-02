// @flags /O2 /Gr
/* UNKNOWN_16BCC0.CPP: data array iteration (continues unknown_16b570.cpp) */

#include "cseries.h"
#include "data_array.h"

// @retail 0x16bcc0
long data_previous_index(s_data_array *data, long datum_index)
{
	long result = NONE;

	if (datum_index == NONE)
	{
		datum_index = data->high_water_index;
	}
	else
	{
		datum_index &= 0xffff;
	}

	datum_index--;
	if (datum_index >= 0 && datum_index < data->high_water_index)
	{
		byte *element = data->data + data->size * datum_index;
		do
		{
			if (*(short *)element)
			{
				result = (*(short *)element << 16) | datum_index;
				break;
			}
			element -= data->size;
		}
		while (datum_index-- >= 0);
	}
	return result;
}
