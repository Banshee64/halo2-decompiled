// @flags /O2 /arch:SSE /Gr
/* NETWORK_STREAMS.CPP: the two kinds of message stream a connection owns
   (src/network_connection.cpp allocates them): the unreliable stream (0x2850
   bytes, vtable 0x450db8) and the reliable stream (0x97c bytes, vtable
   0x450dd8). Both keep their messages in windows over sequence numbers. */

#include "cseries.h"
#include "globals.h"
#include "network_connection.h"
#include <xtl.h>

/* the tuning of the reliable streams */
long g_4cf6e8;
long g_4cf70c[4];

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
	long m_tuning[4];
	long m_unknown978;
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
				long index = sequence_window_index(&stream->m_message_window, sequence);
				s_stream_message *message = index != NONE ? &stream->m_messages[index] : 0;
				stream->m_message_bytes -= message->size;
				free_block(message->data);
				message->data = 0;
			}
		}
		if (sequence_window_count(&stream->m_fragment_window))
		{
			for (long sequence = stream->m_fragment_window.oldest + 1; sequence <= stream->m_fragment_window.newest; sequence++)
			{
				long index = sequence_window_index(&stream->m_fragment_window, sequence);
				s_stream_fragment *fragment = index != NONE ? &stream->m_fragments[index] : 0;
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
	if (sequence_window_count(&m_message_window))
	{
		for (long sequence = m_message_window.oldest + 1; sequence <= m_message_window.newest; sequence++)
		{
			long index = sequence_window_index(&m_message_window, sequence);
			s_stream_message *message = index != NONE ? &m_messages[index] : 0;
			if (message->flags & 4)
			{
				*pending = true;
				return true;
			}
		}
	}
	return false;
}

// @retail 0x94e80
long c_network_unreliable_stream::v3(long a, long b)
{
	return 2;
}

// @retail 0x95b70
s_stream_fragment *unreliable_stream_get_fragment(c_network_unreliable_stream *stream, long sequence)
{
	long index = sequence_window_index(&stream->m_fragment_window, sequence);
	return index != NONE ? &stream->m_fragments[index] : 0;
}

// @retail 0x95cf0
void function_095cf0(s_network_stream_header *header)
{
	c_network_reliable_stream *stream = (c_network_reliable_stream *)header;
	stream->m_unknown05 = false;
	long sequence = (dword)(g_4cf6e8 * network_time_now()) / 1000 & 0xff;
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
	stream->m_tuning[0] = g_4cf70c[0];
	stream->m_tuning[1] = g_4cf70c[1];
	stream->m_tuning[2] = g_4cf70c[2];
	stream->m_tuning[3] = g_4cf70c[3];
	stream->m_unknown978 = 0;
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
			long index = sequence_window_index(&m_acknowledgement_window, sequence);
			word *acknowledgement = index != NONE ? &m_acknowledgements[index] : 0;
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
	long index = sequence_window_index(&stream->m_message_window, sequence);
	return index != NONE ? &stream->m_messages[index] : 0;
}

// @retail 0x96810
void reliable_stream_set_message_size(c_network_reliable_stream *stream, long sequence, long size)
{
	long index = sequence_window_index(&stream->m_message_window, sequence);
	s_reliable_message *message = index != NONE ? &stream->m_messages[index] : 0;
	message->size = size;
	stream->m_bytes += size;
}
