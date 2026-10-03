// stubs for lane N (0x140000..0x14ffff): callees outside the region that are
// not decompiled yet
#include "cseries.h"

struct hash_table;
struct s_physical_object;
struct s_cache_file;
class c_data_allocator;

// @stub 0x13e1a0
hash_table *hash_table_new(const char *name, long data_size, long bucket_count,
	dword (__stdcall *hash_proc)(const void *key), bool (__stdcall *compare_proc)(const void *key_a, const void *key_b),
	long maximum_count, c_data_allocator *allocator) { return 0; }

// @stub 0x13d230
void function_13d230(s_physical_object *physical) { }

// @stub 0x120d80
void function_120d80(void) { }

// @stub 0x1224c0
s_cache_file *function_1224c0(long index) { return 0; }

// @stub 0x122610
void function_122610(void *pixels, long size, void *destination) { }

// @stub 0x1682bf
void function_1682bf(long unit_index, long user_index, long representation_index) { }
