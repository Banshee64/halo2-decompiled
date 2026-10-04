// @flags /O1 /arch:SSE /Gr
/* UNKNOWN_253C8B.CPP: the button widget (type 3, vtable 0x45a628), and the
   texts and buttons a screen's definition describes */

#include "cseries.h"
#include "screen_widgets.h"
#include "unknown_19b516.h"
#include "unknown_234c64.h"
#include "globals.h"

bool function_13ee20(word const *text, long font);

/* a bitmap tag: its bitmaps */
struct s_bitmap_group_view_2541
{
	byte unknown00[0x44];
	long bitmap_count;
};
void function_1496f6(long type, word *buffer);

// @retail 0x253c8b
c_button_widget::c_button_widget(short valuef8, word user_flags) :
	c_user_interface_widget(3, user_flags),
	valuef4(0),
	valuef8(valuef8)
{
}

// @retail 0x237623 deleting c_button_widget
// @retail 0x19b8b1 destructor c_button_widget

// @retail 0x237607
c_user_interface_text *c_button_widget::get_text()
{
	return &text;
}

/* ---- the texts a screen's definition describes ---- */

/* shows a text of the definition in the widget */
// @retail 0x253765
void function_253765(c_text_widget_45a5e0 *widget, short index, s_text_block const *definition)
{
	c_screen_widget *screen = widget->get_screen();
	word buffer[0x100];
	long value14 = 0;
	long justification;
	long font = 1;

	widget->value0a = index;
	widget->value70 = definition->value06;
	buffer[0] = 0;
	if (definition->flags & 1)
	{
		justification = 0;
	}
	else
	{
		justification = (definition->flags & 2) ? 1 : 2;
	}
	if (definition->flags & 4)
	{
		value14 = 1;
	}
	widget->value68 = definition->value04 - 1;
	widget->value6a = definition->value28;
	if (!(definition->flags & 8))
	{
		font = definition->font;
	}
	real_rgb_color color = definition->color;
	((c_widget *)screen)->function_230134(definition->string_id, buffer);
	widget->bounds = definition->bounds;
	widget->get_text()->setup(buffer, font, &color, (short)value14, NONE, justification, NONE);
	if (definition->flags & 8)
	{
		s_user_interface_globals *globals = function_148350();

		widget->get_text()->value14 = widget->get_text()->value14 | 4;
		if (globals)
		{
			real value = globals->value4c;

			widget->get_text()->value20 = value;
		}
		widget->get_text()->cursor = 0;
	}
	function_13ee20(widget->get_text()->get_text(), font);
}

/* shows a text with these bounds, colour and font in the widget */
// @retail 0x2538a6
void function_2538a6(c_text_widget_45a5e0 *widget, short value04, short font, s_widget_bounds const *bounds, real_argb_color const *color, short flags)
{
	s_text_block definition;

	definition.value06 = 0;
	definition.flags = flags;
	definition.value04 = value04;
	*(real_argb_color *)&definition.alpha = *color;
	definition.string_id = NONE;
	definition.font = font;
	definition.bounds = *bounds;
	function_253765(widget, NONE, &definition);
}

/* the subtitle shows the user's gamertag */
// @retail 0x253bc9
void function_253bc9(c_text_widget_458940 *widget, long subtitle_type)
{
	if (function_148350())
	{
		word buffer[0x100];

		buffer[0] = 0;
		function_1496f6(subtitle_type, buffer);
		widget->get_text()->set_text(buffer);
	}
}

/* the button's definition in its screen's first pane */
// @retail 0x253cc8
s_button_block *function_253cc8(c_button_widget *button)
{
	s_button_block *result = 0;
	c_screen_widget *screen = button->get_screen();

	if (screen)
	{
		s_screen_pane *pane = screen->get_first_pane();

		if (pane && button->valuef8 >= 0 && button->valuef8 < pane->button_count)
		{
			result = &pane->buttons[button->valuef8];
		}
	}
	return result;
}

/* shows the button as its definition describes it */
// @retail 0x253cff
void function_253cff(c_button_widget *button)
{
	s_button_block *definition = function_253cc8(button);
	real_rgb_color color;
	long value14 = 0;
	word buffer[0x100];
	long justification = 2;
	long font = 1;

	buffer[0] = 0;
	color.red = 1.0f;
	color.green = 1.0f;
	color.blue = 1.0f;
	if (definition)
	{
		c_screen_widget *screen = button->get_screen();
		s_screen_pane *pane;

		if (definition->flags & 1)
		{
			justification = 0;
		}
		else
		{
			justification = (definition->flags & 2) ? 1 : 2;
		}
		if (definition->flags & 4)
		{
			value14 = 1;
		}
		if (definition->text_flags & 1)
		{
			button->valuef4 |= 1;
		}
		else
		{
			button->valuef4 &= ~1;
		}
		if (definition->text_flags & 2)
		{
			button->valuef4 |= 2;
		}
		else
		{
			button->valuef4 &= ~2;
		}
		button->bounds = definition->bounds;
		button->value68 = definition->value04 - 1;
		button->value6a = definition->value34;
		((c_widget *)screen)->function_230134(definition->string_id, buffer);
		color = definition->color;
		font = definition->font;
		pane = screen->get_first_pane();
		if (pane && button->valuef8 >= 0 && button->valuef8 < pane->button_count)
		{
			button->value0a = button->valuef8;
		}
	}
	button->text.setup(buffer, font, &color, (short)value14, NONE, justification, NONE);
}

/* the value of the button's definition */
// @retail 0x2540d3
long c_button_widget::v6()
{
	s_button_block *definition = function_253cc8(this);

	return definition ? definition->value06 : 0;
}

void function_24c7e4(void *list, s_event **event, long *key);
void function_236299(long sound);

// @retail 0x253e24
void c_button_widget::v3()
{
	real_rgb_color color;

	if (is_in_window())
	{
		s_user_interface_globals *globals = function_148350();

		if (globals)
		{
			color = *(real_rgb_color const *)&globals->title_color.red;
		}
		else
		{
			color.red = 1.0f;
			color.blue = 0.0f;
			color.green = 0.0f;
		}
	}
	else
	{
		s_button_block *definition = function_253cc8(this);

		if (definition)
		{
			color = definition->color;
		}
		else
		{
			color.red = 1.0f;
			color.blue = 1.0f;
			color.green = 1.0f;
		}
	}
	get_text()->color = color;
	c_user_interface_widget::v3();
}

// @retail 0x2540e8
bool c_button_widget::v10(s_widget_event *event)
{
	if (event->type == 5 && (event->param == 0 || event->param == 12))
	{
		function_24c7e4(&handlers, (s_event **)&event, (long *)&valuef8);
		function_236299(1);
		return true;
	}
	else if (event->type == 3)
	{
		if (!(valuef4 & 1))
		{
			v8();
			function_236299(0);
			return true;
		}
	}
	else if (event->type == 1)
	{
		if (!(valuef4 & 1))
		{
			v9();
			function_236299(0);
			return true;
		}
	}
	else if (event->type == 4)
	{
		if (!(valuef4 & 2))
		{
			v8();
			function_236299(0);
			return true;
		}
	}
	else if (event->type == 2)
	{
		if (!(valuef4 & 2))
		{
			v9();
			function_236299(0);
			return true;
		}
	}
	return c_user_interface_widget::v10(event);
}

// @retail 0x254173
long c_button_widget::v17()
{
	s_button_block *definition = function_253cc8(this);

	if (definition && definition->bitmap_tag_index != NONE)
	{
		s_bitmap_group_view_2541 *bitmaps = (s_bitmap_group_view_2541 *)g_4e3b44[definition->bitmap_tag_index & 0xffff].bytes;
		long count = bitmaps->bitmap_count;

		if (is_in_window() && count > 1)
		{
			return 1;
		}
	}
	return 0;
}
