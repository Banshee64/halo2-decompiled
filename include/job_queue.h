/* JOB_QUEUE.H: the worker job queue's nodes (a mutex-guarded free list and a
   used list of 150 nodes of 0x40 bytes; unknown_11fc80.cpp, unknown_120ce0.cpp)
   and the asynchronous tasks they run (unknown_1a17b0.cpp) */
#ifndef JOB_QUEUE_H
#define JOB_QUEUE_H

#include "cseries.h"

/* an asynchronous task: a work function and a copy of its parameters */
struct s_async_task;
typedef long (__stdcall *async_work_callback)(s_async_task *task, void *parameters, long parameters_size);

struct s_async_task
{
	async_work_callback callback;
	byte parameters[0x23];
	char parameters_size;
};

struct s_job_node
{
	long priority;
	long state;
	s_async_task task;
	long unknown30;
	long (__stdcall *function)(s_async_task *task);
	bool *done;
	s_job_node *next;
};

/* the shared read buffer of the job thread (unknown_11fc80.cpp) */
struct s_thread_stack
{
	dword unknown00;
	dword unknown04;
	dword unknown08;
};

extern s_thread_stack g_5020c8;

long async_task_add_work(async_work_callback callback, long parameters_size, void *parameters, long priority, bool *done);

#endif
