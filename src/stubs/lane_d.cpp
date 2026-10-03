// stubs for the game functions outside 0x60000..0x6ffff that lane D's code
// calls and that are not decompiled yet

class c_network_session;
struct s_session_member;

// @stub 0x84270
void __stdcall function_84270(void *watcher)
{
}

// @stub 0x95580
void __stdcall function_095580(void *stream, long message_type, long message_size, const void *message)
{
}

struct s_network_stream_header;

// @stub 0x53a20
void __stdcall function_53a20(unsigned long port, unsigned long size, void *data)
{
}

// @stub 0x12c090
void *__stdcall function_12c090(unsigned long size, unsigned long attributes)
{
	return 0;
}

struct s_voice_routing;
struct s_voice_route;

// @stub 0x565c0
void __stdcall function_565c0(s_voice_routing *routing, unsigned long members, s_voice_route *route)
{
}

class c_simulation_world;

// @stub 0x693a0
void __stdcall function_693a0(c_simulation_world *world)
{
}

// @stub 0x65770
void function_065770(void)
{
}
