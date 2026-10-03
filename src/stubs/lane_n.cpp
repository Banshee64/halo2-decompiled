// stubs for lane N (0x140000..0x14ffff): callees outside the region that are
// not decompiled yet
#include "cseries.h"
#include "havok_memory.h"

struct hash_table;
class c_data_allocator;

// @stub 0x13e1a0
hash_table *hash_table_new(const char *name, long data_size, long bucket_count,
	dword (__stdcall *hash_proc)(const void *key), bool (__stdcall *compare_proc)(const void *key_a, const void *key_b),
	long maximum_count, c_data_allocator *allocator) { return 0; }

// @stub 0x122610
void function_122610(void *pixels, long size, void *destination) { }

// @stub 0x22c3e0
hkPoolMemory::hkPoolMemory() { }

// @stub 0x22cb90
real hkPoolMemory::get_used_fraction(void) { return 0; }

// @stub 0x1c27a0
void function_1c27a0(void) { }

// @stub 0x1c2690
void function_1c2690(void) { }

// @stub 0x1c4590
void __stdcall function_1c4590(long unknown) { }

// @stub 0x148e6d
bool __stdcall function_148e6d(long user_index) { return false; }

// @stub 0x238ea7
void __stdcall function_238ea7(long user_index) { }

// @stub 0x238eb5
void __stdcall function_238eb5(long user_index, long type) { }
