// @flags /O1 /Oi /Gr
/* UNKNOWN_1A2CA7.CPP: the screen that waits for an online task (vtable
   0x4549c0: it shows the task's title and description and calls back when
   the task finishes or is cancelled), and the friends list globals */

#include "cseries.h"
#include <xtl.h>
#include <xonline.h>
#include <string.h>
#include "data_array.h"
#include "screen_widgets.h"
#include "unknown_19b516.h"
#include "online_tasks.h"

class c_online_task_screen;
typedef void (__stdcall *online_task_screen_callback)(c_online_task_screen *screen);

/* the screen (0x624 bytes, screen id 0xb6) */
class c_online_task_screen : public c_screen_widget
{
public:
	c_online_task_screen(long a, long b, word user_flags);
	virtual ~c_online_task_screen();
	virtual void v3();
	virtual bool v10(s_widget_event *event);
	virtual screen_load_proc get_load_proc();

	void set_title(long string_id);
	void set_description(long string_id);

	online_task_screen_callback finished;
	online_task_screen_callback cancelled;
	byte unknown618[4];
	long task_index;
	HRESULT result;
};

/* the friends list: the data arrays of friends and of players, the task that
   fills them, and the last friend request (0x6a2 bytes) and whether it is
   valid */
struct s_friend_request
{
	byte data[0x6a2];
};

struct s_friend_request_globals
{
	s_friend_request request;
	bool valid;
	bool unknown6a3;
};

/* an iteration over a data array keeping the current item (unknown_2b116a.cpp) */
struct s_list_item_iterator
{
	byte *item;
	s_data_iterator iterator;
};

extern s_data_array *g_46e7bc;
extern s_data_array *g_46e7c0;
long g_46e7d4 = NONE;
s_friend_request_globals g_46e814;
byte g_54eae8[4][0xc70];

long function_1a2c81(c_user_interface_widget *widget);
void function_18fe9e(long gamepad_index);
void function_190728(long index);
void __stdcall function_19b527(long a, dword b, long c, word d, long e, long f);
void function_24bac5(void *a);
bool function_2b2327(s_list_item_iterator *iterator);
bool xuid_equal(XUID const *a, XUID const *b, bool compare_guest_number);
void online_get_title_name(DWORD title_id, WCHAR *name, long name_length);
HRESULT online_task_continue(s_online_task *task);
void online_task_dispose(long task_index);
bool function_6c7e0();
dword function_0b4a20(dword key);
c_screen_widget *__stdcall online_task_screen_load(s_screen_parameters *parameters);
void function_1a3294();

/* ---- the screen ---- */

// @retail 0x1a2f70
void online_task_screen_dispose_task(c_online_task_screen *screen)
{
	if (screen->task_index != NONE)
	{
		s_online_task *task = online_task_get(screen->task_index);

		if (task)
			screen->result = online_task_continue(task);
		else
			screen->result = E_FAIL;
		online_task_dispose(screen->task_index);
		screen->task_index = NONE;
	}
}

// @retail 0x1a2fb4
void online_task_screen_finish(c_online_task_screen *screen)
{
	screen->finished = NULL;
	online_task_screen_dispose_task(screen);
}

// @retail 0x1a2d8a
void online_task_screen_end(c_online_task_screen *screen, bool show_error)
{
	bool failed = false;

	if (screen->task_index != NONE)
	{
		switch (online_task_get_status(screen->task_index))
		{
		case 2:
			break;
		case 3:
		case 4:
		case 5:
			failed = true;
			break;
		default:
			return;
		}
	}
	online_task_screen_finish(screen);
	if (failed && show_error)
		function_19b527(1, function_0b4a20(screen->result), 4, screen->user_flags, 0, 0);
}

// @retail 0x1a2ca7
void __stdcall online_task_screen_end_with_error(c_online_task_screen *screen)
{
	online_task_screen_end(screen, true);
}

// @retail 0x1a2cb7
void __stdcall function_1a2cb7(c_online_task_screen *screen)
{
	bool failed = false;

	if (screen->task_index != NONE)
	{
		switch (online_task_get_status(screen->task_index))
		{
		case 2:
			break;
		case 3:
		case 4:
		case 5:
			failed = true;
			break;
		default:
			return;
		}
	}
	online_task_screen_finish(screen);
	if (failed)
		function_19b527(1, function_0b4a20(screen->result), 4, screen->user_flags, 0, 0);
	function_190728(function_1a2c81(screen));
	function_1a3294();
	function_18fe9e(function_1a2c81(screen));
}

// @retail 0x1a2d2f
void __stdcall function_1a2d2f(c_online_task_screen *screen)
{
	bool failed = false;

	if (screen->task_index != NONE)
	{
		switch (online_task_get_status(screen->task_index))
		{
		case 2:
			break;
		case 3:
		case 4:
		case 5:
			failed = true;
			break;
		default:
			return;
		}
	}
	online_task_screen_finish(screen);
	if (failed)
		function_19b527(1, function_0b4a20(screen->result), 4, screen->user_flags, 0, 0);
	function_1a3294();
}

// @retail 0x1a2de1
screen_load_proc c_online_task_screen::get_load_proc()
{
	return online_task_screen_load;
}

// @retail 0x1a2de7
c_screen_widget *__stdcall online_task_screen_load(s_screen_parameters *parameters)
{
	c_online_task_screen *screen = new c_online_task_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x1a2e23
c_online_task_screen::c_online_task_screen(long a, long b, word user_flags) :
	c_screen_widget(0xb6, a, b, user_flags)
{
	task_index = NONE;
	finished = NULL;
	cancelled = NULL;
	result = S_OK;
}

// @retail 0x1a2e63 deleting c_online_task_screen

// @retail 0x1a2e7f
c_online_task_screen::~c_online_task_screen()
{
	if (function_6c7e0() && task_index != NONE)
	{
		s_online_task *task = online_task_get(task_index);

		if (task && task->type == 1)
		{
			long index = function_1a2c81(this);

			if (index >= 0 && index <= 3)
				function_24bac5(g_54eae8[index]);
		}
	}
	online_task_screen_dispose_task(this);
}

// @retail 0x1a2edb
bool c_online_task_screen::v10(s_widget_event *event)
{
	if (event->type == 5 && (event->param == 1 || event->param == 13) && cancelled)
	{
		cancelled(this);
		online_task_screen_finish(this);
		return true;
	}
	return false;
}

// @retail 0x1a2f12
void c_online_task_screen::v3()
{
	if (task_index != NONE)
	{
		set_title(online_task_get_title(task_index));
		set_description(online_task_get_description(task_index));
	}
	if (finished)
	{
		finished(this);
	}
	else if (!TEST_FIELD_BIT(animation.flags.flag1))
	{
		online_task_screen_dispose_task(this);
		function_22e957(3);
	}
	((c_widget *)this)->function_22e391();
}

// @retail 0x1a2fc5
void c_online_task_screen::set_title(long string_id)
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)find_child(6, 0, false);

	if (text)
		text->set_string(string_id);
}

// @retail 0x1a2fe4
void c_online_task_screen::set_description(long string_id)
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)find_child(6, 2, false);

	if (text)
		text->set_string(string_id);
}

/* ---- the friends list ---- */

// @retail 0x1a3230
bool friends_list_task_running()
{
	bool result = false;

	if (g_46e7d4 != NONE && g_46e7bc)
	{
		long status = online_task_get_status(g_46e7d4);

		if (status == 1 || status == 2)
			result = true;
	}
	return result;
}

// @retail 0x1a325a
bool function_1a325a()
{
	return g_46e814.valid ? g_46e814.unknown6a3 : true;
}

// @retail 0x1a3269
bool friend_request_get(s_friend_request *request)
{
	if (g_46e814.valid)
		*request = g_46e814.request;
	else
		memset(request, 0, sizeof(s_friend_request));
	return g_46e814.valid;
}

// @retail 0x1a3294
void function_1a3294()
{
	g_46e814.valid = false;
	memset(&g_46e814.request, 0, sizeof(s_friend_request));
}

// @retail 0x1a32ae
bool friends_list_contains(XUID const *xuid)
{
	bool result = false;

	if (g_46e7bc && xuid->qwUserID)
	{
		s_list_item_iterator iterator;

		iterator.iterator.index = NONE;
		iterator.iterator.datum_index = NONE;
		iterator.iterator.data = g_46e7bc;
		while (function_2b2327(&iterator) && !result)
			result = xuid_equal((XUID const *)(iterator.item + 4), xuid, false);
	}
	return result;
}

// @retail 0x1a32fc
bool players_list_contains(XUID const *xuid)
{
	bool result = false;

	if (g_46e7c0 && xuid->qwUserID)
	{
		s_list_item_iterator iterator;

		iterator.iterator.index = NONE;
		iterator.iterator.datum_index = NONE;
		iterator.iterator.data = g_46e7c0;
		while (function_2b2327(&iterator) && !result)
			result = xuid_equal((XUID const *)(iterator.item + 4), xuid, false);
	}
	return result;
}

// @retail 0x1a353a
void title_name_get(WCHAR *name, long name_length, DWORD title_id)
{
	name[0] = 0;
	if (title_id)
		online_get_title_name(title_id, name, name_length);
}
