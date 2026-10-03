// @flags /O2 /Gr
/* ONLINE_FEEDBACK.CPP: player feedback (online task type 22) and the new
   content check (type 13) (lane D) */

#include "cseries.h"
#include <xtl.h>
#include <xonline.h>
#include "online_tasks.h"

/* set when the new content check finds new content */
bool g_51055d;

// @retail 0x8d3d0
void online_feedback_send(const XUID *xuid, DWORD controller_index, long kind)
{
	XONLINE_FEEDBACK_TYPE types[10] =
	{
		XONLINE_FEEDBACK_POS_ATTITUDE,
		XONLINE_FEEDBACK_POS_SESSION,
		XONLINE_FEEDBACK_NEG_NICKNAME,
		XONLINE_FEEDBACK_NEG_GAMEPLAY,
		XONLINE_FEEDBACK_NEG_SCREAMING,
		XONLINE_FEEDBACK_NEG_HARASSMENT,
		XONLINE_FEEDBACK_NEG_LEWDNESS,
		(XONLINE_FEEDBACK_TYPE)12,
		(XONLINE_FEEDBACK_TYPE)11,
		(XONLINE_FEEDBACK_TYPE)10,
	};
	long task_index = online_task_new_if_logged_on();
	s_online_task *task = online_task_try_get(task_index);

	if (task)
	{
		XONLINE_FEEDBACK_PARAMS params;
		params.lpStringParam = NULL;
		if (SUCCEEDED(XOnlineFeedbackSend(controller_index, *xuid, types[kind], &params, NULL, (PXONLINETASK_HANDLE)&task->handle)))
		{
			task->flags = 9;
			task->type = 22;
			task->controller_index = controller_index;
		}
		else
		{
			online_task_dispose(task_index);
		}
	}
}

// @retail 0x8d4d0
long online_offering_check_new_content(void)
{
	long task_index = online_task_new_if_logged_on();

	if (task_index != NONE)
	{
		s_online_task *task = online_task_try_get(task_index);
		if (task)
		{
			g_51055d = false;
			if (SUCCEEDED(XOnlineOfferingIsNewContentAvailable(0xffffffff, NULL, (PXONLINETASK_HANDLE)&task->handle)))
			{
				task->flags = 9;
				task->type = 13;
				task->controller_index = NONE;
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
