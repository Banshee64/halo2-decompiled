// @flags /O1 /Oi /arch:SSE /Gr
/* UNKNOWN_234C64.CPP: the window manager's screen channels: each holds the
   current screen, the next one, the previous one and a pending request */

#include "cseries.h"
#include <new>
#include <string.h>
#include <stdlib.h>
#include "unknown_234c64.h"

/* ---- globals ---- */

byte g_54d5a8;
byte g_4670cd;
extern dword g_54d5b8;
real g_54d5ac;
real g_54d5b0;
real g_54d5b4;

/* ---- externals ---- */

bool function_6c7e0();
long function_147f4f(void);
s_user_interface_globals *function_148350(void);
long function_199d7c(void);
word function_19022f(void);
c_screen_widget *__stdcall function_2b72e6(s_screen_parameters *request);
c_screen_widget *__stdcall function_2b7333(s_screen_parameters *request);
bool function_23029a(c_screen_widget *screen);
real function_230374(c_screen_widget *screen);
void function_235756(real fade);
void function_2359ce(c_window_channel_459a34 *channel);
void function_235abc(c_window_channel_459a34 *channel);

void function_234dd1(c_window_channel *channel);
bool function_235246(c_window_channel *channel);
void function_2353a5(c_screen_widget *screen, long window);

#define REAL_TO_LONG(x, result) __asm { fld x } __asm { fistp result }

/* ---- the base channel ---- */

// @retail 0x234e43
c_window_channel::c_window_channel() :
	m4(4),
	current(0),
	next(0),
	previous(0),
	focus(0)
{
}

// @retail 0x234e64
void c_window_channel::clear()
{
	current = 0;
	next = 0;
	memset(&request, 0, sizeof(request));
	previous = 0;
	focus = 0;
}

// @retail 0x234dc7
void c_window_channel::reset()
{
	dispose();
}

// @retail 0x234e81
void c_window_channel::dispose()
{
	if (current)
	{
		function_148148(current);
		current = 0;
	}
	if (next)
	{
		function_148148(next);
		next = 0;
	}
	if (previous)
	{
		function_148148(previous);
		previous = 0;
	}
	focus = 0;
}

// @retail 0x234ebc
void c_window_channel::update()
{
	if (next)
	{
		bool modal = TEST_FIELD_BIT(request.type_bit0);
		next->v17();
		((c_widget *)next)->function_22ecb4(true);
		next->v18(&request);
		next->v19();
		if (modal)
		{
			next->function_22e957(1);
			next->v25((s_screen_focus *)&request.id);
		}
		else
		{
			next->function_22e957(0);
		}
		if (current)
		{
			if (previous)
				function_148148(previous);
			if (current)
			{
				previous = current;
				current->function_22e957(modal ? 2 : 3);
			}
		}
		current = next;
		next = 0;
		if (!current->is_in_window())
			((c_widget *)current)->function_22ecb4(true);
	}
	if (next)
		next->v3();
	if (current)
		current->v3();
	if (previous)
		previous->v3();
}

// @retail 0x234f8c
void c_window_channel::render(long window)
{
	s_user_interface_globals *globals = function_148350();
	real fade = 0.f;
	bool fading = false;
	short mode = g_54d5a8;

	if (previous && function_23029a(previous))
	{
		fade = function_230374(previous);
		fading = true;
	}
	if (current && function_23029a(current))
	{
		fade += function_230374(current);
		fading = true;
	}
	if (next && function_23029a(next))
	{
		fade += function_230374(next);
		fading = true;
	}

	bool tint = fading;
	if (mode >= 0 && mode <= 1)
	{
		if (!tint && current)
		{
			fade += function_230374(current);
			tint = current->screen_id != 0xd4;
		}
		fading = false;
	}

	if (fade < 0.f)
		fade = 0.f;
	else if (fade > 1.f)
		fade = 1.f;

	dword color;
	if (globals)
	{
		long alpha, red, green, blue;
		real value;
		value = globals->tint.alpha * fade * 255.f;
		REAL_TO_LONG(value, alpha);
		value = globals->tint.red * 255.f;
		REAL_TO_LONG(value, red);
		value = globals->tint.green * 255.f;
		REAL_TO_LONG(value, green);
		value = globals->tint.blue * 255.f;
		REAL_TO_LONG(value, blue);
		color = (((alpha << 8 | red) << 8 | green) << 8) | blue;
	}
	else
	{
		long alpha;
		real value = (real)(fade * 191.25);
		REAL_TO_LONG(value, alpha);
		color = alpha << 24 | 0xf;
	}

	if (tint)
		((c_render_window *)window)->function_147cdb(color);
	if (fading)
		function_235756(fade);
	if (previous)
		function_2353a5(previous, window);
	if (current)
		function_2353a5(current, window);
	if (next)
		function_2353a5(next, window);
}

// @retail 0x23519d
void c_window_channel::set_next(c_screen_widget *screen, s_screen_parameters *new_request)
{
	if (next == screen)
		next = 0;
	if (next)
	{
		function_148148(next);
		next = 0;
	}
	next = screen;
	(s_screen_parameters &)request = *new_request;
}

// @retail 0x2351d4
void c_window_channel::v7()
{
	if (!TEST_FIELD_BIT(current->animation.flags.flag1))
		current->function_22e957(2);
}

// @retail 0x2351ea
void c_window_channel::remove(c_screen_widget *screen)
{
	if (screen == current)
		current = 0;
	if (screen == next)
		next = 0;
	if (screen == previous)
		previous = 0;
	if (focus && (focus == screen || focus->get_screen() == screen))
		focus = 0;
	if (screen->value5f2)
		screen->v2();
	screen->~c_screen_widget();
	user_interface_free(screen);
}

// @retail 0x2352d3
void c_window_channel::v9()
{
	if (current)
	{
		function_149f49((s_message *)&request, 0x7fff, 0, current->user_flags, current->v20(), current->v21(), (long)current->get_load_proc());
	}
	dispose();
}

// @retail 0x23531c
void c_window_channel::v10()
{
	if (request.type == 0x7fff)
	{
		s_screen_request new_request;
		request.type = 0;
		function_149f49((s_message *)&new_request, 0, (dword *)&request.id, request.user_flags, request.a, request.b, (long)request.load);
		set_next(request.load(&new_request), &new_request);
	}
}

/* ---- helpers the window manager calls ---- */

// @retail 0x23515d
bool function_23515d(c_window_channel *channel, s_event *event)
{
	if (!function_235246(channel))
		return true;

	c_user_interface_widget *screen = channel->focus;
	if (screen)
	{
		while (!((1 << ((long *)event)[1]) & screen->user_flags))
		{
			screen = screen->next;
			if (!screen)
				break;
		}
		if (screen)
			return screen->v10((s_widget_event *)event);
	}
	return false;
}

// @retail 0x235246
bool function_235246(c_window_channel *channel)
{
	bool result = false;
	if (channel->focus)
	{
		c_screen_widget *root = channel->focus->get_screen();
		if (root && root == channel->current && !TEST_FIELD_BIT(root->animation.flags.flag1) && !TEST_FIELD_BIT(root->animation.flags.flag0))
			result = true;
	}
	return result;
}

// @retail 0x235276
bool function_235276(c_window_channel *channel, long index)
{
	c_screen_widget *focus = channel->focus;
	bool result = false;
	if (focus)
		result = ((1 << index) & focus->user_flags) != 0;
	return result;
}

// @retail 0x235294
bool function_235294(c_window_channel *channel, long index)
{
	c_screen_widget *focus = channel->focus;
	bool result = false;
	if (focus)
	{
		short mask = focus->user_flags;
		if (mask != NONE && ((1 << index) & mask))
			result = true;
		else
			result = false;
	}
	return result;
}

// @retail 0x2352c0
void function_2352c0(c_window_channel *channel)
{
	if (channel->previous)
	{
		function_148148(channel->previous);
		channel->previous = 0;
	}
}

// @retail 0x23536a
void function_23536a(c_window_channel *channel, c_screen_widget *screen)
{
	if (screen)
	{
		c_screen_widget *root = screen->get_screen();
		if (root && (root == channel->current || root == channel->next) || !screen->type)
			channel->focus = screen;
	}
}

// @retail 0x23538b
void function_23538b(c_window_channel *channel)
{
	c_screen_widget *screen = channel->current;
	if (screen && !TEST_FIELD_BIT(screen->animation.flags.flag1))
		screen->function_22e957(3);
}

/* ---- the transition channel ---- */

// @retail 0x234c64
c_window_channel_45997c::c_window_channel_45997c()
{
	state = NONE;
	m60 = 250;
	memset(&m38, 0, 0x28);
}

// @retail 0x234c8b
void c_window_channel_45997c::clear()
{
	memset(&m38, 0, 0x28);
	state = NONE;
	c_window_channel::clear();
}

// @retail 0x234ca4
void c_window_channel_45997c::v2()
{
	function_234dd1(this);
	state = NONE;
}

// @retail 0x234cb3
void c_window_channel_45997c::dispose()
{
	c_window_channel::dispose();
	memset(&m38, 0, 0x28);
	state = NONE;
}

// @retail 0x234ccd
void c_window_channel_45997c::update()
{
	if (g_54d5a8 == 2 && !current)
		function_234dd1(this);

	c_screen_widget *screen = current;
	if (screen)
	{
		long new_state;
		long id = function_147f4f();
		if (id == NONE)
			new_state = 0;
		else if (id == 6)
			new_state = 1;
		else if (id == 9)
			new_state = 0;
		else if (id == 11 || id == 19 || id == 30 || id == 186)
			new_state = 2;
		else
			new_state = 3;

		if (state != new_state)
		{
			real value;
			switch (new_state)
			{
			case 0:
				m3c = 0.f;
				value = 0.f;
				break;
			case 1:
				if (state >= 1)
				{
					m3c = 1.f;
					value = 0.f;
				}
				else
				{
					m3c = 0.f;
					value = 0.f;
				}
				break;
			case 2:
				if (state >= 2)
				{
					m3c = 1.f;
					value = 1.f;
				}
				else
				{
					m3c = 0.f;
					value = 1.f;
				}
				break;
			case 3:
				m3c = 1.f;
				value = 1.f;
				break;
			default:
				m3c = 0.f;
				value = 0.f;
				break;
			}
			m50 = value;
			screen->function_22e957(4);
			state = new_state;
		}
	}
	c_window_channel::update();
}

// @retail 0x234dcc
void c_window_channel_45997c::v10()
{
	v2();
}

// @retail 0x234dd1
void function_234dd1(c_window_channel *channel)
{
	s_screen_request request;

	request.type = 0;
	request.user_flags = 0;
	request.a = 6;
	request.b = 4;
	memset(&request.id, NONE, sizeof(request.id));
	request.load = 0;
	if (g_54d5a8 == 2)
	{
		request.load = function_2b72e6;
		if (request.load)
		{
			channel->current = request.load(&request);
			channel->current->v18(&request);
		}
	}
}

/* ---- the queued channel ---- */

// @retail 0x23546b
c_window_channel_4599a8::c_window_channel_4599a8()
{
	queue = 0;
	m3c = false;
	m3d = false;
}

// @retail 0x234e33
c_window_channel_234e33::c_window_channel_234e33()
{
}

// @retail 0x235486
void c_window_channel_4599a8::clear()
{
	queue = 0;
	c_window_channel::clear();
}

// @retail 0x235626
void function_235626(c_window_channel_4599a8 *channel, s_screen_parameters *request)
{
	s_queued_request *queued = channel->queue;
	*request = queued->request;
	channel->queue = queued->next;
	user_interface_free(queued);
}

// @retail 0x235647
void function_235647(c_window_channel_4599a8 *channel)
{
	s_screen_request request;
	while (channel->queue)
		function_235626(channel, &request);
}

// @retail 0x2355ed
void function_2355ed(c_window_channel_4599a8 *channel, s_screen_parameters *request, long window)
{
	s_queued_request *queued = new (user_interface_malloc(sizeof(s_queued_request))) s_queued_request;
	if (queued)
	{
		(s_screen_parameters &)queued->request = *request;
		queued->next = channel->queue;
		queued->window = window;
		channel->queue = queued;
	}
}

// @retail 0x23548f
void c_window_channel_4599a8::dispose()
{
	if (!m3d)
		function_235647(this);
	c_window_channel::dispose();
	m3d = false;
}

// @retail 0x2354aa
void c_window_channel_4599a8::set_next(c_screen_widget *screen, s_screen_parameters *new_request)
{
	if (current && !(new_request->type & 6))
	{
		s_screen_request previous_request;
		function_149f49((s_message *)&previous_request, 2, 0, current->user_flags, current->v20(), current->v21(), (long)current->get_load_proc());
		current->v24((s_screen_focus *)&previous_request.id);
		function_2355ed(this, &previous_request, current->screen_id);
	}
	if (new_request->type & 4)
		function_235647(this);
	c_window_channel::set_next(screen, new_request);
}

// @retail 0x235537
void c_window_channel_4599a8::v7()
{
	v11(1);
}

// @retail 0x23553f
void c_window_channel_4599a8::v11(short count)
{
	s_screen_request request;

	if (count > 1)
	{
		for (word i = count - 1; i != 0; i--)
		{
			if (queue)
				function_235626(this, &request);
		}
	}
	if (queue)
	{
		function_235626(this, &request);
		request.type |= 3;
		request.load(&request);
	}
	else if (m3c && current)
	{
		if (previous)
		{
			function_148148(previous);
			previous = 0;
		}
		if (current)
		{
			previous = current;
			current->function_22e957(2);
		}
	}
}

// @retail 0x2355bd
short c_window_channel_4599a8::v12(long window)
{
	short index = 1;
	s_queued_request *queued;
	for (queued = queue; queued; queued = queued->next, index++)
	{
		if (queued->window == window)
			break;
	}
	if (queued)
		v11(index);
	else
		index = 0;
	return index;
}

// @retail 0x235661
void c_window_channel_4599a8::v9()
{
	m3d = true;
	c_window_channel::v9();
}

/* ---- 0x459a08 ---- */

// @retail 0x235795
c_window_channel_459a08::c_window_channel_459a08()
{
	memset(&m38, 0, 8);
	memset(&m38, 0, 8);
}

// @retail 0x2357b6
void c_window_channel_459a08::update()
{
	if (m38)
		memset(&m38, 0, 8);
	c_window_channel::update();
}

/* ---- 0x459a34 ---- */

/* the screen transition's animation keys (none for NONE) */
// @retail 0x234d9e
void *c_window_channel_45997c::get_transition(short index, long *value, short *count, short *frames)
{
	void *result = 0;

	if (index == NONE)
	{
		*value = 0;
		*count = 0;
		*frames = 0;
	}
	else
	{
		*value = m60;
		*count = 1;
		*frames = 2;
		result = &m38;
	}
	return result;
}

// @retail 0x2357c9
c_window_channel_2357c9::c_window_channel_2357c9()
{
}

// @retail 0x2357d9
c_window_channel_459a34::c_window_channel_459a34()
{
	m38 = 0;
	m3c = 0;
	m1c0 = 0;
	memset(slots, 0, sizeof(slots));
}

// @retail 0x235906
void function_235906(c_window_channel_459a34 *channel)
{
	if (channel->m38)
	{
		channel->remove(channel->m38);
		channel->m38 = 0;
	}
}

// @retail 0x2359a9
void function_2359a9(c_window_channel_459a34 *channel)
{
	if (channel->m3c)
	{
		channel->remove(channel->m3c);
		memset(channel->slots, 0, sizeof(channel->slots));
		channel->m3c = 0;
		channel->m1c0 = 0;
	}
}

// @retail 0x235801
void c_window_channel_459a34::dispose()
{
	function_235906(this);
	function_2359a9(this);
	c_window_channel::dispose();
}

// @retail 0x2358c3
void function_2358c3(c_window_channel_459a34 *channel)
{
	s_screen_request request;
	function_149f49((s_message *)&request, 0, 0, function_19022f(), 4, 4, (long)function_2b7333);
	c_screen_widget *screen = request.load(&request);
	channel->m38 = screen;
	if (screen)
		screen->function_22e957(0);
}

// @retail 0x23586f
void function_23586f(c_window_channel_459a34 *channel)
{
	if (!function_1473b6(channel) && function_6c7e0() && !g_4670cd &&
		function_199d7c() != 6 && function_199d7c() != 7)
	{
		if (!channel->m38)
		{
			function_2358c3(channel);
			if (!channel->m38)
				return;
		}
		channel->m38->v3();
		return;
	}
	function_235906(channel);
}

// @retail 0x235816
void c_window_channel_459a34::update()
{
	if (g_54d5a8 == 2)
		function_23586f(this);
	function_2359ce(this);
	function_235abc(this);
	c_window_channel::update();
}

// @retail 0x23583e
void c_window_channel_459a34::render(long window)
{
	c_window_channel::render(window);
	if (m38)
		function_2353a5(m38, window);
	if (m3c)
		function_2353a5(m3c, window);
}

/* ---- drawing the screens back to front ---- */

// @retail 0x23566a
void function_23566a(c_user_interface_widget *screen, s_screen_sort_entry *entries, long *count)
{
	if (screen->v16())
	{
		for (c_user_interface_widget *child = screen->child; child; child = child->next)
			function_23566a(child, entries, count);
		if (*count < 256)
		{
			entries[*count].screen = screen;
			entries[*count].depth = function_22e9aa((c_widget *)screen);
			entries[*count].layer = screen->value6a;
			(*count)++;
		}
	}
}

// @retail 0x2356d8
int __cdecl function_2356d8(void const *a, void const *b)
{
	s_screen_sort_entry const *x = (s_screen_sort_entry const *)a;
	s_screen_sort_entry const *y = (s_screen_sort_entry const *)b;
	real difference = x->depth - y->depth;
	real distance = difference >= 0.f ? difference : 0.f - difference;

	if (distance <= 0.001f)
	{
		if (x->layer > y->layer)
			return 1;
		if (x->layer < y->layer)
			return -1;
		if (x->screen->type < y->screen->type)
			return 1;
		return x->screen->type > y->screen->type ? -1 : 0;
	}
	if (y->depth > x->depth)
		return 1;
	if (x->depth > y->depth)
		return -1;
	return 0;
}

// @retail 0x2353a5
void function_2353a5(c_screen_widget *screen, long window)
{
	if (!g_4670cd)
	{
		s_screen_sort_entry entries[256];
		long count = 0;

		function_23566a(screen, entries, &count);
		if (count > 0)
			qsort(entries, count, sizeof(s_screen_sort_entry), function_2356d8);
		screen->v22((void *)window);

		real nearest = g_54d5ac - g_54d5b0;
		real farthest = g_54d5b4 - g_54d5b0;
		for (long i = 0; i < count; i++)
		{
			if (entries[i].depth >= nearest && farthest >= entries[i].depth)
				entries[i].screen->v4(window);
		}
		screen->v23((void *)window);
	}
}
