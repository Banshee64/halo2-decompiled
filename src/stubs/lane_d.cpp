#include <wchar.h>
// stubs for the game functions outside 0x60000..0x6ffff that lane D's code
// calls and that are not decompiled yet

class c_class_58d20;
struct s_session_member;

struct s_voice_routing;
struct s_voice_route;

// @stub 0x565c0
void __stdcall function_565c0(s_voice_routing *routing, unsigned long members, s_voice_route *route)
{
}

class c_class_6a600;


// @stub 0x137fe0
void function_137fe0(void)
{
}

// @stub 0x7a840
void function_07a840(void)
{
}




// @stub 0xb2ea0
void function_b2ea0(void)
{
}

// @stub 0xb3200
void function_b3200(void)
{
}


// @stub 0x80390
void function_80390(void)
{
}

// @stub 0x195a40
void function_195a40(void)
{
}


union point3f;
union vector3f;
// @stub 0xa9f70
void __stdcall function_a9f70(long object_index, long mode, const point3f *position)
{
}

// @stub 0xaa260
void __fastcall function_aa260(const vector3f *linear, long object_index, long mode,
 const point3f *position, const vector3f *forward, const vector3f *up, const vector3f *angular)
{
}

struct s_simulation_player_update;

struct s_player_creation_record;


// @stub 0x14bf80
void __stdcall function_14bf80(long player_index, const s_player_creation_record *record)
{
}

struct s_type_9df9da;

// @stub 0x6bff0
bool __stdcall function_6bff0(s_type_9df9da *task)
{
	return false;
}

struct s_network_connection;
struct s_bitstream;

// @stub 0x88980
void __stdcall function_88980(bool reliable, s_network_connection *connection, s_bitstream *stream,
 bool pad, long extra_size, const void *extra, long *packet_size, long *stream_size, long *sent_extra_size)
{
}

struct s_simulation_definition_registry;

// @stub 0x82240
void __stdcall function_82240(s_simulation_definition_registry *registry,
	long *entity_count, long *event_count)
{
}
