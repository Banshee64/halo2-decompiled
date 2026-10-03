// stubs for the game functions outside 0x60000..0x6ffff that lane D's code
// calls and that are not decompiled yet

// @stub 0x8c550
void function_8c550(long task_index)
{
}

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

// @stub 0x94bf0
void function_094bf0(s_network_stream_header *stream)
{
}

// @stub 0x95cf0
void function_095cf0(s_network_stream_header *stream)
{
}

// @stub 0x53a20
void __stdcall function_53a20(unsigned long port, unsigned long size, void *data)
{
}

// @stub 0x12c090
void *__stdcall function_12c090(unsigned long size, unsigned long attributes)
{
	return 0;
}
