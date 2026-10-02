// @flags /O2 /Gr
/* UNKNOWN_225700.CPP: a lifecycle callback (entry 15, initialize) */

#include "cseries.h"
#include <string.h>

struct s_unknown_225700
{
	long unknown0;
	long unknown4;
	long unknown8;
	long unknownc;
};

s_unknown_225700 g_502124;

// @retail 0x225700
void function_225700(void)
{
	memset(&g_502124, 0, sizeof(g_502124));
	g_502124.unknown8 = NONE;
}
