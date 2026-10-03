/* NETWORK_VOICE.H: the voice chat of the game (lane D): the XHV callback
   object (0x476fc8, vtable 0x45095c) and the voice globals (0x4c9878) */

#ifndef NETWORK_VOICE_H
#define NETWORK_VOICE_H

#include "cseries.h"
#include <xtl.h>
#include <xonline.h>
#include <xhv.h>
#include "loop_allocator.h"

/* the title's XHV callbacks and the engine they serve */
class c_voice_xhv : public ITitleXHV
{
public:
	STDMETHOD(LocalChatDataReady)(DWORD port, DWORD size, VOID *data);
	STDMETHOD(CommunicatorStatusUpdate)(DWORD port, XHV_VOICE_COMMUNICATOR_STATUS status);
	STDMETHOD(VoiceMailDataReady)(DWORD port, DWORD duration, DWORD size);
	STDMETHOD(VoiceMailStopped)(DWORD port);
	STDMETHOD(MicrophoneRawDataReady)(DWORD port, DWORD size, VOID *data, BOOL *voice_detected);

	bool initialized;
	byte unknown05[3];
	long mode;
	long unknown0c;
	XHVEngine *engine;
	XHV_VOICE_MASK masks[4];
	long port_modes[4];
	bool communicator_present[4];
	DWORD *voice_mail_sizes[4];
	DWORD *voice_mail_durations[4];
	bool voice_mail_active[4];
	bool chat_data_ready[4];
};

/* the voice globals */
struct s_voice_globals
{
	long unknown00;
	long unknown04;
	long unknown08;
	long unknown0c;
	long mode;
	long session_kind;
	long type;
	long pool_mode;
	long unknown20;
	long unknown24[0x30];
	s_loop_allocator *pool;
	s_loop_allocator *pool2;
	bool use_pool2;
	byte unknownED;
	word unknownEE;
	word unknownF0[16];
	word unknown110[16];
	long port_states[4];
	bool initialized;
	byte unknown141[3];
};

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

/* unknown_191270.cpp */
extern s_voice_effects *g_510c90;

extern c_voice_xhv g_476fc8;
extern s_voice_globals g_4c9878;
extern XHV_PROCESSING_MODE g_52731c[4];

#endif
