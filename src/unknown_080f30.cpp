// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_080F30.CPP: the requests that send and receive messages through
   the pending messages (lane D) */

#include "unknown_11c920.h"
#include "pending_messages.h"
#include "crc.h"

bool function_0b49a0(long index, real *result);

// @retail 0x812a0
dword pending_message_payload_crc(const void *data, long size)
{
	dword crc = 0xffffffff;
	function_163ba0(&crc, (const byte *)data + 4, size - 4);
	return crc;
}

/* how far the request's transfer has got */
// @retail 0x80f30
bool pending_message_request_get_progress(s_pending_message_header *header, real *progress)
{
	bool result = false;
	long index = header->pending_index;
	if (index != NONE)
	{
		s_pending_message *message = &g_4d8c28[index];
		if (message->task_index != NONE)
			return function_0b49a0(message->task_index, progress);
		*progress = 0.0f;
		result = true;
	}
	return result;
}

/* queues a request to receive a message */
// @retail 0x80f70
bool pending_message_request_receive(s_pending_message_header *header, long value10, s_pending_message_values values, short value06)
{
	bool result = false;
	if (header->state == 0)
	{
		header->unknown10 = value10;
		header->values = values;
		header->unknown06 = value06;
		if (header->kind != 0 && (header->flags & 2) && !(header->flags & 8))
		{
			result = true;
		}
		else
		{
			header->pending_index = pending_message_add_received(header, header->data, header->size);
			if (header->pending_index != NONE)
			{
				header->state = 1;
				if (header->kind != 4)
					header->flags &= ~2;
				return true;
			}
		}
	}
	return result;
}

/* queues a request to send a message */
// @retail 0x80ff0
bool pending_message_request_send(s_pending_message_header *header, long value10, s_pending_message_values values, short value06)
{
	bool result = false;
	if (header->state == 0)
	{
		header->unknown10 = value10;
		header->values = values;
		header->unknown06 = value06;
		header->pending_index = pending_message_add(header->size, header, header->data);
		if (header->pending_index != NONE)
		{
			header->state = 2;
			result = true;
		}
	}
	return result;
}
