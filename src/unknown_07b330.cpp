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

#ifndef MIN
#define MIN(a,b) ((a)>(b)?(b):(a))
#endif

struct s_network_message_gateway
{
	byte unknown00[8];
	c_class_93590 *link;
	byte unknown0c[8];
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
