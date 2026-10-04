// @flags /O1 /arch:SSE /Gr
/* UNKNOWN_253C8B.CPP: the button widget (type 3, vtable 0x45a628), and the
   texts and buttons a screen's definition describes */

#include "unknown_11c920.h"
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
c_class_19b8b1::c_class_19b8b1(short valuef8, word user_flags) :
	c_class_1a2c81(3, user_flags),
	valuef4(0),
	valuef8(valuef8)
{
}

// @retail 0x237623 deleting c_class_19b8b1
// @retail 0x19b8b1 destructor c_class_19b8b1

// @retail 0x237607
c_class_22cc8e *c_class_19b8b1::function_22f52e()
{
	return &text;
}

// @retail 0x2541b2
void c_class_19b8b1::function_253b1a(long string_handle)
{
	c_class_1473c9 *screen = get_screen();

	if (screen)
	{
		word buffer[0x100];

		buffer[0] = 0;
		((c_widget *)screen)->function_230134(string_handle, buffer);
		function_22f52e()->set_text(buffer);
	}
}

/* ---- the texts a screen's definition describes ---- */

/* shows a text of the definition in the widget */
// @retail 0x253765
void function_253765(c_text_widget_45a5e0 *widget, short index, s_text_block const *definition)
{
	c_class_1473c9 *screen = widget->get_screen();
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
	color3f color = definition->color;
	((c_widget *)screen)->function_230134(definition->string_handle, buffer);
	widget->bounds = definition->bounds;
	widget->function_22f52e()->setup(buffer, font, &color, (short)value14, NONE, justification, NONE);
	if (definition->flags & 8)
	{
		s_type_954545 *globals = function_148350();

		widget->function_22f52e()->value14 = widget->function_22f52e()->value14 | 4;
		if (globals)
		{
			real value = globals->value4c;

			widget->function_22f52e()->value20 = value;
		}
		widget->function_22f52e()->cursor = 0;
	}
	function_13ee20(widget->function_22f52e()->function_22f52e(), font);
}

/* shows a text with these bounds, colour and font in the widget */
// @retail 0x2538a6
void function_2538a6(c_text_widget_45a5e0 *widget, short value04, short font, s_widget_bounds const *bounds, color4f const *color, short flags)
{
	s_text_block definition;

	definition.value06 = 0;
	definition.flags = flags;
	definition.value04 = value04;
	*(color4f *)&definition.alpha = *color;
	definition.string_handle = NONE;
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
		widget->function_22f52e()->set_text(buffer);
	}
}

/* the button's definition in its screen's first pane */
// @retail 0x253cc8
s_button_block *function_253cc8(c_class_19b8b1 *button)
{
	s_button_block *result = 0;
	c_class_1473c9 *screen = button->get_screen();

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
void function_253cff(c_class_19b8b1 *button)
{
	s_button_block *definition = function_253cc8(button);
	color3f color;
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
		c_class_1473c9 *screen = button->get_screen();
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
		((c_widget *)screen)->function_230134(definition->string_handle, buffer);
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
long c_class_19b8b1::v6()
{
	s_button_block *definition = function_253cc8(this);

	return definition ? definition->value06 : 0;
}

void function_24c7e4(void *list, s_event **event, long *key);
void function_236299(long sound);

// @retail 0x253e24
void c_class_19b8b1::v3()
{
	color3f color;

	if (is_in_window())
	{
		s_type_954545 *globals = function_148350();

		if (globals)
		{
			color = *(color3f const *)&globals->title_color.red;
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
	function_22f52e()->color = color;
	c_class_1a2c81::v3();
}

// @retail 0x2540e8
bool c_class_19b8b1::v10(s_widget_event *event)
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
	return c_class_1a2c81::v10(event);
}

// @retail 0x254173
long c_class_19b8b1::v17()
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

/* sets the text when its characters are in the font's cache */
// @retail 0x253af2
void function_253af2(c_class_1a2c81 *widget, word *string)
{
	if (function_13ee20(string, widget->function_22f52e()->value04))
	{
		widget->function_22f52e()->set_text(string);
	}
}

/* the text's definition in its screen's current pane */
// @retail 0x253c06
s_text_block *function_253c06(c_class_1a2c81 *text)
{
	c_class_1473c9 *screen = text->get_screen();
	s_text_block *result = 0;

	if (screen)
	{
		s_screen_pane *pane = screen->get_current_pane();

		if (pane && text->value0a >= 0 && text->value0a < pane->text_count)
		{
			result = &pane->texts[text->value0a];
		}
	}
	return result;
}

s_screen_definition *function_22f871(c_class_1473c9 *screen);

/* shows a string id of a value block of the screen's definition */
// @retail 0x253c3a
void function_253c3a(long block_index, long index, c_text_widget_45a5e0 *widget)
{
	c_class_1473c9 *screen = widget->get_screen();

	if (screen)
	{
		s_screen_definition *definition = function_22f871(screen);

		if (definition && block_index >= 0 && block_index < definition->value_block_count)
		{
			s_screen_value_block *block = &definition->value_blocks[block_index];

			if (block && index >= 0 && index < block->count)
			{
				long *value = &block->values[index];

				if (value)
				{
					widget->function_253b1a(*value);
				}
			}
		}
	}
}
