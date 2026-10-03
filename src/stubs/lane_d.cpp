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

// @stub 0x137fe0
void function_137fe0(void)
{
}

// @stub 0x7a840
void function_07a840(void)
{
}

struct s_input_record;
struct s_input_update;

// @stub 0x198540
void __stdcall function_198540(const s_input_record *baseline, const s_input_record *record, s_input_update *update)
{
}

// @stub 0x65770
void function_065770(void)
{
}

// @stub 0x199740
bool __stdcall function_199740(unsigned char *buffer, long size, unsigned char *destination, long *decompressed_size)
{
	return false;
}