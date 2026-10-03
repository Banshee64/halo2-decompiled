/* ONLINE_ATTRIBUTES.H: the views of XONLINE_ATTRIBUTE and of their inputs
   that the attribute builders of src/unknown_0b49a0.cpp (0xb4a90, 0xb4b80)
   fill, shared with their callers in src/online_matchmaking.cpp */

#ifndef ONLINE_ATTRIBUTES_H
#define ONLINE_ATTRIBUTES_H

#include "cseries.h"

struct s_entry_pair
{
	long a;
	long a_high;
	__int64 b;
};

struct s_range_input
{
	long x;
	long y;
	bool has_min;
	long min;
	bool has_max;
	long max;
	bool has_count;
	long count;
};

struct s_property_entry
{
	long key;
	long valid;
	__int64 value;
};

struct s_property_input
{
	long v0;
	long v1;
	long v2;
	long v3;
	long v4;
	long v5;
	dword flags;
};

#endif
