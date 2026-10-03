// stubs for game functions not decompiled yet, called by bink_playback.cpp
#include "cseries.h"

/* whether the movie may advance (reads the game options and g_4e6388) */
// @stub 0x12b3c0
bool function_12b3c0(void) { return false; }
/* whether a gamepad button is down (reads input_xbox.cpp's state); retail
   passes the button in esi */
// @stub 0x23e400
bool function_23e400(long button) { return false; }
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
// @stub 0x1358c0
short function_1358c0(short format) { return 0; }
