// @flags /O2 /Gr
/* UNKNOWN_210DB0.CPP: a lifecycle callback (entry 63, initialize) */

#include "unknown_11c920.h"
#include "unknown_123b30.h"
#include "globals.h"

s_unknown_210db0 *g_4f93a4;

// @retail 0x210db0
void function_210db0(void)
{
	g_4f93a4 = (s_unknown_210db0 *)function_123d40("unknown", "unknown", sizeof(s_unknown_210db0));
}
