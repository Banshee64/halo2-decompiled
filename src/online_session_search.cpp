// @flags /O2 /arch:SSE /Gr
/* ONLINE_SESSION_SEARCH.CPP: the Xbox Live matchmaking session searches: the
   online tasks that create, find and delete match sessions, the results of a
   search, and the session identifiers already seen. */

#include "unknown_11c920.h"
#include "globals.h"
#include "online_tasks.h"
#include "online_attributes.h"
#include "unknown_0662e0.h"
#include <xtl.h>
#include <xonline.h>
#include <string.h>
#include <stdlib.h>

#define MAXIMUM_SEARCH_RESULTS 50

void function_6b640(long task_index);
long online_task_new_if_logged_on(void);
void qos_release(long handle);

/* the release routine retail inlines here */
static inline void free_block(void *block)
{
	long info;
	g_4d87f8->allocator->get_info(block, &info);
	s_allocator_globals *globals = g_4d87f8;
	globals->allocator->release(block, NONE);
	if (block != 0)
		globals->count--;
}

static inline s_type_9df9da *online_task_try_and_get(long task_index)
{
	s_type_9df9da *task = 0;
	if (task_index != NONE)
	{
		s_record_pool *data = g_4cf78c;
		long absolute_index = task_index & 0xffff;
		if (absolute_index < data->high_water_index)
		{
			s_type_9df9da *candidate = (s_type_9df9da *)(data->data + data->size * absolute_index);
			if (candidate->salt != 0 && candidate->salt == (task_index >> 16))
				task = candidate;
		}
	}
	return task;
}

/* a session a search found (0x204 bytes) */
struct s_search_result
{
	bool valid;
	bool unknown01;
	bool seen;
	byte unknown03;
	long unknown04;
	bool unknown08;
	byte unknown09[3];
	byte search_result[0x68];
	bool unknown74;
	bool unknown75;
	byte unknown76[2];
	long unknown78;
	long unknown7c;
	long unknown80;
	byte unknown84[0x90 - 0x84];
	long field90;
	byte unknown94[0xaa - 0x94];
	short session_kind;
	byte unknownac[0xbc - 0xac];
	XNKID session_id;
	XNKEY session_key;
	XNADDR session_address;
	byte unknownf8[0x10c - 0xf8];
	long field10c;
	byte unknown110[0x1f8 - 0x110];
	long field1f8;
	long field1fc;
	long field200;
};

/* a session search */
struct s_session_search
{
	bool active;
	byte unknown01[3];
	dword start_time;
	long state;
	byte unknown0c[8];
	bool flag14;
	byte unknown15[7];
	long unknown1c;
	long unknown20;
	byte unknown24[0x34 - 0x24];
	long minimum_score;
	byte unknown38[0x70 - 0x38];
	long task_index;
	long qos_handles[2];
	long qos_counts[2];
	long result_count;
	long unknown88;
	s_search_result *results;
	void *unknown90;
	long seen_count;
	long seen_capacity;
	long unknown9c;
	XNKID *seen;
};

/* the parts of a session a search compares */
struct s_search_session
{
	long kind;
	XNKID id;
	XNKEY key;
	XNADDR address;
	long value;
};

long g_4ce394;
long g_4ce118;
long g_4ce11c;
long g_4ce200[10];
long g_4ce230;
long g_4ce234;
long g_4ce238;
long g_4ce23c[9][9];
long g_4ce380;
long g_4ce384;
double g_4ce388;
long g_4ce390;

extern bool g_4cf792;
extern XNADDR g_4cf793;
bool function_07a9b0(void);

#define SEARCH_LONG(object, offset) (*(long *)((byte *)(object) + (offset)))
#define SEARCH_FLAG(object, offset) (*(bool *)((byte *)(object) + (offset)))

typedef bool (__stdcall *t_search_compare)(void const *, void const *, void const *);
void function_13da70(void *elements, unsigned long count, unsigned long element_size, t_search_compare compare, void const *context);

// @retail 0x90020
long online_match_session_delete(s_search_session const *session, bool *unavailable)
{
	bool service_unavailable = false;
	online_task_exists(8, 0xff);
	long task_index = online_task_new_if_logged_on();
	if (task_index != NONE)
	{
		s_type_9df9da *task = online_task_try_and_get(task_index);
		if (task)
		{
			HRESULT result = XOnlineMatchSessionDelete(session->id, NULL, (PXONLINETASK_HANDLE)&task->handle);
			if (SUCCEEDED(result))
			{
				task->flags = 1;
				task->type = 8;
				task->controller_index = NONE;
				*unavailable = service_unavailable;
				return task_index;
			}
			if (result == 0x80155100)
				service_unavailable = true;
			function_6b640(task_index);
			task_index = NONE;
		}
	}
	*unavailable = service_unavailable;
	return task_index;
}

// @retail 0x900e0
long online_match_session_find(XNKID const *session_id)
{
	if (online_task_exists(9, 0xff) > 2)
		return NONE;
	long task_index = online_task_new_if_logged_on();
	s_type_9df9da *task = function_6b910(task_index);
	if (task)
	{
		if (FAILED(XOnlineMatchSessionFindFromID(*session_id, NULL, (PXONLINETASK_HANDLE)&task->handle)))
		{
			function_6b640(task_index);
			return NONE;
		}
		task->flags = 1;
		task->type = 9;
		task->controller_index = NONE;
	}
	return task_index;
}

// @retail 0x90c80
void __stdcall function_090c80(byte *p)
{
	byte *const *reference = &p;
	s_session_search *search = (s_session_search *)*reference;
	if (search->state == 1)
		search->state = 2;
	if (search->task_index != NONE)
	{
		function_6b640(search->task_index);
		search->task_index = NONE;
	}
	if (search->qos_handles[0] != NONE)
	{
		qos_release(search->qos_handles[0]);
		search->qos_handles[0] = NONE;
	}
	if (search->qos_handles[1] != NONE)
	{
		qos_release(search->qos_handles[1]);
		search->qos_handles[1] = NONE;
	}
	if (search->unknown90)
	{
		free_block(search->unknown90);
		search->unknown90 = 0;
	}
	if (search->results)
	{
		free_block(search->results);
		search->results = 0;
	}
	if (search->seen)
	{
		free_block(search->seen);
		search->seen = 0;
	}
	search->active = false;
}


// @retail 0x91290
bool session_search_seen(s_session_search *search, XNKID const *id)
{
	bool seen = false;
	for (long i = 0; i < search->seen_count && !seen; i++)
	{
		if (memcmp(&search->seen[i], id, sizeof(XNKID)) == 0)
			seen = true;
	}
	return seen;
}

/* the session identifier in a result */
static inline XNKID const *search_result_id(s_search_result const *result)
{
	return (XNKID const *)(result->search_result + 0x10);
}

// @retail 0x912d0
void session_search_mark_seen(s_session_search *search, s_search_result *result)
{
	if (!result->seen)
	{
		XNKID const *id = search_result_id(result);
		result->seen = true;
		result->unknown01 = false;
		if (!session_search_seen(search, id) && search->seen_count < search->seen_capacity)
		{
			search->seen[search->seen_count] = *id;
			search->seen_count++;
		}
	}
}

// @retail 0x91320
void session_search_mark_session(s_session_search *search, s_search_session const *session)
{
	if (search->active)
	{
		for (s_search_result *result = search->results; result < search->results + MAXIMUM_SEARCH_RESULTS; result++)
		{
			if (result->valid && result->unknown74 && memcmp(&session->id, (byte *)result + 0xbc, sizeof(XNKID)) == 0)
			{
				session_search_mark_seen(search, result);
				return;
			}
		}
	}
}

// @retail 0x91380
void session_search_get_progress(s_session_search *search, long *first, long *last, long *count, long *total)
{
	if (search->active)
	{
		long start;
		long end;
		if (search->flag14)
		{
			end = search->unknown20;
			start = search->unknown1c;
		}
		else
		{
			long current = search->unknown20;
			long previous = search->unknown1c;
			end = current + 1;
			start = previous;
			if (previous == NONE)
				start = current;
		}
		long result_count = 0;
		long result_total = 0;
		for (s_search_result *result = search->results; result < search->results + MAXIMUM_SEARCH_RESULTS; result++)
		{
			if (result->valid)
			{
				result_count++;
				result_total += *(long *)((byte *)result + 0x48);
			}
		}
		*first = start;
		*last = end;
		*count = result_count;
		*total = result_total;
	}
}

// @retail 0x90f10
bool __stdcall session_search_result_precedes(void const *a, void const *b, void const *context)
{
	s_search_result const *first = (s_search_result const *)a;
	s_search_result const *second = (s_search_result const *)b;
	if (first->field1f8 < second->field1f8)
		return true;
	if (first->field1f8 > second->field1f8)
		goto failed;
	if (first->unknown74)
	{
		if (first->field90 > second->field90)
			return true;
		if (first->field90 < second->field90)
			goto failed;
	}
	if (first->field1fc > second->field1fc)
		return true;
	if (first->field1fc < second->field1fc)
		goto failed;
	if (first->field10c > second->field10c)
		return true;
failed:
	return false;
}

static inline bool session_search_version_allowed(long type, long lower, long upper)
{
	return type == 4 && lower >= 0x2651 && upper <= 0x2651;
}

// @retail 0x90d90
bool __stdcall session_search_result_allowed(s_session_search *search, s_search_result *entry)
{
	volatile bool result = true;
	long mask;
	long minimum;
	long maximum;
	if (SEARCH_FLAG(entry, 0xa4))
	{
		mask = SEARCH_LONG(entry, 0x110);
		minimum = SEARCH_LONG(entry, 0x12c);
		maximum = SEARCH_LONG(entry, 0x124);
	}
	else
	{
		mask = SEARCH_LONG(entry, 0x70);
		minimum = SEARCH_LONG(entry, 0x64);
		maximum = SEARCH_LONG(entry, 0x68);
	}
	if (SEARCH_FLAG(entry, 0xa4))
	{
		long lower = SEARCH_LONG(entry, 0xb0);
		long upper = SEARCH_LONG(entry, 0xb4);
		if (!session_search_version_allowed(SEARCH_LONG(entry, 0xac), lower, upper))
			return false;
	}
	if (g_transport_globals.initialized && g_transport_globals.started)
		function_07a9b0();
	bool address_valid = g_4cf792;
	XNADDR address = g_4cf793;
	if (address_valid && memcmp(&address, (byte *)entry + 0x24, sizeof(address)) == 0)
		return false;
	if (SEARCH_FLAG(entry, 0xa4) && SEARCH_FLAG(search, 0x15))
	{
		long count = SEARCH_LONG(entry, 0x130);
		if (count > 0)
		{
			for (long i = 0; i < count; i++)
			{
				if (memcmp((byte *)entry + 0x134 + i * 12, (byte *)search + 0x3d, 12) == 0)
					result = false;
			}
			if (!result)
				goto done;
		}
	}
	if (!(mask & (1 << (SEARCH_LONG(search, 0x38) - 1))))
		result = false;
	if (SEARCH_FLAG(search, 0x2c) && minimum < SEARCH_LONG(search, 0x30))
		result = false;
	if (SEARCH_FLAG(search, 0x24) && maximum > SEARCH_LONG(search, 0x28))
		return false;
done:
	return result;
}

// @retail 0x91000
void __stdcall session_search_score_and_sort(s_session_search *search)
{
	if (memcmp(search->unknown90, search->results, search->unknown88) == 0)
		return;
	for (s_search_result *entry = search->results; entry < search->results + MAXIMUM_SEARCH_RESULTS; entry++)
	{
		entry->field1f8 = 0;
		if (entry->valid && entry->unknown01 && !session_search_result_allowed(search, entry))
			entry->unknown01 = false;
		if (entry->valid && entry->unknown01)
		{
			bool has_details = SEARCH_FLAG(entry, 0xa4);
			long kind;
			long value;
			long count;
			if (has_details)
			{
				kind = entry->field10c;
				value = SEARCH_LONG(entry, 0x120);
				count = SEARCH_LONG(entry, 0x11c);
			}
			else
			{
				kind = SEARCH_LONG(entry, 0x60);
				value = SEARCH_LONG(entry, 0x5c);
				count = SEARCH_LONG(entry, 0x6c);
			}
			entry->field1fc = abs(value - SEARCH_LONG(search, 0x4c));
			if (entry->field1fc <= g_4ce230)
				entry->field1f8 += g_4ce234 - ((g_4ce234 - g_4ce238) / (g_4ce230 + 1)) * entry->field1fc;
			if (kind == SEARCH_LONG(search, 0x38))
				entry->field1f8 += g_4ce380;
			long bounded = count < 0 ? 0 : (count > g_4ce384 ? g_4ce384 : count);
			entry->field200 = bounded;
			entry->field1f8 = (long)(entry->field1f8 + bounded * g_4ce388);
			if (has_details)
			{
				if ((SEARCH_LONG(entry, 0xf8) < g_4ce11c || SEARCH_LONG(entry, 0xfc) < g_4ce118) &&
					SEARCH_LONG(search, 0x58) >= g_4ce11c && SEARCH_LONG(search, 0x5c) >= g_4ce118)
					entry->field1f8 += g_4ce390;
				entry->field1f8 += g_4ce23c[SEARCH_LONG(search, 0x6c)][SEARCH_LONG(entry, 0xb8)];
			}
			if (entry->unknown74)
			{
				real value = entry->field90 * 0.01f;
				value = value < 0.0f ? 0.0f : (value > 9.0f ? 9.0f : value);
				long index = (long)value;
				long score;
				if (index < 9)
				{
					real lower = (real)g_4ce200[index];
					real upper = (real)g_4ce200[index + 1];
					score = (long)(lower + (value - index) * (upper - lower));
				}
				else
					score = g_4ce200[9];
				entry->field1f8 += score;
			}
		}
	}
	function_13da70(search->results, MAXIMUM_SEARCH_RESULTS, sizeof(s_search_result), session_search_result_precedes, 0);
	memcpy(search->unknown90, search->results, search->unknown88);
}

static inline long session_search_elapsed(dword start_time)
{
	return (long)((g_510548 ? g_51054c : GetTickCount()) - start_time);
}

// @retail 0x91400
bool session_search_select(s_session_search *search, s_search_session *session)
{
	bool result = false;
	s_search_result *selected = 0;
	long candidate_count = 0;
	long ready_count = 0;
	if (search->active)
	{
		for (s_search_result *entry = search->results; entry < search->results + MAXIMUM_SEARCH_RESULTS; entry++)
		{
			if (entry->valid)
			{
				result = entry->unknown01;
				if (result)
				{
					candidate_count++;
					result = entry->unknown74;
					if (result)
					{
						ready_count++;
						if (entry->field1f8 >= search->minimum_score && selected == 0)
							selected = entry;
					}
				}
			}
		}
		if (selected && (ready_count == candidate_count ||
			(ready_count >= (candidate_count >> 2) &&
			session_search_elapsed(search->start_time) > g_4ce394)))
		{
			if (session)
			{
				memset(session, 0, sizeof(*session));
				session->kind = selected->session_kind;
				session->id = selected->session_id;
				session->key = selected->session_key;
				session->address = selected->session_address;
				session->value = selected->field90;
			}
			return true;
		}
		return false;
	}
	return result;
}

struct s_90160
{
	XNKEY field_0;
	XNKID field_10;
	XNADDR field_18;
	long field_3c;
	long field_40;
	long field_44;
	long field_48;
	long field_4c[7];
};

struct s_long7;
void function_0b4a40(s_long7 *arg_0, const __int64 *arg_1);

// @retail 0x90160
bool function_90160(long arg_0, s_90160 *arg_1)
{
	s_type_9df9da *local_0 = online_task_try_and_get(arg_0);
	bool local_1 = false;
	if (local_0 && online_logon_connected() && (local_0->flags & 4))
	{
		PXONLINE_MATCH_SEARCHRESULT *local_2;
		DWORD local_3;
		if (SUCCEEDED(XOnlineMatchSearchGetResults((XONLINETASK_HANDLE)local_0->handle, &local_2, &local_3)) && local_3 > 0)
		{
			memset(arg_1, 0, sizeof(*arg_1));
			arg_1->field_18 = local_2[0]->HostAddress;
			arg_1->field_0 = local_2[0]->KeyExchangeKey;
			arg_1->field_10 = local_2[0]->SessionID;
			arg_1->field_3c = local_2[0]->dwPublicFilled;
			arg_1->field_40 = local_2[0]->dwPublicOpen;
			arg_1->field_44 = local_2[0]->dwPrivateFilled;
			arg_1->field_48 = local_2[0]->dwPrivateOpen;
			local_1 = true;
			if (local_2[0]->dwNumAttributes == 7)
			{
				__int64 local_4[7];
				static const XONLINE_ATTRIBUTE_SPEC local_5[7] =
				{
					{ X_ATTRIBUTE_DATATYPE_INTEGER, sizeof(ULONGLONG) },
					{ X_ATTRIBUTE_DATATYPE_INTEGER, sizeof(ULONGLONG) },
					{ X_ATTRIBUTE_DATATYPE_INTEGER, sizeof(ULONGLONG) },
					{ X_ATTRIBUTE_DATATYPE_INTEGER, sizeof(ULONGLONG) },
					{ X_ATTRIBUTE_DATATYPE_INTEGER, sizeof(ULONGLONG) },
					{ X_ATTRIBUTE_DATATYPE_INTEGER, sizeof(ULONGLONG) },
					{ X_ATTRIBUTE_DATATYPE_INTEGER, sizeof(ULONGLONG) }
				};
				if (SUCCEEDED(XOnlineMatchSearchParse(local_2[0], local_2[0]->dwNumAttributes, local_5, local_4)))
					function_0b4a40((s_long7 *)arg_1->field_4c, local_4);
			}
		}
	}
	return local_1;
}

long online_match_search(const s_range_input *arg_0);

struct s_qos_target
{
	XNKID kid;
	XNKEY key;
	XNADDR xna;
};

long qos_lookup(long kind, long count, long bits_per_second, s_qos_target *targets);

// @retail 0x90420
void __stdcall function_90420(s_session_search *arg_0, long arg_1)
{
	if (arg_0->qos_handles[arg_1] == NONE)
	{
		long local_0 = 0;
		s_qos_target local_1[MAXIMUM_SEARCH_RESULTS];
		for (long local_2 = 0; local_2 < MAXIMUM_SEARCH_RESULTS; local_2++)
		{
			s_search_result *local_3 = &arg_0->results[local_2];
			if (local_3->valid && local_3->unknown01)
			{
				bool local_4 = false;
				if (!((bool *)&local_3->unknown74)[arg_1])
					local_4 = true;
				if (arg_1 == 0)
				{
					if (local_3->unknown74 && session_search_elapsed(SEARCH_LONG(local_3, 0x1f4)) > g_network_configuration.value35c)
						local_4 = true;
					local_4 = local_4 && local_3->unknown75;
				}
				if (local_4)
				{
					local_1[local_0].kid = *(XNKID *)(local_3->search_result + 0x10);
					local_1[local_0].key = *(XNKEY *)local_3->search_result;
					local_1[local_0].xna = *(XNADDR *)(local_3->search_result + 0x18);
					local_3->unknown78 = arg_1;
					local_3->unknown7c = local_0;
					local_0++;
					local_3->unknown80 = 0;
				}
			}
		}
		if (local_0 > 0)
		{
			arg_0->qos_handles[arg_1] = qos_lookup(arg_1, local_0, g_network_configuration.value358, local_1);
			if (arg_0->qos_handles[arg_1])
				arg_0->qos_counts[arg_1] = local_0;
			else
				arg_0->state = 5;
		}
	}
}

// @retail 0x90b30
void function_90b30(s_session_search *arg_0)
{
	if (arg_0->unknown1c != NONE)
	{
		SEARCH_FLAG(arg_0, 0x2c) = true;
		SEARCH_LONG(arg_0, 0x30) = SEARCH_LONG(arg_0, 0x68) - arg_0->unknown1c < SEARCH_LONG(arg_0, 0x60) ? SEARCH_LONG(arg_0, 0x60) : SEARCH_LONG(arg_0, 0x68) - arg_0->unknown1c;
		SEARCH_FLAG(arg_0, 0x24) = true;
		SEARCH_LONG(arg_0, 0x28) = SEARCH_LONG(arg_0, 0x50) + arg_0->unknown1c > SEARCH_LONG(arg_0, 0x64) ? SEARCH_LONG(arg_0, 0x64) : SEARCH_LONG(arg_0, 0x50) + arg_0->unknown1c;
	}
	else
	{
		SEARCH_FLAG(arg_0, 0x2c) = false;
		SEARCH_FLAG(arg_0, 0x24) = false;
	}
	if (arg_0->unknown1c != NONE && arg_0->unknown20 > 0)
		arg_0->minimum_score = (long)((real)g_network_configuration.value1e8 + ((real)arg_0->unknown1c / (real)arg_0->unknown20) * ((real)g_network_configuration.value1ec - (real)g_network_configuration.value1e8));
	else
		arg_0->minimum_score = g_network_configuration.value1ec;
	s_range_input local_0;
	memset(&local_0, 0, sizeof(local_0));
	local_0.x = SEARCH_LONG(arg_0, 0x10);
	local_0.y = SEARCH_LONG(arg_0, 0x4c);
	local_0.count = SEARCH_LONG(arg_0, 0x38);
	local_0.has_min = SEARCH_FLAG(arg_0, 0x2c);
	local_0.has_max = SEARCH_FLAG(arg_0, 0x24);
	local_0.max = SEARCH_LONG(arg_0, 0x28);
	local_0.min = SEARCH_LONG(arg_0, 0x30);
	memset(arg_0->results, 0, arg_0->unknown88);
	memset(arg_0->unknown90, 0, arg_0->unknown88);
	local_0.has_count = true;
	arg_0->task_index = online_match_search(&local_0);
	if (arg_0->task_index == NONE)
		arg_0->state = 4;
	else
		arg_0->start_time = g_510548 ? g_51054c : GetTickCount();
}

// @retail 0x90f70
void function_90f70(s_session_search *arg_0)
{
	if (arg_0->state != 0 && arg_0->task_index == NONE && arg_0->qos_handles[0] == NONE &&
		arg_0->qos_handles[1] == NONE && !session_search_select(arg_0, 0))
	{
		long local_0 = arg_0->result_count;
		long local_2 = SEARCH_LONG(arg_0, 0xc) + local_0;
		SEARCH_LONG(arg_0, 0xc) = local_2;
		bool local_1 = false;
		if (SEARCH_LONG(arg_0, 0xc) < g_network_configuration.value1b8)
		{
			if (arg_0->unknown1c != NONE)
			{
				if (arg_0->unknown1c < arg_0->unknown20)
				{
					arg_0->unknown1c++;
					local_1 = true;
				}
				else if (arg_0->unknown1c == arg_0->unknown20 && !arg_0->flag14)
				{
					arg_0->unknown1c = NONE;
					local_1 = true;
				}
			}
			if (local_0 != MAXIMUM_SEARCH_RESULTS && !local_1)
				arg_0->state = 0;
			else
				function_90b30(arg_0);
		}
		else
			arg_0->state = 0;
	}
}
