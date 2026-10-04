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

// @stub 0xc92c0
bool __stdcall function_c92c0(long unit_index, long vehicle_index, short seat_index, long *a, bool *b)
{
	return false;
}

struct s_bitstream;
struct s_network_connection;

/* lane D's region: a connection reads the messages of a packet */
// @stub 0x88750
void __fastcall function_088750(s_bitstream *stream, s_network_connection *connection, long packet_size, bool out_of_band)
{
}

/* lane D's region */
// @stub 0x54810
void __stdcall function_054810(void const *data, long size)
{
}
