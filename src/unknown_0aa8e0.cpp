// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_0AA8E0.CPP: the writing of a scenario object name into a simulation
   event as its index in the scenario's sorted block (outside lane J's region;
   the event encodings of src/unknown_09a5e0.cpp call it, and inline the
   reading back) */

#include "cseries.h"
#include "globals.h"
#include "bitstream.h"

typedef long (__stdcall *t_bsearch_compare_function)(const void *, const void *, const void *);
long function_13ddd0(const void *key, const void *base, long count, long element_size, t_bsearch_compare_function compare, const void *context);
/* 0x122cf0, the comparison of two longs (cache_files.cpp) */
long __stdcall cache_tag_group_compare(void const *a, void const *b, void const *context);

struct s_object_name_scenario_view
{
	byte unknown000[0x3d8];
	long object_name_count;
	long *object_names;
};

// @retail 0xaa8e0
void scenario_object_name_encode(long object_name, s_bitstream *stream)
{
	long index = NONE;
	if (object_name != NONE)
	{
		long key = object_name;
		s_object_name_scenario_view *scenario = (s_object_name_scenario_view *)g_4e0350;
		index = NONE;
		if (scenario && scenario->object_name_count > 0)
			index = function_13ddd0(&key, scenario->object_names, scenario->object_name_count, sizeof(long), cache_tag_group_compare, 0);
	}
	stream_write_checked(stream, index + 1, 9);
}
