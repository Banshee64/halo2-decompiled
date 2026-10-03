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

/* a player profile's settings (0x1e0 bytes); the settings screens edit a
   copy at 0x54e5d8 */
struct s_player_profile_settings
{
	byte unknown000[0xfc];
	struct
	{
		dword invert_look : 1;
		dword vibration : 1;
		dword bit2 : 1;
		dword auto_level : 1;
		dword bits4 : 28;
	} controller_flags;
	byte button_layout;
	byte thumbstick_layout;
	byte look_sensitivity;
	byte unknown103[0x118 - 0x103];
	byte colors[4];
	byte model;
	byte unknown11d[2];
	bool flag;
	byte unknown120[0x148 - 0x120];
	long voice_mask;
	long voice_through_tv;
	byte unknown150;
	byte subtitles;
	byte unknown152[0x1e0 - 0x152];
};

/* the profile being edited (0x54e5d0): its player, its datum and a copy of
   its settings */
struct s_profile_edit
{
	long player;
	long profile_index;
	s_player_profile_settings settings;
};

extern s_profile_edit g_54e5d0;

/* a game variant (0x130 bytes): its name, its game engine and flags */
struct s_game_variant
{
	dword unknown00;
	word name[0x20];
	long game_engine_index;
	dword teams_enabled : 1;
	dword motion_sensor_enabled : 1;
	dword flags_bits2 : 30;
	byte unknown4c[0x130 - 0x4c];
};

/* the game variant being edited (or shown when no session holds one) and
   its saved game file index (user_interface_text_parser.cpp) */
extern long g_54e49c;
extern s_game_variant g_54e4a0;

/* unknown_147f6d.cpp */
void function_14800c(long channel, long index);
bool function_148044(long channel, long index, long value);
void profile_edit_begin(long player, s_player_profile_settings *settings, long profile_index);
void profile_edit_save();
void profile_edit_end();

/* the user interface heap (unknown_1a4742.cpp) */
void *__stdcall user_interface_malloc(unsigned int size);
void __stdcall user_interface_free(void *pointer);

typedef c_screen_widget *(__stdcall *screen_load_proc)(s_screen_parameters *parameters);

/* what a screen is loaded with (0x20 bytes, built by function_149f49, which
   unknown_19b516.h declares with this as an s_message): the controllers it is
   for, the two values its constructor takes, and its load procedure. The
   window channels queue these as requests (unknown_234c64.h). */
struct s_screen_parameters
{
	union
	{
		word type;
		struct
		{
			word type_bit0 : 1;
			word type_bit1 : 1;
			word type_bit2 : 1;
		};
	};
	word user_flags;
	long a;
	long b;
	dword field_c;
	dword id[3];
	screen_load_proc load;
};

/* the focus a screen keeps when it is rebuilt: the focused widget and the
   focused datum of its list */
struct s_screen_focus
{
	long unknown00;
	long widget_id;
	long datum;
};

/* a block of values of a screen definition */
struct s_screen_value_block
{
	byte unknown00[4];
	long count;
	long *values;
};

/* a tag reference of a tag block */
struct s_tag_reference
{
	dword group_tag;
	long tag_index;
};

/* a list's definition in its screen's pane */
struct s_list_definition
{
	byte unknown00[4];
	short skin_index;
};

/* a pane of a screen definition (0x4c bytes) */
struct s_screen_pane
{
	byte unknown00[2];
	short value02;
	byte unknown04[0xc - 0x04];
	long list_count;
	s_list_definition *lists;
	byte unknown14[0x4c - 0x14];
};

/* a screen's definition tag */
struct s_screen_definition
{
	byte unknown00[4];
	short screen_id;
	byte unknown06[0x1c - 0x06];
	long string_list_index;
	long pane_count;
	s_screen_pane *panes;
	byte unknown28[0x30 - 0x28];
	long value_block_count;
	s_screen_value_block *value_blocks;
	long bitmap_count;
	s_tag_reference *bitmaps;
};

/* a text buffer of 0x100 characters, empty when constructed (retail's
   out-of-line copy of the constructor is 0x7f8a0) */
struct s_text_256
{
	s_text_256()
	{
		text[0] = 0;
	}

	word text[0x100];
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
	union
	{
		short valuee;
		/* the window channels test these at widget +0x42 */
		struct
		{
			word flag0 : 1;
			word flag1 : 1;
		} flags;
	};
	long duration;
	long value14;
	long start_time;
	long end_time;
	long value20;
	/* x and y move the widget; z is its depth */
	real_point3d offset;
	real scale;
};

/* a widget's bounds */
struct s_widget_bounds
{
	short top;
	short left;
	short bottom;
	short right;
};

/* an intrusive doubly linked list: a node, and the list's head (a node
   leaves its list when it dies; a dying list empties itself) */
struct s_list_node;
struct s_list_head;

void list_node_detach(s_list_node *node);
void list_remove(s_list_head *list, s_list_node *node);
void list_remove_all(s_list_head *list);
void list_append(s_list_head *list, s_list_node *node);

struct s_list_node
{
	s_list_node()
	{
		next = 0;
		previous = 0;
		list = 0;
	}
	~s_list_node()
	{
		list_node_detach(this);
	}

	s_list_node *previous;
	s_list_node *next;
	s_list_head *list;
};

struct s_list_head
{
	s_list_head()
	{
		first = 0;
	}
	~s_list_head()
	{
		list_remove_all(this);
	}

	s_list_node *first;
};

struct s_controller_reference;

/* a list's item handler (vtable 0x45bdb0 for every instance: the one slot
   calls a method of the owner). The list node follows the vtable pointer. */
class c_list_item_delegate : public s_list_node
{
public:
	virtual void invoke(s_controller_reference **controller, long *item) = 0;
};

void delegate_register(s_list_head *list, c_list_item_delegate *delegate);

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
	/* gives the widget and its children new ids */
	virtual void v1();
	virtual void v2();
	/* updates the widget and its children */
	virtual void v3();
	virtual void v4(long) {}
	virtual bool v5(s_widget_event *) { return false; }
	virtual long v6() { return 0; }
	virtual void v7(c_user_interface_widget *) {}
	virtual void v8() {}
	virtual void v9() {}
	/* passes the event up to the parent */
	virtual bool v10(s_widget_event *event);
	virtual long v11() { return 0; }
	virtual long v12() { return 0; }
	virtual void v13() {}
	virtual void v14() {}
	virtual c_user_interface_text *get_text() { return 0; }
	/* whether the widget shows and its animation has ended */
	virtual bool v16();

	/* the widgets are allocated from the user interface heap */
	static void *operator new(unsigned int size)
	{
		return user_interface_malloc(size);
	}

	/* unknown_22e27b.cpp */
	void delete_children();
	/* steps the widget's animation */
	void update(dword time);
	void set_animation(s_widget_animation *animation);
	c_user_interface_widget *find_child(long type, short index, bool recursive);
	c_screen_widget *get_screen();
	bool has_screen();
	bool is_in_window();
	bool has_valid_type();
	real get_depth();
	void get_bounds(s_widget_bounds *bounds);
	void get_real_bounds(real_rectangle2d *bounds);
	void add_child(c_user_interface_widget *widget);
	void remove_child(c_user_interface_widget *widget);
	c_user_interface_widget *find_text(short index);
	c_user_interface_widget *find_bitmap(short index);
	c_user_interface_widget *find_model(short index);
	void set_child_value6e(long type, short index, bool value);
	c_user_interface_widget *find_by_id(long id);
	void set_user_flags(word user_flags);
	c_screen_widget *find_window_screen();
	long new_widget_id();
	/* the first controller of the widget's user flags (unknown_1a2c81.cpp) */
	long get_controller_index();
	void build_animation(s_widget_animation *animation, short index, long type);
	void start_animation(long type);

	/* unknown_24c177.cpp */
	long child_count();
	c_user_interface_widget *get_child(long index);

	/* the old name of start_animation */
	void function_22e957(long type) { start_animation(type); }

	long type;
	word user_flags;
	short value0a;
	long value0c;
	c_user_interface_widget *parent;
	c_user_interface_widget *child;
	c_user_interface_widget *next;
	c_user_interface_widget *previous;
	s_widget_bounds bounds;
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

	virtual long v6();

	/* shows the string with this id from the screen's string list */
	void set_string(long string_id);

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

/* the screen widget (vtable 0x458840); the screens override slots 10, 17,
   18, 19 and 26 */
class c_screen_widget : public c_user_interface_widget
{
public:
	c_screen_widget(long screen_id, long a, long b, word user_flags);

	/* 0x2300ea: a press of B or back leaves the screen (unknown_2300cf.cpp) */
	virtual bool v10(s_widget_event *event);
	virtual bool v16();

	virtual void v17() {}
	virtual void v18(void *parameters) {}
	/* loads the bitmaps of the screen's definition */
	virtual void v19();
	/* the screen's window: its channel and index */
	virtual long v20();
	virtual long v21() { return b; }
	virtual void v22(void *window) {}
	virtual void v23(void *window) {}
	/* remembers the focused widget and the list's focused datum */
	virtual void v24(s_screen_focus *focus);
	/* focuses the list's datum or the widget the focus names */
	virtual void v25(s_screen_focus *focus);
	virtual screen_load_proc get_load_proc() { return 0; }
	virtual bool v27();

	/* unknown_2300cf.cpp */
	s_screen_pane *get_current_pane();
	s_screen_pane *get_first_pane();
	short get_first_pane_value();
	bool set_screen_id(long id);
	long get_definition_value(long block, long index);

	/* places the newly loaded screen in its window (unknown_147f6d.cpp) */
	void function_147f6d(s_screen_parameters *parameters);

	/* the delegate's method (unknown_2300cf.cpp) */
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

/* the screen with a list (vtable 0x4588c0, constructed by 0x230451) */
class c_screen_with_menu : public c_screen_widget
{
public:
	c_screen_with_menu(long screen_id, long a, long b, word user_flags, void *list);

	void *list;
};


/* the window manager disposes of a screen (not decompiled yet) */
void function_148148(c_screen_widget *screen);

/* unknown_19b516.h's c_widget is a list of this family (vtable 0x4594a0)
   with its slots rotated by 8; its v9, v10 and v11 are slots 1, 2 and 3 here.
   See the note there on why the two views are still separate. */
class c_list_widget : public c_user_interface_widget
{
public:
	c_list_widget(word user_flags);

	virtual void v17() {}
	virtual void *get_item_data() { return 0; }
	virtual long get_item_count() { return 0; }
	virtual void v20(c_user_interface_widget *, long) {}
	virtual void v21() {}

	/* unknown_24c177.cpp */
	s_list_definition *get_definition();
	long get_skin_index();
	void *get_item_animation(long index);
	c_user_interface_widget *find_item(long datum);
	c_user_interface_widget *get_focused_item();
	long get_focused_datum();
	void assign_items(long datum);
	void select_datum(long datum);
	void select_item(short item);

	s_data_array *data;
	short value74;
	short value76;
	long value78;
	bool value7c;
	bool wraps;
	bool notify_screen;
	bool value7f;
	s_list_head head80;
	s_list_head item_handlers;
};

/* the widget base of the list items (vtable 0x45c4d0) */
class c_widget_45c4d0 : public c_user_interface_widget
{
public:
	c_widget_45c4d0(long type, word user_flags);
};

/* a list's item widget (vtable 0x459f10, 0x80 bytes); every list keeps an
   array of them at +0x88 */
class c_list_item_widget : public c_widget_45c4d0
{
public:
	c_list_item_widget();

	/* a press of A or start chooses the item */
	virtual bool v10(s_widget_event *event);
	virtual bool v16();
	/* whether the item shows a datum */
	virtual bool v17();

	long value70;
	long value74;
	s_list_head head78;
	s_list_head head7c;
};

/* an item's text, chosen by the item from a table (function_24c75c) */
struct s_list_item_text
{
	short item;
	long string_id;
};

bool function_24c75c(c_list_widget *list, c_user_interface_widget *item, s_list_item_text *table, short text_index, long count);

typedef void (c_list_widget::*list_item_method)(s_controller_reference **controller, long *item);

/* the item handler a list's constructor registers (vtable 0x45bdb0; retail
   folded every list's copy of its one slot into 0x2b27f9) */
class c_list_item_handler : public c_list_item_delegate
{
public:
	c_list_item_handler(c_list_widget *owner, list_item_method method) :
		owner(owner),
		method(method)
	{
	}
	virtual void invoke(s_controller_reference **controller, long *item);

	c_list_widget *owner;
	list_item_method method;
};

/* a list's item widget (a list's children are its items) */
inline c_list_item_widget *widget_item(c_user_interface_widget *widget)
{
	return (c_list_item_widget *)widget;
}

/* a list's datum: the item it shows */
struct s_list_item_datum
{
	short salt;
	short item;
};

/* adds a datum showing this item to a list's data */
__forceinline void list_item_add(c_list_widget *list, short item)
{
	((s_list_item_datum *)list->data->data)[datum_new(list->data) & 0xffff].item = item;
}

/* creates a data array in the user interface heap */
s_data_array *user_interface_data_new(const char *name, long maximum_count, long size);

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
