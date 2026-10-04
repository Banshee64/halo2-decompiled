// @flags /O2 /Gr
/* UNKNOWN_185AB0.CPP: a lifecycle callback (entry 20, initialize) */

#include "unknown_11c920.h"
#include "unknown_123b30.h"
#include "globals.h"

s_unknown_185ab0 *g_4ed284;

// @retail 0x185ab0
void function_185ab0(void)
{
	g_4ed284 = (s_unknown_185ab0 *)function_123d40("unknown", "unknown", sizeof(s_unknown_185ab0));
	g_4ed284->flag = 0;
}
