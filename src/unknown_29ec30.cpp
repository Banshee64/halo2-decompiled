#include "cseries.h"
#include "unknown_29ec30.h"

// @flags /O2 /Ob1 /Gr /arch:SSE

// @retail 0x29ec30
void function_29ec30(playback_arg a, playback_dest_arg dest, playback_arg c, playback_cursor_arg<char> cursor)
{
	*(long *)(dest.p + 0x0) = **cursor.p;
	(*cursor.p)++;
}

// @retail 0x29ec50
void function_29ec50(playback_arg a, playback_dest_arg dest, playback_arg c, playback_cursor_arg<char> cursor)
{
	*(short *)(dest.p + 0x4) = **cursor.p;
	(*cursor.p)++;
}

// @retail 0x29ec70
void function_29ec70(playback_arg a, playback_dest_arg dest, playback_arg c, playback_cursor_arg<long> cursor)
{
	*(long *)(dest.p + 0x10) = **cursor.p;
	(*cursor.p)++;
}

// @retail 0x29ec90
void function_29ec90(playback_arg a, playback_dest_arg dest, playback_arg c, playback_cursor_arg<byte> cursor)
{
	dest.p[8] = **cursor.p;
	*cursor.p += 2;
}

// @retail 0x29ecb0
void function_29ecb0(playback_arg a, playback_dest_arg dest, playback_arg c, playback_cursor_arg<byte> cursor)
{
	dest.p[9] = **cursor.p;
	*cursor.p += 2;
}

// @retail 0x29ecd0
void function_29ecd0(playback_arg a, playback_dest_arg dest, playback_arg c, playback_cursor_arg<real> cursor)
{
	real *p = *cursor.p;
	*(real *)(dest.p + 0x14) = p[0];
	*(real *)(dest.p + 0x18) = p[1];
	*(real *)(dest.p + 0x1c) = 0.0f;
	*cursor.p += 2;
}

// Retail reaches these through a table, which keeps them off the custom convention.
void *const g_29ec30_table[] =
{
	(void *)function_29ec30,
	(void *)function_29ec50,
	(void *)function_29ec70,
	(void *)function_29ec90,
	(void *)function_29ecb0,
	(void *)function_29ecd0,
};

// @retail 0x29ed00
void update_controller_char(const char *data, short *controller)
{
	controller[0] += data[0];
	if (controller[0] > 1000)
		controller[0] -= 1000;
	else if (controller[0] < -1000)
		controller[0] += 1000;
	controller[1] += data[1];
}
