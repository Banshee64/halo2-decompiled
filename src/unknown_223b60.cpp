// @flags /O2 /Gr
#include "cseries.h"
#include <xtl.h>
#include "unknown_223b60.h"

static __int64 read_tsc(void)
{
	volatile __int64 t = 0;
	__asm rdtsc
}

// @retail 0x223b60
__int64 timing_counter_stop(timing_counter *c)
{
	if (!c->stopped)
	{
		__int64 delta = read_tsc() - c->start;
		if (delta < 0) delta = 0;
		c->total += delta;
		c->stopped = true;
	}
	return c->total;
}

// @retail 0x223bb0
double timing_ticks_to_seconds(__int64 ticks)
{
	const __int64 frequency = 733333333;
	__int64 q = ticks / frequency;
	__int64 r = ticks - q * frequency;
	return (double)r / (double)frequency + (double)q;
}

// @retail 0x223c20
__int64 timing_counter_peek(timing_counter *c)
{
	__int64 total = c->total;
	if (!c->stopped)
	{
		__int64 delta = read_tsc() - c->start;
		if (delta < 0) delta = 0;
		total += delta;
	}
	return total;
}

// @retail 0x223e70
void RGBToColor(const word *rgb, S3TC_COLOR *out)
{
	union
	{
		dword d;
		word w;
		byte bytes[4];
	} u;
	dword c = *rgb;
	byte b = (byte)(c << 3);
	b |= b >> 5;
	u.d = c;
	u.w >>= 5;
	byte g = (byte)(u.bytes[0] << 2);
	g |= g >> 6;
	byte r = (byte)((u.d >> 6) << 3);
	r |= r >> 5;
	u.bytes[0] = b;
	u.bytes[1] = g;
	u.bytes[2] = r;
	*(dword *)out = u.d;
}

PRIVATE s_fixup_element *fixup_entry_target(s_fixup_entry *entry, byte *base8)
{
	if (entry->target_index == 0xFFFF)
		return (s_fixup_element *)(base8 + entry->target_offset);
	return (s_fixup_element *)(*(byte **)(base8 + entry->target_offset + 4) + (entry->target_index << 5));
}

// @retail 0x223c70
void fixup_group_apply(s_fixup_group *group, byte *base)
{
	byte *base8 = base + 8;
	s_fixup_pointer *root = (s_fixup_pointer *)((byte *)group + group->pointer_offset);
	root->count = 1;
	root->pointer = base8;
	for (long i = 0; i < group->entry_count; i++)
	{
		s_fixup_entry *entry = &group->entries[i];
		if (entry->offset == NONE)
			continue;
		byte *address = base + group->data_offset + entry->offset + 8;
		switch (entry->type)
		{
		case 0:
		{
			s_fixup_pointer *target = (s_fixup_pointer *)(base8 + entry->target_offset);
			target->count = entry->value / entry->target_index;
			target->pointer = address;
			break;
		}
		case 1:
		{
			s_fixup_pointer *target = (s_fixup_pointer *)(base8 + entry->target_offset);
			target->count = entry->value;
			target->pointer = address;
			break;
		}
		default:
		{
			s_fixup_element *element = fixup_entry_target(entry, base8);
			short quotient = (short)((entry->value - element->base) / element->divisor);
			element->relative = (long)address - element->unknown4;
			element->quotient = quotient;
			s_fixup_trailer *trailer = &element->inline_trailer;
			element->trailer = trailer;
			trailer->count = 1;
			trailer->packed = (dword)address - element->unknown4;
			trailer->unknown8 = 0;
			trailer->packed &= 0xFFFFFFF;
			break;
		}
		}
	}
}

PRIVATE bool fixup_resource_is_busy(D3DResource *resource)
{
	return resource->IsBusy() > 0;
}

// @retail 0x223d80
bool fixup_group_has_resource(s_fixup_group *group, byte *base)
{
	bool result = false;
	byte *base8 = base + 8;
	for (long i = 0; i < group->entry_count; i++)
	{
		s_fixup_entry *entry = &group->entries[i];
		if (entry->offset != NONE && entry->type == 2)
		{
			D3DResource *resource = (D3DResource *)fixup_entry_target(entry, base8)->trailer;
			if (resource)
			{
				if (fixup_resource_is_busy(resource))
				{
					result = true;
					break;
				}
			}
		}
	}
	return result;
}

// @retail 0x223e00
void fixup_group_release_resources(s_fixup_group *group, byte *base)
{
	byte *base8 = base + 8;
	for (long i = 0; i < group->entry_count; i++)
	{
		s_fixup_entry *entry = &group->entries[i];
		if (entry->offset != NONE && entry->type == 2)
		{
			D3DResource *resource = (D3DResource *)fixup_entry_target(entry, base8)->trailer;
			if (resource)
				resource->BlockUntilNotBusy();
		}
	}
}
