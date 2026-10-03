/* UNKNOWN_234C64.H: the window manager's screen channels (the vtables at
   0x45997c, 0x4599a8, 0x4599dc, 0x459a08 and 0x459a34). The window manager
   at 0x54d598 (constructed by 0x147645) holds them: see unknown_147f6d.cpp. */

#ifndef UNKNOWN_234C64_H
#define UNKNOWN_234C64_H

#include "cseries.h"
#include "real_math.h"
#include "screen_widgets.h"
#include "unknown_19b516.h"

/* a screen request as the channels keep and build them: the parameters,
   with field_c cleared on construction */
struct s_screen_request : s_screen_parameters
{
	s_screen_request()
	{
		field_c = 0;
	}
};

class c_window_channel
{
public:
	c_window_channel();

	virtual void clear();
	virtual void reset();
	virtual void v2() {}
	virtual void dispose();
	virtual void update();
	virtual void render(long window);
	virtual void set_next(c_screen_widget *screen, s_screen_parameters *request);
	virtual void v7();
	virtual void remove(c_screen_widget *screen);
	virtual void v9();
	virtual void v10();

	long m4;
	c_screen_widget *current;
	c_screen_widget *next;
	s_screen_request request;
	c_screen_widget *previous;
	c_screen_widget *focus;
};

/* whether a window has a screen or one coming (unknown_1473b6.cpp) */
bool function_1473b6(c_window_channel *window);

/* the channel with a transition state (0x45997c) */
class c_window_channel_45997c : public c_window_channel
{
public:
	c_window_channel_45997c();

	virtual void clear();
	virtual void v2();
	virtual void dispose();
	virtual void update();
	virtual void v10();

	void *get_transition(short index, long *value, short *count, short *frames);

	long m38;
	real m3c;
	byte unknown40[0x10];
	real m50;
	byte unknown54[0xc];
	long m60;
	long state;
};

/* a queued request (0x28 bytes, from the user interface allocator) */
struct s_queued_request
{
	s_queued_request *next;
	s_screen_request request;
	long window;
};

/* the channel with a queue of requests (0x4599a8) */
class c_window_channel_4599a8 : public c_window_channel
{
public:
	c_window_channel_4599a8();

	virtual void clear();
	virtual void dispose();
	virtual void set_next(c_screen_widget *screen, s_screen_parameters *request);
	virtual void v7();
	virtual void v9();
	virtual void v11(short count);
	virtual short v12(long window);

	s_queued_request *queue;
	bool m3c;
	bool m3d;
};

/* a channel derived from the queued one without overrides (its vtable is
   identical to 0x4599a8): the window manager's five at 0x54d62c */
class c_window_channel_234e33 : public c_window_channel_4599a8
{
public:
	c_window_channel_234e33();
};

/* 0x459a08 */
class c_window_channel_459a08 : public c_window_channel
{
public:
	c_window_channel_459a08();

	virtual void update();

	long m38;
	long m3c;
};

struct s_channel_slot
{
	dword time;
	byte unknown04[0x5c];
};

/* 0x459a34 */
class c_window_channel_459a34 : public c_window_channel
{
public:
	c_window_channel_459a34();

	virtual void dispose();
	virtual void update();
	virtual void render(long window);

	c_screen_widget *m38;
	c_screen_widget *m3c;
	s_channel_slot slots[4];
	dword m1c0;
};

/* a channel with the base's vtable (0x4599dc, folded with the base's): the
   window manager's five windows of channel 1 */
class c_window_channel_2357c9 : public c_window_channel
{
public:
	c_window_channel_2357c9();
};

/* the screen settings copied out by function_14887e (0x78 bytes) */
struct s_screen_settings_54dc6c
{
	dword data[0x1e];
};

struct s_window_manager_754
{
	word data[0x6a2 / 2];
};

struct s_window_manager_df6
{
	word data[0x92 / 2];
};

struct s_window_manager_e94
{
	dword data[0x1c];
};

struct s_window_manager_1248
{
	s_window_manager_1248()
	{
		m10 = NONE;
		m0 = 0;
		m4 = 0;
		mc = 0;
		m8 = 0;
	}

	long m0;
	long m4;
	long m8;
	long mc;
	long m10;
};

/* the window manager (0x54d598, constructed by 0x147645; unknown_147f6d.cpp):
   the windows of each channel, three per-controller arrays of five (the
   fifth, index 4, is shared by all controllers) and three single windows
   that only take index 4. function_148262 maps a channel and an index to one
   of them. The profile being edited (g_54e5d0, at +0x1038) is defined on its
   own. */
class c_window_manager
{
public:
	c_window_manager();

	byte unknown00[4];
	bool active;
	byte unknown05[0x2c - 0x05];
	c_window_channel_45997c default_window;
	c_window_channel_234e33 windows_5[5];
	c_window_channel windows_3[5];
	c_window_channel_2357c9 windows_1[5];
	c_window_channel_459a08 window_0;
	byte unknown444[4];
	c_window_channel_459a34 window_4;
	byte unknown60c[4];
	c_window_channel window_2;
	c_screen_widget *screens[0x23];
	s_screen_settings_54dc6c settings;
	byte unknown74c[0x754 - 0x74c];
	s_window_manager_754 m754;
	s_window_manager_df6 mdf6;
	byte unknowne88[0xe94 - 0xe88];
	s_window_manager_e94 me94;
	long mf04;
	byte mf08[0x130];
	/* the profile being edited (g_54e5d0) */
	byte unknown1038[0x1220 - 0x1038];
	bool m1220;
	byte unknown1221[3];
	long m1224;
	long m1228;
	long m122c;
	long m1230;
	byte unknown1234[0x1248 - 0x1234];
	s_window_manager_1248 m1248;
};

extern c_window_manager g_54d598;

struct s_screen_sort_entry
{
	c_user_interface_widget *screen;
	real depth;

	short layer;
};

/* the user interface globals (0x148350): the screen tint */
struct s_tag_reference_8
{
	dword group_tag;
	long tag_index;
};

/* a widget animation of the user interface globals (0x2c bytes): the
   keyframes of each animation type */
struct s_widget_animation_keys
{
	long value;
	short frames;
	short pad;
	long target;
};

struct s_widget_animation_definition
{
	byte unknown00[4];
	s_widget_animation_keys a;
	s_widget_animation_keys b;
	long value1c;
	short mode;
	short pad22;
	short frames24;
	short pad26;
	long target28;
};

struct s_user_interface_globals
{
	byte unknown00[0x6c];
	real_argb_color tint;
	byte unknown7c[0x120 - 0x7c];
	long animation_count;
	s_widget_animation_definition *animations;
	byte unknown128[0x138 - 0x128];
	long skin_count;
	s_tag_reference_8 *skins;
};

/* the user interface globals tag (unknown_1482e8.cpp) */
s_user_interface_globals *function_148350(void);

/* the render window passed down to the screens */
class c_render_window
{
public:
	void function_147cdb(dword color);
};

#endif
