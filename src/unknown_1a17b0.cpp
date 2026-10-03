// @flags /O2 /Gr
/* UNKNOWN_1A17B0.CPP: asynchronous tasks that run a work function with a copy
   of its parameters on the job thread (unknown_11fc80.cpp) */

#include "cseries.h"
#include <xtl.h>
#include <string.h>
#include "job_queue.h"

long g_4e28f0;

s_job_node *function_1208b0(void);
long function_1209c0(s_job_node *node);

// @retail 0x1a17b0
PRIVATE long __stdcall async_task_execute_work(s_async_task *task)
{
	return task->callback(task, task->parameters, task->parameters_size);
}

// @retail 0x1a17d0
long async_task_add_work(async_work_callback callback, long parameters_size, void *parameters, long priority, bool *done)
{
	long result;
	s_async_task task;

	memset(&task, 0, sizeof(task));
	if (parameters_size <= sizeof(task.parameters))
	{
		task.callback = callback;
		memcpy(task.parameters, parameters, parameters_size);
		task.parameters_size = (char)parameters_size;

		g_4e28f0++;
		*done = false;
		s_job_node *node = function_1208b0();
		node->task = task;
		node->done = done;
		node->priority = priority;
		node->function = async_task_execute_work;
		node->unknown30 = 0;
		result = function_1209c0(node);
	}
	return result;
}
