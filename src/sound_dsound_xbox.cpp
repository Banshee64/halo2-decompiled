// @flags /O2 /arch:SSE /Gr
/* SOUND_DSOUND_XBOX.CPP: the DirectSound driver's channels (the sound
   streams in the driver globals), the impulse buffers and the effects
   processor settings. src/unknown_221490.cpp holds the rest of retail's
   sound_dsound_xbox.cpp. */

#include "cseries.h"
#include <xtl.h>
#include <math.h>
#include <stddef.h>
#include "sound_driver.h"
#include "unknown_2ae170.h"

typedef char check_sound_driver_globals_size[sizeof(s_sound_driver_globals) == 0x2ad8 ? 1 : -1];
typedef char check_sound_driver_direct_sound[offsetof(s_sound_driver_globals, direct_sound) == 0x2ab0 ? 1 : -1];
typedef char check_sound_driver_reverbs[offsetof(s_sound_driver_globals, reverbs) == 0x2950 ? 1 : -1];

/* the codecs of the streams: the WMA codec (src/unknown_21ebc0.cpp views it as data)
   and the PCM codec (vtable 0x45715c; c_pcm_codec of src/unknown_2ae170.cpp, viewed
   here as data until that class has a header) */
struct s_47f0d0;
extern s_47f0d0 g_47f0d0;

struct s_47f0f0
{
	void *vtable;
};

s_47f0f0 g_47f0f0;

/* which channels are in which state, and which voices are free (0x60 bytes) */
struct s_sound_driver_channel_usage
{
	dword channels_by_state[4][4];
	long unknown40[3];
	long unknown4c[3];
	dword free_voices[2];
};

/* a curve of the impulse buffers */
struct s_tag_data;
real function_13bb90(s_tag_data const *function, real input, real range);

struct s_sound_impulse_view
{
	short index;
	short mixbin;
	byte unknown04[4];
	s_tag_data const *function;
	long identifier;
};

struct s_looping_impulse_parameters;

void __stdcall sound_driver_stream_callback(LPVOID stream_context, LPVOID packet_context, DWORD status);

#define PIN(n, floor, ceiling) ((n) < (floor) ? (floor) : ((n) > (ceiling) ? (ceiling) : (n)))

static inline s_sound_stream *sound_driver_channel_get(long channel_index)
{
	return &SOUND_DRIVER_GLOBALS->channels[channel_index];
}

/* stops a channel's stream at once, dropping its chunks */
static inline void sound_driver_channel_halt(s_sound_stream *stream)
{
	DWORD status;

	stream->flushing = 1;
	stream->stream->GetStatus(&status);
	stream->stream->Flush();
	stream->codec->stop();
	stream->state = 0;
	sound_stream_release_chunks(stream);
	stream->flushing = 0;
}

// @retail 0x21e330
long function_21e330(real decibels)
{
	long value = (long)(real)((exp(decibels * 0.115129255f) - 1.0f) * 8192.0f);
	long result;

	if (value < 0)
	{
		long negative = 0xc000 - value;
		result = PIN(negative, 0xc000, 0xdfff);
	}
	else
	{
		result = value > 0x7fff ? 0x7fff : value;
	}
	return result;
}

// @retail 0x21e3b0
void function_21e3b0(c_sound_driver_effects *effects)
{
	s_sound_driver_globals *globals = SOUND_DRIVER_GLOBALS;

	if (globals->effects)
	{
		globals->effects->dispose();
		globals = SOUND_DRIVER_GLOBALS;
	}
	globals->effects = effects;
	effects->initialize(globals->effect_levels_a, globals->effect_mixbins, globals->surround);
	SOUND_DRIVER_GLOBALS->effects->set_i3dl2(&g_47005c.room, SOUND_DRIVER_GLOBALS->surround);
}

// @retail 0x21e410
void function_21e410(long type, XBOXADPCMWAVEFORMAT *format, c_sound_stream_codec **codec)
{
	if (type == 2)
	{
		format->wfx.wFormatTag = WAVE_FORMAT_PCM;
		format->wfx.nChannels = 2;
		format->wfx.nSamplesPerSec = 44100;
		format->wfx.nAvgBytesPerSec = 176400;
		format->wfx.nBlockAlign = 4;
		format->wfx.wBitsPerSample = 16;
		format->wfx.cbSize = 0;
		*codec = (c_sound_stream_codec *)&g_47f0d0;
	}
	else
	{
		WORD channels;

		format->wfx.wFormatTag = WAVE_FORMAT_XBOX_ADPCM;
		format->wfx.wBitsPerSample = 4;
		channels = (WORD)(type == 1 ? 2.0f : 1.0f);
		format->wfx.nChannels = channels;
		format->wfx.nSamplesPerSec = 44100;
		format->wfx.cbSize = 2;
		format->wSamplesPerBlock = 64;
		format->wfx.nBlockAlign = channels * 36;
		format->wfx.nAvgBytesPerSec = format->wfx.nBlockAlign * 689;
		*codec = (c_sound_stream_codec *)&g_47f0f0;
	}
}

// @retail 0x21f4e0
bool function_21f4e0(long type, long channel_index)
{
	s_sound_stream *stream = sound_driver_channel_get(channel_index);
	DSSTREAMDESC description = {0};
	XBOXADPCMWAVEFORMAT format;
	c_sound_stream_codec *codec = NULL;

	function_21e410(type, &format, &codec);
	description.lpwfxFormat = (LPWAVEFORMATEX)&format;
	description.dwFlags = 0;
	description.dwMaxAttachedPackets = 2;
	description.lpfnCallback = sound_driver_stream_callback;
	description.lpvContext = (LPVOID)channel_index;
	return sound_stream_create(stream, codec, &description, (short)type);
}

// @retail 0x220730
void __stdcall sound_driver_stream_callback(LPVOID stream_context, LPVOID packet_context, DWORD status)
{
	if (packet_context)
	{
		long channel_index = (long)stream_context;

		if (PIN(channel_index, 0, SOUND_DRIVER_GLOBALS->channel_count - 1) == channel_index)
		{
			s_sound_stream *stream = sound_driver_channel_get(channel_index);

			stream->codec->chunk_finished(stream, (s_sound_chunk *)packet_context, status);
			if (status == XMEDIAPACKET_STATUS_SUCCESS && !(stream->flushing || stream->unknown03_3))
			{
				sound_stream_update(stream);
			}
		}
	}
}

// @retail 0x21f570
void function_21f570(void)
{
	long i;

	for (i = 0; i < 2; i++)
	{
		IDirectSoundBuffer_Pause(SOUND_DRIVER_GLOBALS->impulse_buffers[i], DSBPAUSE_PAUSE);
	}
}

// @retail 0x21f5a0
void function_21f5a0(void)
{
	long i;

	for (i = 0; i < 2; i++)
	{
		IDirectSoundBuffer_Pause(SOUND_DRIVER_GLOBALS->impulse_buffers[i], DSBPAUSE_RESUME);
	}
}

// @retail 0x21f490
void function_21f490(long headroom, long mixbin)
{
	if (PIN(mixbin, 0, 31) == mixbin)
	{
		IDirectSound_SetMixBinHeadroom(SOUND_DRIVER_GLOBALS->direct_sound, mixbin, PIN(headroom, 0, 7));
	}
}

// @retail 0x21f430
void __stdcall function_21f430(long controller_index)
{
	s_sound_driver_globals *globals = SOUND_DRIVER_GLOBALS;
	bool paused = false;
	long i;

	for (i = 0; i < globals->channel_count; i++)
	{
		if (globals->channels[i].unknown28 == controller_index && globals->channels[i].state)
		{
			IDirectSoundStream_Pause(globals->channels[i].stream, DSSTREAMPAUSE_SYNCHPLAYBACK);
			globals = SOUND_DRIVER_GLOBALS;
			paused = true;
		}
	}
	if (paused)
	{
		IDirectSound_SynchPlayback(globals->direct_sound);
	}
}

// @retail 0x21f650
real function_21f650(long channel_index, long mode)
{
	s_sound_stream *stream = sound_driver_channel_get(channel_index);
	long cents;

	switch (mode)
	{
	case 0:
		cents = stream->unknown08 + 0x11f5;
		break;
	case 1:
		cents = stream->unknown08 + 0x1f5;
		break;
	default:
		cents = stream->unknown08 + 0x95c;
		break;
	}
	return (real)exp(cents * (1.0f / 4096.0f) * 0.693147182f);
}

// @retail 0x21f360
void __stdcall function_21f360(s_sound_driver_channel_usage *usage)
{
	s_sound_driver_globals *globals = SOUND_DRIVER_GLOBALS;
	long i;

	memset(usage, 0, sizeof(*usage));
	memset(usage->free_voices, 0xff, ((globals->unknown0008 + 31) >> 5) * sizeof(dword));
	for (i = 0; i < globals->channel_count; i++)
	{
		s_sound_stream *stream = &globals->channels[i];

		usage->channels_by_state[stream->state][i >> 5] |= 1 << (i & 31);
		if (stream->unknown00 != 0xff && stream->state)
		{
			char voice_index = stream->unknown00;
			usage->free_voices[voice_index >> 5] &= ~(1 << (voice_index & 31));
		}
	}
	for (i = 0; i < 3; i++)
	{
		usage->unknown40[i] = globals->unknown1a0c[i];
		usage->unknown4c[i] = globals->unknown1a12[i];
	}
}

// @retail 0x21f5d0
void function_21f5d0(long channel_index, long offset)
{
	s_sound_stream *stream = sound_driver_channel_get(channel_index);

	sound_driver_channel_halt(stream);
	stream->unknown0c = -64.0f;
	sound_stream_stop(stream);
	stream->offset = offset > 0 ? offset : 0;
	if (stream->unknown28 != NONE)
	{
		IDirectSoundStream_Pause(stream->stream, DSSTREAMPAUSE_PAUSE);
	}
	sound_stream_set_envelope(stream, 0.1f, 0.1f);
}

// @retail 0x21f290
void function_21f290(void)
{
	long i;

	for (i = 0; i < SOUND_DRIVER_GLOBALS->channel_count; i++)
	{
		s_sound_stream *stream = sound_driver_channel_get(i);

		sound_driver_channel_halt(stream);
		stream->unknown0c = -64.0f;
		sound_stream_stop(stream);
	}
}

// @retail 0x21f6d0
void __stdcall function_21f6d0(dword const *levels_a, dword const *levels_b, void const *settings)
{
	s_sound_driver_globals *globals = SOUND_DRIVER_GLOBALS;

	globals->effect_levels_a[0] = levels_a[0];
	globals->effect_levels_a[1] = levels_a[1];
	globals->effect_levels_b[0] = levels_b[0];
	globals->effect_levels_b[1] = levels_b[1];
	memcpy(globals->effect_settings, settings, sizeof(globals->effect_settings));
	function_21e3b0(globals->effects);
}

// @retail 0x21f720
void __stdcall function_21f720(s_looping_impulse_parameters const *parameters_)
{
	s_sound_impulse_view const *parameters = (s_sound_impulse_view const *)parameters_;

	if (PIN(parameters->mixbin, 0, 31) == parameters->mixbin && PIN(parameters->index, 0, 1) == parameters->index)
	{
		long index = parameters->index;
		LPDIRECTSOUNDBUFFER buffer = SOUND_DRIVER_GLOBALS->impulse_buffers[index];

		if (parameters->identifier != SOUND_DRIVER_GLOBALS->impulse_ids[index])
		{
			byte *data;
			DWORD size;
			dword i;

			IDirectSoundBuffer_Pause(buffer, DSBPAUSE_PAUSE);
			IDirectSoundBuffer_Lock(buffer, 0, 1000, (LPVOID *)&data, &size, NULL, NULL, 0);
			for (i = 0; i < size; i++)
			{
				real value = function_13bb90(parameters->function, (long)i * 0.001f, 0.0f);
				long sample;
				byte level;

				value *= 255.0f;
				__asm
				{
					fld value
					fistp sample
				}
				level = (byte)sample;
				data[i] = level > 0xff ? 0xff : level;
			}
			IDirectSoundBuffer_Pause(buffer, DSBPAUSE_RESUME);
			SOUND_DRIVER_GLOBALS->impulse_ids[index] = parameters->identifier;
		}
		IDirectSoundBuffer_SetCurrentPosition(buffer, 0);
		IDirectSoundBuffer_SetVolume(buffer, 0);
	}
}
