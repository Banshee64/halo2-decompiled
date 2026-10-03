// @flags /O1 /Oi /arch:SSE /Gr
/* UNKNOWN_22E27B.CPP: the user interface widget base class (vtable 0x458788)
   and the intrusive lists the widgets keep their delegates in */

#include "cseries.h"
#include <string.h>
#include "screen_widgets.h"

extern dword g_54d5b8;

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
void list_remove_all(s_list_node **list)
{
	while (*list)
	{
		list_remove(list, *list);
	}
}

// @retail 0x22f0b7
void list_append(s_list_node **list, s_list_node *node)
{
	node->list = list;
	if (!*list)
	{
		*list = node;
	}
	else
	{
		s_list_node *last = *list;
		while (last->next)
		{
			last = last->next;
		}
		last->next = node;
		node->previous = last;
	}
}

// @retail 0x22f0d2
void list_remove(s_list_node **list, s_list_node *node)
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
	if (*list == node)
	{
		*list = next;
	}
}

// @retail 0x22f4cd
void delegate_register(s_list_node **list, c_list_item_delegate *delegate)
{
	list_append(list, delegate);
}
