#include "cseries.h"
#include "globals.h"
#include "unknown_19c1d0.h"
#include <string.h>

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
	return g_4e034c->b_valid ? g_4e034c->b : 0;
}

static s_entry_b *get_entry_b(s_table_b *t, long i)
{
	if (t && t->count > 0)
		return t->data + i;
	return 0;
}

static s_table_a *get_table_a()
{
	s_table_a *t = 0;
	if (g_4e0350 && g_4e034c)
	{
		if (g_4e034c->a_valid)
			t = g_4e034c->a;
	}
	return t;
}

static long get_count_a()
{
	s_table_a *t = get_table_a();
	return t ? t->count : 0;
}

static s_entry_a *get_entry_a(s_table_a *t, long i)
{
	if (t && t->count > 0)
		return t->data + i;
	return 0;
}

static s_table_c *get_table_c()
{
	return g_4e034c->b_valid ? (s_table_c *)g_4e034c->b : 0;
}

static long get_count_c()
{
	s_table_c *t = get_table_c();
	return t ? t->count : 0;
}

static s_entry_c *get_entry_c(s_table_c *t, long i)
{
	if (t && t->count > 0)
		return t->data + i;
	return 0;
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
	if (table)
	{
		long count = table->count;
		for (long i = 0; i < count; i++)
		{
			s_entry_b *entry = 0;
			s_table_b *t = get_table_b();
			if (t && t->count > 0)
				entry = t->data + i;
			if (entry->key == key)
				return entry;
		}
	}
	return 0;
}

// @retail 0x19c270
s_entry_a *function_19c270(long key0, long key1)
{
	s_table_a *table = get_table_a();
	if (table)
	{
		long count = table->count;
		for (long i = 0; i < count; i++)
		{
			s_entry_a *entry = get_entry_a(get_table_a(), i);
			if (entry->key1 == key1 && entry->key0 == key0)
				return entry;
		}
	}
	return 0;
}

// @retail 0x19c320
s_entry_a *function_19c320(const char *name)
{
	s_table_a *table = get_table_a();
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
				return entry;
		}
	}
	return 0;
}

// @retail 0x19c440
long function_19c440(long key0, long key1)
{
	long result = NONE;
	s_table_a *table = get_table_a();
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
	long result = NONE;
	s_table_a *table = get_table_a();
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
