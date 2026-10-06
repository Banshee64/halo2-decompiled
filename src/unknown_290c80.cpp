// @flags /O2 /Gr
/* UNKNOWN_290C80.CPP: the ai's iterator over a chain of objects (an outside
   function lane I's handlers call) */

#include "unknown_11c920.h"
#include "unknown_2551c0.h"
#include "globals.h"
#include "object_iterator.h"

struct s_290c50_iterator
{
	long unknown00;
	s_record_pool_iterator records;
	long object_index;
};

// @retail 0x290c50
void function_290c50(s_290c50_iterator *iterator)
{
	if (g_4f55d0->active)
	{
		iterator->records.data = g_5044c8;
		iterator->records.index = NONE;
		iterator->records.datum_index = NONE;
		iterator->object_index = NONE;
	}
}

/* the ai data of an object, at the object's ai_offset */
struct s_object_ai_data
{
	union
	{
		word flags;
		struct { word attached : 1; word other_flags : 15; };
	};
	byte unknown02[2];
	long index04;
	long index08;
	long next_object_index;
	long index10;
	point3f position14;
	long index20;
	byte unknown24[0x4c - 0x24];
	short countdown;
	byte unknown4e[2];
	long index50;
	byte unknown54[2];
	bool flag56;
	bool position_pending;
	point3f position58;
	short index64;
	short count66;
	byte unknown68[0x74 - 0x68];
	point3f position74;
};

// @retail 0x28da50
void function_28da50(s_object_ai_data *data)
{
	data->flags = 0;
	data->index04 = NONE;
	data->index10 = NONE;
	data->next_object_index = NONE;
	data->index20 = NONE;
	data->index08 = NONE;
	data->position14 = *g_468788;
	data->index50 = NONE;
	data->flag56 = false;
	data->count66 = 0;
	data->index64 = NONE;
	data->position74 = *g_468788;
}

point3f *function_b9dd0(long object_index, point3f *result);

// @retail 0x28f890
void function_28f890(long object_index, s_object_ai_data *data)
{
	s_handler_object_view *object = handler_object_get(object_index);
	function_b9dd0(object_index, &data->position14);
	data->index20 = NONE;
	if (*(long *)((byte *)object + 0x14) != NONE)
		data->attached = true;
	else
		data->attached = false;
}

inline s_object_ai_data *object_ai_data(s_handler_object_view *object)
{
	return object->flags134 ? NULL : (s_object_ai_data *)((byte *)object + object->ai_offset);
}

// @retail 0x28fdf0
void function_28fdf0(long perception_index)
{
	long object_index = perception_get(perception_index)->object_index;
	while (object_index != NONE)
	{
		s_object_ai_data *data = object_ai_data(handler_object_get(object_index));
		if (!data)
			break;
		object_index = data->next_object_index;
		data->index20 = NONE;
	}
}

// @retail 0x290bf0
void function_290bf0(long perception_index, short team)
{
	long object_index = perception_get(perception_index)->object_index;
	while (object_index != NONE)
	{
		s_handler_object_view *object = handler_object_get(object_index);
		s_object_ai_data *data = object_ai_data(object);
		if (!data)
			break;
		*(short *)((byte *)object + 0x12e) = team;
		object_index = data->next_object_index;
	}
}

// @retail 0x28fd90
void function_28fd90(long perception_index, long index)
{
	long object_index = perception_get(perception_index)->object_index;
	while (object_index != NONE)
	{
		s_object_ai_data *data = object_ai_data(handler_object_get(object_index));
		if (!data)
			break;
		if (data->index10 == index)
			data->index10 = NONE;
		if (data->index50 == index)
			data->index50 = NONE;
		object_index = data->next_object_index;
	}
}

// @retail 0x290190
bool function_290190(long object_index, point3f *position)
{
	bool result = false;
	s_handler_object_view *object = handler_object_get(object_index);
	s_object_ai_data *data = object_ai_data(object);
	if (data && data->position_pending)
	{
		*position = data->position58;
		data->position_pending = false;
		result = true;
	}
	return result;
}

struct s_290b90_entry
{
	long key;
	byte unknown04[0x48 - 4];
	long perception_index;
	short count;
	byte unknown4e[2];
};

s_290b90_entry *g_5044c4;

// @retail 0x290b90
void function_290b90(long key)
{
	for (long i = 0; i < 5; i++)
	{
		if (g_5044c4[i].key == key)
		{
			if (g_5044c4[i].perception_index != NONE)
				*((byte *)perception_get(g_5044c4[i].perception_index) + 0x31) = false;
			g_5044c4[i].key = NONE;
			g_5044c4[i].perception_index = NONE;
			g_5044c4[i].count = 0;
		}
	}
}

// @retail 0x28e160
void function_28e160(long actor_index)
{
	s_handler_actor_view *actor = (s_handler_actor_view *)actor_get(actor_index);
	if (actor->perception_index != NONE)
	{
		long perception_index = actor->perception_index;
		s_perception_datum *perception = perception_get(perception_index);
		actor->perception_index = NONE;
		*(long *)((byte *)perception + 4) = NONE;
		long object_index = perception_get(perception_index)->object_index;
		while (object_index != NONE)
		{
			s_handler_object_view *object = handler_object_get(object_index);
			s_object_ai_data *data = object_ai_data(object);
			object_index = data ? data->next_object_index : NONE;
			data = object_ai_data(object);
			if (data)
				data->index04 = NONE;
		}
	}
}

struct s_28e5c0_object
{
	byte unknown00[0x12c];
	word bits0_4 : 5;
	word paused : 1;
	word bits6_15 : 10;
};

// @retail 0x28e5c0
void function_28e5c0(long object_index, s_object_ai_data *data)
{
	if (!data->position_pending)
	{
		s_28e5c0_object *object = (s_28e5c0_object *)object_get(object_index);
		if (!(bool)object->paused && data->countdown > 0)
			data->countdown--;
	}
}

// @retail 0x290c80
s_handler_object_view *function_290c80(s_ai_object_iterator *iterator)
{
	s_handler_object_view *object = NULL;

	if (iterator->next_index != NONE)
	{
		object = handler_object_get(iterator->next_index);
		iterator->index = iterator->next_index;

		s_object_ai_data *data;
		if (!object->flags134 && (data = (s_object_ai_data *)((byte *)object + object->ai_offset)) != NULL)
		{
			iterator->next_index = data->next_object_index;
		}
		else
		{
			iterator->next_index = NONE;
		}
	}

	return object;
}

// @retail 0x28d9d0
void function_28d9d0(void)
{
	s_type_f1af8e iterator;
	iterator.signature = 0x86868686;
	iterator.type_mask = 0x1000;
	iterator.flags = 0;
	iterator.index = 0;
	iterator.object_index = NONE;
	s_handler_object_view *object;
	while ((object = (s_handler_object_view *)function_baeb0(&iterator)) != NULL)
	{
		s_object_ai_data *data = object_ai_data(object);
		if (data)
		{
			data->index08 = NONE;
			data->index04 = NONE;
			data->next_object_index = NONE;
		}
	}
	g_5044c8->valid = false;
}
