/* UNKNOWN_2AE170.H: streamed sounds (src/unknown_2ae170.cpp): a DirectSound
   stream fed with cached sound chunks through a codec, either the WMA decoder
   (vtable 0x45711c, instance 0x47f0d0) or plain PCM (vtable 0x45715c,
   instance 0x47f0f0) */

#ifndef UNKNOWN_2AE170_H
#define UNKNOWN_2AE170_H

#include "cseries.h"
#include <xtl.h>
#include "unknown_218850.h"

class c_sound_stream_codec;

enum
{
	k_maximum_stream_chunks = 2
};

/* which cache entries the streams use (see c_sound_stream_codec::mark_cache_usage) */
struct s_sound_cache_usage
{
	dword flags[0x90];
	byte counts[1];
};

struct s_sound_stream
{
	byte unknown00;
	char state : 3;
	char channel_count : 3;
	char unknown01_6 : 2;
	char chunk_count : 3;
	char started_count : 3;
	char unknown02_6 : 2;
	byte unknown03_0 : 2;
	byte flushing : 1;
	byte unknown03_3 : 1;
	byte unknown03_4 : 4;
	byte unknown04[4];
	long unknown08;
	real unknown0c;
	byte unknown10[0xc];
	s_sound_chunk *chunks[k_maximum_stream_chunks];
	IDirectSoundStream *stream;
	long unknown28;
	long offset;
	c_sound_stream_codec *codec;
};

class c_sound_stream_codec
{
public:
	virtual void attach(s_sound_stream *stream) {}
	virtual void start(s_sound_stream *stream) {}
	virtual void mark_cache_usage(s_sound_stream *stream, s_sound_cache_usage *usage) {}
	virtual bool get_packet(s_sound_stream *stream, XMEDIAPACKET *packet) { return false; }
	virtual void packet_submitted(s_sound_stream *stream, XMEDIAPACKET *packet) {}
	virtual void packet_failed(s_sound_stream *stream, XMEDIAPACKET *packet) {}
	virtual void chunk_finished(s_sound_stream *stream, s_sound_chunk *chunk, long unused) {}
	virtual void stop() {}
	virtual bool can_submit(s_sound_stream *stream, bool submitted) { return false; }
};

void sound_stream_flush(s_sound_stream *stream);
void sound_stream_add_chunk(s_sound_stream *stream, s_sound_chunk *chunk);
void sound_stream_release_chunk(s_sound_stream *stream, s_sound_chunk *chunk);
bool sound_stream_create(s_sound_stream *stream, c_sound_stream_codec *codec, DSSTREAMDESC *description, short channel_count);
void sound_stream_stop(s_sound_stream *stream);
void sound_stream_reset(s_sound_stream *stream);
void sound_stream_set_envelope(s_sound_stream *stream, real attack, real release);
void sound_stream_update(s_sound_stream *stream);
void sound_stream_release_chunks(s_sound_stream *stream);

#endif
