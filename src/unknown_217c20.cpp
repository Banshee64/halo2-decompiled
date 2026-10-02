// @flags /O2 /Gr
/* UNKNOWN_217C20.CPP: a lifecycle callback (entry 10, dispose) */

#include "cseries.h"
#include <string.h>

byte g_51ea18[0x6f * 4];

// @retail 0x217c20
void function_217c20(void)
{
	memset(g_51ea18, 0, sizeof(g_51ea18));
}
