// @flags /O1 /Gr
/* UNKNOWN_253C8B.CPP: the button widget (type 3, vtable 0x45a628) */

#include "cseries.h"
#include "screen_widgets.h"

// @retail 0x253c8b
c_button_widget::c_button_widget(word valuef8, word user_flags) :
	c_user_interface_widget(3, user_flags),
	valuef4(0),
	valuef8(valuef8)
{
}

// @retail 0x237623 deleting c_button_widget
// @retail 0x19b8b1 destructor c_button_widget

// @retail 0x237607
c_user_interface_text *c_button_widget::get_text()
{
	return &text;
}
