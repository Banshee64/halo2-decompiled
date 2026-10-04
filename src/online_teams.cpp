#include "cseries.h"
#include <xtl.h>
#include <xonline.h>
#include "data_array.h"
#include "globals.h"
#include "online_tasks.h"

// @flags /O2 /Gr

/* ONLINE_TEAMS.CPP: the online tasks that change a team (clan) and its
   members: delete the team (type 0x19), answer an invitation (0x1d), set a
   member's rank (0x1e) and remove a member (0x1f) (UI lane; the open range
   next to 0xabc70) */

/* online_task_try_get with the salt taken first */
static inline s_online_task *online_task_try_get_salted(long task_index)
{
	s_online_task *result = 0;

	if (task_index != NONE)
	{
		s_data_array *data = g_4cf78c;
		long index = task_index & 0xffff;
		long salt = task_index >> 16;

		if (index < data->high_water_index)
		{
			byte *datum = data->data + data->size * index;

			if (*(short *)datum != 0 && *(short *)datum == salt)
			{
				result = (s_online_task *)datum;
			}
		}
	}

	return result;
}

// @retail 0xabf10
long online_team_delete(XUID const *team, long controller_index)
{
	long task_index = online_task_new_if_logged_on();

	if (task_index != NONE)
	{
		s_online_task *task = online_task_try_get(task_index);

		if (task)
		{
			if (SUCCEEDED(XOnlineTeamDelete(controller_index, *team, NULL, (XONLINETASK_HANDLE *)&task->handle)))
			{
				task->flags = 1;
				task->type = 0x19;
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

/* answer: 0 accept, 1 decline, otherwise never */
// @retail 0xabfa0
long online_team_answer_recruit(XUID const *team, long controller_index, long answer)
{
	long task_index = online_task_new_if_logged_on();

	if (task_index != NONE)
	{
		s_online_task *task = online_task_try_get_salted(task_index);

		if (task)
		{
			XONLINE_PEER_ANSWER_TYPE type;

			switch (answer)
			{
			case 0:
				type = XONLINE_PEER_ANSWER_NO;
				break;
			case 1:
				type = XONLINE_PEER_ANSWER_YES;
				break;
			default:
				type = XONLINE_PEER_ANSWER_NEVER;
				break;
			}
			if (SUCCEEDED(XOnlineTeamMemberAnswerRecruit(controller_index, *team, type, NULL, (XONLINETASK_HANDLE *)&task->handle)))
			{
				task->flags = 1;
				task->type = 0x1d;
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

/* the game keeps a member's rank (0..3) where Live keeps the privileges */
// @retail 0xac360
long online_team_member_set_rank(long controller_index, XUID const *team, XONLINE_TEAM_MEMBER const *member)
{
	long task_index = online_task_new_if_logged_on();

	if (task_index != NONE)
	{
		s_online_task *task = online_task_try_get_salted(task_index);

		if (task)
		{
			XONLINE_TEAM_MEMBER_PROPERTIES properties = member->TeamMemberProperties;

			properties.dwPrivileges = 0;
			switch (member->TeamMemberProperties.dwPrivileges)
			{
			case 3:
				properties.dwPrivileges = 0xffffffff;
				break;
			case 2:
				properties.dwPrivileges |= 0xc;
			case 1:
				properties.dwPrivileges |= 0x10;
			case 0:
				break;
			default:
				__assume(0);
			}
			if (SUCCEEDED(XOnlineTeamMemberSetProperties(controller_index, *team, member->xuidTeamMember, &properties, NULL, (XONLINETASK_HANDLE *)&task->handle)))
			{
				task->flags = 1;
				task->type = 0x1e;
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

// @retail 0xac050
long online_team_member_remove(long controller_index, XUID const *team, XUID const *member)
{
	long task_index = online_task_new_if_logged_on();

	if (task_index != NONE)
	{
		s_online_task *task = online_task_try_get(task_index);

		if (task)
		{
			if (SUCCEEDED(XOnlineTeamMemberRemove(controller_index, *team, *member, NULL, (XONLINETASK_HANDLE *)&task->handle)))
			{
				task->flags = 1;
				task->type = 0x1f;
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
