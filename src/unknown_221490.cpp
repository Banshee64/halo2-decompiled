// @flags /O2 /Ob1 /arch:SSE /Gr
#include "cseries.h"
#include <xtl.h>
#include <string.h>
#include "game_state.h"
#include "crc.h"
#include "globals.h"

enum
{
	k_sound_class_count = 54,
	k_silence_buffer_size = 1000
};

/* the wave format with natural alignment (the SDK header packs it) */
struct s_wave_format
{
	word format_tag;
	word channels;
	dword samples_per_second;
	dword average_bytes_per_second;
	word block_align;
	word bits_per_sample;
	word extra_size;
};

/* a sound class volume fade, 16 bytes; target and current are real values
   handled as raw bits */
struct s_sound_class_fade
{
	dword target;
	dword current;
	real time;
	byte flags;
	byte unknownd[3];
};

/* a mix bin table: a count followed by that many pairs */
struct s_mixbin_list
{
	long count;
	DSMIXBINVOLUMEPAIR pairs[8];
};

struct s_mixbin_settings
{
	DSMIXBINVOLUMEPAIR pairs[8];
	DSMIXBINS mixbins;
};

struct s_sound_tag_data
{
	byte unknown00[4];
	byte *unknown4;
};

struct s_unknown_5c
{
	byte unknown00[0x5c];
};

dword g_510800_pool_base;
long g_510804_pool_size;
dword g_510808_pool_checksum;

s_sound_class_fade *g_502118;
bool g_50211c;
char const *g_470090[k_sound_class_count];

// @retail 0x221490
void function_221490(
	LPDIRECTSOUNDBUFFER *buffer_reference)
{
	DSBUFFERDESC description = {0};
	s_wave_format format = {0};
	DSMIXBINVOLUMEPAIR pair;
	DSMIXBINS mixbins;
	LPDIRECTSOUNDBUFFER buffer = NULL;
	long aligned_size;
	byte *memory;
	byte *top;

	format.format_tag = WAVE_FORMAT_PCM;
	format.channels = 1;
	format.samples_per_second = k_silence_buffer_size;
	format.average_bytes_per_second = k_silence_buffer_size;
	format.block_align = 1;
	format.bits_per_sample = 8;
	format.extra_size = 0;

	pair.dwMixBin = 0xe;
	pair.lVolume = 0;
	mixbins.dwMixBinCount = 1;
	mixbins.lpMixBinVolumePairs = &pair;

	description.dwSize = sizeof(description);
	description.dwBufferBytes = 0;
	description.lpwfxFormat = (LPWAVEFORMATEX)&format;
	description.lpMixBins = &mixbins;
	description.dwInputMixBin = 0;

	DirectSoundCreateBuffer(&description, &buffer);

	top = (byte *)g_510800_pool_base + g_510804_pool_size;
	memory = (byte *)(((dword)top + 3) & ~3);
	aligned_size = (memory - top) + k_silence_buffer_size;
	g_510804_pool_size += aligned_size;
	crc_checksum_buffer(&g_510808_pool_checksum, &aligned_size, sizeof(aligned_size));

	memset(memory, 0, k_silence_buffer_size);
	IDirectSoundBuffer_SetBufferData(buffer, memory, k_silence_buffer_size);
	IDirectSoundBuffer_SetHeadroom(buffer, 0);
	IDirectSoundBuffer_SetVolume(buffer, -10000);
	IDirectSoundBuffer_Play(buffer, 0, 0, DSBPLAY_LOOPING);

	*buffer_reference = buffer;
}

// @retail 0x2215d0
void function_2215d0(
	s_mixbin_list *list,
	s_mixbin_settings *settings,
	LPDIRECTSOUNDSTREAM stream)
{
	DSMIXBINS out_mixbins;
	DSMIXBINVOLUMEPAIR out_pairs[8];
	long values[32];
	long out_count = 0;
	dword changed_mask = 0;
	dword seen_mask = 0;
	bool changed = false;
	long i;

	out_mixbins.dwMixBinCount = 0;
	out_mixbins.lpMixBinVolumePairs = out_pairs;

	if (list && list->count == (long)settings->mixbins.dwMixBinCount)
	{
		long count = list->count;

		for (i = 0; i < count; i++)
		{
			long mixbin = settings->pairs[i].dwMixBin;
			long volume = settings->pairs[i].lVolume;
			dword bit;

			if (list->pairs[i].dwMixBin != (dword)mixbin)
				goto set_all;

			if (list->pairs[i].lVolume != volume)
			{
				bit = 1 << mixbin;
				if (seen_mask & bit)
				{
					if (volume != values[i])
						goto set_all;
					if (changed_mask & bit)
						goto set_all;
				}
				changed_mask |= bit;
				out_count = out_mixbins.dwMixBinCount;
				changed = true;
			}

			bit = 1 << mixbin;
			if (!(seen_mask & bit))
			{
				out_pairs[out_count].lVolume = volume;
				seen_mask |= bit;
				out_pairs[out_count].dwMixBin = mixbin;
				out_count++;
				values[mixbin] = volume;
				out_mixbins.dwMixBinCount = out_count;
			}
		}

		if (changed)
			IDirectSoundStream_SetMixBinVolumes(stream, &out_mixbins);
		return;
	}

set_all:
	IDirectSoundStream_SetMixBins(stream, &settings->mixbins);
}

// @retail 0x2216f0
void function_2216f0(
	s_mixbin_list *list,
	s_mixbin_settings *settings,
	LPDIRECTSOUNDBUFFER buffer)
{
	DSMIXBINS out_mixbins;
	DSMIXBINVOLUMEPAIR out_pairs[8];
	long values[32];
	long out_count = 0;
	dword changed_mask = 0;
	dword seen_mask = 0;
	bool changed = false;
	long i;

	out_mixbins.dwMixBinCount = 0;
	out_mixbins.lpMixBinVolumePairs = out_pairs;

	if (list && list->count == (long)settings->mixbins.dwMixBinCount)
	{
		long count = list->count;

		for (i = 0; i < count; i++)
		{
			long mixbin = settings->pairs[i].dwMixBin;
			long volume = settings->pairs[i].lVolume;
			dword bit;

			if (list->pairs[i].dwMixBin != (dword)mixbin)
				goto set_all;

			if (list->pairs[i].lVolume != volume)
			{
				bit = 1 << mixbin;
				if (seen_mask & bit)
				{
					if (volume != values[i])
						goto set_all;
					if (changed_mask & bit)
						goto set_all;
				}
				changed_mask |= bit;
				out_count = out_mixbins.dwMixBinCount;
				changed = true;
			}

			bit = 1 << mixbin;
			if (!(seen_mask & bit))
			{
				out_pairs[out_count].lVolume = volume;
				seen_mask |= bit;
				out_pairs[out_count].dwMixBin = mixbin;
				out_count++;
				values[mixbin] = volume;
				out_mixbins.dwMixBinCount = out_count;
			}
		}

		if (changed)
			IDirectSoundBuffer_SetMixBinVolumes(buffer, &out_mixbins);
		return;
	}

set_all:
	IDirectSoundBuffer_SetMixBins(buffer, &settings->mixbins);
}

// @retail 0x221810
s_unknown_5c *function_221810(
	short index)
{
	s_tag_header_globals *globals = g_4e034c;
	s_tag_header *header = globals->header ? globals->header_alt : NULL;
	s_sound_tag_data *table = g_4e3b44[header->datum_index & 0xffff].sound;

	return (s_unknown_5c *)table->unknown4 + index;
}

// @retail 0x221850
void function_221850(void)
{
	g_502118 = (s_sound_class_fade *)game_state_malloc("", "", k_sound_class_count * sizeof(s_sound_class_fade));
	memset(g_502118, 0, k_sound_class_count * sizeof(s_sound_class_fade));
}

// @retail 0x2218a0
void function_2218a0(void)
{
	long i;

	g_50211c = true;
	for (i = 0; i < k_sound_class_count; i++)
	{
		g_502118[i].current = 0;
		g_502118[i].target = 0;
		g_502118[i].time = 0.0f;
	}
}

// @retail 0x2218e0
void function_2218e0(void)
{
	g_50211c = false;
}

// @retail 0x2218f0
void function_2218f0(void)
{
	g_502118 = NULL;
}

// @retail 0x221900
void function_221900(
	real delta)
{
	s_sound_class_fade *fade;
	long i;

	if (delta > 0.0f)
	{
		fade = g_502118;
		for (i = 0; i < k_sound_class_count; i++, fade++)
		{
			if (fade->time > delta)
			{
				dword target_bits = fade->target;
				dword current_bits = fade->current;
				real target = *(real *)&target_bits;
				real current = *(real *)&current_bits;
				real result = (target - current) * (delta / fade->time) + current;

				fade->current = *(dword *)&result;
				fade->time = fade->time - delta;
			}
			else
			{
				fade->current = fade->target;
				fade->time = 0.0f;
			}
		}
	}
}

// @retail 0x221980
void __stdcall function_221980(
	char const *name,
	long value_bits,
	real time)
{
	long i;

	for (i = 0; i < k_sound_class_count; i++)
	{
		if (*g_470090[i] && strstr(g_470090[i], name))
		{
			s_sound_class_fade *fade = &g_502118[i];
			long bits = value_bits;
			real value = *(real *)&bits;
			dword target;

			if (value < -64.0f)
				target = 0xc2800000;
			else if (value > 0.0f)
				target = 0;
			else
				target = value_bits;

			fade->target = target;
			fade->time = time < 0.0f ? 0.0f : time;
			if (time == 0.0f)
				fade->current = target;
		}
	}
}

PRIVATE void set_fade_flag(
	byte *flags,
	byte mask,
	bool value)
{
	if (value)
		*flags |= mask;
	else
		*flags &= ~mask;
}

// @retail 0x221a20
void function_221a20(
	char const *name,
	bool set)
{
	short i;

	for (i = 0; i < k_sound_class_count; ++i)
	{
		if (*g_470090[i] && strstr(g_470090[i], name))
			set_fade_flag(&g_502118[i].flags, 1, !set);
	}
}

void (__stdcall *const g_221980_callbacks[])(char const *, long, real) =
{
	function_221980
};
