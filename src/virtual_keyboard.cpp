#include <string.h>
#include "cseries.h"
#include "screen_widgets.h"

// @flags /O1 /Gr

/* VIRTUAL_KEYBOARD.CPP: the virtual keyboard screen (vtable 0x459ba0), which
   edits a string in place: opening it and handing it the string. */

struct s_message;
void function_149f49(s_message *message, word a, dword *id, word b, long c, long d, long e);
void unicode_string_copy(word *destination, const word *source, long maximum_count);
c_screen_widget *__stdcall function_23784f(s_screen_parameters *request);

/* the keyboard screen's fields (the screen is not written yet) */
struct s_virtual_keyboard
{
	byte unknown000[0x610];
	/* what the string is: a gamertag, a variant's name, ... */
	long type;
	long value614;
	/* the string as it was */
	word original[0x100];
	/* the string being edited, its end and its maximum length */
	word *string;
	word *end;
	short maximum_length;
	byte unknown822[0x828 - 0x822];
	bool editing;
	byte unknown829[0x3730 - 0x829];
	word default_string[0x10];
};

/* hands the keyboard the string to edit */
// @retail 0x2381de
void virtual_keyboard_set_string(s_virtual_keyboard *keyboard, word *string, short maximum_length)
{
	long length;

	if (keyboard->type == 16 && !string)
	{
		string = keyboard->default_string;
		maximum_length = 16;
	}
	unicode_string_copy(keyboard->original, string, 0x100);
	keyboard->string = string;
	length = wcslen((wchar_t *)string);
	if (keyboard->type == 7 && length > 2)
	{
		keyboard->string[length - 2] = 0;
	}
	if (keyboard->type >= 0 && (keyboard->type <= 11 || keyboard->type > 14 && keyboard->type <= 16))
	{
		keyboard->maximum_length = maximum_length > 17 ? 17 : maximum_length;
		if (maximum_length > 17)
		{
			keyboard->string[keyboard->maximum_length - 1] = 0;
		}
	}
	else
	{
		keyboard->maximum_length = maximum_length;
	}
	keyboard->maximum_length = keyboard->maximum_length > 150 ? 150 : keyboard->maximum_length;
	keyboard->end = keyboard->string + wcslen((wchar_t *)keyboard->string);
	keyboard->editing = true;
}

/* opens the keyboard on a string */
// @retail 0x238c21
void function_238c21(long controller, long type, word *name, long maximum_count)
{
	s_screen_parameters parameters;
	s_virtual_keyboard *keyboard;

	parameters.field_c = 0;
	function_149f49((s_message *)&parameters, 0, 0, 1 << controller, 2, 4, (long)function_23784f);
	keyboard = (s_virtual_keyboard *)parameters.load(&parameters);
	keyboard->type = type;
	virtual_keyboard_set_string(keyboard, name, (short)maximum_count);
}

/* the same, with a second value */
// @retail 0x238c69
void function_238c69(long mode, long type, word *name, long maximum_count, long controller)
{
	s_screen_parameters parameters;
	s_virtual_keyboard *keyboard;

	parameters.field_c = 0;
	function_149f49((s_message *)&parameters, 0, 0, 1 << controller, 2, 4, (long)function_23784f);
	keyboard = (s_virtual_keyboard *)parameters.load(&parameters);
	keyboard->type = mode;
	virtual_keyboard_set_string(keyboard, name, (short)maximum_count);
	keyboard->value614 = type;
}

bool function_199bef(const word *machine_name, const word *session_name);

/* the keyboard's string names a session to join */
// @retail 0x238922
bool function_238922(s_virtual_keyboard *keyboard)
{
	function_199bef(0, keyboard->string);
	return true;
}
