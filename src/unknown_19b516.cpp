// @flags /O1 /arch:SSE /Gr
/* UNKNOWN_19B516.CPP: the methods of the widget class whose vtable is at
   0x4594e0 */

#include "cseries.h"
#include "globals.h"
#include "unknown_19b516.h"

struct s_player_row
{
	word name[0x42];
	long count;
	long value;
	long value2;
	byte unknown90[0x70];
	s_id_triplet id;
	byte unknown10c[0x114 - 0x10c];
};

struct s_name_request
{
	long type;
	s_id_triplet id;
	char name[16];
	byte unknown20[0x78 - 0x20];
};

struct s_player_ref
{
	byte unknown00[4];
	long player;
};

dword g_54d5b8;
s_player_row g_55caf0[1];
long g_51ec08;

// @retail 0x22e3b4
bool c_widget::v0()
{
	return m6d && m6e && m50 <= g_54d5b8;
}

// @retail 0x24bb60
void c_widget::v1()
{
	c_list_view *list = (c_list_view *)this;
	void *item = list->get_first();
	long count = list->get_count();
	word index = m8;
	while (count > 0)
	{
		item = (byte *)function_22eb18(function_22ee92(item, index)) + 0x80;
		count--;
	}
}

// @retail 0x19b516
void *c_widget::v2()
{
	return sub88;
}

// @retail 0x232d3f
long c_widget::v3()
{
	return 0x10;
}

// @retail 0x233ea9
void c_widget::v4(s_event *event, long unused)
{
	c_widget *parent_widget = parent;
	c_widget *root = parent_widget->parent->parent;
	if (parent_widget && parent_widget == (c_widget *)((long *)root)[0x680 / 4])
	{
		long index = event->index & 0xffff;
		if (index >= 0 && index < m8a4)
		{
			v22(event, index);
			if (!m8a8)
			{
				if (((byte *)root)[0x5ef0])
				{
					event->function_251963();
				}
				else
				{
					function_233f0f(index, this);
				}
			}
		}
	}
}

// @retail 0x233e97
bool c_widget::v5(s_event *event)
{
	bool result = false;
	if (m8a0 == result)
	{
		result = function_24c3f8(event);
	}
	return result;
}

// @retail 0x234a97
void c_widget::v6(c_widget *window, long row)
{
	word text[256];
	word percent_text[256];

	text[0] = 0;
	c_text_widget *header = (c_text_widget *)window->function_22edb8(10, 0, 0);
	c_text_widget *name = ((c_widget *)header)->function_22edb8(6, 0, 0);
	c_text_widget *score = ((c_widget *)header)->function_22edb8(6, 1, 0);
	c_text_widget *total = ((c_widget *)header)->function_22edb8(6, 2, 0);
	c_text_widget *percent = ((c_widget *)header)->function_22edb8(6, 3, 0);
	c_text_widget *extra = ((c_widget *)header)->function_22edb8(6, 4, 0);
	function_233f0f(row, window);
	if (name)
	{
		name->get_text()->set_text((word *)&g_55caf0[row]);
	}
	if (score)
	{
		function_1630e0(text, (const word *)L"%d", g_55caf0[row].value);
		score->get_text()->set_text(text);
	}
	if (total)
	{
		function_1630e0(text, (const word *)L"%d", g_55caf0[row].count);
		total->get_text()->set_text(text);
	}
	if (percent)
	{
		long percentage;

		percent_text[0] = 0;
		percentage = 0;
		if (g_55caf0[row].count > 0)
		{
			percentage = (long)((real)g_55caf0[row].value * 100.0f / (real)g_55caf0[row].count);
		}
		function_22eeee()->function_230134(0x11000759, text);
		function_1630e0(percent_text, text, percentage);
		percent->get_text()->set_text(percent_text);
	}
	if (extra)
	{
		function_1630e0(text, (const word *)L"%d", g_55caf0[row].value2);
		extra->get_text()->set_text(text);
	}
}

// @retail 0x234509 deleting c_widget

// @retail 0x24bb9c
void c_widget::v9()
{
	function_22e315();
	function_24c0c4(this);
	if (!function_22ed7a() && child)
	{
		v7(child);
		function_24c610(child, this);
	}
}

// @retail 0x24bbd0
void c_widget::v10()
{
	if (m7f)
	{
		s_data_array *p = m70;
		if (p)
		{
			data_dispose(p);
			m70 = 0;
		}
	}
	function_22e335();
}

// @retail 0x24bd61
void c_widget::v11()
{
	if (function_24c63e(this))
	{
		word value = m74;
		if (value)
		{
			m74 = value - 1;
		}
	}
	if (function_24c676(this))
	{
		word value = m76;
		if (value)
		{
			m76 = value - 1;
		}
	}
	function_22e391();
}

// @retail 0x22e930
void c_widget::v13(s_event *event)
{
	function_22e89c(event);
	for (c_widget *widget = child; widget; widget = widget->next)
	{
		widget->v5(event);
	}
}

// @retail 0x230130
long c_widget::v14()
{
	return m78;
}

// @retail 0x22eb8c
void c_widget::v15(c_widget *widget)
{
	if (widget)
	{
		widget->function_22ecb4(true);
		for (c_widget *item = widget->child; item; item = item->next)
		{
			if (item->function_22e37f())
			{
				widget->v7(item);
				break;
			}
		}
	}
}

// @retail 0x22ebc3
void c_widget::v16()
{
	c_widget *widget = next;
	if (!widget)
	{
		c_widget *p = prev;
		widget = this;
		while (p)
		{
			widget = p;
			p = widget->prev;
		}
	}
	while (widget)
	{
		if (widget->function_22e37f())
		{
			function_22ecb4(false);
			widget->function_22ecb4(true);
			return;
		}
		c_widget *candidate = widget->next;
		if (!candidate)
		{
			c_widget *p = widget->prev;
			candidate = widget;
			while (p)
			{
				candidate = p;
				p = candidate->prev;
			}
		}
		widget = candidate;
	}
}

// @retail 0x22ec1b
void c_widget::v17()
{
	c_widget *widget = prev;
	if (!widget)
	{
		c_widget *p = next;
		widget = this;
		while (p)
		{
			widget = p;
			p = widget->next;
		}
	}
	while (widget)
	{
		if (widget->function_22e37f())
		{
			function_22ecb4(false);
			widget->function_22ecb4(true);
			return;
		}
		c_widget *candidate = widget->prev;
		if (!candidate)
		{
			c_widget *p = widget->next;
			candidate = widget;
			while (p)
			{
				candidate = p;
				p = candidate->next;
			}
		}
		widget = candidate;
	}
}

// @retail 0x24c024
bool c_widget::v18(s_event *event)
{
	switch (event->type)
	{
	case 1:
		function_24c1c5(this, -1);
		return true;
	case 3:
		function_24c1c5(this, 1);
		return true;
	case 5:
		if (event->param == 0 || event->param == 12)
		{
			if (!function_22ef1b())
			{
				c_widget *item = function_24bae6(this);
				if (item)
				{
					long key = *(long *)((byte *)item + 4);
					key = *(long *)((byte *)item + 0x70);
					function_24c7e4(sub84, &event, &key);
				}
			}
			return true;
		}
		break;
	}
	return function_22ec73(event);
}

// @retail 0x22ec90
long c_widget::v19()
{
	return function_22eeee()->v20();
}

// @retail 0x22ec9e
long c_widget::v20()
{
	c_widget *widget = function_22eeee();
	if (!widget)
	{
		return 4;
	}
	return widget->v21();
}

// @retail 0x24c09d
long c_widget::v21()
{
	v7(child);
	return function_24c610(child, this);
}

// @retail 0x24bda2
void c_widget::v12(long a)
{
	short x;
	short y;
	s_bounds bounds;

	function_2363d4(a, &x, &y);
	if (function_22f0ff(this))
	{
		return;
	}
	s_sprite_placement *placement = function_14837a(function_24c0b3(this));
	if (!placement)
	{
		return;
	}
	long tag_index = placement->tag_index;
	s_sprite_tag *tag = (s_sprite_tag *)g_4e3b44[tag_index & 0xffff].flags;
	real scale = function_22e9aa(this);
	function_22e9c6(&bounds.a);
	if (tag_index == NONE)
	{
		return;
	}
	if (function_24c63e(this))
	{
		real opacity = 1.0f;
		if (m74 == 0)
		{
			opacity = 0.5f;
		}
		s_sprite_element *element = &tag->sprite->main;
		if (tag->sprite)
		{
			s_float_rect from;
			s_float_rect to;
			real width = (real)element->width;
			real height = (real)element->height;
			real px = (real)(placement->main_x + bounds.b);
			real py = (real)(placement->main_y + bounds.c);

			from.x0 = 0.0f;
			from.x1 = width;
			from.y0 = 0.0f;
			from.y1 = height;
			to.x0 = (real)y + px;
			to.x1 = (real)y + (width + px);
			to.y0 = (real)x + py;
			to.y1 = (real)x + (py - height);
			function_23618e(&to, scale, a);
			opacity *= 255.0f;
			long alpha;
			__asm
			{
				fld opacity
				fistp alpha
			}
			function_235e5e(element, &from, &to, (alpha << 24) | 0xffffff, 0, 0);
		}
	}
	if (function_24c676(this))
	{
		real opacity = 1.0f;
		if (m76 == 0)
		{
			opacity = 0.5f;
		}
		s_sprite_element *element = &tag->sprite->second;
		if (element)
		{
			s_float_rect from;
			s_float_rect to;
			real width = (real)element->width;
			real height = (real)element->height;
			real px = (real)(placement->second_x + bounds.b);
			real py = (real)(placement->second_y + bounds.c);

			from.x0 = 0.0f;
			from.x1 = width;
			from.y0 = 0.0f;
			from.y1 = height;
			to.x0 = (real)y + px;
			to.x1 = (real)y + (width + px);
			to.y0 = (real)x + py;
			to.y1 = (real)x + (py - height);
			function_23618e(&to, scale, a);
			opacity *= 255.0f;
			long alpha;
			__asm
			{
				fld opacity
				fistp alpha
			}
			function_235e5e(element, &from, &to, (alpha << 24) | 0xffffff, 0, 0);
		}
	}
}

// @retail 0x23403c
void c_widget_handler::v24(long **a, long *b)
{
	long handle = *b;
	long index = handle & 0xffff;
	s_player_row *row = &g_55caf0[index];

	{
		dword local[2];

		function_18ff47(((s_player_ref *)*a)->player, local);
		if (m8a0 || handle == NONE || !function_6c7e0() || function_199994() || function_1999b3() || function_1900a5(((s_player_ref *)*a)->player))
		{
			return;
		}
		if (row->id.function_63d00(local, 0))
		{
			return;
		}
	}
	if (index >= 0 && index < g_51ec08 && !(row->id.c & 3))
	{
		s_name_request request = { 0 };
		s_message message;

		message.field_c = 0;
		request.type = 1;
		request.id = row->id;
		unicode_string_to_ascii((const word *)row, request.name, 16);
		request.name[15] = 0;
		function_148893(&request, 1);
		if (request.id.a | request.id.b)
		{
			function_149f49(&message, 0, 0, 1 << ((s_player_ref *)*a)->player, 3, 4, 0x2b7223);
			message.callback(&message);
		}
	}
}
