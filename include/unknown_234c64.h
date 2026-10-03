/* UNKNOWN_234C64.H: the window manager's screen channels (the vtables at
   0x45997c, 0x4599a8, 0x4599dc, 0x459a08 and 0x459a34) */

#ifndef UNKNOWN_234C64_H
#define UNKNOWN_234C64_H

#include "cseries.h"
#include "real_math.h"
#include "screen_widget.h"

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
	virtual void set_next(c_screen_widget *screen, s_screen_request *request);
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
	virtual void set_next(c_screen_widget *screen, s_screen_request *request);
	virtual void v7();
	virtual void v9();
	virtual void v11(short count);
	virtual short v12(long window);

	s_queued_request *queue;
	bool m3c;
	bool m3d;
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

struct s_screen_sort_entry
{
	c_screen_widget *screen;
	real depth;

	short layer;
};

/* the user interface globals (0x148350): the screen tint */
struct s_user_interface_globals
{
	byte unknown00[0x6c];
	real_argb_color tint;
};

/* the render window passed down to the screens */
class c_render_window
{
public:
	void function_147cdb(dword color);
};

#endif
