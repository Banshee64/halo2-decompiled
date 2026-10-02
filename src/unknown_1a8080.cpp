// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "unknown_26b230.h"
#include "globals.h"

struct s_slot;

typedef bool (__stdcall *t_slot_test)(long, s_slot *);
typedef void (__stdcall *t_slot_proc)(long, s_slot *);
typedef short (__stdcall *t_slot_query)(long);

struct s_slot_handler
{
	short unknown0;
	short kind;
	long unknown4;
	long unknown8;
	long unknownc;
	t_slot_query unknown10;
	void *unknown14;
	t_slot_test start;
	t_slot_proc stop;
	short wanted_type;
	byte unknown22[2];
	t_slot_query query;
	byte unknown28[0x1c];
	t_slot_proc update_a;
	t_slot_proc update_b;
};

struct s_slot
{
	short type;
	short state;
	short unknown4;
	byte unknown6[2];
	long time;
	byte unknownc[0x34];
};

struct s_slot_entry
{
	short type;
	short field2;
	short field4;
	short field6;
	short field8;
	short priority;
};

struct s_slot_owner_entry
{
	byte unknown0[0x84];
	short unknown84;
	byte unknown86[0xa];
	s_slot slots[4];
	short current;
	byte unknown192[0x32];
	s_slot_entry entries[3];
	byte unknown1e8[0x6a0];
};

s_slot_handler *g_46eeb8[32];

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
				break;
			slot--;
		}
		if (slot == -1)
			slot = 0;
	}
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
	s_slot_handler *handler = g_46eeb8[s->type];

	if (handler->kind == 2 && handler->update_a)
		handler->update_a(index, s);
}

// @retail 0x1a8320
void function_1a8320(long index)
{
	s_slot_owner_entry *entry = OWNER_ENTRY(index);
	s_slot *s = &entry->slots[entry->current];
	s_slot_handler *handler = g_46eeb8[s->type];

	if (handler->kind == 2 && handler->update_b)
		handler->update_b(index, s);
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

/* retail's data holds the default handler (0x47d930) */
s_slot_handler g_47d930 =
{
	0x68, 1, 0, -2, 0, function_1a8370, 0, function_1a8380, 0, -1, {0}, 0, {0}, 0, 0
};
