/* SCREEN_WIDGETS.H: the user interface widget classes of the screens and
   lists in 0x2b0000..0x2cb8c0, numbered by their retail vtable slots.

   Three base layouts recur in the vtables at 0x458788..0x45d920:
   - the widget (17 slots, e.g. 0x458788);
   - the screen (28 slots, e.g. 0x458840): slot 26 returns the screen's load
     procedure (the __stdcall function that allocates and constructs it, which
     the screen history uses to rebuild it), slot 27 reads a flag at +0x5f4;
   - the list (22 slots, e.g. 0x458988): slot 18 returns the list's item
     data, slot 19 its item count, slot 20 fills in one item.
   The slots not decompiled yet have empty bodies here. unknown_19b516.h views
   the 24-slot list at 0x4594a0 as c_widget, with its slots rotated by 8. */

#ifndef SCREEN_WIDGETS_H
#define SCREEN_WIDGETS_H

#include "cseries.h"
#include "data_array.h"

struct s_screen_parameters;
struct s_widget_event;
class c_screen_widget;

typedef c_screen_widget *(__stdcall *screen_load_proc)(s_screen_parameters *parameters);

class c_user_interface_widget
{
public:
	virtual ~c_user_interface_widget() {}
	virtual void v1() {}
	virtual void v2() {}
	virtual void v3() {}
	virtual void v4(long) {}
	virtual bool v5(s_widget_event *) { return false; }
	virtual long v6() { return 0; }
	virtual void v7(c_user_interface_widget *) {}
	virtual void v8() {}
	virtual void v9() {}
	virtual bool v10(s_widget_event *) { return false; }
	virtual long v11() { return 0; }
	virtual long v12() { return 0; }
	virtual void v13() {}
	virtual void v14() {}
	virtual long v15() { return 0; }
	virtual bool v16() { return false; }

	byte unknown04[0xc];
	c_user_interface_widget *parent;
	c_user_interface_widget *child;
	byte unknown18[0x70 - 0x18];
	s_data_array *data;
	byte unknown74[0x80 - 0x74];
};

class c_screen_widget : public c_user_interface_widget
{
public:
	virtual void v17() {}
	virtual void v18() {}
	virtual void v19() {}
	virtual long v20() { return 0; }
	virtual long v21() { return 0; }
	virtual void v22() {}
	virtual void v23() {}
	virtual void v24() {}
	virtual void v25() {}
	virtual screen_load_proc get_load_proc() { return 0; }
	virtual bool v27() { return false; }
};

class c_list_widget : public c_user_interface_widget
{
public:
	virtual void v17() {}
	virtual void *get_item_data() { return 0; }
	virtual long get_item_count() { return 0; }
	virtual void v20(c_user_interface_widget *, long) {}
	virtual void v21() {}
};

/* the lists with a 23rd slot (0x45b3e0, 0x45b510) and the 26-slot lists of
   the vtable at 0x459e88 (0x45af88): slot 22 returns the items and their
   count */
class c_list_widget_with_items : public c_list_widget
{
public:
	virtual void *get_items(long *count) { return 0; }
};

class c_list_widget_26 : public c_list_widget_with_items
{
public:
	virtual void v23() {}
	virtual void v24() {}
	virtual void v25() {}
};

#endif
