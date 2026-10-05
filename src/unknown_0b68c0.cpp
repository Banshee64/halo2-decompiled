// @flags /O2 /Gr
/* UNKNOWN_0B68C0.CPP: function_b68c0 (entry 27, dispose) */

#include "unknown_11c920.h"
#include "globals.h"
#include "data_array.h"
#include "unknown_0b68c0.h"
#include <string.h>

struct s_callback_entry
{
	void (*callback)(void);
	byte unknown04[0x34];
};

struct s_data_header_40
{
	byte unknown00[0x24];
	c_data_allocator *allocator;
	byte unknown28[0x18];
};

s_callback_entry g_4674ac[3];
s_callback_node *g_4e0330;
void *g_4e0310;
void *g_4e0314;
void *g_4e0318;
bool g_4de2f0;
s_data_header_40 *g_4de2ec;
void *g_4de2e0;
void *g_4de2e4;
void *g_4de2e8;
void *g_4de2d4;
void *g_4de2d8;
void *g_4de2dc;

// @retail 0xb68c0
void function_b68c0(void)
{
	s_callback_entry *entry = g_4674ac;
	long count = 3;
	do
	{
		if (entry->callback)
		{
			entry->callback();
		}
		entry++;
		count--;
	} while (count);

	for (s_callback_node *node = g_4e0330; node; node = node->next)
	{
		if (node->callback)
		{
			node->callback();
		}
	}

	if (g_4e0310)
	{
		g_4e0310 = 0;
	}
	if (g_4e0318)
	{
		g_4e0318 = 0;
	}
	if (g_4e0314)
	{
		g_4e0314 = 0;
	}

	if (g_4de2f0)
	{
		s_data_header_40 *data = g_4de2ec;
		c_data_allocator *allocator = data->allocator;

		memset(data, 0, sizeof(*data));
		allocator->deallocate(data);

		data_dispose(g_4e0300);
	}

	g_4e0300 = 0;
	g_4de2ec = 0;
	if (g_4de2e0)
	{
		g_4de2e0 = 0;
	}
	if (g_4de2e8)
	{
		g_4de2e8 = 0;
	}
	if (g_4de2e4)
	{
		g_4de2e4 = 0;
	}
	if (g_4de2d4)
	{
		g_4de2d4 = 0;
	}
	if (g_4de2dc)
	{
		g_4de2dc = 0;
	}
	if (g_4de2d8)
	{
		g_4de2d8 = 0;
	}
}
