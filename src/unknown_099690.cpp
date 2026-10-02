// @flags /O2 /Gr
/* UNKNOWN_099690.CPP */

#include "cseries.h"

struct s_099690_entry
{
	long a24;
	long l28;
	short state;
	short next;
	long l30;
	short s34;
	short pad36;
};

struct s_099690_peer
{
	byte unknown00[0x44];
	byte unknown44[0x400 * 8];
};

struct s_099690_globals
{
	byte unknown00[0xc];
	long bit_index;
	byte unknown10[4];
	byte *peers;
	byte unknown18[0xc];
	s_099690_entry entries[0x400];
	long head;
	byte unknown5028[4];
	long count502c;
	byte unknown5030[4];
	long count5034;
	byte unknown5038[4];
	long count503c;
};

// @retail 0x99690
void function_99690(s_099690_globals *g, long index, long new_state)
{
	long i = index & 0x3ff;
	s_099690_entry *entry = &g->entries[i];
	word *peer = (word *)(g->peers + i * 8 + 0x44);
	short old_state = entry->state;

	if (new_state == old_state)
		return;

	if (old_state == 1)
		g->count502c--;
	else if (old_state == 3)
	{
		if (peer[1] & (1 << g->bit_index))
			g->count503c--;
		else if (entry->l28)
			g->count5034--;
	}

	if (new_state == 1)
		g->count502c++;
	else if (new_state == 3)
	{
		if (peer[1] & (1 << g->bit_index))
			g->count503c++;
		else if (entry->l28)
			g->count5034++;
	}
	else if (new_state == 0)
	{
		entry->a24 = NONE;
		if (g->head == i)
		{
			g->head = entry->next;
			entry->next = (short)NONE;
			entry->s34 = 0;
			entry->state = (short)new_state;
			return;
		}
		else
		{
			long j = 0;
			do
			{
				if (g->entries[j].next == i)
				{
					g->entries[j].next = entry->next;
					break;
				}
				j++;
			}
			while (j < 0x400);
			entry->next = (short)NONE;
			entry->s34 = 0;
			entry->state = (short)new_state;
			return;
		}
	}

	if (entry->state == 0)
	{
		entry->a24 = index;
		entry->l28 = 0;
		entry->l30 = 0;
		entry->next = (short)g->head;
		g->head = i;
	}
	entry->state = (short)new_state;
}