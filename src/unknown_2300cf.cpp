// @flags /O1 /arch:SSE /Gr
/* UNKNOWN_2300CF.CPP: the screen widget's own methods and the screen with a
   list (the vtables at 0x458840 and 0x4588c0); the widget and screen
   constructors they build on (0x22e27b, 0x22f5ca) are in unknown_22e27b.cpp */

#include "cseries.h"
#include "screen_widgets.h"
#include "unknown_19b516.h"
#include "globals.h"

long function_1480ff(long screen_id);
void unicode_string_list_get_string(long tag_index, long string_id, word *buffer);
void function_22fba9(c_screen_widget *screen);
void __stdcall function_22fc08(c_screen_widget *screen);

/* the screen's definition tag */
// @retail 0x22f871
s_screen_definition *function_22f871(c_screen_widget *screen)
{
	s_screen_definition *definition = 0;
	long tag_index = function_1480ff(screen->screen_id);

	if (tag_index != NONE)
	{
		definition = (s_screen_definition *)g_4e3b44[tag_index & 0xffff].bytes;
	}
	return definition;
}

/* the pane the screen shows */
// @retail 0x22f899
s_screen_pane *c_screen_widget::get_current_pane()
{
	short *pane_index = &value5f0;
	s_screen_pane *result = 0;

	if (*pane_index != NONE)
	{
		s_screen_definition *definition = function_22f871(this);
		if (definition && *pane_index < definition->pane_count)
		{
			result = &definition->panes[*pane_index];
		}
	}
	return result;
}

// @retail 0x22f8c7
s_screen_pane *c_screen_widget::get_first_pane()
{
	s_screen_pane *result = 0;
	s_screen_definition *definition = function_22f871(this);

	if (definition && definition->pane_count > 0)
	{
		result = definition->panes;
	}
	return result;
}

/* a screen in a group (type 5) shows only while it is the group's current one */
struct s_screen_group_view
{
	byte unknown00[0x70];
	c_screen_widget *current;
};

// @retail 0x22f6ab
bool c_screen_widget::v16()
{
	bool result = c_user_interface_widget::v16();

	if (result && parent && parent->type == 5)
	{
		result = this == ((s_screen_group_view *)parent)->current;
	}
	return result;
}

void function_23625d(long tag_index);

// @retail 0x22f672
void c_screen_widget::v19()
{
	value5f2 = true;
	s_screen_definition *definition = function_22f871(this);
	if (definition)
	{
		for (long i = 0; i < definition->bitmap_count; i++)
		{
			s_tag_reference *bitmap = &definition->bitmaps[i];
			if (bitmap->tag_index != NONE)
			{
				function_23625d(bitmap->tag_index);
			}
		}
	}
}

// @retail 0x1473c9
bool c_screen_widget::v27()
{
	return value5f4;
}

// @retail 0x2300cf
short c_screen_widget::get_first_pane_value()
{
	s_screen_definition *definition = function_22f871(this);
	short result = 0;

	if (definition && definition->pane_count > 0)
	{
		result = definition->panes->value02;
	}
	return result;
}

// @retail 0x23012c
long c_screen_widget::v20()
{
	return a;
}

bool function_1480ed(long screen_id);

/* switches to another screen definition until the bitmaps are loaded */
// @retail 0x230154
bool c_screen_widget::set_screen_id(long id)
{
	bool result = false;

	if (!value5f2 && function_1480ed(id))
	{
		screen_id = id;
		result = true;
	}
	return result;
}

// @retail 0x230195
long function_230195(s_screen_definition *definition, long block_index, long index)
{
	long result = 0;

	if (block_index >= 0 && block_index < definition->value_block_count)
	{
		s_screen_value_block *block = &definition->value_blocks[block_index];
		if (block && index >= 0 && index < block->count)
		{
			long *value = &block->values[index];
			if (value)
			{
				result = *value;
			}
		}
	}
	return result;
}

// @retail 0x230172
long c_screen_widget::get_definition_value(long block_index, long index)
{
	long result = 0;
	s_screen_definition *definition = function_22f871(this);

	if (definition)
	{
		result = function_230195(definition, block_index, index);
	}
	return result;
}

// @retail 0x2301c3
void c_screen_widget::v24(s_screen_focus *focus)
{
	c_screen_widget *screen = find_window_screen();
	long datum = NONE;
	c_user_interface_widget *widget = child;
	c_user_interface_widget *next = widget ? widget->next : 0;

	while (widget && datum == NONE)
	{
		switch (widget->type)
		{
		case 1:
			datum = ((c_list_widget *)widget)->get_focused_datum();
			break;
		}
		widget = next;
		next = next ? next->next : 0;
	}
	focus->unknown00 = 0;
	focus->widget_id = screen ? screen->value0c : NONE;
	focus->datum = datum;
}

// @retail 0x23021e
void c_screen_widget::v25(s_screen_focus *focus)
{
	if (focus->datum != NONE)
	{
		c_user_interface_widget *widget;

		for (widget = child; widget; widget = widget->next)
		{
			if (widget->type == 1)
			{
				break;
			}
		}
		((c_list_widget *)widget)->select_datum(focus->datum);
	}
	else if (focus->widget_id != NONE)
	{
		v7(find_by_id(focus->widget_id));
	}
}

// @retail 0x2300ea
bool c_screen_widget::v10(s_widget_event *event)
{
	if (event->type == 5)
	{
		switch (event->param)
		{
		case 1:
		case 13:
			function_14800c(v20(), v21());
			return true;
		}
	}
	return c_user_interface_widget::v10(event);
}

// @retail 0x230134
void c_widget::function_230134(long string_id, word *buffer)
{
	buffer[0] = 0;
	if (string_id != NONE)
	{
		s_screen_definition *definition = function_22f871((c_screen_widget *)this);
		if (definition)
			unicode_string_list_get_string(definition->string_list_index, string_id, buffer);
	}
}

// @retail 0x230427
void c_screen_widget::function_230427(short *delta)
{
	short value = value5f3 + *delta;
	function_22fba9(this);
	value5f0 = value;
	function_22fc08(this);
}

// @retail 0x230451
c_screen_with_menu::c_screen_with_menu(long screen_id, long a, long b, word user_flags, void *list) :
	c_screen_widget(screen_id, a, b, user_flags),
	list(list)
{
}
