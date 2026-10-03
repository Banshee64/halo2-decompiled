/* UNKNOWN_19B510.H: the error dialog screens (unknown_19b510.cpp) and the
   button widget the "ok" dialog holds */

#ifndef UNKNOWN_19B510_H
#define UNKNOWN_19B510_H

#include "cseries.h"
#include "screen_widgets.h"

/* a dialog's definition, as 0x23661f reads it (0x20 bytes): the string list
   and the ids of its strings, the number of choices and its screen */
struct s_dialog_definition
{
	byte unknown00[6];
	char choices;
	byte unknown07;
	long string_list_index;
	long title;
	long message;
	long first_choice;
	long second_choice;
	short screen_id;
	byte unknown1e[2];
};

void function_23661f(s_dialog_definition *definition, long dialog_id);

/* the widget at +0xe1c of the "ok" dialog (vtable 0x45a628, 0x100 bytes; its
   constructor is 0x253c8b) */
class c_dialog_button : public c_user_interface_widget
{
public:
	c_dialog_button(short index, word user_flags);

	c_user_interface_text text;
	byte unknownb4[0xf4 - 0xb4];
	long valuef4;
	short index;
	byte unknownfa[2];
	s_list_head handlers;
};

#endif
