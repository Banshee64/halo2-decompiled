// @flags /O2 /arch:SSE /Gr
/* CSERIES_MEMORY.CPP: Bungie's memory comparison wrapper. Retail inlines it
   everywhere, so it has no address of its own; it lives in its own file so
   that the memcmp intrinsic is expanded here, before LTCG inlines it, as in
   retail (a full three-way comparison even where callers test only for
   equality). */

#include "cseries.h"
#include <string.h>

long function_xf5684f(void const *a, void const *b, long size)
{
	return memcmp(a, b, size);
}
