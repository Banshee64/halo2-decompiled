#include "cseries.h"
#include <xtl.h>

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

/* the sound globals (bink_playback.cpp): the DirectSound object */
struct s_bink_sound_settings
{
	byte unknown0000;
	bool surround;
	byte unknown0002[0x2ab0 - 0x2];
	void *direct_sound;
};

extern s_bink_sound_settings *g_51ebe4;

/* the voice effect settings: four blocks of effect data, the effects changed
   since the last update and the ones changed before it */
struct s_voice_effects
{
	byte unknown00[0x22];
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
