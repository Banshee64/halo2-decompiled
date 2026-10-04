// @flags /O2 /Ob1 /Gr
/* ONLINE_FEEDBACK.CPP: player feedback (online task type 22) and the new
   content check (type 13) (lane D) */

#include "cseries.h"
#include <xtl.h>
#include <xonline.h>
#include <string.h>
#include <wchar.h>
#include "online_tasks.h"

/* set when the new content check finds new content */
bool g_51055d;

// @retail 0x8d3d0
void online_feedback_send(const XUID *xuid, DWORD controller_index, long kind)
{
	XONLINE_FEEDBACK_TYPE types[10] =
	{
		XONLINE_FEEDBACK_POS_ATTITUDE,
		XONLINE_FEEDBACK_POS_SESSION,
		XONLINE_FEEDBACK_NEG_NICKNAME,
		XONLINE_FEEDBACK_NEG_GAMEPLAY,
		XONLINE_FEEDBACK_NEG_SCREAMING,
		XONLINE_FEEDBACK_NEG_HARASSMENT,
		XONLINE_FEEDBACK_NEG_LEWDNESS,
		(XONLINE_FEEDBACK_TYPE)12,
		(XONLINE_FEEDBACK_TYPE)11,
		(XONLINE_FEEDBACK_TYPE)10,
	};
	long task_index = online_task_new_if_logged_on();
	s_type_9df9da *task = online_task_try_get(task_index);

	if (task)
	{
		XONLINE_FEEDBACK_PARAMS params;
		params.lpStringParam = NULL;
		if (SUCCEEDED(XOnlineFeedbackSend(controller_index, *xuid, types[kind], &params, NULL, (PXONLINETASK_HANDLE)&task->handle)))
		{
			task->flags = 9;
			task->type = 22;
			task->controller_index = controller_index;
		}
		else
		{
			function_6b640(task_index);
		}
	}
}

// @retail 0x8d4d0
long online_offering_check_new_content(void)
{
	long task_index = online_task_new_if_logged_on();

	if (task_index != NONE)
	{
		s_type_9df9da *task = online_task_try_get(task_index);
		if (task)
		{
			g_51055d = false;
			if (SUCCEEDED(XOnlineOfferingIsNewContentAvailable(0xffffffff, NULL, (PXONLINETASK_HANDLE)&task->handle)))
			{
				task->flags = 9;
				task->type = 13;
				task->controller_index = NONE;
			}
			else
			{
				function_6b640(task_index);
				task_index = NONE;
			}
		}
	}
	return task_index;
}

/* ---- the downloaded content on the hard disk (XFindFirstContent) ---- */

/* one content package: its directory and display name (0x204 bytes) */
struct s_content_item
{
	char directory[MAX_PATH];
	wchar_t display_name[MAX_CONTENT_DISPLAY_NAME];
};

/* set when a content enumeration fails for any reason other than running out */
bool g_54e7f8;

// @retail 0x8d550
void content_item_from_find_data(const XCONTENT_FIND_DATA *data, s_content_item *item)
{
	strncpy(item->directory, data->szContentDirectory, MAX_PATH);
	item->directory[MAX_PATH - 1] = 0;
	wchar_t *display_name = item->display_name;
	wcsncpy(display_name, data->szDisplayName, MAX_CONTENT_DISPLAY_NAME - 1);
	display_name[MAX_CONTENT_DISPLAY_NAME - 1] = 0;
}

// @retail 0x8d5a0
bool content_find_first(s_content_item *item, HANDLE *find_handle)
{
	XCONTENT_FIND_DATA data;
	SetLastError(0);
	HANDLE handle = XFindFirstContent("t:\\", 0, &data);
	*find_handle = 0;
	memset(item, 0, sizeof(*item));
	if (handle == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() != ERROR_FILE_NOT_FOUND)
			g_54e7f8 = true;
		return false;
	}
	content_item_from_find_data(&data, item);
	*find_handle = handle;
	return true;
}

// @retail 0x8d620
bool content_find_next(HANDLE find_handle, s_content_item *item)
{
	bool result = false;
	XCONTENT_FIND_DATA data;
	memset(item, 0, sizeof(*item));
	if (XFindNextContent(find_handle, &data))
	{
		content_item_from_find_data(&data, item);
		result = true;
	}
	else
	{
		if (GetLastError() != ERROR_NO_MORE_FILES)
			g_54e7f8 = true;
		XFindClose(find_handle);
	}
	return result;
}