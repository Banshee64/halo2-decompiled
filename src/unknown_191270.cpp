#include "cseries.h"
#include <xtl.h>
#include <string.h>
#include "crc.h"
#include "bink_playback.h"

// @flags /O2 /Gr

/* UNKNOWN_191270.CPP: the voice and sound settings and their string ids */

// @retail 0x1914f0
long function_1914f0(long string_id)
{
	long result = NONE;

	switch (string_id)
	{
	case 0x1300012d:
		result = 0xd;
		break;
	case 0x14000135:
		result = 0xe;
		break;
	case 0x1600013c:
		result = 0x1b;
		break;
	case 0x1600013d:
		result = 0x1c;
		break;
	case 0x1600013e:
		result = 0x1d;
		break;
	case 0x1600013f:
		result = 0x1e;
		break;
	}

	return result;
}

// @retail 0x191660
dword function_191660(long string_id, long index)
{
	dword result = 0;

	if (index == NONE)
	{
		switch (string_id)
		{
		case 0x1300012d:
			result = 2;
			break;
		case 0x18000136:
			result = 0x30;
			break;
		case 0x17000137:
			result = 0xc0;
			break;
		case 0x11000138:
			result = 0xf0;
			break;
		}
	}
	else if (string_id == 0x11000138)
	{
		switch (index)
		{
		case 0:
			result = 0x10;
			break;
		case 1:
			result = 0x20;
			break;
		case 2:
			result = 0x40;
			break;
		case 3:
			result = 0x80;
			break;
		}
	}

	return result;
}

/* the voice effect settings: four blocks of effect data, the effects changed
   since the last update and the ones changed before it */
struct s_voice_effects
{
	LPDSEFFECTIMAGEDESC description;
	short indices[15];
	word changed;
	word previous_changed;
	byte unknown26[2];
	dword effects[4][2];
};

s_voice_effects *g_510c90;

// @retail 0x191270
void function_191270(void)
{
	LPDIRECTSOUND direct_sound = (LPDIRECTSOUND)g_51ebe4->direct_sound;

	IDirectSound_SetEffectData(direct_sound, 4, 0x20, g_510c90->effects[0], 8, DSFX_IMMEDIATE);
	IDirectSound_SetEffectData(direct_sound, 5, 0x20, g_510c90->effects[1], 8, DSFX_IMMEDIATE);
	IDirectSound_SetEffectData(direct_sound, 6, 0x20, g_510c90->effects[2], 8, DSFX_IMMEDIATE);
	IDirectSound_SetEffectData(direct_sound, 7, 0x20, g_510c90->effects[3], 8, DSFX_IMMEDIATE);
	if (g_510c90)
	{
		g_510c90->previous_changed = 0;
		g_510c90->changed = 0;
	}
}

// @retail 0x191550
void function_191550(void)
{
	dword changed = g_510c90->previous_changed & ~g_510c90->changed;
	LPDIRECTSOUND direct_sound = (LPDIRECTSOUND)g_51ebe4->direct_sound;

	if (changed & 0x10)
		IDirectSound_SetEffectData(direct_sound, 4, 0x20, g_510c90->effects[0], 8, DSFX_IMMEDIATE);
	if (changed & 0x20)
		IDirectSound_SetEffectData(direct_sound, 5, 0x20, g_510c90->effects[1], 8, DSFX_IMMEDIATE);
	if (changed & 0x40)
		IDirectSound_SetEffectData(direct_sound, 6, 0x20, g_510c90->effects[2], 8, DSFX_IMMEDIATE);
	if (changed & 0x80)
		IDirectSound_SetEffectData(direct_sound, 7, 0x20, g_510c90->effects[3], 8, DSFX_IMMEDIATE);
	g_510c90->previous_changed = g_510c90->changed;
	g_510c90->changed = 0;
}

/* the static memory pool (unknown_221490.cpp) */
extern dword g_510800_pool_base;
extern long g_510804_pool_size;
extern dword g_510808_pool_checksum;

// @retail 0x191300
bool function_191300(LPDIRECTSOUND direct_sound)
{
	bool result = false;

	if (XLoadSection("DSPImage"))
	{
		byte *top = (byte *)(g_510804_pool_size + g_510800_pool_base);
		s_voice_effects *effects = (s_voice_effects *)(((dword)top + 3) & ~3);
		long aligned_size = ((byte *)effects - top) + sizeof(s_voice_effects);
		DSEFFECTIMAGELOC location;

		g_510804_pool_size += aligned_size;
		crc_checksum_buffer(&g_510808_pool_checksum, &aligned_size, sizeof(aligned_size));
		memset(effects->indices, 0xff, sizeof(effects->indices));
		effects->changed = 0;
		effects->previous_changed = 0;
		g_510c90 = effects;
		location.dwI3DL2ReverbIndex = 9;
		location.dwCrosstalkIndex = 10;
		if (SUCCEEDED(XAudioDownloadEffectsImage("DSPImage", &location, XAUDIO_DOWNLOADFX_XBESECTION, &effects->description)))
			result = true;
		XFreeSection("DSPImage");

		if (result)
		{
			IDirectSound_GetEffectData(direct_sound, 4, 0x20, g_510c90->effects[0], 8);
			IDirectSound_GetEffectData(direct_sound, 5, 0x20, g_510c90->effects[1], 8);
			IDirectSound_GetEffectData(direct_sound, 6, 0x20, g_510c90->effects[2], 8);
			IDirectSound_GetEffectData(direct_sound, 7, 0x20, g_510c90->effects[3], 8);
		}
	}

	return result;
}
