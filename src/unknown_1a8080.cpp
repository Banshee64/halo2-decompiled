// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "unknown_26b230.h"
#include "globals.h"
#include "slot_owner.h"
#include "slot_handler.h"

/* the handlers by slot type (slot_handler.h) */
s_slot_handler *g_46eeb8[k_slot_type_count];

#define OWNER_ENTRY(index) ((s_slot_owner_entry *)(g_4f55f0->data + ((index) & 0xffff) * sizeof(s_slot_owner_entry)))

// @retail 0x1a8080
short function_1a8080(short type, short slot, s_slot_owner_entry *entry)
{
	short result = -1;
	short wanted = g_46eeb8[type]->wanted_type;

	if (wanted != -1)
	{
		while (slot >= 0)
		{
			short t = entry->slots[slot].type;
			if (t == wanted || t == 4)
				goto local_0;
			slot--;
		}
		slot = 0;
		goto local_1;
	local_0:
		if (slot == -1)
			slot = 0;
	}
local_1:
	if (slot < 3)
		result = slot + 1;
	return result;
}

// @retail 0x1a80e0
bool function_1a80e0(long index, short type, s_slot *data, short slot)
{
	s_slot_owner_entry *entry = OWNER_ENTRY(index);
	s_slot_handler *handler = g_46eeb8[type];
	bool ok;

	if (data)
		data->type = type;
	if (handler->start)
		ok = handler->start(index, data);
	else
		ok = true;
	if (ok)
	{
		short i;
		for (i = entry->current; i >= slot; i--)
		{
			s_slot *s = &entry->slots[i];
			t_slot_proc stop = g_46eeb8[s->type]->stop;
			if (stop)
				stop(index, i < 4 ? s : 0);
			s->type = -1;
			s->state = 1;
		}
		entry->current = slot;
		if (slot < 4)
		{
			s_slot *p = &entry->slots[slot];
			if (p)
			{
				if (data)
				{
					if (handler->kind == 1)
						data->unknown4 = -1;
					*p = *data;
				}
				else
				{
					p->type = type;
				}
				p->state = 0;
				p->time = g_510c54->game_time;
			}
		}
	}
	return ok;
}

// @retail 0x1a8220
bool function_1a8220(long index, short a, short b, long unknown, short c, short d, short e)
{
	s_slot_owner_entry *entry = OWNER_ENTRY(index);
	short best = -1;
	short threshold = a;
	short i = 0;

	do
	{
		s_slot_entry *p = &entry->entries[i];
		if (p->type == -1)
		{
			best = i;
			break;
		}
		if (p->priority < threshold)
		{
			threshold = p->priority;
			best = i;
		}
		i++;
	}
	while (i < 3);

	if (best == -1)
		return false;
	s_slot_entry *p = &entry->entries[best];
	p->type = a;
	p->field8 = b;
	p->field4 = c;
	p->priority = a;
	p->field6 = d;
	p->field2 = e;
	return true;
}

// @retail 0x1a82d0
void function_1a82d0(long index)
{
	s_slot_owner_entry *entry = OWNER_ENTRY(index);
	s_slot *s = &entry->slots[entry->current];
	s_slot_handler_2 *handler = (s_slot_handler_2 *)g_46eeb8[s->type];

	if (handler->head.kind == 2 && handler->update44)
		handler->update44(index, s);
}

// @retail 0x1a8320
void function_1a8320(long index)
{
	s_slot_owner_entry *entry = OWNER_ENTRY(index);
	s_slot *s = &entry->slots[entry->current];
	s_slot_handler_2 *handler = (s_slot_handler_2 *)g_46eeb8[s->type];

	if (handler->head.kind == 2 && handler->update48)
		handler->update48(index, s);
}

// @retail 0x1a8370
short __stdcall function_1a8370(long)
{
	return 1;
}

// @retail 0x1a8380
bool __stdcall function_1a8380(long index, s_slot *)
{
	OWNER_ENTRY(index)->unknown84 = 4;
	return true;
}

/* retail's data holds the default handler (slot group 0x68, 0x47d930) and
   its children */
s_slot_child g_46f350[3] =
{
	{0x5f, 0, NONE, {0}, 0, 0, 0},
	{0x60, 1, NONE, {0}, 0, 0, 0},
	{5, 0, 0, {0}, 0, 0, 0},
};

s_slot_handler_1 g_47d930 =
{
	{
		0x68, 1, 0, -2, 0,
		function_1a8370, function_1bced0, function_1a8380, 0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1a79e0, 3, g_46f350
};
