#include "unknown_11c920.h"
#include "screen_widgets.h"
#include "data_array.h"

// @flags /O1 /Gr

void function_24a190(void *p);

/* Rebuilds the list while keeping its focused slot in range. */
// @retail 0x24a150
void function_24a150(void *p)
{
	c_class_1474e8 *list = (c_class_1474e8 *)p;
	long index = list->get_focused_datum() & 0xffff;
	function_24a190(p);
	if (index < 0)
		index = 0;
	else if (index > list->data->actual_count - 1)
		index = list->data->actual_count - 1;
	list->select_datum(index_to_datum_index(list->data, index));
}
