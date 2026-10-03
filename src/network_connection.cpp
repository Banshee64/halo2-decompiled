// @flags /O2 /Ob1 /arch:SSE /Gr
/* NETWORK_CONNECTION.CPP: the connections (0xf8 bytes each, at 0x4d87d4) and
   the two kinds of stream they own (lane D) */

#include "cseries.h"
#include "globals.h"
#include "network_connection.h"
#include <xtl.h>

bool g_4d8ba0;
s_connection_counter g_4e6398;

s_link g_528000;
byte g_528b28[4];
byte g_529188[4];
s_connection_config g_4cf6d4;

void function_094bf0(s_network_stream_header *stream);
void function_095cf0(s_network_stream_header *stream);

static inline long network_time_now(void)
{
	if (g_510548)
		return g_51054c;
	return GetTickCount();
}

static inline long network_time_since(long time)
{
	return network_time_now() - time;
}

static inline long network_stream_window_space(s_network_stream_header *stream)
{
	s_network_stream_window *window = &stream->window;
	return (window->end - stream->window.next + 0x200) << 5;
}

// @retail 0x820f0
long network_reliable_stream_allocate(long owner)
{
	long result = NONE;

	if (g_4d8ba0 && g_4d87d0 > 0)
	{
		for (long i = 0; i < g_4d87d0; i++)
		{
			if (!network_reliable_stream_get(i)->active)
			{
				s_network_stream_header *stream = network_reliable_stream_get(i);
				function_095cf0(stream);
				stream->owner = owner;
				stream->active = true;
				result = i;
				break;
			}
		}
	}
	return result;
}

// @retail 0x82150
long network_stream_allocate(long owner)
{
	long result = NONE;

	if (g_4d8ba0 && g_4d87d0 > 0)
	{
		for (long i = 0; i < g_4d87d0; i++)
		{
			if (!network_stream_get(i)->active)
			{
				s_network_stream_header *stream = network_stream_get(i);
				function_094bf0(stream);
				stream->owner = owner;
				stream->active = true;
				stream->unknown0c = (void *)0x528588;
				result = i;
				break;
			}
		}
	}
	return result;
}

// @retail 0x880b0
bool network_connection_flags_valid(dword flags)
{
	bool valid = !(flags & 0xffffff00) && (!(flags & 1) || !(flags & 2)) && (!(flags & 0x40) || !(flags & 0x80));
	if (flags & 4)
		valid = valid && !(flags & 8);
	if (!(flags & 8))
		valid = valid && !(flags & 0x30);
	if (!(flags & 0x10))
		valid = valid && !(flags & 0x20);
	return valid;
}

// @retail 0x88650
void network_connection_close(s_network_connection *connection, long reason)
{
	if (connection->state == 5 && reason != 6)
	{
		struct
		{
			long remote_sequence;
			long local_sequence;
			long reason;
		} message;

		message.local_sequence = connection->local_sequence;
		message.remote_sequence = connection->remote_sequence;
		message.reason = reason;
		function_07b140(connection->link, (long)&connection->address, 7, sizeof(message), &message);
	}
	if (connection->callback)
		connection->callback->function(connection->callback->context);
	link_remove_entry(connection->link_list, connection->id);
	connection->previous_address = connection->address;
	connection->state = 2;
	connection->close_reason = reason;
	connection->local_sequence = NONE;
	connection->remote_sequence = NONE;
}

// @retail 0x886e0
void network_connection_dispose(s_network_connection *connection)
{
	if (connection->state > 2)
		network_connection_close(connection, 3);
	connection->owner = 0;
	if (connection->stream_index != NONE)
	{
		s_network_stream_header *stream = network_stream_get(connection->stream_index);
		function_094bf0(stream);
		stream->active = false;
		stream->owner = 0;
		connection->stream_index = NONE;
	}
	if (connection->reliable_stream_index != NONE)
	{
		s_network_stream_header *stream = network_reliable_stream_get(connection->reliable_stream_index);
		stream->active = false;
		stream->owner = 0;
		connection->reliable_stream_index = NONE;
	}
	connection->state = 0;
}

// @retail 0x88d20
void network_connection_reset_timers(s_network_connection *connection)
{
	for (long i = 0; i < 6; i++)
	{
		connection->timers[i].time = network_time_now();
		connection->timers[i].counter = g_4e6398;
	}
}

// @retail 0x88d70
void network_connection_reset_timer(s_network_connection *connection, long index)
{
	connection->timers[index].time = network_time_now();
	connection->timers[index].counter = g_4e6398;
}

// @retail 0x88fd0
long network_connection_get_reliable_value48(s_network_connection *connection)
{
	long result = 0;

	if (connection->state == 5 && (connection->flags & 8))
		result = *(long *)((byte *)network_reliable_stream_get(connection->reliable_stream_index) + 0x48);
	return result;
}

// @retail 0x89000
long network_connection_get_reliable_pending(s_network_connection *connection)
{
	long result = 0;

	if (connection->state == 5 && (connection->flags & 8))
	{
		byte *stream = (byte *)network_reliable_stream_get(connection->reliable_stream_index);
		result = *(long *)(stream + 0x30) - *(long *)(stream + 0x44);
	}
	return result;
}

// @retail 0x89030
long network_connection_get_reliable_window(s_network_connection *connection)
{
	long result = 0;

	if (connection->state == 5 && (connection->flags & 8))
	{
		byte *stream = (byte *)network_reliable_stream_get(connection->reliable_stream_index);
		result = *(long *)(stream + 0x94c) - *(long *)(stream + 0x30) + 0x80;
	}
	return result;
}

// @retail 0x89070
long network_connection_send_capacity(s_network_connection *connection)
{
	long result = 0;

	if (connection->state == 5 && (connection->flags & 0x10))
	{
		result = network_stream_window_space(network_stream_get(connection->stream_index));
	}
	return result;
}

// @retail 0x890b0
void network_connection_update_handshake(s_network_connection *connection)
{
	if (network_time_since(connection->handshake_time) >= connection->config->connect_timeout)
	{
		network_connection_close(connection, 4);
		return;
	}
	if (network_time_since(connection->handshake_next_time) >= 0 && connection->handshake_count < connection->config->retry_count)
	{
		struct
		{
			long sequence;
			dword flags;
		} message;

		message.flags = connection->flags;
		message.sequence = connection->local_sequence;
		function_07b140(connection->link, (long)&connection->address, 4, sizeof(message), &message);
		connection->handshake_next_time = network_time_now() + connection->config->retry_interval;
	}
}

// @retail 0x89180
void network_connection_send_acknowledge(s_network_connection *connection, bool reliable)
{
	struct
	{
		long remote_sequence;
		long local_sequence;
	} message;

	message.local_sequence = connection->local_sequence;
	message.remote_sequence = connection->remote_sequence;
	if (connection->flags & 0x10)
	{
		if (reliable)
			function_095580(network_stream_get(connection->stream_index), 6, sizeof(message), &message);
	}
	else
	{
		function_07b140(connection->link, (long)&connection->address, 6, sizeof(message), &message);
	}
}

// @retail 0x88360
void network_connection_establish(s_network_connection *connection, long remote_sequence)
{
	bool established = false;

	if (connection->state == 3)
	{
		connection->state = 4;
		connection->establish_time = network_time_now();
		connection->remote_sequence = remote_sequence;
		established = true;
		network_connection_reset_timers(connection);
		if (!(connection->flags & 0x10))
			connection->state = 5;
	}
	network_connection_send_acknowledge(connection, established);
}
