#include "cseries.h"
#include "globals.h"
#include "unknown_19c1d0.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

// @flags /O2 /Gr

struct s_entry_b
{
	byte unknown00[4];
	long key;
	byte unknown08[0xb50 - 8];
};

struct s_entry_c
{
	long key;
	byte unknown04[0xc64 - 4];
};

struct s_table_b
{
	long unknown00;
	long unknown04;
	long count;
	s_entry_b *data;
};

struct s_table_a
{
	long count;
	s_entry_a *data;
};

struct s_table_c
{
	long unknown00[4];
	long count;
	s_entry_c *data;
};

static s_table_b *get_table_b()
{
	s_tag_header_globals *globals = g_4e034c;
	s_table_b *table;
	if (globals->b_valid)
		table = globals->b;
	else
		table = 0;
	return table;
}

static s_entry_b *get_entry_b(s_table_b *table, long index)
{
	s_entry_b *entry = 0;
	if (table && table->count > 0)
		entry = table->data + index;
	return entry;
}

static s_table_a *get_table_a()
{
	s_table_a *table = 0;
	if (g_4e0350 && g_4e034c)
	{
		s_tag_header_globals *globals = g_4e034c;
		if (globals->a_valid)
			table = globals->a;
		else
			table = 0;
	}
	return table;
}

static s_entry_a *get_entry_a(s_table_a *table, long index)
{
	s_entry_a *entry = 0;
	if (table && table->count > 0)
		entry = table->data + index;
	return entry;
}

static s_table_c *get_table_c()
{
	s_tag_header_globals *globals = g_4e034c;
	s_table_c *table;
	if (globals->b_valid)
		table = (s_table_c *)globals->b;
	else
		table = 0;
	return table;
}

static s_entry_c *get_entry_c(s_table_c *table, long index)
{
	s_entry_c *entry = 0;
	if (table && table->count > 0)
		entry = table->data + index;
	return entry;
}

// @retail 0x19c1d0
void function_19c1d0()
{
	g_4ee4e8->valid = 0;
	g_4ee4e4->valid = 0;
}

// @retail 0x19c1f0
s_entry_b *function_19c1f0(long key)
{
	s_table_b *table = get_table_b();
	s_entry_b *result = 0;
	if (table)
	{
		long count = table->count;
		for (long i = 0; i < count; i++)
		{
			s_entry_b *entry = get_entry_b(get_table_b(), i);
			if (entry->key == key)
			{
				result = entry;
				break;
			}
		}
	}
	return result;
}

// @retail 0x19c270
s_entry_a *function_19c270(long key0, long key1)
{
	s_table_a *table = get_table_a();
	s_entry_a *result = 0;
	if (table)
	{
		long count = table->count;
		for (long i = 0; i < count; i++)
		{
			s_entry_a *entry = get_entry_a(get_table_a(), i);
			if (entry->key1 == key1 && entry->key0 == key0)
			{
				result = entry;
				break;
			}
		}
	}
	return result;
}

// @retail 0x19c320
s_entry_a *function_19c320(const char *name)
{
	s_table_a *table = get_table_a();
	s_entry_a *result = 0;
	if (table)
	{
		for (long i = 0; i < table->count; i++)
		{
			s_entry_a *entry = get_entry_a(get_table_a(), i);
			const char *a = strrchr(name, 0x5c);
			a = a ? a + 1 : name;
			const char *b = strrchr(entry->name, 0x5c);
			b = b ? b + 1 : entry->name;
			if (!strcmp(b, a))
			{
				result = entry;
				break;
			}
		}
	}
	return result;
}

// @retail 0x19c440
long function_19c440(long key0, long key1)
{
	s_table_a *table = get_table_a();
	long result = NONE;
	if (table)
	{
		long count = table->count;
		for (long i = 0; i < count; i++)
		{
			s_entry_a *entry = get_entry_a(get_table_a(), i);
			if (entry->key0 == key0 && entry->key1 > key1)
			{
				if (result == NONE || entry->key1 < result)
					result = entry->key1;
			}
		}
	}
	return result;
}

// @retail 0x19c4e0
long function_19c4e0(long key0)
{
	s_table_a *table = get_table_a();
	long result = NONE;
	if (table)
	{
		long count = table->count;
		for (long i = 0; i < count; i++)
		{
			s_entry_a *entry = get_entry_a(get_table_a(), i);
			if (entry->key0 == key0 && entry->key1 != NONE && result <= entry->key1)
				result = entry->key1;
		}
	}
	return result;
}

// @retail 0x19c580
long function_19c580()
{
	long result = NONE;
	s_table_c *table = get_table_c();
	if (table)
	{
		long count = table->count;
		for (long i = 0; i < count; i++)
		{
			s_entry_c *entry = get_entry_c(get_table_c(), i);
			long key = entry->key;
			if (key != NONE && (result == NONE || key < result))
				result = key;
		}
	}
	return result;
}

// @retail 0x19c5f0
s_entry_c *function_19c5f0(long key)
{
	s_table_c *table = get_table_c();
	s_entry_c *result = 0;
	if (table)
	{
		long count = table->count;
		for (long i = 0; i < count; i++)
		{
			s_entry_c *entry = get_entry_c(get_table_c(), i);
			if (entry->key == key)
			{
				result = entry;
				break;
			}
		}
	}
	return result;
}

// @retail 0x19c670
s_data_array *function_19c670()
{
	s_table_b *table = get_table_b();
	s_data_array *result = 0;
	if (table)
		result = g_4ee4e8;
	return result;
}

// @retail 0x19c6a0
s_data_array *function_19c6a0()
{
	s_table_b *table = get_table_b();
	s_data_array *result = 0;
	if (table)
		result = g_4ee4e4;
	return result;
}

// @retail 0x19c7c0
bool __stdcall function_19c7c0(long a, long b, void *context)
{
	return a > b;
}

// @retail 0x19c7d0
long __stdcall function_19c7d0(long a, long b, void *context)
{
	return a - b;
}

// @retail 0x19c970
const char *levels_get_path(long campaign_id, long map_id)
{
	const char *path = 0;
	if (campaign_id == NONE)
	{
		s_entry_c *level = function_19c5f0(map_id);
		if (level)
			path = (const char *)level + 0xb4c;
	}
	else
	{
		s_entry_a *level = function_19c270(campaign_id, map_id);
		if (level)
			path = level->name;
	}
	return path;
}

// @retail 0x19c9a0
int __cdecl function_19c9a0(const void *a, const void *b)
{
	s_entry_c *x = function_19c5f0(*(const long *)a);
	s_entry_c *y = function_19c5f0(*(const long *)b);
	long xs = *(long *)((byte *)x + 0xc4c);
	long ys = *(long *)((byte *)y + 0xc4c);
	if (xs > ys)
		return 1;
	return xs < ys ? -1 : 0;
}

/* a path of up to 259 characters */
struct s_level_path
{
	char string[0x104];
};

// @retail 0x19c9e0
char *level_path_print(s_level_path *path, char const *format, ...)
{
	va_list arguments;

	va_start(arguments, format);
	_vsnprintf(path->string, sizeof(path->string) - 1, format, arguments);
	path->string[sizeof(path->string) - 1] = 0;

	return path->string;
}
