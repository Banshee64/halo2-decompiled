// @flags /O2 /Gr
#include "unknown_11c920.h"
#include "globals.h"

struct s_a0c4_object
{
	byte unknown00[0x2098];
	void *vtable2098;
	byte unknown209c[0xa0ac - 0x209c];
	void *vtable_a0ac;
};

struct s_flagged
{
	byte unknown00[0x29];
	bool field29;
};

byte g_5291a0[4];
byte g_450cb8[4];
byte g_450d60[4];
s_a0c4_object *g_4d87e8;
s_flagged *g_4d87f0;
s_flagged *g_4d87ec;
void *g_4d87d4;
void *g_4d87d8;
void *g_4d87dc;
void *g_4d87fc;
long g_4d87d0;
long g_4d87e0;
byte g_4d87e4;
long g_4d87c4;
long g_4d87c8;
long g_4d87cc;

void __stdcall function_78880(void *p);

// @retail 0x81f80
void function_81f80(void)
{
	function_78880(g_5291a0);

	if (g_4d87e8)
	{
		g_4d87e8->vtable_a0ac = g_450cb8;
		g_4d87e8->vtable2098 = g_450d60;
		g_4d87e8 = 0;
	}
	if (g_4d87f0)
	{
		g_4d87f0->field29 = false;
		g_4d87f0 = 0;
	}
	if (g_4d87ec)
	{
		g_4d87ec->field29 = false;
		g_4d87ec = 0;
	}
	if (g_4d87d4)
	{
		g_4d87d4 = 0;
	}
	if (g_4d87d8)
	{
		g_4d87d8 = 0;
	}
	if (g_4d87dc)
	{
		g_4d87dc = 0;
	}
	if (g_4d87f8)
	{
		delete g_4d87f8;
		g_4d87f8 = 0;
	}
	if (g_4d87fc)
	{
		g_4d87fc = 0;
	}
	g_4d87d0 = 0;
	g_4d87e0 = 0;
	g_4d87e4 = 0;
	g_4d87c4 = 0;
	g_4d87c8 = 0;
	g_4d87cc = 0;
}
#include "unknown_08b110.h"
#include <new>

struct s_081550_fields;
void function_081550(s_081550_fields *fields);

// @retail 0x821c0
long function_821c0(void **arg_f0f1ad, c_replication_view_storage **out_storage)
{
	s_record_pool *views = (s_record_pool *)g_4d87ec;
	void *view = 0;
	c_replication_view_storage *storage = 0;
	long index = record_pool_allocate(views);
	if (index != NONE)
	{
		view = views->data + (index & 0xffff) * 0xb4;
		if (view)
			function_081550((s_081550_fields *)view);
		if (g_4d87e4)
		{
			s_record_pool *data = (s_record_pool *)g_4d87f0;
			long storage_index = datum_new_at_index_with_salt(data, index);
			storage = new (data->data + (storage_index & 0xffff) * 0xad30) c_replication_view_storage;
		}
	}
	*arg_f0f1ad = view;
	*out_storage = storage;
	return index;
}
