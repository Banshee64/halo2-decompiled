// @flags /O2 /Gr
/* UNKNOWN_0937E0.CPP: the message gateway: the header in front of
   every message in a packet (its type in 8 bits, its size in 16) */

#include "unknown_11c920.h"
#include "bitstream.h"
#include "network_message_types.h"

bool function_1946f0(s_bitstream *stream);

// @retail 0x937e0
void network_message_write_header(s_bitstream *stream, long type, long size)
{
	stream_write_checked(stream, type, 8);
	stream_write_checked(stream, size, 16);
}

// @retail 0x93860
bool network_message_read_header(s_bitstream *stream, long *type, c_type_659ceb const *collection, long *size)
{
	bool result = false;
	*type = function_1959c0(stream, 8);
	*size = function_1959c0(stream, 16);
	if (!stream_overflowed(stream) && *type >= 0 && *type < k_network_message_type_count)
	{
		s_message_type const *definition = &collection->m_types[*type];
		if (definition->initialized && *size >= definition->minimum_size && *size <= definition->maximum_size)
			result = !function_1946f0(stream);
	}
	return result;
}
