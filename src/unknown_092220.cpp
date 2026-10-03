// @flags /O2 /Gr
/* UNKNOWN_092220.CPP: Xbox Live online tasks that follow the team
   balancing (0x92220..0x927b0) */

#include "cseries.h"
#include "globals.h"
#include "online_tasks.h"
#include <xtl.h>
#include <xonline.h>

HRESULT online_task_continue(s_online_task *task);
long online_task_new_if_logged_on(void);
void online_task_dispose(long task_index);

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

/* online_task_new (src/online_tasks.cpp), which retail inlines here */
static inline long online_task_new_inline(void)
{
	long task_index = datum_new(g_4cf78c);
	if (task_index != NONE)
	{
		s_online_task *task = (s_online_task *)g_4cf78c->data + (task_index & 0xffff);
		task->handle = 0;
		task->type = NONE;
		task->controller_index = NONE;
		task->flags = 0;
	}
	return task_index;
}

/* starts reading the given statistics */
// @retail 0x92220
long online_stats_read(word count, XONLINE_STAT_SPEC *specs)
{
	for (long i = 0; i < count; i++)
	{
		if (specs[i].xuidUser.dwUserFlags == 0xbad00000 || (specs[i].xuidUser.dwUserFlags & 3))
			return NONE;
	}
	long task_index = NONE;
	if (online_logon_connected())
		task_index = online_task_new_inline();
	if (task_index != NONE)
	{
		s_online_task *task = online_task_try_and_get(task_index);
		if (task)
		{
			if (SUCCEEDED(XOnlineStatRead(count, specs, NULL, (PXONLINETASK_HANDLE)&task->handle)))
			{
				task->flags = 1;
				task->type = 0x12;
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

/* starts writing the given statistics */
// @retail 0x923c0
long online_stats_write(XONLINE_STAT_SPEC const *specs, word count)
{
	long task_index = online_task_new_if_logged_on();
	if (task_index != NONE)
	{
		s_online_task *task = online_task_try_and_get(task_index);
		if (task)
		{
			if (SUCCEEDED(XOnlineStatWrite(count, specs, NULL, (PXONLINETASK_HANDLE)&task->handle)))
			{
				task->flags = 1;
				task->type = 0x13;
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

/* whether the task failed because the service is not available */
// @retail 0x92750
bool online_task_service_unavailable(long task_index)
{
	bool result = false;
	s_online_task *task = online_task_try_and_get(task_index);
	if (online_task_continue(task) == 0x8015b108)
		result = true;
	return result;
}
