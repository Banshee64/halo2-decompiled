// @flags /O1 /arch:SSE /Gr
/* UNKNOWN_24C177.CPP: a list's item widgets and the data they show: which
   item is focused, and moving the selection to a datum */

#include "cseries.h"
#include "data_array.h"
#include "screen_widgets.h"

long data_previous_index(s_data_array *data, long datum_index);

typedef long (__stdcall *datum_step_proc)(s_data_array *data, long datum_index);

/* a list's items are list item widgets; retail reads the widget's type
   (a check whose read survived) wherever it treats a child as one */
static inline c_list_item_widget *list_item(c_user_interface_widget *widget)
{
	volatile long type = widget->type;
	return (c_list_item_widget *)widget;
}

// @retail 0x24c177
long c_user_interface_widget::child_count()
{
	long count = 0;

	for (c_user_interface_widget *widget = child; widget; widget = widget->next)
	{
		count++;
	}
	return count;
}

// @retail 0x24c1a4
c_user_interface_widget *c_user_interface_widget::get_child(long index)
{
	for (c_user_interface_widget *widget = child; widget; widget = widget->next)
	{
		if (index-- == 0)
		{
			return widget;
		}
	}
	return 0;
}

// @retail 0x24bae6
c_user_interface_widget *c_list_widget::get_focused_item()
{
	c_user_interface_widget *widget;

	for (widget = child; widget; widget = widget->next)
	{
		if (widget->is_in_window())
		{
			break;
		}
	}
	return widget;
}

// @retail 0x24c447
long c_list_widget::get_focused_datum()
{
	c_user_interface_widget *item = get_focused_item();
	long datum = NONE;

	if (item)
	{
		datum = list_item(item)->value70;
	}
	return datum;
}

// @retail 0x24c5d0
c_user_interface_widget *c_list_widget::find_item(long datum)
{
	c_user_interface_widget *widget;

	for (widget = child; widget; widget = widget->next)
	{
		if (list_item(widget)->value70 == datum)
		{
			break;
		}
	}
	return widget;
}

/* the steps between the data of a list's items: with or without wrapping */

// @retail 0x24c6b7
long __stdcall datum_next_wrapping(s_data_array *data, long datum_index)
{
	long result;

	if (data)
	{
		result = data_next_index(data, datum_index);
		if (result != NONE)
		{
			return result;
		}
		result = data_next_index(data, NONE);
		if (result != datum_index)
		{
			return result;
		}
	}
	return NONE;
}

// @retail 0x24c6e2
long __stdcall datum_previous_wrapping(s_data_array *data, long datum_index)
{
	long result;

	if (data)
	{
		result = data_previous_index(data, datum_index);
		if (result == NONE)
		{
			long index = result;
			while ((index = data_next_index(data, index)) != NONE)
			{
				result = index;
			}
			if (result == datum_index)
			{
				result = NONE;
			}
		}
	}
	else
	{
		result = NONE;
	}
	return result;
}

// @retail 0x24c71e
long __stdcall datum_next(s_data_array *data, long datum_index)
{
	if (data && datum_index != NONE)
	{
		return data_next_index(data, datum_index);
	}
	return NONE;
}

// @retail 0x24c73e
long __stdcall datum_previous(s_data_array *data, long datum_index)
{
	long result = NONE;

	if (data && datum_index != NONE)
	{
		result = data_previous_index(data, datum_index);
	}
	return result;
}

/* shows the data around the datum in the items around the focused one */
// @retail 0x24c461
void c_list_widget::assign_items(long datum)
{
	c_user_interface_widget *focused = get_focused_item();
	datum_step_proc next = wraps ? datum_next_wrapping : datum_next;
	datum_step_proc previous = wraps ? datum_previous_wrapping : datum_previous;

	if (focused)
	{
		c_user_interface_widget *widget;
		long value;

		list_item(focused)->value70 = datum;
		value = datum;
		for (widget = focused->next; value = next(data, value), widget; widget = widget->next)
		{
			list_item(widget)->value70 = value;
		}
		value = datum;
		for (widget = focused->previous; value = previous(data, value), widget; widget = widget->previous)
		{
			list_item(widget)->value70 = value;
		}

		if (notify_screen)
		{
			c_screen_widget *screen = get_screen();
			if (screen)
			{
				long focused_datum = get_focused_datum();
				if (focused_datum != NONE)
				{
					screen->function_230427(&focused_datum);
				}
			}
		}
	}
}

// @retail 0x24c515
void c_list_widget::select_datum(long datum)
{
	c_user_interface_widget *item = find_item(datum);

	if (!item)
	{
		long count = child_count();
		s_data_array *items = data;
		long datum_count = items ? items->actual_count : 0;

		if (datum_count <= count)
		{
			return;
		}

		long datum_index = data_next_index(items, NONE);
		long index = 0;
		for (; datum_index != NONE; datum_index = data_next_index(items, datum_index), index++)
		{
			if (datum_index == datum)
			{
				item = get_child(index <= count - 1 ? index : count - 1);
				break;
			}
		}
		if (!item)
		{
			return;
		}
	}
	v7(item);
	assign_items(datum);
}

// @retail 0x24c591
void c_list_widget::select_item(short item)
{
	s_data_array *items = data;
	long datum_index;

	for (datum_index = data_next_index(items, NONE); datum_index != NONE; datum_index = data_next_index(items, datum_index))
	{
		if ((datum_index & 0xffff) == item)
		{
			break;
		}
	}
	if (datum_index != NONE)
	{
		select_datum(datum_index);
	}
}
