// @flags /O2 /Gr
/* UNKNOWN_1C22C0.CPP: clump object fix-up, bit-vector and, element tables */

#include "unknown_11c920.h"
#include "unknown_26b230.h"
#include "globals.h"
#include <new>


struct s_clump_object_view
{
	byte unknown000[0x338];
	long node_index;
	byte unknown33c[0xe0];
	short state41c;
	byte unknown41e[2];
	short state420;
	byte unknown422[0x66];
	byte flag488;
	byte unknown489[0x3ff];
};

struct s_node_state_view
{
	byte unknown00[0x24];
	short type;
	byte unknown26[0x16];
};

/* slot handler 0x81 (g_47eff8, unknown_1c20f0.cpp) holds this function as
   its update48, at 0x47f040 */
// @retail 0x1c22c0
void __stdcall function_1c22c0(long object_index, long unused)
{
	s_clump_object_view *object = (s_clump_object_view *)(g_4f55f0->data + (object_index & 0xffff) * sizeof(s_clump_object_view));

	if (object->node_index != NONE)
	{
		s_node_state_view *node = (s_node_state_view *)(g_502418->data + (object->node_index & 0xffff) * sizeof(s_node_state_view));

		if (node->type >= 1 && node->type <= 2)
		{
			object->state41c = 4;
			object->state420 = 2;
			object->flag488 = 1;
		}
	}
}

// @retail 0x1c2330
void function_1c2330(long bit_count, dword *a, dword *b, dword *result)
{
	long count = (bit_count + 31) >> 5;
	long i;

	for (i = 0; i < count; i++)
	{
		result[i] = a[i] & b[i];
	}
}

class c_part
{
public:
	virtual ~c_part() {}
	c_part() : value08(0) {}
	byte unknown04[2];
	word value06;
	long value08;
};

class c_part_a : public c_part
{
public:
	virtual ~c_part_a();
	c_part_a() { value06 = 0x80; }
	byte unknown0c[4];
};

class c_part_linked : public c_part
{
public:
	c_part_linked(c_part_a *a) : owner(a) {}
	c_part_a *owner;
};

class c_part_b : public c_part_linked
{
public:
	virtual ~c_part_b();
	c_part_b(c_part_a *a) : c_part_linked(a) { value06 = 0x80; }
};

class c_part_c : public c_part
{
public:
	virtual ~c_part_c();
	c_part_c() { value06 = 0x80; }
	byte unknown0c[0x24];
};

// @retail 0x1c2370
c_part_a::~c_part_a()
{
	__assume(0);
}

// @retail 0x1c2380
c_part_b::~c_part_b()
{
	__assume(0);
}

// @retail 0x1c2360
c_part_c::~c_part_c()
{
	__assume(0);
}

struct s_pair_element
{
	byte unknown00[0x20];
	c_part_a a;
	c_part_b b;
	byte unknown40[0x40];
};

struct s_single_element
{
	byte unknown00[0x20];
	c_part_c c;
};

struct s_part_table
{
	byte unknown00[0x20];
	long pair_a_count;
	s_pair_element *pairs_a;
	long single_count;
	s_single_element *singles;
	long pair_b_count;
	s_pair_element *pairs_b;

	void function_1c2390();
	c_part *function_1c2460(long index);
};

// @retail 0x1c2390
void s_part_table::function_1c2390()
{
	long i;

	for (i = 0; i < pair_a_count; i++)
	{
		s_pair_element *pair = &pairs_a[i];
		new (&pair->a) c_part_a;
		new (&pair->b) c_part_b(&pair->a);
	}
	for (i = 0; i < single_count; i++)
	{
		new (&singles[i].c) c_part_c;
	}
	for (i = 0; i < pair_b_count; i++)
	{
		s_pair_element *pair = &pairs_b[i];
		new (&pair->a) c_part_a;
		new (&pair->b) c_part_b(&pair->a);
	}
}

// @retail 0x1c2460
c_part *s_part_table::function_1c2460(long index)
{
	if (single_count)
	{
		return &singles[index].c;
	}
	return &pairs_b[index].b;
}
