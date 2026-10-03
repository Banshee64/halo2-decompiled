#include "cseries.h"
#include <xtl.h>
#include <xonline.h>
#include "data_array.h"
#include "globals.h"
#include "online_tasks.h"

// @flags /O2 /Gr

/* UNKNOWN_0ABC70.CPP: the online task that enumerates the teams (clans) of a
   signed-in user */

// @retail 0xabc70
long function_abc70(long controller_index, XONLINE_USER *user)
{
	long task_index = online_task_new_if_logged_on();

	if (task_index != NONE)
	{
		s_online_task *task = online_task_try_get(task_index);

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
