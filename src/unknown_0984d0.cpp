#include "cseries.h"
#include "globals.h"
#include "unknown_08b110.h"

// @flags /O2 /Ob1 /Gr

// @retail 0x984d0
void c_vtable_450cd0::v3(long a1, long a2, long a3, long a4, long a5, long a6)
{
	if (node == 0)
	{
		s_allocator_globals *globals = g_4d87f8;
		s_node_450cd0 *block = (s_node_450cd0 *)globals->allocator->allocate(0x10, 0, 0);
		if (block == 0)
		{
			globals->allocator->compact(0);
			block = (s_node_450cd0 *)globals->allocator->allocate(0x10, 0, 0);
		}
		if (block != 0)
		{
			globals->count++;
			block->unknown00 = NONE;
			block->unknown04 = 0;
			block->unknown08 = 0;
			block->next = 0;
		}
		node = block;
		if (node != 0)
		{
			node->unknown00 = a4;
			node->unknown04 = 0;
			node->unknown08 = 0;
			node->next = 0;
			s_node_450cd0 **link = &head;
			while (*link != 0)
				link = &(*link)->next;
			*link = node;
		}
		else
			unknown0a = 1;
	}

	if (!unknown0a)
	{
		switch (a2)
		{
		case 1:
			function_98620(this, a5, a1, a3, a6);
			break;
		case 2:
			function_986d0(this, a1, a5, a6);
			break;
		case 3:
			function_98750(this, a1, a5, a3, a6);
			break;
		case 4:
			function_988f0(this, a1, a5, a6);
			break;
		case 5:
			function_989f0(this, a5, a1, a3, a6);
			break;
		}
	}
}

// @retail 0x98aa0
void c_vtable_450cd0::v4(long a1, s_bitstream *stream)
{
	function_195720(stream, 0, 3);
	node = 0;
}

// @retail 0x98fb0
void c_vtable_450cd0::v6(s_request_450cd0 *a1)
{
	switch (a1->kind)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		break;
	}
}