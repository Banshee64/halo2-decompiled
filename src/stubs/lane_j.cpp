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

/* lane D's region: a view's baseline update (a c_simulation_view method) */
class c_network_session;
class c_network_message_handler;
struct s_session_id;
struct s_session_member_identity;
struct transport_address;

/* lane D's region: the session handlers the message handler calls */
// @stub 0x61570
void __stdcall function_061570(c_network_session *session, bool flag)
{
}

// @stub 0x5e3f0
bool __stdcall function_05e3f0(c_network_session *session, transport_address const *address)
{
	return false;
}

// @stub 0x5cb80
bool __stdcall function_05cb80(c_network_session *session, void const *message)
{
	return false;
}

// @stub 0x5efd0
bool __stdcall function_05efd0(c_network_session *session, long remote_index, void const *message)
{
	return false;
}

// @stub 0x5d9e0
bool __stdcall function_05d9e0(c_network_session *session, void const *message)
{
	return false;
}

// @stub 0x5e6b0
bool __stdcall function_05e6b0(c_network_session *session, long remote_index)
{
	return false;
}

// @stub 0x5e7f0
bool __stdcall function_05e7f0(c_network_session *session, transport_address const *address, void const *message, long *reason, bool *has_identity, s_session_member_identity *identity)
{
	return false;
}

/* lane D's region: a connection's reconnect */
// @stub 0x88220
void __stdcall function_088220(transport_address const *address, struct s_network_connection *connection, long flag)
{
}

/* lane J's, kept out of the build in src/network_message_handler.cpp (they
   change lane D's 0x5a520 convention): the session disband and boot handlers */
// @stub 0x94310
void __stdcall function_094310(c_network_message_handler *handler, s_session_id const *message, transport_address const *address)
{
}

// @stub 0x94330
void __stdcall function_094330(c_network_message_handler *handler, s_session_id const *message, transport_address const *address)
{
}

class c_simulation_view;

// @stub 0x85e70
bool __stdcall function_085e70(c_simulation_view *view, long id, long sequence, void const *data)
{
	return false;
}
