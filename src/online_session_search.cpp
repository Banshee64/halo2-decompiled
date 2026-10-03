// @flags /O2 /arch:SSE /Gr
/* ONLINE_SESSION_SEARCH.CPP: the Xbox Live matchmaking session searches: the
   online tasks that create, find and delete match sessions, the results of a
   search, and the session identifiers already seen. */

#include "cseries.h"
#include "globals.h"
#include "online_tasks.h"
#include <xtl.h>
#include <xonline.h>
#include <string.h>

#define MAXIMUM_SEARCH_RESULTS 50

void online_task_dispose(long task_index);
long online_task_new_if_logged_on(void);
void qos_release(long handle);

/* the release routine retail inlines here */
static inline void free_block(void *block)
{
	long info;
	if (!g_4d87f8->allocator->get_info(block, &info))
		info = NONE;
	s_allocator_globals *globals = g_4d87f8;
	globals->allocator->release(block, NONE);
	if (block != 0)
		globals->count--;
}

static inline s_online_task *online_task_try_and_get(long task_index)
{
	s_online_task *task = 0;
	if (task_index != NONE)
	{
		s_data_array *data = g_4cf78c;
		long absolute_index = task_index & 0xffff;
		if (absolute_index < data->high_water_index)
		{
			s_online_task *candidate = (s_online_task *)(data->data + data->size * absolute_index);
			if (candidate->salt != 0 && candidate->salt == (task_index >> 16))
				task = candidate;
		}
	}
	return task;
}

/* a session a search found (0x204 bytes) */
struct s_search_result
{
	bool valid;
	bool unknown01;
	bool seen;
	byte unknown03;
	long unknown04;
	bool unknown08;
	byte unknown09[3];
	byte search_result[0x68];
	bool unknown74;
	bool unknown75;
	byte unknown76[2];
	long unknown78;
	long unknown7c;
	long unknown80;
	byte unknown84[0x204 - 0x84];
};

/* a session search */
struct s_session_search
{
	bool active;
	byte unknown01[3];
	dword start_time;
	long state;
	byte unknown0c[8];
	bool flag14;
	byte unknown15[7];
	long unknown1c;
	long unknown20;
	byte unknown24[0x70 - 0x24];
	long task_index;
	long qos_handles[2];
	long qos_counts[2];
	long result_count;
	long unknown88;
	s_search_result *results;
	void *unknown90;
	long seen_count;
	long seen_capacity;
	long unknown9c;
	XNKID *seen;
};

/* the parts of a session a search compares */
struct s_search_session
{
	byte unknown00[4];
	XNKID id;
};

// @retail 0x90020
long online_match_session_delete(s_search_session const *session, bool *unavailable)
{
	bool service_unavailable = false;
	online_task_exists(8, 0xff);
	long task_index = online_task_new_if_logged_on();
	if (task_index != NONE)
	{
		s_online_task *task = online_task_try_and_get(task_index);
		if (task)
		{
			HRESULT result = XOnlineMatchSessionDelete(session->id, NULL, (PXONLINETASK_HANDLE)&task->handle);
			if (SUCCEEDED(result))
			{
				task->flags = 1;
				task->type = 8;
				task->controller_index = NONE;
				*unavailable = service_unavailable;
				return task_index;
			}
			if (result == 0x80155100)
				service_unavailable = true;
			online_task_dispose(task_index);
			task_index = NONE;
		}
	}
	*unavailable = service_unavailable;
	return task_index;
}

// @retail 0x900e0
long online_match_session_find(XNKID const *session_id)
{
	if (online_task_exists(9, 0xff) > 2)
		return NONE;
	long task_index = online_task_new_if_logged_on();
	s_online_task *task = online_task_get(task_index);
	if (task)
	{
		if (FAILED(XOnlineMatchSessionFindFromID(*session_id, NULL, (PXONLINETASK_HANDLE)&task->handle)))
		{
			online_task_dispose(task_index);
			return NONE;
		}
		task->flags = 1;
		task->type = 9;
		task->controller_index = NONE;
	}
	return task_index;
}

// @retail 0x90c80
void function_090c80(byte *p)
{
	s_session_search *search = (s_session_search *)p;
	if (search->state == 1)
		search->state = 2;
	if (search->task_index != NONE)
	{
		online_task_dispose(search->task_index);
		search->task_index = NONE;
	}
	if (search->qos_handles[0] != NONE)
	{
		qos_release(search->qos_handles[0]);
		search->qos_handles[0] = NONE;
	}
	if (search->qos_handles[1] != NONE)
	{
		qos_release(search->qos_handles[1]);
		search->qos_handles[1] = NONE;
	}
	if (search->unknown90)
	{
		free_block(search->unknown90);
		search->unknown90 = 0;
	}
	if (search->results)
	{
		free_block(search->results);
		search->results = 0;
	}
	if (search->seen)
	{
		free_block(search->seen);
		search->seen = 0;
	}
	search->active = false;
}

// @retail 0x91290
bool session_search_seen(s_session_search *search, XNKID const *id)
{
	bool seen = false;
	for (long i = 0; i < search->seen_count && !seen; i++)
	{
		if (memcmp(&search->seen[i], id, sizeof(XNKID)) == 0)
			seen = true;
	}
	return seen;
}

/* the session identifier in a result */
static inline XNKID const *search_result_id(s_search_result const *result)
{
	return (XNKID const *)(result->search_result + 0x10);
}

// @retail 0x912d0
void session_search_mark_seen(s_session_search *search, s_search_result *result)
{
	if (!result->seen)
	{
		XNKID const *id = search_result_id(result);
		result->seen = true;
		result->unknown01 = false;
		if (!session_search_seen(search, id) && search->seen_count < search->seen_capacity)
		{
			search->seen[search->seen_count] = *id;
			search->seen_count++;
		}
	}
}

// @retail 0x91320
void session_search_mark_session(s_session_search *search, s_search_session const *session)
{
	if (search->active)
	{
		for (s_search_result *result = search->results; result < search->results + MAXIMUM_SEARCH_RESULTS; result++)
		{
			if (result->valid && result->unknown74 && memcmp(&session->id, (byte *)result + 0xbc, sizeof(XNKID)) == 0)
			{
				session_search_mark_seen(search, result);
				return;
			}
		}
	}
}

// @retail 0x91380
void session_search_get_progress(s_session_search *search, long *first, long *last, long *count, long *total)
{
	if (search->active)
	{
		long start;
		long end;
		if (search->flag14)
		{
			end = search->unknown20;
			start = search->unknown1c;
		}
		else
		{
			long current = search->unknown20;
			long previous = search->unknown1c;
			end = current + 1;
			start = previous;
			if (previous == NONE)
				start = current;
		}
		long result_count = 0;
		long result_total = 0;
		for (s_search_result *result = search->results; result < search->results + MAXIMUM_SEARCH_RESULTS; result++)
		{
			if (result->valid)
			{
				result_count++;
				result_total += *(long *)((byte *)result + 0x48);
			}
		}
		*first = start;
		*last = end;
		*count = result_count;
		*total = result_total;
	}
}
