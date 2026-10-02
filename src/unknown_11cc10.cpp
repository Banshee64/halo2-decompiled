// @flags /O2 /Gr
/* UNKNOWN_11CC10.CPP: a lifecycle callback (entry 2, initialize_for_new_map) */

#include "cseries.h"
#include "crc.h"
#include "data_array.h"
#include "globals.h"

bool g_5107fc;

/* the static memory pool (defined with the sound code, unknown_221490.cpp) */
extern dword g_510800_pool_base;
extern long g_510804_pool_size;
extern dword g_510808_pool_checksum;

// @retail 0x11cc10
void function_11cc10(void)
{
	g_5107fc = true;
}

// @retail 0x11cc20
s_data_array *function_11cc20(long maximum_count, const char *name, long size)
{
	byte *top = (byte *)(g_510804_pool_size + g_510800_pool_base);
	byte *memory = (byte *)(((dword)top + 3) & ~3);
	long aligned_size = (memory - top) + sizeof(s_data_array) + maximum_count * size + ((maximum_count + 31) >> 5) * 4;

	g_510804_pool_size += aligned_size;
	crc_checksum_buffer(&g_510808_pool_checksum, &aligned_size, sizeof(aligned_size));
	data_initialize((s_data_array *)memory, name, maximum_count, size, 0, g_46875c);
	return (s_data_array *)memory;
}
