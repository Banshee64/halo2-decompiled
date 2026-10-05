/* UNKNOWN_2B116A.H: shared types of the screens and lists of
   src/unknown_2b116a.cpp */

#ifndef UNKNOWN_2B116A_H
#define UNKNOWN_2B116A_H

#include "unknown_11c920.h"
#include "data_array.h"
#include "screen_widgets.h"

/* an iterator over a list's items (the item, then the data iterator) */
struct s_list_item_iterator
{
	byte *item;
	s_record_pool_iterator iterator;
};

bool function_2b2327(s_list_item_iterator *iterator);

/* a screen that shows a short text and a bitmap (vtable 0x45bd40; the window
   channels load it, 0x23591a) */
class c_screen_45bd40 : public c_class_1473c9
{
public:
	c_screen_45bd40(long a, long b, word user_flags);

	/* shows the text */
	virtual void v3();
	virtual void v18(void *parameters);
	virtual screen_load_proc get_load_proc();

	void set_text(const char *string);
	void set_bitmap(short index);

	word text[0x10];
};

#endif
