// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_08E1E0.CPP: a table of 32 pending messages with two queues of 16
   indices into it (lane D) */

#include "cseries.h"
#include <string.h>
#include "crc.h"
#include "pending_messages.h"


/* function_x86aaf2, which retail inlines here (this file is /Ob1 so that 0x8e1e0 stays
   a call) */
static inline void crc_new_inlined(dword *crc_reference)
{
	*crc_reference = 0xffffffff;
}

long g_4d8ba8[2][16];
s_pending_message g_4d8c28[32];

// @retail 0x8e1e0
long pending_message_find_free(void)
{
	long index = NONE;
	long i = 0;

	do
	{
		if (i >= 32)
			break;
		index = !g_4d8c28[i].header ? i : index;
		i++;
	} while (index == NONE);
	return index;
}

// @retail 0x8e210
void pending_messages_reset(void)
{
	memset(g_4d8ba8, NONE, sizeof(g_4d8ba8));
	long i = 0;
	do
	{
		g_4d8c28[i].header = 0;
		g_4d8c28[i].task_index = NONE;
		g_4d8c28[i].size = 0;
		g_4d8c28[i].data = 0;
		i++;
	} while (i < 32);
}

// @retail 0x8e500
long pending_message_add(long size, s_pending_message_header *header, void *data)
{
	long index = pending_message_find_free();
	long slot = NONE;

	long i = 0;
	do
	{
		if (i >= 16)
			break;
		slot = g_4d8ba8[0][i] == NONE ? i : slot;
		i++;
	} while (slot == NONE);
	if (header->kind == 1 || header->kind == 2)
	{
		crc_new_inlined((dword *)data);
		function_163ba0((dword *)data, data, size);
	}
	g_4d8c28[index].size = size;
	g_4d8c28[index].data = data;
	g_4d8c28[index].header = header;
	g_4d8c28[index].task_index = NONE;
	g_4d8ba8[0][slot] = index;
	return index;
}

// @retail 0x8e580
long pending_message_add_received(s_pending_message_header *header, void *data, long size)
{
	long index = pending_message_find_free();
	long slot = NONE;

	long i = 0;
	do
	{
		if (i >= 16)
			break;
		slot = g_4d8ba8[1][i] == NONE ? i : slot;
		i++;
	} while (slot == NONE);
	g_4d8c28[index].task_index = NONE;
	g_4d8c28[index].size = size;
	g_4d8c28[index].header = header;
	g_4d8c28[index].data = data;
	g_4d8ba8[1][slot] = index;
	return index;
}
