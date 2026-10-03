/* ONLINE_TASKS.H: the online tasks (g_4cf78c) and the online account code
   that drives them (lane D) */

#ifndef ONLINE_TASKS_H
#define ONLINE_TASKS_H

#include "cseries.h"
#include "data_array.h"

/* a machine's network address */
struct s_online_address
{
	byte bytes[6];
};

/* one online task (0x14 bytes): flags bit 0 started, bit 1 running, bit 2
   finished, bit 5 failed */
struct s_online_task
{
	short salt;
	union
	{
		word flags;
		struct
		{
			word started : 1;
			word running : 1;
			word finished : 1;
			word unknown3 : 2;
			word failed : 1;
		} flag_bits;
	};
	long type;
	long controller_index;
	void *handle;
	long result;
};

/* the tasks: g_4cf78c (globals.h) */

void online_tasks_initialize(void);
long online_task_get_status(long task_index);
inline long online_task_new(void);
long online_task_get_type(long task_index);
long online_task_find(long type, long controller_index);
bool online_task_exists(long type, long controller_index);
s_online_task *online_task_get(long task_index);
long online_task_get_title(long task_index);
long online_task_get_description(long task_index);
void online_check_development_address(void);

/* the Live logon task (online_tasks.cpp) */
extern long g_467214;
long online_task_get_logon_status(long task_index);

static inline bool online_logon_connected(void)
{
	bool connected = false;
	if (g_467214 != NONE)
	{
		switch (online_task_get_logon_status(g_467214))
		{
		case 1:
			connected = true;
			break;
		}
	}
	return connected;
}

#endif
