// stubs for the game functions lane J's code (0x090000..0x09ffff) calls that
// are not decompiled yet
#include "cseries.h"

// @stub 0x1e98e0
void __stdcall function_1e98e0(void *statborg, long b, void *data)
{
}

// @stub 0x1e9ad0
bool __stdcall function_1e9ad0(void *statborg, long b, void *data)
{
	return false;
}

struct s_transport_endpoint;
struct transport_address;

// @stub 0xb4ed0
bool transport_endpoint_bind(s_transport_endpoint *endpoint, transport_address const *address)
{
	return false;
}

// @stub 0xa9120
void __stdcall function_a9120(long unit_index, long trick)
{
}

struct s_sequence_window;

// @stub 0x1a4840
void sequence_window_advance_1a4840(s_sequence_window *window, long sequence)
{
}

// @stub 0xc92c0
bool __stdcall function_c92c0(long unit_index, long vehicle_index, short seat_index, long *a, bool *b)
{
	return false;
}
