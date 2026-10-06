// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_07B330.CPP: the message gateway's outgoing packet: the messages
   written for one address collect in a bit stream until the gateway sends
   them through the link as one out-of-band packet (lane J, for lane D's
   observer, 0x784a0) */

#include "unknown_11c920.h"
#include "bitstream.h"
#include "unknown_07aec0.h"
#include "unknown_092870.h"
#include <string.h>
#include "network_message_types.h"

class c_class_938e0
{
public:
	void function_938e0(const s_type_99af70 *address, long message_type, const void *message);
};

#ifndef MIN
#define MIN(a,b) ((a)>(b)?(b):(a))
#endif

struct s_network_message_gateway
{
	virtual bool read_packet(const s_type_99af70 *address, s_bitstream *stream);
	byte unknown04[4];
	c_class_93590 *link;
	c_type_659ceb *message_types;
	c_class_938e0 *handler;
	bool outgoing_packet_pending;
	byte unknown15[3];
	byte outgoing_packet_storage[0x600];
	s_type_99af70 outgoing_packet_address;
	s_bitstream outgoing_packet;
};

/* ends a written stream: its size in bytes, rounded up to the stream's
   alignment */
static inline void stream_finish_writing(s_bitstream *stream)
{
	long size_in_bytes = (stream->bit_position + 7) / 8;
	stream->size_in_bytes = size_in_bytes;
	long remainder = size_in_bytes % stream->unknown08;
	if (remainder != 0)
		stream->size_in_bytes = stream->unknown08 - remainder + size_in_bytes;
	stream->mode = 2;
}

// @retail 0x7b330
void network_message_gateway_send_pending_messages(s_network_message_gateway *gateway)
{
	if (gateway->outgoing_packet_pending)
	{
		stream_write_bit(&gateway->outgoing_packet, false);
		stream_finish_writing(&gateway->outgoing_packet);
		network_link_send_out_of_band(gateway->link, &gateway->outgoing_packet, &gateway->outgoing_packet_address, NULL);
		gateway->outgoing_packet_pending = false;
	}
}

// @retail 0x7b390
void network_message_gateway_send_pending_messages_to_address(s_network_message_gateway *gateway, s_type_99af70 const *address)
{
	if (gateway->outgoing_packet_pending)
	{
		s_type_99af70 const *a = &gateway->outgoing_packet_address;
		short length = MIN(a->address_length, address->address_length);
		if (a->address_length > 0 && a->address_length == address->address_length && memcmp(a, address, length) == 0)
			network_message_gateway_send_pending_messages(gateway);
	}
}

bool network_message_read_header(s_bitstream *stream, long *type, c_type_659ceb const *collection, long *size);

// @retail 0x7afd0
bool s_network_message_gateway::read_packet(const s_type_99af70 *address, s_bitstream *stream)
{
	unsigned __int64 storage[8192];
	bool result = true;
	stream->mode = 3;
	stream->bit_position = 0;
	stream->checkpoint_count = 0;
	stream->error = false;
	if (function_1959c0(stream, 32) == 'debg')
		stream->error = true;
	else
	{
		stream->bit_position = 0;
		stream->error = false;
	}
	if (!stream_overflowed(stream))
	{
		while (stream_read_bit(stream))
		{
			long type = NONE;
			long size = 0;
			c_type_659ceb *collection = message_types;
			bool decoded = false;
			if (network_message_read_header(stream, &type, collection, &size))
			{
				memset(storage, 0, size);
				decoded = collection->m_types[type].decode(stream, size, storage);
			}
			result = decoded;
			if (!result)
				break;
			if (handler)
				handler->function_938e0(address, type, storage);
		}
		if (result)
			stream->mode = 5;
		return result;
	}
	return false;
}
