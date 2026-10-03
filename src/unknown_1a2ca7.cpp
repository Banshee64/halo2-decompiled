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

/* a friend's details (12 bytes) */
struct s_friend_details
{
	dword unknown0;
	dword unknown4;
	dword unknown8;
};

/* a player of the players list (0xac bytes) */
struct s_friend_player
{
	byte unknown00[2];
	short unknown02;
	union
	{
		XUID xuid;
		struct
		{
			dword unknown04;
			dword unknown08;
			dword unknown0c;
		};
	};
	union
	{
		char gamertag[16];
		bool unknown10;
	};
	union
	{
		dword flags20;
		struct
		{
			byte unknown20;
			byte : 3;
			byte flags21_3 : 1;
			byte : 4;
		};
	};
	dword unknown24[2];
	dword unknown2c;
	s_friend_details details;
	short unknown3c;
	byte unknown3e[0xa4 - 0x3e];
	long state;
	dword flagsa8;
};

/* a player as the online service reports it (0x92 bytes) */
#pragma pack(push, 1)
struct s_online_player
{
	XUID xuid;
	char gamertag[16];
	long state;
	byte unknown20[0x86 - 0x20];
	union
	{
		dword flags;
		struct
		{
			byte flags_0 : 1;
			byte : 7;
		};
		struct
		{
			dword : 2;
			dword flags_2 : 1;
			dword : 29;
		};
	};
	byte unknown8a[0x92 - 0x8a];
};
#pragma pack(pop)

/* an entry of the list of player references (8 bytes) */
struct s_friend_player_reference
{
	byte unknown0[4];
	long player_index;
};

/* a player's online status block (unknown_18f576.cpp) */
struct s_player_slot_blockb82
{
	byte data[0x92];
};

bool function_18ffc3(long index, s_player_slot_blockb82 *block);

long g_46e7b8 = NONE;
extern s_data_array *g_46e7bc;
extern s_data_array *g_46e7c0;
s_data_array *g_46e7c4;
s_data_array *g_46e7c8;
s_data_array *g_46e7cc;
long g_46e7d0 = NONE;
long g_46e7d4 = NONE;
long g_46e7d8 = NONE;
long g_46e7dc = NONE;
long g_46e7e0 = NONE;
long g_46e7e4 = NONE;
long g_46e7e8 = NONE;
s_friend_details g_46e7ec;
s_friend_details g_46e7f8;
s_friend_details g_46e804;
long g_46e810;
s_friend_request_globals g_46e814;
byte g_54eae8[4][0xc70];

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
void unicode_string_copy(word *destination, const word *source, long maximum_count);
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
	function_190728(screen->get_controller_index());
	function_1a3294();
	function_18fe9e(screen->get_controller_index());
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
			long index = get_controller_index();

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

// @retail 0x1a334a
bool function_1a334a(long index, XUID const *xuid)
{
	s_player_slot_blockb82 block;
	bool result = false;

	if (function_18ffc3(index, &block) && *(long *)&block.data[0x1c] == 3 && g_46e7c0)
	{
		s_list_item_iterator iterator;

		result = true;
		iterator.iterator.index = NONE;
		iterator.iterator.datum_index = NONE;
		iterator.iterator.data = g_46e7c0;
		while (function_2b2327(&iterator))
		{
			byte *player = iterator.item;

			if (((XUID *)(player + 4))->qwUserID && *(long *)(player + 0xa4) == 3 &&
				xuid_equal((XUID const *)(player + 0x30), xuid, false))
			{
				result = false;
				break;
			}
		}
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

// @retail 0x1a35c8
void friends_list_reset(bool dispose)
{
	g_46e7b8 = NONE;
	if (dispose)
	{
		if (g_46e7bc)
		{
			data_dispose(g_46e7bc);
			g_46e7bc = NULL;
		}
		if (g_46e7c0)
		{
			data_dispose(g_46e7c0);
			g_46e7c0 = NULL;
		}
		if (g_46e7c4)
		{
			data_dispose(g_46e7c4);
			g_46e7c4 = NULL;
		}
		if (g_46e7c8)
		{
			data_dispose(g_46e7c8);
			g_46e7c8 = NULL;
		}
		if (g_46e7cc)
		{
			data_dispose(g_46e7cc);
			g_46e7cc = NULL;
			g_46e814.unknown6a3 = false;
		}
	}
	else
	{
		data_delete_all(g_46e7bc);
		data_delete_all(g_46e7c0);
		data_delete_all(g_46e7c4);
		g_46e814.unknown6a3 = false;
	}
	if (g_46e7d0 != NONE)
	{
		online_task_dispose(g_46e7d0);
		g_46e7d0 = NONE;
	}
	if (g_46e7d4 != NONE)
	{
		online_task_dispose(g_46e7d4);
		g_46e7d4 = NONE;
	}
	if (g_46e7d8 != NONE)
	{
		online_task_dispose(g_46e7d8);
		g_46e7d8 = NONE;
	}
	if (g_46e7e0 != NONE)
	{
		online_task_dispose(g_46e7e0);
		g_46e7e0 = NONE;
	}
	if (g_46e7e4 != NONE)
	{
		online_task_dispose(g_46e7e4);
		g_46e7e4 = NONE;
	}
	if (g_46e7e8 != NONE)
	{
		online_task_dispose(g_46e7e8);
		g_46e7e8 = NONE;
	}
	memset(&g_46e7ec, 0, sizeof(g_46e7ec));
	memset(&g_46e7f8, 0, sizeof(g_46e7f8));
	memset(&g_46e804, 0, sizeof(g_46e804));
	memset(&g_46e814.request, 0, sizeof(s_friend_request));
	g_46e810 = 0;
	g_46e814.valid = false;
}

// @retail 0x1a43eb
void friends_player_new()
{
	long player_index = datum_new(g_46e7c0);
	long reference_index;
	s_friend_player *player = (s_friend_player *)(g_46e7c0->data + (player_index & 0xffff) * sizeof(s_friend_player));

	reference_index = datum_new(g_46e7c4);
	s_friend_player_reference *reference = (s_friend_player_reference *)(g_46e7c4->data + (reference_index & 0xffff) * sizeof(s_friend_player_reference));

	player->unknown04 = 0;
	player->unknown08 = 0;
	player->unknown10 = false;
	player->unknown3c = 0;
	reference->player_index = player_index;
}

// @retail 0x1a460f
bool friend_details_get(XUID const *xuid, s_friend_details *details)
{
	bool result = false;

	if (xuid && xuid->qwUserID && details && g_46e7c8)
	{
		s_list_item_iterator iterator;

		iterator.iterator.index = NONE;
		iterator.iterator.datum_index = NONE;
		iterator.iterator.data = g_46e7c8;
		while (function_2b2327(&iterator))
		{
			XUID entry_xuid;

			entry_xuid.qwUserID = *(ULONGLONG *)iterator.item;
			entry_xuid.dwUserFlags = 0;
			if (xuid_equal(xuid, &entry_xuid, false))
			{
				*details = *(s_friend_details *)(iterator.item + 8);
				result = true;
				break;
			}
		}
	}
	return result;
}

// @retail 0x1a4692
bool friend_name_get(XUID const *xuid, word *name)
{
	bool result = false;

	if (xuid && xuid->qwUserID && g_46e7cc)
	{
		s_list_item_iterator iterator;

		iterator.iterator.index = NONE;
		iterator.iterator.datum_index = NONE;
		iterator.iterator.data = g_46e7cc;
		while (function_2b2327(&iterator))
		{
			XUID entry_xuid;

			entry_xuid.qwUserID = *(ULONGLONG *)iterator.item;
			entry_xuid.dwUserFlags = 0;
			if (xuid_equal(xuid, &entry_xuid, false))
			{
				unicode_string_copy(name, (word *)(iterator.item + 8), 16);
				result = true;
				break;
			}
		}
	}
	return result;
}
/* copies at most count characters and terminates the copy */
static inline char *string_copy(char *destination, char const *source, long count)
{
	strncpy(destination, source, count);
	destination[count - 1] = 0;
	return destination;
}

// @retail 0x1a4336
void friends_player_set(s_friend_player *player, s_online_player const *source, short value)
{
	player->unknown02 = value;
	player->xuid = source->xuid;
	string_copy(player->gamertag, source->gamertag, 16);
	player->flags20 = 0;
	player->unknown3c = 0;
	player->flags20 = source->flags_0 << 10;
	if (TEST_FIELD_BIT(source->flags_2))
		player->flags21_3 = true;
	else
		player->flags21_3 = false;
	player->flagsa8 = source->flags;
	memset(player->unknown24, 0, sizeof(player->unknown24));
	player->unknown2c = 0;
	if (!friend_details_get(&player->xuid, &player->details))
		memset(&player->details, 0, sizeof(s_friend_details));
	player->state = source->state;
	player->flagsa8 = source->flags;
}

void function_23620d(long string_id, word *buffer);

/* the text of a friend's state */
// @retail 0x1a4714
void function_1a4714(long state, word *buffer)
{
	long string_id = 0;

	switch (state)
	{
	case 0:
		string_id = 0x40002c6;
		break;
	case 1:
		string_id = 0x60002c7;
		break;
	case 2:
		string_id = 0xd0002c8;
		break;
	case 3:
		string_id = 0x90002c9;
		break;
	}
	function_23620d(string_id, buffer);
}