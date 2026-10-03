// @flags /O1 /Oi /Gr
/* UNKNOWN_1490EC.CPP: queries of the window manager's channels (the rest of
   the window manager is in unknown_147f6d.cpp) */

#include "cseries.h"
#include "unknown_234c64.h"

c_window_channel *function_148262(long channel, long index);
void function_23538b(c_window_channel *channel);

#define WINDOW_IN_USE(window) function_1473b6(window)

/* the fields of a screen read here */
struct s_screen_view
{
	byte unknown000[0x70];
	long screen_id;
	byte unknown074[0x610 - 0x74];
	long user_index;
};

// @retail 0x1490ec
bool window_manager_channel_window_in_use(long channel, long index)
{
	bool result = false;

	switch (channel)
	{
	case 0:
		result = index == 4 && WINDOW_IN_USE(&g_54d598.window_0);
		break;
	case 1:
		result = WINDOW_IN_USE(&g_54d598.windows_1[index]);
		break;
	case 2:
		result = index == 4 && WINDOW_IN_USE(&g_54d598.window_2);
		break;
	case 3:
		result = WINDOW_IN_USE(&g_54d598.windows_3[index]);
		break;
	case 4:
		result = index == 4 && WINDOW_IN_USE(&g_54d598.window_4);
		break;
	case 5:
		result = WINDOW_IN_USE(&g_54d598.windows_5[index]);
		break;
	default:
		result = index == 4 && WINDOW_IN_USE(&g_54d598.default_window);
		break;
	}
	return result;
}

// @retail 0x14917c
bool window_manager_channel_in_use(long channel)
{
	bool result = false;

	for (long index = 0; !result && index < 5; index++)
	{
		switch (channel)
		{
		case 0:
			result = WINDOW_IN_USE(&g_54d598.window_0);
			index = 5;
			break;
		case 1:
			result = WINDOW_IN_USE(&g_54d598.windows_1[index]);
			break;
		case 2:
			result = WINDOW_IN_USE(&g_54d598.window_2);
			index = 5;
			break;
		case 3:
			result = WINDOW_IN_USE(&g_54d598.windows_3[index]);
			break;
		case 4:
			result = WINDOW_IN_USE(&g_54d598.window_4);
			index = 5;
			break;
		case 5:
			result = WINDOW_IN_USE(&g_54d598.windows_5[index]);
			break;
		case 6:
			result = WINDOW_IN_USE(&g_54d598.default_window);
			index = 5;
			break;
		}
	}
	return result;
}

// @retail 0x149227
bool window_manager_any_window_in_use(void)
{
	if (window_manager_channel_in_use(0) || window_manager_channel_in_use(1) || window_manager_channel_in_use(3) ||
		window_manager_channel_in_use(5) || window_manager_channel_in_use(6) || window_manager_channel_in_use(4))
	{
		return true;
	}
	return window_manager_channel_in_use(2);
}

// @retail 0x149278
bool screen_is_pause_screen(c_screen_widget *screen)
{
	bool result = false;

	if (screen)
	{
		long screen_id = ((s_screen_view *)screen)->screen_id;

		if (screen_id >= 7 && (screen_id <= 8 || screen_id == 0xf0))
		{
			result = true;
		}
	}
	return result;
}

// @retail 0x149296
bool window_manager_channel_has_pause_screen(long channel)
{
	bool result = false;

	for (long index = 0; index < 5; index++)
	{
		c_window_channel *window = function_148262(channel, index);

		if (window && (screen_is_pause_screen(window->current) || screen_is_pause_screen(window->next)))
		{
			result = true;
			break;
		}
	}
	return result;
}

// @retail 0x1492d6
bool window_manager_window_has_pause_screen_for_user(long channel, long index, long user_index)
{
	bool result = false;
	c_window_channel *window = function_148262(channel, index);

	if (window)
	{
		c_screen_widget *current = window->current;
		c_screen_widget *next = window->next;

		if (screen_is_pause_screen(current) && ((s_screen_view *)current)->user_index == user_index)
		{
			result = true;
		}
		if (screen_is_pause_screen(next) && ((s_screen_view *)next)->user_index == user_index)
		{
			result = true;
		}
	}
	return result;
}

// @retail 0x149318
bool window_manager_has_pause_screen(void)
{
	long user_indices[3] = { 0x3a, 0x23, 0x38 };
	bool result = false;

	for (dword i = 0; i < sizeof(user_indices) / sizeof(user_indices[0]); i++)
	{
		if (window_manager_window_has_pause_screen_for_user(1, 4, user_indices[i]))
		{
			result = true;
		}
	}
	return result;
}

// @retail 0x14a152
void function_14a152(void)
{
	c_window_channel *window = &g_54d598.default_window;

	window->dispose();
	for (long index = 0; index < 5; index++)
	{
		window = &g_54d598.windows_5[index];
		window->reset();
		window = &g_54d598.windows_3[index];
		window->reset();
		window = &g_54d598.windows_1[index];
		window->reset();
		if (index == 4)
		{
			window = &g_54d598.window_0;
			window->dispose();
			window = &g_54d598.window_4;
			window->dispose();
			window = &g_54d598.window_2;
			window->dispose();
		}
	}
}

// @retail 0x14a1c3
void function_14a1c3(void)
{
	c_window_channel *window = &g_54d598.default_window;

	window->dispose();
	for (long index = 0; index < 5; index++)
	{
		window = &g_54d598.windows_3[index];
		window->reset();
		if (index == 4)
		{
			window = &g_54d598.window_0;
			window->dispose();
			window = &g_54d598.window_4;
			window->dispose();
			window = &g_54d598.window_2;
			window->dispose();
		}
	}
}

// @retail 0x14935c
void function_14935c(void)
{
	function_23538b(&g_54d598.window_2);
}
