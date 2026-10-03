// @flags /O1 /Oi /arch:SSE /Gr
/* UNKNOWN_2AFD97.CPP: the widgets a screen's definition describes besides
   its texts: bitmaps, models, and groups of widgets laid out on a grid */

#include "cseries.h"
#include <string.h>
#include <stdlib.h>
#include "globals.h"
#include "screen_widgets.h"

void function_23625d(long tag_index);
c_user_interface_widget *function_22fa30(c_user_interface_widget *parent, s_bitmap_block *definition);
void function_253765(c_text_widget_45a5e0 *widget, short index, s_text_block const *definition);

c_widget_45ad18 *function_2baf5d(c_user_interface_widget *parent, s_widget_point *point, long index, s_widget_block_18 *definition);

/* the views of 0x2b0a14..0x2b0b19 (unknown_2b116a.cpp) */
struct s_widget_view_2b0a;
long function_2b0a68(s_widget_view_2b0a *widget);
bool function_2b0a48(s_widget_view_2b0a *widget);
bool function_2b12ba(s_widget_view_2b0a *widget);

extern dword g_54d5b8;

#define PIN(value, minimum, maximum) ((value) < (minimum) ? (minimum) : ((value) > (maximum) ? (maximum) : (value)))

struct bitmap_data
{
	byte unknown00[4];
	short width;
	short height;
	byte unknown08[0x74 - 8];
};

struct s_bitmap_group_view
{
	byte unknown00[0x44];
	long bitmap_count;
	bitmap_data *bitmaps;
};

// @retail 0x2b01eb
c_bitmap_widget::c_bitmap_widget(s_bitmap_block *definition) :
	c_user_interface_widget(8, 0),
	definition(definition),
	start_time(g_54d5b8),
	value78(0),
	value7c(0.0f),
	value80(0.0f),
	value84(0.0f),
	sequence(0),
	bitmap(0)
{
	bitmap_data *shown = 0;
	s_bitmap_group_view *group = 0;
	s_widget_bounds bounds;

	value68 = this->definition->value04 - 1;
	function_23625d(this->definition->tag_index);
	if (this->definition->tag_index != NONE)
	{
		group = (s_bitmap_group_view *)g_4e3b44[this->definition->tag_index & 0xffff].bytes;
		if (group)
		{
			shown = &group->bitmaps[sequence];
		}
	}
	memset(&bounds, 0, sizeof(bounds));
	if (function_2b0a68((s_widget_view_2b0a *)this))
	{
		bounds.right = this->definition->x;
		bounds.left = this->definition->x;
		bounds.top = this->definition->y;
		bounds.bottom = this->definition->height;
	}
	else if (shown)
	{
		bounds.top = this->definition->y;
		bounds.left = this->definition->x;
		bounds.bottom = this->definition->y - shown->height;
		bounds.right = this->definition->x + shown->width;
	}
	sequence = 0;
	this->bounds = bounds;
	if (group && this->definition->sequence >= 0 && this->definition->sequence < group->bitmap_count)
	{
		sequence = this->definition->sequence;
	}
	value6a = definition->value20;
}

// @retail 0x2b0b5e
c_model_widget::c_model_widget(s_model_block *definition) :
	c_user_interface_widget(7, 0),
	definition(definition)
{
	value68 = definition->value04 - 1;
	bounds = definition->bounds;
	value6a = definition->value08;
}

// @retail 0x2b1111
c_widget_45adf0::c_widget_45adf0(s_widget_block_24 *definition) :
	c_user_interface_widget(9, 0),
	value84(NONE)
{
	this->definition = definition;
	memset(value74, 0, sizeof(value74));
	bounds = this->definition->bounds;
	value68 = definition->value04;
	value6a = definition->value08;
	if (this->definition->tag_index != NONE)
	{
		function_23625d(this->definition->tag_index);
	}
}

// @retail 0x2afd97
c_widget_45ad18::c_widget_45ad18(long index, s_widget_block_18 *definition) :
	c_widget_45c4d0(10, 0),
	index(index),
	definition(definition)
{
}

/* the bounds of the widget's children that show */
// @retail 0x2bafa4
void function_2bafa4(c_user_interface_widget *widget, s_widget_bounds *bounds)
{
	c_user_interface_widget *child;

	bounds->left = 0x7fff;
	bounds->right = -0x8000;
	bounds->top = -0x8000;
	bounds->bottom = 0x7fff;
	for (child = widget->child; child; child = child->next)
	{
		bool hidden;

		if (child->type == 8)
		{
			hidden = function_2b0a48((s_widget_view_2b0a *)child);
		}
		else if (child->type == 9)
		{
			hidden = function_2b12ba((s_widget_view_2b0a *)child);
		}
		else
		{
			hidden = false;
		}
		if (!hidden)
		{
			s_widget_bounds child_bounds;

			child->get_bounds(&child_bounds);
			if (child_bounds.left < bounds->left)
			{
				bounds->left = child_bounds.left;
			}
			if (child_bounds.right > bounds->right)
			{
				bounds->right = child_bounds.right;
			}
			if (child_bounds.top > bounds->top)
			{
				bounds->top = child_bounds.top;
			}
			if (child_bounds.bottom < bounds->bottom)
			{
				bounds->bottom = child_bounds.bottom;
			}
		}
	}
	if ((short)(bounds->right - bounds->left) < 0 || (short)(bounds->top - bounds->bottom) < 0)
	{
		memset(bounds, 0, 4);
	}
}

#if 0
/* a text of a group (needs the text widgets' constructors for a controller,
   0x2bac52 and 0x2bac6a: see unknown_22e27b.cpp) */
// (retail 0x2baeb1, once written)
c_text_widget_45a5e0 *function_2baeb1(c_user_interface_widget *parent, long index, s_text_block *definition)
{
	c_text_widget_45a5e0 *text;

	if (definition->flags & 0x10)
	{
		text = new c_text_widget_32(parent->get_controller_index());
	}
	else
	{
		text = new c_text_widget_458940(parent->get_controller_index());
	}
	if (text)
	{
		text->m6c = true;
		parent->add_child(text);
		function_253765(text, (short)index, definition);
		text->get_text()->set_text((word *)L"");
	}
	return text;
}
#endif
c_text_widget_45a5e0 *__stdcall function_2baeb1(c_user_interface_widget *parent, long index, s_text_block *definition);

// @retail 0x2baf38
c_widget_45adf0 *function_2baf38(c_user_interface_widget *parent, s_widget_block_24 *definition)
{
	c_widget_45adf0 *widget = new c_widget_45adf0(definition);

	if (widget)
	{
		widget->m6c = true;
		parent->add_child(widget);
	}
	return widget;
}

/* places a group's widgets at the point */
// @retail 0x2bacbc
void function_2bacbc(c_user_interface_widget *widget, s_widget_group_definition *group, s_widget_point *point)
{
	s_widget_bounds bounds;
	long i;

	for (i = 0; i < group->bitmap_count; i++)
	{
		c_user_interface_widget *bitmap = function_22fa30(widget, &group->bitmaps[i]);

		if (bitmap)
		{
			short height;

			bitmap->get_bounds(&bounds);
			height = (short)abs(bounds.bottom - bounds.top);
			bounds.top += height;
			bounds.bottom += height;
			bounds.top += point->y;
			bounds.left += point->x;
			bounds.right += point->x;
			bounds.bottom += point->y;
			bitmap->bounds = bounds;
			bitmap->value0a = (short)i;
		}
	}
	for (i = 0; i < group->block_24_count; i++)
	{
		c_user_interface_widget *child = function_2baf38(widget, &group->blocks_24[i]);

		if (child)
		{
			short height;

			child->get_bounds(&bounds);
			height = (short)abs(bounds.bottom - bounds.top);
			bounds.top += height;
			bounds.bottom += height;
			bounds.top += point->y;
			bounds.left += point->x;
			bounds.right += point->x;
			bounds.bottom += point->y;
			child->bounds = bounds;
			child->value0a = (short)i;
		}
	}
	for (i = 0; i < group->block_18_count; i++)
	{
		s_widget_block_18 *block = &group->blocks_18[i];
		long j;

		for (j = 0; j < block->count; j++)
		{
			c_user_interface_widget *child = function_2baf5d(widget, point, j, block);

			if (child)
			{
				s_widget_bounds child_bounds;
				short height;

				child->get_bounds(&child_bounds);
				height = (short)abs(child_bounds.bottom - child_bounds.top);
				child_bounds.top += height;
				child_bounds.bottom += height;
				child->value0a = (short)i;
			}
		}
	}
	for (i = 0; i < group->text_count; i++)
	{
		c_text_widget_45a5e0 *text = function_2baeb1(widget, i, &group->texts[i]);

		if (text)
		{
			text->get_bounds(&bounds);
			bounds.top += point->y;
			bounds.left += point->x;
			bounds.right += point->x;
			bounds.bottom += point->y;
			text->bounds = bounds;
		}
	}
}

/* places the widget's group in its cell of the grid */
// @retail 0x2afdbc
void c_widget_45ad18::place(s_widget_point *origin)
{
	if (definition->tag_index != NONE)
	{
		s_widget_group_definition *group = (s_widget_group_definition *)g_4e3b44[definition->tag_index & 0xffff].bytes;
		long rows = definition->rows > 1 ? definition->rows : 1;
		long columns = definition->columns > 1 ? definition->columns : 1;
		long column;
		long row;

		if (definition->order != 0)
		{
			column = index / rows;
			row = index % rows;
		}
		else
		{
			column = index % columns;
			row = index / columns;
		}
		if (PIN(column, 0, columns - 1) == column && PIN(row, 0, rows - 1) == row)
		{
			s_widget_point point;
			s_widget_bounds group_bounds;

			point.x = definition->x;
			point.y = definition->y;
			point.x += definition->x_step * (short)column + origin->x;
			point.y += definition->y_step * (short)row + origin->y;
			function_2bacbc(this, group, &point);
			function_2bafa4(this, &group_bounds);
			bounds = group_bounds;
		}
	}
}

// @retail 0x2baf5d
c_widget_45ad18 *function_2baf5d(c_user_interface_widget *parent, s_widget_point *point, long index, s_widget_block_18 *definition)
{
	c_widget_45ad18 *widget = new c_widget_45ad18(index, definition);

	if (widget)
	{
		widget->m6c = true;
		parent->add_child(widget);
		widget->place(point);
	}
	return widget;
}
