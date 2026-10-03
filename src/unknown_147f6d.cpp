// @flags /O1 /Oi /Gr
/* UNKNOWN_147F6D.CPP: the screen windows (0x54d598..), where a newly loaded
   screen is placed.

   The window manager object at 0x54d598 (constructed by 0x147645) holds the
   windows of each channel: three per-controller arrays of five (the fifth,
   index 4, is shared by all controllers) and three single windows that only
   take index 4. 0x148262 maps a channel and an index to one of them. The
   windows are the screen channels of unknown_234c64.h. */

#include "cseries.h"
#include "screen_widgets.h"
#include "unknown_19b516.h"
#include "unknown_234c64.h"
#include "globals.h"
#include <string.h>

c_window_manager g_54d598;

// @retail 0x147645
c_window_manager::c_window_manager()
{
}

long g_47ff54;

void *function_1482e8(void);

void function_236299(long sound);

s_profile_edit g_54e5d0;

struct s_player_profile; /* unknown_18f576.cpp */
void player_slot_get_profile(long index, s_player_profile *profile, long *profile_index);
void __stdcall function_18fd20(long player, s_player_profile_settings *settings, long profile_index);
void __stdcall function_215367(long player, long profile_index, void *data, long flags);
void __stdcall function_2153dd(long player, long profile_index, s_player_profile_settings *settings, long flags);

// @retail 0x147f6d
void c_screen_widget::function_147f6d(s_screen_parameters *parameters)
{
	long index = v21();
	c_window_channel *window;

	switch (parameters->a)
	{
	case 0:
		window = index == 4 ? &g_54d598.window_0 : 0;
		break;
	case 1:
		window = &g_54d598.windows_1[index];
		break;
	case 2:
		window = index == 4 ? &g_54d598.window_2 : 0;
		break;
	case 3:
		window = &g_54d598.windows_3[index];
		break;
	case 4:
		window = index == 4 ? &g_54d598.window_4 : 0;
		break;
	case 5:
		window = &g_54d598.windows_5[index];
		break;
	default:
		window = 0;
		break;
	}

	if (window)
	{
		if (!(parameters->type & 1) && screen_id != 9)
		{
			function_236299(3);
		}
		window->set_next(this, parameters);
	}
	else
	{
		this->~c_screen_widget();
		user_interface_free(this);
	}
}

/* the screen definitions of the user interface globals tag: a block of tag
   references whose tags start with their screen id at +4 */
struct s_screen_reference
{
	dword group_tag;
	long tag_index;
};

struct s_screen_references
{
	byte unknown00[8];
	long count;
	s_screen_reference *references;
};

// @retail 0x147f4f
long function_147f4f()
{
	long result = NONE;

	if (g_54d598.windows_5[4].focus)
	{
		c_screen_widget *screen = g_54d598.windows_5[4].focus->get_screen();
		if (screen)
		{
			result = screen->screen_id;
		}
	}
	return result;
}

// @retail 0x148098
long function_148098(long screen_id)
{
	s_screen_references *references = (s_screen_references *)function_1482e8();
	long result = NONE;

	if (references)
	{
		for (long i = 0; i < references->count; i++)
		{
			s_screen_reference *reference = &references->references[i];
			if (reference->tag_index != NONE &&
				((s_screen_definition *)g_4e3b44[reference->tag_index & 0xffff].bytes)->screen_id == screen_id)
			{
				result = reference->tag_index;
				break;
			}
		}
	}
	return result;
}

// @retail 0x1480ff
long function_1480ff(long screen_id)
{
	long result = function_148098(screen_id);

	if (result == NONE && screen_id != g_47ff54)
	{
		g_47ff54 = screen_id;
	}
	return result;
}

// @retail 0x148262
c_window_channel *function_148262(long channel, long index)
{
	switch (channel)
	{
	case 0:
		return index == 4 ? &g_54d598.window_0 : 0;
	case 1:
		return &g_54d598.windows_1[index];
	case 2:
		return index == 4 ? &g_54d598.window_2 : 0;
	case 3:
		return &g_54d598.windows_3[index];
	case 4:
		return index == 4 ? &g_54d598.window_4 : 0;
	case 5:
		return &g_54d598.windows_5[index];
	}
	return &g_54d598.default_window;
}

/* whether a window has a screen or one coming */
// @retail 0x1473b6
inline bool function_1473b6(c_window_channel *window)
{
	return window->current != 0 || window->next != 0;
}

/* takes a screen out of its window */
// @retail 0x148148
void function_148148(c_screen_widget *screen)
{
	c_window_channel *window = function_148262(screen->v20(), screen->v21());
	bool active;

	window->remove(screen);
	active = function_1473b6(&g_54d598.default_window) || function_1473b6(&g_54d598.window_0) ||
		function_1473b6(&g_54d598.window_4) || function_1473b6(&g_54d598.window_2);
	for (long i = 0; !active && i < 5; i++)
	{
		active = function_1473b6(&g_54d598.windows_5[i]) || function_1473b6(&g_54d598.windows_3[i]) ||
			function_1473b6(&g_54d598.windows_1[i]);
	}
	if (!active)
	{
		g_54d598.active = false;
	}
	for (unsigned long i = 0; i < 0x23; i++)
	{
		if (g_54d598.screens[i] == screen)
		{
			g_54d598.screens[i] = 0;
		}
	}
}

// @retail 0x14887e
void function_14887e(s_screen_settings_54dc6c *settings)
{
	if (settings)
	{
		*settings = g_54d598.settings;
	}
}

/* the screen a window shows */
// @retail 0x148d91
c_screen_widget *function_148d91(long channel, long index)
{
	c_window_channel *window;

	switch (channel)
	{
	case 0:
		window = index == 4 ? &g_54d598.window_0 : 0;
		break;
	case 1:
		window = &g_54d598.windows_1[index];
		break;
	case 2:
		window = index == 4 ? &g_54d598.window_2 : 0;
		break;
	case 3:
		window = &g_54d598.windows_3[index];
		break;
	case 4:
		window = index == 4 ? &g_54d598.window_4 : 0;
		break;
	case 5:
		window = &g_54d598.windows_5[index];
		break;
	default:
		window = &g_54d598.default_window;
		break;
	}
	return window->focus;
}

/* leaves the window's current screen */
// @retail 0x14800c
void function_14800c(long channel, long index)
{
	c_window_channel *window;

	switch (channel)
	{
	case 2:
		g_54d598.window_2.v7();
		break;
	case 3:
		window = &g_54d598.windows_3[index];
		window->v7();
		break;
	case 5:
		window = &g_54d598.windows_5[index];
		window->v7();
		break;
	}
	function_236299(4);
}

// @retail 0x148044
bool function_148044(long channel, long index, long value)
{
	bool result = false;
	c_window_channel *window;

	switch (channel)
	{
	case 2:
		g_54d598.window_2.v7();
		break;
	case 3:
		window = &g_54d598.windows_3[index];
		window->v7();
		break;
	case 5:
		window = &g_54d598.windows_5[index];
		result = ((c_window_channel_4599a8 *)window)->v12(value) > 0;
		break;
	}
	if (result)
	{
		function_236299(4);
	}
	return result;
}

/* whether a screen of this id has a definition */
// @retail 0x1480ed
bool function_1480ed(long screen_id)
{
	long tag_index = function_148098(screen_id);

	return tag_index != NONE;
}

/* remembers a screen (once) */
// @retail 0x148119
void function_148119(c_screen_widget *screen)
{
	unsigned long i;

	for (i = 0; i < 0x23; i++)
	{
		if (g_54d598.screens[i] == screen)
		{
			return;
		}
	}
	for (i = 0; i < 0x23; i++)
	{
		if (!g_54d598.screens[i])
		{
			g_54d598.screens[i] = screen;
			return;
		}
	}
}

/* whether a window's current screen has this id */
// @retail 0x148222
long function_148222(long channel, long index, long screen_id)
{
	c_screen_widget *screen = function_148262(channel, index)->current;

	if (screen && screen->screen_id == screen_id)
	{
		return 1;
	}
	return 0;
}

void function_23538b(c_window_channel *channel);

// @retail 0x148241
void function_148241(long channel, long index, long screen_id)
{
	c_window_channel *window = function_148262(channel, index);

	if (window && window->current && window->current->screen_id == screen_id)
	{
		function_23538b(window);
	}
}

struct s_window_manager_text
{
	long type;
	char text04[0xc];
	char text10[1];
};

// @retail 0x148956
const char *function_148956(s_window_manager_text *text)
{
	const char *result = "";

	switch (text->type)
	{
	case 1:
		result = text->text10;
		break;
	case 2:
		result = text->text10;
		break;
	case 3:
		result = text->text04;
		break;
	}
	return result;
}

// @retail 0x14896e
void function_14896e(s_window_manager_754 *a, s_window_manager_df6 *b)
{
	*a = g_54d598.m754;
	*b = g_54d598.mdf6;
}

// @retail 0x148995
void function_148995(s_window_manager_e94 *value)
{
	if (value)
	{
		g_54d598.me94 = *value;
	}
	else
	{
		memset(&g_54d598.me94, 0, sizeof(g_54d598.me94));
	}
}

// @retail 0x148a58
void function_148a58()
{
	if (g_54d598.mf04 != NONE)
	{
		if (!(bool)(((dword)g_54d598.mf04 >> 21) & 1))
		{
			function_215367(NONE, g_54d598.mf04, g_54d598.mf08, 0);
		}
		g_54d598.mf04 = NONE;
	}
	memset(g_54d598.mf08, 0, sizeof(g_54d598.mf08));
}

// @retail 0x148a8d
void function_148a8d()
{
	g_54d598.mf04 = NONE;
	memset(g_54d598.mf08, 0, sizeof(g_54d598.mf08));
}

void __stdcall function_19b5af(long a, long message, long b, dword controller_flags, void *callback0, void *callback1, long c);
bool __stdcall function_236964(long controller);
bool __stdcall function_236989(long controller);
bool __stdcall function_2523b7(long controller);

/* tells the controllers why a game variant could not be saved */
// @retail 0x148aa3
void function_148aa3(long error, dword controller_flags)
{
	switch (error)
	{
	case 1:
		function_19b5af(3, 0x4b, 4, controller_flags, function_236964, function_2523b7, 0);
		break;
	case 2:
		function_19b5af(3, 0x4f, 4, controller_flags, function_236989, function_2523b7, 0);
		break;
	default:
		function_19b5af(3, 0xd, 4, controller_flags, function_236989, function_2523b7, 0);
		break;
	}
}

/* starts editing a player's profile settings */
// @retail 0x148ada
void profile_edit_begin(long player, s_player_profile_settings *settings, long profile_index)
{
	g_54e5d0.player = player;
	g_54e5d0.profile_index = profile_index;
	memcpy(&g_54e5d0.settings, settings, sizeof(g_54e5d0.settings));
}

/* writes the edited settings back to the profile */
// @retail 0x148b8c
void profile_edit_save()
{
	long profile_index = g_54e5d0.profile_index;

	if (profile_index != NONE)
	{
		long player = g_54e5d0.player;
		long pinned = player < 0 ? 0 : (player > 3 ? 3 : player);

		if (pinned == player)
		{
			byte profile[0x1e0];
			long found_index;

			player_slot_get_profile(player, (s_player_profile *)profile, &found_index);
			if (found_index == profile_index)
			{
				function_18fd20(player, &g_54e5d0.settings, found_index);
				return;
			}
		}
		if (!(bool)(((dword)profile_index >> 21) & 1))
		{
			function_2153dd(player, profile_index, &g_54e5d0.settings, 0);
		}
	}
}

/* saves the edited settings and stops editing */
// @retail 0x148c21
void function_148c21()
{
	memset(&g_54e5d0.settings, 0, sizeof(g_54e5d0.settings));
	g_54e5d0.player = NONE;
	g_54e5d0.profile_index = NONE;
}

/* unknown_216c40.cpp */
long saved_game_file_size_in_blocks(long size);
long saved_game_file_type_size_in_blocks(long type);

/* the blocks a saved film takes */
// @retail 0x148c9d
long saved_film_size_in_blocks()
{
	return saved_game_file_size_in_blocks(0x40c400);
}

/* the blocks a player profile, a game variant and a saved film take
   together, and one more */
// @retail 0x148d0d
long minimal_storage_size_in_blocks()
{
	return saved_game_file_type_size_in_blocks(1) + saved_game_file_type_size_in_blocks(0) + saved_film_size_in_blocks() + 1;
}

void function_236299(long sound);
void __stdcall function_148b27(long index);

/* stops editing the game variant: a built-in one only plays a sound */
// @retail 0x148a2c
void function_148a2c()
{
	if (g_54e49c != NONE)
	{
		if (!(bool)(((dword)g_54e49c >> 21) & 1))
		{
			function_148b27(g_54e49c);
		}
		else
		{
			function_236299(2);
		}
	}
	g_54e49c = NONE;
}

/* the same for the player profile */
// @retail 0x148afb
void function_148afb()
{
	if (g_54e5d0.profile_index != NONE)
	{
		if (!(bool)(((dword)g_54e5d0.profile_index >> 21) & 1))
		{
			function_148b27(g_54e5d0.profile_index);
		}
		else
		{
			function_236299(2);
		}
	}
	g_54e5d0.profile_index = NONE;
}

void function_121040(long value);

// @retail 0x148cfc
void function_148cfc(long value)
{
	g_54d598.m1220 = true;
	g_54d598.m1224 = value;
	function_121040(value);
}

// @retail 0x148d42
void function_148d42(long value)
{
	if (value < 0)
	{
		g_54d598.m1230 = 0;
	}
	else
	{
		g_54d598.m1230 = value > 3 ? 3 : value;
	}
}

struct s_entry_a;
s_entry_a *function_19c270(long key0, long key1);

// @retail 0x148d61
s_entry_a *function_148d61()
{
	return function_19c270(g_54d598.m1228, g_54d598.m122c);
}

// @retail 0x148d73
char *function_148d73()
{
	char *result = 0;
	byte *entry = (byte *)function_148d61();
	char *name = (char *)(entry + 8);

	if (entry && name && *name)
	{
		result = name;
	}
	return result;
}

// @retail 0x148bff
void profile_edit_end()
{
	profile_edit_save();
	memset(&g_54e5d0.settings, 0, sizeof(g_54e5d0.settings));
	g_54e5d0.player = NONE;
	g_54e5d0.profile_index = NONE;
}
