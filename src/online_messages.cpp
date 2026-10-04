// @flags /O2 /Gr
/* ONLINE_MESSAGES.CPP: the Live messages: enumerating a user's messages,
   their details (task type 38), attachments (type 39) and deleting them
   (lane D) */

#include "cseries.h"
#include <xtl.h>
#include <xonline.h>
#include <string.h>
#include <wchar.h>
#include "online_tasks.h"
#include "online_message_entries.h"
#include "loop_allocator.h"

void function_08ebd0(s_entry_source *source, s_entry *entry);
long first_person_animation_type_from_weapon_state(long state);

#define k_maximum_messages 125

// @retail 0x8e750 standard
void __stdcall online_messages_enumerate(DWORD controller_index, s_entry *entries, long *count)
{
	XONLINE_MSG_SUMMARY summaries[k_maximum_messages];
	DWORD summary_count;

	*count = 0;
	if (online_logon_connected() && SUCCEEDED(XOnlineMessageEnumerate(controller_index, summaries, &summary_count)))
	{
		for (DWORD i = 0; i < summary_count; i++)
			function_08ebd0((s_entry_source *)&summaries[i], &entries[i]);
		*count = summary_count;
	}
}

// @retail 0x8e7e0
bool online_messages_find_from(const XUID *sender, DWORD controller_index, long kind)
{
	bool result = false;
	s_entry entries[k_maximum_messages];
	long count = k_maximum_messages;

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
	bool result = false;
	s_entry entries[k_maximum_messages];
	long count = k_maximum_messages;

	online_messages_enumerate(controller_index, entries, &count);
	for (long i = 0; i < count; i++)
	{
		if (*(ULONGLONG *)&entries[i] == sender->qwUserID && TEST_FIELD_BIT(entries[i].flag_bits.flag10))
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

/* the text of a message being written */
// @retail 0x8ef40
void online_message_block_set_text(s_state_block *block, const wchar_t *text)
{
	wchar_t *block_text = (wchar_t *)&block->unknownc;
	wcsncpy(block_text, text, 255);
	block_text[255] = 0;
}

// @retail 0x8ef60
void online_message_block_set_values(s_state_block *block, long value210, long value214, long value20c)
{
	if (value210 < 0x499a)
	{
		block->unknown210 = value210;
		block->unknown20c = value20c;
		*(long *)block->unknown214 = value214;
	}
}

/* the message property tag of each of the game's message properties */
// @retail 0x8f3f0
long online_message_property_get_tag(long property)
{
	switch (property)
	{
	case 0:
		return 0x9c1;
	case 1:
		return 0x3c2;
	case 2:
		return 0x4c3;
	case 3:
		return 0x6c4;
	case 4:
		return 0x4c5;
	case 5:
		return 0x581;
	case 6:
		return 0x681;
	default:
		return NONE;
	}
}

// @retail 0x8f450
bool online_message_property_size_valid(long property, DWORD size)
{
	bool result;

	switch (property)
	{
	case 0:
		result = size > 0;
		break;
	case 1:
		result = size == 2;
		break;
	case 2:
		result = size == 4;
		break;
	case 3:
		result = size > 0 && !(size & 1);
		break;
	case 4:
		result = size == 4;
		break;
	case 5:
		result = size == 8;
		break;
	case 6:
		result = size > 0 && !(size & 1);
		break;
	default:
		result = false;
		break;
	}
	return result;
}

// @retail 0x8f4b0
void online_message_block_set_property(s_state_block *block, long property, DWORD size, const void *value)
{
	long tag = online_message_property_get_tag(property);

	if (tag != NONE)
	{
		if (online_message_property_size_valid(property, size))
		{
			if (online_logon_connected())
				XOnlineMessageSetProperty((XONLINE_MSG_HANDLE)block->unknown21c, (WORD)tag, size, value, 0);
			return;
		}
	}
	block->unknown8 = 4;
}

extern s_loop_allocator *g_51e998;

/* loop_free (0x18e430) and user_interface_free (0x1a4826), inlined here */
static inline void loop_free_inline(s_loop_allocator *loop, void **pointer)
{
	s_loop_block *block = (s_loop_block *)*pointer - 1;

	loop->free += block->size;
	if (block->previous)
	{
		block->previous->next = block->next;
	}
	else
	{
		loop->first = block->next;
	}
	if (block->next)
	{
		block->next->previous = block->previous;
	}
	else
	{
		loop->last = block->previous;
	}
}

static inline void user_interface_free_inline(void *pointer)
{
	loop_free_inline(g_51e998, &pointer);
}

/* frees a message block (allocated from the user interface's pool) */
// @retail 0x8fa30
void function_08fa30(s_state_block *block)
{
	if (block->active)
		online_message_block_reset(block);
	user_interface_free_inline(block);
}
