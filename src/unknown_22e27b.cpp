// @flags /O1 /Oi /arch:SSE /Gr
/* UNKNOWN_22E27B.CPP: the user interface widget base class (vtable 0x458788)
   and the intrusive lists the widgets keep their delegates in */

#include "cseries.h"
#include <string.h>
#include <wchar.h>
#include "screen_widgets.h"

extern dword g_54d5b8;

struct s_name_buffer;
void function_08cc20(s_name_buffer *buffer, const wchar_t *name);

/* the bounds every screen starts with */
struct s_screen_bounds
{
	short bounds[4];
};

s_screen_bounds g_485a92;

// @retail 0x22e27b
c_user_interface_widget::c_user_interface_widget(long type, word user_flags)
{
	s_widget_animation animation;

	value0a = NONE;
	value0c = NONE;
	this->type = type;
	this->user_flags = user_flags;
	parent = 0;
	child = 0;
	next = 0;
	previous = 0;
	memset(bounds, 0, sizeof(bounds));
	memset(&color, 0, sizeof(color));
	value68 = NONE;
	value6a = 0;
	m6c = false;
	value6d = true;
	value6e = true;
	memset(bounds, 0, sizeof(bounds));
	color.red = 1.0f;
	color.blue = 1.0f;
	color.green = 1.0f;

	memset(&animation, 0, sizeof(animation));
	animation.type = NONE;
	animation.direction = 1;
	animation.value14 = 0;
	set_animation(&animation);
}

// @retail 0x2bac9a deleting
c_user_interface_widget::~c_user_interface_widget()
{
	delete_children();
}

// @retail 0x22e34b
void c_user_interface_widget::delete_children()
{
	c_user_interface_widget *widget = child;

	child = 0;
	while (widget)
	{
		c_user_interface_widget *next_widget = widget->next;

		widget->delete_children();
		if (widget->m6c)
		{
			widget->~c_user_interface_widget();
			user_interface_free(widget);
		}
		widget = next_widget;
	}
}

// @retail 0x22e89c
void c_user_interface_widget::set_animation(s_widget_animation *definition)
{
	long time = g_54d5b8;
	long direction;

	animation.type = definition->type;
	animation.target = definition->target;
	animation.value8 = definition->value8;
	animation.valuea = definition->valuea;
	if (definition->direction)
	{
		direction = definition->direction >= 0 ? 1 : -1;
	}
	else
	{
		direction = 0;
	}
	animation.direction = direction >= 0 ? 1 : -1;
	animation.valuee = definition->valuee;
	animation.duration = definition->duration;
	animation.value14 = definition->value14;
	animation.start_time = time;
	animation.end_time = definition->duration + time;
	animation.value20 = definition->value20;
	animation.value2c = 0.0f;
	animation.value28 = 0.0f;
	animation.progress = 0.0f;
	animation.scale = 1.0f;
}

// @retail 0x22edb8
c_user_interface_widget *c_user_interface_widget::find_child(long type, short index, bool recursive)
{
	c_user_interface_widget *result = 0;
	c_user_interface_widget *widget;
	short count = index;

	if (count >= 0)
	{
		for (widget = child; widget; widget = widget->next)
		{
			if (widget->type == type && count-- == 0)
			{
				return widget;
			}
		}
	}
	if (recursive)
	{
		for (widget = child; widget; widget = widget->next)
		{
			result = widget->find_child(type, index, recursive);
			if (result)
			{
				break;
			}
		}
	}
	return result;
}

// @retail 0x22f092
void list_node_detach(s_list_node *node)
{
	if (node->list)
	{
		list_remove(node->list, node);
		node->list = 0;
	}
}

// @retail 0x22f0a5
void list_remove_all(s_list_head *list)
{
	while (list->first)
	{
		list_remove(list, list->first);
	}
}

// @retail 0x22f0b7
void list_append(s_list_head *list, s_list_node *node)
{
	node->list = list;
	if (!list->first)
	{
		list->first = node;
	}
	else
	{
		s_list_node *last = list->first;
		while (last->next)
		{
			last = last->next;
		}
		last->next = node;
		node->previous = last;
	}
}

// @retail 0x22f0d2
void list_remove(s_list_head *list, s_list_node *node)
{
	s_list_node *next = node->next;

	if (node->previous)
	{
		node->previous->next = node->next;
	}
	if (node->next)
	{
		node->next->previous = node->previous;
	}
	node->next = 0;
	node->previous = 0;
	node->list = 0;
	if (list->first == node)
	{
		list->first = next;
	}
}

// @retail 0x22f4cd
void delegate_register(s_list_head *list, c_list_item_delegate *delegate)
{
	list_append(list, delegate);
}

// @retail 0x22cc8e
c_user_interface_text::c_user_interface_text()
{
	value04 = 0;
	memset(&color, 0, sizeof(color));
	value18 = NONE;
	value24 = NONE;
	cursor = NONE;
	value14 = 0;
	value38 = 0;
	length = 0;
	value40 = 0;
	value16 = 1;
	value1c = 2;
	value20 = 1.0f;
	color.blue = 1.0f;
	color.green = 1.0f;
	color.red = 1.0f;
}

// @retail 0x22f4db
void c_user_interface_text::update_length()
{
	length = (short)wcslen(get_text());
	if (cursor >= 0)
	{
		cursor = 0;
	}
}

// @retail 0x22f52e
word *c_user_interface_text_buffer::get_text()
{
	return text;
}

// @retail 0x22f532
c_user_interface_text_buffer::c_user_interface_text_buffer()
{
	text[0] = 0;
}

// @retail 0x22f545
void c_user_interface_text_buffer::set_text(word *string)
{
	function_08cc20((s_name_buffer *)text, string);
	update_length();
}

// @retail 0x253746
c_text_widget_45a5e0::c_text_widget_45a5e0(word user_flags) :
	c_user_interface_widget(6, user_flags)
{
	value70 = 0;
}

// @retail 0x22f57f
c_user_interface_text *c_text_widget_458940::get_text()
{
	return &text;
}

// @retail 0x22f583
c_text_widget_458940::c_text_widget_458940(word user_flags) :
	c_text_widget_45a5e0(user_flags)
{
}

// @retail 0x22f5a1 deleting c_text_widget_458940

// @retail 0x22f5ca
c_screen_widget::c_screen_widget(long screen_id, long a, long b, word user_flags) :
	c_user_interface_widget(0, user_flags),
	screen_id(screen_id),
	a(a),
	b(b),
	next_widget_id(NONE),
	title(0),
	subtitle(0),
	value5f0(NONE),
	value5f2(false),
	value5f3(0),
	value5f4(false),
	delegate(this, &c_screen_widget::function_230427)
{
	value0c = ++next_widget_id;
	type = 0;
	value6d = true;
	*(s_screen_bounds *)bounds = g_485a92;
}

// @retail 0x2c883c deleting c_screen_widget

// @retail 0x24bb00
c_list_widget::c_list_widget(word user_flags) :
	c_user_interface_widget(1, user_flags),
	data(0),
	value74(0),
	value76(0),
	value78(0),
	value7c(false),
	value7d(false),
	value7e(false),
	value7f(true)
{
}

// @retail 0x24bb44 deleting c_list_widget

// @retail 0x2bac82
c_widget_45c4d0::c_widget_45c4d0(long type, word user_flags) :
	c_user_interface_widget(type, user_flags)
{
}

// @retail 0x24abae
c_list_item_widget::c_list_item_widget() :
	c_widget_45c4d0(2, 0),
	value70(NONE),
	value74(0)
{
	type = 2;
	value6d = true;
}
