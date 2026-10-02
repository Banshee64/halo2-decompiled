// @flags /O2 /Gr
/* UNKNOWN_1C2670.CPP: a lifecycle callback (entry 59, initialize_for_new_map) */

#include "cseries.h"
#include <string.h>

long *g_51e9cc;

// @retail 0x1c2670
void function_1c2670(void)
{
	memset(g_51e9cc, 0xff, 0x7d0 * sizeof(long));
}
