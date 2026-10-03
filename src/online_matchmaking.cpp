// @flags /O2 /Gr
/* ONLINE_MATCHMAKING.CPP: the Live matchmaking tasks: searches (task type
   6), session create, update and info (types 7 and 10) (lane D) */

#include "cseries.h"
#include <xtl.h>
#include <xonline.h>
#include <string.h>
#include "online_tasks.h"
#include "online_attributes.h"

long function_0b4a90(s_entry_pair *out, const s_range_input *in);
long function_0b4b80(long count, s_property_entry *out, const s_property_input *in);

/* the attributes of a search result: seven integers */
const XONLINE_ATTRIBUTE_SPEC g_44050c[7] =
{
	{ X_ATTRIBUTE_DATATYPE_INTEGER, sizeof(ULONGLONG) },
	{ X_ATTRIBUTE_DATATYPE_INTEGER, sizeof(ULONGLONG) },
	{ X_ATTRIBUTE_DATATYPE_INTEGER, sizeof(ULONGLONG) },
	{ X_ATTRIBUTE_DATATYPE_INTEGER, sizeof(ULONGLONG) },
	{ X_ATTRIBUTE_DATATYPE_INTEGER, sizeof(ULONGLONG) },
	{ X_ATTRIBUTE_DATATYPE_INTEGER, sizeof(ULONGLONG) },
	{ X_ATTRIBUTE_DATATYPE_INTEGER, sizeof(ULONGLONG) },
};

/* the session a host advertises */
struct s_online_match_session
{
	byte unknown00[0x10];
	XNKID session_id;
	byte unknown18[0x3c - 0x18];
	DWORD public_filled;
	DWORD public_open;
	DWORD private_filled;
	DWORD private_open;
	s_property_input properties;
};

/* what a session's info task returns */
struct s_online_match_session_info
{
	XNKEY key;
	XNKID session_id;
	XNADDR address;
	byte unknown3c[0x68 - 0x3c];
};

// @retail 0x8fa80
long online_match_search(const s_range_input *input)
{
	if (online_task_exists(6, 0xff) > 2)
		return NONE;

	long task_index = online_task_new_if_logged_on();
	s_online_task *task = online_task_get(task_index);
	if (task)
	{
		s_entry_pair attributes[12];
		long attribute_count = function_0b4a90(attributes, input);
		DWORD results_size = XOnlineMatchSearchResultsLen(50, 7, g_44050c);
		if (SUCCEEDED(XOnlineMatchSearch(1, 50, attribute_count, (PXONLINE_ATTRIBUTE)attributes, results_size, NULL, (PXONLINETASK_HANDLE)&task->handle)))
		{
			task->flags = 1;
			task->type = 6;
			task->controller_index = NONE;
		}
		else
		{
			online_task_dispose(task_index);
			return NONE;
		}
	}
	return task_index;
}

// @retail 0x8fcc0
long online_match_session_create(const s_online_match_session *session)
{
	long task_index = NONE;

	if (online_task_exists(7, 0xff) <= 2)
	{
		task_index = online_task_new_if_logged_on();
		s_online_task *task = online_task_get(task_index);
		if (task)
		{
			s_property_entry attributes[15];
			long attribute_count = function_0b4b80(15, attributes, &session->properties);
			if (SUCCEEDED(XOnlineMatchSessionCreate(session->public_filled, session->public_open, session->private_filled, session->private_open,
				attribute_count, (PXONLINE_ATTRIBUTE)attributes, NULL, (PXONLINETASK_HANDLE)&task->handle)))
			{
				task->flags = 1;
				task->type = 7;
				task->controller_index = NONE;
			}
			else
			{
				online_task_dispose(task_index);
				return NONE;
			}
		}
	}
	return task_index;
}

// @retail 0x8fd90
bool online_match_session_get_info(long task_index, s_online_match_session_info *info)
{
	s_online_task *task = online_task_try_get(task_index);

	if (task)
	{
		word flags = task->flags;
		if (flags & 4)
		{
			if (!(flags & 0x20) && online_logon_connected() &&
				SUCCEEDED(XOnlineMatchSessionGetInfo((XONLINETASK_HANDLE)task->handle, &info->session_id, &info->key)))
			{
				XNetGetTitleXnAddr(&info->address);
			}
			else
			{
				memset(info, 0, sizeof(*info));
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}

// @retail 0x8fe20
long online_match_session_update(const s_online_match_session *session)
{
	long task_index = NONE;

	if (*(const __int64 *)&session->session_id != 0 && online_task_exists(10, 0xff) <= 2)
	{
		task_index = online_task_new_if_logged_on();
		s_online_task *task = online_task_get(task_index);
		if (task)
		{
			s_property_entry attributes[15];
			long attribute_count = function_0b4b80(15, attributes, &session->properties);
			if (SUCCEEDED(XOnlineMatchSessionUpdate(session->session_id, session->public_filled, session->public_open, session->private_filled, session->private_open,
				attribute_count, (PXONLINE_ATTRIBUTE)attributes, NULL, (PXONLINETASK_HANDLE)&task->handle)))
			{
				task->flags = 1;
				task->type = 10;
				task->controller_index = NONE;
			}
			else
			{
				online_task_dispose(task_index);
				return NONE;
			}
		}
	}
	return task_index;
}

/* whether a task finished; succeeded tells whether it did without failing */
// @retail 0x8ff20
bool online_task_get_finished(long task_index, bool *succeeded)
{
	s_online_task *task = online_task_try_get(task_index);
	bool success = false;

	if (task && online_logon_connected())
	{
		bool result;

		if (task->flag_bits.finished)
		{
			success = !task->flag_bits.failed;
			result = true;
		}
		else
		{
			result = false;
		}
		*succeeded = success;
		return result;
	}
	*succeeded = success;
	return true;
}

HRESULT online_task_continue(s_online_task *task);

// @retail 0x8ffc0
bool online_task_continue_failed(long task_index)
{
	bool result = false;
	s_online_task *task = online_task_try_get(task_index);
	HRESULT hr = online_task_continue(task);

	if (FAILED(hr) && hr != 0x80155100)
		result = true;
	return result;
}
