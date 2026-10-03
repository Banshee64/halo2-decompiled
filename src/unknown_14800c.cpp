// @flags /O1 /arch:SSE /Gr
/* UNKNOWN_14800C.CPP: the window manager's "back" handling (decompiled by
   lane G for 0x2300ea, which calls it with register arguments) */

#include "cseries.h"
#include "unknown_234c64.h"

/* the window manager's channels */
c_window_channel_4599a8 g_54d62c[5];
c_window_channel g_54d76c[5];
c_window_channel g_54dba8;

void function_236299(long sound);

// @retail 0x14800c
void function_14800c(long window_type, long index)
{
	switch (window_type)
	{
	case 2:
		g_54dba8.v7();
		break;
	case 3:
		(&g_54d76c[index])->v7();
		break;
	case 5:
		(&g_54d62c[index])->v7();
		break;
	}
	function_236299(4);
}
