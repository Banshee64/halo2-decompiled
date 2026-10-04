// @flags /O2 /arch:SSE /Gr /GL-
/* NETWORK_VOICE_MAIL.CPP: whether a port's voice mail is playing or recording.
   Retail built it without LTCG: it keeps the standard convention, and its
   callers (0x2c93ca) treat edx as clobbered across the call. */

#include "cseries.h"
#include <xtl.h>
#include <xonline.h>
#include <xhv.h>
#include "network_voice.h"

static inline bool voice_available(void)
{
	return g_4c9878.initialized && g_476fc8.initialized;
}

// @retail 0x53970
bool __stdcall voice_mail_is_active(long port)
{
	bool result = false;
	if (voice_available())
		result = g_476fc8.voice_mail_active[port];
	return result;
}
