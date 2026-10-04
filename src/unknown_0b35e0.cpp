// @flags /O2 /Gr
/* UNKNOWN_0B35E0.CPP */

#include "unknown_11c920.h"

struct s_0b35e0_entry
{
	byte used;
	byte unknown01[0x71];
	short s72;
	byte unknown74[0x784 - 0x74];
};

long g_4d8f14;
s_0b35e0_entry *g_4d8f18;

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