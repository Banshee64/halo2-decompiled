// @flags /O2 /Gr
/* UNKNOWN_092220.CPP: Xbox Live online tasks that follow the team
   balancing (0x92220..0x927b0) */

#include "cseries.h"
#include "globals.h"
#include "online_tasks.h"
#include <xtl.h>

HRESULT online_task_continue(s_online_task *task);

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
