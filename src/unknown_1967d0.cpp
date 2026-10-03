// @flags /O2 /Gr
/* UNKNOWN_1967D0.CPP: game speed (the input recording: counter clamps, the
   snapshot of the input state into a record, and the packet codecs that
   write a record's changes into a bit stream and merge them back) */

#include "cseries.h"
#include "globals.h"
#include "bitstream.h"
#include <string.h>

struct s_counter_range
{
	word minimum;
	word maximum;
	byte unknown04[12];
};

struct s_input_entry
{
	byte active;
	byte unknown01;
	word value;
	byte unknown04[0x14];
};

/* one counter's range for the bit stream codecs: the value is sent as
   (value - minimum) in the given number of bits */
struct s_counter_bits
{
	byte unknown00[8];
	word minimum;
	word maximum;
	long bits;
};

/* eleven bytes: a six byte address, a used flag and four more */
struct s_input_address
{
	byte data[11];
};

/* the record of the input state of one tick (0x4cc0 bytes) */
struct s_input_record
{
	byte flag0;
	byte flag1;
	byte unknown02[2];
	dword value4;
	byte flag8;
	byte unknown09[3];
	dword valuec;
	s_input_device_view devices[4][4];
	s_input_entry entries[4][4];
	s_input_counter groups[16][0x1b5];
	s_input_counter pairs[16][16][2];
	s_input_counter counters[16][45];
	s_input_address addresses[4][4];
};

/* the packed form the record is merged from */
struct s_device_update
{
	byte changed;
	byte full;
	byte unknown02[2];
	s_input_device_view view;
};

struct s_entry_update
{
	byte changed;
	byte full;
	byte unknown02[2];
	s_input_entry entry;
};

struct s_counters_update
{
	byte flag;
	byte unknown01;
	s_input_counter counters[45];
};

struct s_counter_group_update
{
	byte flag0;
	byte unknown01;
	s_input_counter first[45];
	byte flag1;
	byte unknown5d;
	s_input_counter second[32];
	struct
	{
		byte flag;
		byte unknown01;
		s_input_counter counters[7];
	} entries[45];
};

struct s_pair_update
{
	byte flag;
	byte unknown01;
	s_input_counter counters[2];
};

struct s_address_update
{
	byte flag;
	s_input_address address;
};

struct s_input_update
{
	byte flag0;
	byte unknown01[3];
	dword value4;
	byte flag8;
	byte unknown09[3];
	dword valuec;
	s_device_update devices[4][4];
	s_entry_update entries[4][4];
	s_counter_group_update groups[16];
	s_pair_update pairs[16][16];
	s_counters_update counters[16];
	s_address_update addresses[4][4];
};

extern byte g_510ca1;
extern s_input_entry g_511a74[16];
struct s_flagged_value
{
	byte flag;
	byte unknown01[3];
	dword value;
};

s_flagged_value g_511020;
s_flagged_value g_511028;
s_counter_range g_46ddc8[64];
s_input_counter g_511bf4[0x2020];
s_input_counter g_515694[0x2d * 16];
s_input_address g_51e8d4[16];

// @retail 0x001967d0
void function_1967d0(long a, long b, long c, long delta)
{
	if (g_510ca0 && !g_510ca1)
	{
		long minimum = g_46ddc8[b].minimum;
		long maximum = g_46ddc8[b].maximum;
		if (a != NONE)
		{
			s_input_counter *counter = &g_511bf4[a * 0x1b5 + b];
			long value = counter->value;
			value += delta;
			if (value < minimum)
				value = minimum;
			else if (value > maximum)
				value = maximum;
			counter->value = value;
		}
		if (c != NONE)
		{
			s_input_counter *counter = &g_515694[c * 0x2d + b];
			long value = counter->value;
			value += delta;
			if (value < minimum)
				value = minimum;
			else if (value > maximum)
				value = maximum;
			counter->value = value;
		}
	}
}

// @retail 0x001968b0
void function_1968b0(long c, long a, long b, long value)
{
	if (g_510ca0 && !g_510ca1)
	{
		long minimum = g_46ddc8[b].minimum;
		long maximum = g_46ddc8[b].maximum;
		if (a != NONE)
		{
			long clamped = value;
			if (clamped < minimum)
				clamped = minimum;
			else if (clamped > maximum)
				clamped = maximum;
			g_511bf4[a * 0x1b5 + b].value = clamped;
		}
		if (c != NONE)
		{
			long clamped = value;
			if (clamped < minimum)
				clamped = minimum;
			else if (clamped > maximum)
				clamped = maximum;
			g_515694[c * 0x2d + b].value = clamped;
		}
	}
}

// @retail 0x00196960
long function_196960(long a, long b, long c)
{
	long result = NONE;
	if (a != NONE)
		result = g_511bf4[a * 0x1b5 + b].value;
	if (c != NONE)
		result = g_515694[c * 0x2d + b].value;
	return result;
}

// @retail 0x00197360
void function_197360(s_input_record *record)
{
	memset(record, 0, sizeof(*record));
	record->flag1 = g_511020.flag;
	if (record->flag1)
		record->value4 = g_511020.value;
	record->flag8 = g_511028.flag;
	if (g_511028.flag)
		record->valuec = g_511028.value;
	record->flag0 = g_510cb1;
	memcpy(record->devices, input_device(0), sizeof(record->devices));
	memcpy(record->groups, g_511bf4, 0x4040);
	memcpy(record->entries, g_511a74, sizeof(record->entries));
	memcpy(record->addresses, g_51e8d4, sizeof(record->addresses));
}

// @retail 0x00197480
void function_197480(s_bitstream *stream, s_input_counter *counters, long count, s_counter_bits *ranges)
{
	long i;
	for (i = 0; i < count; i++)
	{
		stream_write_bit(stream, counters[i].flag);
		if (counters[i].flag)
		{
			long value = counters[i].value - ranges[i].minimum;
			stream_write_checked(stream, value, ranges[i].bits);
		}
	}
}

// @retail 0x00197590
bool function_197590(s_bitstream *stream, s_input_counter *counters, long count, s_counter_bits *ranges)
{
	bool result = true;
	long i;
	for (i = 0; i < count; i++)
	{
		counters[i].flag = stream_read_bit(stream);
		if (counters[i].flag)
		{
			long value = function_1959c0(stream, ranges[i].bits) + ranges[i].minimum;
			result = result && value <= ranges[i].maximum;
			counters[i].value = value;
		}
	}
	return result;
}

// @retail 0x001984e0
bool function_1984e0(long count, s_input_counter *current, s_input_counter *out, s_input_counter *previous)
{
	bool changed = false;
	long i;
	for (i = 0; i < count; i++)
	{
		if (current[i].value != previous[i].value)
		{
			out[i].flag = 1;
			out[i].value = current[i].value;
			changed = true;
		}
		else
		{
			out[i].flag = 0;
		}
	}
	return changed;
}

// @retail 0x001988e0
void function_1988e0(s_input_record *record, s_input_update *update)
{
	long i;
	long j;
	long k;

	record->flag1 = update->flag0;
	if (record->flag1)
		record->value4 = update->value4;
	record->flag0 = update->flag8;
	if (record->flag0)
		record->valuec = update->valuec;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			s_device_update *device = &update->devices[i][j];
			if (device->changed)
			{
				s_input_device_view *d = &record->devices[i][j];
				d->active = device->view.active;
				d->button = device->view.button;
				if (device->full)
					*d = device->view;
			}
		}
	}

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			s_entry_update *entry = &update->entries[i][j];
			if (entry->changed)
			{
				s_input_entry *e = &record->entries[i][j];
				e->active = entry->entry.active;
				e->unknown01 = entry->entry.unknown01;
				e->value = entry->entry.value;
				if (entry->full)
					*e = entry->entry;
			}
		}
	}

	for (i = 0; i < 16; i++)
	{
		s_counter_group_update *group = &update->groups[i];
		s_input_counter *counters = record->groups[i];
		
		if (group->flag0)
		{
			for (j = 0; j < 45; j++)
			{
				if (group->first[j].flag)
					counters[j].value = group->first[j].value;
			}
		}
		if (group->flag1)
		{
			for (j = 0; j < 32; j++)
			{
				if (group->second[j].flag)
					counters[45 + j].value = group->second[j].value;
			}
		}		for (j = 0; j < 45; j++)
		{
			if (group->entries[j].flag)
			{
				for (k = 0; k < 7; k++)
				{
					if (group->entries[j].counters[k].flag)
						counters[78 + j * 8 + k].value = group->entries[j].counters[k].value;
				}
			}
		}
	}

	for (i = 0; i < 16; i++)
	{
		for (j = 0; j < 16; j++)
		{
			s_pair_update *pair = &update->pairs[i][j];
			s_input_counter *counters = record->pairs[i][j];
			if (pair->flag)
			{
				k = 2;
				do
				{
					if (pair->counters[0].flag)
						counters[0].value = pair->counters[0].value;
					if (pair->counters[1].flag)
						counters[1].value = pair->counters[1].value;
					k--;
				}
				while (k);
			}
		}
	}

	for (i = 0; i < 16; i++)
	{
		s_counters_update *group = &update->counters[i];
		s_input_counter *counters = record->counters[i];
		if (group->flag)
		{
			for (j = 0; j < 45; j++)
			{
				if (group->counters[j].flag)
					counters[j].value = group->counters[j].value;
			}
		}
	}

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			s_address_update *address = &update->addresses[i][j];
			s_input_address *d = &record->addresses[i][j];
			if (address->flag)
				*d = address->address;
		}
	}
}

// @retail 0x00199010
bool __stdcall function_199010(long a, long b, long unused)
{
	long result = input_device(a)->button - input_device(b)->button;
	if (result == 0)
		result = input_device(b)->axis - input_device(a)->axis;
	return result > 0;
}

// @retail 0x00199060
bool __stdcall function_199060(long a, long b, long unused)
{
	long result = (char)g_511a74[a].unknown01 - (char)g_511a74[b].unknown01;
	if (result == 0)
		result = (short)g_511a74[b].value - (short)g_511a74[a].value;
	return result > 0;
}

struct s_address_table
{
	byte unknown00[0xdc24];
	struct
	{
		byte address[6];
		byte used;
		byte unknown07[4];
	} entries[16];
};

// @retail 0x001991f0
long function_1991f0(s_address_table *table, byte const *address)
{
	long index = NONE;
	dword i;
	for (i = 0; i < 16; i++)
	{
		if (!table->entries[i].used)
		{
			index = i;
			break;
		}
	}
	if (index != NONE)
	{
		memset(&table->entries[index], 0, sizeof(table->entries[index]));
		table->entries[index].used = 1;
		memcpy(table->entries[index].address, address, 6);
	}
	return index;
}

// @retail 0x00199250
long function_199250(s_address_table *table, byte const *address)
{
	long result = NONE;
	dword i;
	for (i = 0; i < 16 && result == NONE; i++)
	{
		if (table->entries[i].used && memcmp(table->entries[i].address, address, 6) == 0)
			result = i;
	}
	return result;
}
