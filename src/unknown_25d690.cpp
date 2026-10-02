// @flags /O2 /Gr
#include "cseries.h"
#include "globals.h"

struct s_prop_type_entry
{
	short id;
	short unknown2;
	short unknown4;
	short kind;
	short unknown8[4];
};

struct prop_state
{
	byte unknown0[0xc4];
};

struct prop_view
{
	byte unknown0[0x60];
};

struct s_prop_datum
{
	byte unknown0[8];
	long state_index;
	byte unknownc[8];
	long view_index;
	short type;
};

struct s_prop_node
{
	byte unknown0[0x14];
	long view_index;
};

s_prop_type_entry g_470f10[10] =
{
	{0, 0, -1, -1, {-1, 0, 0, 0}},
	{1, 5, 2, 2, {2, 0, 0, 0}},
	{2, 2, 2, 1, {0, 0, 0, 0}},
	{3, 5, 2, 1, {0, 0, 0, 0}},
	{4, 1, 1, 0, {0, 0, 0, 0}},
	{5, 1, 1, 0, {0, 0, 0, 0}},
	{6, 1, 1, 0, {0, 0, 0, 0}},
	{7, 1, 1, 0, {0, 0, 0, 0}},
	{8, 1, 1, 0, {0, 0, 0, 0}},
	{-1, 0, 2, 3, {0, 0, 0, 0}},
};


// @retail 0x25d690
prop_state *prop_state_get(s_prop_datum *datum)
{
	if (g_470f10[datum->type].kind == 1)
	{
		return (prop_state *)(g_50241c->data + (datum->state_index & 0xffff) * 0xc4 + 0x58);
	}
	if (datum->view_index != NONE)
	{
		return (prop_state *)(g_502414->data + (datum->view_index & 0xffff) * 0x124 + 4);
	}
	return (prop_state *)(g_50241c->data + (datum->state_index & 0xffff) * 0xc4 + 0x58);
}

// @retail 0x25d700
prop_view *prop_view_get(long index)
{
	long tmp0 = index & 0xffff;
	prop_view *result = NULL;
	s_prop_node *node = (s_prop_node *)(g_502418->data + tmp0 * 0x3c);
	if (node->view_index != NONE)
	{
		byte *base = g_502414->data + (node->view_index & 0xffff) * 0x124;
		if (base)
		{
			result = (prop_view *)(base + 0x70);
		}
	}
	return result;
}

// @retail 0x25d740
prop_view *function_25d740(s_prop_node *node)
{
	prop_view *result = NULL;
	if (node->view_index != NONE)
	{
		byte *base = g_502414->data + (node->view_index & 0xffff) * 0x124;
		if (base)
		{
			result = (prop_view *)(base + 0x70);
		}
	}
	return result;
}
