// @flags /O1 /Oi /arch:SSE /Gr
/* USER_INTERFACE_WIDGET_WINDOW.CPP: building a screen from its definition:
   its title and subtitle, its panes (their buttons, list, texts, bitmaps,
   models and widget groups) and the widgets of its widget set */

#include "cseries.h"
#include <string.h>
#include "screen_widgets.h"
#include "unknown_19b516.h"
#include "unknown_234c64.h"
#include "screen_online_y_menu.h"

s_screen_definition *function_22f871(c_screen_widget *screen);
void function_08cc20(struct s_name_buffer *buffer, const wchar_t *name);
void function_253765(c_text_widget_45a5e0 *widget, short index, s_text_block const *definition);
void function_2538a6(c_text_widget_45a5e0 *widget, short value04, short font, s_widget_bounds const *bounds, real_argb_color const *color, short flags);
void function_253bc9(c_text_widget_458940 *widget, long subtitle_type);
void function_253cff(c_button_widget *button);
void function_24bc12(c_list_widget *list, bool remove_extra);
c_widget_45adf0 *function_2baf38(c_user_interface_widget *parent, s_widget_block_24 *definition);

/* ---- the widgets a pane holds ---- */

// @retail 0x22f9bf
c_text_widget_45a5e0 *function_22f9bf(c_screen_widget *screen, long index, s_text_block *definition)
{
	c_text_widget_45a5e0 *text;

	if (definition->flags & 0x10)
	{
		text = new c_text_widget_32(screen->user_flags);
	}
	else
	{
		text = new c_text_widget_458940(screen->user_flags);
	}
	if (text)
	{
		text->m6c = true;
		screen->add_child(text);
		function_253765(text, (short)index, definition);
	}
	return text;
}

// @retail 0x22fa30
c_user_interface_widget *function_22fa30(c_user_interface_widget *parent, s_bitmap_block *definition)
{
	c_bitmap_widget *bitmap = new c_bitmap_widget(definition);

	if (bitmap)
	{
		bitmap->m6c = true;
		parent->add_child(bitmap);
	}
	return bitmap;
}

// @retail 0x22fa5b
c_user_interface_widget *function_22fa5b(c_user_interface_widget *parent, s_model_block *definition)
{
	c_model_widget *model = new c_model_widget(definition);

	if (model)
	{
		model->m6c = true;
		parent->add_child(model);
	}
	return model;
}

// @retail 0x22fa7d
c_widget_45ad18 *function_22fa7d(c_screen_widget *screen, long index, s_widget_block_18 *definition)
{
	c_widget_45ad18 *widget = new c_widget_45ad18(index, definition);

	if (widget)
	{
		s_widget_point origin;

		widget->m6c = true;
		screen->add_child(widget);
		memset(&origin, 0, sizeof(origin));
		widget->place(&origin);
	}
	return widget;
}

/* the widgets of the current pane: bitmaps, groups, texts and models */
// @retail 0x22fc08
void __stdcall function_22fc08(c_screen_widget *screen)
{
	s_screen_pane *pane = screen->get_current_pane();

	if (pane)
	{
		bool animating = function_22f0ff((c_widget *)screen);
		long bitmap_index;
		long block_index;
		long group_index;
		long text_index;
		long model_index;

		for (bitmap_index = 0; bitmap_index < pane->bitmap_count; bitmap_index++)
		{
			c_user_interface_widget *bitmap = function_22fa30(screen, &pane->bitmaps[bitmap_index]);

			if (bitmap)
			{
				if (animating)
				{
					bitmap->start_animation(screen->animation.type);
				}
				bitmap->value0a = (short)bitmap_index;
			}
		}
		for (block_index = 0; block_index < pane->block_24_count; block_index++)
		{
			c_user_interface_widget *widget = function_2baf38(screen, &pane->blocks_24[block_index]);

			if (widget)
			{
				if (animating)
				{
					widget->start_animation(screen->animation.type);
				}
				widget->value0a = (short)block_index;
			}
		}
		for (group_index = 0; group_index < pane->block_18_count; group_index++)
		{
			s_widget_block_18 *block = &pane->blocks_18[group_index];
			long j;

			for (j = 0; j < block->count; j++)
			{
				c_user_interface_widget *widget = function_22fa7d(screen, j, block);

				if (widget)
				{
					if (animating)
					{
						widget->start_animation(screen->animation.type);
					}
					widget->value0a = (short)group_index;
				}
			}
		}
		for (text_index = 0; text_index < pane->text_count; text_index++)
		{
			c_user_interface_widget *text = function_22f9bf(screen, text_index, &pane->texts[text_index]);

			if (text && animating)
			{
				text->start_animation(screen->animation.type);
			}
		}
		for (model_index = 0; model_index < pane->model_count; model_index++)
		{
			c_user_interface_widget *model = function_22fa5b(screen, &pane->models[model_index]);

			if (model)
			{
				if (animating)
				{
					model->start_animation(screen->animation.type);
				}
				model->value0a = (short)model_index;
			}
		}
	}
}

/* the models and bitmaps of a widget set of the user interface globals */
// @retail 0x22facd
void function_22facd(c_screen_widget *screen, short set_index)
{
	bool animating = function_22f0ff((c_widget *)screen);

	if (set_index >= 0 && set_index < 0x20)
	{
		s_user_interface_globals *globals = function_148350();

		if (globals && set_index < globals->widget_set_count)
		{
			s_widget_set *set = &globals->widget_sets[set_index];
			long i;

			for (i = 0; i < set->model_count; i++)
			{
				c_user_interface_widget *model = function_22fa5b(screen, &set->models[i]);

				if (model && animating)
				{
					model->start_animation(screen->animation.type);
				}
			}
			for (i = 0; i < set->bitmap_count; i++)
			{
				c_user_interface_widget *bitmap = function_22fa30(screen, &set->bitmaps[i]);

				if (bitmap)
				{
					if (animating)
					{
						bitmap->start_animation(screen->animation.type);
					}
					bitmap->value0a = (short)-i;
				}
			}
		}
	}
}

/* ---- the panes ---- */

/* builds the screen's current pane: its buttons or its list, then the rest
   of its widgets */
// @retail 0x22f6ca
void function_22f6ca(c_screen_widget *screen, s_screen_layout *layout, bool rebuild)
{
	if (rebuild || (screen->value5f0 >= 0 && screen->value5f0 < layout->count))
	{
		s_screen_definition *definition = function_22f871(screen);
		s_screen_pane *pane = screen->get_current_pane();

		if (definition && pane)
		{
			screen->value68 = pane->value02 - 1;
			if (rebuild)
			{
				long i;

				for (i = 0; i < pane->button_count; i++)
				{
					c_user_interface_widget *button = layout->lists[screen->value5f0].widget[i];

					if (!button)
					{
						break;
					}
					screen->add_child(button);
					function_253cff((c_button_widget *)layout->lists[screen->value5f0].widget[i]);
				}
				if (pane->list_count > 0)
				{
					screen->add_child(layout->lists[screen->value5f0].list);
					goto build_list;
				}
			}
			else if (layout->lists[screen->value5f0].type > 0)
			{
				long count = layout->lists[screen->value5f0].type;
				long i;

				if (count > pane->button_count)
				{
					count = pane->button_count;
				}
				for (i = 0; i < count; i++)
				{
					screen->add_child(layout->lists[screen->value5f0].widget[i]);
					function_253cff((c_button_widget *)layout->lists[screen->value5f0].widget[i]);
				}
			}
			else if (layout->lists[screen->value5f0].list)
			{
				screen->add_child(layout->lists[screen->value5f0].list);
				rebuild = false;
build_list:
				function_24bc12(layout->lists[screen->value5f0].list, rebuild);
				if (definition->flag1)
				{
					layout->lists[screen->value5f0].list->notify_screen = true;
					delegate_register(&layout->lists[screen->value5f0].list->head80, (c_list_item_delegate *)&screen->delegate);
				}
			}
			function_22fc08(screen);
		}
	}
}

/* the title: its text and size come from the definition */
// @retail 0x22fda6
void __stdcall function_22fda6(c_screen_widget *screen)
{
	s_user_interface_globals *globals = function_148350();
	s_screen_definition *definition = function_22f871(screen);
	word name[0x100];
	short pane_value;
	real_argb_color color;
	long font;
	s_widget_bounds bounds;

	name[0] = 0;
	pane_value = screen->get_first_pane_value();
	color.alpha = 1.0f;
	color.red = 0.7f;
	color.green = 0.7f;
	color.blue = 0.7f;
	font = 1;
	if (definition ? !definition->no_title : true)
	{
		bounds.left = -100;
		bounds.right = 100;
		bounds.top = 100;
		bounds.bottom = 80;
		screen->add_child(&screen->title);
		if (globals)
		{
			color = globals->title_color;
			if (definition)
			{
				if (definition->flags & 1)
				{
					font = globals->title_fonts[3];
					bounds = globals->title_bounds[3].title;
				}
				else if (definition->flags & 8)
				{
					font = globals->title_fonts[2];
					bounds = globals->title_bounds[2].title;
				}
				else if (definition->flags & 0x10)
				{
					font = globals->title_fonts[1];
					bounds = globals->title_bounds[1].title;
				}
				else
				{
					font = globals->title_fonts[0];
					bounds = globals->title_bounds[0].title;
				}
				screen->title.set_string(definition->title_string_id);
				function_08cc20((s_name_buffer *)name, (const wchar_t *)screen->title.get_text()->get_text());
			}
		}
		function_2538a6(&screen->title, pane_value, (short)font, &bounds, &color, 1);
		screen->title.get_text()->value14 |= 2;
		screen->title.get_text()->set_text(name);
		screen->title.value6a = 0x7fff;
	}
}

/* the subtitle: the user's gamertag */
// @retail 0x22ff53
void __stdcall function_22ff53(c_screen_widget *screen)
{
	s_user_interface_globals *globals = function_148350();
	s_screen_definition *definition = function_22f871(screen);
	short pane_value = screen->get_first_pane_value();
	word name[0x100];
	real_argb_color color;
	s_widget_bounds bounds;

	name[0] = 0;
	color.alpha = 1.0f;
	color.red = 0.7f;
	color.green = 0.7f;
	color.blue = 0.7f;
	wcsncpy((wchar_t *)name, L"", 0xff);
	name[0xff] = 0;
	bounds.left = -100;
	bounds.right = 100;
	bounds.top = -100;
	bounds.bottom = -120;
	screen->add_child(&screen->subtitle);
	if (globals)
	{
		color = globals->title_color;
		if (definition)
		{
			color = definition->subtitle_color;
			if (definition->flags & 1)
			{
				bounds = globals->title_bounds[3].subtitle;
			}
			else if (definition->flags & 8)
			{
				bounds = globals->title_bounds[2].subtitle;
			}
			else if (definition->flags & 0x10)
			{
				bounds = globals->title_bounds[1].subtitle;
			}
			else
			{
				bounds = globals->title_bounds[0].subtitle;
			}
			function_253bc9(&screen->subtitle, definition->value06);
			function_08cc20((s_name_buffer *)name, (const wchar_t *)screen->subtitle.get_text()->get_text());
		}
	}
	function_2538a6(&screen->subtitle, pane_value, 1, &bounds, &color, 2);
	screen->subtitle.get_text()->set_text(name);
	screen->subtitle.value6a = 0x7fff;
}

/* builds the screen from its definition: its title and subtitle, its panes
   (each pane a child of the layout's container when there are several) and
   its widget set */
// @retail 0x22f8df
void c_screen_widget::build(s_screen_layout *layout)
{
	s_screen_definition *definition = function_22f871(this);

	function_22fda6(this);
	function_22ff53(this);
	if (definition)
	{
		bool rebuild = layout->count == NONE;

		value5f4 = TEST_FIELD_BIT(definition->flag5);
		if (definition->pane_count > 0)
		{
			if (definition->pane_count > 1 && layout->container)
			{
				c_screen_widget *pane = (c_screen_widget *)layout->container->child;
				short i;

				value5f0 = 0;
				value68 = definition->panes->value02 - 1;
				add_child(layout->container);
				for (i = 0; i < definition->pane_count; i++)
				{
					pane->value5f0 = i;
					function_22f6ca(pane, layout, rebuild);
					pane = (c_screen_widget *)pane->next;
				}
			}
			else if (definition->pane_count == 1 || definition->flag1)
			{
				value5f0 = 0;
				function_22f6ca(this, layout, rebuild);
			}
			function_22facd(this, definition->widget_set - 1);
		}
	}
}
