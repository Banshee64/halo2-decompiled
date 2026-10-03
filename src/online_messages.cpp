// @flags /O2 /Gr
/* ONLINE_MESSAGES.CPP: the Live messages: enumerating a user's messages,
   their details (task type 38), attachments (type 39) and deleting them
   (lane D) */

#include "cseries.h"
#include <xtl.h>
#include <xonline.h>
#include <string.h>
#include "online_tasks.h"

/* the views of a message summary, of the game's copy of it and of a
   message being read that src/unknown_08ebc0.cpp defines */
struct s_entry_source
{
	long unknown0;
	long unknown4;
	long unknown8;
	byte type;
	byte unknownd[3];
	long unknown10;
	long unknown14;
	long unknown18;
	long unknown1c;
	long unknown20;
	union
	{
		byte low;
		dword all;
	};
	long unknown28;
	short unknown2c;
	short unknown2e;
	char name[0x10];
};

struct s_entry
{
	long unknown0;
	long unknown4;
	long unknown8;
	char name[0x10];
	union
	{
		dword flags;
		struct
		{
			dword unknown_bits0 : 10;
			dword flag10 : 1;
			dword flag11 : 1;
			dword flag12 : 1;
		} flag_bits;
	};
	long unknown20;
	long unknown24;
	long unknown28;
	long unknown2c;
	long unknown30;
	long unknown34;
	short unknown38;
	short unknown3a;
	byte unknown3c[4];
};

struct s_state_block
{
	byte active;
	byte unknown1[3];
	long unknown4;
	long unknown8;
	short unknownc;
	byte unknownE[0x1fe];
	long unknown20c;
	long unknown210;
	byte unknown214[4];
	long unknown218;
	long unknown21c;
};

void function_08ebd0(s_entry *entry, s_entry_source *source);
long first_person_animation_type_from_weapon_state(long state);

#define k_maximum_messages 125

// @retail 0x8e750
void online_messages_enumerate(DWORD controller_index, s_entry *entries, long *count)
{
	XONLINE_MSG_SUMMARY summaries[k_maximum_messages];
	DWORD summary_count;

	*count = 0;
	if (online_logon_connected() && SUCCEEDED(XOnlineMessageEnumerate(controller_index, summaries, &summary_count)))
	{
		for (DWORD i = 0; i < summary_count; i++)
			function_08ebd0(&entries[i], (s_entry_source *)&summaries[i]);
		*count = summary_count;
	}
}

// @retail 0x8e7e0
bool online_messages_find_from(const XUID *sender, DWORD controller_index, long kind)
{
	s_entry entries[k_maximum_messages];
	long count = k_maximum_messages;
	bool result = false;

	online_messages_enumerate(controller_index, entries, &count);
	for (long i = 0; i < count; i++)
	{
		s_entry *entry = &entries[i];
		if (*(ULONGLONG *)entry == sender->qwUserID &&
			(kind == 1 || !TEST_FIELD_BIT(entry->flag_bits.flag10)) &&
			(kind == 2 || !TEST_FIELD_BIT(entry->flag_bits.flag12)) &&
			(kind != 3 && kind != 4 || TEST_FIELD_BIT(entry->flag_bits.flag11)))
		{
			result = true;
			break;
		}
	}
	return result;
}

// @retail 0x8e890
bool online_messages_find_flagged_from(const XUID *sender, DWORD controller_index)
{
	s_entry entries[k_maximum_messages];
	long count = k_maximum_messages;
	bool result = false;

	online_messages_enumerate(controller_index, entries, &count);
	for (long i = 0; i < count; i++)
	{
		s_entry *entry = &entries[i];
		if (*(ULONGLONG *)entry == sender->qwUserID && TEST_FIELD_BIT(entry->flag_bits.flag10))
		{
			result = true;
			break;
		}
	}
	return result;
}

// @retail 0x8e920
void online_message_delete(DWORD controller_index, DWORD message_id, bool block_sender)
{
	if (online_logon_connected())
		XOnlineMessageDelete(controller_index, message_id, block_sender);
}

// @retail 0x8e950
long online_message_details(DWORD controller_index, const s_entry *entry)
{
	long task_index = online_task_new_if_logged_on();

	if (task_index != NONE)
	{
		s_online_task *task = online_task_try_get(task_index);
		if (task)
		{
			if (SUCCEEDED(XOnlineMessageDetails(controller_index, entry->unknown20, XONLINE_MSG_FLAG_READ, 0, NULL, (PXONLINETASK_HANDLE)&task->handle)))
			{
				task->flags = 1;
				task->type = 38;
				task->controller_index = controller_index;
			}
			else
			{
				online_task_dispose(task_index);
				task_index = NONE;
			}
		}
	}
	return task_index;
}

// @retail 0x8e9e0
bool online_message_details_get_property(long task_index, long property, void *buffer, DWORD size, bool *too_small, DWORD *required_size)
{
	bool result = false;

	if (size > 0)
		memset(buffer, 0, size);
	*too_small = false;
	*required_size = 0;

	s_online_task *task = online_task_try_get(task_index);
	if (task && online_logon_connected())
	{
		HRESULT hr = XOnlineMessageDetailsGetResultsProperty((XONLINETASK_HANDLE)task->handle, (WORD)first_person_animation_type_from_weapon_state(property), size, buffer, &size, NULL);
		result = SUCCEEDED(hr);
		if (!result && hr == 0x80155a02)
		{
			*too_small = true;
			*required_size = size;
		}
	}
	return result;
}

// @retail 0x8eab0
long online_message_download_attachment(long details_task_index, long property, void *buffer, DWORD size)
{
	long task_index = NONE;

	if (online_task_get_status(details_task_index) == 2)
	{
		s_online_task *details_task = online_task_get(details_task_index);
		if (details_task && online_logon_connected())
		{
			task_index = online_task_new_if_logged_on();
			s_online_task *task = online_task_get(task_index);
			if (task)
			{
				if (SUCCEEDED(XOnlineMessageDownloadAttachmentToMemory((XONLINETASK_HANDLE)details_task->handle, (WORD)first_person_animation_type_from_weapon_state(property),
					(PBYTE)buffer, size, NULL, (PXONLINETASK_HANDLE)&task->handle)))
				{
					task->flags = 1;
					task->type = 39;
					task->controller_index = details_task->controller_index;
				}
				else
				{
					online_task_dispose(task_index);
					return NONE;
				}
			}
		}
	}
	return task_index;
}

// @retail 0x8eb50
bool online_message_download_get_results(long task_index, DWORD *received_size, BYTE **data, DWORD *total_size)
{
	*data = 0;
	*received_size = 0;
	*total_size = 0;
	if (online_task_get_status(task_index) == 2)
	{
		s_online_task *task = online_task_get(task_index);
		if (task && online_logon_connected() &&
			SUCCEEDED(XOnlineMessageDownloadAttachmentToMemoryGetResults((XONLINETASK_HANDLE)task->handle, data, received_size, total_size)))
		{
			return true;
		}
	}
	return false;
}

// @retail 0x8eee0
void online_message_block_reset(s_state_block *block)
{
	if (block->unknown218 != NONE)
	{
		online_task_dispose(block->unknown218);
		block->unknown218 = NONE;
	}
	XONLINE_MSG_HANDLE message = (XONLINE_MSG_HANDLE)block->unknown21c;
	if (message && message != (XONLINE_MSG_HANDLE)NONE && SUCCEEDED(XOnlineMessageDestroy(message)))
		block->unknown21c = 0;
	block->unknownc = 0;
	block->unknown20c = 0;
	block->active = false;
}
