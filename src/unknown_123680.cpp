// @flags /O2 /Gr
/* UNKNOWN_123680.CPP: the cache of streamed tag resources (0x123310..0x123b00) */

#include <xtl.h>
#include "cseries.h"
#include "data_array.h"
#include "unknown_123680.h"
#include "physical_memory.h"

/* a load of a resource into its block (g_4e3b48, 0x10 bytes each, at the
   block's index) */
struct s_cache_load
{
	short salt;
	short unknown02;
	bool done;
	byte unknown05[3];
	long handle;
	s_cache_resource *resource;
};

/* a request to load a resource (g_4e3b4c, 12 bytes each) */
struct s_cache_request
{
	short salt;
	short unknown02;
	long priority;
	s_cache_resource *resource;
};

/* by request priority: the most requests, how long ago a resource of that
   priority counts as used, and how long its block stays */
struct s_cache_priority
{
	long maximum_requests;
	long age;
	long lifetime;
};

const s_cache_priority g_468810[3] =
{
	{ 8, 0, 0 },
	{ 5, 10, 30 },
	{ 2, 20, 50 },
};

extern s_physical_object *g_4e3b54;
extern dword g_4e3b50;
s_data_array *g_4e3b48;
s_data_array *g_4e3b4c;
dword g_55e71c;
bool g_510c20;
bool g_510c21;

void function_1239d0(void);
long function_1234a0(s_cache_resource *resource, bool flush);
void function_1235b0(s_cache_load *load, long name, long priority);
bool function_1237a0(s_cache_load *load);

// @retail 0x123680
long function_123680(s_cache_resource *resource)
{
	long result = NONE;

	if (resource->block_index == NONE)
	{
		s_data_iterator iterator;
		s_cache_request *request;

		iterator.data = g_4e3b4c;
		iterator.index = NONE;
		iterator.datum_index = NONE;
		while ((request = (s_cache_request *)data_iterator_next_inlined(&iterator)) != NULL)
		{
			if (request->resource == resource)
			{
				result = iterator.datum_index;
				break;
			}
		}
	}
	return result;
}

// @retail 0x1236f0
void function_1236f0(s_cache_resource *resource, bool urgent)
{
	if (g_510c21)
	{
		long priority = (urgent ? 0 : 1) + 1;

		if (resource->block_index == NONE)
		{
			if (function_123680(resource) == NONE)
			{
				long request_index = datum_new(g_4e3b4c);

				if (request_index != NONE)
				{
					s_cache_request *request = &((s_cache_request *)g_4e3b4c->data)[request_index & 0xffff];

					request->priority = priority;
					request->resource = resource;
					function_1239d0();
				}
				else if (GetTickCount() > g_55e71c)
				{
					g_55e71c = GetTickCount() + 30000;
				}
			}
		}
		else
		{
			long age = g_468810[priority].age;
			long *last_used_time = &((s_physical_block *)g_4e3b54->blocks->data)[resource->block_index & 0xffff].time;
			long time = g_4e3b54->time;

			if (time - *last_used_time > age)
			{
				*last_used_time = time - age;
			}
		}
	}
}

// @retail 0x1237e0
void *function_1237e0(s_cache_resource *resource, long name)
{
	long request_index = function_123680(resource);
	long block_index;
	s_physical_block *block;

	if (request_index != NONE)
	{
		datum_delete(g_4e3b4c, request_index);
	}
	block_index = resource->block_index;
	if (block_index == NONE)
	{
		s_cache_load *load;

		block_index = function_1234a0(resource, true);
		load = &((s_cache_load *)g_4e3b48->data)[datum_new_at_index_with_salt(g_4e3b48, block_index) & 0xffff];
		load->handle = NONE;
		load->done = false;
		load->resource = resource;
		resource->block_index = block_index;
		function_1235b0(load, name, 0);
	}
	else
	{
		function_1237a0(&((s_cache_load *)g_4e3b48->data)[block_index & 0xffff]);
	}
	((s_physical_block *)g_4e3b54->blocks->data)[resource->block_index & 0xffff].time = g_4e3b54->time;
	block = &((s_physical_block *)g_4e3b54->blocks->data)[resource->block_index & 0xffff];
	return (void *)(g_4e3b50 + (block->offset << g_4e3b54->page_shift));
}
