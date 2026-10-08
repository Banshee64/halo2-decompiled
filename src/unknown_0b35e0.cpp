// @flags /O2 /Gr
/* UNKNOWN_0B35E0.CPP */

#include "unknown_11c920.h"
#include "globals.h"
#include <xtl.h>
#include <string.h>

struct s_online_match_session_info
{
	XNKEY key;
	XNKID session_id;
	XNADDR address;
	byte unknown3c[0x68 - 0x3c];
};

struct s_0b35e0_entry
{
	byte used;
	byte unknown01[0x71];
	short s72;
	byte unknown74[0x784 - 0x74];
};

long g_4d8f14;
s_0b35e0_entry *g_4d8f18;

// @retail 0xb3500
void function_b3500(s_online_match_session_info const *record, s_0b35e0_entry *entry)
{
	memset(entry, 0, sizeof(*entry));
	entry->used = true;
	*(long *)((byte *)entry + 4) = g_510548 ? g_51054c : GetTickCount();
	*(XNKID *)((byte *)entry + 8) = record->session_id;
	*(XNKEY *)((byte *)entry + 0x10) = record->key;
	*(XNADDR *)((byte *)entry + 0x20) = record->address;
}

// @retail 0xb35e0
s_0b35e0_entry *function_b35e0(long index)
{
	s_0b35e0_entry *entry = 0;

	if (index >= 0 && index < g_4d8f14)
	{
		s_0b35e0_entry *candidate = (s_0b35e0_entry *)((byte *)g_4d8f18 + index * 0x784);
		if (candidate->used && candidate->s72 == 0)
			entry = candidate;
	}

	return entry;
}
