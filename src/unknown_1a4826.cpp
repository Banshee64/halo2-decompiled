// @flags /O1 /Gr
/* UNKNOWN_1A4826.CPP: frees a block of the user interface's memory pool
   (g_51e998, unknown_1a474c.cpp) */

#include "cseries.h"
#include "loop_allocator.h"

extern s_loop_allocator *g_51e998;

void loop_free(s_loop_allocator *loop, void **pointer);

// @retail 0x1a4826
void __stdcall user_interface_free(void *pointer)
{
	loop_free(g_51e998, &pointer);
}
