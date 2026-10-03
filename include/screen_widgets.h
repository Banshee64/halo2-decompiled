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
#include "real_math.h"

class __single_inheritance c_screen_widget;
struct s_screen_parameters;

/* frees a block of the user interface heap */
void __stdcall user_interface_free(void *pointer);

typedef c_screen_widget *(__stdcall *screen_load_proc)(s_screen_parameters *parameters);

/* what a screen is loaded with (0x20 bytes, built by function_149f49, which
   unknown_19b516.h declares with this as an s_message): the controllers it is
   for, the two values its constructor takes, and its load procedure */
struct s_screen_parameters
{
	word type;
	word user_flags;
	long a;
	long b;
	dword field_c;
	dword id[3];
	screen_load_proc load;
};

/* a reference to a controller (the index at +4) */
struct s_controller_reference
{
	byte unknown00[4];
	long controller_index;
};

/* an input event (type 5 is a button press; param is the button) */
struct s_widget_event
{
	long type;
	long controller_index;
	long param;
};

/* an animation of a widget (0x34 bytes): the definition is copied into the
   widget's state by c_user_interface_widget::set_animation */
struct s_widget_animation
{
	long type;
	long target;
	short value8;
	short valuea;
	short direction;
	short valuee;
	long duration;
	long value14;
	long start_time;
	long end_time;
	long value20;
	real progress;
	real value28;
	real value2c;
	real scale;
};

/* a node of an intrusive doubly linked list; the list is its head pointer */
struct s_list_node;
void list_node_detach(s_list_node *node);

struct s_list_node
{
	s_list_node()
	{
		next = 0;
		previous = 0;
		list = 0;
	}
	/* leaves its list */
	~s_list_node()
	{
		list_node_detach(this);
	}

	s_list_node *previous;
	s_list_node *next;
	s_list_node **list;
};

void list_remove(s_list_node **list, s_list_node *node);
void list_remove_all(s_list_node **list);
void list_append(s_list_node **list, s_list_node *node);

struct s_controller_reference;

/* a list's item handler (vtable 0x45bdb0 for every instance: the one slot
   calls a method of the owner). The list node follows the vtable pointer. */
class c_list_item_delegate : public s_list_node
{
public:
	virtual void invoke(s_controller_reference **controller, long *item) = 0;
};

void delegate_register(s_list_node **list, c_list_item_delegate *delegate);

/* a widget's text (vtable 0x4576d0): slot 1 sets the string, slot 2 returns
   it */
class c_user_interface_text
{
public:
	c_user_interface_text();
	virtual ~c_user_interface_text() {}
	/* pure in retail (the stand-ins cannot construct an abstract class) */
	virtual void set_text(word *text) {}
	virtual word *get_text() { return 0; }

	void update_length();

	long value04;
	real_rgb_color color;
	short value14;
	short value16;
	long value18;
	long value1c;
	real value20;
	long value24;
	byte unknown28[0x38 - 0x28];
	long value38;
	short cursor;
	short length;
	long value40;
};

/* a text with its own buffer (vtable 0x458930) */
class c_user_interface_text_buffer : public c_user_interface_text
{
public:
	c_user_interface_text_buffer();
	virtual void set_text(word *text);
	virtual word *get_text();

	word text[0x100];
};

class c_user_interface_widget
{
public:
	c_user_interface_widget(long type, word user_flags);
	virtual ~c_user_interface_widget();
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
	virtual c_user_interface_text *get_text() { return 0; }
	virtual bool v16() { return false; }

	/* the widgets are allocated from the user interface heap (0x1a47fd) */
	static void *__stdcall operator new(unsigned int size);

	/* unknown_22e27b.cpp */
	void delete_children();
	void set_animation(s_widget_animation *animation);
	c_user_interface_widget *find_child(long type, short index, bool recursive);

	long type;
	word user_flags;
	short value0a;
	long value0c;
	c_user_interface_widget *parent;
	c_user_interface_widget *child;
	c_user_interface_widget *next;
	c_user_interface_widget *previous;
	short bounds[4];
	real_rgb_color color;
	s_widget_animation animation;
	short value68;
	short value6a;
	bool m6c;
	bool value6d;
	bool value6e;
	byte unknown6f;
};

/* a text widget (type 6, vtable 0x45a5e0); slot 15 returns its text */
class c_text_widget_45a5e0 : public c_user_interface_widget
{
public:
	c_text_widget_45a5e0(word user_flags);

	long value70;
};

/* a text widget with its own text buffer (vtable 0x458940); a screen has two */
class c_text_widget_458940 : public c_text_widget_45a5e0
{
public:
	c_text_widget_458940(word user_flags);
	virtual c_user_interface_text *get_text();

	c_user_interface_text_buffer text;
};

/* the screen's delegate (vtable 0x45bdb0: retail folded its one slot with
   the list item delegates') */
class c_screen_delegate : public s_list_node
{
public:
	c_screen_delegate(c_screen_widget *owner, void (c_screen_widget::*method)(short *delta)) :
		owner(owner),
		method(method)
	{
	}
	virtual void invoke(short *delta)
	{
		(owner->*method)(delta);
	}

	c_screen_widget *owner;
	void (c_screen_widget::*method)(short *delta);
};

class c_screen_widget : public c_user_interface_widget
{
public:
	c_screen_widget(long screen_id, long a, long b, word user_flags);

	/* 0x2300ea: a press of B or back leaves the screen (stub) */
	virtual bool v10(s_widget_event *event);

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

	/* places the newly loaded screen in its window (unknown_147f6d.cpp) */
	void function_147f6d(s_screen_parameters *parameters);

	/* the delegate's method (0x230427, not decompiled yet) */
	void function_230427(short *delta);

	long screen_id;
	long a;
	long b;
	long next_widget_id;
	c_text_widget_458940 title;
	c_text_widget_458940 subtitle;
	short value5f0;
	bool value5f2;
	char value5f3;
	bool value5f4;
	byte unknown5f5[3];
	c_screen_delegate delegate;
};

/* unknown_19b516.h's c_widget is a list of this family (vtable 0x4594a0)
   with its slots rotated by 8; its v9, v10 and v11 are slots 1, 2 and 3 here.
   See the note there on why the two views are still separate. */
class c_list_widget : public c_user_interface_widget
{
public:
	virtual void v17() {}
	virtual void *get_item_data() { return 0; }
	virtual long get_item_count() { return 0; }
	virtual void v20(c_user_interface_widget *, long) {}
	virtual void v21() {}

	s_data_array *data;
	byte unknown74[0x80 - 0x74];
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
