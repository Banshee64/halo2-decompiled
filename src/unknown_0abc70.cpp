#include "cseries.h"
#include <xtl.h>
#include <xonline.h>
#include "data_array.h"
#include "globals.h"
#include "online_tasks.h"

// @flags /O2 /Gr

/* UNKNOWN_0ABC70.CPP: the online task that enumerates the teams (clans) of a
   signed-in user */

long online_task_new_if_logged_on(void);
void online_task_dispose(long task_index);

/* retail inlines datum_get here, as online_tasks.cpp does */
static inline s_online_task *online_task_try_and_get(long task_index)
{
	s_online_task *result = 0;

	if (task_index != NONE)
	{
		s_data_array *data = g_4cf78c;
		long index = task_index & 0xffff;

		if (index < data->high_water_index)
		{
			byte *datum = data->data + data->size * index;
			short salt = *(short *)datum;

			if (salt != 0 && salt == (task_index >> 16))
			{
				result = (s_online_task *)datum;
			}
		}
	}

	return result;
}

// @retail 0xabc70
long function_abc70(long controller_index, XONLINE_USER *user)
{
	long task_index = online_task_new_if_logged_on();

	if (task_index != NONE)
	{
		s_online_task *task = online_task_try_and_get(task_index);

		if (task)
		{
			if (SUCCEEDED(XOnlineTeamEnumerateByUserXUID(controller_index, user->xuid, NULL, (XONLINETASK_HANDLE *)&task->handle)))
			{
				task->flags = 1;
				task->type = 0x17;
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
