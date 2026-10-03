// @flags /O2 /Gr
/* UNKNOWN_1A17B0.CPP: asynchronous tasks that run a work function with a copy
   of its parameters on the job thread (async.cpp) */

#include "cseries.h"
#include <xtl.h>
#include <string.h>
#include "async.h"

s_job_node *function_1208b0(void);

// @retail 0x1a17b0
PRIVATE long __stdcall async_task_execute_work(s_async_task *task)
{
	return task->work.callback(task, task->work.parameters, task->work.parameters_size);
}

// @retail 0x1a17d0
long async_task_add_work(async_task_work_function callback, long parameters_size, void *parameters, long priority, bool *done)
{
	long result;
	s_async_task task;

	memset(&task, 0, sizeof(task));
	if (parameters_size <= sizeof(task.work.parameters))
	{
		task.work.callback = callback;
		memcpy(task.work.parameters, parameters, parameters_size);
		task.work.parameters_size = (char)parameters_size;

		async_globals.tasks_added++;
		*done = false;
		s_job_node *node = function_1208b0();
		node->task = task;
		node->done = done;
		node->priority = priority;
		node->callback = async_task_execute_work;
		node->category = 0;
		result = async_task_queue(node);
	}
	return result;
}
