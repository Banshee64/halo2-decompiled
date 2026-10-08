// @flags /O2 /Gr /arch:SSE
#include "unknown_11c920.h"
#include "globals.h"
#include "data_array.h"

extern s_record_pool *g_51ecb4;
void function_293b30(long arg_0);

struct s_293830
{
	byte *field_0;
	s_record_pool_iterator field_4;
	long field_10;
};

__forceinline byte *function_293831(s_293830 *arg_0)
{
	byte *local_0 = NULL;
	if (g_4f55d0->active)
	{
		local_0 = data_iterator_next_inlined(&arg_0->field_4);
		arg_0->field_0 = local_0;
	}
	return local_0;
}

// @retail 0x293830
void function_293830()
{
	s_293830 local_0;
	if (g_4f55d0->active)
	{
		local_0.field_4.data = g_51ecb4;
		local_0.field_4.index = NONE;
	}
	while (function_293831(&local_0) != NULL)
		function_293b30(local_0.field_4.datum_index);
}
