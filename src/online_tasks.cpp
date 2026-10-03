// @flags /O2 /Ob1 /Gr
/* ONLINE_TASKS.CPP: the online tasks (the data array g_4cf78c, 24 tasks of
   0x14 bytes) (lane D) */

#include "cseries.h"
#include <xtl.h>
#include <xonline.h>
#include <string.h>
#include "data_array.h"
#include "globals.h"
#include "online_tasks.h"

c_data_allocator *g_468758;
byte g_4771c8[0x2580];
long g_479748;

/* the local machine's address and the two development addresses it is
   compared with */
s_online_address g_4cf7cc;
const s_online_address g_43ff84[2] =
{
	{ 0x00, 0x0d, 0x3a, 0x5d, 0xd1, 0xf1 },
	{ 0x00, 0x50, 0xf2, 0x10, 0x52, 0x20 },
};
bool g_50944e;

/* retail inlines datum_get here (unknown_16b570.cpp is built /Ob1) */
static inline s_online_task *online_task_try_and_get(long task_index)
{
	s_online_task *result = 0;

	if (task_index != NONE)
	{
		s_data_array *data = g_4cf78c;
		long index = task_index & 0xffff;

		if (index < data->high_water_index)
		{
			byte *datum = data->data + data->size * index;
			short salt = *(short *)datum;

			if (salt != 0 && salt == (task_index >> 16))
			{
				result = (s_online_task *)datum;
			}
		}
	}

	return result;
}

// @retail 0x6b3e0
void online_tasks_initialize(void)
{
	g_4cf78c = data_new_inlined("online tasks", 24, sizeof(s_online_task), 0, g_468758);
	g_4cf78c->valid = true;
	data_delete_all(g_4cf78c);
	online_check_development_address();
	memset(g_4771c8, 0, sizeof(g_4771c8));
	g_479748 = NONE;
}

// @retail 0x6b5d0
long online_task_get_status(long task_index)
{
	s_online_task *task = online_task_try_and_get(task_index);

	long status;

	if (!task)
		status = 5;
	else if (task->flag_bits.finished)
		status = task->flag_bits.failed ? 3 : 2;
	else if (task->flag_bits.running)
		status = 1;
	else
		status = task->flag_bits.started ? 0 : 4;
	return status;
}

// @retail 0x6b6f0
inline long online_task_new(void)
{
	long task_index = datum_new(g_4cf78c);

	if (task_index != NONE)
	{
		s_online_task *task = (s_online_task *)g_4cf78c->data + (task_index & 0xffff);
		task->handle = 0;
		task->type = NONE;
		task->controller_index = NONE;
		task->flags = 0;
	}
	return task_index;
}

// @retail 0x6b7e0
long online_task_get_type(long task_index)
{
	return ((s_online_task *)g_4cf78c->data)[task_index & 0xffff].type;
}

// @retail 0x6b800
long online_task_find(long type, long controller_index)
{
	s_data_array *data = g_4cf78c;
	long index = NONE;

	for (;;)
	{
		long found = NONE;
		index++;
		if (index >= 0)
		{
			for (; index < data->high_water_index; index++)
			{
				if (data->bitmap[index >> 5] & (1 << (index & 0x1f)))
				{
					found = index;
					break;
				}
			}
		}
		index = found;
		if (index == NONE)
			break;

		s_online_task *task = (s_online_task *)(data->data + data->size * index);
		long task_index = (task->salt << 16) | index;
		if (task->type == type && (task->controller_index == controller_index || controller_index == NONE || controller_index == 0xff))
			return task_index;
	}
	return NONE;
}

// @retail 0x6b890
bool online_task_exists(long type, long controller_index)
{
	s_data_array *data = g_4cf78c;
	long index = NONE;

	for (;;)
	{
		long found = NONE;
		index++;
		if (index >= 0)
		{
			for (; index < data->high_water_index; index++)
			{
				if (data->bitmap[index >> 5] & (1 << (index & 0x1f)))
				{
					found = index;
					break;
				}
			}
		}
		index = found;
		if (index == NONE)
			break;

		s_online_task *task = (s_online_task *)(data->data + data->size * index);
		if (!task)
			break;
		if (task->type == type && (task->controller_index == controller_index || controller_index == NONE || controller_index == 0xff))
			return true;
	}
	return false;
}

// @retail 0x6b910
s_online_task *online_task_get(long task_index)
{
	return online_task_try_and_get(task_index);
}

// @retail 0x6ba80
long online_task_get_title(long task_index)
{
	s_online_task *task = online_task_try_and_get(task_index);
	long result = 0;

	if (task)
	{
		switch (task->type)
		{
		case 0:
			result = 0x700061f;
			break;
		case 1:
			result = 0x14000620;
			break;
		case 2:
			result = 0x11000621;
			break;
		case 3:
			result = 0x15000622;
			break;
		case 4:
			result = 0x10000623;
			break;
		case 5:
			result = 0x1c000624;
			break;
		case 6:
			result = 0xe000625;
			break;
		case 7:
			result = 0x10000626;
			break;
		case 8:
			result = 0x10000627;
			break;
		case 9:
			result = 0x16000628;
			break;
		case 10:
			result = 0x10000629;
			break;
		case 11:
			result = 0x1200062a;
			break;
		case 12:
			result = 0x1600062b;
			break;
		case 13:
			result = 0x1900062c;
			break;
		case 14:
			result = 0x1400062d;
			break;
		case 15:
			result = 0x1600062e;
			break;
		case 16:
			result = 0x1200062f;
			break;
		case 17:
			result = 0x11000630;
			break;
		case 18:
			result = 0xb000631;
			break;
		case 19:
			result = 0xb000632;
			break;
		case 20:
			result = 0xd000633;
			break;
		case 21:
			result = 0x13000634;
			break;
		case 22:
			result = 0x15000635;
			break;
		case 23:
			result = 0x11000636;
			break;
		case 24:
			result = 0xd000637;
			break;
		case 25:
			result = 0xd000638;
			break;
		case 26:
			result = 0xd000639;
			break;
		case 27:
			result = 0x1400063a;
			break;
		case 28:
			result = 0x2100063b;
			break;
		case 29:
			result = 0x1500063c;
			break;
		case 30:
			result = 0x1f00063d;
			break;
		case 31:
			result = 0x1400063e;
			break;
		case 32:
			result = 0x1800063f;
			break;
		case 33:
			result = 0xa000640;
			break;
		case 34:
			result = 0x10000641;
			break;
		case 35:
			result = 0xe000642;
			break;
		case 36:
			result = 0x1a000643;
			break;
		case 37:
			result = 0x1c000644;
			break;
		case 38:
			result = 0x1a000645;
			break;
		case 39:
			result = 0x1d000646;
			break;
		case 41:
			result = 0x1f000647;
			break;
		case 42:
			result = 0x21000648;
			break;
		case 43:
			result = 0x20000649;
			break;
		case 44:
			result = 0x2200064a;
			break;
		case 45:
			result = 0xf00064b;
			break;
		default:
			result = NONE;
			break;
		}
	}
	return result;
}

// @retail 0x6bd10
long online_task_get_description(long task_index)
{
	s_online_task *task = online_task_try_and_get(task_index);
	long result = 0;

	if (task)
	{
		switch (task->type)
		{
		case 0:
			result = 0x700064c;
			break;
		case 1:
			result = 0x1400064d;
			break;
		case 2:
			result = 0x1100064e;
			break;
		case 3:
			result = 0x1500064f;
			break;
		case 4:
			result = 0x10000650;
			break;
		case 5:
			result = 0x1c000651;
			break;
		case 6:
			result = 0xe000652;
			break;
		case 7:
			result = 0x10000653;
			break;
		case 8:
			result = 0x10000654;
			break;
		case 9:
			result = 0x16000655;
			break;
		case 10:
			result = 0x10000656;
			break;
		case 11:
			result = 0x12000657;
			break;
		case 12:
			result = 0x16000658;
			break;
		case 13:
			result = 0x19000659;
			break;
		case 14:
			result = 0x1400065a;
			break;
		case 15:
			result = 0x1600065b;
			break;
		case 16:
			result = 0x1200065c;
			break;
		case 17:
			result = 0x1100065d;
			break;
		case 18:
			result = 0xb00065e;
			break;
		case 19:
			result = 0xb00065f;
			break;
		case 20:
			result = 0xd000660;
			break;
		case 21:
			result = 0x13000661;
			break;
		case 22:
			result = 0x15000662;
			break;
		case 23:
			result = 0x11000663;
			break;
		case 24:
			result = 0xd000664;
			break;
		case 25:
			result = 0xd000665;
			break;
		case 26:
			result = 0xd000666;
			break;
		case 27:
			result = 0x14000667;
			break;
		case 28:
			result = 0x21000668;
			break;
		case 29:
			result = 0x15000669;
			break;
		case 30:
			result = 0x1f00066a;
			break;
		case 31:
			result = 0x1400066b;
			break;
		case 32:
			result = 0x1800066c;
			break;
		case 33:
			result = 0xa00066d;
			break;
		case 34:
			result = 0x1000066e;
			break;
		case 35:
			result = 0xe00066f;
			break;
		case 36:
			result = 0x1a000670;
			break;
		case 37:
			result = 0x1c000671;
			break;
		case 38:
			result = 0x1a000672;
			break;
		case 39:
			result = 0x1d000673;
			break;
		case 41:
			result = 0x1f000674;
			break;
		case 42:
			result = 0x21000675;
			break;
		case 43:
			result = 0x20000676;
			break;
		case 44:
			result = 0x22000677;
			break;
		case 45:
			result = 0xf000678;
			break;
		default:
			result = NONE;
			break;
		}
	}
	return result;
}

// @retail 0x6bfa0
void online_check_development_address(void)
{
	bool development = false;

	for (dword i = 0; i < sizeof(g_43ff84) / sizeof(g_43ff84[0]); i++)
	{
		if (!memcmp(&g_4cf7cc, &g_43ff84[i], sizeof(s_online_address)))
		{
			development = true;
			break;
		}
	}
	g_50944e = development;
}

/* not decompiled yet (src/stubs/lane_d.cpp) */
bool function_8d7c0(void);
void function_8c550(long task_index);

long g_467214 = NONE;
long g_467218 = 10;
bool g_50944f;

// @retail 0x6cd50
long online_task_get_logon_status(long task_index)
{
	s_online_task *task;
	if (task_index == NONE || (task = online_task_get(task_index)) == 0)
		return g_467218;
	if (task->type == 1 || task->flag_bits.failed)
		return task->result;

	void *handle = task->handle;
	if (!handle || handle == (void *)NONE)
	{
		g_467218 = 10;
		return 10;
	}
	if (!function_8d7c0())
	{
		task->flag_bits.failed = true;
		task->result = 5;
		g_467218 = 5;
		return 5;
	}

	switch (XOnlineLogonTaskGetResults((XONLINETASK_HANDLE)handle))
	{
	case 0x80151001:
		task->flag_bits.failed = true;
		task->result = 4;
		g_467218 = 4;
		return 4;
	case 0x80151002:
		task->flag_bits.failed = true;
		task->result = 3;
		g_467218 = 3;
		return 3;
	case 0x80151003:
		task->flag_bits.failed = true;
		task->result = 8;
		g_467218 = 8;
		return 8;
	case 0x80151004:
		task->flag_bits.failed = true;
		task->result = 5;
		g_467218 = 5;
		return 5;
	case 0x80151005:
		task->flag_bits.failed = true;
		task->result = 7;
		g_467218 = 7;
		return 7;
	case 0x80151006:
		task->flag_bits.failed = true;
		task->result = 6;
		g_467218 = 6;
		return 6;
	case XONLINE_E_LOGON_USER_ACCOUNT_REQUIRES_MANAGEMENT:
		task->flag_bits.failed = true;
		task->result = 2;
		g_467218 = 2;
		return 2;
	case S_OK:
		g_467218 = 0;
		return 0;
	case XONLINE_S_LOGON_CONNECTION_ESTABLISHED:
		g_467218 = 1;
		return 1;
	case XONLINE_S_LOGON_USER_HAS_MESSAGE:
		g_50944f = true;
	default:
		task->flag_bits.failed = true;
		task->result = 10;
		g_467218 = 10;
		return 10;
	}
}

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

// @retail 0x6c670
HRESULT online_task_continue(s_online_task *task)
{
	HRESULT result = E_FAIL;
	if (task && task->handle && task->handle != (void *)NONE)
	{
		if (task->type == 0 && function_8d7c0() || online_logon_connected())
			result = XOnlineTaskContinue((XONLINETASK_HANDLE)task->handle);
		else
			result = 0x80151000;
	}
	return result;
}

// @retail 0x6c450
void online_task_update(s_online_task *task)
{
	HRESULT result = online_task_continue(task);
	if (SUCCEEDED(result))
	{
		if (result == XONLINETASK_S_RESULTS_AVAIL)
			task->flags |= 2;
		else if (result != XONLINETASK_S_RUNNING && result != XONLINETASK_S_RUNNING_IDLE)
			task->flags = (task->flags & ~1) | 4;
	}
	else
	{
		task->flags = (task->flags & ~1) | 0x24;
	}
}

// @retail 0x6b640
void online_task_dispose(long task_index)
{
	s_online_task *task = online_task_try_and_get(task_index);
	if (task)
	{
		void *handle = task->handle;
		if (handle && handle != (void *)NONE)
		{
			switch (task->type)
			{
			case 2:
				{
					HRESULT result = S_OK;
					while (result != XONLINETASK_S_RUNNING_IDLE)
					{
						result = online_task_continue(task);
						if (FAILED(result))
							break;
					}
				}
				break;
			case 3:
				if (SUCCEEDED(XOnlineFriendsEnumerateFinish((XONLINETASK_HANDLE)handle)))
					online_task_continue(task);
				break;
			case 0x21:
				function_8c550(task_index);
				break;
			}
			XOnlineTaskClose((XONLINETASK_HANDLE)task->handle);
			task->handle = 0;
		}
		datum_delete(g_4cf78c, task_index);
	}
}

// @retail 0x6b730
long online_task_new_if_logged_on(void)
{
	if (online_logon_connected())
		return online_task_new();
	return NONE;
}

// @retail 0x6b780
void online_task_restart(long task_index)
{
	s_online_task *task = (s_online_task *)g_4cf78c->data + (task_index & 0xffff);
	long type = task->type;
	long controller_index = task->controller_index;
	online_task_dispose(task_index);
	long new_index = datum_new_at_index_with_salt(g_4cf78c, task_index);
	s_online_task *new_task = (s_online_task *)g_4cf78c->data + (new_index & 0xffff);
	new_task->type = type;
	new_task->controller_index = controller_index;
	new_task->flags = 0;
}

// @retail 0x6c7c0
bool online_get_logon_status(long *status)
{
	bool valid = g_467214 != NONE;
	*status = online_task_get_logon_status(g_467214);
	return valid;
}

// @retail 0x6c7e0
bool function_6c7e0()
{
	return online_logon_connected();
}

// @retail 0x6b550
long online_get_nat_type(void)
{
	long nat_type = 0;
	if (online_logon_connected())
	{
		switch (XOnlineGetNatType())
		{
		case XONLINE_NAT_OPEN:
			nat_type = 1;
			break;
		case XONLINE_NAT_MODERATE:
			nat_type = 2;
			break;
		case XONLINE_NAT_STRICT:
			nat_type = 3;
			break;
		}
	}
	return nat_type;
}

// @retail 0x6d040
void online_set_notification_state(const XNKID *session_id, DWORD user_index, BYTE *state_data, DWORD state_flags)
{
	if (online_logon_connected())
	{
		XNKID id;
		if (session_id)
			id = *session_id;
		else
			memset(&id, 0, sizeof(id));
		XOnlineNotificationSetState(user_index, state_flags, id, 4, state_data);
	}
}
