// @flags /O2 /Gr
/* DATA_ITERATOR.CPP: the data iterator that remembers its current datum, used
   all over the game (lane D) */

#include "cseries.h"
#include "data_array.h"

// @retail 0x6b380
bool data_datum_iterator_next(s_data_datum_iterator *iterator)
{
	s_data_array *data = iterator->data;
	long index = data_next_absolute_index(data, iterator->index + 1);
	byte *datum;

	if (index != NONE)
	{
		datum = data->data + data->size * index;
		iterator->index = index;
		iterator->datum_index = (*(short *)datum << 16) | index;
	}
	else
	{
		iterator->index = data->maximum_count;
		iterator->datum_index = NONE;
		datum = 0;
	}
	iterator->datum = datum;
	return datum != 0;
}
