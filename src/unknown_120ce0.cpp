// @flags /O2 /Gr
/* UNKNOWN_120CE0.CPP: change the priority of a queued worker job. Decompiled
   by lane F for 0x12de70; the job queue itself is in unknown_11fc80.cpp,
   whose file this function probably shares. */

#include "cseries.h"
#include <xtl.h>

/* the worker job queue's nodes (as unknown_11fc80.cpp defines them) */
struct s_job_node
{
	long priority;
	long state;
	byte unknown08[0x34];
	s_job_node *next;
};

extern s_job_node g_4e0368[150];
extern s_job_node *g_4e28ec;
extern long g_4e035c;

void __stdcall function_120900(s_job_node *node);

// @retail 0x120ce0
bool function_120ce0(long job, long priority)
{
	bool result = false;
	s_job_node *node;

	WaitForSingleObject((HANDLE)g_4e035c, (DWORD)-1);
	node = &g_4e0368[(byte)job];
	if (node->state == job)
	{
		if (g_4e28ec == node)
		{
			g_4e28ec = node->next;
		}
		else
		{
			s_job_node *previous = g_4e28ec;
			while (previous->next != node)
				previous = previous->next;
			previous->next = node->next;
		}
		node->priority = priority;
		function_120900(node);
		result = true;
	}
	ReleaseMutex((HANDLE)g_4e035c);
	return result;
}
