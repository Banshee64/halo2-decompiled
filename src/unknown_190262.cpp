// @flags /O1 /Ob2 /arch:SSE /Gr
#include "cseries.h"
#include "unknown_19b516.h"

// @retail 0x190262
long function_190262(long value)
{
	long result;

	switch (value)
	{
	case NONE:
		result = 0;
		break;
	case 0:
		result = 1;
		break;
	case 1:
		result = 2;
		break;
	case 2:
		result = 3;
		break;
	default:
		result = NONE;
		break;
	}
	return result;
}

// the widget at the top of the parent chain, or this widget when it has no
// parent and no child
// @retail 0x22eeee
c_widget *c_widget::function_22eeee()
{
	c_widget *widget = parent;

	if (widget)
	{
		c_widget *next;
		while ((next = widget->parent) != 0)
			widget = next;
	}
	if (!widget && *(long *)unknown04 == 0)
		widget = this;
	return widget;
}
