// stubs for lane N (0x140000..0x14ffff): callees outside the region that are
// not decompiled yet
#include "cseries.h"

struct hash_table;
class c_data_allocator;

// @stub 0x13e1a0
hash_table *hash_table_new(const char *name, long data_size, long bucket_count,
	dword (__stdcall *hash_proc)(const void *key), bool (__stdcall *compare_proc)(const void *key_a, const void *key_b),
	long maximum_count, c_data_allocator *allocator) { return 0; }

// @stub 0x122610
void function_122610(void *pixels, long size, void *destination) { }

