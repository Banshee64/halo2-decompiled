/* JOB_QUEUE.H: the worker job queue's nodes (a mutex-guarded free list and a
   used list of 150 nodes of 0x40 bytes; unknown_11fc80.cpp, unknown_120ce0.cpp) */
#ifndef JOB_QUEUE_H
#define JOB_QUEUE_H

#include "cseries.h"

struct s_job_node
{
	long priority;
	long state;
	byte unknown08[0x34];
	s_job_node *next;
};

#endif
