// @flags /O2 /Gr
#include "unknown_11c920.h"
#include "globals.h"

struct s_1c3d00
{
	byte field_0[0xc];
	void *field_c;
	long field_10;
	dword field_14;
	void function_1c3d00();
};

// @retail 0x1c3d00
void s_1c3d00::function_1c3d00()
{
	if (!(field_14 & 0x80000000))
		g_480118->allocate((long)field_c, (field_14 & 0x7fffffff) * 8, 0x12);
}
