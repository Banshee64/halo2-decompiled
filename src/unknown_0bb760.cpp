#include "cseries.h"
#include "globals.h"

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
	byte unknown24[0xb0];
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

struct s_object_iterator
{
	dword type_mask;
	byte flags;
	byte unknown05;
	short index;
	long object_index;
	long signature;
};

struct s_object_list
{
	byte unknown00[4];
	short count;
	short unknown06;
	long first_index;
};

long *g_4de2d0;
s_object_list *g_4de2f4;

s_object *function_baeb0(s_object_iterator *iterator);
void function_bae80(s_object_iterator *iterator, dword type_mask, byte flags);
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
	s_object_iterator iterator;
	function_bae80(&iterator, 0, 0);
	s_object *object;
	while ((object = function_baeb0(&iterator)) != 0)
	{
		if (object->unknownd4 != NONE)
			function_bb7b0(iterator.object_index);
	}
}

// @retail 0xbb880
void function_bb880(long a)
{
	s_object_iterator iterator;
	function_bae80(&iterator, 0, 0);
	while (function_baeb0(&iterator))
		function_108ef0(iterator.object_index, a);
}

// @retail 0xbb8f0
void function_bb8f0(long a)
{
	s_object_iterator iterator;
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
