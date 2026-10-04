/* SOUND_DRIVER.H: the DirectSound driver globals (g_51ebe4, 0x2ad8 bytes in
   the physical memory pool; src/unknown_221490.cpp and src/sound_dsound_xbox.cpp,
   both of retail's sound_dsound_xbox.cpp). bink_playback.h views the same
   globals for their DirectSound object. */

#ifndef SOUND_DRIVER_H
#define SOUND_DRIVER_H

#include "cseries.h"
#include <xtl.h>
#include "real_math.h"
#include "unknown_2ae170.h"

enum
{
	k_sound_driver_channel_count = 0x80,
	k_sound_driver_voice_count = 0x40,
	k_sound_driver_reverb_count = 2
};

/* a voice of the sound driver (0x3c bytes): its 3d buffer and the submix
   buffer it plays into */
struct s_sound_driver_voice
{
	dword unknown00;
	byte flags;
	byte unknown05[3];
	point3f position;
	byte unknown14[0x2c - 0x14];
	real unknown2c;
	real unknown30;
	LPDIRECTSOUNDBUFFER buffer;
	LPDIRECTSOUNDBUFFER submix;
};

/* the I3DL2 reverb of an environment (0x48 bytes) */
struct s_sound_driver_reverb
{
	byte unknown00[8];
	real room;
	real room_hf;
	real room_rolloff;
	real decay_time;
	real decay_hf_ratio;
	real reflections;
	real reflections_delay;
	real reverb;
	real reverb_delay;
	real diffusion;
	real density;
	real hf_reference;
	byte unknown38[0x48 - 0x38];
};

/* the occlusion of an environment's reverb (0x10 bytes) */
struct s_sound_driver_occlusion
{
	real minimum;
	real maximum;
	real scale;
	real offset;
};

/* the effects processor (vtable 0x457140, instance 0x47f088; its methods
   are in src/unknown_2ae170.cpp) */
class c_sound_driver_effects
{
public:
	virtual void initialize(void *settings, void *buffers, bool rear) = 0;
	virtual void dispose() = 0;
	virtual void set_i3dl2(void *parameters, long unused) = 0;
};

struct s_sound_driver_globals
{
	bool unknown0000;
	bool surround;
	byte unknown0002[2];
	short channel_count;
	short voice_count;
	short unknown0008;
	byte unknown000a[2];
	s_sound_stream channels[k_sound_driver_channel_count];
	short unknown1a0c[3];
	short unknown1a12[3];
	s_sound_driver_voice voices[k_sound_driver_voice_count];
	point3f listener_position;
	vector3f listener_forward;
	vector3f listener_up;
	byte unknown293c[0x2950 - 0x293c];
	s_sound_driver_reverb reverbs[k_sound_driver_reverb_count];
	real reverb_scales[k_sound_driver_reverb_count];
	s_sound_driver_occlusion occlusions[k_sound_driver_reverb_count];
	bool reverb_dirty[k_sound_driver_reverb_count];
	bool occlusion_dirty[k_sound_driver_reverb_count];
	dword effect_levels_a[2];
	dword effect_levels_b[2];
	byte effect_settings[0x30];
	long effect_mixbins[3];
	LPDIRECTSOUNDBUFFER effect_buffers_a[3];
	LPDIRECTSOUNDBUFFER effect_buffers_b[3];
	LPDIRECTSOUNDBUFFER submix_buffers[8];
	LPDIRECTSOUNDBUFFER impulse_buffers[2];
	long impulse_ids[2];
	DSCAPS caps;
	LPDIRECTSOUND direct_sound;
	c_sound_driver_effects *effects;
	byte unknown2ab8[4];
	real volume_a;
	real volume_b;
	real volume_c;
	real volume_d;
	long unknown2acc;
	real unknown2ad0;
	dword unknown2ad4;
};

/* the effect parameters the driver sends to the effects processor */
struct s_sound_effect_parameters
{
	bool dirty;
	byte unknown01[3];
	long room;
	long room_hf;
	long direct;
	long direct_hf;
	real unknown70;
	long unknown74;
	real unknown78;
	long unknown7c;
	real unknown80;
	long unknown84;
	real unknown88;
};

extern s_sound_effect_parameters g_47005c;

struct s_bink_sound_settings;
extern s_bink_sound_settings *g_51ebe4;

#define SOUND_DRIVER_GLOBALS ((s_sound_driver_globals *)g_51ebe4)

#endif
