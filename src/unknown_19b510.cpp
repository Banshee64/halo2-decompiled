// @flags /O1 /Gr
/* UNKNOWN_19B510.CPP: the error dialogs. The base dialog screen (vtable
   0x454640) shows a definition's title and message; the "ok" dialog (vtable
   0x4545d0) has a button, the ok/cancel dialog (vtable 0x454560) an "error
   ok/cancel list" of its two choices (vtable 0x454508). Each calls back the
   code that opened it when the player chooses. */

#include "cseries.h"
#include "screen_widgets.h"
#include "unknown_19b516.h"
#include "unknown_19b510.h"

void function_148a58();
long function_1480ff(long screen_id);
void unicode_string_list_get_string(long tag_index, long string_id, word *buffer);

/* the base dialog screen (vtable 0x454640) */
class c_dialog_screen : public c_screen_widget
{
public:
	c_dialog_screen(long screen_id, long a, long b, word user_flags);

	virtual void v3();
	/* a press of B, back or start is the dialog's */
	virtual bool v10(s_widget_event *event);

	void set_dialog(long dialog_id, bool unused);

	long dialog_id;
	word title[0x100];
	word message[0x100];
	word first_choice[0x100];
	word second_choice[0x100];
	char choices;
	byte unknowne15[3];
	dialog_closed_callback closed;
};

/* the "error ok/cancel list" (vtable 0x454508) */
class c_dialog_choice_list : public c_list_widget
{
public:
	c_dialog_choice_list(word user_flags);

	virtual long get_item_count();
	virtual void v20(c_user_interface_widget *item, long unused);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[2];
	dialog_choice_callback first_chosen;
	dialog_choice_callback second_chosen;
	c_list_item_handler handler;
};

/* the "ok" dialog (vtable 0x4545d0) */
class c_dialog_ok_screen : public c_dialog_screen
{
public:
	c_dialog_ok_screen(long a, long b, word user_flags);

	virtual bool v10(s_widget_event *event);
	virtual screen_load_proc get_load_proc();

	void handle_button(s_controller_reference **controller, long *item);

	c_dialog_button button;
	c_list_item_handler handler;
	dialog_choice_callback chosen;
};

/* the ok/cancel dialog (vtable 0x454560) */
class c_dialog_choice_screen : public c_dialog_screen
{
public:
	c_dialog_choice_screen(long a, long b, word user_flags);

	virtual bool v10(s_widget_event *event);
	virtual void v19();
	virtual screen_load_proc get_load_proc();

	c_dialog_choice_list list;
};

c_screen_widget *__stdcall dialog_ok_screen_load(s_screen_parameters *parameters);
c_screen_widget *__stdcall dialog_choice_screen_load(s_screen_parameters *parameters);

// @retail 0x19b510
screen_load_proc c_dialog_ok_screen::get_load_proc()
{
	return dialog_ok_screen_load;
}

// @retail 0x19b51d
long c_dialog_choice_list::get_item_count()
{
	return 2;
}

// @retail 0x19b521
screen_load_proc c_dialog_choice_screen::get_load_proc()
{
	return dialog_choice_screen_load;
}

/* opens the "ok" dialog */
// @retail 0x19b527
void dialog_ok_show(long a, long dialog_id, long b, word user_flags, dialog_choice_callback chosen, dialog_closed_callback closed)
{
	s_screen_parameters parameters;
	c_dialog_ok_screen *screen;

	parameters.field_c = 0;
	function_149f49((s_message *)&parameters, 0, 0, user_flags, a, b, (long)dialog_ok_screen_load);
	screen = (c_dialog_ok_screen *)parameters.load(&parameters);
	if (screen)
	{
		s_dialog_definition definition;

		function_23661f(&definition, dialog_id);
		screen->set_dialog(dialog_id, false);
		screen->chosen = chosen;
		screen->closed = closed;
		screen->choices = definition.choices;
	}
}

void dialog_choice_show(long a, long dialog_id, long b, word user_flags, dialog_choice_callback first_chosen, dialog_choice_callback second_chosen, dialog_closed_callback closed);

/* opens the ok/cancel dialog without choice callbacks */
// @retail 0x19b590
void dialog_choice_show_default(long a, long b, word user_flags, dialog_choice_callback first_chosen, long dialog_id)
{
	dialog_choice_show(a, dialog_id, b, user_flags, first_chosen, 0, 0);
}

/* opens the ok/cancel dialog */
// @retail 0x19b5af
void dialog_choice_show(long a, long dialog_id, long b, word user_flags, dialog_choice_callback first_chosen, dialog_choice_callback second_chosen, dialog_closed_callback closed)
{
	s_screen_parameters parameters;
	c_dialog_choice_screen *screen;

	parameters.field_c = 0;
	function_149f49((s_message *)&parameters, 0, 0, user_flags, a, b, (long)dialog_choice_screen_load);
	screen = (c_dialog_choice_screen *)parameters.load(&parameters);
	if (screen)
	{
		s_dialog_definition definition;

		function_23661f(&definition, dialog_id);
		screen->set_dialog(dialog_id, true);
		screen->list.first_chosen = first_chosen;
		screen->list.second_chosen = second_chosen;
		screen->closed = closed;
		screen->choices = definition.choices;
		screen->set_screen_id(definition.screen_id);
	}
}

// @retail 0x19b62e
c_dialog_screen::c_dialog_screen(long screen_id, long a, long b, word user_flags) :
	c_screen_widget(screen_id, a, b, user_flags),
	dialog_id(0)
{
	title[0] = 0;
	message[0] = 0;
	first_choice[0] = 0;
	second_choice[0] = 0;
	closed = 0;
	choices = 0;
}

/* shows the title and message, and closes the dialog when its callback says
   so */
// @retail 0x19b682
void c_dialog_screen::v3()
{
	c_user_interface_widget *title_widget = find_child(6, 0, false);
	c_user_interface_widget *message_widget = find_child(6, 2, false);

	((c_widget *)(void *)this)->function_22e391();
	if (title_widget)
		title_widget->get_text()->set_text(title);
	if (message_widget)
		message_widget->get_text()->set_text(message);
	if (closed && closed(this, dialog_id) && !TEST_FIELD_BIT(animation.flags.flag1))
		start_animation(3);
}

// @retail 0x19b70b
bool c_dialog_screen::v10(s_widget_event *event)
{
	bool result = false;

	if (event->type == 5)
	{
		switch (event->param)
		{
		case 1:
		case 3:
		case 13:
			result = true;
			break;
		}
	}
	return result;
}

/* reads the dialog's strings from its definition */
// @retail 0x19b72a
void c_dialog_screen::set_dialog(long dialog_id, bool unused)
{
	s_dialog_definition definition;

	(void)unused;

	function_23661f(&definition, dialog_id);
	title[0] = 0;
	message[0] = 0;
	first_choice[0] = 0;
	second_choice[0] = 0;
	this->dialog_id = dialog_id;
	if (definition.string_list_index != NONE)
	{
		long string_list_index = definition.string_list_index;

		unicode_string_list_get_string(string_list_index, definition.title, title);
		unicode_string_list_get_string(string_list_index, definition.message, message);
		unicode_string_list_get_string(string_list_index, definition.first_choice, first_choice);
		unicode_string_list_get_string(string_list_index, definition.second_choice, second_choice);
	}
	else
	{
		((c_widget *)(void *)this)->function_230134(definition.title, title);
		((c_widget *)(void *)this)->function_230134(definition.message, message);
		((c_widget *)(void *)this)->function_230134(definition.first_choice, first_choice);
		((c_widget *)(void *)this)->function_230134(definition.second_choice, second_choice);
	}
	choices = definition.choices;
}

// @retail 0x19b7f8
c_screen_widget *__stdcall dialog_ok_screen_load(s_screen_parameters *parameters)
{
	c_dialog_ok_screen *screen = new c_dialog_ok_screen(parameters->a, parameters->b, parameters->user_flags);

	if (screen)
	{
		screen->m6c = true;
		screen->function_147f6d(parameters);
	}
	return screen;
}

// @retail 0x19b83c
c_dialog_ok_screen::c_dialog_ok_screen(long a, long b, word user_flags) :
	c_dialog_screen(8, a, b, user_flags),
	button(0, user_flags),
	handler((c_list_widget *)(void *)this, (list_item_method)&c_dialog_ok_screen::handle_button),
	chosen(0)
{
}

// @retail 0x19b895 deleting c_dialog_ok_screen
// @retail 0x19b8d0 destructor c_dialog_ok_screen
// @retail 0x19b8b1 destructor c_dialog_button

/* closes the dialog when the callback says so */
// @retail 0x19b97a
void c_dialog_ok_screen::handle_button(s_controller_reference **controller, long *item)
{
	bool close;

	if (chosen)
		close = chosen((*controller)->controller_index);
	else
		close = true;
	if (close)
		start_animation(3);
}

// @retail 0x19b9a7
bool c_dialog_ok_screen::v10(s_widget_event *event)
{
	return c_dialog_screen::v10(event);
}

// @retail 0x19b9ac
c_dialog_choice_list::c_dialog_choice_list(word user_flags) :
	c_list_widget(user_flags),
	first_chosen(0),
	second_chosen(0),
	handler(this, (list_item_method)&c_dialog_choice_list::handle_item)
{
	data = user_interface_data_new("error ok/cancel list", 2, 4);
	data_make_valid(data);
	for (long i = 0; i < data->maximum_count; i++)
	{
		datum_new(data);
	}
	delegate_register(&item_handlers, &handler);
}

// @retail 0x19ba48 deleting c_dialog_choice_list
// @retail 0x19ba66 destructor c_dialog_choice_list

/* shows the choice's text: the dialog's, or "ok" or "cancel" */
// @retail 0x19ba9c
void c_dialog_choice_list::v20(c_user_interface_widget *item, long unused)
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)item->find_child(6, 0, false);

	if (text)
	{
		c_dialog_screen *screen = (c_dialog_screen *)get_screen();
		word *first = screen->first_choice;
		word *second = screen->second_choice;

		switch ((short)((c_list_item_widget *)item)->value70)
		{
		case 0:
			if (first && *first)
				text->get_text()->set_text(first);
			else
				text->set_string(0xf000146);
			break;
		case 1:
			if (second && *second)
				text->get_text()->set_text(second);
			else
				text->set_string(0x13000147);
			break;
		}
	}
}

/* closes the dialog when the choice's callback says so */
// @retail 0x19bb1e
void c_dialog_choice_list::handle_item(s_controller_reference **controller, long *item)
{
	dialog_choice_callback callback;

	switch ((short)*item)
	{
	case 0:
		callback = first_chosen;
		break;
	case 1:
		callback = second_chosen;
		break;
	default:
		__assume(0);
	}
	if (!callback || callback((*controller)->controller_index))
	{
		c_screen_widget *screen = get_screen();
		if (screen)
			screen->start_animation(3);
	}
}

// @retail 0x19bb66
c_screen_widget *__stdcall dialog_choice_screen_load(s_screen_parameters *parameters)
{
	c_dialog_choice_screen *screen = new c_dialog_choice_screen(parameters->a, parameters->b, parameters->user_flags);

	if (screen)
	{
		screen->m6c = true;
		screen->function_147f6d(parameters);
	}
	return screen;
}

// @retail 0x19bba8
c_dialog_choice_screen::c_dialog_choice_screen(long a, long b, word user_flags) :
	c_dialog_screen(7, a, b, user_flags),
	list(user_flags)
{
}

// @retail 0x19bbd9 deleting c_dialog_choice_screen
// @retail 0x19bbf7 destructor c_dialog_choice_screen

/* focuses the dialog's default choice */
// @retail 0x19bc60
void c_dialog_choice_screen::v19()
{
	if (choices)
	{
		if (choices == 1)
			list.select_item(0);
		else if (choices == 2)
			list.select_item(1);
	}
	c_screen_widget::v19();
}

/* B or back chooses the second choice */
// @retail 0x19bc8e
bool c_dialog_choice_screen::v10(s_widget_event *event)
{
	if (event->type == 5)
	{
		switch (event->param)
		{
		case 1:
		case 13:
		{
			long item = 1;

			list.handle_item((s_controller_reference **)&event, &item);
			return true;
		}
		}
	}
	return c_dialog_screen::v10(event);
}
