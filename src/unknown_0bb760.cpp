#include "unknown_11c920.h"
#include "globals.h"
#include "object_iterator.h"

// @flags /O2 /arch:SSE /Gr

/* local views of the object, its header and the object list (the shared
   header data is s_object_header_data in globals.h; 0bad50 and 108a90 keep
   their own views of the same objects) */
struct s_object
{
	byte unknown00[4];
	union
	{
		dword flags;
		struct
		{
			dword : 14;
			dword flag14 : 1;
			dword flag15 : 1;
			dword : 16;
		};
	};
	byte unknown08[0x14];
	long next_index;
	long next_time;
	byte export24;
	byte export25;
	byte export_flags;
	byte unknown27[0xad];
	long unknownd4;
	byte unknownd8;
};

struct s_object_header
{
	short identifier;
	byte flag0 : 1;
	byte : 7;
	byte type;
	byte unknown04[4];
	s_object *object;
};

struct s_object_list
{
	byte unknown00[4];
	short count;
	short unknown06;
	long first_index;
	byte unknown0c[8];
	long time14;

};

long *g_4de2d0;
s_object_list *g_4de2f4;

struct s_scenario_kind_ab
{
	byte unknown00[0x10];
	short type;
};

// @retail 0xbc970
bool function_bc970(long object_index, long id, real *value_out)
{
	real value = 0.0f;
	bool result = false;
	if (object_index != NONE)
	{
		s_object *object = ((s_object_header *)g_4e0300->data)[object_index & 0xffff].object;
		s_scenario_kind_ab *scenario = (s_scenario_kind_ab *)g_4e0350;
		if ((object->export_flags & 8) || !scenario || scenario->type != 0)
		{
			switch (id)
			{
			case 0x0d0005ff: value = 1.0f; break;
			case 0x170005fa: value = object->export24 * (1.0f / 64.0f); break;
			case 0x170005fb: value = object->export25 * (1.0f / 32.0f); break;
			case 0x180005fc: if (!(object->export_flags & 1)) value = 1.0f; break;
			case 0x1d0005fd: value = (object->export_flags & 2) ? 1.0f : -1.0f; break;
			case 0x1d0005fe: value = (object->export_flags & 4) ? 1.0f : -1.0f; break;
			}
			result = true;
		}
	}
	*value_out = value;
	return result;
}

struct s_local_time
{
	long year;
	long month;
	long day;
	long hour;
	long minute;
	long second;
};

void function_139030(s_local_time *time);
long function_139090();

// @retail 0xbca70
real function_bca70()
{
	long stamp = g_4de2f4->time14;
	real result = 1.0f;
	if (stamp && g_510c54->game_time > stamp)
	{
		real elapsed = (g_510c54->game_time - stamp) * g_510c54->rate;
		if (elapsed > 1.0f)
			result = 0.0f;
		else if (elapsed > 20.0f)
			result = 1.0f;
		else
			result = (elapsed - 15.0f) * 0.2f;
	}
	return result;
}

// @retail 0xbcad0
real function_bcad0()
{
	s_local_time time;
	function_139030(&time);
	long hour = time.hour;
	if (hour < 1)
		hour = 1;
	else if (hour > 24)
		hour = 24;
	return (hour % 12) / 12.0f;
}

// @retail 0xbcb10
real function_bcb10()
{
	s_local_time time;
	function_139030(&time);
	long minute = time.minute;
	if (minute < 0)
		minute = 0;
	else if (minute > 59)
		minute = 59;
	return minute / 60.0f;
}

// @retail 0xbcb60
real function_bcb60()
{
	s_local_time time;
	function_139030(&time);
	long second = time.second;
	if (second < 0)
		second = 0;
	else if (second > 59)
		second = 59;
	return second / 60.0f;
}

// @retail 0xbcbb0
real function_bcbb0()
{
	/* Retail retains the scratch value's stores, including the initial zero. */
	volatile real result = 0.0f;
	switch (function_139090())
	{
	case 1: result = 0.25f; return 0.25f;
	case 2: result = 0.5f; return 0.5f;
	case 3: result = 0.75f; return 0.75f;
	case 4: result = 1.0f; break;
	}
	return result;
}

void function_108e10(long object_index);
void function_108e80(long object_index);
void function_108ef0(long object_index, long a);
void function_108f60(long object_index, long a);

// @retail 0xbb760
long function_bb760(short index)
{
	if (index >= 0 && index < 0x280)
		return g_4de2d0[index];
	return NONE;
}

// @retail 0xbb780
void function_bb780(long object_index, long value)
{
	((s_object_header *)g_4e0300->data)[object_index & 0xffff].object->unknownd4 = value;
	function_108e10(object_index);
}

// @retail 0xbb7b0
void function_bb7b0(long object_index)
{
	s_object *object = ((s_object_header *)g_4e0300->data)[object_index & 0xffff].object;
	function_108e80(object_index);
	object->unknownd4 = NONE;
	object->unknownd8 = 0;
}

// @retail 0xbb7f0
void function_bb7f0()
{
	s_type_f1af8e iterator;
	function_bae80(&iterator, 0, 0);
	s_object *object;
	while ((object = function_baeb0(&iterator)) != 0)
	{
		if (object->unknownd4 != NONE)
			function_bb7b0(iterator.object_index);
	}
}

// @retail 0xbb880
void __stdcall function_bb880(long a)
{
	struct
	{
		s_object *object;
		s_type_f1af8e iterator;
	} state;

	function_bae80(&state.iterator, 0, 0);
	while ((state.object = function_baeb0(&state.iterator)) != 0)
	{
		function_108ef0(state.iterator.object_index, a);
	}
}

// @retail 0xbb8f0
void function_bb8f0(long a)
{
	s_type_f1af8e iterator;
	function_bae80(&iterator, 0, 0);
	s_object *object = function_baeb0(&iterator);
	(void)&object;
	while (object)
	{
		function_108f60(iterator.object_index, a);
		object = function_baeb0(&iterator);
	}
}

// @retail 0xbb950
void function_bb950(long object_index, bool add, long delta)
{
	s_object_header *header = &((s_object_header *)g_4e0300->data)[object_index & 0xffff];
	s_object *object = header->object;

	if (add)
	{
		if (!TEST_FIELD_BIT(object->flag14) && !TEST_FIELD_BIT(object->flag15))
		{
			s_object_list *list = g_4de2f4;
			object->next_index = list->first_index;
			list->first_index = object_index;
			object->flag14 = 1;
			if (TEST_FIELD_BIT(header->flag0))
				list->count++;
		}
		long time = g_510c54->game_time;
		time += delta;
		long current = object->next_time;
		object->next_time = (current > time) ? current : time;
	}
	else if (TEST_FIELD_BIT(object->flag14))
	{
		s_object_list *list = g_4de2f4;
		long *link = &list->first_index;
		while (*link != object_index)
			link = &((s_object_header *)g_4e0300->data)[*link & 0xffff].object->next_index;
		*link = object->next_index;
		object->next_index = NONE;
		object->flag14 = 0;
		if (TEST_FIELD_BIT(header->flag0))
			list->count--;
	}
}
