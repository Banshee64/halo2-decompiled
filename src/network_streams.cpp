// @flags /O2 /arch:SSE /Gr
/* NETWORK_STREAMS.CPP: the two kinds of message stream a connection owns
   (src/network_connection.cpp allocates them): the unreliable stream (0x2850
   bytes, vtable 0x450db8) and the reliable stream (0x97c bytes, vtable
   0x450dd8). Both keep their messages in windows over sequence numbers. */

#include "cseries.h"
#include "globals.h"
#include "network_connection.h"
#include "network_configuration.h"
#include <xtl.h>
#include <stdlib.h>


/* a window over a range of sequence numbers: the messages oldest+1..newest,
   kept in a ring buffer from head */
struct s_sequence_window
{
	long capacity;
	long newest;
	long oldest;
	long size;
	long head;
	long count;
};

static inline long sequence_window_count(s_sequence_window const *window)
{
	return window->newest - window->oldest;
}

static inline long sequence_window_first(s_sequence_window const *window)
{
	return window->count == 0 ? NONE : window->head;
}

static inline long sequence_window_index(s_sequence_window const *window, long sequence)
{
	long index = NONE;
	if (sequence > window->oldest && sequence <= window->newest)
	{
		long offset = sequence - window->oldest - 1;
		index = (offset + sequence_window_first(window)) % window->capacity;
	}
	return index;
}

static inline void sequence_window_reset(s_sequence_window *window, long sequence)
{
	window->newest = sequence;
	window->oldest = sequence;
	window->head = 0;
	window->count = 0;
}

static inline void sequence_window_extend(s_sequence_window *window, long sequence)
{
	if (sequence > window->newest)
	{
		window->count += sequence - window->newest;
		window->newest = sequence;
	}
}

static inline void sequence_window_advance(s_sequence_window *window, long sequence)
{
	long advance = sequence - window->oldest;
	if (advance > 0)
	{
		if (window->count >= advance)
		{
			window->head = (window->head + advance) % window->size;
			window->count -= advance;
		}
		window->oldest = sequence;
	}
}

/* the release routine retail inlines here */
static inline void free_block(void *block)
{
	long info;
	if (!g_4d87f8->allocator->get_info(block, &info))
		info = NONE;
	s_allocator_globals *globals = g_4d87f8;
	globals->allocator->release(block, NONE);
	if (block != 0)
		globals->count--;
}

struct s_stream_message
{
	byte flags;
	byte size;
	word unknown02;
	void *data;
	long unknown08;
};

struct s_stream_fragment
{
	byte flags;
	byte size;
	word unknown02;
	void *data;
};

struct s_reliable_message
{
	long unknown00;
	long size;
	long unknown08;
	long unknown0c;
};

class c_network_stream
{
public:
	virtual long v0() { return 0; }
	virtual void v1() {}
	virtual bool v2(bool *pending) { return false; }
	virtual long v3(long a, long b) { return 0; }
	virtual void v4() {}
	virtual void v5() {}
	virtual void v6() {}
	virtual void v7() {}
};

class c_network_unreliable_stream : public c_network_stream
{
public:
	virtual bool v2(bool *pending);
	virtual long v3(long a, long b);

	bool m_active;
	bool m_unknown05;
	long m_owner;
	void *m_unknown0c;
	bool m_messages_valid;
	s_sequence_window m_message_window;
	s_stream_message m_messages[512];
	bool m_fragments_valid;
	s_sequence_window m_fragment_window;
	s_stream_fragment m_fragments[512];
	long m_message_bytes;
	long m_fragment_bytes;
};

class c_network_reliable_stream : public c_network_stream
{
public:
	virtual bool v2(bool *pending);
	virtual long v3(long a, long b);
	void advance_acknowledgements();
	long read_acknowledgement(long *message_sequence, long sequence, bool valid, long distance);
	void mark_received(long sequence);
	void update_round_trip(long type, long round_trip_time, long sequence, long time);

	bool m_active;
	bool m_unknown05;
	long m_owner;
	bool m_acknowledgements_valid;
	s_sequence_window m_acknowledgement_window;
	bool m_messages_valid;
	s_sequence_window m_message_window;
	long m_next_sequence;
	long m_bytes;
	word m_acknowledgements[0x80];
	s_reliable_message m_messages[0x80];
	long m_last_sequence;
	long m_unknown950;
	long m_unknown954;
	bool m_unknown958;
	bool m_unknown959;
	long m_unknown95c;
	long m_unknown960;
	long m_unknown964;
	long m_round_trip_minimum;
	long m_round_trip_average;
	long m_round_trip_deviation;
	long m_timeout;
	long m_backoff;
};

static inline dword network_time_now(void)
{
	if (g_510548)
		return g_51054c;
	return GetTickCount();
}

// @retail 0x94bf0
void function_094bf0(s_network_stream_header *header)
{
	c_network_unreliable_stream *stream = (c_network_unreliable_stream *)header;
	if (stream->m_active)
	{
		if (sequence_window_count(&stream->m_message_window))
		{
			for (long sequence = stream->m_message_window.oldest + 1; sequence <= stream->m_message_window.newest; sequence++)
			{
				s_stream_message *message;
				long index = sequence_window_index(&stream->m_message_window, sequence);
				message = 0;
				if (index != NONE)
					message = &stream->m_messages[index];
				stream->m_message_bytes -= message->size;
				free_block(message->data);
				message->data = 0;
			}
		}
		if (sequence_window_count(&stream->m_fragment_window))
		{
			for (long sequence = stream->m_fragment_window.oldest + 1; sequence <= stream->m_fragment_window.newest; sequence++)
			{
				s_stream_fragment *fragment;
				long index = sequence_window_index(&stream->m_fragment_window, sequence);
				fragment = 0;
				if (index != NONE)
					fragment = &stream->m_fragments[index];
				if (fragment->data)
				{
					stream->m_fragment_bytes -= fragment->size;
					free_block(fragment->data);
					fragment->data = 0;
				}
			}
		}
	}
	stream->m_unknown05 = false;
	stream->m_message_window.newest = 0;
	stream->m_message_window.oldest = 0;
	stream->m_message_window.head = 0;
	stream->m_message_window.count = 0;
	stream->m_messages_valid = true;
	stream->m_fragment_window.newest = 0;
	stream->m_fragment_window.oldest = 0;
	stream->m_fragment_window.head = 0;
	stream->m_fragment_window.count = 0;
	stream->m_fragments_valid = true;
	stream->m_message_bytes = 0;
	stream->m_fragment_bytes = 0;
}

// @retail 0x94df0
bool c_network_unreliable_stream::v2(bool *pending)
{
	bool result = false;
	if (sequence_window_count(&m_message_window))
	{
		for (long sequence = m_message_window.oldest + 1; sequence <= m_message_window.newest; sequence++)
		{
			s_stream_message *message;
			long index = sequence_window_index(&m_message_window, sequence);
			message = 0;
			if (index != NONE)
				message = &m_messages[index];
			if (message->flags & 4)
			{
				result = true;
				break;
			}
		}
	}
	if (result)
		*pending = true;
	return result;
}

// @retail 0x94e80
long c_network_unreliable_stream::v3(long a, long b)
{
	return 2;
}

// @retail 0x95b70
s_stream_fragment *unreliable_stream_get_fragment(c_network_unreliable_stream *stream, long sequence)
{
	s_stream_fragment *result;
	long index = sequence_window_index(&stream->m_fragment_window, sequence);
	result = 0;
	if (index != NONE)
		result = &stream->m_fragments[index];
	return result;
}

// @retail 0x95cf0
void function_095cf0(s_network_stream_header *header)
{
	c_network_reliable_stream *stream = (c_network_reliable_stream *)header;
	stream->m_unknown05 = false;
	long sequence = (dword)(g_network_configuration.value16a8 * network_time_now()) / 1000 & 0xff;
	stream->m_message_window.newest = sequence;
	stream->m_message_window.oldest = sequence;
	stream->m_message_window.head = 0;
	stream->m_message_window.count = 0;
	stream->m_messages_valid = true;
	stream->m_acknowledgement_window.newest = 0;
	stream->m_acknowledgement_window.oldest = 0;
	stream->m_acknowledgement_window.head = 0;
	stream->m_acknowledgement_window.count = 0;
	stream->m_acknowledgements_valid = false;
	stream->m_next_sequence = sequence;
	stream->m_bytes = 0;
	stream->m_unknown950 = 0;
	stream->m_unknown954 = 0;
	stream->m_unknown95c = 0;
	stream->m_unknown960 = 0;
	stream->m_unknown964 = 0;
	stream->m_unknown958 = false;
	stream->m_last_sequence = sequence - 1;
	stream->m_unknown959 = true;
	stream->m_round_trip_minimum = g_network_configuration.value16cc;
	stream->m_round_trip_average = g_network_configuration.value16d0;
	stream->m_round_trip_deviation = g_network_configuration.value16d4;
	stream->m_timeout = g_network_configuration.value16d8;
	stream->m_backoff = 0;
}

// @retail 0x95dc0
bool c_network_reliable_stream::v2(bool *pending)
{
	bool result = false;
	if (m_acknowledgements_valid &&
		(m_unknown950 < m_acknowledgement_window.newest || m_unknown954 < m_acknowledgement_window.newest))
	{
		result = true;
	}
	if (!m_unknown959)
		result = true;
	return result;
}

// @retail 0x96c50
void c_network_reliable_stream::advance_acknowledgements()
{
	if (m_acknowledgements_valid && sequence_window_count(&m_acknowledgement_window))
	{
		do
		{
			long sequence = m_acknowledgement_window.oldest + 1;
			word *acknowledgement;
			long index = sequence_window_index(&m_acknowledgement_window, sequence);
			acknowledgement = 0;
			if (index != NONE)
				acknowledgement = &m_acknowledgements[index];
			if (!(*acknowledgement & 1))
				break;
			sequence_window_advance(&m_acknowledgement_window, sequence);
		}
		while (sequence_window_count(&m_acknowledgement_window));
	}
}

// @retail 0x95df0
long c_network_reliable_stream::v3(long a, long b)
{
	advance_acknowledgements();
	if (m_acknowledgements_valid)
	{
		if (!sequence_window_count(&m_acknowledgement_window))
			return 0x12;
		if (sequence_window_count(&m_acknowledgement_window) <= 9)
			return 0x1a;
		return sequence_window_count(&m_acknowledgement_window) + 0x19;
	}
	return 0x13;
}

// @retail 0x966d0
s_reliable_message *reliable_stream_get_message(c_network_reliable_stream *stream, long sequence)
{
	s_reliable_message *result;
	long index = sequence_window_index(&stream->m_message_window, sequence);
	result = 0;
	if (index != NONE)
		result = &stream->m_messages[index];
	return result;
}

// @retail 0x96810
void reliable_stream_set_message_size(c_network_reliable_stream *stream, long sequence, long size)
{
	s_reliable_message *message;
	long index = sequence_window_index(&stream->m_message_window, sequence);
	message = 0;
	if (index != NONE)
		message = &stream->m_messages[index];
	message->size = size;
	stream->m_bytes += size;
}

// @retail 0x96710
long c_network_reliable_stream::read_acknowledgement(long *message_sequence, long sequence, bool valid, long distance)
{
	if (!m_acknowledgements_valid)
	{
		sequence_window_reset(&m_acknowledgement_window, sequence - 1);
		m_acknowledgements_valid = true;
	}
	long newest = m_acknowledgement_window.newest;
	long delta = sequence - (newest & 0xff);
	if (delta > 0x80)
		delta -= 0x100;
	else if (delta <= -0x80)
		delta += 0x100;
	long acknowledged = delta + newest;
	*message_sequence = valid ? acknowledged : NONE;
	m_unknown958 = distance == 0;
	long oldest = acknowledged - distance;
	m_unknown954 = oldest > m_unknown954 ? oldest : m_unknown954;
	if (m_acknowledgement_window.newest <= oldest)
	{
		sequence_window_reset(&m_acknowledgement_window, oldest);
		m_acknowledgements_valid = true;
	}
	else
	{
		while (m_acknowledgement_window.oldest < oldest)
			sequence_window_advance(&m_acknowledgement_window, m_acknowledgement_window.oldest + 1);
	}
	long result = 0;
	if (acknowledged < m_acknowledgement_window.oldest || acknowledged == m_acknowledgement_window.oldest && valid)
		result = 2;
	return result;
}

// @retail 0x96860
void c_network_reliable_stream::mark_received(long sequence)
{
	if (sequence != NONE)
	{
		while (sequence > m_acknowledgement_window.newest)
		{
			sequence_window_extend(&m_acknowledgement_window, m_acknowledgement_window.newest + 1);
			word *acknowledgement;
			long index = sequence_window_index(&m_acknowledgement_window, m_acknowledgement_window.newest);
			acknowledgement = 0;
			if (index != NONE)
				acknowledgement = &m_acknowledgements[index];
			*acknowledgement = 0;
		}
		word *acknowledgement;
		long index = sequence_window_index(&m_acknowledgement_window, sequence);
		acknowledgement = 0;
		if (index != NONE)
			acknowledgement = &m_acknowledgements[index];
		if (!(*acknowledgement & 1))
			*acknowledgement |= 1;
	}
}

// @retail 0x96d80
void c_network_reliable_stream::update_round_trip(long type, long round_trip_time, long sequence, long time)
{
	if (type == 1 || type == 4 || type == 5)
	{
		m_unknown95c = time;
		m_unknown960 = round_trip_time;
		m_unknown964 = sequence;
		m_last_sequence = sequence > m_last_sequence ? sequence : m_last_sequence;
		m_round_trip_minimum = m_round_trip_minimum > round_trip_time ? round_trip_time : m_round_trip_minimum;
		long average = m_round_trip_average;
		long error = round_trip_time - average;
		m_round_trip_average = (error >> g_network_configuration.value16b8) + average;
		m_round_trip_deviation = ((abs(error) - m_round_trip_deviation) >> g_network_configuration.value16bc) + m_round_trip_deviation;
		long variance = m_round_trip_deviation * g_network_configuration.value16c0;
		m_timeout = (variance <= g_network_configuration.value16c4 ? g_network_configuration.value16c4 : variance) + m_round_trip_average;
		m_timeout = m_timeout <= g_network_configuration.value16c8 ? g_network_configuration.value16c8 : m_timeout;
		m_backoff -= g_network_configuration.value16e4;
		m_backoff = m_backoff <= 0 ? 0 : m_backoff;
	}
	else if (type == 3)
	{
		m_backoff += g_network_configuration.value16dc;
		m_backoff = m_backoff > g_network_configuration.value16e0 ? g_network_configuration.value16e0 : m_backoff;
	}
}
