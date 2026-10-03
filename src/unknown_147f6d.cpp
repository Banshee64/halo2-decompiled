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

/* the window manager's channels (unknown_234c64.h), at the offsets its
   constructor 0x147645 gives them: 0x54d884's five are constructed by
   0x2357c9, a channel class not written yet */
c_window_channel_45997c g_54d5c4;
c_window_channel_234e33 g_54d62c[5];
c_window_channel g_54d76c[5];
c_window_channel g_54d884[5];
c_window_channel_459a08 g_54d99c;
c_window_channel_459a34 g_54d9e0;
c_window_channel g_54dba8;

/* the screen settings copied out by function_14887e (0x78 bytes) */
struct s_screen_settings_54dc6c
{
	dword data[0x1e];
};

s_screen_settings_54dc6c g_54dc6c;
long g_47ff54;

void *function_1482e8(void);

void function_236299(long sound);

s_profile_edit g_54e5d0;

struct s_player_profile; /* unknown_18f576.cpp */
void player_slot_get_profile(long index, s_player_profile *profile, long *profile_index);
void __stdcall function_18fd20(long player, s_player_profile_settings *settings, long profile_index);
void __stdcall function_2153dd(long player, long profile_index, s_player_profile_settings *settings, long flags);

// @retail 0x147f6d
void c_screen_widget::function_147f6d(s_screen_parameters *parameters)
{
	long index = v21();
	c_window_channel *window;

	switch (parameters->a)
	{
	case 0:
		window = index == 4 ? &g_54d99c : 0;
		break;
	case 1:
		window = &g_54d884[index];
		break;
	case 2:
		window = index == 4 ? &g_54dba8 : 0;
		break;
	case 3:
		window = &g_54d76c[index];
		break;
	case 4:
		window = index == 4 ? &g_54d9e0 : 0;
		break;
	case 5:
		window = &g_54d62c[index];
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

struct s_screen_definition
{
	byte unknown00[4];
	short screen_id;
};

// @retail 0x147f4f
long function_147f4f()
{
	long result = NONE;

	if (g_54d62c[4].focus)
	{
		c_screen_widget *screen = g_54d62c[4].focus->get_screen();
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
		return index == 4 ? &g_54d99c : 0;
	case 1:
		return &g_54d884[index];
	case 2:
		return index == 4 ? &g_54dba8 : 0;
	case 3:
		return &g_54d76c[index];
	case 4:
		return index == 4 ? &g_54d9e0 : 0;
	case 5:
		return &g_54d62c[index];
	}
	return &g_54d5c4;
}

// @retail 0x14887e
void function_14887e(s_screen_settings_54dc6c *settings)
{
	if (settings)
	{
		*settings = g_54dc6c;
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
		window = index == 4 ? &g_54d99c : 0;
		break;
	case 1:
		window = &g_54d884[index];
		break;
	case 2:
		window = index == 4 ? &g_54dba8 : 0;
		break;
	case 3:
		window = &g_54d76c[index];
		break;
	case 4:
		window = index == 4 ? &g_54d9e0 : 0;
		break;
	case 5:
		window = &g_54d62c[index];
		break;
	default:
		window = &g_54d5c4;
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
		g_54dba8.v7();
		break;
	case 3:
		window = &g_54d76c[index];
		window->v7();
		break;
	case 5:
		window = &g_54d62c[index];
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
		g_54dba8.v7();
		break;
	case 3:
		window = &g_54d76c[index];
		window->v7();
		break;
	case 5:
		window = &g_54d62c[index];
		result = ((c_window_channel_4599a8 *)window)->v12(value) > 0;
		break;
	}
	if (result)
	{
		function_236299(4);
	}
	return result;
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
// @retail 0x148bff
void profile_edit_end()
{
	profile_edit_save();
	memset(&g_54e5d0.settings, 0, sizeof(g_54e5d0.settings));
	g_54e5d0.player = NONE;
	g_54e5d0.profile_index = NONE;
}
