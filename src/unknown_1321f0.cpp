// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1321F0.CPP */

#include "cseries.h"
#include "real_math.h"
#include "globals.h"
#include <math.h>
#include <string.h>
#include <xtl.h>

#define k_pi 3.14159265359f

struct s_1321f0
{
	byte unknown000[0x21c];
	dword flags[1];
};

// @retail 0x1321f0
bool function_1321f0(long index, s_1321f0 const *data)
{
	return (data->flags[index >> 5] & (1 << (index & 31))) != 0;
}

struct s_sort_entry
{
	short value;
	byte unknown02[0x1a - 2];
};

struct s_sort_context
{
	byte unknown000[0xa6c];
	short count;
	s_sort_entry entries[128];
	byte unknown176e[2];
	long order[0x200];
};

// @retail 0x134950
bool __stdcall function_134950(long a, long b, void const *context)
{
	s_sort_context const *data = (s_sort_context const *)context;
	return data->entries[a].value > data->entries[b].value;
}

struct s_132d60
{
	s_sort_context *context;
	byte unknown04[4];
	short cluster_index;
	byte unknown0a[0x2a68 - 0xa];
	real_point3d point;
	real radius;
};

extern long g_4e7414;
extern bool g_4e7411;
short __stdcall function_14a5b0(short cluster_index, real_point3d const *point, real radius, long maximum_count, short *clusters);
void sort_4byte(long *elements, unsigned long count, void *unused, bool (__stdcall *compare)(long, long, const void *), const void *context);

// @retail 0x132d60
void function_132d60(s_132d60 *data)
{
	real radius = data->radius;
	short clusters[0x200];
	short count = 0;

	if (data->cluster_index != NONE)
	{
		if (radius > 0.0f)
		{
			g_4e7414++;
			g_4e7411 = true;
			count = function_14a5b0(data->cluster_index, &data->point, radius, 0x200, clusters);
			g_4e7411 = false;
			if (count > 0x200)
			{
				count = 0x200;
			}
		}
		else
		{
			count = 1;
			clusters[0] = data->cluster_index;
		}
	}

	data->context->count = count;
	for (long i = 0; i < data->context->count; i++)
	{
		s_sort_entry *entry = &data->context->entries[i];
		entry->value = clusters[i];
		*(short *)&entry->unknown02[0] = 0;
		data->context->order[i] = i;
	}
	sort_4byte(data->context->order, data->context->count, &radius, function_134950, data->context);
}

/* ---- bit vector pools ---- */

/* the entry list of unknown_163110.cpp (its constructor and swap are there) */
class c_entry_list
{
public:
	c_entry_list(long maximum_count);
	~c_entry_list()
	{
		delete[] shorts_b;
		shorts_b = NULL;
		delete[] longs_a;
		longs_a = NULL;
		delete[] longs_c;
		longs_c = NULL;
		delete[] shorts_d;
		shorts_d = NULL;
	}
	void swap(short index0, short index1);

	long maximum_count;
	short count;
	short *shorts_b;
	long *longs_a;
	long *longs_c;
	short *shorts_d;
};

struct s_bit_vector_pool_sizes
{
	short unknown0;
	short list_sizes[4];
	short record_count;
};

struct s_bit_vector_pool
{
	void *context;
	byte unknown004[8];
	c_entry_list *lists[4];
	long indices[0x80];
	dword flags[0x10];
	dword pool[0x200];
	word pool_used;
	word entry_count;
	dword entries[0x200][4];
	byte unknown2a60[0x6c];
	byte *records;
	byte unknown2ad0[4];
	s_bit_vector_pool_sizes sizes;
};

/* the records are 0x9c bytes each */
#define k_bit_vector_pool_record_size 0x9c

/* takes enough dwords from the pool for a bit vector of this many bits */
// @retail 0x1332b0
dword *function_1332b0(long bit_count, s_bit_vector_pool *data)
{
	dword *result = NULL;
	if (bit_count > 0)
	{
		word used = data->pool_used;
		long count = (bit_count + 31) >> 5;
		result = data->pool;
		if (used + count < 0x200)
		{
			result = &data->pool[used];
			used += count;
			data->pool_used = used;
		}
	}
	return result;
}

// @retail 0x134300
long function_134300(s_bit_vector_pool *data, short bit)
{
	long index = data->entry_count++;
	if (index >= 0 && index < 0x200)
	{
		data->entries[index][3] = 0;
		data->entries[index][2] = 0;
		data->entries[index][1] = 0;
		data->entries[index][0] = 0;
		data->entries[(short)index][bit >> 5] |= 1 << (bit & 31);
		return index;
	}
	data->entry_count = 0x200;
	return 0;
}

// @retail 0x1348e0
void function_1348e0(s_bit_vector_pool *data)
{
	data->lists[0]->count = 0;
	data->lists[1]->count = 0;
	data->lists[2]->count = 0;
	data->lists[3]->count = 0;
	memset(data->flags, 0, sizeof(data->flags));
	memset(data->pool, 0, data->pool_used * sizeof(dword));
	data->pool_used = 0;
	data->entry_count = 0;
	for (long i = 0; i < 0x80; i++)
	{
		data->indices[i] = NONE;
	}
}

/* a bit field from seven flags */
// @retail 0x132b10
dword function_132b10(bool b, bool d, bool c, bool f, bool g, bool e, bool a)
{
	return (b ? 0x4000 : 0) | (c ? 0x2000 : 0) | (d ? 0x1000 : 0) | (e ? 0x800 : 0) | (f ? 0x400 : 0) | (g ? 0x200 : 0) | (a ? 0x8000 : 0);
}

/* a 16 bit real: sign, ten bits of mantissa, then five of exponent */
// @retail 0x135880
real function_135880(word value)
{
	dword sign = value >> 15;
	dword mantissa = (value >> 5) & 0x3ff;
	dword exponent = value & 0x1f;
	dword bits = (sign << 31) | (mantissa << 13) | ((exponent + 0x70) << 23);
	return *(real *)&bits;
}
// @retail 0x134980
void function_134980(s_bit_vector_pool *data, s_bit_vector_pool_sizes const *sizes, void *context)
{
	data->context = context;
	data->sizes = *sizes;
	data->lists[0] = new c_entry_list(data->sizes.list_sizes[0]);
	data->lists[2] = new c_entry_list(data->sizes.list_sizes[2]);
	data->lists[3] = new c_entry_list(data->sizes.list_sizes[3]);
	data->lists[1] = NULL;
	if (data->sizes.list_sizes[1])
	{
		data->lists[1] = new c_entry_list(data->sizes.list_sizes[1]);
	}
	data->pool_used = 0;
	data->entry_count = 0;
	data->records = NULL;
	if (data->sizes.record_count)
	{
		data->records = (byte *)operator new(data->sizes.record_count * k_bit_vector_pool_record_size);
	}
}

// @retail 0x134b20
void function_134b20(s_bit_vector_pool *data)
{
	delete data->lists[0];
	data->lists[0] = NULL;
	delete data->lists[1];
	data->lists[1] = NULL;
	delete data->lists[2];
	data->lists[2] = NULL;
	delete data->lists[3];
	data->lists[3] = NULL;
	operator delete(data->records);
	data->records = NULL;
}

/* the two bit vector pools, each with a scratch buffer carved from one
   virtual allocation (ai.cpp borrows the same buffer) */
extern byte *g_510c44;
s_bit_vector_pool g_547f88;
s_bit_vector_pool g_54aa68;

// @retail 0x131ff0
void function_131ff0(void)
{
	s_bit_vector_pool_sizes sizes0;
	s_bit_vector_pool_sizes sizes1;
	byte *buffer;

	sizes0.unknown0 = 1;
	sizes0.list_sizes[0] = 0x80;
	sizes0.list_sizes[1] = 0x40;
	sizes0.list_sizes[2] = 0x100;
	sizes0.list_sizes[3] = 0x180;
	sizes0.record_count = 0;
	sizes1.unknown0 = 6;
	sizes1.list_sizes[0] = 0x40;
	sizes1.list_sizes[1] = 1;
	sizes1.list_sizes[2] = 0x80;
	sizes1.list_sizes[3] = 0x60;
	sizes1.record_count = 0x100;

	buffer = (byte *)VirtualAlloc(NULL, 0x452e8, MEM_COMMIT | MEM_TOP_DOWN, PAGE_READWRITE);
	if (!buffer)
	{
		GetLastError();
	}
	g_510c44 = buffer;

	function_134980(&g_547f88, &sizes0, g_510c44);
	function_134980(&g_54aa68, &sizes1, g_510c44 + 0x22974);
}

// @retail 0x1320b0
void function_1320b0(void)
{
	function_134b20(&g_547f88);
	function_134b20(&g_54aa68);

	if (g_510c44)
	{
		if (!VirtualFree(g_510c44, 0, MEM_RELEASE))
		{
			GetLastError();
		}
		g_510c44 = NULL;
	}
}

struct s_134240_object
{
	long definition_index;
};

struct s_134240_object_header
{
	byte unknown00[8];
	s_134240_object *object;
};

/* moves the objects whose model has nodes to the front of the third list */
// @retail 0x134240
void function_134240(s_bit_vector_pool *data)
{
	long kept = 0;
	for (long i = 0; i < data->lists[2]->count; i++)
	{
		long object_index = data->lists[2]->longs_a[(short)i];
		if (object_index == NONE)
		{
			continue;
		}
		s_134240_object *object = ((s_134240_object_header *)g_4e0300->data)[object_index & 0xffff].object;
		if (!object || object->definition_index == NONE)
		{
			continue;
		}
		byte *definition = g_4e3b44[object->definition_index & 0xffff].bytes;
		if (!definition || *(long *)(definition + 0x38) == NONE)
		{
			continue;
		}
		byte *model = g_4e3b44[*(long *)(definition + 0x38) & 0xffff].bytes;
		if (!model || *(long *)(model + 4) == NONE)
		{
			continue;
		}
		byte *render_model = g_4e3b44[*(long *)(model + 4) & 0xffff].bytes;
		if (*(long *)(render_model + 0x74) > 0)
		{
			if (i != kept)
			{
				data->lists[2]->swap((short)i, (short)kept);
			}
			kept++;
		}
	}
}

static inline real transition_cosine(real x)
{
	real t;
	if (0.0f > x)
	{
		t = 0.0f;
	}
	else if (x > 1.0f)
	{
		t = 1.0f;
	}
	else
	{
		t = x;
	}
	return (real)(0.5f - cos(t * k_pi) * 0.5f);
}

// @retail 0x134c50
real function_134c50(real x)
{
	return 0.0f > transition_cosine(x) ? 0.0f : (transition_cosine(x) > 1.0f ? 1.0f : transition_cosine(x));
}
