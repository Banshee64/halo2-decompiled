#include "unknown_11c920.h"
#include <xtl.h>
#include <xonline.h>
#include "data_array.h"
#include "globals.h"
#include "online_tasks.h"

// @flags /O2 /Gr

/* UNKNOWN_0ABC70.CPP: the online task that enumerates the teams (clans) of a
   signed-in user */

// @retail 0xabc70
long function_abc70(long controller_index, XUID const *xuid)
{
	long task_index = online_task_new_if_logged_on();

	if (task_index != NONE)
	{
		s_type_9df9da *task = online_task_try_get(task_index);

		if (task)
		{
			if (SUCCEEDED(XOnlineTeamEnumerateByUserXUID(controller_index, *xuid, NULL, (XONLINETASK_HANDLE *)&task->handle)))
			{
				task->flags = 1;
				task->type = 0x17;
				task->controller_index = controller_index;
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
