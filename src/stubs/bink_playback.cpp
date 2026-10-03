// stubs for game functions not decompiled yet, called by bink_playback.cpp
#include "cseries.h"

/* whether the movie may advance (reads the game options and g_4e6388) */
// @stub 0x12b3c0
bool function_12b3c0(void) { return false; }
// @stub 0x35b90
void __stdcall function_35b90(void *material) { }
// @stub 0x363a0
void __stdcall function_363a0(void *vertices) { }
// @stub 0x1e930
void __stdcall function_1e930(long a) { }
/* retail passes the argument in edx */
// @stub 0x2148b0
bool function_2148b0(long a) { return false; }
/* builds a texture header; retail passes its destination in edi */
// @stub 0x23e340
struct D3DTexture *function_23e340(short width, short height, short format,
	void *(__stdcall *allocate)(long size, long alignment), long *size, void **data) { return 0; }
