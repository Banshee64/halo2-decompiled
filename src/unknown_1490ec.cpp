// @flags /O1 /Oi /Gr
/* UNKNOWN_1490EC.CPP: queries of the window manager's channels (the rest of
   the window manager is in unknown_147f6d.cpp) */

#include "cseries.h"
#include "unknown_234c64.h"
#include "globals.h"
#include "unknown_19b516.h"

c_window_channel *function_148262(long channel, long index);
void function_23538b(c_window_channel *channel);

#define WINDOW_IN_USE(window) function_1473b6(window)

bool function_1900a5(long player);
bool __stdcall function_148e6d(long user_index);
bool function_138800();
long function_147f4f();
c_screen_widget *__stdcall function_231995(s_screen_parameters *request);

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

/* opens the screen of a user (0x231995) in channel 6 */
// @retail 0x1496c7
void function_1496c7(long user_index)
{
	s_screen_parameters parameters;

	parameters.field_c = 0;
	function_149f49((s_message *)&parameters, 6, 0, 1 << user_index, 4, 4, (long)function_231995);
	parameters.load(&parameters);
}

/* the string ids of 18 entries */
// @retail 0x14986f
long function_14986f(char index)
{
	long string_ids[18] =
	{
		0x100030a, 0x100030b, 0x100030c, 0x100030d, 0x100030e, 0x100030f, 0x1000310, 0x1000311, 0x1000312,
		0x1000313, 0x2000314, 0x2000315, 0x2000316, 0x2000317, 0x2000318, 0x2000319, 0x200031a, 0x200031b
	};
	char const *index_reference = &index;
	long result = NONE;

	if (*index_reference != NONE && *index_reference >= 0 && *index_reference < 18)
	{
		result = string_ids[*index_reference];
	}
	return result;
}

// @retail 0x14990f
long function_14990f(byte index)
{
	long string_ids[62] =
	{
		0x100030a, 0x100030b, 0x100030c, 0x100030d, 0x100030e, 0x100030f, 0x1000310, 0x1000311,
		0x1000312, 0x1000313, 0x2000314, 0x2000315, 0x2000316, 0x2000317, 0x2000318, 0x2000319,
		0x200031a, 0x200031b, 0x200031c, 0x200031d, 0x200031e, 0x200031f, 0x2000320, 0x2000321,
		0x2000322, 0x2000323, 0x2000324, 0x2000325, 0x2000326, 0x2000327, 0x2000328, 0x2000329,
		0x200032a, 0x200032b, 0x200032c, 0x200032d, 0x200032e, 0x200032f, 0x2000330, 0x2000331,
		0x2000332, 0x2000333, 0x2000334, 0x2000335, 0x2000336, 0x2000337, 0x2000338, 0x2000339,
		0x200033a, 0x200033b, 0x200033c, 0x200033d, 0x200033e, 0x200033f, 0x2000340, 0x2000341,
		0x2000342, 0x2000343, 0x2000344, 0x2000345, 0x2000346, 0x2000347
	};
	byte const *index_reference = &index;
	long result = NONE;

	if ((*index_reference > 0x3f ? 0x3f : *index_reference) == *index_reference)
	{
		result = string_ids[*index_reference];
	}
	return result;
}

// @retail 0x149b5a
long function_149b5a(byte index)
{
	long string_ids[62] =
	{
		0xb00036f, 0xb000370, 0xb000371, 0xb000372, 0xb000373, 0xb000374, 0xb000375, 0xb000376,
		0xb000377, 0xb000378, 0xc000379, 0xc00037a, 0xc00037b, 0xc00037c, 0xc00037d, 0xc00037e,
		0xc00037f, 0xc000380, 0xc000381, 0xc000382, 0xc000383, 0xc000384, 0xc000385, 0xc000386,
		0xc000387, 0xc000388, 0xc000389, 0xc00038a, 0xc00038b, 0xc00038c, 0xc00038d, 0xc00038e,
		0xc00038f, 0xc000390, 0xc000391, 0xc000392, 0xc000393, 0xc000394, 0xc000395, 0xc000396,
		0xc000397, 0xc000398, 0xc000399, 0xc00039a, 0xc00039b, 0xc00039c, 0xc00039d, 0xc00039e,
		0xc00039f, 0xc0003a0, 0xc0003a1, 0xc0003a2, 0xc0003a3, 0xc0003a4, 0xc0003a5, 0xc0003a6,
		0xc0003a7, 0xc0003a8, 0xc0003a9, 0xc0003aa, 0xc0003ab, 0xc0003ac
	};
	byte const *index_reference = &index;
	long result = NONE;

	if ((*index_reference > 0x3f ? 0x3f : *index_reference) == *index_reference)
	{
		result = string_ids[*index_reference];
	}
	return result;
}

// @retail 0x149da5
long function_149da5(byte index)
{
	long string_ids[30] =
	{
		0xb0003af, 0xb0003b0, 0xb0003b1, 0xb0003b2, 0xb0003b3, 0xb0003b4, 0xb0003b5, 0xb0003b6,
		0xb0003b7, 0xb0003b8, 0xc0003b9, 0xc0003ba, 0xc0003bb, 0xc0003bc, 0xc0003bd, 0xc0003be,
		0xc0003bf, 0xc0003c0, 0xc0003c1, 0xc0003c2, 0xc0003c3, 0xc0003c4, 0xc0003c5, 0xc0003c6,
		0xc0003c7, 0xc0003c8, 0xc0003c9, 0xc0003ca, 0xc0003cb, 0xc0003cc
	};
	byte const *index_reference = &index;
	long result = NONE;

	if ((*index_reference > 0x3f ? 0x3f : *index_reference) == *index_reference)
	{
		result = string_ids[*index_reference];
	}
	return result;
}

/* opens a screen in channel 3 */
// @retail 0x149ef3
void function_149ef3(word user_flags, long load)
{
	s_screen_parameters parameters;

	parameters.field_c = 0;
	function_149f49((s_message *)&parameters, 0, 0, user_flags, 3, 4, load);
	parameters.load(&parameters);
}

/* opens a screen in channel 5 */
// @retail 0x149f1e
void function_149f1e(word user_flags, long load)
{
	s_screen_parameters parameters;

	parameters.field_c = 0;
	function_149f49((s_message *)&parameters, 0, 0, user_flags, 5, 4, load);
	parameters.load(&parameters);
}

// @retail 0x14a0d7
bool function_14a0d7(long user_index)
{
	if (function_6c7e0() && TEST_FIELD_BIT(g_54e8e0[user_index].flag5) && !function_1900a5(user_index) &&
		!window_manager_channel_in_use(4) && !window_manager_channel_in_use(2) && !window_manager_channel_in_use(0) &&
		!window_manager_channel_in_use(1) && !window_manager_channel_in_use(3) && window_manager_channel_in_use(5) &&
		function_148e6d(user_index))
	{
		return true;
	}
	return false;
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

// @retail 0x14a224
bool function_14a224(void)
{
	bool result = false;

	if (function_138800() && g_4e6948->state == 3)
	{
		long screen = function_147f4f();

		if (screen == 6 || screen == 9)
		{
			result = true;
		}
	}
	return result;
}
