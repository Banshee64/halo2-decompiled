#include "unknown_24fa12.h"
#include "unknown_11c920.h"

// @flags /O1 /Gr

// @retail 0x24fa12
c_session_user_state::c_session_user_state()
{
	index = NONE;
	active = false;
}

struct s_session_screen_state
{
	byte unknown00[0x814];
	c_session_user_state users[4];
};

// @retail 0x24fa23
long function_24fa23(s_session_screen_state const *screen)
{
	return screen->users[0].active || screen->users[1].active ||
		screen->users[2].active || screen->users[3].active;
}

c_class_1473c9 *__stdcall function_24fa4c(s_screen_parameters *parameters);

// @retail 0x24fa1d
screen_load_proc c_screen_24fd74::get_load_proc()
{
	return function_24fa4c;
}

// @retail 0x24fa4c
c_class_1473c9 *__stdcall function_24fa4c(s_screen_parameters *parameters)
{
	c_screen_24fd74 *screen = new c_screen_24fd74(parameters->a, parameters->b, parameters->user_flags);
	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x24fa88
c_screen_24fd74::c_screen_24fd74(long a, long b, word user_flags) :
	c_class_1473c9(0xe, a, b, user_flags),
	button0(0, user_flags),
	button1(1, user_flags),
	value810(true),
	value811(true),
	value812(0),
	value813(false),
	countdown_stage(NONE),
	countdown_time(0),
	mode(0),
	value142e(false),
	choice0(this, (session_choice_method)&c_screen_24fd74::handle_lobby_choice),
	choice1(this, (session_choice_method)&c_screen_24fd74::handle_lobby_choice),
	value1460(false),
	value1464(0),
	value1468(false),
	value146c(NONE),
	state_shown_time(0)
{
}

// @retail 0x24fba2 destructor c_screen_24fd74

// @retail 0x24fb86 deleting c_screen_24fd74

typedef char session_screen_size_check[sizeof(c_screen_24fd74) == 0x1474 ? 1 : -1];

extern bool g_51ec98;
bool function_199971(void);
bool function_592f0(void);
long function_148d30(void);
bool function_199e6d(long index);
void function_19a0af(long value);
long function_18fa4d(long mode);
bool function_19a78e(long controller, long countdown, long minimum);
void function_236299(long sound);
bool function_2510e1(void);

// @retail 0x24fbea
void function_24fbea(c_screen_24fd74 *screen)
{
	if (function_199971())
	{
		if (function_592f0() && g_51ec98 && function_199e6d(function_148d30()))
		{
			function_19a0af(function_148d30());
			if (!function_19a78e(function_18fa4d(0), 3, 3))
				function_236299(2);
		}
	}
	else if (!function_2510e1())
		screen->value142e = true;
}

long function_19989d(void);

// @retail 0x24ff0a
void function_24ff0a(c_screen_24fd74 *screen)
{
	short mode = 0;
	switch (function_19989d())
	{
	case 0: mode = 1; break;
	case 1: mode = 0; break;
	case 2: mode = 1; break;
	case 3: mode = 0; break;
	case 4: mode = 1; break;
	case 5: mode = 0; break;
	case 6: mode = 2; break;
	default: mode = screen->mode; break;
	}
	if (mode != screen->mode)
	{
		long pane = mode;
		screen->function_230427((short *)&pane);
		screen->mode = mode;
	}
}

struct s_session_player_view;
bool function_19a902(void);
long function_251364(s_session_player_view *player);

// @retail 0x24ff61
void function_24ff61(c_class_1a2c81 *screen, bool selected, s_session_player_view *player)
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)screen->find_child(6, 1, false);
	if (text)
	{
		long string_handle;
		if (function_19a902())
			string_handle = selected ? 0x2700021b : 0x2000021a;
		else
		{
			byte active = (byte)function_251364(player);
			if (selected)
				string_handle = active ? 0x25000217 : 0x26000216;
			else
				string_handle = active ? 0x1d000219 : 0x1e000218;
		}
		text->function_253b1a(string_handle);
	}
}

long function_1480ff(long screen_id);
bool function_13ee20(word const *string, long font);
void function_199b45(void);
byte *function_19aaa5(long player_index);

// @retail 0x24fc53
void c_screen_24fd74::v18(void *parameters)
{
	volatile long definition_index = function_1480ff(screen_id);
	c_class_1a2c81 *buttons[2] = { &button0, &button1 };
	s_screen_layout layout = { 0, 1, { { 2, buttons, 0, 0 } } };
	build(&layout);
	delegate_register(&button0.handlers, &choice0);
	delegate_register(&button1.handlers, &choice1);
	value1464 = 0;
	value146c = NONE;
	value1468 = false;
	value1460 = false;
	c_class_1a2c81::v1();
	v7(&button0);
	if (!(*(byte *)parameters & 1))
		function_24fbea(this);
	function_13ee20((word const *)L"0123456789", 4);
	g_51ec98 = false;
}

// @retail 0x24fd21
void c_screen_24fd74::v2()
{
	if (value1468)
	{
		function_199b45();
		value1468 = false;
	}
	c_class_1a2c81::v2();
}

// @retail 0x24fd41
int __cdecl function_24fd41(void const *left, void const *right)
{
	/* The comparator's first argument is stack-resident in retail. */
	void const *const *local_left_reference = &left;
	byte *local_left = function_19aaa5(*(long const *)*local_left_reference);
	byte *local_right = function_19aaa5(*(long const *)right);
	char a = *(char *)(local_left + 0x7c);
	char b = *(char *)(local_right + 0x7c);
	return a > b ? 1 : a < b ? -1 : 0;
}
