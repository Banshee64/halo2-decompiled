// @flags /O1 /Gr
/* UNKNOWN_1A2C81.CPP: the controller a widget belongs to */

#include "cseries.h"
#include "screen_widgets.h"

// @retail 0x1a2c81
long c_user_interface_widget::get_controller_index()
{
	word flags = user_flags;

	if (flags & 1)
	{
		return 0;
	}
	if (flags & 2)
	{
		return 1;
	}
	if (flags & 4)
	{
		return 2;
	}
	return (flags & 8) ? 3 : NONE;
}
